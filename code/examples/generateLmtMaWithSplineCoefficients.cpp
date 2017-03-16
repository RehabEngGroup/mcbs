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

#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <sstream>
//#include <cstdlib>
#include <map>
using std::map;
#include <algorithm>

#include "mcbs/SplineSet.h"
#include "mcbs/SplineSetCreator.h"
#include "mcbs/MusclesCfg.h"
using namespace mcbs;

int main(int argc, const char* argv[]) {

    //:TODO: Please modify the input and output description... this for all the programs
    cout << "------------------------------------------------\n";
    cout << " Eval Spline from Coefficients\n";
    cout << "------------------------------------------------\n";

    // Check command line arguments
    if ( argc != 4 ) {
        cout << "Usage: generateLmtMaWithSplineCoefficients coeffDirectory inputDirectory outputDirectory\n";
        cout << "coeffDirectory:  directory with all the binary files with the coefficients\n";
        cout << "inputDirectory:  directory containing angles.in (list of the angle configurations) and muscles.in (list of muscles to consider for each coordinate) files \n";
        cout << "outputDirectory: directory where lmt and moment arms .out files are saved\n";
        exit(EXIT_FAILURE);
    }

    string coeffDir  = argv[1];
    string inputDir = argv[2];
    string outputDir = argv[3];

    //--------------> read the muscles
    string musclesFilename = inputDir + "/muscles.in";
    MusclesCfg muscCfg(musclesFilename);
    const vector<string> &musclesNames=muscCfg.getMuscleNames();
    const map<string, vector<string> > &musclesConnectedToDofs=muscCfg.getMusclesPerDof();

    // and you create the splines
    std::shared_ptr<SplineSetBase> splineSet = SplineSetCreator::makeSplineSet(coeffDir, musclesNames);



    //------>  read the angles
    string anglesFileName = inputDir + "/angles.in";
    ifstream anglesFile(anglesFileName.c_str());
    if (!anglesFile.is_open()) {
        cout << "ERROR: " << anglesFileName << " input file could not be open\n";
        exit(EXIT_FAILURE);
    }

    vector< string > dofsNames;
    string line;
    std::getline(anglesFile, line);
    std::istringstream iss(line);
    string token;
    //read dof names
    while(iss >> token)
       dofsNames.push_back(token);

    vector< string > splineDofsNames=splineSet->getDofNames();
    for (auto name:splineDofsNames)
        std::cout << name << "  ";
    std::cout << std::endl;
    //compare dof names provided in angles.in with those used for generate the splines (we must provide our "test" angles
    //with the same order)
    vector <int> dofOrder;
    for( vector <string>::iterator dofIt=dofsNames.begin(); dofIt!=dofsNames.end(); ++dofIt)
    {
        vector <string>::iterator foundDof=std::find(splineDofsNames.begin(), splineDofsNames.end(), *dofIt );
        if (foundDof!=splineDofsNames.end())
            dofOrder.push_back( foundDof -splineDofsNames.begin() );
        else
        {
            cout << "ERROR: " << *dofIt << " degree of freedom provided in angles.in file was not found in spline degrees of freedom\n";
            exit(EXIT_FAILURE);
        }
    }

    //read angle combinations
    vector< vector< double > > anglesCombinations;
    double nextValue;
    while(anglesFile.good()) {
        bool stillGood=true;
        vector<double> currentAngleCombination(dofsNames.size());
        for (int j=0; j<dofsNames.size(); ++j) {

            if (anglesFile>>nextValue)
                currentAngleCombination.at(dofOrder.at(j)) = nextValue;
            else stillGood=false;
        }
        if (stillGood)
            anglesCombinations.push_back(currentAngleCombination);
    }
    anglesFile.close();


    splineSet->evalLmt(outputDir, anglesCombinations );
    splineSet->evalMa(outputDir, musclesConnectedToDofs, anglesCombinations  );
    splineSet->evalMaDerivative(outputDir, musclesConnectedToDofs, anglesCombinations);
    exit(EXIT_SUCCESS);
}

