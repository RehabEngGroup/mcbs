

#include <memory>
using std::shared_ptr;
#include <functional>
#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <fstream>
using std::ofstream;
using std::ifstream;
#include <cstdlib>
#include "mcbs/MusclesCfg.h"
#include "mcbs/DofCfg.h"
#include "mcbs/SplineSetBase.h"
#include "mcbs/SplineSetCreator.h"
using namespace mcbs;

#include "LmtMaFromOpenSim.h"



int main(int argc, const char* argv[])
{

    if ( argc != 5 ) {
        cout << "Usage: osimSpliner <inputModel>.osim <dofAngles>.cfg <muscles>.in outputDirectory\n";
        exit(EXIT_FAILURE);
    }

    string inputModel = argv[1];
    string outputDir = argv[4];

    //-- read the muscles we want in the grid
    string musclesFilename = argv[3];
    MusclesCfg muscCfg(musclesFilename);
    const vector<string> &musclesNames=muscCfg.getMuscleNames();

    //-- create the angles combinations
    string configFilename = argv[2];
    DofCfg cfg(configFilename);


    //-- setup the grid generator
    string osimModelFilename = inputModel;
    LmtMaFromOpenSim lmtGrid(osimModelFilename,cfg.getDofNames(), musclesNames, cfg.getAngleCombinations());
    std::vector<std::vector<double> > lmts;
    lmtGrid.getGrid(lmts);

    shared_ptr<SplineSetBase> splineSet = SplineSetCreator::makeSplineSet(cfg.getDofNames().size(), cfg.getDofNames(), musclesNames, cfg.getAnglesStartValue(), cfg.getAnglesStopValue(), cfg.getNoSteps(), lmts);
    splineSet->exportCoefficients(outputDir);

    std::string newDofCfgFilename=outputDir + "/dofAngles.cfg";
    ofstream newDofCfg(newDofCfgFilename);
    if (!newDofCfg.is_open()) {
        cout << "ERROR: " << newDofCfgFilename << " output file could not be opened\n";
        exit(EXIT_FAILURE);
    }
    newDofCfg << cfg;
    newDofCfg.close();
    return 0;
}

