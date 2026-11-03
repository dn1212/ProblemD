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
void print_result(vector<Node*> nodes, vector<Buffer> buffers, vector<Path*> paths, double& tns_ss, double& tns_ssori, double& wns_ss, double& wns_ssori, double& tns_ff, double& tns_ffori, double& wns_ff, double& wns_ffori, double& area, double& areaori);
void print_score(double tns_ss, double tns_ssori, double wns_ss, double wns_ssori, double tns_ff, double tns_ffori, double wns_ff, double wns_ffori, double area, double areaori);
# endif // PRINTER_H