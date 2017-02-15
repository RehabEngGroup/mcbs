#ifndef SPLINE_SET_BASE_H
#define SPLINE_SET_BASE_H

#include <vector>
#include <string>
#include <map>

namespace mcbs {
    class SplineSetBase {
        public:
            virtual std::vector<std::string> getDofNames() = 0;

            virtual void getLmt(std::vector<double>& lmtValues,
                                const std::vector<double>& dofValues) = 0;

            virtual void getMa(const std::string& dofName,
                               const std::vector< std::string >& musclesConnectedToDofs,
                               const std::vector< double >& angleCombinations,
                               std::vector <double>& maValues) = 0;


            virtual void getMaDerivative(const std::string& dofName,
                                         const std::vector< std::string >& musclesConnectedToDofs,
                                         const std::vector< double >& angleCombinations,
                                         std::vector <double>& maDerivativeValues) = 0;

            virtual void evalLmt(const std::string& outputDir,
                                 const std::vector< std::vector< double > >& angleCombinations) = 0;


            virtual void evalMa(const std::string& outputDir,
                                const std::map< std::string,
                                std::vector< std::string > >& musclesConnectedToDofs,
                                const std::vector< std::vector< double > >& angleCombinations) = 0;

            virtual void evalMaDerivative(const std::string& outputDir,
                                  const std::map< std::string, std::vector< std::string > >& musclesConnectedToDofs,
                                  const std::vector< std::vector< double > >& angleCombinations) = 0;

            virtual void exportCoefficients(const std::string& outDir) = 0;
    };
}
#endif //SPLINE_SET_BASE_H
