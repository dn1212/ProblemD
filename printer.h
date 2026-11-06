# ifndef PRINTER_H
# define PRINTER_H
# include "parser.h"
/*------printer for project--------*/
/*content include*/
// Original Setup WNS/TNS（SS corner）
// Hold WNS/TNS（FF corner）
// Area table
// Score α、β、γ (initial value: α=0.4、β=0.3、γ=0.3)
using namespace std;
struct TimingResult {
    double tns_ss = 0.0;
    double wns_ss = 0.0;

    double tns_ff = 0.0;
    double wns_ff = 0.0;

    double area = 0.0;
};
void update_result(
    const vector<Node*>& nodes,
    const vector<Buffer>& buffers,
    const vector<Path*>& paths,
    const unordered_map<string, Buffer>& bufferMap,
    TimingResult& result
);
void print_score(double tns_ss, double tns_ssori, double wns_ss, double wns_ssori, double tns_ff, double tns_ffori, double wns_ff, double wns_ffori, double area, double areaori);
void updateClockDelay(Node* node,unordered_map<string, Buffer>& bufferLib);
# endif // PRINTER_H