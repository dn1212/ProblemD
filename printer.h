#ifndef PRINTER_H
#define PRINTER_H

#include <iostream>
#include <vector>
#include <unordered_map>

#include "parser.h"

using namespace std;


struct TimingResult {
    double tns_ss = 0.0;
    double wns_ss = 0.0;

    double tns_ff = 0.0;
    double wns_ff = 0.0;

    double area = 0.0;
};


void update_result(
    const vector<Node*>& nodes,
    const vector<Buffer>& buffers,
    const vector<Path*>& paths,
    const unordered_map<string, Buffer>& bufferMap,
    TimingResult& result
);


double calculate_score(
    const TimingResult& result,
    const TimingResult& originalResult
);


void print_score(
    const TimingResult& result,
    const TimingResult& originalResult
);


void updateClockDelay(
    Node* node,
    unordered_map<string, Buffer>& bufferLib
);


#endif