/*
 * Copyright (C) 2013 C. Pizzolato, M. Reggiani, M. Sartori
 *
 * This file is part of Multidimensional Cubic Bspline Library (MCBS).
 *
 * MCBS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MCBS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MCBS.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "LmtMaFromOpenSim.h"

#include <vector>
using std::vector;
#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::endl;
#include <fstream>
using std::ofstream;
#include <cstdlib>
#include <fstream>
using std::ostream;
#include <OpenSim/OpenSim.h>
#include <memory>

namespace mcbs{
    const int NUMBER_DIGIT_OUTPUT = 15;

    LmtMaFromOpenSim::LmtMaFromOpenSim(){
        cout << "LmtMaFromOpenSim default constructor: you should not be here!";
        exit(EXIT_FAILURE);
    }

    LmtMaFromOpenSim::LmtMaFromOpenSim(const string& osimModelFilename, const vector<string>& dofNames, const vector<string>& musclesNames, const vector< vector<double> >& angleCombinations, bool computeMoments) : computeMoments_(computeMoments) {
        osimModel_ = new OpenSim::Model(osimModelFilename);
        computeLmtMaFromOpenSim(dofNames, musclesNames, angleCombinations);
    }


    LmtMaFromOpenSim::~LmtMaFromOpenSim(){

    }

    LmtMaFromOpenSim::LmtMaFromOpenSim(const LmtMaFromOpenSim& orig) {
        // *this = orig; //not sure about correctness of copy constructors in OpenSim
        cout << "LmtMaFromOpenSim: copy constructor: Should not be here!!!\n";
        exit(EXIT_FAILURE);
    }

    void LmtMaFromOpenSim::computeLmtMaFromOpenSim(const vector<string>& dofNames, const vector<string>& musclesNames, const vector< vector<double> >& angleCombinations) {
        // clear data from previous analysis
        lmtData_.clear();
        maData_.clear();
        nDofs_ = dofNames.size();
        osimDofNames_=dofNames;
        osimMusclesNames_=musclesNames;
        lmtData_.resize(musclesNames.size());
        maData_.resize(dofNames.size());
        for (int i=0; i<maData_.size(); ++i)
            maData_.at(i).resize(musclesNames.size());
        SimTK::State& si = osimModel_->initSystem();
        osimModel_->getMultibodySystem().realize(si, SimTK::Stage::Position);


        int nRows = angleCombinations.size();
        for(int iRow = 0; iRow < nRows; ++iRow) {
            for(int iCol = 0; iCol < angleCombinations.at(iRow).size(); ++iCol) {
                try {// in case the coordinate name is not found in the input model
                    osimModel_->updCoordinateSet().get(osimDofNames_.at(iCol)).setValue(si, angleCombinations.at(iRow).at(iCol));
                }
                catch (const OpenSim::Exception& x) {
                    x.print(cout);
                }
            }
            osimModel_->getMultibodySystem().realize(si, SimTK::Stage::Position);
            for (int musc=0; musc<osimMusclesNames_.size(); ++musc){
                lmtData_[musc].push_back(osimModel_->updMuscles().get(osimMusclesNames_[musc]).getLength(si));
                for (int dof=0; dof<osimDofNames_.size(); ++dof)
                    maData_.at(dof).at(musc).push_back(osimModel_->getMuscles().get(osimMusclesNames_[musc]).computeMomentArm(si, osimModel_->updCoordinateSet().get(osimDofNames_[dof])));
            }
        }
    }

    void LmtMaFromOpenSim::saveLmt(ostream &os) {
        for (size_t i = 0; i < osimMusclesNames_.size() - 1; ++i)
            os << osimMusclesNames_.at(i) << "\t";
        // the last one is printed outside the for cycle because we do not want " " at the end of the line
        os << osimMusclesNames_.back() << endl;
        size_t nRows = lmtData_.at(1).size();
        size_t nCols = lmtData_.size();
        for (size_t iRow = 0; iRow < nRows; ++iRow) {
            for (size_t iCol = 0; iCol < nCols; ++iCol)
                os << lmtData_.at(iCol).at(iRow) << "\t";
            os << endl;
        }
    }

    void LmtMaFromOpenSim::saveMa(const string& os, const std::map< string, vector< string > >& musclesConnectedToDofs) {
        cout << "saving MA" << endl;
        for (size_t coordinateIterator = 0; coordinateIterator < osimDofNames_.size(); ++coordinateIterator) {
            // open next ma file
            string maFileName = os + "ma_" + osimDofNames_.at(coordinateIterator) + ".out";
            cout << maFileName << endl;
            ofstream maFile(maFileName.c_str());
            const vector<string>& musclesToConsider = musclesConnectedToDofs.at(osimDofNames_.at(coordinateIterator));
            for (size_t i = 0; i < musclesToConsider.size() - 1; ++i)
                maFile << musclesToConsider.at(i) << "\t";
            // the last one is printed outside the for cycle because we do not want "\t" at the end of the line
            maFile << musclesToConsider.back() << endl;
            size_t nRows = maData_.at(coordinateIterator).at(1).size();
            //find the columns (muscles) we are interested in for this dof (coordinate)
            vector<int> muscleIndices(musclesToConsider.size());
            for (size_t i = 0; i < musclesToConsider.size(); ++i)
            {
                vector<string>::const_iterator foundMuscle = std::find(osimMusclesNames_.begin(), osimMusclesNames_.end(), musclesToConsider.at(i));
                if (foundMuscle != osimMusclesNames_.end())
                    muscleIndices[i] = foundMuscle - osimMusclesNames_.begin();
                else
                {
                    cout << "ERROR while checking muscle names to write to output for each dof" << endl;
                    exit(EXIT_FAILURE);
                }
            }
            for (size_t iRow = 0; iRow < nRows; ++iRow) {
                for (size_t iCol = 0; iCol < muscleIndices.size(); ++iCol)
                    maFile << maData_.at(coordinateIterator).at(muscleIndices[iCol]).at(iRow) << "\t";
                maFile << endl;
            }

            maFile.close();
        }
    }
}
