#include "SusyScan.h"
#include "ConfigFile.h"

SusyScan::SusyScan(std::string filename)
{
  ConfigFile config(filename);
  Mzero 	= config.read<double>("Mzero", 0);
  Mhalf	 	= config.read<double>("Mhalf", 0);
  Mu 		= config.read<double>("Mu", 0);
  TanBeta 	= config.read<double>("TanBeta", 0);
  Azero 	= config.read<double>("Azero", 0);
  Run 		= config.read<double>("Run", 0);
  background 	= config.read<double>("background", 0);
  signal_contamination 	= config.read<double>("signal.signalregion.IsoMuon", 10);
  background_uncertainty = config.read<double>("background.uncertainty", 0);
  data 		= config.read<double>("data", 0);
  signal 	= config.read<double>("signal", 0);
  signal_uncertainty = config.read<double>("signal.uncertainty", 0);
  Xsection 	= config.read<double>("Xsection", 0);
  ExpXsecLimit 	= config.read<double>("ExpXsecLimit", 0);
  ObsXsecLimit 	= config.read<double>("ObsXsecLimit", 0);
  ExpNsigLimit 	= config.read<double>("ExpNsigLimit", 0);
  ObsNsigLimit 	= config.read<double>("ObsNsigLimit", 0);
  lnQ_b_exp 	= config.read<double>("-2lnQ_b@exp", 0);
  lnQ_b_obs 	= config.read<double>("-2lnQ_b@obs", 0);
  lnQ_b_xsec 	= config.read<double>("-2lnQ_b@xsec", 0);
  lnQ_sb_exp 	= config.read<double>("-2lnQ_sb@exp", 0);
  lnQ_sb_obs 	= config.read<double>("-2lnQ_sb@obs", 0);
  lnQ_sb_xsec 	= config.read<double>("-2lnQ_sb@xsec", 0);
  CLb_b_exp 	= config.read<double>("CLb_b@exp", 0);
  CLb_b_obs 	= config.read<double>("CLb_b@obs", 0);
  CLb_b_xsec 	= config.read<double>("CLb_b@xsec", 0);
  CLs_exp 	= config.read<double>("CLs@exp", 0);
  CLs_obs 	= config.read<double>("CLs@obs", 0);
  CLs_xsec 	= config.read<double>("CLs@xsec", 0);
  CLs_b_exp 	= config.read<double>("CLs_b@exp", 0);
  CLs_b_obs 	= config.read<double>("CLs_b@obs", 0);
  CLs_b_xsec 	= config.read<double>("CLs_b@xsec", 0);
  CLs_b_n1_exp 	= config.read<double>("CLs_b_n1@exp", 0);
  CLs_b_n1_obs 	= config.read<double>("CLs_b_n1@obs", 0);
  CLs_b_n1_xsec = config.read<double>("CLs_b_n1@xsec", 0);
  CLs_b_n2_exp 	= config.read<double>("CLs_b_n2@exp", 0);
  CLs_b_n2_obs 	= config.read<double>("CLs_b_n2@obs", 0);
  CLs_b_n2_xsec	= config.read<double>("CLs_b_n2@xsec", 0);
  CLs_b_p1_exp 	= config.read<double>("CLs_b_p1@exp", 0);
  CLs_b_p1_obs 	= config.read<double>("CLs_b_p1@obs", 0);
  CLs_b_p1_xsec	= config.read<double>("CLs_b_p1@xsec", 0);
  CLs_b_p2_exp 	= config.read<double>("CLs_b_p2@exp", 0);
  CLs_b_p2_obs 	= config.read<double>("CLs_b_p2@obs", 0);
  CLs_b_p2_xsec	= config.read<double>("CLs_b_p2@xsec", 0);
  CLsb_b_exp 	= config.read<double>("CLsb_b@exp", 0);
  CLsb_b_obs 	= config.read<double>("CLsb_b@obs", 0);
  CLsb_b_xsec	= config.read<double>("CLsb_b@xsec", 0);
  PLExpXsecLimit= config.read<double>("RooSimpleProfile.xsec.ExpUpperLimit", 0);
  PLObsXsecLimit= config.read<double>("RooSimpleProfile.xsec.ObsUpperLimit", 0);
  PLExpNsigLimit= config.read<double>("RooSimpleProfile.signal.ExpUpperLimit", 0);
  PLObsNsigLimit= config.read<double>("RooSimpleProfile.signal.ObsUpperLimit", 0);
  FCExpXsecLimit= config.read<double>("RooFC.xsec.ExpUpperLimit", 0);
  FCObsXsecLimit= config.read<double>("RooFC.xsec.ObsUpperLimit", 0);
  FCExpNsigLimit= config.read<double>("RooFC.signal.ExpUpperLimit", 0);
  FCObsNsigLimit= config.read<double>("RooFC.signal.ObsUpperLimit", 0);
  MCMCExpXsecLimit= config.read<double>("RooMCMC.xsec.ExpUpperLimit", 0);
  MCMCObsXsecLimit= config.read<double>("RooMCMC.xsec.ObsUpperLimit", 0);
  MCMCExpNsigLimit= config.read<double>("RooMCMC.signal.ExpUpperLimit", 0);
  MCMCObsNsigLimit= config.read<double>("RooMCMC.signal.ObsUpperLimit", 0);
}
