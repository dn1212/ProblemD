#include "parser.h"
using namespace std;

void Parser::readNode(){

    // open structure file
    ifstream clkTree;
    clkTree.open("D_testcase/testcase/testcase0/clk_tree.structure");

    if(clkTree.fail()){
        cout << "fail open clk_tree.structure";
        exit(1);
    }

    string line;

    // =========================
    // read root
    // =========================
    getline(clkTree, line);

    Node* root = new Node;

    root->name = "ROOT_CLK";
    root->nodeType = "ROOT";
    root->cellType = "";

    root->isBuffer = false;
    root->isFF = false;

    root->clkDelaySS = 0.0;
    root->clkDelayFF = 0.0;

    nodes.push_back(root);

    // name -> Node*
    nodeMap[root->name] = root;


    // =========================
    // parent information
    // =========================
    vector<Node*> lastNode;
    lastNode.push_back(root);


    // =========================
    // read nodes
    // =========================
    while(getline(clkTree, line)){

        if(line.empty())
            continue;

        // -------------------------
        // level
        // -------------------------
        size_t leftBracket = line.find('[');
        size_t rightBracket = line.find(']');

        string level_str =
            line.substr(
                leftBracket + 1,
                rightBracket - leftBracket - 1
            );

        int level = stoi(level_str);


        // -------------------------
        // name
        // -------------------------
        size_t nameStart =
            line.find_first_not_of(' ', rightBracket + 1);

        size_t nameEnd =
            line.find(' ', nameStart);

        string name =
            line.substr(
                nameStart,
                nameEnd - nameStart
            );


        // -------------------------
        // nodeType
        //
        // BUF_0 -> BUF
        // FF_12 -> FF
        // -------------------------
        size_t underscore = name.find('_');

        string nodeType =
            name.substr(0, underscore);


        // -------------------------
        // cellType
        //
        // BUF_0 (REALBUF_X8)
        // -> REALBUF_X8
        // -------------------------
        size_t leftParen = line.find('(');
        size_t rightParen = line.find(')');

        string cellType =
            line.substr(
                leftParen + 1,
                rightParen - leftParen - 1
            );


        // =========================
        // create Node
        // =========================
        Node* node = new Node;

        node->name = name;
        node->nodeType = nodeType;
        node->cellType = cellType;


        // FF / Buffer
        if(nodeType == "FF"){
            node->isFF = true;
            node->isBuffer = false;
        }
        else if(nodeType == "BUF"){
            node->isBuffer = true;
            node->isFF = false;
        }


        // vector
        nodes.push_back(node);
        // unordered_map
        // name -> Node*
        nodeMap[node->name] = node;


        // =========================
        // parent
        // =========================
        if(level >= static_cast<int>(lastNode.size())){
            lastNode.resize(level + 1, nullptr);
        }

        Node* parent = lastNode[level - 1];

        node->parent = parent;
        parent->children.push_back(node);
        lastNode[level] = node;
    }


    clkTree.close();
}
void Parser::readPath() {

    // ============================
    // FF_delay.rpt
    // ============================
    ifstream ffdelay(
        "D_testcase/testcase/testcase0/FF_delay.rpt"
    );

    if (ffdelay.fail()) {
        cout << "fail open FF_delay.rpt" << endl;
        exit(1);
    }

    string line;

    string temp1, temp2;
    char colon;
    double ffClockPeriod;

    // ----------------------------
    // Clock Period
    // ----------------------------
    getline(ffdelay, line);

    stringstream ffClock(line);

    ffClock
        >> temp1
        >> temp2
        >> colon
        >> ffClockPeriod;


    // skip header
    getline(ffdelay, line);
    getline(ffdelay, line);


    // ============================
    // Read FF paths
    // ============================
    while (getline(ffdelay, line)) {

        if (line.empty())
            continue;

        string pathName;
        string launchName;
        string captureName;
        string arrow;

        char colon;
        double ffDelay;

        stringstream ss(line);

        ss >> pathName
           >> colon
           >> launchName
           >> arrow
           >> captureName
           >> ffDelay;


        // ----------------------------
        // O(1) 找 Node
        // ----------------------------
        auto launchIt =
            nodeMap.find(launchName);

        auto captureIt =
            nodeMap.find(captureName);


        if (launchIt == nodeMap.end() ||
            captureIt == nodeMap.end()) {

            cout
                << "Error: node not found: "
                << launchName
                << " -> "
                << captureName
                << endl;

            continue;
        }


        Node* launchNode =
            launchIt->second;

        Node* captureNode =
            captureIt->second;


        // ----------------------------
        // 建立 Path
        // ----------------------------
        Path* path = new Path;

        path->name = pathName;

        path->launch =
            launchNode;

        path->capture =
            captureNode;

        path->FF_delay =
            ffDelay;

        path->clockPeriod =
            ffClockPeriod;


        paths.push_back(path);

        // Path1 -> Path*
        pathMap[pathName] = path;
    }

    ffdelay.close();



    // ============================
    // SS_delay.rpt
    // ============================
    ifstream ssdelay(
        "D_testcase/testcase/testcase0/SS_delay.rpt"
    );

    if (ssdelay.fail()) {
        cout << "fail open SS_delay.rpt" << endl;
        exit(1);
    }


    double ssClockPeriod;

    // ----------------------------
    // Clock Period
    // ----------------------------
    getline(ssdelay, line);

    stringstream ssClock(line);

    ssClock
        >> temp1
        >> temp2
        >> colon
        >> ssClockPeriod;


    // skip header
    getline(ssdelay, line);
    getline(ssdelay, line);


    // ============================
    // Read SS paths
    // ============================
    while (getline(ssdelay, line)) {

        if (line.empty())
            continue;

        string pathName;
        string launchName;
        string captureName;
        string arrow;

        char colon;
        double ssDelay;


        stringstream ss(line);

        ss >> pathName
           >> colon
           >> launchName
           >> arrow
           >> captureName
           >> ssDelay;


        // ----------------------------
        // 直接用 pathName 找 Path
        // O(1)
        // ----------------------------
        auto pathIt =
            pathMap.find(pathName);


        if (pathIt != pathMap.end()) {

            Path* path =
                pathIt->second;

            path->SS_delay =
                ssDelay;

        }
        else {

            // 理論上 FF report 沒有的 path
            // 但 SS report 出現了
            // 還是可以建立它

            auto launchIt =
                nodeMap.find(launchName);

            auto captureIt =
                nodeMap.find(captureName);


            if (launchIt == nodeMap.end() ||
                captureIt == nodeMap.end()) {

                cout
                    << "Error: node not found: "
                    << launchName
                    << " -> "
                    << captureName
                    << endl;

                continue;
            }


            Path* path = new Path;

            path->name =
                pathName;

            path->launch =
                launchIt->second;

            path->capture =
                captureIt->second;

            path->SS_delay =
                ssDelay;

            path->clockPeriod =
                ssClockPeriod;


            paths.push_back(path);

            pathMap[pathName] =
                path;
        }
    }


    ssdelay.close();
}

void Parser::readBuffer(){
    ifstream bufLib;
    bufLib.open("D_testcase/testcase/testcase0/buf.lib");

    if(bufLib.fail()){
        cout << "fail open buf.lib";
        exit(1);
    }

    string line;

    while(getline(bufLib, line)){

        // 找到一個新的 cell
        if(line.find("cell (") == string::npos){
            continue;
        }

        Buffer buffer;

        // =========================
        // name
        // =========================
        size_t leftParen = line.find('(');
        size_t rightParen = line.find(')');

        buffer.name =
            line.substr(
                leftParen + 1,
                rightParen - leftParen - 1
            );

        // =========================
        // SIZE
        // =========================
        getline(bufLib, line);

        size_t sizeStart = line.find("SIZE") + 4;
        size_t byPos = line.find("BY");

        buffer.width =
            stod(line.substr(
                sizeStart,
                byPos - sizeStart
            ));

        buffer.height =
            stod(line.substr(byPos + 2));

        // =========================
        // SS_DELAY
        // =========================
        getline(bufLib, line);

        size_t pos =
            line.find("SS_DELAY") + 8;

        while(true){

            size_t start =
                line.find_first_not_of(
                    " \t",
                    pos
                );

            if(start == string::npos){
                break;
            }

            size_t end =
                line.find_first_of(
                    " \t",
                    start
                );

            buffer.SS_delay.push_back(
                stod(
                    line.substr(
                        start,
                        end - start
                    )
                )
            );

            if(end == string::npos){
                break;
            }

            pos = end;
        }

        // =========================
        // FF_DELAY
        // =========================
        getline(bufLib, line);

        pos =
            line.find("FF_DELAY") + 8;

        while(true){

            size_t start =
                line.find_first_not_of(
                    " \t",
                    pos
                );

            if(start == string::npos){
                break;
            }

            size_t end =
                line.find_first_of(
                    " \t",
                    start
                );

            buffer.FF_delay.push_back(
                stod(
                    line.substr(
                        start,
                        end - start
                    )
                )
            );

            if(end == string::npos){
                break;
            }

            pos = end;
        }

        // =========================
        // fanoutMax
        // =========================
        buffer.fanoutMax =
            static_cast<int>(
                buffer.SS_delay.size()
            );

        // =========================
        // save
        // =========================
        buffers.push_back(buffer);

        bufferMap[buffer.name] = buffer;
    }

    bufLib.close();
}