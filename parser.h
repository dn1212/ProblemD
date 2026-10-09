#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Node {
public:
    string name;
    string nodeType;
    string cellType;
    Node* parent = nullptr;
    vector<Node*> children;
};

class Buffer {
public:
    string name;
    double width;
    double height;
    int fanout_max;
    vector<double> SS_delay;
    vector<double> FF_delay;
};

class Path {
public:
    Node* launch = nullptr;
    Node* capture = nullptr;
    double SS_delay;
    double FF_delay;
};

class Parser{
public:
    // node
    vector<Node*> nodes;
    void readNode();
    
    // buffer
    
    void readBuffer();
    
    //path
    void readPath();
};

#endif // PARSER_H