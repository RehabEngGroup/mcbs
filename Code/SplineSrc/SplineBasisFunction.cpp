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


#include "SplineBasisFunction.h" 
#include <cmath>

double SplineBasisFunction::getValue(double x, int k, double a, double h){
  double t = fabs( ( (x - a) / h ) - (k - 1) );
  
  if ( (t <= 2) && (t >= 1) )   return pow(( 2 - t ), 3);
  else  if ( t < 1 )            return (4 - 6 * t * t + 3 * pow(t,3));
    else return 0;
}

double SplineBasisFunction::getFirstDerivative(double x, int k, double a, double h) {
  double t = ( ( (x - a) / h ) - (k - 1) );

  if ( (t <= 2) && (t >= 1) )                  return -3*(2-t)*(2-t)/h;
  else if ( (t < 1) && (t >= 0) )              return (-12*t+9*t*t)/h;
       else if ( (t < 0) && ( t >= -1) )       return (-12*t-9*t*t)/h;
            else if ( (t < -1) && (t >= -2) )  return 3*(2+t)*(2+t)/h;
                 else return 0;
}


