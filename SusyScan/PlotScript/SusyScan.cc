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
  background_uncertainty = config.read<double>("background.uncertainty", 0);
  data 		= config.read<double>("data", 0);
  signal 	= config.read<double>("signal", 0);
  signal_uncertainty = config.read<double>("signal.uncertainty", 0);
  Xsection 	= config.read<double>("Xsection", 0);
  ExpXsecLimit 	= config.read<double>("ExpXsecLimit", 0);
  ObsXsecLimit 	= config.read<double>("ObsXsecLimit", 0);
  lnQ_b_exp 	= config.read<double>("-2lnQ_b@exp", 0);
  lnQ_b_obs 	= config.read<double>("-2lnQ_b@obs", 0);
  lnQ_sb_exp 	= config.read<double>("-2lnQ_sb@exp", 0);
  lnQ_sb_obs 	= config.read<double>("-2lnQ_sb@obs", 0);
  CLb_b_exp 	= config.read<double>("CLb_b_exp", 0);
  CLb_b_obs 	= config.read<double>("CLb_b_obs", 0);
  CLs_exp 	= config.read<double>("CLs_exp", 0);
  CLs_obs 	= config.read<double>("CLs_obs", 0);
  CLs_b_exp 	= config.read<double>("CLs_b_exp", 0);
  CLs_b_obs 	= config.read<double>("CLs_b_obs", 0);
  CLs_b_n1_exp 	= config.read<double>("CLs_b_n1_exp", 0);
  CLs_b_n1_obs 	= config.read<double>("CLs_b_n1_obs", 0);
  CLs_b_n2_exp 	= config.read<double>("CLs_b_n2_exp", 0);
  CLs_b_n2_obs 	= config.read<double>("CLs_b_n2_obs", 0);
  CLs_b_p1_exp 	= config.read<double>("CLs_b_p1_exp", 0);
  CLs_b_p1_obs 	= config.read<double>("CLs_b_p1_obs", 0);
  CLs_b_p2_exp 	= config.read<double>("CLs_b_p2_exp", 0);
  CLs_b_p2_obs 	= config.read<double>("CLs_b_p2_obs", 0);
  CLsb_b_exp 	= config.read<double>("CLsb_b_exp", 0);
  CLsb_b_obs 	= config.read<double>("CLsb_b_obs", 0);
}
