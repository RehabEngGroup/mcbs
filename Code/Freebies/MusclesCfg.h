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

#ifndef MusclesCfg_h
#define MusclesCfg_h

#include <string>
#include <vector>
#include <map>

class MusclesCfg{
private:
    std::vector<std::string> muscleNames_;
    std::map<std::string, std::vector<std::string> > musclesConnectedToDof_;
    MusclesCfg(){};
public:
    MusclesCfg(const std::string& musclesFilename);
    const std::vector<std::string>& getMuscleNames();
    const std::map<std::string, std::vector<std::string> >& getMusclesPerDof();
};
#endif
