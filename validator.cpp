#include "validator.h"
using namespace std;

bool Validator::checkFanout(const vector<Node*>& nodes, const vector<Buffer>& buffers) {

    for (int i = 0; i < nodes.size(); i++) {
        
        Node* node = nodes[i];
        if (!node->isBuffer) continue;
        bool found = false;

        for (int j = 0; j < buffers.size(); j++) {

            if (node->cellType == buffers[j].name) {
                found = true;
                int fanout = node->children.size();

                if (fanout > buffers[j].fanoutMax || fanout == 0) {
                    cout << "Invalid fanout: " << node->name << endl;
                    return false;
                }

                break;
            }
        }

        if (!found) {
            cout << "Buffer type not found: " << node->cellType << endl;
            return false;
        }
    }
    return true;
}

bool Validator::checkNewBuffer(const vector<Node*>& nodes) {

    for (int i = 0; i < nodes.size(); i++) {

        string name = nodes[i]->name;

        if (name.substr(0, 8) != "NEW_BUF_") {
            continue;
        }

        if (!nodes[i]->isBuffer) {
            cout << "Invalid new buffer: " << name << endl;
            return false;
        }

        if (name.size() == 8) {
            cout << "Invalid buffer name: " << name << endl;
            return false;
        }

        for (int j = 8; j < name.size(); j++) {
            if (name[j] < '0' || name[j] > '9') {
                cout << "Invalid buffer name: " << name << endl;
                return false;
            }
        }

        for (int j = i + 1; j < nodes.size(); j++) {
            if (name == nodes[j]->name) {
                cout << "Duplicate buffer name: " << name << endl;
                return false;
            }
        }
    }
    return true;
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

bool Validator::checkOriginalNode() {
    ifstream original("D_testcase/testcase/testcase0/clk_tree.structure");
    ifstream modified("modified_clk_tree.structure");

    if (!original.is_open() || !modified.is_open()) {
        cout << "Cannot open file" << endl;
        return false;
    }

    string line1, line2;
    string name1, name2;

    while (getline(original, line1)) {

        stringstream ss1(line1);
        ss1 >> name1;

        if (name1 == "Root:") {
            ss1 >> name1;
        }
        else {
            ss1 >> name1;
        }

        // find next not NEW_BUF node
        do {
            if (!getline(modified, line2)) {
                return false;
            }

            stringstream ss2(line2);
            ss2 >> name2;

            if (name2 == "Root:") {
                ss2 >> name2;
            }
            else {
                ss2 >> name2;
            }

        } while (name2.substr(0, 8) == "NEW_BUF_");

        if (name1 != name2) {
            cout << "Original node mismatch: "
                 << name1 << " != " << name2 << endl;
            return false;
        }
    }

    while (getline(modified, line2)) {
        stringstream ss(line2);
        ss >> name2 >> name2;

        if (name2.substr(0, 8) != "NEW_BUF_") {
            return false;
        }
    }

    return true;
}