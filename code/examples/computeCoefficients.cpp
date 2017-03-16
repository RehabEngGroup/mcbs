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
#include <cstdlib>

#include "mcbs/SplineSet.h"
#include "mcbs/SplineSetCreator.h"
using namespace mcbs;


int main(int argc, const char* argv[]) {
    cout << "------------------------------------------------\n";
    cout << " Compute Spline Coefficients\n";
    cout << "------------------------------------------------\n";

   // Check command line arguments
    if ( argc != 3 ) {
        cout << "Usage: computeCoefficients inputDirectory outputDirectory\n";
        cout << "inputDirectory:  directory with lmt.in and dofAngles.cfg";
        cout << "outputDirectory: directory where to store binary files with\n";
        cout << "                 the coefficients\n";
        exit(EXIT_FAILURE);
    }

    string inputDir  = argv[1];
    string outputDir = argv[2];

    string inputDataFilename = inputDir + "/lmt.in";


    ifstream lmtDataFile(inputDataFilename.c_str());

    if (!lmtDataFile.is_open()) {
        cout << "ERROR: " << inputDataFilename << " could not be open\n";
        exit(EXIT_FAILURE);
    }
    std::string dummy;
    int nDofs;
    lmtDataFile >> dummy; // TODO check that dummy is "ndof"
    lmtDataFile >> nDofs;

    std::shared_ptr<SplineSetBase> splineSet = SplineSetCreator::makeSplineSet(nDofs, inputDataFilename);
    splineSet->exportCoefficients(outputDir);


    string dofAnglesFilename = outputDir + "/dofAngles.cfg";

    ofstream dofAnglesFile(dofAnglesFilename.c_str());
    if (!dofAnglesFile.is_open()) {
        cout << "ERROR: " << dofAnglesFilename << " output file could not be open\n";
        exit(EXIT_FAILURE);
    }

    dofAnglesFile << "ndof " << nDofs << endl;
      // --- Read DOFs
    for (int i=0; i < nDofs; ++i) {
        string dofName;
        double start, stop, numIntervals;
        lmtDataFile >> dofName;
        lmtDataFile >> start;
        lmtDataFile >> stop;
        lmtDataFile >> numIntervals;
        dofAnglesFile << dofName << "\t" << start << "\t" << stop << "\t" << numIntervals << endl;
    }

    dofAnglesFile.close();
    lmtDataFile.close();
    exit(EXIT_SUCCESS);
}

