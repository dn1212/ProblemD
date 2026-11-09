#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <iostream>
#include <vector>
#include "parser.h"

using namespace std;

class Optimizer{
public:
    void runSA(int iterations, double initialTemp, double coolingRate);
};

#endif // OPTIMIZER_H