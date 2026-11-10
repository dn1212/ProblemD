#include <iostream>

#include "parser.h"
#include "printer.h"
#include "editor.h"
#include "optimizer.h"

using namespace std;


int main(){

    Parser parser;
    Editor editor;
    Optimizer optimizer;


    parser.readNode();
    parser.readBuffer();
    parser.readPath();


    auto rootIt =
        parser.nodeMap.find(
            "ROOT_CLK"
        );


    if(rootIt ==
       parser.nodeMap.end()){

        cout << "ROOT_CLK not found"
             << endl;

        return 1;
    }


    Node* root =
        rootIt->second;


    root->clkDelaySS = 0.0;
    root->clkDelayFF = 0.0;


    updateClockDelay(
        root,
        parser.bufferMap
    );


    TimingResult originalResult;


    update_result(
        parser.nodes,
        parser.buffers,
        parser.paths,
        parser.bufferMap,
        originalResult
    );


    cout << "===== Original Result ====="
         << endl;


    cout << "TNS SS: "
         << originalResult.tns_ss
         << endl;


    cout << "WNS SS: "
         << originalResult.wns_ss
         << endl;


    cout << "TNS FF: "
         << originalResult.tns_ff
         << endl;


    cout << "WNS FF: "
         << originalResult.wns_ff
         << endl;


    cout << "Area: "
         << originalResult.area
         << endl;


    cout << "\n===== Start Optimization ====="
         << endl;


    optimizer.optimize(
        parser,
        editor,
        originalResult
    );


    root->clkDelaySS = 0.0;
    root->clkDelayFF = 0.0;


    updateClockDelay(
        root,
        parser.bufferMap
    );


    TimingResult finalResult;


    update_result(
        parser.nodes,
        parser.buffers,
        parser.paths,
        parser.bufferMap,
        finalResult
    );


    cout << "\n===== Final Result ====="
         << endl;


    cout << "TNS SS: "
         << finalResult.tns_ss
         << endl;


    cout << "WNS SS: "
         << finalResult.wns_ss
         << endl;


    cout << "TNS FF: "
         << finalResult.tns_ff
         << endl;


    cout << "WNS FF: "
         << finalResult.wns_ff
         << endl;


    cout << "Area: "
         << finalResult.area
         << endl;


    cout << "\n===== Difference ====="
         << endl;


    cout << "TNS SS change: "
         << finalResult.tns_ss
            - originalResult.tns_ss
         << endl;


    cout << "WNS SS change: "
         << finalResult.wns_ss
            - originalResult.wns_ss
         << endl;


    cout << "TNS FF change: "
         << finalResult.tns_ff
            - originalResult.tns_ff
         << endl;


    cout << "WNS FF change: "
         << finalResult.wns_ff
            - originalResult.wns_ff
         << endl;


    cout << "Area change: "
         << finalResult.area
            - originalResult.area
         << endl;


    cout << "\n===== Score ====="
         << endl;


    print_score(
        finalResult,
        originalResult
    );


    return 0;
}