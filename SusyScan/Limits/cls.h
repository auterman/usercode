#ifndef CLS_H
#define CLS_H

#include <string>
#include <vector>

#ifndef ROOT_TObjArray
#include "TObjArray.h"
#endif
#include "TVectorDfwd.h"

class TH1;
class TH1D;
class TObjArray;
class ConfigFile;

class cls
{
 public:
  cls();
  cls(std::string n,TH1*s,TH1*b,TH1*d); //One signal histogram
  cls(std::string n,std::string ScanParName,std::vector<double>ScanPar,std::vector<TH1*>s,TH1*b,TH1*d); //Many signal histogram
  virtual ~cls(){};
  
  void SetOutputFileName(const std::string n){outputfilename_=n;};
  void SetSignal(TH1 const &s){signal_=&s;};
  void SetBackground(TH1 const &b){backgd_=&b;};
  void SetData(TH1 const &d){data_=&d;};
  void SetUncertainty(TH1 *es,TH1 *eb,TObjArray *names){esup_=esdn_=es; ebup_=ebdn_=eb;names_=names;syst_=true;};
  void SetUncertainty(TH1 *esup,TH1 *esdn,TH1 *ebup,TH1 *ebdn, TObjArray *names){esup_=esup;esdn_=esdn; ebup_=ebup;ebdn_=ebdn; names_=names;syst_=true;};

  void WriteResult(const std::string out);
  void WriteResult(ConfigFile *);
  void WriteConfidence(ConfigFile *);
  double GetObservedXsecLimit(double cl, double min=0, double max=1000);
  double GetExpectedXsecLimit(double cl, double min=0, double max=1000);
  void Draw(bool doeps=false);
  void DrawVsSignalParam(bool doeps=false);
  void DrawVsXsec(bool doeps=false);
  void SetStat(bool stat){stat_=stat;};
  void SetNMC(int n){fNMC_=n;};
  double operator()(const double x, const double * par);

 private:
  TH1 * MakePseudoData(TH1 const*b, TH1 const*s=0);
  void GetXandYbyInterpolation( const double x, TH1D *& signal);
  double GetTotalStatError(const TH1* arg);
  void Sort(double &v1, double &v2);
  
  bool stat_;
  bool syst_;
  bool isPseudoData;  
  bool do1DScan_;
  int plotindex_;
  int fNMC_;
  std::string outputfilename_, ScanParName_;
  std::vector<double> ScanPar_;
  std::vector <TH1*> signals_;
  TH1 const *signal_, *backgd_, *data_;
  TObjArray* names_;//systematics with same names are correlated
  TH1 const *esup_, //systematic uncertainty *s*ignal u*p*
            *esdn_, //systematic uncertainty *s*ignal dow*n*
            *ebup_, //systematic uncertainty *b*ackground u*p*
            *ebdn_; //systematic uncertainty *b*ackground dow*n*
};


#endif
