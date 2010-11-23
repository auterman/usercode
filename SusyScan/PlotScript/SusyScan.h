#ifndef SUSYSCAN_H
#define SUSYSCAN_H

#include <string>

class SusyScan{
 public:
  SusyScan(std::string file);
  
  double Mzero;
  double Mhalf;
  double Mu;
  double TanBeta;
  double Azero;
  double Run;
  double background;
  double background_uncertainty;
  double data;
  double signal;
  double signal_uncertainty;
  double Xsection;
  double ExpXsecLimit;
  double ObsXsecLimit;
  double lnQ_b_exp;
  double lnQ_b_obs;
  double lnQ_sb_exp;
  double lnQ_sb_obs;
  double CLb_b_exp;
  double CLb_b_obs;
  double CLs_exp;
  double CLs_obs;
  double CLs_b_exp;
  double CLs_b_obs;
  double CLs_b_n1_exp;
  double CLs_b_n1_obs;
  double CLs_b_n2_exp;
  double CLs_b_n2_obs;
  double CLs_b_p1_exp;
  double CLs_b_p1_obs;
  double CLs_b_p2_exp;
  double CLs_b_p2_obs;
  double CLsb_b_exp;
  double CLsb_b_obs;

};


#endif
