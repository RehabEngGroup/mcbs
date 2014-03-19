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

#include <OpenSim/Common/Storage.h>

#include "LmtMaFromOpenSim.h"
#include "DofCfg.h" 


void readMuscles(const string& musclesFilename, vector<string>& musclesNames) {
    ifstream musclesFile(musclesFilename.c_str());
    if (!musclesFile) {
        cout << "Error: file " << musclesFilename << " could not be opened\n";
        exit(EXIT_FAILURE);
    }
    string nextMuscle;
    musclesFile >> nextMuscle;
    while (!musclesFile.eof()) {
        musclesNames.push_back(nextMuscle);
        musclesFile >> nextMuscle;
    } 
    
    musclesFile.close();
}


void printAnglesCombinations(ostream &os, vector< vector<double> > combinations) {
    int nRows = combinations.at(0).size();
    int nCols = combinations.size();
    os << nRows << endl;
    for (int iRow = 0; iRow < nRows; ++iRow) {
        for (int iCol = 0; iCol < nCols; ++iCol) 
           os << combinations.at(iCol).at(iRow) << " ";
        os << endl;
    }    
}
 
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
    readMuscles(musclesFilename, musclesNames);
    
    //-- create the angles combinations
    string configFilename = inputDir + "/dofAngles.cfg";
    DofCfg cfg(configFilename);
    
    //-- create the eval files for spline (compute the lmt and ma with opensim) 
    //   to be compared with the output of spline software
    
    // NodesData: angles.in
    string anglesFileName = outputDir + "/NodesData/angles.in";
    ofstream anglesFile(anglesFileName.c_str());
    if (!anglesFile.is_open()) {
        cout << "ERROR: NodesData/angles.in output file could not be open\n";
        exit(EXIT_FAILURE);
    }
    printAnglesCombinations(anglesFile, cfg.getAngleCombinations());
    anglesFile.close();
    
    // NodesData: lmt.in 
   
    string osimModelFilename = inputDir + "/model.osim";
    LmtMaFromOpenSim lmtGridNodesData(osimModelFilename, cfg.getDofNames(), musclesNames, cfg.getAngleCombinations());
    
    string lmtFilename = outputDir + "/NodesData/lmt.in";
    ofstream lmtFile(lmtFilename.c_str());
    if (!lmtFile.is_open()) {
        cout << "ERROR: NodesData/lmt.in output file could not be open\n";
        exit(EXIT_FAILURE);
    }
    lmtFile << cfg.getNoAngleCombinations() << endl;
    lmtGridNodesData.saveLmt(lmtFile);
    
    lmtGridNodesData.saveMa(outputDir + "/NodesData/");
    
    lmtFile.close();
    
    // BetweeenNodesData: angles.in
    anglesFileName = outputDir + "/BetweenNodesData/angles.in";
    anglesFile.open(anglesFileName.c_str());
    if (!anglesFile.is_open()) {
        cout << "ERROR: BetweenNodesData/angles.in output file could not be open\n";
        exit(EXIT_FAILURE);
    }
    printAnglesCombinations(anglesFile, cfg.getBetweenAngleCombinations());
    anglesFile.close();
    
    // NodesData: lmt.in 
    LmtMaFromOpenSim lmtGridBetweenNodesData(osimModelFilename, cfg.getDofNames(), musclesNames, cfg.getBetweenAngleCombinations());
    
    lmtFilename = outputDir + "/BetweenNodesData/lmt.in";
    lmtFile.open(lmtFilename.c_str());
    if (!lmtFile.is_open()) {
        cout << "ERROR: NodesData/lmt.in output file could not be open\n";
        exit(EXIT_FAILURE);
    }
    lmtFile << cfg.getNoBetweenAngleCombinations() << endl;
    lmtGridBetweenNodesData.saveLmt(lmtFile);
    
    
    // NodesData: ma.in 
    lmtGridBetweenNodesData.saveMa(outputDir + "/BetweenNodesData/");
    lmtFile.close();
    
     
    
    
    
    
    exit(EXIT_SUCCESS);
  
}