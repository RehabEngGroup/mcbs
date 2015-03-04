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
#include <vector>
using std::vector;
#include <stdlib.h>

#include "../SplineSrc/include/mcbs/Spline.h"
#include "../SplineSrc/include/mcbs/Spline.cpp"
#include "mex.h"


template <int dim> class SplineMatlab: public Spline<dim>{
public:
  SplineMatlab(const std::vector<double>& a, const std::vector<double>& b, const std::vector<int>& n ):Spline<dim>(a,b, n){};
  std::vector<double> getCcoefficients(){return this->c_;};
  void setCcoefficients(std::vector<double> newC){this->c_=newC;};
  
};
void mexFunction(
                 int          nlhs,
                 mxArray      *plhs[],
                 int          nrhs,
                 const mxArray *prhs[]
                 )
{
  const int N_DOF = 4;
  
  // Check arguments 
  if (nrhs<4)
  {
    mexErrMsgTxt("Four input arguments required: y, a, b, n");
  } 
  else if (nlhs > 1) 
  { 
    mexErrMsgTxt("Too many output arguments.");
  }
  
  if(!mxIsDouble(prhs[0]) || mxGetNumberOfDimensions(prhs[0])!=2)
    mexErrMsgTxt("Argument y must be a double matrix.");
  if(!mxIsDouble(prhs[1]) || mxGetNumberOfDimensions(prhs[1])!=2 || (mxGetDimensions(prhs[1])[0]!=1 && mxGetDimensions(prhs[1])[1]!=1))
    mexErrMsgTxt("Argument a must be a double vector.");
  if(!mxIsDouble(prhs[2]) || mxGetNumberOfDimensions(prhs[2])!=2 || (mxGetDimensions(prhs[2])[0]!=1 && mxGetDimensions(prhs[2])[1]!=1))
    mexErrMsgTxt("Argument b must be a double vector.");
  if(!mxIsInt32(prhs[3]) || mxGetNumberOfDimensions(prhs[3])!=2 || (mxGetDimensions(prhs[3])[0]!=1 && mxGetDimensions(prhs[3])[1]!=1))
    mexErrMsgTxt("Argument n must be an int vector.");

  if(mxGetDimensions(prhs[1])[0]*mxGetDimensions(prhs[1])[1]!=N_DOF || mxGetDimensions(prhs[2])[0]*mxGetDimensions(prhs[2])[1]!=N_DOF || mxGetDimensions(prhs[3])[0]*mxGetDimensions(prhs[3])[1]!=N_DOF)
    mexErrMsgIdAndTxt("createSpline:wrongInputs","a, b and n must be all of length %d. If you want to use a different number of degrees of freedom, change N_DOF in the mex file and recompile it.", N_DOF);
 
  double* yArrayPtr;
  double* aArrayPtr;
  double* bArrayPtr;
  int* nArrayPtr;
 
  std::vector<double> a;
  std::vector<double> b;
  std::vector<int> n;
  vector< vector< double> > y;

  yArrayPtr=mxGetPr(prhs[0]);  
  aArrayPtr=mxGetPr(prhs[1]);
  bArrayPtr=mxGetPr(prhs[2]);
  nArrayPtr=(int*) mxGetData(prhs[3]);
 
  int noMuscles=mxGetN(prhs[0]);
  int noSamples=mxGetM(prhs[0]);
  //fill structures
  for (int i=0; i<N_DOF; ++i)
  {
   n.push_back(nArrayPtr[i]);
   a.push_back(aArrayPtr[i]);
   b.push_back(bArrayPtr[i]);
  }
 
  for (int i=0; i<noMuscles; ++i)
  {
    std::vector<double> dummyVec;
    dummyVec.clear();
    for (int k=0; k<noSamples; ++k)
    {
      dummyVec.push_back(yArrayPtr[k+i*noSamples]);
    }
    y.push_back(dummyVec);
  } 
 
  vector< SplineMatlab<N_DOF> > splines;
  for (int i = 0; i < noMuscles; ++i) 
  {
    SplineMatlab<N_DOF> newSpline(a,b,n);
    splines.push_back(newSpline);
  }

// #ifdef LOG
//   cout << "Created " << splines_.size() << " splines.\n";
// #endif     

  // now compute coefficients for each muscle
  for (int i = 0; i < noMuscles; ++i) 
  {
    vector<double> currentMuscle(y[i]);
    splines[i].computeCoefficients(currentMuscle, currentMuscle.begin());
  }

//   plhs[0]=mxCreateDoubleMatrix(splines[0].getCcoefficients().size(), noMuscles, mxREAL);
//   double* cArrayPtr=mxGetPr(plhs[0]);
  // set up output structure
  const char *fieldnames[]={"C", "a", "b", "n"};
  int structDim[]={noMuscles};
  plhs[0]=mxCreateStructArray(1, structDim,  4, fieldnames);

  // assign spline coefficients to matrix
  for (int k=0; k<noMuscles; ++k)
  {
    mxArray * Cmatrix=mxCreateDoubleMatrix(splines[0].getCcoefficients().size(), 1, mxREAL);
    double* cArrayPtr=mxGetPr(Cmatrix);
    vector<double> Ccoef=splines[k].getCcoefficients();
    for (int j=0; j<Ccoef.size(); ++j)
      cArrayPtr[j]=Ccoef.at(j);
      //fill in output structure
    mxSetField(plhs[0],k,"C",Cmatrix);
    mxSetField(plhs[0],k,"a",mxDuplicateArray(prhs[1]));
    mxSetField(plhs[0],k,"b",mxDuplicateArray(prhs[2]));
    mxSetField(plhs[0],k,"n",mxDuplicateArray(prhs[3]));
    
  }



}
