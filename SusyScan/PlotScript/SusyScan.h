#ifndef SUSYSCAN_H
#define SUSYSCAN_H

#include <string>
#include <vector>

class SusyScan{
 public:
  SusyScan();
  SusyScan(const SusyScan&);
  SusyScan(std::string file);
  
 private:	
  std::vector<double*> p;
 public:
  const SusyScan operator*(const double f) const;
  const SusyScan operator+(const SusyScan& f) const;
  
  double Mzero;
  double Mhalf;
  double Mu;
  double TanBeta;
  double Azero;
  double Run;
  double data;
  double Xsection;
  double background;
  double background_uncertainty;
  double signal_JEC_UP;
  double signal_JEC_DN;
  double signal_MuIso_UP;
  double signal_MuIso_DN;
  double signal_kfactor_UP;
  double signal_kfactor_DN;
  double signal_kfactor;
  double signal_PDF_UP;
  double signal_PDF_DN;
  double signal_XSPDF_UP;
  double signal_XSPDF_DN;
  double signal;
  double signal_uncertainty;
  double signal_contamination;
  double ExpXsecLimit;
  double ObsXsecLimit;
  double ExpNsigLimit;
  double ObsNsigLimit;
  double lnQ_b_exp;
  double lnQ_b_obs;
  double lnQ_b_xsec;
  double lnQ_sb_exp;
  double lnQ_sb_obs;
  double lnQ_sb_xsec;
  double CLb_b_exp;
  double CLb_b_obs;
  double CLb_b_xsec;
  double CLs_exp;
  double CLs_obs;
  double CLs_xsec;
  double CLs_b_exp;
  double CLs_bNoSig_xsec;
  double CLs_b_obs;
  double CLs_b_xsec;
  double CLs_b_n1_exp;
  double CLs_b_n1_obs;
  double CLs_b_n1_xsec;
  double CLs_b_n2_exp;
  double CLs_b_n2_obs;
  double CLs_b_n2_xsec;
  double CLs_b_p1_exp;
  double CLs_b_p1_obs;
  double CLs_b_p1_xsec;
  double CLs_b_p2_exp;
  double CLs_b_p2_obs;
  double CLs_b_p2_xsec;
  double CLsb_b_exp;
  double CLsb_b_obs;
  double CLsb_b_xsec;
  double PLExpXsecLimit;
  double PLObsXsecLimit;
  double PLExpNsigLimit;
  double PLObsNsigLimit;
  double FCExpXsecLimit;
  double FCObsXsecLimit;
  double FCExpNsigLimit;
  double FCObsNsigLimit;
  double MCMCExpXsecLimit;
  double MCMCObsXsecLimit;
  double MCMCExpNsigLimit;
  double MCMCObsNsigLimit;
  
  double NLO_signal;
  double NLO_signal_uncertainty;
  double NLO_signal_contamination;
  double NLO_ExpXsecLimit;
  double NLO_ObsXsecLimit;
  double NLO_ExpNsigLimit;
  double NLO_ObsNsigLimit;
  double NLO_lnQ_b_exp;
  double NLO_lnQ_b_obs;
  double NLO_lnQ_b_xsec;
  double NLO_lnQ_sb_exp;
  double NLO_lnQ_sb_obs;
  double NLO_lnQ_sb_xsec;
  double NLO_CLb_b_exp;
  double NLO_CLb_b_obs;
  double NLO_CLb_b_xsec;
  double NLO_CLs_exp;
  double NLO_CLs_obs;
  double NLOHybrid_CLs_xsec;
  double NLOHybrid_CLs_xsec_error;
  double NLO_CLs_xsec;
  double NLO_CLs_bNoSig_xsec;
  double NLO_CLs_b_exp;
  double NLO_CLs_b_obs;
  double NLO_CLs_b_xsec;
  double NLO_CLs_b_n1_exp;
  double NLO_CLs_b_n1_obs;
  double NLO_CLs_b_n1_xsec;
  double NLO_CLs_b_n2_exp;
  double NLO_CLs_b_n2_obs;
  double NLO_CLs_b_n2_xsec;
  double NLO_CLs_b_p1_exp;
  double NLO_CLs_b_p1_obs;
  double NLO_CLs_b_p1_xsec;
  double NLO_CLs_b_p2_exp;
  double NLO_CLs_b_p2_obs;
  double NLO_CLs_b_p2_xsec;
  double NLO_CLsb_b_exp;
  double NLO_CLsb_b_obs;
  double NLO_CLsb_b_xsec;
  double NLO_PLExpXsecLimit;
  double NLO_PLObsXsecLimit;
  double NLO_PLExpNsigLimit;
  double NLO_PLObsNsigLimit;
  double NLO_FCExpXsecLimit;
  double NLO_FCObsXsecLimit;
  double NLO_FCExpNsigLimit;
  double NLO_FCObsNsigLimit;
  double NLO_MCMCExpXsecLimit;
  double NLO_MCMCObsXsecLimit;
  double NLO_MCMCExpNsigLimit;
  double NLO_MCMCObsNsigLimit;

  double M1;
  double M2;
  double M3;
  double MGL;
  double MUL;
  double MB1;
  double MSN;
  double MNTAU;
  double MZ1;
  double MW1;
  double MHL;
  double MUR;
  double MB2;
  double MEL;
  double MTAU1;
  double MZ2;
  double MW2;
  double MHH;
  double MDL;
  double MT1;
  double MER;
  double MTAU2;
  double MZ3;
  double MHA;
  double MDR;
  double MT2;
  double MZ4;
  double MHp;

 private:
  void SetPtr();

};


#endif
