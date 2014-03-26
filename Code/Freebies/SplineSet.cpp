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


#include "SplineSet.h"
#include "Spline.cpp"

#include <stdlib.h>
#include <iostream>
using std::cout; 
#include <fstream>
using std::ifstream;
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <sstream>
using std::stringstream;
#include <algorithm>
using std::find;
#include <iostream>
using std::endl;
#include <iomanip>
#include <map>
using std::map;
#include <fstream>
using std::ifstream;
using std::ofstream;

inline double radians (double d) {
return d * M_PI / 180;
}

inline double degrees (double r) {
return r * 180/ M_PI;
}

SplineSet::SplineSet() {
    cout << "SplineSet constructor: You should not be here... sorry about that\n";
    exit(EXIT_FAILURE);
}

SplineSet::SplineSet(const string& inputDataFilename)
:dofNames_(N_DOF),  a_(N_DOF), b_(N_DOF), n_(N_DOF), inputDataFile_(inputDataFilename.c_str()) {

  if (!inputDataFile_.is_open()) {
    cout << "ERROR: " << inputDataFilename << " could not be open\n";
    exit(EXIT_FAILURE);
  }

#ifdef LOG
  cout << "Reading input data from: " 
       << inputDataFilename << endl;
#endif

  readInputData();  
  inputDataFile_.close();

#ifdef LOG
  cout << "Read the following interpolation data:\n";
  displayInputData(); 
#endif

  // create the noMuscles_ splines
  for (int i = 0; i < noMuscles_; ++i) {
     Spline<N_DOF> newSpline(a_,b_,n_);
     splines_.push_back(newSpline);
  }

#ifdef LOG
  cout << "Created " << splines_.size() << " splines.\n";
#endif     

  // now compute coefficients for each muscle
  for (int i = 0; i < noMuscles_; ++i) {
    vector<double> currentMuscle(y_[i]);
    splines_[i].computeCoefficients(currentMuscle, currentMuscle.begin());
  }

}


void SplineSet::readInputData() {
 
  // --- Read DOFs 
  for (int i=0; i < N_DOF; ++i) {
    inputDataFile_ >> dofNames_[i];
    inputDataFile_ >> a_[i]; a_[i] = radians(a_[i]);
    inputDataFile_ >> b_[i]; b_[i] = radians(b_[i]);
    inputDataFile_ >> n_[i]; 
  }
 
  string line;
  getline(inputDataFile_, line, '\n'); getline(inputDataFile_, line, '\n');  
  stringstream myStream(line);
  string nextMuscleName;
  // --- Read Interpolation Data
  // 1. first their names
  do {
    myStream >> nextMuscleName;
    muscleNames_.push_back(nextMuscleName); 
  } while (!myStream.eof());

  noMuscles_ = muscleNames_.size();
  
  // 2. then their values for all the possible combination of DOFs values
  // 2a. create the matrix to store them
  noInputData_ = 1;
  for (int i = 0; i < N_DOF; ++i) 
    noInputData_ *= ( n_[i]+1 );
  for (int i = 0; i < noMuscles_; ++i)
    y_.push_back(vector<double>(noInputData_));
    
  // 2b. read the data 
  for (int j=0; j < noInputData_; ++j) 
    for (int i=0; i < noMuscles_; ++i) {
      inputDataFile_ >> y_[i][j];
    }   
    
}


SplineSet::SplineSet(const string& coeffDir, const vector<string>& muscleNames)
:dofNames_(N_DOF),  a_(N_DOF), b_(N_DOF), n_(N_DOF), muscleNames_(muscleNames) { 
    string anglesFilename = coeffDir + "/dofAngles.cfg";
    ifstream anglesFile(anglesFilename.c_str());
    if (!anglesFile.is_open()) {
        cout << "ERROR: " << anglesFilename << " could not be open\n";
        exit(EXIT_FAILURE);
    }

    string trash;
    anglesFile >> trash;
    
    int noDofs;
    anglesFile >> noDofs;

        
    for (int i=0; i < N_DOF; ++i) {
        anglesFile >> dofNames_[i];
        anglesFile >> a_[i]; a_[i] = radians(a_[i]);
        anglesFile >> b_[i]; b_[i] = radians(b_[i]);
        anglesFile >> n_[i]; 
    }
  
    anglesFile.close();

#ifdef LOG
    cout << "Created " << splines_.size() << " splines.\n";
#endif     

    noMuscles_ = muscleNames_.size();
    // create the noMuscles_ splines
    for (int i = 0; i < noMuscles_; ++i) {
        Spline<N_DOF> newSpline(a_,b_,n_);
        splines_.push_back(newSpline);
    }

#ifdef LOG
    cout << "Created " << splines_.size() << " splines.\n";
#endif     
 
    
#ifdef LOG
    cout << "Set up spline with coefficients:\n"; 
#endif
    for (int i = 0; i < noMuscles_; ++i) {
        vector<double> coefficients;
        getCoefficientsFromFile(coefficients, coeffDir, muscleNames_.at(i));
        splines_.at(i).setCoefficients(coefficients);
    }    
}


void SplineSet::getCoefficientsFromFile(vector<double>& coefficientsFromFile, const string& coeffDir, const string& muscleName) {

    string coeffFilename = coeffDir + "/" + muscleName +".bin";
    std::fstream inputCoeffFile(coeffFilename.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
    if (!inputCoeffFile.is_open()) {
        cout << "ERROR: " << coeffFilename << " could not be open\n";
        exit(EXIT_FAILURE);
    }
    
    cout << "Using " << muscleName << ".bin as coefficients file\n";
    std::ifstream::pos_type dataSize;
    dataSize = inputCoeffFile.tellg();
    char* dataFromFile;
    dataFromFile = new char [dataSize];
    inputCoeffFile.seekg (0, std::ios::beg);
    inputCoeffFile.read (dataFromFile, dataSize);
    inputCoeffFile.close();
    double* ptr = (double*)dataFromFile;
    do{
        coefficientsFromFile.push_back(*(ptr++));
    } while (ptr < (double*)(dataFromFile + dataSize));
    delete[] dataFromFile;
}


SplineSet::SplineSet(const SplineSet& orig) {
    cout << "SplineSet copy constructor: You should not be here... sorry about that\n";
    exit(EXIT_FAILURE);
}



void SplineSet::exportCoefficients(const string& outDir){
    
    for(unsigned int i = 0; i < muscleNames_.size(); ++i) {
        string outputFilename = outDir + muscleNames_.at(i) + ".bin";
        std::fstream outputCoeffFile(outputFilename.c_str(), std::ios::out | std::ios::binary);
        if (!outputCoeffFile.is_open()) {
            cout << "ERROR: "+ outputFilename +"File could not be open\n";
            exit(EXIT_FAILURE);
        }
        vector<double> coefficients = splines_.at(i).getCoefficients();
        for (unsigned int j = 0; j < coefficients.size(); ++j) {
            double c = coefficients.at(j);
            outputCoeffFile.write( (const char*) &c, sizeof(c) );
        }
        outputCoeffFile.close();
    }
}

/** 
 * return the lmtValues for the set of muscles when 
 * a set of dof values are received in input
 */
void SplineSet::getLmt(vector<double>& lmtValues, const vector<double>& dofValues) {
    lmtValues.clear();
    for (unsigned int currentMuscle=0; currentMuscle < muscleNames_.size(); ++currentMuscle)
        lmtValues.push_back(splines_.at(currentMuscle).getValue(dofValues));
}

/**
 * store in files the values of Ma
 */


inline double roundIt(double x, double n ) {
  return floor( x * pow(10.0, n) + 0.5) / pow(10.0, n);
}
 
void SplineSet::evalLmt( const string& outputDir, const vector< vector< double > >& angleCombinations ) {
  
    string lmtFileName = outputDir + "/lmt.out";
    ofstream lmtFile(lmtFileName.c_str());
    if (!lmtFile.is_open()) {
        cout << "ERROR: " << lmtFileName << " output file could not be open\n";
        exit(EXIT_FAILURE);
    }
    
    int noCombinations = angleCombinations.size();  
    for (int i = 0; i < noCombinations; ++i) { 
        vector<double> lmtValues;
        for (unsigned int currentMuscle=0; currentMuscle < muscleNames_.size(); ++currentMuscle)
           lmtValues.push_back(splines_.at(currentMuscle).getValue(angleCombinations.at(i)));
           for (unsigned int j = 0; j < muscleNames_.size(); ++j)
              lmtFile << lmtValues.at(j) << " ";
        lmtFile << endl;
    }
    lmtFile.close();
}


void SplineSet::getMa( const string& dofName, 
                       const vector< string >& musclesConnectedToDofs, 
                       const vector< double >& angleCombinations,
                       vector <double>& maValues  ) {

  bool found = false; 
  int k = 0;
  int dofIndex; 
  while ((!found) && (k < dofNames_.size()) ) {
    if (dofName == dofNames_.at(k)) {
      dofIndex = k;
      found = true; 
    }
    k++;
  }
  
  if (!found) {
    cout << "ERROR: " << dofName << " not found\n";
    exit(EXIT_FAILURE);
  }
   
 
  for (unsigned int i = 0; i < musclesConnectedToDofs.size(); ++i) {   
    vector<string>::iterator iter = find(muscleNames_.begin(), muscleNames_.end(), musclesConnectedToDofs.at(i));
    if (iter == muscleNames_.end()) {
      cout << "something went wrong.. muscle not found in Splines \n";
      exit(EXIT_FAILURE);
    } 
    size_t splineNum = std::distance(muscleNames_.begin(), iter);
        
    maValues.at(i) = -splines_[splineNum].getFirstDerivative(angleCombinations,dofIndex);
  }
  
}

void SplineSet::evalMa(const string& outputDir, const  map<string, vector<string> >& musclesConnectedToDofs, const vector< vector< double > >& angleCombinations  ) {
  
  
  const int DIGIT_NUM = 8;   
  const int NUMBER_DIGIT_OUTPUT = 8;
  
  
  
  // for all the degrees of freedom in the map
  for ( map<string, vector<string> >::const_iterator musclesIt = musclesConnectedToDofs.begin(); musclesIt!= musclesConnectedToDofs.end() ; ++musclesIt) {
    vector<string>::iterator dofIt=find(dofNames_.begin(), dofNames_.end(), musclesIt->first);
    if ( dofIt==dofNames_.end() ) {
      cout << "ERROR: dofsNames differ\n";
      exit(EXIT_FAILURE);
    }
    size_t dofIndex=dofIt-dofNames_.begin(); //index for this degree of freedom in our member variables
    
    // Then open the outputDataFile
    string outputDataFilename = outputDir +  "ma" + dofNames_[dofIndex] + ".out";
    ofstream outputDataFile(outputDataFilename.c_str()); 
    
    if (!outputDataFile.is_open()) {
      cout << "ERROR: outputDataFile could not be open\n";
      exit(EXIT_FAILURE);
    }

    for (unsigned int i = 0; i < musclesIt->second.size(); ++i)
      outputDataFile << musclesIt->second.at(i) << "\t";
    outputDataFile << endl;
   

    double nextValue;
    for (unsigned int j = 0; j < angleCombinations.size(); ++j) {
      for (unsigned int i = 0; i < musclesIt->second.size(); ++i) {
         vector<string>::iterator iter = find(muscleNames_.begin(), muscleNames_.end(), musclesIt->second.at(i));
         if (iter == muscleNames_.end()) {
           cout << "something went wrong\n";
           exit(EXIT_FAILURE);
         } 
        size_t splineNum = std::distance(muscleNames_.begin(), iter);
        outputDataFile << std::setprecision(NUMBER_DIGIT_OUTPUT) << std::fixed << -roundIt(splines_[splineNum].getFirstDerivative(angleCombinations.at(j),dofIndex), DIGIT_NUM+2) << "\t";
          
      } 
      outputDataFile << endl;
    }
    
  outputDataFile.close();
  }
  
}

  
