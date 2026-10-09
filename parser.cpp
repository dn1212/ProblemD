#include "parser.h"
#include <fstream>
#include <sstream>
using namespace std;

void Parser::readBuffer() {
    // Implementation for parsing buffer logic
    return;
}
void Parser::readNode(){

    // open structure file
    ifstream clkTree;
    clkTree.open("D_testcase/testcase/testcase0/clk_tree.structure");
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
    return;
}
void Parser::readPath() {
    // FF_delay.rpt
    std::ifstream ffdelay;
    ffdelay.open("D_testcase/testcase/testcase0/FF_delay.rpt");

    if (ffdelay.fail()) {
        cout << "fail open FF_delay.rpt";
        exit(1);
    }

    string line;

    // clock period
    getline(ffdelay, line);

    string temp1, temp2;
    char colon;
    double clock_period;

    stringstream ffClock(line);

    ffClock >> temp1 >> temp2 >> colon >> clock_period;

    cout << "Clock period = " << clock_period << endl;

    // skip header
    getline(ffdelay, line);
    getline(ffdelay, line);

    // read path
    while (getline(ffdelay, line)) {
        
        if (line.empty())
            continue;

        string pathName;
        string launchName;
        string captureName;
        string arrow;
        char colon;
        double FF_delay;

        stringstream ss(line);

        ss >> pathName
           >> colon
           >> launchName
           >> arrow
           >> captureName
           >> FF_delay;

        cout << pathName << " "
             << launchName << " "
             << captureName << " "
             << FF_delay << endl;

        // find launch and capture nodes
        Node* launchNode = nullptr;
        Node* captureNode = nullptr;

        for (Node* node : nodes) {

            if (node->name == launchName)
                launchNode = node;

            if (node->name == captureName)
                captureNode = node;

            if (launchNode && captureNode)
                break;
        }

        // create Path object
        if (launchNode && captureNode) {

            Path* path = new Path;

            path->launch = launchNode;
            path->capture = captureNode;
            path->FF_delay = FF_delay;

  
            paths.push_back(path);

        } else {

            cout << "Error: Launch or Capture node not found for path: "
                 << launchName << " -> "
                 << captureName << endl;
        }
    }

    ffdelay.close();

    // SS_delay.rpt
    std::ifstream ssdelay;
    ssdelay.open("D_testcase/testcase/testcase0/SS_delay.rpt");

    if (ssdelay.fail()) {
        cout << "fail open SS_delay.rpt";
        exit(1);
    }

    // clock period
    getline(ssdelay, line);

    stringstream ssClock(line);

    ssClock >> temp1 >> temp2 >> colon >> clock_period;

    cout << "Clock period = " << clock_period << endl;

    // skip header
    getline(ssdelay, line);
    getline(ssdelay, line);
    // read path
    while (getline(ssdelay, line)) {
        if (line.empty())
            continue;

        string pathName;
        string launchName;
        string captureName;
        string arrow;
        char colon;
        double SS_delay;

        stringstream ss(line);

        ss >> pathName
           >> colon
           >> launchName
           >> arrow
           >> captureName
           >> SS_delay;

        cout << pathName << " "
             << launchName << " "
             << captureName << " "
             << SS_delay << endl;

        // find launch and capture nodes
        Node* launchNode = nullptr;
        Node* captureNode = nullptr;

        for (Node* node : nodes) {

            if (node->name == launchName)
                launchNode = node;

            if (node->name == captureName)
                captureNode = node;

            if (launchNode && captureNode)
                break;
        }

        // create Path object
        bool pathExists = false;
        if (launchNode && captureNode) {
            for (Path* path : paths) {
                if (path->launch == launchNode && path->capture == captureNode) {
                    path->SS_delay = SS_delay; 
                    pathExists = true;
                    break;
                }
            }
            if(!pathExists) {
                Path* path = new Path;

                path->launch = launchNode;
                path->capture = captureNode;
                path->SS_delay = SS_delay;
                paths.push_back(path);
            }
            

        } else {

            cout << "Error: Launch or Capture node not found for path: "
                 << launchName << " -> "
                 << captureName << endl;
        }
    }

    ssdelay.close();
}
