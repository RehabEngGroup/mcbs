#include "ReadMuscles.h"
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;
#include <stdlib.h>

void readMuscles(const string& musclesFilename, vector<string>& musclesNames) {
    ifstream musclesFile(musclesFilename.c_str());
    if (!musclesFile) {
        cout << "Error: file " << musclesFilename << " could not be opened\n";
        exit(EXIT_FAILURE);
    }
    string line;

    while(std::getline(musclesFile, line)) {
        std::istringstream iss(line);
        string nextMuscle;
        iss >> nextMuscle;
        musclesNames.push_back(nextMuscle);
    }
    musclesFile.close();
}

void readMuscles(const string& musclesFilename, vector<string>& musclesNames, map<string, vector<string> >& musclesConnectedToDofs) {
    ifstream musclesFile(musclesFilename.c_str());
    if (!musclesFile) {
        cout << "Error: file " << musclesFilename << " could not be opened\n";
        exit(EXIT_FAILURE);
    }
    string line;
    while(std::getline(musclesFile, line)) {
        std::istringstream iss(line);
        string nextMuscle;
        iss >> nextMuscle;
        musclesNames.push_back(nextMuscle);
        string nextDof;
        while (iss >> nextDof && nextDof!="") {
            musclesConnectedToDofs[nextDof].push_back(nextMuscle);
        }
    }
    musclesFile.close();
}
