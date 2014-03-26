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
#include<sstream>
#include <stdlib.h>
#include "SplineSet.h"
#include <math.h>
#include "ReadMuscles.h"
#include <map>
using std::map;


inline double radians (double d) {
return d * M_PI / 180;
}

int main(int argc, const char* argv[]) {
  
    //:TODO: Please modify the input and output description... this for all the programs
    cout << "------------------------------------------------\n";
    cout << " Eval Spline from Coefficients\n";
    cout << "------------------------------------------------\n";
    
    // Check command line arguments
    if ( argc != 4 ) {
        cout << "Usage: generateLmtMaWithSplineCoefficients coeffDirectory inputDirectory outputDirectory\n";
        cout << "coeffDirectory:  directory with all the binary files with the coefficients\n";
        cout << "inputDirectory:  directory containing angles.in (list of the angle configurations), muscles.in (list of muscles) and dof.in(list of muscles to consider for each coordinate) files \n";
        cout << "outputDirectory: directory where lmt and moment arms .out files are saved\n";
        exit(EXIT_FAILURE);
    }
    
    string coeffDir  = argv[1];
    string inputDir = argv[2]; 
    string outputDir = argv[3];
  
    //--------------> read the muscles
    string musclesFilename = inputDir + "/muscles.in"; 
    vector<string> musclesNames;
    map<string, vector<string> >musclesConnectedToDofs;
    readMuscles(musclesFilename, musclesNames, musclesConnectedToDofs);
      
    // and you create the splines
    SplineSet splineSet(coeffDir, musclesNames);
    
    

    //------>  read the angles
    string anglesFileName = inputDir + "/angles.in";
    ifstream anglesFile(anglesFileName.c_str());
    if (!anglesFile.is_open()) {
        cout << "ERROR: " << anglesFileName << " input file could not be open\n";
        exit(EXIT_FAILURE);
    }
    
    int noCombinations;
    anglesFile >> noCombinations;
    vector< vector< double > > anglesCombinations; 
    size_t dofsSize=musclesConnectedToDofs.size();
    for (int i = 0; i < noCombinations; ++i) {
        vector<double> currentAngleCombination(dofsSize);
        for (int j=dofsSize-1; j>=0; --j) {
            anglesFile >> currentAngleCombination.at(j);
            currentAngleCombination.at(j) = radians(currentAngleCombination.at(j));
        }  
        anglesCombinations.push_back(currentAngleCombination);
    }
    
    anglesFile.close();

   
    splineSet.evalLmt(outputDir, anglesCombinations );
    splineSet.evalMa(outputDir, musclesConnectedToDofs, anglesCombinations  );
    
    exit(EXIT_SUCCESS);
} 
     
       

  