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


#ifndef SplineData_h
#define SplineData_h

#include "Spline.h"

#include <vector>
using std::vector;
#include <string>
using std::string;

#include <fstream>
using std::ifstream;
using std::ofstream;

const int N_DOF = 4;


class SplineData { 
public:
  SplineData(const string& inputDataFilename);
  void setEvalDataDir(const string& evalDataDir) {evalDataDir_ = evalDataDir;}
  void readEvalAngles();
  void evalLmt();
  void evalMa(); 
private:
  void readInputData();
  void displayInputData();
  void openEvalFile(ifstream& evalDataFile);
  void openOutputFile(ofstream& outputDataFile); 
  ifstream inputDataFile_;
  
  // Interpolation Data
  vector<string> dofName_;
  vector<double> a_;
  vector<double> b_;
  vector<int> n_;
  vector<string> muscleNames_; 
  int noMuscles_;
  int noInputData_; 
  vector< vector< double> > y_;
    
  // Splines
  vector< Spline<N_DOF> > splines_;
  
  // EvalData
  string evalDataDir_;
  int noEvalData_;
  vector < vector <double> > angles_;
  
};


#endif
