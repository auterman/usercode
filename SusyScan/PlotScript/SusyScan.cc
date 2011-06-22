#include "SusyScan.h"
#include "ConfigFile.h"


SusyScan::SusyScan()
{
  SetPtr();
  for (std::vector<double*>::iterator it=p.begin(); it!=p.end(); ++it)
    **it = 0.;
}
SusyScan::SusyScan(const SusyScan& c)
{
  SetPtr();
  //std::cout<<Mzero<<"=m0, "<<Mhalf <<"=m12, " <<p.size()<<"<-this, c:"<<c.p.size()<<std::endl; 

  std::vector<double*>::iterator it=p.begin();
  std::vector<double*>::const_iterator ct=c.p.begin();
  for (; ct!=c.p.end(); ++it, ++ct)
    **it = **ct;
}


SusyScan::SusyScan(std::string filename)
{
  SetPtr();
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
  signal_JEC_UP 	= config.read<double>("signal.JEC_.UP", 0);
  signal_JEC_DN 	= config.read<double>("signal.JEC_.DN", 0);
  signal_MuIso_UP 	= config.read<double>("signal.MuIso_.UP", 0);
  signal_MuIso_DN 	= config.read<double>("signal.MuIso_.DN", 0);
  signal_kfactor_UP 	= config.read<double>("signal.kFactorUP", 0);
  signal_kfactor_DN 	= config.read<double>("signal.kFactorDN", 0);
  signal_PDF_UP 	= config.read<double>("signal.PDF.UP", 0);
  signal_PDF_DN 	= config.read<double>("signal.PDF.DN", 0);
  signal_XSPDF_UP 	= config.read<double>("signal.PDFXsec.UP", 0);
  signal_XSPDF_DN 	= config.read<double>("signal.PDFXsec.DN", 0);
  signal_kfactor   	= config.read<double>("signal.kFactor", 0);

  signal_contamination 	= config.read<double>("signal.LO.signalregion.IsoMuon", 0) + 
                          config.read<double>("signal.LO.signalregion.Tau", 0);
  signal 	= config.read<double>("signal.LO", 0);
  signal_uncertainty = config.read<double>("signal.LO.uncertainty", 0);
  Xsection 	= config.read<double>("Xsection", 0);
  ExpXsecLimit 	= config.read<double>("LO.ExpXsecLimit", 0);
  ObsXsecLimit 	= config.read<double>("LO.ObsXsecLimit", 0);
  ExpNsigLimit 	= config.read<double>("LO.ExpNsigLimit", 0);
  ObsNsigLimit 	= config.read<double>("LO.ObsNsigLimit", 0);
  lnQ_b_exp 	= config.read<double>("LO.-2lnQ_b@exp", 0);
  lnQ_b_obs 	= config.read<double>("LO.-2lnQ_b@obs", 0);
  lnQ_b_xsec 	= config.read<double>("LO.-2lnQ_b@xsec", 0);
  lnQ_sb_exp 	= config.read<double>("LO.-2lnQ_sb@exp", 0);
  lnQ_sb_obs 	= config.read<double>("LO.-2lnQ_sb@obs", 0);
  lnQ_sb_xsec 	= config.read<double>("LO.-2lnQ_sb@xsec", 0);
  CLb_b_exp 	= config.read<double>("LO.CLb_b@exp", 0);
  CLb_b_obs 	= config.read<double>("LO.CLb_b@obs", 0);
  CLb_b_xsec 	= config.read<double>("LO.CLb_b@xsec", 0);
  CLs_exp 	= config.read<double>("LO.CLs@exp", 0);
  CLs_obs 	= config.read<double>("LO.CLs@obs", 0);
  CLs_xsec 	= config.read<double>("LO.CLs@xsec", 0);
  CLs_bNoSig_xsec=config.read<double>("LO.CLs_b_DataNoSignalHyp@xsec", 0);
  CLs_b_exp 	= config.read<double>("LO.CLs_b@exp", 0);
  CLs_b_obs 	= config.read<double>("LO.CLs_b@obs", 0);
  CLs_b_xsec 	= config.read<double>("LO.CLs_b@xsec", 0);
  CLs_b_n1_exp 	= config.read<double>("LO.CLs_b_n1@exp", 0);
  CLs_b_n1_obs 	= config.read<double>("LO.CLs_b_n1@obs", 0);
  CLs_b_n1_xsec = config.read<double>("LO.CLs_b_n1@xsec", 0);
  CLs_b_n2_exp 	= config.read<double>("LO.CLs_b_n2@exp", 0);
  CLs_b_n2_obs 	= config.read<double>("LO.CLs_b_n2@obs", 0);
  CLs_b_n2_xsec	= config.read<double>("LO.CLs_b_n2@xsec", 0);
  CLs_b_p1_exp 	= config.read<double>("LO.CLs_b_p1@exp", 0);
  CLs_b_p1_obs 	= config.read<double>("LO.CLs_b_p1@obs", 0);
  CLs_b_p1_xsec	= config.read<double>("LO.CLs_b_p1@xsec", 0);
  CLs_b_p2_exp 	= config.read<double>("LO.CLs_b_p2@exp", 0);
  CLs_b_p2_obs 	= config.read<double>("LO.CLs_b_p2@obs", 0);
  CLs_b_p2_xsec	= config.read<double>("LO.CLs_b_p2@xsec", 0);
  CLsb_b_exp 	= config.read<double>("LO.CLsb_b@exp", 0);
  CLsb_b_obs 	= config.read<double>("LO.CLsb_b@obs", 0);
  CLsb_b_xsec	= config.read<double>("LO.CLsb_b@xsec", 0);
  PLExpXsecLimit= config.read<double>("RooSimpleProfile.xsec.LO.ExpUpperLimit", 0);
  PLObsXsecLimit= config.read<double>("RooSimpleProfile.xsec.LO.ObsUpperLimit", 0);
  PLExpNsigLimit= config.read<double>("RooSimpleProfile.signal.LO.ExpUpperLimit", 0);
  PLObsNsigLimit= config.read<double>("RooSimpleProfile.signal.LO.ObsUpperLimit", 0);
  FCExpXsecLimit= config.read<double>("RooFC.xsec.LO.ExpUpperLimit", 0);
  FCObsXsecLimit= config.read<double>("RooFC.xsec.LO.ObsUpperLimit", 0);
  FCExpNsigLimit= config.read<double>("RooFC.signal.LO.ExpUpperLimit", 0);
  FCObsNsigLimit= config.read<double>("RooFC.signal.LO.ObsUpperLimit", 0);
  MCMCExpXsecLimit= config.read<double>("RooMCMC.xsec.LO.ExpUpperLimit", 0);
  MCMCObsXsecLimit= config.read<double>("RooMCMC.xsec.LO.ObsUpperLimit", 0);
  MCMCExpNsigLimit= config.read<double>("RooMCMC.signal.LO.ExpUpperLimit", 0);
  MCMCObsNsigLimit= config.read<double>("RooMCMC.signal.LO.ObsUpperLimit", 0);

  NLO_signal_contamination 	= config.read<double>("signal.NLO.signalregion.IsoMuon", 0) +
                                  config.read<double>("signal.NLO.signalregion.Tau", 0);
  NLO_signal 	= config.read<double>("signal.NLO", 0);
  NLO_signal_uncertainty = config.read<double>("signal.NLO.uncertainty", 0);
  NLO_ExpXsecLimit 	= config.read<double>("NLO.ExpXsecLimit", 0);
  NLO_ObsXsecLimit 	= config.read<double>("NLO.ObsXsecLimit", 0);
  NLO_ExpNsigLimit 	= config.read<double>("NLO.ExpNsigLimit", 0);
  NLO_ObsNsigLimit 	= config.read<double>("NLO.ObsNsigLimit", 0);
  NLO_lnQ_b_exp 	= config.read<double>("NLO.-2lnQ_b@exp", 0);
  NLO_lnQ_b_obs 	= config.read<double>("NLO.-2lnQ_b@obs", 0);
  NLO_lnQ_b_xsec 	= config.read<double>("NLO.-2lnQ_b@xsec", 0);
  NLO_lnQ_sb_exp 	= config.read<double>("NLO.-2lnQ_sb@exp", 0);
  NLO_lnQ_sb_obs 	= config.read<double>("NLO.-2lnQ_sb@obs", 0);
  NLO_lnQ_sb_xsec 	= config.read<double>("NLO.-2lnQ_sb@xsec", 0);
  NLO_CLb_b_exp 	= config.read<double>("NLO.CLb_b@exp", 0);
  NLO_CLb_b_obs 	= config.read<double>("NLO.CLb_b@obs", 0);
  NLO_CLb_b_xsec 	= config.read<double>("NLO.CLb_b@xsec", 0);
  NLO_CLs_exp 	= config.read<double>("NLO.CLs@exp", 0);
  NLO_CLs_obs 	= config.read<double>("NLO.CLs@obs", 0);
  NLO_CLs_xsec 	= config.read<double>("NLO.CLs@xsec", 0);
  NLOHybrid_CLs_xsec 	= config.read<double>("NLOHybrid_CLs@xsec", 0);
  NLOHybrid_CLs_xsec_error 	= config.read<double>("NLOHybrid_CLs_error@xsec", 0);
  NLO_CLs_bNoSig_xsec 	= config.read<double>("NLO.CLs_b_DataNoSignalHyp@xsec", 0);
  NLO_CLs_b_exp 	= config.read<double>("NLO.CLs_b@exp", 0);
  NLO_CLs_b_obs 	= config.read<double>("NLO.CLs_b@obs", 0);
  NLO_CLs_b_xsec 	= config.read<double>("NLO.CLs_b@xsec", 0);
  NLO_CLs_b_n1_exp 	= config.read<double>("NLO.CLs_b_n1@exp", 0);
  NLO_CLs_b_n1_obs 	= config.read<double>("NLO.CLs_b_n1@obs", 0);
  NLO_CLs_b_n1_xsec = config.read<double>("NLO.CLs_b_n1@xsec", 0);
  NLO_CLs_b_n2_exp 	= config.read<double>("NLO.CLs_b_n2@exp", 0);
  NLO_CLs_b_n2_obs 	= config.read<double>("NLO.CLs_b_n2@obs", 0);
  NLO_CLs_b_n2_xsec	= config.read<double>("NLO.CLs_b_n2@xsec", 0);
  NLO_CLs_b_p1_exp 	= config.read<double>("NLO.CLs_b_p1@exp", 0);
  NLO_CLs_b_p1_obs 	= config.read<double>("NLO.CLs_b_p1@obs", 0);
  NLO_CLs_b_p1_xsec	= config.read<double>("NLO.CLs_b_p1@xsec", 0);
  NLO_CLs_b_p2_exp 	= config.read<double>("NLO.CLs_b_p2@exp", 0);
  NLO_CLs_b_p2_obs 	= config.read<double>("NLO.CLs_b_p2@obs", 0);
  NLO_CLs_b_p2_xsec	= config.read<double>("NLO.CLs_b_p2@xsec", 0);
  NLO_CLsb_b_exp 	= config.read<double>("NLO.CLsb_b@exp", 0);
  NLO_CLsb_b_obs 	= config.read<double>("NLO.CLsb_b@obs", 0);
  NLO_CLsb_b_xsec	= config.read<double>("NLO.CLsb_b@xsec", 0);
  NLO_PLExpXsecLimit= config.read<double>("RooSimpleProfile.xsec.NLO.ExpUpperLimit", 0);
  NLO_PLObsXsecLimit= config.read<double>("RooSimpleProfile.xsec.NLO.ObsUpperLimit", 0);
  NLO_PLExpNsigLimit= config.read<double>("RooSimpleProfile.signal.NLO.ExpUpperLimit", 0);
  NLO_PLObsNsigLimit= config.read<double>("RooSimpleProfile.signal.NLO.ObsUpperLimit", 0);
  NLO_FCExpXsecLimit= config.read<double>("RooFC.xsec.NLO.ExpUpperLimit", 0);
  NLO_FCObsXsecLimit= config.read<double>("RooFC.xsec.NLO.ObsUpperLimit", 0);
  NLO_FCExpNsigLimit= config.read<double>("RooFC.signal.NLO.ExpUpperLimit", 0);
  NLO_FCObsNsigLimit= config.read<double>("RooFC.signal.NLO.ObsUpperLimit", 0);
  NLO_MCMCExpXsecLimit= config.read<double>("RooMCMC.xsec.NLO.ExpUpperLimit", 0);
  NLO_MCMCObsXsecLimit= config.read<double>("RooMCMC.xsec.NLO.ObsUpperLimit", 0);
  NLO_MCMCExpNsigLimit= config.read<double>("RooMCMC.signal.NLO.ExpUpperLimit", 0);
  NLO_MCMCObsNsigLimit= config.read<double>("RooMCMC.signal.NLO.ObsUpperLimit", 0);
}

void SusyScan::SetPtr()
{
  if (p.size()!=0) return; 
  p.push_back( &Mzero);
  p.push_back( &Mhalf);
  p.push_back( &Mu);
  p.push_back( &TanBeta);
  p.push_back( &Azero);
  p.push_back( &Run);
  p.push_back( &data);
  p.push_back( &Xsection);
  p.push_back( &background);
  p.push_back( &background_uncertainty);
  p.push_back( &signal_JEC_UP);
  p.push_back( &signal_JEC_DN);
  p.push_back( &signal_MuIso_UP);
  p.push_back( &signal_MuIso_DN);
  p.push_back( &signal_kfactor_UP);
  p.push_back( &signal_kfactor_DN);
  p.push_back( &signal_PDF_UP);
  p.push_back( &signal_PDF_DN);
  p.push_back( &signal_XSPDF_UP);
  p.push_back( &signal_XSPDF_DN);
  p.push_back( &signal_kfactor);
  p.push_back( &signal);
  p.push_back( &signal_uncertainty);
  p.push_back( &signal_contamination);
  p.push_back( &ExpXsecLimit);
  p.push_back( &ObsXsecLimit);
  p.push_back( &ExpNsigLimit);
  p.push_back( &ObsNsigLimit);
  p.push_back( &lnQ_b_exp);
  p.push_back( &lnQ_b_obs);
  p.push_back( &lnQ_b_xsec);
  p.push_back( &lnQ_sb_exp);
  p.push_back( &lnQ_sb_obs);
  p.push_back( &lnQ_sb_xsec);
  p.push_back( &CLb_b_exp);
  p.push_back( &CLb_b_obs);
  p.push_back( &CLb_b_xsec);
  p.push_back( &CLs_exp);
  p.push_back( &CLs_obs);
  p.push_back( &CLs_xsec);
  p.push_back( &CLs_b_exp);
  p.push_back( &CLs_bNoSig_xsec);
  p.push_back( &CLs_b_obs);
  p.push_back( &CLs_b_xsec);
  p.push_back( &CLs_b_n1_exp);
  p.push_back( &CLs_b_n1_obs);
  p.push_back( &CLs_b_n1_xsec);
  p.push_back( &CLs_b_n2_exp);
  p.push_back( &CLs_b_n2_obs);
  p.push_back( &CLs_b_n2_xsec);
  p.push_back( &CLs_b_p1_exp);
  p.push_back( &CLs_b_p1_obs);
  p.push_back( &CLs_b_p1_xsec);
  p.push_back( &CLs_b_p2_exp);
  p.push_back( &CLs_b_p2_obs);
  p.push_back( &CLs_b_p2_xsec);
  p.push_back( &CLsb_b_exp);
  p.push_back( &CLsb_b_obs);
  p.push_back( &CLsb_b_xsec);
  p.push_back( &PLExpXsecLimit);
  p.push_back( &PLObsXsecLimit);
  p.push_back( &PLExpNsigLimit);
  p.push_back( &PLObsNsigLimit);
  p.push_back( &FCExpXsecLimit);
  p.push_back( &FCObsXsecLimit);
  p.push_back( &FCExpNsigLimit);
  p.push_back( &FCObsNsigLimit);
  p.push_back( &MCMCExpXsecLimit);
  p.push_back( &MCMCObsXsecLimit);
  p.push_back( &MCMCExpNsigLimit);
  p.push_back( &MCMCObsNsigLimit);
  p.push_back( &NLO_signal);
  p.push_back( &NLO_signal_uncertainty);
  p.push_back( &NLO_signal_contamination);
  p.push_back( &NLO_ExpXsecLimit);
  p.push_back( &NLO_ObsXsecLimit);
  p.push_back( &NLO_ExpNsigLimit);
  p.push_back( &NLO_ObsNsigLimit);
  p.push_back( &NLO_lnQ_b_exp);
  p.push_back( &NLO_lnQ_b_obs);
  p.push_back( &NLO_lnQ_b_xsec);
  p.push_back( &NLO_lnQ_sb_exp);
  p.push_back( &NLO_lnQ_sb_obs);
  p.push_back( &NLO_lnQ_sb_xsec);
  p.push_back( &NLO_CLb_b_exp);
  p.push_back( &NLO_CLb_b_obs);
  p.push_back( &NLO_CLb_b_xsec);
  p.push_back( &NLO_CLs_exp);
  p.push_back( &NLO_CLs_obs);
  p.push_back( &NLOHybrid_CLs_xsec);
  p.push_back( &NLOHybrid_CLs_xsec_error);
  p.push_back( &NLO_CLs_xsec);
  p.push_back( &NLO_CLs_bNoSig_xsec);
  p.push_back( &NLO_CLs_b_exp);
  p.push_back( &NLO_CLs_b_obs);
  p.push_back( &NLO_CLs_b_xsec);
  p.push_back( &NLO_CLs_b_n1_exp);
  p.push_back( &NLO_CLs_b_n1_obs);
  p.push_back( &NLO_CLs_b_n1_xsec);
  p.push_back( &NLO_CLs_b_n2_exp);
  p.push_back( &NLO_CLs_b_n2_obs);
  p.push_back( &NLO_CLs_b_n2_xsec);
  p.push_back( &NLO_CLs_b_p1_exp);
  p.push_back( &NLO_CLs_b_p1_obs);
  p.push_back( &NLO_CLs_b_p1_xsec);
  p.push_back( &NLO_CLs_b_p2_exp);
  p.push_back( &NLO_CLs_b_p2_obs);
  p.push_back( &NLO_CLs_b_p2_xsec);
  p.push_back( &NLO_CLsb_b_exp);
  p.push_back( &NLO_CLsb_b_obs);
  p.push_back( &NLO_CLsb_b_xsec);
  p.push_back( &NLO_PLExpXsecLimit);
  p.push_back( &NLO_PLObsXsecLimit);
  p.push_back( &NLO_PLExpNsigLimit);
  p.push_back( &NLO_PLObsNsigLimit);
  p.push_back( &NLO_FCExpXsecLimit);
  p.push_back( &NLO_FCObsXsecLimit);
  p.push_back( &NLO_FCExpNsigLimit);
  p.push_back( &NLO_FCObsNsigLimit);
  p.push_back( &NLO_MCMCExpXsecLimit);
  p.push_back( &NLO_MCMCObsXsecLimit);
  p.push_back( &NLO_MCMCExpNsigLimit);
  p.push_back( &NLO_MCMCObsNsigLimit);
  p.push_back( &M1);
  p.push_back( &M2);
  p.push_back( &M3);
  p.push_back( &MGL);
  p.push_back( &MUL);
  p.push_back( &MB1);
  p.push_back( &MSN);
  p.push_back( &MNTAU);
  p.push_back( &MZ1);
  p.push_back( &MW1);
  p.push_back( &MHL);
  p.push_back( &MUR);
  p.push_back( &MB2);
  p.push_back( &MEL);
  p.push_back( &MTAU1);
  p.push_back( &MZ2);
  p.push_back( &MW2);
  p.push_back( &MHH);
  p.push_back( &MDL);
  p.push_back( &MT1);
  p.push_back( &MER);
  p.push_back( &MTAU2);
  p.push_back( &MZ3);
  p.push_back( &MHA);
  p.push_back( &MDR);
  p.push_back( &MT2);
  p.push_back( &MZ4);
  p.push_back( &MHp);

}

const SusyScan SusyScan::operator*(const double f) const
{
  SusyScan res(*this);
  for (std::vector<double*>::iterator it=res.p.begin(); it!=res.p.end(); ++it)
    **it *= f;
  return res;
}

const SusyScan SusyScan::operator+(const SusyScan& f) const
{
  SusyScan res(*this);
  std::vector<double*>::iterator it=res.p.begin(); 
  std::vector<double*>::const_iterator fi=  f.p.begin(); 
  for (; it!=res.p.end(); ++it, ++fi)
    **it += **fi;
  return res;
}
