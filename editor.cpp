#include "editor.h"
using namespace std;

void Editor::insertBuffer(Node* parent, Node* child, string cellType, vector<Node*>& nodes){
    
    if (parent == nullptr || child == nullptr) {
        cout << "Invalid node" << endl;
        return;
    }
    
    Node* newBuffer = new Node;
    newBuffer->name = "NEW_BUF_" + to_string(bufferCount);
    bufferCount++;

    newBuffer->nodeType = "BUF";
    newBuffer->isBuffer = true;
    newBuffer->isFF = false;
    newBuffer->cellType = cellType;
    newBuffer->parent = parent;
    newBuffer->children.push_back(child);

    child->parent = newBuffer;
    for(int i = 0; i < parent->children.size(); i++){
        if (parent->children[i] == child) {
            parent->children[i] = newBuffer;
            break;
        }
    }
    nodes.push_back(newBuffer);
}

void Editor::resizeBuffer(Node* node, string newCellType){
    if (node == nullptr) {
        cout << "Node is null" << endl;
        return;
    }

    node->cellType = newCellType;
}