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

#include "../src/Spline.h"
#include "../src/Spline.cpp"
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
  
  // Check arguments
 cout << "n inputs " << nrhs << " n outputs" << nlhs <<endl;
  if (nrhs!=2)
  {
    mexErrMsgTxt("Two input arguments required: C, x");
  } else if (nlhs > 3) { //CHECK
  mexErrMsgTxt("Too many output arguments.");
}
  const int N_DOF = 4; 

  double* aArrayPtr;
  double* bArrayPtr;
  double* cArrayPtr;
  int* nArrayPtr;
  
  double* xArrayPtr;
  if(!mxIsDouble(prhs[1]) || mxGetNumberOfDimensions(prhs[1])!=2)
    mexErrMsgTxt("Argument x must be a double matrix.");
  int noSamples=mxGetM(prhs[1]);
  xArrayPtr=mxGetPr(prhs[1]);
  if (mxGetN(prhs[1])!=N_DOF)
    mexErrMsgIdAndTxt("evalSpline:wrongInputs","Argument x must have %d columns, corresponding to the number of degrees of freedom.", N_DOF);
    

  
  int noMuscles=mxGetNumberOfElements(prhs[0]);
  vector< SplineMatlab<N_DOF> > splines;
  for (int k=0; k<noMuscles; ++k)
  { 
    std::vector<double> a;
    std::vector<double> b;
    std::vector<int> n;
//     a.clear();
//     b.clear();
//     n.clear();
    mxArray *aArray=mxGetField(prhs[0], k, "a");
    mxArray *bArray=mxGetField(prhs[0], k, "b");
    mxArray *nArray=mxGetField(prhs[0], k, "n");
    mxArray *cArray=mxGetField(prhs[0], k, "C"); // check number of coefficients?
    if(!mxIsDouble(aArray) || mxGetNumberOfDimensions(aArray)!=2 || (mxGetDimensions(aArray)[0]!=1 && mxGetDimensions(aArray)[1]!=1))
      mexErrMsgIdAndTxt("evalSpline:wrongInputs","Field a in element %d must be a double vector.", k);
    if(!mxIsDouble(bArray) || mxGetNumberOfDimensions(bArray)!=2 || (mxGetDimensions(bArray)[0]!=1 && mxGetDimensions(bArray)[1]!=1))
      mexErrMsgIdAndTxt("evalSpline:wrongInputs","Field b in element %d must be a double vector.", k);
    if(!mxIsInt32(nArray) || mxGetNumberOfDimensions(nArray)!=2 || (mxGetDimensions(nArray)[0]!=1 && mxGetDimensions(nArray)[1]!=1))
      mexErrMsgIdAndTxt("evalSpline:wrongInputs","Field n in element %d must be an int vector.", k);
    if(!mxIsDouble(cArray) || mxGetNumberOfDimensions(cArray)!=2 || (mxGetDimensions(cArray)[0]!=1 && mxGetDimensions(cArray)[1]!=1))
      mexErrMsgIdAndTxt("evalSpline:wrongInputs","Field C in element %d must be a double vector.", k);

    if(mxGetDimensions(aArray)[0]*mxGetDimensions(aArray)[1]!=N_DOF || mxGetDimensions(bArray)[0]*mxGetDimensions(bArray)[1]!=N_DOF || mxGetDimensions(nArray)[0]*mxGetDimensions(nArray)[1]!=N_DOF)
      mexErrMsgIdAndTxt("evalSpline:wrongInputs","a, b and n in element %d must be all of length %d. If you want to use a different number of degrees of freedom, change N_DOF in the mex file and recompile it.", k, N_DOF);
 

    aArrayPtr=mxGetPr(aArray);
    bArrayPtr=mxGetPr(bArray);
    cArrayPtr=mxGetPr(cArray);
    nArrayPtr=(int*) mxGetData(nArray);
    
    n.assign(nArrayPtr, nArrayPtr+N_DOF);
    a.assign(aArrayPtr, aArrayPtr+N_DOF);
    b.assign(bArrayPtr, bArrayPtr+N_DOF);
    //fill structures
//     for (int i=0; i<N_DOF; ++i)
//     {
//       n.push_back(nArrayPtr[i]);
//       a.push_back(aArrayPtr[i]);
//       b.push_back(bArrayPtr[i]);
//     }
  
    SplineMatlab<N_DOF> newSpline(a,b,n);
    splines.push_back(newSpline);
  
    std::vector<double> newCoefficients;
    int noCoefficients=mxGetM(cArray);
    newCoefficients.clear();
    for (int j=0; j<noCoefficients; ++j)
    {
      newCoefficients.push_back(cArrayPtr[j]);
    }
    splines.back().setCcoefficients(newCoefficients);
  
  }
 
  
  plhs[0]=mxCreateDoubleMatrix(noSamples, noMuscles, mxREAL);
  double *lmtArrayPtr=mxGetPr(plhs[0]);
  double *maArrayPtr;
  if (nlhs>1)
  {
    mwSize dims[]={noSamples, noMuscles, N_DOF};
    plhs[1]=mxCreateNumericArray(3, dims, mxDOUBLE_CLASS, mxREAL);
    maArrayPtr=mxGetPr(plhs[1]);
  }
  
  vector<double> angles(N_DOF);
  

  for (int j = 0; j < noSamples; ++j) 
  {
    angles.clear();
    for (int k=N_DOF-1; k>=0; --k)
    {
      angles.push_back(xArrayPtr[j+(k * noSamples)]);

     // cout << "angolo "<< j << " dof " << k << ": "<< radians(xArrayPtr[j+(k * noSamples)])<< "\t";
    }
    for (int i = 0; i < noMuscles; ++i) 
    { 
      //cout << "pre lmt sample " << j << " musc " << i <<endl;
      lmtArrayPtr[j+(i*noSamples)]=splines[i].getValue(angles);
      //cout << "post lmt sample " << j << " musc " << i <<endl;      
      if(nlhs>1)
      {
        for (int k=0; k<N_DOF; ++k)
          maArrayPtr[j+(i*noSamples)+(k*noMuscles*noSamples)]=splines[i].getFirstDerivative(angles,k);
          
      }
    }
  }

} 

