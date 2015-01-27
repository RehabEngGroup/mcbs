// Copyright (c) 2011, Massimo Sartori and Monica Reggiani
// All rights reserved.

// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
// - Redistributions of source code must retain the above copyright notice, 
//   this list of conditions and the following disclaimer.
// - Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include <sstream>
using std::stringstream;
#include <iostream>
using std::cout;
using std::endl;

#include <cmath>
#include <cstdlib>

#include <fstream>
using std::ifstream;
using std::ofstream;

#include <iomanip>

#include "SplineData.h"
#include "Spline.cpp"

//#define LOG

const int DIGIT_NUM = 8;   
const int NUMBER_DIGIT_OUTPUT = 8;

inline double roundIt(double x, double n = DIGIT_NUM) {
  return floor( x * pow(10.0, n) + 0.5) / pow(10.0, n);
}

inline double radians (double d) {
return d * M_PI / 180;
}

inline double degrees (double r) {
return r * 180/ M_PI;
}


SplineData::SplineData(const string& inputDataFilename)
:inputDataFile_(inputDataFilename.c_str()), dofName_(N_DOF),  a_(N_DOF), b_(N_DOF), n_(N_DOF) {

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


void SplineData::readInputData() {
 
  // --- Read DOFs 
  for (int i=0; i < N_DOF; ++i) {
    inputDataFile_ >> dofName_[i];
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



void SplineData::displayInputData() {

  cout << "-- DOFs  \n";
  cout << "DofName\t a \t b \t n \t h \n";
  for (int i=N_DOF-1; i >=0; --i) {
    cout << dofName_[i] << "\t";
    cout << a_[i] << "\t" ;
    cout << b_[i] << "\t" ;
    cout << n_[i] << "\t" ;
    cout << (b_[i]-a_[i])/n_[i] << endl; 
  } 
  
  cout << "-- Data \n"; 
  for (int i = N_DOF-1; i >=0; --i) 
     cout << dofName_[i] << "\t";
  
  for (int i=0; i < noMuscles_; ++i) {
     cout << muscleNames_[i] << "\t";
  }
  cout << endl;
 
    
  vector<int> index(N_DOF);
  double tot, mul;
  for (int soFar = 0; soFar < noInputData_; ++soFar) {
    tot = 0; mul = noInputData_;
    for (int i = N_DOF-1; i >0; --i) {
      mul = mul / ( n_[i] + 1 );
      index[i] = ( soFar - tot )/ mul;
      tot += index[i] * mul;
    } 
    index[0] = soFar % ( n_[0] + 1 );
    for (int i = N_DOF-1; i >= 0; --i) {
       cout << a_[i] + index[i] * (b_[i]-a_[i])/n_[i] << "\t";
    }
    for (int j = 0; j < noMuscles_; ++j) 
           cout << y_[j][soFar] << "\t";
    cout << endl;
  }
}


void SplineData::readEvalAngles() {
  string anglesFilename = evalDataDir_ + "/angles.in";  
  ifstream anglesFile(anglesFilename.c_str());
  
  if (!anglesFile.is_open()) {
    cout << "ERROR: " << anglesFilename << " could not be open\n";
    exit(EXIT_FAILURE);
  }

  anglesFile >> noEvalData_;
  for (int i = 0; i < noEvalData_; ++i)
    angles_.push_back( vector<double>(N_DOF));
       
  for (int i=0; i < noEvalData_; ++i) 
    for (int j=N_DOF-1; j>=0; --j) { 
      anglesFile >> angles_[i][j]; angles_[i][j] = radians(angles_[i][j]);
    }  
  
  anglesFile.close();

}



void SplineData::openOutputFile(ofstream& outputDataFile) {
  
  if (!outputDataFile.is_open()) {
    cout << "ERROR: outputDataFile could not be open\n";
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < noMuscles_; ++i) 
    outputDataFile << muscleNames_[i] << "\t";
  outputDataFile << endl;
  
  for (int i = 0; i < noMuscles_; ++i)
    outputDataFile << "eval\t";
  outputDataFile << endl;
}  

void SplineData::openEvalFile(ifstream& evalDataFile) {
  
  
  if (!evalDataFile.is_open()) {
    cout << "ERROR: evalData File could not be open\n";
    exit(EXIT_FAILURE);
  }


  // check we have the same amount of data of angles
  int numRows;
  evalDataFile >> numRows;
  
  if (numRows != noEvalData_) {
    cout << "ERROR: we have " << noEvalData_ << " angles, but " << numRows 
         << " lines of data in input from the evalDataFile"  << endl;
    exit(EXIT_FAILURE);
  }
  
  // check we have the same muscles 
  string line;
  vector<string> muscleNames;
  getline(evalDataFile, line, '\n'); getline(evalDataFile, line, '\n');  
  stringstream myStream(line);
  string nextMuscle; 
 
  do {
    myStream >> nextMuscle;
    muscleNames.push_back(nextMuscle); 
  } while (!myStream.eof());
  
  if (muscleNames.size() != static_cast<unsigned int>(noMuscles_)) {
    cout << "ERROR: we have " << noMuscles_ << " interpolated muscles, but " << muscleNames.size()
         << " muscles in the evalDataFile" << endl;
    exit(EXIT_FAILURE);
  }
  
  for (int i = 0 ; i < noMuscles_; ++i) {
     if (muscleNames[i] != muscleNames_[i]) {
        cout << "ERROR: the " << i << "-th muscle is " 
             << muscleNames_[i] << " in the interpolated Data while"
             << muscleNames[i] << " in file evalData " << endl;
        exit(EXIT_FAILURE);
    }
  }
   
}



void SplineData::evalLmt() {

  // First open the inputDataFile
  string evalDataFilename = evalDataDir_ + "lmt.in";  
  ifstream evalDataFile(evalDataFilename.c_str());
  
  openEvalFile(evalDataFile);
    
  // Then open the outputDataFile

  string outputDataFilename = evalDataDir_ + "lmt.out";  
  ofstream outputDataFile(outputDataFilename.c_str());
  openOutputFile(outputDataFile);

  // now readData   

  double nextValue;
  for (int j = 0; j < noEvalData_; ++j) {
   for (int i = 0; i < noMuscles_; ++i) {      
      // we round the results at the number of digits of the input file
      evalDataFile >> nextValue;
     // outputDataFile <<  std::setprecision(NUMBER_DIGIT_OUTPUT) << std::fixed << nextValue << "\t";
      outputDataFile <<  std::setprecision(NUMBER_DIGIT_OUTPUT) << std::fixed << roundIt(splines_[i].getValue(angles_[j]), DIGIT_NUM+2) << "\t";
   }
   outputDataFile << endl;
  }  
  outputDataFile.close();   
}


void SplineData::evalMa() {
  // for all the degree of freedom
  for (int k = 0; k < N_DOF; ++k) {
      
    // First open the inputDataFile
    string evalDataFilename = evalDataDir_ + "ma" + dofName_[k] + ".in";  
    ifstream evalDataFile(evalDataFilename.c_str());
  
    openEvalFile(evalDataFile);
    
    // Then open the outputDataFile

    string outputDataFilename = evalDataDir_ +  "ma" + dofName_[k] + ".out";  
    ofstream outputDataFile(outputDataFilename.c_str());
    openOutputFile(outputDataFile);

    double nextValue;
    for (int j = 0; j < noEvalData_; ++j) {
      for (int i = 0; i < noMuscles_; ++i) {  
        evalDataFile >> nextValue;
      //  outputDataFile << std::setprecision(NUMBER_DIGIT_OUTPUT) << std::fixed << nextValue << "\t";
        outputDataFile << std::setprecision(NUMBER_DIGIT_OUTPUT) << std::fixed << -roundIt(splines_[i].getFirstDerivative(angles_[j],k), DIGIT_NUM+2) << "\t";
          
      } 
      outputDataFile << endl;
    }
    outputDataFile.close();
  }
}

