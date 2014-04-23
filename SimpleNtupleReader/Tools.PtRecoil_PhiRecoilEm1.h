#ifndef TOOLS_H
#define TOOLS_H

#include "Interface.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TPad.h"

#include <map>
#include <set>
#include <string>
#include <cmath>
#include <vector>
#include <numeric>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sstream>
#include <cassert>


const static double metbins[] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 120, 160, 200, 270, 350, 500}; 
const static int n_metbins = 16;

const static double metphibins[] = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9, 4.0, 4.1, 4.2, 4.3, 4.4, 4.5, 4.6, 4.7, 4.8, 4.9, 5.0, 5.1, 5.2, 5.3, 5.4, 5.5, 5.6, 5.7, 5.8, 5.9, 6.0, 6.1, 6.2, 6.3, 6.4}; 
const static int n_metphibins = 64;

const static double htbins[] = {500,600,700,800,900,1000,1100,1200,1300,1400,1500,1700,2000}; 
const static int n_htbins = 12;

const static double weightbins[] = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.5, 2.0, 3, 5, 10, 20}; 
const static int n_weightbins = 16;

const static double stdbinning[] = {0,100,200,300,400,500,600,700,800,900,1000}; 
const static int n_stdbins = 11;

const static double bins_64_nPi_Pi[] = {-3.2,-3.1,-3.0,-2.9,-2.8,-2.7,-2.6,-2.5,-2.4,-2.3,-2.2,-2.1,-2.0,-1.9,-1.8,-1.7,-1.6,-1.5,-1.4,-1.3,-1.2,-1.1,-1.0,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1, 3.2}; 
const static int n_64 = 64;

const static double bins_50_0_100[]  = {0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,98,100}; 
const static double bins_50_0_1000[] = {0,20,40,60,80,100,120,140,160,180,200,220,240,260,280,300,320,340,360,380,400,420,440,460,480,500,520,540,560,580,600,620,640,660,680,700,720,740,760,780,800,820,840,860,880,900,920,940,960,980,1000}; 
const static double bins_50_0_1500[] = {0,30,60,90,120,150,180,210,240,270,300,330,360,390,420,450,480,510,540,570,600,630,660,690,720,750,780,810,840,870,900,930,960,990,1020,1050,1080,1110,1140,1170,1200,1230,1260,1290,1320,1350,1380,1410,1440,1470,1500}; 
const static int n_50 = 50;

const static double bins_11_0_10[] = {-0.5,0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5,10.5}; 

///Class to print the Status
const static char barspin[4] = {'-','\\','|','/'};
template<typename T>
class Status : public Processor<T> {
  public:
    Status(std::string name):Processor<T>(name){}
    virtual bool Process(T*t,Long64_t i,Long64_t n,double w){
      if (i==0)             std::cout <<"   > "<< n << " events: \n" << std::flush;
      if (n&&(n/times)&&(i%(n/times))==0) std::cout << "   "<<barspin[(i/(n/times))%4]<<" "<<i/(n/times) << "% \r"<< std::flush;
      return true;
    }
  private:
    const static int times = 100;
};

///data helper class for the Plotter
class Histograms {
  public:
    Histograms(const std::string& d,const std::string& l):dir_(d),label_(l){}
    void Add(Histograms* r);
    virtual void Book();
    void Fill(const std::string&s,double c, double w=1.);
    TH1* Get(const std::string&s){return h_[s];}
    std::map<std::string, TH1*> * GetAll(){return &h_;}    
    void Write();
  
  private:
    std::string dir_,label_;
    std::map<std::string, TH1*> h_;    
};

template<typename T>
class Plotter : public Processor<T> {
  public:
    Plotter(std::string dir,std::string n):Processor<T>(n),dir_(dir),weight_(1.0){h_=new Histograms(dir,n);}
    //virtual void Init(){};
    virtual bool Process(T*t,Long64_t i,Long64_t n,double);
    virtual void Terminate(){};
    
    virtual void Book(){h_->Book();}
    virtual void Write(){h_->Write();}
    TH1* GetPlot(const std::string& s){return h_->Get(s);}


  protected:
    std::string dir_;
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
  //h_->Fill( "mht", t->mht, w );
  h_->Fill( "ht", t->ht, w );
  h_->Fill( "em1_pt", t->photons_pt[t->ThePhoton], w );
  h_->Fill( "weight", w );
  
  h_->Fill("em1_phi", t->ThePhotonPhi, w);
  h_->Fill("em1_eta", t->ThePhotonEta, w);
  h_->Fill("em1_ptstar" , t->photons__ptJet[t->ThePhoton], w);
  h_->Fill("em1_thePt" ,  t->ThePhotonPt, w);
  h_->Fill("em1_sigmaIphiIphi", t->photons_sigmaIphiIphi[t->ThePhoton], w);
  h_->Fill("em1_sigmaIetaIeta", t->photons_sigmaIetaIeta[t->ThePhoton], w);
  h_->Fill("em1_r9", t->photons_r9[t->ThePhoton], w);
  h_->Fill("em1_hadTowOverEm", t->photons_hadTowOverEm[t->ThePhoton], w);
  h_->Fill("em1_chargedIso", t->photons_chargedIso[t->ThePhoton], w);
  h_->Fill("em1_neutralIso", t->photons_neutralIso[t->ThePhoton], w);
  h_->Fill("em1_photonIso", t->photons_photonIso[t->ThePhoton], w);
  h_->Fill("em1_conversionSafeVeto", t->photons_conversionSafeVeto[t->ThePhoton], w);
  h_->Fill("em1_pixelseed" , t->photons_pixelseed[t->ThePhoton], w);
  h_->Fill("em1_genInformation", t->photons_genInformation[t->ThePhoton], w);
  h_->Fill("em1_matchedJetIndex", t->photons_matchedJetIndex[t->ThePhoton], w);
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

class Binnings {
 public:
  Binnings(const double *b, int n){binning_ = new std::vector<double>(b,b+n);}
    std::vector<double>* GetArray(){return binning_;}
    virtual int GetNBins(){return binning_->size();}
    virtual double GetBinBorder(int v){return binning_->at(v);}
    virtual int GetBin(double v){
      return std::upper_bound(binning_->begin(),binning_->end(),v)-binning_->begin();
    }
  protected:
    std::vector<double>* binning_;
};

///Yields class is the data structure for the weighter class; it contains different bins
///of event yields e.g. used for the QCD weighting
///this class also defines the binning of the QCD weigting stuff (GetBin)
class Yields{
 public:
    Yields(){}
    Yields(const std::string& s):label_(s){
      /// ------------------------------------------------------------
      ///
      /// QCD Reweighting binning definition
      ///
      /// ------------------------------------------------------------
      //binning_["photon_ptstar"] = new Binnings(bins_50_0_1000, n_50+1);
      binning_["recoil_pt"] = new Binnings(bins_50_0_1500, n_50+1);
      //binning_["phi_met_em1"] = new Binnings(bins_64_nPi_Pi, n_64+1);
      //binning_["phi_mht_em1"] = new Binnings( bins_64_nPi_Pi, n_64+1);
      //binning_["phi_mht_recoil"] = new Binnings( bins_64_nPi_Pi, n_64+1);
      binning_["phi_recoil_em1"] = new Binnings( bins_64_nPi_Pi, n_64+1);
      /// ------------------------------------------------------------
      /// ------------------------------------------------------------
    }

    void Add(Yields*r);
    std::map<int,Yield> * GetYields(){return &yield;}
    Yield * GetYield(int bin){return &yield[bin];}
    virtual int GetBin(float met,float metPhi,float ht,float Sig,
                       float g_pt, float g_eta, float g_phi, 
                       int njets, float *jets_pt, float *jets_eta, float *jets_phi)
		       {
//std::cout <<"    virtual int GetBin(float met,float metPhi,float ht,float Sig," <<std::endl;
      ROOT::Math::PtEtaPhiEVector recoil = Recoil(g_pt, g_eta, g_phi, jets_pt, jets_eta, jets_phi, njets );

      //int bin = binning_["photon_ptstar"]->GetBin( g_pt );
      int bin = binning_["recoil_pt"]->GetBin( Recoil_pt(  &recoil ) );
      //bin += binning_["photon_ptstar"]->GetNBins() * binning_["recoil_pt"]->GetBin( Recoil_pt(  &recoil ) );
      bin += binning_["recoil_pt"]->GetNBins() * 
             binning_["phi_recoil_em1"]->GetBin( DeltaPhi( Recoil_phi( &recoil ), g_phi) );
      //bin += binning_["recoil_pt"]->GetNBins() * 
      //       binning_["phi_recoil_em1"]->GetNBins() * 
      //       binning_["photon_ptstar"]->GetBin( g_pt );
//std::cout <<" DONE   virtual int GetBin(float met,float metPhi,float ht,float Sig," <<std::endl;
      return bin;
    }
    virtual int GetNBins(){
      int n=1;
      for (std::map<std::string,Binnings*>::iterator it=binning_.begin();it!=binning_.end();++it)
        n *= it->second->GetNBins();
      return n;
    }
    int WeightsDimension(){return binning_.size();}
    std::map<std::string,Binnings*> * GetBinning(){return &binning_;}
    
    double Weighted(int b){return yield[b].weighted();}
    unsigned Unweighted(int b){return yield[b].unweighted();}
    double Error(int b){return yield[b].error();}

 protected: 
    std::string label_;
    std::map<int,Yield> yield;    
    std::map<std::string,Binnings*> binning_;
};

///class to collect event yields used for the QCD weighting
template<typename T>
class Weighter : public Processor<T> {
  public:
    Weighter(std::string n):Processor<T>(n){
      yields_=new Yields("QCD_weighting"); 
    }
    //virtual void Init(){};
    virtual bool Process(T*t,Long64_t i,Long64_t n,double);
    virtual void Terminate();//{std::cout << "  Summary Weighter '"<<Processor<T>::name_<<"': "<<yields_->Weighted(0)<<" +- "<<yields_->Error(0)<<"  ("<<yields_->Unweighted(0)<<")"<<std::endl;};

    Yields* GetYields(){return yields_;}

  private:
    Yields* yields_;
};

template<typename T>
bool Weighter<T>::Process(T*t,Long64_t i,Long64_t n,double w)
{
  //bool res = Processor<T>::Process(t,i,n,w);
  //std::cout<< "Weighter<T>::Process(T*t,Long64_t i,Long64_t n,double w)"<<std::endl;
  if (t->met<100.) 
    yields_->GetYield( 
      yields_->GetBin(
        t->met,t->metPhi,t->ht,t->metSig,
        t->ThePhotonPt, t->ThePhotonEta, t->ThePhotonPhi, 
        t->jets_, t->jets_pt, t->jets_eta, t->jets_phi
       )
     )->Add( 1, w );
  //std::cout<< "Weighter<T>::Process(T*t,Long64_t i,Long64_t n,double w) DONE!"<<std::endl;

  return true;
}

template<typename T>
void Weighter<T>::Terminate()
{  
  std::cout << "  Summary Weighter '"<<Processor<T>::name_<<"': "<<yields_->Weighted(0)<<" +- "<<yields_->Error(0)<<"  ("<<yields_->Unweighted(0)<<")"<<std::endl;
/*
  std::map<std::string,Binnings*> * binning = yields_->GetBinning();
  std::string axis[3];
  int i=0;
  for (std::map<std::string,Binnings*>::iterator it=binning->begin();(it!=binning->end()&&i!=3);++it)
    axis[i++]=it->first;
    
  if (yields_->WeightsDimension()==1) {
    TH1 * w = new TH1F( ((std::string)"h1_weight_"+Processor<T>::name_).c_str(),
                    ((std::string)"QCD weighting;"+axis[0]+";weight").c_str(),
                    (*binning)[axis[0]]->GetNBins(),(*binning)[axis[0]]->GetArray()
		  );
    TH1 * we = (TH1F*)w->Clone();		  
    for (int x=0; x<(*binning)[axis[0]]->GetNBins(); ++x){
        w->SetBinContent(x,GetYields()->Weighted(x) );
        we->SetBinContent(x,GetYields()->Error(x) );
    }
    struct stat st={0};
    if(stat(dir_.c_str(),&st)==-1)
       mkdir(dir_.c_str(), 0700);
    if(stat(((std::string)dir_+"/log/").c_str(),&st)==-1)
       mkdir(((std::string)dir_+"/log/").c_str(), 0700);
    TCanvas * c1 = new TCanvas("","",600,600);
    gPad->SetLogy(0);
    it->second->SetMinimum(0);
    it->second->Draw("he");
    c1->SaveAs(((std::string)dir_+"/log/h1_weight_"+Processor<T>::name_+".pdf").c_str());
    delete c1; 
  }
  else if (yields_->WeightsDimension()==2) {
    TH2 * w = new TH1F( ((std::string)"h2_weight_"+Processor<T>::name_).c_str(),
                    ((std::string)"QCD weighting;"+axis[0]+";"+axis[1]).c_str(),
                    (*binning)[axis[0]]->GetNBins(),(*binning)[axis[0]]->GetArray(),
                    (*binning)[axis[1]]->GetNBins(),(*binning)[axis[1]]->GetArray()
		  );
    TH2 * we = (TH2F*)w->Clone();		  
    for (int x=0; x<(*binning)[axis[0]]->GetNBins(); ++x)
      for (int y=0; y<(*binning)[axis[1]]->GetNBins(); ++y){
        w->SetBinContent(x,y,GetYields()->Weighted(x+y*(*binning)[axis[0]]->GetNBins()) );
        we->SetBinContent(x,y,GetYields()->Error(x+y*(*binning)[axis[0]]->GetNBins()) );
      }
    struct stat st={0};
    if(stat(dir_.c_str(),&st)==-1)
       mkdir(dir_.c_str(), 0700);
    if(stat(((std::string)dir_+"/log/").c_str(),&st)==-1)
       mkdir(((std::string)dir_+"/log/").c_str(), 0700);
    TCanvas * c1 = new TCanvas("","",600,600);
    gPad->SetLogy(0);
    it->second->SetMinimum(0);
    it->second->Draw("he");
    c1->SaveAs(((std::string)dir_+"/log/h2_weight_"+Processor<T>::name_+".pdf").c_str());
    delete c1; 
  }
*/
};


///Closure test class ==============================================================================

/*
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
*/

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
      return std::upper_bound(binning_->begin(),binning_->end(),v)-binning_->begin();
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
    void AddRef(std::map<std::string, YieldDataClass*>* ref){
      for (std::map<std::string, YieldDataClass*>::iterator it=ref->begin();it!=ref->end();++it){
        y_[it->first]->Add( it->second );
      }
    }
    void SetBinning(const std::string& s,const double *b,int n){y_[s]->SetBinning(b,n);}
    
    YieldDataClass* GetYieldsRef(const std::string& s){return y_[s];}
    std::map<std::string, YieldDataClass*> * GetRef(){return &y_;}

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
class Closure : public Processor<T> {
  public:
    Closure(std::string d,std::string n):Processor<T>(n),dir_(d),denominator_(0),nominator_(0){
      //yields_ = new ControlYieldsMET(n);
    }
    virtual void Init();
    virtual bool Process(T*t,Long64_t i,Long64_t n,double);
    virtual void Write();
    virtual void Book();

    //Yields* GetYields(){  return yields_;}
    //std::map<std::string, YieldDataClass*>* GetRef(){  return myYields_->GetRef();}
    //void AddRef(std::map<std::string, YieldDataClass*>* ref){  return myYields_->AddRef(ref);}
    void AddRef(std::map<std::string,MyYields*> *y){   
        for (std::map<std::string,MyYields*>::iterator it=y->begin();it!=y->end();++it)
          yields_[it->first]->AddRef( it->second->GetRef() );
    }
    
    void SetDenominator(Yields*y){ denominator_=y;}
    void SetNominator(Yields*y){   nominator_=y;}
    //void SetSignalYields(ControlYieldsMET*y){   signal_=y;}
    //ControlYieldsMET* GetSignalYields(){  return signal_;}
    //void SetSignalHists(Histograms *y){   sighists_=y;}
    //Histograms * GetHists(){  return Plotter<T>::h_;}
    //void SetSignalYields(MyYields *y){   sigYields_=y;}
    void AddSignalYields(std::map<std::string,MyYields*> *y){   
      if (!sig_.size()) sig_= *y;
      else if (y) 
        for (std::map<std::string,MyYields*>::iterator it=y->begin();it!=y->end();++it)
          sig_[it->first]->AddRef( it->second->GetRef() );
    }
    std::map<std::string,MyYields*> * GetYields(){  return &yields_;}

  private:
    std::string dir_;
    //ControlYieldsMET * yields_;      //control region MET>100, loose
    Yields * denominator_, //Nenner: MET<100, loose isolated 
	   * nominator_;   //Z�hler: MET<100, tight isolated
    //ControlYieldsMET * signal_;  //signal region MET>100, tight 
    //Histograms * sighists_; 	   
    std::vector<double> weights_,weighterrors_; //lookuptable for weights to speed up process
    
    
    //MyYields * myYields_;
    //MyYields * sigYields_;

    std::map<std::string,MyYields*> yields_, sig_;
    double weight_;
    
    void BookHistogram(const std::string& s, const std::string title, const double * bins, int nbins){
      //myYields_ = new MyYields(title);  
      //myYields_->Add(s, new YieldDataClass(s));
      //myYields_->SetBinning(s, bins, nbins);
      yields_[s] = new MyYields(title);  
      yields_[s]->Add(s, new YieldDataClass(s));
      yields_[s]->SetBinning(s, bins, nbins);
    }
    void Fill(const std::string& s, double var, double w ){
      yields_[s]->Add(s, yields_[s]->GetBin(s,var), 1, w  );
    }
};


template<typename T>
void Closure<T>::Book()
{
  //Plotter<T>::Book();

std::cout << "void Closure<T>::Book()" << std::endl;

  BookHistogram("met","closure",metbins, n_metbins+1);
  BookHistogram("met_trans","closure",metbins, n_metbins+1);
  BookHistogram("met_paral","closure",metbins, n_metbins+1);
  BookHistogram("ht", "closure",htbins,  n_htbins+1);
  BookHistogram("met_const", "closure", bins_50_0_1000, n_50+1);
  BookHistogram("met_corr", "closure", bins_50_0_1000, n_50+1);
  BookHistogram("met_phi", "closure", metphibins, n_metphibins+1);
  BookHistogram("met_signif", "closure", bins_50_0_100, n_50+1);
  BookHistogram("mht", "closure", bins_50_0_1000, n_50+1);
  BookHistogram("mht_trans", "closure", bins_50_0_1000, n_50+1);
  BookHistogram("mht_paral", "closure", bins_50_0_1000, n_50+1);
  BookHistogram("mht_phi", "closure", metphibins, n_metphibins+1);
  BookHistogram("em1_pt", "closure", bins_50_0_1000, n_50+1);
  BookHistogram("em1_thePt", "closure", bins_50_0_1000, n_50+1);
  BookHistogram("em1_ptstar", "closure", bins_50_0_1000, n_50+1);
  BookHistogram("em1_phi", "closure", bins_64_nPi_Pi, n_64+1);
  BookHistogram("weight", "closure", weightbins, n_weightbins+1 );
  BookHistogram("phi_met_em1", "closure", bins_64_nPi_Pi, n_64+1);
  BookHistogram("phi_mht_em1", "closure", bins_64_nPi_Pi, n_64+1);
  BookHistogram("phi_mht_recoil", "closure", bins_64_nPi_Pi, n_64+1);
  BookHistogram("phi_recoil_em1", "closure", bins_64_nPi_Pi, n_64+1);
  BookHistogram("recoil_ht", "closure",bins_50_0_1500, n_50+1);
  BookHistogram("recoil_pt", "closure",bins_50_0_1500, n_50+1);
  BookHistogram("recoil_phi", "closure",bins_64_nPi_Pi, n_64+1);
  BookHistogram("n_jet", "closure",bins_11_0_10, 12);
  BookHistogram("n_loose", "closure",bins_11_0_10, 12);
  BookHistogram("n_tight", "closure",bins_11_0_10, 12);
  
  if (!denominator_ || !nominator_) return;
  for (int b=0; b<nominator_->GetNBins(); ++b) {
    double d = denominator_->Weighted( b ); //loose
    double de = denominator_->Error( b ); //loose
    double n = nominator_->Weighted( b ); //tight
    double ne = nominator_->Error( b );
    weights_.push_back( (d==0?1.0:n / d) );
    weighterrors_.push_back( (d==0?1.0: sqrt( ne*ne/(d*d) + de*de*n*n/(d*d*d*d) ) ) );
    //std::cout << "  Summary Closure '"<<Processor<T>::name_<<"' weight (bin=" <<b<<") = "
    //          << weights_.back() << " +- "<< weighterrors_.back()<<std::endl;
  }  
}

template<typename T>
void Closure<T>::Init()
{
  //Plotter<T>::Init();
}


template<typename T>
bool Closure<T>::Process(T*t,Long64_t i,Long64_t n,double w)
{
  if (denominator_ && nominator_){
    int bin = nominator_->GetBin(       
           t->met,t->metPhi,t->ht,t->metSig,
           t->ThePhotonPt, t->ThePhotonEta, t->ThePhotonPhi, 
           t->jets_, t->jets_pt, t->jets_eta, t->jets_phi
         ); 
    weight_ = weights_[bin];
  } else 
    weight_ = 1.0;
  //int bin = yields_->GetBin( t->met,0,0 );
  //yields_->GetYield( bin )->Add( 1, Plotter<T>::weight_ * w );
  
  //int mybin = myYields_->GetBin("met",t->met);
  //myYields_->Add("met", myYields_->GetBin("met",t->met), 1, Plotter<T>::weight_ * w  );

  //std::cout << "Closure<T>::Process(T*t,Long64_t i="<<i<<",Long64_t n="<<n<<",double w="<<w<<")"<<std::endl;
  double weight = weight_ * w ;
  
  if (t->photons_>t->kMaxphotons) std::cerr<<"t->photons_="<<t->photons_<<" > t->kMaxphotons="<<t->kMaxphotons<<std::endl;
  if (t->jets_>t->kMaxjets) std::cerr<<"t->jets_="<<t->jets_<<" > t->kMaxjets="<<t->kMaxjets<<std::endl;
  if (t->ThePhoton<0||t->ThePhoton>t->photons_) std::cerr<<"t->ThePhoton="<<t->ThePhoton<<" but t->photons_="<<t->photons_<<std::endl;
 
  
  Fill("met",       t->met, weight);
  Fill("met_trans", CalcTransMet(t->met,t->metPhi-kPI,t->ThePhotonPhi), weight);
  Fill("met_paral", CalcParalMet(t->met,t->metPhi-kPI,t->ThePhotonPhi), weight);
  Fill("ht",        t->ht,  weight);
  Fill("met_const", t->met, weight);
  Fill("met_phi",   t->metPhi, weight);
  Fill("met_signif",t->metSig, weight);
  Fill("em1_pt",    t->photons_pt[t->ThePhoton], weight);
  Fill("em1_thePt", t->ThePhotonPt, weight);
  Fill("em1_ptstar",t->photons__ptJet[t->ThePhoton], weight);
  Fill("em1_phi",   t->ThePhotonPhi, weight);
  Fill("weight",    weight, 1. );
  Fill("phi_met_em1", DeltaPhi(t->metPhi-kPI, t->ThePhotonPhi), weight);

  ROOT::Math::PtEtaPhiEVector recoil = Recoil(t->ThePhotonPt, t->ThePhotonEta, t->ThePhotonPhi, t->jets_pt, t->jets_eta, t->jets_phi, t->jets_ );
  Fill("recoil_ht",   Recoil_ht(t->ThePhotonPt, t->ThePhotonEta, t->ThePhotonPhi, t->jets_pt, t->jets_eta, t->jets_phi, t->jets_ ), weight );
  Fill("recoil_pt",   Recoil_pt(  &recoil ), weight );
  Fill("recoil_phi",  Recoil_phi( &recoil ), weight );
  Fill("phi_recoil_em1", DeltaPhi( Recoil_phi( &recoil ), t->ThePhotonPhi), weight);

  float g_pt  = t->ThePhotonPt;
  float g_eta = t->ThePhotonEta;
  float g_phi = t->ThePhotonPhi;
  float mht = Mht(g_pt,g_eta,g_phi, t->jets_pt, t->jets_eta, t->jets_phi, t->jets_ );
  float mht_phi = MhtPhi(g_pt,g_eta,g_phi, t->jets_pt, t->jets_eta, t->jets_phi, t->jets_ );
  Fill("mht",mht, weight );
  Fill("mht_phi",mht_phi, weight );
  Fill("mht_trans", CalcTransMet(mht,mht_phi,g_phi), weight);
  Fill("mht_paral", CalcParalMet(mht,mht_phi,g_phi), weight);
  Fill("phi_mht_em1",   DeltaPhi(mht_phi, g_phi), weight);
  Fill("phi_mht_recoil",DeltaPhi(mht_phi, Recoil_phi( &recoil )), weight);
  Fill("met_corr",    CorectedMet(t->met,t->metPhi-kPI,t->photons_pt[t->ThePhoton], t->photons_eta[t->ThePhoton], t->photons_phi[t->ThePhoton], g_pt ,g_eta, g_phi ), weight);
  Fill("n_jet",       JetMult(  g_pt, g_eta, g_phi, t->jets_pt, t->jets_eta, t->jets_phi, t->jets_), weight);
  Fill("n_loose",     LooseMult(t->photons_,t->photons_pt, t->photons__ptJet, t->photons_phi, t->photons_eta,t->photons_hadTowOverEm,t->photons_sigmaIetaIeta,t->photons_chargedIso,t->photons_neutralIso,t->photons_photonIso,t->photons_pixelseed), weight);
  Fill("n_tight",     TightMult(t->photons_,t->photons_pt, t->photons__ptJet, t->photons_phi, t->photons_eta,t->photons_hadTowOverEm,t->photons_sigmaIetaIeta,t->photons_chargedIso,t->photons_neutralIso,t->photons_photonIso,t->photons_pixelseed), weight);

  //std::cout << "Closure<T>::Process(T*t,Long64_t i="<<i<<",Long64_t n="<<n<<",double w="<<w<<") Done!"<<std::endl;
  
  return true;//Processor<T>::Process(t,i,n,w);
}


void RatioPlot(TH1*a, TH1*b, const std::string& dir, const std::string& file, const std::string& t);

template<typename T>
void Closure<T>::Write()
{
  for (std::map<std::string,MyYields*>::iterator it=yields_.begin();it!=yields_.end();++it) {
    TH1 * pred = it->second->GetPlot(it->first);
    pred->SetTitle("Prediction");
    TH1 * sighist = 0;
    if (sig_[it->first]) {
      sighist = sig_[it->first]->GetPlot( it->first );  
      sighist->SetTitle("Direct simulation");    
      RatioPlot(sighist, pred, dir_, Closure<T>::name_+"_"+it->first, Processor<T>::name_);
    }  
  }

  if (!denominator_ || !nominator_) return;

  std::map<std::string,Binnings*> * binning = denominator_->GetBinning();
  std::string axis[3];
  int i=0;
  for (std::map<std::string,Binnings*>::iterator it=binning->begin();(it!=binning->end()&&i!=3);++it)
    axis[i++]=it->first;
    
//std::cout <<"void Closure<T>::Write() dim="<< denominator_->WeightsDimension()<<std::endl;
  if (denominator_->WeightsDimension()==1) {
    std::stringstream ss;
    ss  <<"h1_weight_"<<Processor<T>::name_<<"_" << plotnr++;
    std::vector<double>* v1 = (*binning)[axis[0]]->GetArray();
    int n1=v1->size();
    double a1[n1];
    for (int i=0; i<n1; ++i)a1[i]=(*v1)[i];
    TH1 * w = new TH1F( ss.str().c_str(),
                    ((std::string)"QCD weighting;"+axis[0]+";weight").c_str(),
                    n1-1,a1
		  );
    for (int x=0; x<(*binning)[axis[0]]->GetNBins(); ++x){
        w->SetBinContent(x,weights_[x] );
        w->SetBinError(x,weighterrors_[x] );
    }
    struct stat st={0};
    if(stat(dir_.c_str(),&st)==-1)
       mkdir(dir_.c_str(), 0700);
    if(stat(((std::string)dir_+"/log/").c_str(),&st)==-1)
       mkdir(((std::string)dir_+"/log/").c_str(), 0700);
    TCanvas * c1 = new TCanvas("","",600,600);
    c1->cd();
    gPad->SetLogy(0);
    w->Draw("he");
    c1->SaveAs(((std::string)dir_+"/log/h1_weight_"+Processor<T>::name_+".pdf").c_str());
    delete w;
    delete c1; 
  }
  else if (denominator_->WeightsDimension()==2) {

    std::stringstream ss;
    ss  <<"h2_weight_"<<Processor<T>::name_<<"_" << plotnr++;
    std::vector<double>* v1 = (*binning)[axis[0]]->GetArray();
    int n1=v1->size();
    double a1[n1];
    for (int i=0; i<n1; ++i)a1[i]=(*v1)[i];
    std::vector<double>* v2 = (*binning)[axis[1]]->GetArray();
    int n2=v2->size();
    double a2[n2];
    for (int i=0; i<n2; ++i)a2[i]=(*v2)[i];

    TH2F * w = new TH2F( ss.str().c_str(),
                    ((std::string)"QCD weighting;"+axis[0]+";"+axis[1]).c_str(),
                    n1,a1,n2,a2
		  );
    TH2F * we = (TH2F*)w->Clone();		  
    for (int x=0; x<(*binning)[axis[0]]->GetNBins(); ++x)
      for (int y=0; y<(*binning)[axis[1]]->GetNBins(); ++y){
        w->SetBinContent(x,y,weights_[x+y*(*binning)[axis[0]]->GetNBins() ]);
        we->SetBinContent(x,y,weighterrors_[x+y*(*binning)[axis[0]]->GetNBins() ]);
      }
    struct stat st={0};
    if(stat(dir_.c_str(),&st)==-1)
       mkdir(dir_.c_str(), 0700);
    if(stat(((std::string)dir_+"/log/").c_str(),&st)==-1)
       mkdir(((std::string)dir_+"/log/").c_str(), 0700);
    TCanvas * c1 = new TCanvas("cw1","cw1",600,600);
    c1->cd();
    gPad->SetLogy(0);
    w->Draw("Colz");
    c1->SaveAs(((std::string)dir_+"/log/h2_weight_"+Processor<T>::name_+".pdf").c_str());
    we->Draw("Colz");
    c1->SaveAs(((std::string)dir_+"/log/h2_weighterror_"+Processor<T>::name_+".pdf").c_str());
    delete we;
    delete w;
    delete c1; 

  }
//std::cout <<"void Closure<T>::Write() dim="<< std::endl;

}



////Cutter 
template<typename T>
class Cutter : public Processor<T> {
  public:
    Cutter(std::string n):Processor<T>(n),d_tot(0),d_pass(0),i_tot(0), i_pass(0){}
    //virtual void Init(){};
    virtual bool Process(T*t,Long64_t i,Long64_t n,double w) {
      ++i_tot;
      d_tot += w;
      //ROOT::Math::PtEtaPhiEVector recoil = Recoil(t->ThePhotonPt, t->ThePhotonEta, t->ThePhotonPhi, t->jets_pt, t->jets_eta, t->jets_phi, t->jets_ );
         //||
	// || t->photons__ptJet[t->ThePhoton]<=0
         // Recoil_pt(  &recoil )<150.
      if ( t->ThePhotonPt<110.  ) {
	return false;
      }	
      ++i_pass;
      d_pass += w;
      return true;
    }
    virtual void Terminate(){
      std::cout << "  Summary Cutter '"<<Processor<T>::name_<<"' Terminate: "<<d_pass<<" ("<<i_pass<<") / "
                << d_tot << " ("<<i_tot<<") passed all cuts";
      if (d_tot&&i_tot) std::cout << ", i.e. "<< d_pass/d_tot*100.<<"% ("<<100.*i_pass/i_tot<<"%)."<<std::endl;
    }
  protected:
   double d_tot, d_pass;
   int    i_tot, i_pass;  
};



////Cutter 
template<typename T>
class Cutter_looseID : public Cutter<T> {
  public:
    Cutter_looseID(std::string n):Cutter<T>(n){}
    //virtual void Init(){};
    virtual bool Process(T*t,Long64_t i,Long64_t n,double w) {
      ++Cutter<T>::i_tot;
      Cutter<T>::d_tot += w;
      
      double found_pt = 0;
      for (int i=0; i<t->photons_;++i) {
        if ( loose_isolated(t->photons_pt[i], t->photons__ptJet[i], t->photons_phi[i], t->photons_eta[i],
                           t->photons_hadTowOverEm[i],t->photons_sigmaIetaIeta[i],
			   t->photons_chargedIso[i],t->photons_neutralIso[i],t->photons_photonIso[i])
		   && t->photons_pixelseed[i]==0		
	   ) 
	{
	   double pt = (t->photons__ptJet[i]>0?t->photons__ptJet[i]:t->photons_pt[i]);
  	   if (pt>found_pt) {
	     found_pt=pt; 
	     t->ThePhoton = i; 
	     t->ThePhotonPt = pt; 
	     if (t->photons__ptJet[i]>0) {
	       assert(t->photons_matchedJetIndex[i]>=0 && t->photons_matchedJetIndex[i]<t->jets_);
	       t->ThePhotonPhi = t->jets_phi[t->photons_matchedJetIndex[i]]; 
	       t->ThePhotonEta = t->jets_eta[t->photons_matchedJetIndex[i]];
	     } else {
	       t->ThePhotonPhi = t->photons_phi[i]; 
	       t->ThePhotonEta = t->photons_eta[i];
	     }
	   } 
	}   	
      }	
      if (found_pt==0 
          || 
          !LeptonVeto(t->electrons_, t->electrons_pt, t->electrons_eta,t->muons_, t->muons_pt, t->muons_eta)
         ) 
	 return false;
      ++Cutter<T>::i_pass;
      Cutter<T>::d_pass += w;
      return true;
    }
};


////Cutter 
template<typename T>
class Cutter_tightID : public Cutter<T> {
  public:
    Cutter_tightID(std::string n):Cutter<T>(n){}
    //virtual void Init(){};
    virtual bool Process(T*t,Long64_t i,Long64_t n,double w) {
      ++Cutter<T>::i_tot;
      Cutter<T>::d_tot += w;
      
      double found_pt = 0;
      for (int i=0; i<t->photons_;++i) {
        if ( tight_isolated(t->photons_pt[i], t->photons__ptJet[i], t->photons_phi[i], t->photons_eta[i],
                           t->photons_hadTowOverEm[i],t->photons_sigmaIetaIeta[i],
			   t->photons_chargedIso[i],t->photons_neutralIso[i],t->photons_photonIso[i])
             && t->photons_pixelseed[i]==0		
	   ) 
	{
	   double pt = (t->photons__ptJet[i]>0?t->photons__ptJet[i]:t->photons_pt[i]);
  	   if (pt>found_pt) {
	     found_pt=pt; 
	     t->ThePhoton = i; 
	     t->ThePhotonPt = pt; 
	     if (t->photons__ptJet[i]>0) {
	       assert(t->photons_matchedJetIndex[i]>=0 && t->photons_matchedJetIndex[i]<t->jets_);
	       t->ThePhotonPhi = t->jets_phi[t->photons_matchedJetIndex[i]]; 
	       t->ThePhotonEta = t->jets_eta[t->photons_matchedJetIndex[i]];
	     } else {
	       t->ThePhotonPhi = t->photons_phi[i]; 
	       t->ThePhotonEta = t->photons_eta[i];
	     }
	   } 
	}   	
      }	
      if (found_pt==0 
          || 
          !LeptonVeto(t->electrons_, t->electrons_pt, t->electrons_eta,t->muons_, t->muons_pt, t->muons_eta)
         ) 
	 return false;
      ++Cutter<T>::i_pass;
      Cutter<T>::d_pass += w;
      return true;
    }
};


///Doublicate Event Filter -------------------------------------------------------------------------
///Helper class for doublicate event filter
struct EventId { 
   EventId(UInt_t nr, UInt_t lumi, UInt_t run):nr(nr),lumi(lumi),run(run){} 
   UInt_t nr, lumi, run; 
   bool operator<(const EventId& rh) const {
     return (nr!=rh.nr? nr<rh.nr : (lumi!=rh.lumi? lumi<rh.lumi : run<rh.run) );
   }
};


////Doublecate event filter 
template<typename T>
class DoubleCountFilter : public Processor<T> {
  public:
  
    DoubleCountFilter(std::string n):Processor<T>(n),d_tot(0),d_pass(0),i_tot(0), i_pass(0){}
    //virtual void Init(){};
    virtual bool Process(T*t,Long64_t i,Long64_t n,double w) {
      ++i_tot;
      d_tot += w;
      if (evts_.insert( EventId (t->eventNumber, t->runNumber, t->luminosityBlockNumber) ).second){
        ++i_pass;
        d_pass += w;
        return true;
      }	
      else
        return false;	
    }
    virtual void Terminate(){
      std::cout << "  Doublicate Event Filter '"<<Processor<T>::name_<<"' Terminate: "<<d_pass<<" ("<<i_pass<<") / "
                << d_tot << " ("<<i_tot<<") passed all cuts";
      if (d_tot&&i_tot) std::cout << ", i.e. "<< d_pass/d_tot*100.<<"% ("<<100.*i_pass/i_tot<<"%)."<<std::endl;
    }
    
    std::set<EventId> * Get(){return &evts_;}
    void Set(std::set<EventId>*l){evts_.insert(l->begin(),l->end()); }
  protected:
   std::set<EventId> evts_;
   double d_tot, d_pass;
   int    i_tot, i_pass;  
};


#endif