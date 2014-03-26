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


#ifndef SplineSet_h
#define SplineSet_h

#include <fstream>
#include <vector>
#include <string>
#include <map>
#include "Spline.h"
#include "DofCfg.h"


const int N_DOF = 4;

class SplineSet
{
public:
    SplineSet();
    SplineSet(const std::string& inputDataFilename);
    SplineSet(const std::string& coeffDir, const std::vector<std::string>& muscleNames);
    SplineSet(const SplineSet& orig);
    std::vector<std::string> getDofNames();
    void exportCoefficients(const std::string& outDir);
    void getLmt(std::vector<double>& lmtValues, const std::vector<double>& dofValues);
    void getMa(const std::string& dofName, 
                       const std::vector< std::string >& musclesConnectedToDofs, 
                       const std::vector< double >& angleCombinations,
                       std::vector <double>& maValues);
    void evalLmt( const std::string& outputDir, const std::vector< std::vector< double > >& angleCombinations ); 
    void evalMa(const std::string& outputDir, const std::map< std::string, std::vector< std::string > >& musclesConnectedToDofs, const std::vector< std::vector< double > >& angleCombinations  );
  
private:
    std::vector< Spline<N_DOF> >  splines_;
    std::vector< std::string >    dofNames_;
    std::vector< double > a_;
    std::vector< double > b_;
    std::vector< int > n_;
    std::vector< std::vector< double > > y_;
    
    
    std::ifstream inputDataFile_;
    std::vector< std::string > muscleNames_;
    int noMuscles_;
    int noInputData_;
    
    void createSplines();
    void readInputData();
    void importCoefficients(const std::string& coeffDir, const std::vector<std::string>& muscleNames);
    void getCoefficientsFromFile(std::vector<double>& coefficientsFromFile, const std::string& coeffDir, const std::string& muscleName);
};

#endif 
