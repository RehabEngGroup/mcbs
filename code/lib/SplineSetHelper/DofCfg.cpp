/*
 * Copyright (C) 2013 C. Pizzolato, M. Reggiani, M. Sartori
 * Copyright (C) 2015 E.Ceseracciu
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
using std::ostream;
#include <fstream>
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <cstdlib>

#include <cmath>

#include "mcbs/DofCfg.h"

namespace mcbs {
    DofCfg::DofCfg() {

        cout << "DofCfg default constructor: aborting... you cannot create the dof configuration without an input file!\n";
        exit(EXIT_FAILURE);

    }

    /*! Creates the required dof configuration from the values provided with the cfgFilename
        This is the only constructor that should be used. We suppose that you have a file that provides
        information about the required grids of dof configurations.

        It is not difficult to develop other constructor if required. You should only be aware that
        the software assumes that the DofCfg constructors compute the grids of
        "Nodes" and "BetweenNodes" configurations.

        */

    DofCfg::DofCfg(string cfgFilename){

        std::ifstream  cfgFile(cfgFilename.c_str());
        if (!cfgFile.is_open()) {
            cout << "ERROR: configuration file could not be open\n";
            exit(EXIT_FAILURE);
        }

        string trash;
        cfgFile >> trash;

        int noDofs;
        cfgFile >> noDofs;

        for (; noDofs > 0; --noDofs){ //TODO: check that cfgFile still has content at each cycle!
            string dofName;    cfgFile >> dofName;      dofNames_.push_back(dofName);
            double startAngle; cfgFile >> startAngle;   anglesStartValue_.push_back(startAngle);
            double stopAngle;  cfgFile >> stopAngle;    anglesStopValue_.push_back(stopAngle);
            int noIntervals;   cfgFile >> noIntervals;  noIntervals_.push_back(noIntervals);
        }

        cfgFile.close();

        if (!isValid()) {
            cout << "LmtSplineGenerator: ERROR configuration file\n";
            exit(EXIT_FAILURE);
        }

        nCols_ = dofNames_.size();
        nRows_ = 1;
        for (unsigned int i = 0; i < dofNames_.size(); ++i)
            nRows_ *= (noIntervals_.at(i) + 1);
        for (int i = 0; i < nRows_; ++i)
            anglesCombinations_.push_back(vector<double>(nCols_));

        multeplicity_ = 1;
        generateAnglesCombinations();
        generateBetweenAnglesCombinations();

    }

    //! Creates the grid of dof configurations that are used to compute splines coefficients. Each configuration is a "Node"

    void DofCfg::generateAnglesCombinations() {
        createCombinations(anglesStartValue_, anglesStopValue_, noIntervals_, anglesCombinations_, 0);
    }

    /*! Creates the grid of dof configurations that are between the "Nodes" (dof configurations used to computed splines coefficients).
        This configurations should be the most difficult to correctly interpolates as they are the one with the largest distances
        from "Nodes"
        */
    void DofCfg::generateBetweenAnglesCombinations() {
        vector<int> noIntervals;
        for (int i = 0; i < nCols_; ++i)
            noIntervals.push_back(noIntervals_.at(i) - 1);
        vector<double> startValues;
        for (int i = 0; i < nCols_; ++i)
            startValues.push_back(anglesStartValue_.at(i) + (anglesStopValue_.at(i) - anglesStartValue_.at(i)) / (noIntervals_.at(i)));
        vector<double> stopValues;
        for (int i = 0; i < nCols_; ++i)
            stopValues.push_back(anglesStopValue_.at(i) - (anglesStopValue_.at(i) - anglesStartValue_.at(i)) / (noIntervals_.at(i)));


        int nRows = 1;
        for (unsigned int i = 0; i < dofNames_.size(); ++i)
            nRows *= (noIntervals.at(i) + 1);
        for (int i = 0; i < nRows_; ++i)
            betweenAnglesCombinations_.push_back(vector<double>(nCols_));

        multeplicity_ = 1;
        createCombinations(startValues, stopValues, noIntervals, betweenAnglesCombinations_, 0);
    }

    /*! Creates a grid of dof configuration
        Through recursion is compute all the possible angle combinations for the required dof
        \param [in] startValues specifies the initial angle for each dof
        \param [in] stopValues specifies the final angle for each dof
        \param [in] noIntervals specifies the number of intervals for each dof
        \param [out] combinations the output matrix with the sequence of dof configurations
        \param [in] pos step in the recursion algorithm. The number of steps is the value of dof
        \sa generateBetweenAnglesCombinations()  generateAnglesCombinations()
        */
    void DofCfg::createCombinations(const vector<double>& startValues, const vector<double>& stopValues, const vector<int>& noIntervals, vector< vector<double> >& combinations, int pos) {
        double increment = (stopValues.at(pos) - startValues.at(pos)) / (noIntervals.at(pos));
        int rowCounter = 0;

        int nRows = 1;
        for (unsigned int i = 0; i < dofNames_.size(); ++i)
            nRows *= (noIntervals.at(i) + 1);

        do {
            for (int i = 0; i < noIntervals.at(pos) + 1; ++i)
                for (int j = 0; j < multeplicity_; ++j) {
                    combinations.at(rowCounter).at(pos) = startValues.at(pos) + i*increment;
                    ++rowCounter;
                }
        } while (rowCounter < nRows);
        if (pos < (nCols_ - 1)) {
            multeplicity_ *= (noIntervals.at(pos) + 1);
            createCombinations(startValues, stopValues, noIntervals, combinations, pos + 1);
        }
    }

    ostream & operator<<(ostream &os, const DofCfg& dofcfg) {

        os << "ndof " << dofcfg.dofNames_.size() << endl;
        for (unsigned int i = 0; i < dofcfg.dofNames_.size(); ++i)
            os << dofcfg.dofNames_.at(i) << " " << dofcfg.anglesStartValue_.at(i) << " " << dofcfg.anglesStopValue_.at(i) << " " << dofcfg.noIntervals_.at(i) << endl;
        return os;
    }
}
