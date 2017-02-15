#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <iostream>

#include "mcbs/SplineSetCreator.h"
#include "mcbs/SplineSet.h"
#include "mcbs/SplineSetBase.h"

namespace mcbs {
    template<int N, typename... Args> static auto wrap(Args... args) -> decltype(SplineSet<N>::make(args...)) {
        return SplineSet<N>::make(args...);
    }

    //we use this because the second constructor has plenty of arguments
    template<int N> static std::shared_ptr<SplineSetBase> wrapAlias(const std::vector< std::string > dofNames,
                                                                               const std::vector< std::string > musclesNames,
                                                                               const std::vector<double>& a,
                                                                               const std::vector<double>& b,
                                                                               const std::vector<int>& n,
                                                                               const std::vector<std::vector<double> >& y) {
        return wrap < N, const std::vector< std::string >, const std::vector< std::string >,
                  const std::vector<double>&, const std::vector<double>&, const std::vector<int>&,
                  const std::vector<std::vector<double> >& > (dofNames, musclesNames, a, b, n, y);


    };

    std::shared_ptr<SplineSetBase> SplineSetCreator::makeSplineSet(int degree, const std::string& inputDataFilename) {
        std::function<std::shared_ptr<SplineSetBase>(const std::string&)> ftable[] = {
            nullptr,
            nullptr,
            &wrap<2, std::string>,
            &wrap<3, std::string>,
            &wrap<4, std::string>,
            &wrap<5, std::string>,
            &wrap<6, std::string>,
            &wrap<7, std::string>,
        };

        if (degree > 7) {
            std::cout << "Degree of mcbs too high.\nExit.\n";
            std::exit(EXIT_FAILURE);
        }
        return ftable[degree](inputDataFilename);
        }



    std::shared_ptr<SplineSetBase> SplineSetCreator::makeSplineSet(
        int degree,
        const std::vector<std::string > dofNames,
        const std::vector<std::string > musclesNames,
        const std::vector<double>& a,
        const std::vector<double>& b,
        const std::vector<int>& n,
        const std::vector<std::vector<double>>& y) {
            std::function < std::shared_ptr<SplineSetBase>(
                const std::vector<std::string >,
                const std::vector<std::string >,
                const std::vector<double>&,
                const std::vector<double>&,
                const std::vector<int>&,
                const std::vector<std::vector<double> >&) > ftable[] = {
                    nullptr,
                    nullptr,
                    &wrapAlias<2>,
                    &wrapAlias<3>,
                    &wrapAlias<4>,
                    &wrapAlias<5>,
                    &wrapAlias<6>,
                    &wrapAlias<7>,
                };

            if (degree > 7) {
                std::cout << "Degree of mcbs too high.\nExit.\n";
                std::exit(EXIT_FAILURE);
            }
            return ftable[degree](dofNames, musclesNames, a, b, n, y);
        }

    std::shared_ptr<SplineSetBase> SplineSetCreator::makeSplineSet(
        const std::string& coeffDir,
        const std::vector<std::string>& muscleNames) {

        std::string anglesFilename = coeffDir + "/dofAngles.cfg";
        std::ifstream anglesFile(anglesFilename.c_str());
        if (!anglesFile.is_open()) {
            std::cout << "ERROR: " << anglesFilename << " could not be opened" << std::endl;
            exit(EXIT_FAILURE);
        }

        std::string trash;
        anglesFile >> trash;

        int noDofs;
        anglesFile >> noDofs;

            std::function < std::shared_ptr<SplineSetBase>(
                const std::string&,
                const std::vector<std::string>&) > ftable[] = {
                    nullptr,
                    nullptr,
                    &wrap<2, const std::string&, const std::vector<std::string>&>,
                    &wrap<3, const std::string&, const std::vector<std::string>&>,
                    &wrap<4, const std::string&, const std::vector<std::string>&>,
                    &wrap<5, const std::string&, const std::vector<std::string>&>,
                    &wrap<6, const std::string&, const std::vector<std::string>&>,
                    &wrap<7, const std::string&, const std::vector<std::string>&>,
                };

            if (noDofs > 7) {
                std::cout << "Degree of mcbs too high.\nExit.\n";
                std::exit(EXIT_FAILURE);
            }
            return ftable[noDofs](coeffDir, muscleNames);
        }

}
