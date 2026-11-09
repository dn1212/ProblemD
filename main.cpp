#include <iostream>
#include "parser.h"
#include "printer.h"
#include "editor.h"

using namespace std;

int main(){

    Parser parser;
    Editor editor;

    parser.readNode();
    parser.readBuffer();
    parser.readPath();

    Node* root = parser.nodeMap["ROOT_CLK"];

    root->clkDelaySS = 0.0;
    root->clkDelayFF = 0.0;

    updateClockDelay(
        root,
        parser.bufferMap
    );

    TimingResult originalResult;

    cout << "===== Original Result =====" << endl;

    update_result(
        parser.nodes,
        parser.buffers,
        parser.paths,
        parser.bufferMap,
        originalResult
    );

    cout << "\n===== Original Timing =====" << endl;

    cout << "TNS SS: "
         << originalResult.tns_ss << endl;

    cout << "WNS SS: "
         << originalResult.wns_ss << endl;

    cout << "TNS FF: "
         << originalResult.tns_ff << endl;

    cout << "WNS FF: "
         << originalResult.wns_ff << endl;

    cout << "Area: "
         << originalResult.area << endl;


    Node* testParent = nullptr;
    Node* testChild = nullptr;

    for(Node* node : parser.nodes){

        if(node->isBuffer &&
           !node->children.empty()){

            testParent = node;
            testChild = node->children[0];

            break;
        }
    }

    if(testParent == nullptr ||
       testChild == nullptr){

        cout << "Cannot find test edge" << endl;
        return 1;
    }


    cout << "\n===== Insert Buffer =====" << endl;

    cout << "Before:" << endl;

    cout << testParent->name
         << " -> "
         << testChild->name
         << endl;


    editor.insertBuffer(
        testParent,
        testChild,
        "REALBUF_X4",
        parser.nodes
    );


    Node* newBuffer =
        testChild->parent;


    cout << "\nAfter:" << endl;

    cout << testParent->name
         << " -> "
         << newBuffer->name
         << " -> "
         << testChild->name
         << endl;

    cout << "New Buffer Type: "
         << newBuffer->cellType
         << endl;


    root->clkDelaySS = 0.0;
    root->clkDelayFF = 0.0;

    updateClockDelay(
        root,
        parser.bufferMap
    );


    TimingResult newResult;

    cout << "\n===== Result After Insert =====" << endl;

    update_result(
        parser.nodes,
        parser.buffers,
        parser.paths,
        parser.bufferMap,
        newResult
    );


    cout << "\n===== New Timing =====" << endl;

    cout << "TNS SS: "
         << newResult.tns_ss << endl;

    cout << "WNS SS: "
         << newResult.wns_ss << endl;

    cout << "TNS FF: "
         << newResult.tns_ff << endl;

    cout << "WNS FF: "
         << newResult.wns_ff << endl;

    cout << "Area: "
         << newResult.area << endl;


    cout << "\n===== Difference =====" << endl;

    cout << "TNS SS change: "
         << newResult.tns_ss
            - originalResult.tns_ss
         << endl;

    cout << "WNS SS change: "
         << newResult.wns_ss
            - originalResult.wns_ss
         << endl;

    cout << "TNS FF change: "
         << newResult.tns_ff
            - originalResult.tns_ff
         << endl;

    cout << "WNS FF change: "
         << newResult.wns_ff
            - originalResult.wns_ff
         << endl;

    cout << "Area change: "
         << newResult.area
            - originalResult.area
         << endl;


    cout << "\n===== Score =====" << endl;

    print_score(
        newResult.tns_ss,
        originalResult.tns_ss,

        newResult.wns_ss,
        originalResult.wns_ss,

        newResult.tns_ff,
        originalResult.tns_ff,

        newResult.wns_ff,
        originalResult.wns_ff,

        newResult.area,
        originalResult.area
    );

    return 0;
}