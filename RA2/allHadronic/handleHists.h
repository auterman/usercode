#ifndef HANDLEHISTS
#define HANDLEHISTS

#include <string>
#include <vector>
#include <map>
#include <TH1.h>


class ConfigFile;

class handleHists
{
 public:
  handleHists( ConfigFile* );
  ~handleHists();
  
  void PlotHistograms(const std::string out);
  void Print();
  
 private:
  TH1 * GetHist(const std::string file, const std::string hist);
  TH1 * GetSyst(const std::string, const TH1*h1, const TH1*h2=0, const TH1*h3=0);
 
  std::vector<TH1*> _qcd_hists; 
  std::vector<TH1*> _ttbar_hists; 
  std::vector<TH1*> _znunu_hists; 
  std::vector<TH1*> _signal_hists; 
  std::vector<TH1*> _data_hists; 
  
  std::map<const TH1*, std::vector<TH1*> > _syst_up_corr;   //uncertainties for the 
  std::map<const TH1*, std::vector<TH1*> > _syst_dn_corr;   //histogram "key", e.g. 
  std::map<const TH1*, std::vector<TH1*> > _syst_up_uncorr; //_backgd_hists[0]
  std::map<const TH1*, std::vector<TH1*> > _syst_dn_uncorr;
  
  int _plotindex; //internal counter to make internal hist names unique
};

#endif
