#ifndef HANDLEHISTS
#define HANDLEHISTS

#include <string>
#include <vector>
#include <map>

class TH1;
class TGraph;
class ConfigFile;

class handleHists
{
 public:
  handleHists( ConfigFile* );
  ~handleHists();
  
  void PlotStackedBackground(const std::string out);
  void PlotAllSystematics(const std::string out);
  void PlotHistograms(const std::string out);
  void Print();
  
 private:
  TH1 * GetHist(const std::string file, const std::string hist, const std::string treename="", const std::string uncertainty="", const std::string direction="");
  TH1 * GetSyst(const std::string, const TH1*h1, const TH1*h2=0, const TH1*h3=0);
  TGraph * MakeBand(const TH1*h1, const TH1*h2);
 
  std::vector<TH1*> _qcd_hists; 
  std::vector<TH1*> _ttbar_hists; 
  std::vector<TH1*> _znunu_hists; 
  std::vector<TH1*> _signal_hists; 
  std::vector<TH1*> _data_hists; 
  
  std::map<const TH1*, std::vector<TH1*> > _syst_up_corr;   //uncertainties for the 
  std::map<const TH1*, std::vector<TH1*> > _syst_dn_corr;   //histogram "key", e.g. 
  std::map<const TH1*, std::vector<TH1*> > _syst_up_uncorr; //_backgd_hists[0]
  std::map<const TH1*, std::vector<TH1*> > _syst_dn_uncorr;
  
  std::string ndata, nsignal, nznunu, nttbar, nqcd;
  std::vector<std::string> vars, corr_unc, uncorr_unc;
  int _plotindex; //internal counter to make internal hist names unique
  
  bool _plotSystematics; //switch to plot all systematic uncertainties
  bool _showSignalBand; //switch to show the signal uncert
  bool _showDiff;
  double _SumOfDiffs;
};

#endif
