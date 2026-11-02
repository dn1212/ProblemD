#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

class Validator{
public:
    bool checkFanout();
    bool checkOriginalNode();
    bool checkNewBuffer();
};

#endif // VALIDATOR_H