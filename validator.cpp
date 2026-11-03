#include "validator.h"
using namespace std;

bool Validator::checkFanout(){

}

bool Validator::checkOriginalNode(){

}

bool Validator::checkNewBuffer(){

}

void Validator::output(Node* root){
    
    // output file
    ofstream outFile("modified_clk_tree.structure");
    if(outFile.fail()){
        cout << "fail output modified_clk_tree.structure";
        exit(1);
    }

    // root
    if(root == nullptr){
        cout << "root is nullptr";
        return;
    }
    outFile << "Root: " << root->name << endl;

    // node DFS
    vector<Node*> stack;
    vector<int> levels;

    stack.push_back(root);
    levels.push_back(0);

    while (!stack.empty()) {

        Node* node = stack.back();
        int level = levels.back();

        stack.pop_back();
        levels.pop_back();

        if(level != 0){
            for(int i = 0; i < level - 1; i++) outFile << "\t";
            outFile << "[" << level << "] " << node->name;
            outFile << " (" << node->cellType << ")";
            if (node->children.empty()) outFile << " (SINK)";
            outFile << endl;
        }

        for (int i = node->children.size() - 1; i >= 0; i--) {
            stack.push_back(node->children[i]);
            levels.push_back(level + 1);
        }
    }

    outFile.close();
    return;
}