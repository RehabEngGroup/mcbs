/*
 * Copyright (C) 2013 M. Reggiani, M. Sartori
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


#ifndef Spline_h
#define Spline_h


#include <vector>
#include <exception>

#include <mcbs/SplineBasisFunction.h>

namespace mcbs {

class x_out_of_bounds: public std::exception
{
public:
  virtual const char* what() const throw()
  {
    return "Values x are out of boundaries";
  }
};


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
    std::vector<double>& getCoefficients() { return c_; }
    void setCoefficients(const std::vector<double>& coefficients) { c_ = coefficients; }

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
    double getSecondDerivative(const std::vector<double>& x, const int dimDerivative);
    std::vector<double>& getCoefficients() { return c_; }
    void setCoefficients(const std::vector<double>& coefficients) { c_ = coefficients; }

    friend class Spline<dim+1>;
};

}


#endif
