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


#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <stdlib.h>

#include "SplineData.h"


int main(int argc, const char* argv[]) 
{
  cout << "----------------------------------------------------\n";
  cout << " SIMPLE PROGRAM TO TEST Spline CLASS\n";
  cout << " Copyright (C) 2010 Massimo Sartori, Monica Reggiani\n";
  cout << "----------------------------------------------------\n";

  // Check command line arguments
  if ( argc != 2 ) {
    cout << "Usage: testSpline dataDirectory\n";
    cout << " dataDirectory: directory with data, read README.*\n ";
    cout << "           and prepare your data file accordingly\n";
    exit(EXIT_FAILURE);
  }
  
  cout << "Computing Splines\n";
  
  // Build a set of spline (one for each muscle) 
  // reading the input data from the "lmt.in" file
  
  string dataDirectory = argv[1];
  string inputDataFilename = dataDirectory + "InputData/lmt.in";  

  SplineData splineData(inputDataFilename);

  // Now use the spline to evaluate lmt & ma on the nodes 
  // used as input to build the spline
  string evalDataDir;
  evalDataDir =  dataDirectory + "NodesData/";
  
  splineData.setEvalDataDir(evalDataDir);
  // get the angles
  splineData.readEvalAngles();
  
  cout << "Eval data for Lmt on Nodes\n";
  // now evaluate Lmt
  splineData.evalLmt();
  
  cout << "Eval data for Ma on Nodes\n";
  // and evaluate Ma
  splineData.evalMa();
  
  
  // Now use the spline to evaluate lmt & ma on the nodes 
  // between the ones used as input to build the spline
  evalDataDir =  dataDirectory + "BetweenNodesData/";
  
  splineData.setEvalDataDir(evalDataDir);
  // get the angles
  splineData.readEvalAngles();
  
  cout << "Eval data for Lmt between Nodes\n";
  // now evaluate Lmt
  splineData.evalLmt();
  
  cout << "Eval data for Ma between Nodes\n";
  // and evaluate Ma
  splineData.evalMa();
  exit(EXIT_SUCCESS);
}
