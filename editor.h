#ifndef EDITOR_H
#define EDITOR_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

class Editor{
public:
    void insertBuffer();
    void resizeBuffer();
};

#endif // EDITOR_H