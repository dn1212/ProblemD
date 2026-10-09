#include "parser.h"
using namespace std;

void Parser::readNode(){

    // open structure file
    ifstream clkTree;
    clkTree.open("testcase/testcase0/clk_tree.structure");
    if(clkTree.fail()){
        cout << "fail open clk_tree.structure";
        exit(1);
    }
    
    string line;
    
    // read root
    getline(clkTree, line);
    Node* root = new Node;
    root->name = "ROOT_CLK";
    root->nodeType = "ROOT";
    root->cellType = "";
    nodes.push_back(root);
    
    // parent
    vector<Node*> lastNode;
    lastNode.push_back(root);
    
    // read nodes
    while(getline(clkTree, line)){

        // level
        size_t leftBracket = line.find('[');
        size_t rightBracket = line.find(']');
        string level_str = line.substr(leftBracket + 1, rightBracket - leftBracket - 1);
        int level = stoi(level_str);

        // name
        size_t nameStart = line.find_first_not_of(' ', rightBracket + 1);
        size_t nameEnd = line.find(' ', nameStart);
        string name = line.substr(nameStart, nameEnd - nameStart);

        // nodeType
        size_t underscore = name.find('_');
        string nodeType = name.substr(0, underscore);

        // cellType
        size_t leftParen = line.find('(');
        size_t rightParen = line.find(')');
        string cellType = line.substr(leftParen + 1, rightParen - leftParen - 1);

        // new Node
        Node* node = new Node;
        node->name = name;
        node->nodeType = nodeType;
        node->cellType = cellType;
        nodes.push_back(node);

        // parent
        if (level >= static_cast<int>(lastNode.size())) {
            lastNode.resize(level + 1, nullptr);
        }
        Node* parent = lastNode[level - 1];
        node->parent = parent;
        parent->children.push_back(node);
        lastNode[level] = node;
    }

    clkTree.close();
}

void Parser::readBuffer(){

}

int main() {
    Parser parser;
    parser.readNode();

    /* TEST NODE
    // Test 1: Total number of nodes
    cout << "Total nodes: "
         << parser.nodes.size() << endl;

    // Test 2: Print first 10 nodes
    for (int i = 0; i < 10 && i < parser.nodes.size(); i++) {

        Node* node = parser.nodes[i];

        cout << "------------------" << endl;
        cout << "Name: " << node->name << endl;
        cout << "Node Type: " << node->nodeType << endl;
        cout << "Cell Type: " << node->cellType << endl;

        // Parent
        if (node->parent != nullptr) {
            cout << "Parent: " << node->parent->name << endl;
        }
        else {
            cout << "Parent: None" << endl;
        }

        // Children
        cout << "Children: ";
        for (Node* child : node->children) {
            cout << child->name << " ";
        }
        cout << endl;
    }

    // Free allocated nodes
    for (Node* node : parser.nodes) {
        delete node;
    }
    */

    return 0;
}