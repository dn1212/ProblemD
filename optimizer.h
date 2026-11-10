#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>

#include "parser.h"
#include "editor.h"
#include "printer.h"

using namespace std;

struct Candidate {
    Node* node = nullptr;

    string oldCellType;
    string newCellType;

    double score = 0.0;
};

class Optimizer {
public:

    void optimize(
        Parser& parser,
        Editor& editor,
        const TimingResult& originalResult
    );

private:

    Candidate findBestResize(
        Parser& parser,
        Editor& editor,
        const TimingResult& originalResult,
        double currentScore
    );

    vector<Path*> getCriticalPaths(
        Parser& parser,
        int topK
    );

    vector<Node*> getCriticalBuffers(
        Parser& parser,
        int topK
    );

    Node* findLCA(
        Node* a,
        Node* b
    );
};

#endif