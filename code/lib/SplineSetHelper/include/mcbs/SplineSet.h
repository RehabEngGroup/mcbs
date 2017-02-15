/*
 * Copyright (C) 2013 C. Pizzolato, M. Reggiani, M. Sartori
 * Copyright (C) 2015 E.Ceseracciu
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
#include <memory>
#include <mcbs/Spline.h>
#include "SplineSetBase.h"

//const int N_DOF_SPLINESET = 3;
namespace mcbs {
    template <int N_DOF_SPLINESET>
    class SplineSet: public mcbs::SplineSetBase
    {
    public:
        SplineSet() = delete;
        SplineSet(const std::string& inputDataFilename);
        SplineSet(const std::vector< std::string > dofNames, const std::vector< std::string > musclesNames, const std::vector<double>& a, const std::vector<double>& b, const std::vector<int>& n, const std::vector<std::vector<double> >& y);
        SplineSet(const std::string& coeffDir, const std::vector<std::string>& muscleNames);
        SplineSet(const SplineSet& orig) = delete;
        std::vector<std::string> getDofNames();
        void exportCoefficients(const std::string& outDir) override;
        void getLmt(std::vector<double>& lmtValues, const std::vector<double>& dofValues) override;
        void getMa(const std::string& dofName,
            const std::vector< std::string >& musclesConnectedToDofs,
            const std::vector< double >& angleCombinations,
            std::vector <double>& maValues) override;
        void getMaDerivative(const std::string& dofName,
            const std::vector< std::string >& musclesConnectedToDofs,
            const std::vector< double >& angleCombinations,
            std::vector <double>& maDerivativeValues) override;
        void evalLmt(const std::string& outputDir, const std::vector< std::vector< double > >& angleCombinations) override;
        void evalMa(const std::string& outputDir, const std::map< std::string, std::vector< std::string > >& musclesConnectedToDofs, const std::vector< std::vector< double > >& angleCombinations) override;
        void evalMaDerivative(const std::string& outputDir, const std::map< std::string, std::vector< std::string > >& musclesConnectedToDofs, const std::vector< std::vector< double > >& angleCombinations) override;
        static std::shared_ptr<SplineSet > make(const std::string& inputDataFilename) {return std::shared_ptr<SplineSet<N_DOF_SPLINESET> >(new SplineSet<N_DOF_SPLINESET>(inputDataFilename));};
        static std::shared_ptr<SplineSet > make(const std::vector< std::string > dofNames, const std::vector< std::string > musclesNames, const std::vector<double>& a, const std::vector<double>& b, const std::vector<int>& n, const std::vector<std::vector<double> >& y) {
            return std::shared_ptr<SplineSet<N_DOF_SPLINESET> >(new SplineSet<N_DOF_SPLINESET>(dofNames, musclesNames, a, b, n, y));
        };
        static std::shared_ptr<SplineSet> make(const std::string& coeffDir,
                                                          const std::vector<std::string>& muscleNames){
            return std::shared_ptr<SplineSet<N_DOF_SPLINESET>>(new SplineSet<N_DOF_SPLINESET>(coeffDir, muscleNames));
        }
    private:
        std::vector< mcbs::Spline<N_DOF_SPLINESET> >  splines_;
        std::vector< std::string >    dofNames_;
        std::vector< double > a_;
        std::vector< double > b_;
        std::vector< int > n_;
        std::vector< std::vector< double > > y_;
        std::ifstream inputDataFile_;
        std::vector< std::string > muscleNames_;
        int noMuscles_;
        int noInputData_;
        void readInputData();
        void getCoefficientsFromFile(std::vector<double>& coefficientsFromFile, const std::string& coeffDir, const std::string& muscleName);
    };
}
#endif
