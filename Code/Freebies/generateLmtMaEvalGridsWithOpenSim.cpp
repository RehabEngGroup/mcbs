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
using std::ostream;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <fstream>
using std::ofstream;
using std::ifstream;
#include <stdlib.h>
#include <map>
using std::map;

#include "LmtMaFromOpenSim.h"
#include "DofCfg.h"
#include "ReadMuscles.h"



 
// muscles.in (list of the muscles we are interested in
// model.osim (model for opensim)
// angles

int main(int argc, const char* argv[]) {
    // Check command line arguments
    if ( argc != 3 ) {
        cout << "Usage: testSpline inputDirectory outputDirectory\n"; 
        exit(EXIT_FAILURE);
    }
  
    string inputDir = argv[1]; 
    string outputDir = argv[2];
    
    //-- read the muscles we want in the grid
    string musclesFilename = inputDir + "/muscles.in"; 
    vector<string> musclesNames;
    map<string, vector<string> > musclesConnectedToDofs;
    readMuscles(musclesFilename, musclesNames, musclesConnectedToDofs);

    
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

    //read angle combinations
    vector< vector< double > > anglesCombinations;
    double nextValue;
    while(anglesFile.good()) {
        bool stillGood=true;
        vector<double> currentAngleCombination(dofsNames.size());
        for (int j=0; j<dofsNames.size(); ++j) {

            if (anglesFile>>nextValue)
                currentAngleCombination.at(j) = nextValue;
            else stillGood=false;
        }
        if (stillGood)
            anglesCombinations.push_back(currentAngleCombination);
    }
    anglesFile.close();

    string osimModelFilename = inputDir + "/model.osim";
    LmtMaFromOpenSim lmtGridNodesData(osimModelFilename, dofsNames, musclesNames, anglesCombinations, true);
    
    string lmtFilename = outputDir + "lmt.out";
    ofstream lmtFile(lmtFilename.c_str());
    if (!lmtFile.is_open()) {
        cout << "ERROR: lmt.out output file could not be open\n";
        exit(EXIT_FAILURE);
    }
    lmtGridNodesData.saveLmt(lmtFile);
    lmtGridNodesData.saveMa(outputDir, musclesConnectedToDofs);
    lmtFile.close();

    exit(EXIT_SUCCESS);
  
}
