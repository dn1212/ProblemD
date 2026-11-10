#include "optimizer.h"

using namespace std;


void Optimizer::optimize(
    Parser& parser,
    Editor& editor,
    const TimingResult& originalResult
){
    auto rootIt =
        parser.nodeMap.find("ROOT_CLK");

    if(rootIt == parser.nodeMap.end()){
        cout << "ROOT_CLK not found" << endl;
        return;
    }

    Node* root =
        rootIt->second;


    TimingResult currentResult;


    root->clkDelaySS = 0.0;
    root->clkDelayFF = 0.0;

    updateClockDelay(
        root,
        parser.bufferMap
    );


    update_result(
        parser.nodes,
        parser.buffers,
        parser.paths,
        parser.bufferMap,
        currentResult
    );


    double currentScore =
        calculate_score(
            currentResult,
            originalResult
        );


    int maxRound = 10;


    for(int round = 0;
        round < maxRound;
        round++){

        cout << "\n===== Round "
             << round + 1
             << " ====="
             << endl;


        cout << "Current Score: "
             << currentScore
             << endl;


        Candidate best =
            findBestResize(
                parser,
                editor,
                originalResult,
                currentScore
            );


        if(best.node == nullptr){

            cout << "No better resize found."
                 << endl;

            break;
        }


        cout << "Apply: "
             << best.node->name
             << " "
             << best.oldCellType
             << " -> "
             << best.newCellType
             << endl;


        cout << "New Score: "
             << best.score
             << endl;


        editor.resizeBuffer(
            best.node,
            best.newCellType
        );


        root->clkDelaySS = 0.0;
        root->clkDelayFF = 0.0;


        updateClockDelay(
            root,
            parser.bufferMap
        );


        update_result(
            parser.nodes,
            parser.buffers,
            parser.paths,
            parser.bufferMap,
            currentResult
        );


        currentScore =
            calculate_score(
                currentResult,
                originalResult
            );


        cout << "TNS SS: "
             << currentResult.tns_ss
             << endl;

        cout << "WNS SS: "
             << currentResult.wns_ss
             << endl;

        cout << "TNS FF: "
             << currentResult.tns_ff
             << endl;

        cout << "WNS FF: "
             << currentResult.wns_ff
             << endl;

        cout << "Area: "
             << currentResult.area
             << endl;
    }
}


Candidate Optimizer::findBestResize(
    Parser& parser,
    Editor& editor,
    const TimingResult& originalResult,
    double currentScore
){
    Candidate bestCandidate;

    double bestScore =
        currentScore;


    auto rootIt =
        parser.nodeMap.find("ROOT_CLK");

    if(rootIt == parser.nodeMap.end()){
        return bestCandidate;
    }


    Node* root =
        rootIt->second;


    vector<Node*> candidates =
        getCriticalBuffers(
            parser,
            200
        );


    cout << "Critical Buffer Candidates: "
         << candidates.size()
         << endl;


    for(Node* node : candidates){

        if(node == nullptr ||
           !node->isBuffer){
            continue;
        }


        string oldCellType =
            node->cellType;


        int currentIndex = -1;


        for(int i = 0;
            i < static_cast<int>(
                parser.buffers.size()
            );
            i++){

            if(parser.buffers[i].name ==
               oldCellType){

                currentIndex = i;

                break;
            }
        }


        if(currentIndex == -1){
            continue;
        }


        vector<int> tryIndex;


        if(currentIndex > 0){

            tryIndex.push_back(
                currentIndex - 1
            );
        }


        if(currentIndex + 1 <
           static_cast<int>(
               parser.buffers.size()
           )){

            tryIndex.push_back(
                currentIndex + 1
            );
        }


        int fanout =
            static_cast<int>(
                node->children.size()
            );


        for(int index : tryIndex){

            const Buffer& buffer =
                parser.buffers[index];


            if(fanout <= 0){
                continue;
            }


            if(fanout >
               buffer.fanoutMax){

                continue;
            }


            editor.resizeBuffer(
                node,
                buffer.name
            );


            root->clkDelaySS = 0.0;
            root->clkDelayFF = 0.0;


            updateClockDelay(
                root,
                parser.bufferMap
            );


            TimingResult testResult;


            update_result(
                parser.nodes,
                parser.buffers,
                parser.paths,
                parser.bufferMap,
                testResult
            );


            double score =
                calculate_score(
                    testResult,
                    originalResult
                );


            if(score >
               bestScore + 1e-12){

                bestScore =
                    score;


                bestCandidate.node =
                    node;


                bestCandidate.oldCellType =
                    oldCellType;


                bestCandidate.newCellType =
                    buffer.name;


                bestCandidate.score =
                    score;
            }


            editor.resizeBuffer(
                node,
                oldCellType
            );
        }
    }


    root->clkDelaySS = 0.0;
    root->clkDelayFF = 0.0;


    updateClockDelay(
        root,
        parser.bufferMap
    );


    return bestCandidate;
}


vector<Path*> Optimizer::getCriticalPaths(
    Parser& parser,
    int topK
){
    vector<pair<double, Path*>>
        pathSlack;


    for(Path* path : parser.paths){

        if(path == nullptr ||
           path->launch == nullptr ||
           path->capture == nullptr){

            continue;
        }


        double t_clk =
            path->clockPeriod;


        double t_setup =
            t_clk * 0.08;


        double skew =
            path->capture->clkDelaySS
            -
            path->launch->clkDelaySS;


        double slack =
            t_clk
            -
            t_setup
            -
            path->SS_delay
            +
            skew;


        if(slack < 0){

            pathSlack.push_back(
                {
                    slack,
                    path
                }
            );
        }
    }


    sort(
        pathSlack.begin(),
        pathSlack.end(),

        [](const auto& a,
           const auto& b){

            return a.first <
                   b.first;
        }
    );


    vector<Path*>
        criticalPaths;


    int count =
        min(
            topK,
            static_cast<int>(
                pathSlack.size()
            )
        );


    for(int i = 0;
        i < count;
        i++){

        criticalPaths.push_back(
            pathSlack[i].second
        );
    }


    return criticalPaths;
}


vector<Node*> Optimizer::getCriticalBuffers(
    Parser& parser,
    int topK
){
    vector<Path*> criticalPaths =
        getCriticalPaths(
            parser,
            topK
        );


    unordered_set<Node*>
        candidateSet;


    for(Path* path :
        criticalPaths){

        Node* lca =
            findLCA(
                path->launch,
                path->capture
            );


        if(lca == nullptr){
            continue;
        }


        Node* cur =
            path->launch->parent;


        while(cur != nullptr &&
              cur != lca){

            if(cur->isBuffer){

                candidateSet.insert(
                    cur
                );
            }


            cur =
                cur->parent;
        }


        cur =
            path->capture->parent;


        while(cur != nullptr &&
              cur != lca){

            if(cur->isBuffer){

                candidateSet.insert(
                    cur
                );
            }


            cur =
                cur->parent;
        }
    }


    vector<Node*> candidates;


    for(Node* node :
        candidateSet){

        candidates.push_back(
            node
        );
    }


    return candidates;
}


Node* Optimizer::findLCA(
    Node* a,
    Node* b
){
    if(a == nullptr ||
       b == nullptr){

        return nullptr;
    }


    unordered_set<Node*>
        ancestors;


    Node* cur = a;


    while(cur != nullptr){

        ancestors.insert(
            cur
        );

        cur =
            cur->parent;
    }


    cur = b;


    while(cur != nullptr){

        if(ancestors.find(cur)
           != ancestors.end()){

            return cur;
        }


        cur =
            cur->parent;
    }


    return nullptr;
}