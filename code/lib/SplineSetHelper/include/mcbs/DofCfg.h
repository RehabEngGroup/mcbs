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

#ifndef DofCfg_h
#define DofCfg_h

#include <vector>
#include <string>
#include <iostream>

namespace mcbs {

    /*! \brief Computes and stores grid of dof configurations
        Based on the data of a file whose name is provided in the constructor the class compute
        a sequence of dof configuration ("Nodes") that will be used to compute coefficient for the splines
        It also compute a sequence of dof configurations ("BetweenNodes") that lies in between
        the previous configurations. This configurations can be used to evaluate the effectiveness
        of splines interpolation.
        */

    class DofCfg{

    public:
        DofCfg();
        DofCfg(std::string cfgFilename);
        const std::vector<std::string>& getDofNames() const { return dofNames_; }
        const std::vector<double>& getAnglesStartValue() const { return anglesStartValue_; }
        const std::vector<double>& getAnglesStopValue() const { return anglesStopValue_; }
        const std::vector<int>& getNoSteps() const { return noIntervals_; }
        int getNoAngleCombinations() const { return nRows_; }
        int getNoBetweenAngleCombinations() const { return betweenAnglesCombinations_.at(0).size(); }
        const std::vector<std::vector<double> >& getAngleCombinations() const { return anglesCombinations_; }
        const std::vector<std::vector<double> >& getBetweenAngleCombinations() const { return betweenAnglesCombinations_; }

    private:
        std::vector<std::string> dofNames_;              // std-style names
        std::vector<double>      anglesStartValue_;      // degrees
        std::vector<double>      anglesStopValue_;       // degrees
        std::vector<int>         noIntervals_;
        std::vector< std::vector<double> >  anglesCombinations_;
        std::vector< std::vector<double> >  betweenAnglesCombinations_;
        int  multeplicity_;
        int  nRows_;
        int  nCols_;
        void generateAnglesCombinations();
        void generateBetweenAnglesCombinations();
        void createCombinations(const std::vector<double>& startValues,
            const std::vector<double>& stopValues,
            const std::vector<int>& noIntervals,
            std::vector< std::vector<double> >& combinations,
            int pos);

        bool isValid() {
            return (dofNames_.size() == anglesStartValue_.size()) &&
                (anglesStopValue_.size() == noIntervals_.size()) &&
                (dofNames_.size() == noIntervals_.size());
        }

        friend std::ostream & operator<<(std::ostream &os, const DofCfg& dofcfg);

    };

}
#endif
