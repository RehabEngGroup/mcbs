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

#include "OpenSim2StdTools.h"

#include <string>
using std::string;
#include <vector>
using std::vector;

namespace OpenSim2StdTools {
    const string osimMuscleNames[] = { "glut_med1", "glut_med2", "glut_med3", "glut_min1",
                                  "glut_min2", "glut_min3", "semimem", "semiten",
                                  "bifemlh", "bifemsh", "sar", "add_long", "add_brev",
                                  "add_mag1", "add_mag2", "add_mag3", "tfl", "grac",
                                  "glut_max1", "glut_max2", "glut_max3", "iliacus",
                                  "psoas", "rect_fem", "vas_med", "vas_int", "vas_lat",
                                  "med_gas", "lat_gas", "soleus", "tib_ant", "per_brev",
                                  "per_long", "per_tert", "\0"};                                 
    const string stdMuscleNames[] = { "gmed1", "gmed2", "gmed3", "gmin1", "gmin2", "gmin3",
                                 "semimem", "semiten", "bicfemlh", "bicfemsh", "sar",
                                 "addlong", "addbrev", "addmag1", "addmag2", "addmag3",
                                 "tfl", "gra", "gmax1", "gmax2", "gmax3", "illiacus",
                                 "psoas", "recfem", "vasmed", "vasint", "vaslat", "gasmed",
                                 "gaslat", "sol", "tibant", "perbrev", "perlong", "perter", "\0"};
                                 
    const string osimDofNames[] = { "hip_flexion_r", "hip_adduction_r", "hip_rotation_r", "knee_angle_r", "ankle_angle_r", "\0"};
    const string stdDofNames[] = { "R_hip_fe", "R_hip_aa", "R_hip_rot", "R_knee_fe", "R_ankle_fe", "\0"};

    const string std2osimMuscleName(const string& srcStdName) {
        string dstOsimName;
        string s = srcStdName.substr(srcStdName.rfind('_')); 
        string myStdName;
        if (s == "_r" || s == "_l")
            myStdName = srcStdName.substr(0, srcStdName.size()-2);
        else{
            myStdName = srcStdName;
            s = "";
        }
        int i = 0;
        while (stdMuscleNames[i] != "\0") {
            if (stdMuscleNames[i] == myStdName)
                dstOsimName = osimMuscleNames[i] + s;
            ++i;    
        }
  
        return dstOsimName;
    }
 
    const string osim2stdMuscleName(const string& srcOsimName) {
        string dstStdName;
        string s = srcOsimName.substr(srcOsimName.rfind('_')); 
        string myOsimName;
        if (s == "_r" || s == "_l")
            myOsimName = srcOsimName.substr(0, srcOsimName.size()-2);
        else {
            myOsimName = srcOsimName;
            s = "";
        }
        int i = 0;
        while (osimMuscleNames[i] != "\0") {
            if (osimMuscleNames[i] == myOsimName)
                dstStdName = stdMuscleNames[i] + s;
            ++i;    
        }
        return dstStdName;
    }
  
    const string std2osimDofName(const string& srcStdName) {
        string dstOsimName;
        int i = 0;
        while (stdDofNames[i] != "\0") {
            if (stdDofNames[i] == srcStdName)
                dstOsimName = osimDofNames[i];
            ++i;       
        }
  
        return dstOsimName;
    } 

    const string osim2stdDofName(const string& srcOsimName) {
        string dstStdName;
        int i = 0;
        while (osimDofNames[i] != "\0"){
            if (osimDofNames[i] == srcOsimName)
                dstStdName = stdDofNames[i];
            ++i;    
        } 

        return dstStdName;      
    }


}