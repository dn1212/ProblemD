#include <iostream>
#include "parser.h"

using namespace std;

int main() {
    Parser parser;

    // 先讀 node，因為 readPath 會去 nodes 裡找 launch / capture
    parser.readNode();

    // 讀 buffer library
    parser.readBuffer();

    // 再讀 FF / SS path
    parser.readPath();

    cout << "\n===== Nodes =====" << endl;
    cout << "Total nodes: " << parser.nodes.size() << endl;

    for (Node* node : parser.nodes) {
        cout << node->name
             << "  type=" << node->nodeType
             << "  cell=" << node->cellType
             << endl;
    }

    cout << "\n===== Buffers =====" << endl;
    cout << "Total buffers: " << parser.buffers.size() << endl;

    for (const Buffer& buffer : parser.buffers) {
        cout << "name=" << buffer.name
             << " | width=" << buffer.width
             << " | height=" << buffer.height
             << " | fanoutMax=" << buffer.fanoutMax
             << endl;

        cout << "  SS_delay: ";
        for (double delay : buffer.SS_delay) {
            cout << delay << " ";
        }
        cout << endl;

        cout << "  FF_delay: ";
        for (double delay : buffer.FF_delay) {
            cout << delay << " ";
        }
        cout << endl;
    }

    cout << "\n===== Paths =====" << endl;
    cout << "Total paths: " << parser.paths.size() << endl;

    for (Path* path : parser.paths) {
        cout << path->launch->name
             << " -> "
             << path->capture->name
             << " | FF_delay="
             << path->FF_delay
             << " | SS_delay="
             << path->SS_delay
             << endl;
    }

    return 0;
}