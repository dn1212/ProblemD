#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;
class Node {
public:
    string name;
    string nodeType;
    string cellType;
    Node* parent = nullptr;
    vector<Node*> children;
    
    bool isBuffer = false;
    bool isFF = false;

    double clkDelaySS = 0.0;
    double clkDelayFF = 0.0;
};

class Buffer {
public:
    string name;
    double width;
    double height;
    int fanoutMax;
    vector<double> SS_delay;
    vector<double> FF_delay;
};

class Path {
public:
    string name;
    double clockPeriod = 0.0;
    Node* launch = nullptr;
    Node* capture = nullptr;
    double SS_delay = 0.0;
    double FF_delay = 0.0;
};

class Parser{
public:
    // node
    vector<Node*> nodes;
    unordered_map<string, Node*> nodeMap;
    void readNode();
    // buffer
    vector<Buffer> buffers;
    unordered_map<string, Buffer> bufferMap;
    void readBuffer();
    //path
    vector<Path*> paths;
    unordered_map<string, Path*> pathMap;
    void readPath();
};

#endif // PARSER_H