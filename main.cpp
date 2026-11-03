#include <iostream>
#include <algorithm>
#include "parser.h"
#include "validator.h"
#include "printer.h"

using namespace std;

int main() {

    Parser parser;

    parser.readNode();
    parser.readBuffer();
    parser.readPath();

    cout << "===== Parser Finished =====" << endl;

    Node* root = parser.nodeMap["ROOT_CLK"];

    root->clkDelaySS = 0.0;
    root->clkDelayFF = 0.0;

    updateClockDelay(
        root,
        parser.bufferMap
    );

    cout << "\n===== Clock Delay =====" << endl;

    for (Node* node : parser.nodes) {

        if (node->isFF) {
            cout << node->name
                 << " SS Clock Delay: "
                 << node->clkDelaySS
                 << ", FF Clock Delay: "
                 << node->clkDelayFF
                 << endl;
        }
    }

    cout << "\n===== Path Skew =====" << endl;

    for (Path* path : parser.paths) {

        double skew_ss =
            path->capture->clkDelaySS
            - path->launch->clkDelaySS;

        double skew_ff =
            path->capture->clkDelayFF
            - path->launch->clkDelayFF;

        cout << path->launch->name
             << " -> "
             << path->capture->name
             << endl;

        cout << "SS skew: "
             << skew_ss
             << endl;

        cout << "FF skew: "
             << skew_ff
             << endl;
    }

    TimingResult result;

    cout << "\n===== Timing Result =====" << endl;

    update_result(
        parser.nodes,
        parser.buffers,
        parser.paths,
        result
    );

    cout << "\n===== Final Result =====" << endl;

    cout << "TNS SS: "
         << result.tns_ss
         << endl;

    cout << "WNS SS: "
         << result.wns_ss
         << endl;

    cout << "TNS FF: "
         << result.tns_ff
         << endl;

    cout << "WNS FF: "
         << result.wns_ff
         << endl;

    cout << "Area: "
         << result.area
         << endl;

    return 0;
}