# include "printer.h"
/*------printer for project--------*/
/*content include*/
// Original Setup WNS/TNS（SS corner）
// Hold WNS/TNS（FF corner）
// Area table
// Score α、β、γ (initial value: α=0.4、β=0.3、γ=0.3)

void update_result(
    const vector<Node*>& nodes,
    const vector<Buffer>& buffers,
    const vector<Path*>& paths,
    TimingResult& result
){
    // Print Original Setup WNS/TNS (SS corner)
    result.tns_ss = 0.0;
    result.wns_ss = 0.0;
    result.tns_ff = 0.0;
    result.wns_ff = 0.0;
    result.area = 0.0;

    cout << "Original Setup WNS/TNS (SS corner):" << endl;
    for (const auto& path : paths) {
        cout << "Path: " << path->launch->name << " -> " << path->capture->name
             << ", SS Delay: " << path->SS_delay << endl;
        double t_clk = path ->clockPeriod;
        double t_setup = t_clk * 0.08;
        double d_data = path->SS_delay;
        double slack_ss = t_clk - t_setup - d_data;
        if (slack_ss < 0) {
            result.tns_ss += slack_ss; // Accumulate negative slack for TNS
        }
        if (slack_ss < result.wns_ss) {
            result. wns_ss = slack_ss; // Update maximum WNS
        }
    }
    

    // Print Hold WNS/TNS (FF corner)
    cout << "\nHold WNS/TNS (FF corner):" << endl;
    for (const auto& path : paths) {
        cout << "Path: " << path->launch->name << " -> " << path->capture->name
             << ", FF Delay: " << path->FF_delay << endl;
        double t_clk = path ->clockPeriod;
        double t_hold = t_clk * 0.05;
        double d_data = path->FF_delay;
        double slack_ff = d_data - t_hold;
        if (slack_ff < 0) {
            result.tns_ff += slack_ff; // Accumulate negative slack for TNS
        }
        if (slack_ff < result.wns_ff) {
            result.wns_ff = slack_ff; // Update maximum WNS
        }
    }

    // Print Area table
    cout << "\nArea Table:" << endl;
    for (const auto& buffer : buffers) {
        cout << "Buffer: " << buffer.name
             << ", Width: " << buffer.width
             << ", Height: " << buffer.height
             << ", Fanout Max: " << buffer.fanoutMax
             << endl;
    }

    
}

void print_score(double tns_ss, double tns_ssori, double wns_ss, double wns_ssori, double tns_ff, double tns_ffori, double wns_ff, double wns_ffori, double area, double areaori) {
    cout << "\nScore Report:" << endl;
    cout << "TNS SS: " << tns_ss << ", Original TNS SS: " << tns_ssori << endl;
    cout << "TNS FF: " << tns_ff << ", Original TNS FF: " << tns_ffori << endl;
    cout << "WNS SS: " << wns_ss << ", Original WNS SS: " << wns_ssori << endl;
    cout << "WNS FF: " << wns_ff << ", Original WNS FF: " << wns_ffori << endl;
    cout << "Area: " << area << ", Original Area: " << areaori << endl;
    // Print Score α, β, γ
    double alpha = 0.4; // Example value
    double beta = 0.3;  // Example value
    double gamma = 0.3; // Example value
    double score = alpha * ((1 - tns_ss / tns_ssori)+(1- wns_ss / wns_ssori)) + beta * ((1 - tns_ff / tns_ffori) + (1- wns_ff / wns_ffori)) + gamma * (1 - area / areaori);
    cout << "\nα: " << alpha << ", β: " << beta
         << ", γ: " << gamma
         << endl;
    cout << "Final Score: " << score << endl;
}