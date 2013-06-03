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



#ifndef Spline_h
#define Spline_h


#include <vector>

#include "SplineBasisFunction.h"

template <int dim>
class Spline; 

template <>
class Spline<1> {
  private:
    double a_;
    double b_;
    int n_;   
    double h_;
     
    void computeInterval(int& l, int& m, const double x) const;

    std::vector<double> c_; 
    
  public:
    Spline(const double a, const double b, const int n);
    Spline(const std::vector<double>& a, const std::vector<double>& b, const std::vector<int>& n); 
    void computeFewCoefficients(const std::vector<double>& y, std::vector<double>::iterator fromWhereInY); 
    void computeCoefficients(const std::vector<double>& y, std::vector<double>::iterator fromWhereInY); 
    double getValue(const double x) const;
    double getFirstDerivative(const double x);
    template<int dim> friend class Spline;
};


template <int dim>
class Spline {
  protected:
    std::vector<double> a_;
    std::vector<double> b_;
    std::vector<int> n_;
    std::vector<double> h_;
    
    int sizeOfY_;
    bool checkValues(const std::vector<double>& x) const;
    void computeInterval(std::vector<int>& l, std::vector<int>& m, const std::vector<double>& x) const;
    Spline<dim-1> splineFirstPhase_;
    Spline<1> splineSecondPhase_;
    std::vector<double> c_; 
    
  public:
    Spline(const std::vector<double>& a, const std::vector<double>& b, const std::vector<int>& n ); 
    void computeCoefficients(std::vector<double>& y, std::vector<double>::iterator fromWhereInY);
    double getValue(const std::vector<double>& x) const;
    double getFirstDerivative(const std::vector<double>& x, const int dimDerivative);
    
    friend class Spline<dim+1>;
};



#endif
