#ifndef TOOLS_H
#define TOOLS_H

#include "Interface.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TPad.h"


#include <map>
#include <string>
#include <cmath>
#include <vector>
#include <numeric>

const static double metbins[] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 120, 160, 200, 270, 350, 500}; 
const static int n_metbins = 16;
const static std::vector<double> m_b(metbins,metbins+n_metbins);
static int plotnr = 0;

///data helper class for the Plotter
class Histograms {
  public:
    Histograms(const std::string& l):label_(l){}
    void Add(Histograms* r);
    void Book();
    void Fill(const std::string&s,double c, double w=1.);
    TH1* Get(const std::string&s){return h_[s];}
    std::map<std::string, TH1*> * GetAll(){return &h_;}    
    void Write();
  
  private:
    std::string label_;
    std::map<std::string, TH1*> h_;    
};

template<typename T>
class Plotter : public Processor<T> {
  public:
    Plotter(std::string n):Processor<T>(n),weight_(1.0){h_=new Histograms(n);}
    //virtual void Init(){};
    virtual bool Process(T*t,Long64_t i,Long64_t n,double);
    virtual void Terminate(){};
    
    virtual void Book(){h_->Book();}
    virtual void Write(){h_->Write();}
    TH1* GetPlot(const std::string& s){return h_->Get(s);}


  protected:
    double weight_;
    Histograms * h_;
};



template<typename T>
bool Plotter<T>::Process(T*t,Long64_t i,Long64_t n,double w)
{
  w *= weight_;
  bool res = Processor<T>::Process(t,i,n,w);
  
  if (h_) {
  h_->Fill( "met", t->met, w );
  h_->Fill( "met_const", t->met, w );
  h_->Fill( "mht", t->mht, w );
  h_->Fill( "ht", t->ht, w );
  h_->Fill( "em1_pt", t->photons_pt[0], w );
  h_->Fill( "weight", w );
  }
  return res;
}


///helper class for Yield
template<typename T> struct square
{
    T operator()(const T& Left, const T& Right) const {   
        return (Left + Right*Right);
    }
};

///Yield class can calculate statistical uncertaities for a set of weighted events
class Yield{ 
 public:
  Yield(){}
  //Set
  Yield(unsigned n, double weight){Add(n,weight);}
  void Add(unsigned n, double weight){for (unsigned i=0;i<n;++i)w.push_back(weight);}
  void Add(std::vector<double> * r){w.insert(w.end(),r->begin(),r->end());}
  //Get
  unsigned unweighted(){return w.size();}
  double weighted(){ return std::accumulate(w.begin(),w.end(),0.);}
  double error(){return sqrt( std::accumulate(w.begin(),w.end(),0.,square<double>()) );}
  std::vector<double> * Get(){return &w;} 

 private:
    std::vector<double> w;
};

///Yields class is the data structure for the weighter class; it contains different bins
///of event yields e.g. used for the QCD weighting
///this class also defines the binning of the QCD weigting stuff (GetBin)
class Yields{
 public:
    Yields(){}
    Yields(const std::string& s):label_(s){}

    void Add(Yields*r);
    std::map<int,Yield> * GetYields(){return &yield;}
    Yield * GetYield(int bin){return &yield[bin];}
    virtual int GetBin(double met, double pt, double ht){return 0;}//i.e. just one single bin for now.
    virtual int GetNBins(){return 1;}
    
    double Weighted(int b){return yield[b].weighted();}
    double Unweighted(int b){return yield[b].unweighted();}
    double Error(int b){return yield[b].error();}

 protected: 
    std::string label_;
    std::map<int,Yield> yield;    
};

///class to collect event yields used for the QCD weighting
template<typename T>
class Weighter : public Processor<T> {
  public:
    Weighter(std::string n):Processor<T>(n){yields_=new Yields(); }
    //virtual void Init(){};
    virtual bool Process(T*t,Long64_t i,Long64_t n,double);
    virtual void Terminate(){std::cout << "Weighter '"<<Processor<T>::name_<<"': "<<yields_->Weighted(0)<<" +- "<<yields_->Error(0)<<"  ("<<yields_->Unweighted(0)<<")"<<std::endl;};

    Yields* GetYields(){return yields_;}

  private:
    Yields * yields_;
};

template<typename T>
bool Weighter<T>::Process(T*t,Long64_t i,Long64_t n,double w)
{
  bool res = Processor<T>::Process(t,i,n,w);

  if (t->met<100.) yields_->GetYield( yields_->GetBin(t->met,t->photons_pt[0],t->ht) )->Add( 1, w );

  return res;
}


///Closure test class ==============================================================================

class ControlYieldsMET : public Yields
{
  public:
    ControlYieldsMET(const std::string& s):Yields(s){ 
      h_ = new TH1F("hc_met","",n_metbins,metbins);}
    virtual int GetBin(double met, double pt, double ht){
      return std::upper_bound(m_b.begin(),m_b.end(),met)-m_b.begin();
      //return h_->GetXaxis()->FindBin(met);
    }
  
  protected:
    TH1 * h_;
    
};


class YieldDataClass : public Yields
{
  public:
    YieldDataClass():Yields(){ }
    YieldDataClass(const std::string& s):Yields(s){ }
    virtual ~YieldDataClass(){delete binning_;}
    void SetBinning(const double *b,int n){binning_ = new std::vector<double>(b,b+n);}
    virtual int GetNBins(){return binning_->size();}
    virtual double GetBinBorder(int v){return binning_->at(v);}
    virtual int GetBin(double v){
      //std::cout << "GetBin:: met = "<<v<<" in bin "<<  std::upper_bound(binning_->begin(),binning_->end(),v)-binning_->begin()-1<<std::endl;
      return std::upper_bound(binning_->begin(),binning_->end(),v)-binning_->begin()-1;
    }

  protected:
    std::vector<double>* binning_;
};

///Improvement of above class
class MyYields
{
  public:
    MyYields(const std::string& s):label_(s){} 
    
    void Add(const std::string& s, YieldDataClass* d){y_[s]=d;}
    void Add(const std::string& s, int bin, int n, double w){y_[s]->GetYield(bin)->Add(n,w);}
    void SetBinning(const std::string& s,const double *b,int n){y_[s]->SetBinning(b,n);}
    
    int GetNBins(const std::string& s){return y_[s]->GetNBins();}
    int GetBin(  const std::string& s, double v){return y_[s]->GetBin(v);}
    double GetBinBorder(const std::string& s, int v){return y_[s]->GetBinBorder(v);}
    TH1 * GetPlot(const std::string& s);
    double Weighted(const std::string& s, int b){return y_[s]->Weighted(b);}
    int  Unweighted(const std::string& s, int b){return y_[s]->Unweighted(b);}
    double Error(   const std::string& s, int b){return y_[s]->Error(b);}
      
  protected:
    std::string label_; 
    std::map<std::string, YieldDataClass*> y_;
  
};

///Class for the closure
template<typename T>
class Closure : public Plotter<T> {
  public:
    Closure(std::string n):Plotter<T>(n),denominator_(0),nominator_(0),sighists_(0){
      yields_ = new ControlYieldsMET(n);
    }
    virtual void Init();
    virtual bool Process(T*t,Long64_t i,Long64_t n,double);
    virtual void Write();

    Yields* GetYields(){  return yields_;}
    void SetDenominator(Yields*y){ denominator_=y;}
    void SetNominator(Yields*y){   nominator_=y;}
    void SetSignalYields(ControlYieldsMET*y){   signal_=y;}
    ControlYieldsMET* GetSignalYields(){  return signal_;}
    void SetSignalHists(Histograms *y){   sighists_=y;}
    Histograms * GetHists(){  return Plotter<T>::h_;}

  private:
    ControlYieldsMET * yields_;      //control region MET>100, loose
    Yields * denominator_, //Nenner: MET<100, loose isolated 
	   * nominator_;   //Zähler: MET<100, tight isolated
    ControlYieldsMET * signal_;  //signal region MET>100, tight 
    Histograms * sighists_; 	   
    std::vector<double> weights_; //lookuptable for weights to speed up process
    
    
    MyYields * myYields_;
};


template<typename T>
void Closure<T>::Init()
{
  Plotter<T>::Init();

  myYields_ = new MyYields("Data class containing the closure yields");  
  myYields_->Add("met", new YieldDataClass("met"));
  myYields_->SetBinning("met", metbins, n_metbins);
  
  if (!denominator_ || !nominator_) return;
  for (int b=0; b<nominator_->GetNBins(); ++b) {
    double d = denominator_->Weighted( b );
    weights_.push_back( (d==0?1.0:nominator_->Weighted( b ) / d) );
    std::cout << "Closure '"<<Plotter<T>::name_<<"' weight (bin=" <<b<<") = "<< weights_.back() << std::endl;
  }  
}

template<typename T>
bool Closure<T>::Process(T*t,Long64_t i,Long64_t n,double w)
{
  if (denominator_ && nominator_){
    int bin = nominator_->GetBin( t->met,0,0 ); //nominator_->GetBin( t->met,t->photons_pt[0],t->ht );
    Plotter<T>::weight_ = weights_[bin];
  } else 
    Plotter<T>::weight_ = 1.0;
  int bin = yields_->GetBin( t->met,0,0 );
  yields_->GetYield( bin )->Add( 1, Plotter<T>::weight_ * w );
  
  myYields_->Add("met", myYields_->GetBin("met",t->met), 1, Plotter<T>::weight_ * w  );
  
  return Plotter<T>::Process(t,i,n,w);
}


template<typename T>
void Closure<T>::Write()
{
  if (!Plotter<T>::h_->Get( "met" )) {
    std::cerr<<"ERROR: Closure<T>::Write(): Your forgot to book the histograms!"<<std::endl;
    exit(2);
  }
  for (std::map<int,Yield>::iterator it=yields_->GetYields()->begin(); 
       it!=yields_->GetYields()->end();++it) { 
    Plotter<T>::h_->Get( "met" )->SetBinContent( it->first, it->second.weighted() );
    Plotter<T>::h_->Get( "met" )->SetBinError( it->first, it->second.error() );
  }

  //Plotter<T>::Write();
  TCanvas * c1 = new TCanvas("","",600,600);
  std::string label = Plotter<T>::name_;
     
  gPad->SetLogy(0);
  //Plotter<T>::h_->Get( "met" )->Draw("he");
  TH1 * met = myYields_->GetPlot("met");
  met->Draw("he");
  if (sighists_ && sighists_->Get( "met" )) {
    sighists_->Get( "met" )->SetMarkerStyle(8);
    sighists_->Get( "met" )->Draw("pe,same");
  }  
  c1->SaveAs(((std::string)"plots/"+label+"_met.pdf").c_str());

  gPad->SetLogy(1);
  //Plotter<T>::h_->Get( "met" )->Draw("he");
  met->Draw("he");
  if (sighists_ && sighists_->Get( "met" ))
    sighists_->Get( "met" )->Draw("pe,same");
  c1->SaveAs(((std::string)"plots/"+label+"_met_log.pdf").c_str());

  delete c1;
}


#endif
