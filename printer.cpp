#include "printer.h"


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


    for(Node* node : nodes){

        if(node->isBuffer){

            auto it =
                bufferMap.find(
                    node->cellType
                );


            if(it !=
               bufferMap.end()){

                result.area +=
                    it->second.width
                    *
                    it->second.height;
            }
        }
    }


    for(const auto& path : paths){

        double t_clk =
            path->clockPeriod;


        double t_setup =
            t_clk * 0.08;


        double d_data =
            path->SS_delay;


        double d_launch =
            path->launch->clkDelaySS;


        double d_capture =
            path->capture->clkDelaySS;


        double skew_ss =
            d_capture -
            d_launch;


        double slack_ss =
            t_clk
            -
            t_setup
            -
            d_data
            +
            skew_ss;


        if(slack_ss < 0){

            result.tns_ss +=
                slack_ss;
        }


        if(slack_ss <
           result.wns_ss){

            result.wns_ss =
                slack_ss;
        }
    }


    for(const auto& path : paths){

        double t_clk =
            path->clockPeriod;


        double t_hold =
            t_clk * 0.05;


        double d_data =
            path->FF_delay;


        double d_launch =
            path->launch->clkDelayFF;


        double d_capture =
            path->capture->clkDelayFF;


        double skew_ff =
            d_capture -
            d_launch;


        double slack_ff =
            d_data
            -
            t_hold
            -
            skew_ff;


        if(slack_ff < 0){

            result.tns_ff +=
                slack_ff;
        }


        if(slack_ff <
           result.wns_ff){

            result.wns_ff =
                slack_ff;
        }
    }
}


double calculate_score(
    const TimingResult& result,
    const TimingResult& originalResult
){
    double alpha = 0.4;
    double beta = 0.3;
    double gamma = 0.3;


    double score =
        alpha * (
            (1 - result.tns_ss /
                 originalResult.tns_ss)
            +
            (1 - result.wns_ss /
                 originalResult.wns_ss)
        )
        +
        beta * (
            (1 - result.tns_ff /
                 originalResult.tns_ff)
            +
            (1 - result.wns_ff /
                 originalResult.wns_ff)
        )
        +
        gamma * (
            1 - result.area /
                originalResult.area
        );


    return score;
}


void print_score(
    const TimingResult& result,
    const TimingResult& originalResult
){
    double score =
        calculate_score(
            result,
            originalResult
        );


    cout << "\nScore Report:"
         << endl;


    cout << "TNS SS: "
         << result.tns_ss
         << ", Original TNS SS: "
         << originalResult.tns_ss
         << endl;


    cout << "TNS FF: "
         << result.tns_ff
         << ", Original TNS FF: "
         << originalResult.tns_ff
         << endl;


    cout << "WNS SS: "
         << result.wns_ss
         << ", Original WNS SS: "
         << originalResult.wns_ss
         << endl;


    cout << "WNS FF: "
         << result.wns_ff
         << ", Original WNS FF: "
         << originalResult.wns_ff
         << endl;


    cout << "Area: "
         << result.area
         << ", Original Area: "
         << originalResult.area
         << endl;


    cout << "Final Score: "
         << score
         << endl;
}


void updateClockDelay(
    Node* node,
    unordered_map<string, Buffer>& bufferLib
){
    for(Node* child :
        node->children){

        child->clkDelaySS =
            node->clkDelaySS;


        child->clkDelayFF =
            node->clkDelayFF;


        if(node->isBuffer){

            int fanout =
                static_cast<int>(
                    node->children.size()
                );


            auto it =
                bufferLib.find(
                    node->cellType
                );


            if(it ==
               bufferLib.end()){

                cout << "Buffer not found: "
                     << node->cellType
                     << endl;

                exit(1);
            }


            Buffer& buf =
                it->second;


            if(fanout <= 0 ||
               fanout >
               buf.fanoutMax){

                cout << "Invalid fanout: "
                     << node->name
                     << endl;

                exit(1);
            }


            child->clkDelaySS +=
                buf.SS_delay[
                    fanout - 1
                ];


            child->clkDelayFF +=
                buf.FF_delay[
                    fanout - 1
                ];
        }


        updateClockDelay(
            child,
            bufferLib
        );
    }
}