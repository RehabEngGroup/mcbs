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

#ifndef OpenSim2StdTools_h
#define OpenSim2StdTools_h

#include <string>
#include <vector>
#include <OpenSim/OpenSim.h>

namespace OpenSim2StdTools {

    template <class T>
    void vector2array(const std::vector<T>& srcVector, OpenSim::Array<T>& dstArray){
        int size = srcVector.size();
        dstArray.setSize(size); 
        for(int i = 0; i < size; ++i)
            dstArray.set(i, srcVector.at(i));
    }

    template <class T>
    void array2vector(const OpenSim::Array<T>& srcArray, std::vector<T>& dstVector){
        dstVector.clean();
        int size = srcArray.getSize();
        dstVector.resize(size);
        for(int i = 0; i < size; ++i)
            dstVector.at(i) = srcArray.get(i);
    }
    
}
#endif