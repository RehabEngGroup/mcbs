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
        cout << "coeffDirectory:  directory with all the binary files with the coefficients";
        cout << "inputDirectory: angles.in and muscle.in a file with a list of the angle configurations";
        cout << "outputDirectory: directory with NodesData/ and BetweenNodesData/ directory\n";
        cout << "               , output of the generateLmtMaEvalGrids program.\n";
        exit(EXIT_FAILURE);
    }
    
    string coeffDir  = argv[1];
    string inputDir = argv[2]; 
    string outputDir = argv[3];
  
    //--------------> read the muscles
    string musclesFilename = inputDir + "/muscles.in"; 
    vector<string> musclesNames;
    readMuscles(musclesFilename, musclesNames);
      
    // and you create the splines
    SplineSet splineSet(coeffDir, musclesNames);
    
    
    //---->  read the dofs and the muscles connected
    vector< string > dofsNames;
    vector< vector< string > > musclesConnectedToDofs;
    string dofsFileName = inputDir + "/dofs.in";
    ifstream dofsFile(dofsFileName.c_str());
    if (!dofsFile.is_open()) {
      cout << "ERROR: " << dofsFileName << " input file could not be open\n";
      exit(EXIT_FAILURE);
    }
    
    string line;
    
    while(std::getline(dofsFile, line)) {
      std::istringstream iss(line);
      string token;
      iss >> token;
      dofsNames.push_back(token);
      
      vector< string > connectedMuscles;
      while (iss >> token) {
        connectedMuscles.push_back(token);  
      }
      musclesConnectedToDofs.push_back(connectedMuscles);
    }
    
    
    
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
    for (int i = 0; i < noCombinations; ++i) {
        vector<double> currentAngleCombination(dofsNames.size());
        for (int j=dofsNames.size()-1; j>=0; --j) {
            anglesFile >> currentAngleCombination.at(j);
            currentAngleCombination.at(j) = radians(currentAngleCombination.at(j));
        }  
        anglesCombinations.push_back(currentAngleCombination);
    }
    
    anglesFile.close();
    
  /*  for(int i = 0; i < dofsNames.size(); ++i) {
      cout << dofsNames.at(i) << ": ";
      for (int j = 0; j < musclesConnectedToDofs.at(i).size(); ++j)
        cout << musclesConnectedToDofs.at(i).at(j) << " ";
      cout << endl;
    }
    
   */

   
    splineSet.evalLmt(outputDir, anglesCombinations );
    splineSet.evalMa(outputDir, dofsNames, musclesConnectedToDofs, anglesCombinations  );
    
    exit(EXIT_SUCCESS);
} 
     
       

  