#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "parser.h"

using namespace std;

class Validator{
public:
    bool checkFanout();
    bool checkOriginalNode();
    bool checkNewBuffer();

    void output(Node* root);
};

#endif // VALIDATOR_H