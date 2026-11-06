# include "printer.h"

void update_result(
    const vector<Node*>& nodes,
    const vector<Buffer>& buffers,
    const vector<Path*>& paths,
    const unordered_map<string, Buffer>& bufferMap,
    TimingResult& result
){
    result.tns_ss = 0.0;
    result.wns_ss = 0.0;
    result.tns_ff = 0.0;
    result.wns_ff = 0.0;
    result.area = 0.0;
    for (Node* node : nodes) {

        if (node->isBuffer) {

            auto it = bufferMap.find(node->cellType);

            if (it != bufferMap.end()) {

                result.area +=
                    it->second.width *
                    it->second.height;
            }
        }
    }
    // test 
    int setupViolation = 0;
    int holdViolation = 0;
    Path* worstSetupPath = nullptr;
    Path* worstHoldPath = nullptr;

    // cout << "Setup WNS/TNS (SS corner):" << endl;
    for (const auto& path : paths) {
        // cout << "Path: " << path->launch->name << " -> " << path->capture->name
        //      << ", SS Delay: " << path->SS_delay << endl;

        double t_clk = path->clockPeriod;
        double t_setup = t_clk * 0.08;
        double d_data = path->SS_delay;

        double d_launch = path->launch->clkDelaySS;
        double d_capture = path->capture->clkDelaySS;
        double skew_ss = d_capture - d_launch;

        double slack_ss = t_clk - t_setup - d_data + skew_ss;

        if (slack_ss < 0) {
            result.tns_ss += slack_ss;
            setupViolation++;
        }

        if (slack_ss < result.wns_ss) {
            result.wns_ss = slack_ss;
            worstSetupPath = path;
        }
    }
    

    // cout << "\nHold WNS/TNS (FF corner):" << endl;
    for (const auto& path : paths) {
        // cout << "Path: " << path->launch->name << " -> " << path->capture->name
        //      << ", FF Delay: " << path->FF_delay << endl;

        double t_clk = path->clockPeriod;
        double t_hold = t_clk * 0.05;
        double d_data = path->FF_delay;

        double d_launch = path->launch->clkDelayFF;
        double d_capture = path->capture->clkDelayFF;
        double skew_ff = d_capture - d_launch;

        double slack_ff = d_data - t_hold - skew_ff;

        if (slack_ff < 0) {
            result.tns_ff += slack_ff;
            holdViolation++;
        }

        if (slack_ff < result.wns_ff) {
            result.wns_ff = slack_ff;
            worstHoldPath = path;
        }
    }

    cout << "\nArea Table:" << endl;
    for (const auto& buffer : buffers) {
        cout << "Buffer: " << buffer.name
             << ", Width: " << buffer.width
             << ", Height: " << buffer.height
             << ", Fanout Max: " << buffer.fanoutMax
             << endl;
    }
    cout << "Total Paths: " << paths.size() << endl;
    cout << "Setup Violations: " << setupViolation << endl;
    cout << "Hold Violations: " << holdViolation << endl;
    if (worstSetupPath != nullptr) {
        cout << "Worst Setup Path: "
            << worstSetupPath->launch->name
            << " -> "
            << worstSetupPath->capture->name
            << endl;
    }

    if (worstHoldPath != nullptr) {
        cout << "Worst Hold Path: "
            << worstHoldPath->launch->name
            << " -> "
            << worstHoldPath->capture->name
            << endl;
    }
    cout << "Launch clk SS: "
        << worstSetupPath->launch->clkDelaySS << endl;

    cout << "Capture clk SS: "
        << worstSetupPath->capture->clkDelaySS << endl;

    cout << "Data delay SS: "
        << worstSetupPath->SS_delay << endl;

    cout << "Clock period: "
        << worstSetupPath->clockPeriod << endl;
}

void print_score(double tns_ss, double tns_ssori, double wns_ss, double wns_ssori, double tns_ff, double tns_ffori, double wns_ff, double wns_ffori, double area, double areaori) {
    cout << "\nScore Report:" << endl;
    cout << "TNS SS: " << tns_ss << ", Original TNS SS: " << tns_ssori << endl;
    cout << "TNS FF: " << tns_ff << ", Original TNS FF: " << tns_ffori << endl;
    cout << "WNS SS: " << wns_ss << ", Original WNS SS: " << wns_ssori << endl;
    cout << "WNS FF: " << wns_ff << ", Original WNS FF: " << wns_ffori << endl;
    cout << "Area: " << area << ", Original Area: " << areaori << endl;

    double alpha = 0.4;
    double beta = 0.3;
    double gamma = 0.3;

    double score = alpha * ((1 - tns_ss / tns_ssori)+(1- wns_ss / wns_ssori))
                 + beta * ((1 - tns_ff / tns_ffori) + (1- wns_ff / wns_ffori))
                 + gamma * (1 - area / areaori);

    cout << "\nalpha: " << alpha << ", beta: " << beta
         << ", gamma: " << gamma
         << endl;

    cout << "Final Score: " << score << endl;
}

void updateClockDelay(Node* node,
                      unordered_map<string, Buffer>& bufferLib)
{
    for (Node* child : node->children) {

        child->clkDelaySS = node->clkDelaySS;
        child->clkDelayFF = node->clkDelayFF;

        if (node->isBuffer) {
            int fanout = node->children.size();

            Buffer& buf = bufferLib[node->cellType];

            child->clkDelaySS +=
                buf.SS_delay[fanout - 1];

            child->clkDelayFF +=
                buf.FF_delay[fanout - 1];
        }

        updateClockDelay(child, bufferLib);
    }
}