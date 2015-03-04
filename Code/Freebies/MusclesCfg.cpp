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

#include "MusclesCfg.h"
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;
#include <cstdlib>


MusclesCfg::MusclesCfg(const string& musclesFilename)
{
    ifstream musclesFile(musclesFilename.c_str());
    if (!musclesFile) {
        cout << "Error: file " << musclesFilename << " could not be opened\n";
        exit(EXIT_FAILURE);
    }
    string line;
    while(std::getline(musclesFile, line)) {
        std::istringstream iss(line);
        string nextMuscle;
        iss >> nextMuscle;
        muscleNames_.push_back(nextMuscle);
        string nextDof;
        while (iss >> nextDof && nextDof!="") {
            musclesConnectedToDof_[nextDof].push_back(nextMuscle);
        }
    }
    musclesFile.close();
}

const vector< string >& MusclesCfg::getMuscleNames()
{
    return muscleNames_;
}

const map< string, vector< string > >& MusclesCfg::getMusclesPerDof()
{
    return musclesConnectedToDof_;
}
