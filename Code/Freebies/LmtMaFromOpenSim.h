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

#ifndef LmtMaFromOpenSim_h
#define LmtMaFromOpenSim_h

#include <string>
#include <vector>
#include <iostream>
#include "OpenSim/OpenSim.h"

/*! Computes lmt and ma values using OpenSim 
    You need to provide the osim model, the sequence of muscles and dofNames and the angleCombinations
    The constructor uses OpenSim to compute lmt and ma at the angleCombinations provided as input
 */
class LmtMaFromOpenSim{
public:
    LmtMaFromOpenSim();
    LmtMaFromOpenSim(const std::string& osimModelFilename, const std::vector<std::string>& dofNames, 
                     const std::vector<std::string>& musclesNames, 
                     const std::vector< std::vector<double> >& angleCombinations, bool computeMoments=false);
    
    LmtMaFromOpenSim(const LmtMaFromOpenSim& orig);
    ~LmtMaFromOpenSim();
    
     void saveLmt(std::ostream &os);
     void saveMa(const std::string &os);
    
//    void printAnglesCombinations(std::string anglesFilename);
  

 //   void const getGrid(std::vector< std::vector<double> >& lmtData) { lmtData = lmtData_; }
 //  void setOutDir(std::string& outDir)                             { outDir_ =  outDir; }

 
private:
   
    
    void convertToStorage(const std::vector<std::vector<double> >& anglesCombinations);
    void runMuscleAnalysis();
    
    //:TODO: so far computes only Lmt Grids
    void computeLmtMaFromOpenSim(const std::vector<std::string>& dofNames, 
                           const std::vector<std::string>& musclesNames, 
                           const std::vector< std::vector<double> >& angleCombinations);
    
    OpenSim::Model*                    osimModel_;
    
    OpenSim::Storage                   anglesDataForModel_;
    std::vector<std::string>           osimMusclesNames_;
    std::vector<std::string>           osimDofNames_;
    std::vector<std::string>           osimCoordinateNames_;
    int                                nDofs_;
    
    std::vector< std::vector<double> > lmtData_;
    std::vector< std::vector < std::vector<double> > > maData_;
    double                             startTime_;
    double                             endTime_;
    bool                               computeMoments_;
       
};

#endif