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
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <fstream>
using std::ofstream;
using std::ifstream;
#include <stdlib.h>

#include "LmtMaFromOpenSim.h"
#include "DofCfg.h" 


/*! Read the muscle names from the provided file
    \param [in] musclesFilename list the name of the muscles we are interested in
                the file format is just the list of muscle names (one for each row)
                Do not use OpenSim muscle names. Use the name required by your applications
    \param [out] musclesNames a vector with the name of the muscles, ready to be used
                 in our applications
 
 */
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


/*!
    This program generates the lmt.in file required for the computation of Spline coefficients
    @param argv[1] is the input directory. This directory MUST include three files: model.osim
           is the opensim model used to compute lmt for the muscles listed in the file muscles.in
           at the dof configuration defined through the file dofAngles.cfg 
    @param argv[2] is the output directory. The program will create the file lmt.in (see documentation for the file format)
    @author M. Reggiani 
 */

int main(int argc, const char* argv[]) {
    // Check command line arguments
    if ( argc != 3 ) {
        cout << "Usage: generateLmtGridWithOpenSim inputDirectory outputDirectory\n"; 
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
    
    
    //-- setup the grid generator
    string osimModelFilename = inputDir + "/model.osim";
    LmtMaFromOpenSim lmtGrid(osimModelFilename,cfg.getDofNames(), musclesNames, cfg.getAngleCombinations());
    
    //-- create the input files for spline 
    string lmtFilename = outputDir + "/lmt.in";
    ofstream lmtFile(lmtFilename.c_str());
    if (!lmtFile.is_open()) {
        cout << "ERROR: lmt.in output file could not be open\n";
        exit(EXIT_FAILURE);
    }
    lmtFile << cfg; 
    lmtGrid.saveLmt(lmtFile);
    lmtFile.close();
    
    exit(EXIT_SUCCESS);
  
}