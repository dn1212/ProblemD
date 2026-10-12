#include <iostream>
#include <algorithm>
#include "parser.h"

using namespace std;

int main() {
    Parser parser;

    // 讀取資料
    parser.readNode();
    parser.readBuffer();
    parser.readPath();

    const int testCount = 5;

    // =========================
    // Test Nodes
    // =========================
    cout << "\n===== Nodes =====" << endl;
    cout << "Total nodes: " << parser.nodes.size() << endl;

    for (int i = 0; i < 5; i++) {
        Node* node = parser.nodes[i];

        cout << "[" << i << "] "
             << node->name
             << " | type=" << node->nodeType
             << " | cell=" << node->cellType
             << endl;
    }

    // =========================
    // Test Buffers
    // =========================
    cout << "\n===== Buffers =====" << endl;
    cout << "Total buffers: " << parser.buffers.size() << endl;

    for (int i = 0; i < 5; i++) {
        Buffer& buffer = parser.buffers[i];

        cout << "[" << i << "] "
             << buffer.name
             << " | width=" << buffer.width
             << " | height=" << buffer.height
             << " | fanoutMax=" << buffer.fanoutMax
             << endl;

        cout << "    SS_delay: ";
        for (int j = 0;
             j < min(testCount, (int)buffer.SS_delay.size());
             j++) {
            cout << buffer.SS_delay[j] << " ";
        }
        cout << endl;

        cout << "    FF_delay: ";
        for (int j = 0;
             j < min(testCount, (int)buffer.FF_delay.size());
             j++) {
            cout << buffer.FF_delay[j] << " ";
        }
        cout << endl;
    }

    // =========================
    // Test Paths
    // =========================
    cout << "\n===== Paths =====" << endl;
    cout << "Total paths: " << parser.paths.size() << endl;

    for (int i = 0; i < 5; i++) {
        Path* path = parser.paths[i];

        cout << "[" << i << "] "
             << path->launch->name
             << " -> "
             << path->capture->name
             << " | FF_delay=" << path->FF_delay
             << " | SS_delay=" << path->SS_delay
             << endl;
    }

    return 0;
}