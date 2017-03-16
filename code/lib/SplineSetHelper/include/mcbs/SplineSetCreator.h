#ifndef SPLINE_SET_CREATOR_H
#define SPLINE_SET_CREATOR_H

#include <vector>
#include <string>
#include <memory>

#include "SplineSetBase.h"

namespace mcbs {
    class SplineSetCreator {
        public:
            static std::shared_ptr<SplineSetBase> makeSplineSet(int degree,
                                                                const std::string& inputDataFilename);

            static std::shared_ptr<SplineSetBase> makeSplineSet(int degree,
                                                                const std::vector< std::string > dofNames,
                                                                const std::vector< std::string > musclesNames,
                                                                const std::vector<double>& a,
                                                                const std::vector<double>& b,
                                                                const std::vector<int>& n,
                                                                const std::vector<std::vector<double> >& y);

            static std::shared_ptr<SplineSetBase> makeSplineSet(const std::string& coeffDir,
                                                                const std::vector<std::string>& muscleNames);
    };
};

#endif // SPLINE_SET_CREATOR_H
