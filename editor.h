#ifndef EDITOR_H
#define EDITOR_H

#include <iostream>
#include <vector>
#include <string>
#include "parser.h"

using namespace std;

class Editor{
public:
    int bufferCount = 0;
    void insertBuffer(Node* parent, Node* child, string cellType, vector<Node*>& nodes);
    void resizeBuffer(Node* node, string newCellType);
};

#endif // EDITOR_H