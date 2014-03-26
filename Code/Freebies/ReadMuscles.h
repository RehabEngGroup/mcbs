#ifndef READMUSCLES_H
#define READMUSCLES_H

#include <string>
#include <vector>
#include <map>

/*! Read the muscle names from the provided file
    \param [in] musclesFilename list the name of the muscles we are interested in
                the file format is just the list of muscle names (one for each row)
                Do not use OpenSim muscle names. Use the name required by your applications
    \param [out] musclesNames a vector with the name of the muscles, ready to be used
                 in our applications
 */
void readMuscles(const std::string& musclesFilename, std::vector<std::string>& musclesNames);
void readMuscles(const std::string& musclesFilename, std::vector<std::string>& musclesNames, std::map<std::string, std::vector<std::string> >& musclesConnectedToDofs);
#endif
