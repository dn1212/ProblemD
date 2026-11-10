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
    bool checkFanout(const vector<Node*>& nodes, const vector<Buffer>& buffers);
    
    bool checkNewBuffer(const vector<Node*>& nodes);

    void output(Node* root);

    bool checkOriginalNode();
};

#endif // VALIDATOR_H