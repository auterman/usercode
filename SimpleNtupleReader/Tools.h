#ifndef TOOLS_H
#define TOOLS_H
#include "Interface.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TStyle.h"
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




const static double nu_bins[] = {0,1,1.5,2.25,3.375,5.0625,7.59375,11.3906,17.0859,25.6289,38.4434,57.665,86.4976, 120,150,180,210,240,270,300,330,360,390,420,450,480,510,540,570,600,630,660,690,720,750,780,810,840,870,900,930,960,990,1020,1050,1080,1110,1140,1170,1200,1230,1260,1290,1320,1350,1380,1410,1440,1470,1500};
const static int n_nu_bins = 59;

const static double fak1p5_bins[] = {0,1,1.5,2.25,3.375,5.0625,7.59375,11.3906,17.0859,25.6289,38.4434,57.665,86.4976,129.746,194.62,291.929,437.894,656.841,985.261,1477.89,2000};
const static int n_fak1p5_bins = 20;

const static double metbins[] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 100, 120, 160, 200, 270, 350, 500};
const static int n_metbins = 15;

const static double newmetbins[] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 120, 150, 200, 260, 340, 430, 550};
const static int n_newmetbins = 17;

const static double fibonacci[] = {0,5,10,15,25,40,65,100,170,285,455,740};
const static int n_fibonacci = 11;

const static double met_optim[] = {0, 10, 20, 30, 40, 50, 60, 70, 75, 80, 85, 90, 95, 100, 105, 120, 160, 200, 270, 350, 500};
const static int n_met_optim = 20;

const static double metphibins[] = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9, 4.0, 4.1, 4.2, 4.3, 4.4, 4.5, 4.6, 4.7, 4.8, 4.9, 5.0, 5.1, 5.2, 5.3, 5.4, 5.5, 5.6, 5.7, 5.8, 5.9, 6.0, 6.1, 6.2, 6.3, 6.4};
const static int n_metphibins = 64;

const static double htbins[] = {500,600,700,800,900,1000,1100,1200,1300,1400,1500,1700,2000};
const static double htbins2[] = {500 , 520 , 540 , 560 , 580 , 600 , 620 , 640 , 660 , 680 , 700 , 720 , 740 , 760 , 780 , 800 , 820 , 840 , 860 , 880 , 900 , 920 , 940 , 960 , 980 , 1000 , 1020 , 1040 , 1060 , 1080 , 1100 , 1120 , 1140 , 1160 , 1180 , 1200 , 1220 , 1240 , 1260 , 1280 , 1300 , 1320 , 1340 , 1360 , 1380 , 1400 , 1420 , 1440 , 1460 , 1480 , 1500 , 1520 , 1540 , 1560 , 1580 , 1600 , 1620 , 1640 , 1660 , 1680 , 1700 , 1720 , 1740 , 1760 , 1780 , 1800 , 1820 , 1840 , 1860 , 1880 , 1900 , 1920 , 1940 , 1960 , 1980 , 2000 , 2020 , 2040 , 2060 , 2080 , 2100 , 2120 , 2140 , 2160 , 2180 , 2200 , 2220 , 2240 , 2260 , 2280 , 2300 , 2320 , 2340 , 2360 , 2380 , 2400 , 2420 , 2440 , 2460 , 2480, 2500};
const static int n_htbins = 12;
const static int n_htbins2 = 100;

const static double weightbins[] = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.5, 2.0, 3, 5, 10, 20};
const static int n_weightbins = 16;

const static double stdbinning[] = {0,100,200,300,400,500,600,700,800,900,1000};
const static int n_stdbins = 11;

const static double bins_64_nPi_Pi[] = {-3.2,-3.1,-3.0,-2.9,-2.8,-2.7,-2.6,-2.5,-2.4,-2.3,-2.2,-2.1,-2.0,-1.9,-1.8,-1.7,-1.6,-1.5,-1.4,-1.3,-1.2,-1.1,-1.0,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1, 3.2};
const static int n_64 = 64;
const static double bins_16_nPi_Pi[] = {-3.2,-2.8,-2.4,-2.0,-1.6,-1.2,-0.8,-0.4,0,0.4,0.8,1.2,1.6,2.0,2.4,2.8,3.2};
const static double bins_8_nPi_Pi[] = {-3.2,-2.4,-1.6,-0.8,0,0.8,1.6,2.4,3.2};
const static int n_8 = 8;

const static double bins_50_0_100[]  = {0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,98,100};
const static double bins_50_0_200[]  = {0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300,310,320,330,340,350,360,370,380,390,400,410,420,430,440,450,460,470,480,490,500};
const static double bins_50_0_500[]  = {0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300,310,320,330,340,350,360,370,380,390,400,410,420,430,440,450,460,470,480,490,500};
const static double bins_50_0_1000[] = {0,20,40,60,80,100,120,140,160,180,200,220,240,260,280,300,320,340,360,380,400,420,440,460,480,500,520,540,560,580,600,620,640,660,680,700,720,740,760,780,800,820,840,860,880,900,920,940,960,980,1000};
const static double bins_20_110_1160[] = {110 , 160 , 210 , 260 , 310 , 360 , 410 , 460 , 510 , 560 , 610 , 660 , 710 , 760 , 810 , 860 , 910 , 960 , 1010 , 1060 , 1110 };
const static double bins_50_0_1500[] = {0,30,60,90,120,150,180,210,240,270,300,330,360,390,420,450,480,510,540,570,600,630,660,690,720,750,780,810,840,870,900,930,960,990,1020,1050,1080,1110,1140,1170,1200,1230,1260,1290,1320,1350,1380,1410,1440,1470,1500};
const static double bins_50_0_2p5[]  = {0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1,1.05,1.1,1.15,1.2,1.25,1.3,1.35,1.4,1.45,1.5,1.55,1.6,1.65,1.7,1.75,1.8,1.85,1.9,1.95,2,2.05,2.1,2.15,2.2,2.25,2.3,2.35,2.4,2.45,2.5};
const static double bins_50_0_5[]    = {0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,3,3.1,3.2,3.3,3.4,3.5,3.6,3.7,3.8,3.9,4,4.1,4.2,4.3,4.4,4.5,4.6,4.7,4.8,4.9,5};
const static double bins_200_0_10[]  = {0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1,1.05,1.1,1.15,1.2,1.25,1.3,1.35,1.4,1.45,1.5,1.55,1.6,1.65,1.7,1.75,1.8,1.85,1.9,1.95,2,2.05,2.1,2.15,2.2,2.25,2.3,2.35,2.4,2.45,2.5,2.55,2.6,2.65,2.7,2.75,2.8,2.85,2.9,2.95,3,3.05,3.1,3.15,3.2,3.25,3.3,3.35,3.4,3.45,3.5,3.55,3.6,3.65,3.7,3.75,3.8,3.85,3.9,3.95,4,4.05,4.1,4.15,4.2,4.25,4.3,4.35,4.4,4.45,4.5,4.55,4.6,4.65,4.7,4.75,4.8,4.85,4.9,4.95,5,5.05,5.1,5.15,5.2,5.25,5.3,5.35,5.4,5.45,5.5,5.55,5.6,5.65,5.7,5.75,5.8,5.85,5.9,5.95,6,6.05,6.10001,6.15001,6.20001,6.25001,6.30001,6.35001,6.40001,6.45001,6.50001,6.55001,6.60001,6.65001,6.70001,6.75001,6.80001,6.85001,6.90001,6.95001,7.00001,7.05001,7.10001,7.15001,7.20001,7.25001,7.30001,7.35001,7.40001,7.45001,7.50001,7.55001,7.60001,7.65001,7.70001,7.75001,7.80001,7.85001,7.90001,7.95001,8.00001,8.05001,8.10001,8.15001,8.20001,8.25001,8.30001,8.35001,8.40001,8.45001,8.50001,8.55001,8.60001,8.65001,8.70002,8.75002,8.80002,8.85002,8.90002,8.95002,9.00002,9.05002,9.10002,9.15002,9.20002,9.25002,9.30002,9.35002,9.40002,9.45002,9.50002,9.55002,9.60002,9.65002,9.70002,9.75002,9.80002,9.85002,9.90002,9.95002};
const static double bins_20_0_10[]   = {0,0.5,1,1.5,2,2.5,3,3.5,4,4.5,5,5.5,6,6.5,7,7.5,8,8.5,9,9.5,10};
const static double bins_50_n100_100[] = {-100, -96, -92, -88, -84, -80, -76, -72, -68, -64, -60, -56, -52, -48, -44, -40, -36, -32, -28, -24, -20, -16, -12, -8, -4, 0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96,100};
const static double bins_50_n200_200[] = {-200, -192, -184, -176, -168, -160, -152, -144, -136, -128, -120, -112, -104, -96, -88, -80, -72, -64, -56, -48, -40, -32, -24, -16, -8, 0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128, 136, 144, 152, 160, 168, 176, 184, 192, 200};
const static int n_50 = 50;

const static double bins_11_0_10[] = {-0.5,0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5,10.5};
const static double bins_9_2_10[] = {1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5,10.5};
const static double bins_7_2_8[] = {1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5};
const static double single_bin[] = {0};

const static double bins_test_ht[] = { 500, 515, 537, 571, 621, 697, 2000};
const static double bins_test_ptstar[] = { 110, 125, 147, 181, 231, 307, 1800};
const static int n_test_ht = 6;
const static int n_test_ptstar = 6;

void setStyle();



///Class to print the Status
const static char barspin[4] = {'-','\\','|','/'};
template<typename T>
class Status : public Processor<T> {
public:
    Status(std::string name):Processor<T>(name) {}
    virtual bool Process(T*t,Long64_t i,Long64_t n,double w) {
        if (i==0) std::cout <<"   > "<< n << " Events: \n" << std::flush;
        if (n&&(n/times)&&(i%(n/times))==0) std::cout << "   "<<barspin[(i/(n/times))%4]<<" "<<i/(n/times) << "% \r"<< std::flush;
        return true;
    }
private:
    const static int times = 100;
};

///data helper class for the Plotter
class Histograms {
public:
    Histograms(const std::string& d,const std::string& l):dir_(d),label_(l) {}
    void Add(Histograms* r);
    virtual void Book();
    void Fill(const std::string&s,double c, double w=1.);
    TH1* Get(const std::string&s) {
        return h_[s];
    }
    std::map<std::string, TH1*> * GetAll() {
        return &h_;
    }
    void Write();

private:
    std::string dir_,label_;
    std::map<std::string, TH1*> h_;
};

template<typename T>
class Plotter : public Processor<T> {
public:
    Plotter(std::string dir,std::string n):Processor<T>(n),dir_(dir),weight_(1.0) {
        h_=new Histograms(dir,n);
    }
    //virtual void Init(){};
    virtual bool Process(T*t,Long64_t i,Long64_t n,double);
    virtual void Terminate() {};

    virtual void Book() {
        h_->Book();
    }
    virtual void Write() {
        h_->Write();
    }
    TH1* GetPlot(const std::string& s) {
        return h_->Get(s);
    }


protected:
    std::string dir_;
    double weight_;
    Histograms * h_;
};



template<typename T>
bool Plotter<T>::Process(T*t,Long64_t i,Long64_t n,double w)
{
    w *= weight_;
    w *= t->weight;
    bool res = Processor<T>::Process(t,i,n,w);

    if (h_) {
        h_->Fill( "met", t->met, w );
        h_->Fill( "idxphoton", t->ThePhoton, w );
        h_->Fill( "met_const", t->met, w );
        //h_->Fill( "mht", t->mht, w );
        h_->Fill( "ht", t->ht, w );
        h_->Fill( "ht_const", t->ht, w );
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
        h_->Fill("em1_bitFlag", t->photons_bitFlag[t->ThePhoton], w);
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
struct pair_square
{
    double operator()(double Left, const std::pair<int,double>& Right) const {
        return (Left + Right.second*Right.second);
    }
};

///Yield class can calculate statistical uncertaities for a set of weighted Events (i.e. one bin of one hist)
class Yield {
public:
    Yield():left_(0) {}
    Yield(Yield*left):left_(left) {}
    //Set
    Yield(unsigned n, double weight):left_(0) {
        Add(n,weight,0,0);
    }
    Yield(unsigned n, double weight, double we, int bin):left_(0) {
        Add(n,weight,we,bin);
    }
    Yield(unsigned n, double weight, double we, int bin, Yield*left):left_(left) {
        Add(n,weight,we,bin);
    }
    void Add(unsigned n, double weight, double weighterror, int bin) {
        for (unsigned i=0; i<n; ++i) {
            w.push_back(weight);
            //we[bin]+=weighterror; //add syst. errors of same "bin" linearly
            we[ 0 ]+=weighterror; //add syst. errors of *ALL* "bin" linearly (for x-check with Knut)
        }
    }
    void Add(unsigned n, double weight, double weighterror, int bin, const std::string& str, double w) {
        Add(n, weight, weighterror, bin);
	samples[str]=w;
    }
    void AddWeight(std::vector<double> * r) {
        w.insert(w.end(),r->begin(),r->end());
    }
    void AddWeightError(std::map<int,double> * r) {
        for (std::map<int,double>::iterator it=r->begin(); it!=r->end(); ++it)
            we[it->first]+=it->second;
    }
    //Get
    unsigned unweighted() {
        return w.size();
    }
    double weighted() {
        return      std::accumulate(w.begin(), w.end(), 0.);
    }
    double weighterror() {
        return sqrt(std::accumulate(we.begin(),we.end(),0.,pair_square()));
    }
    double error() {
        return sqrt(std::accumulate(w.begin(), w.end(), 0.,square<double>()) 
	            + PoissonError2()
		   );
    }
    std::vector<double>  * GetWeights() {
        return &w;
    }
    std::map<int,double> * GetWeightErrors() {
        return &we;
    }
    std::map<std::string,double> * GetSamples(){return &samples;} 
    void AddSamples(std::map<std::string, double> * rh){
      for (std::map<std::string, double>::iterator it=rh->begin(); it!=rh->end(); ++it)
        if (samples.find(it->first)==samples.end())
	  samples[it->first] = it->second;
    } 
    
    double PoissonError2()
    {
      if (!left_) return 0;
      double res=0;
      std::map<std::string, double> * leftsamples = left_->GetSamples();
      for (std::map<std::string, double>::iterator ls=leftsamples->begin(); ls!=leftsamples->end(); ++ls){
        //std::cout << "searching "<<ls->first<<"  w="<<ls->second<<std::endl;
	if (samples.find(ls->first)==samples.end()) {
	  //found a bin where sample *ls is not contained, but is contained in its direct left neigbor
	  res += 1.8410 * 1.8410 * ls->second * ls->second; //add Poisson error squared
	  //std::cout << "Poisson error sample="<<ls->first<<" err2="<<res<<std::endl;
	}
      }
      return res;
    }

private:
    std::vector<double> w;
    std::map<int,double> we;
    
    //for Poisson err on zero:
    std::map<std::string, double> samples; //samples in *this* bin and their average weight
    Yield* left_;
};

class Binnings {
public:
    //Binnings(const double *b, int n){binning_ = new std::vector<double>(b,b+n);}
    Binnings(const double *b, int n,float(*eval)(float/*met*/,float/*metPhi*/,float/*ht*/,float/*Sig*/,float/*g_pt*/, float/*g_eta*/, float/*g_phi*/, int/*njets*/, float* /*jets_pt*/, float* /*jets_eta*/, float* /*jet_phi*/))
        :eval_(eval) {
        binning_ = new std::vector<double>(b,b+n);
    }
    std::vector<double>* GetArray() {
        return binning_;
    }
    virtual int GetNBins() {
        return binning_->size();
    }
    virtual double GetBinBorder(int v) {
        return binning_->at(v);
    }
    virtual int GetBin(double v) {
        return std::upper_bound(binning_->begin(),binning_->end(),v)-binning_->begin();
    }
    virtual int GetBin(float met,float metPhi,float ht,float Sig,
                       float g_pt, float g_eta, float g_phi,
                       int njets, float *jets_pt, float *jets_eta, float *jets_phi) {
        return GetBin( eval_(met,metPhi,ht,Sig,g_pt,g_eta,g_phi,njets,jets_pt,jets_eta,jets_phi) );
    }
protected:
    std::vector<double>* binning_;
    float(*eval_)(float/*met*/,float/*metPhi*/,float/*ht*/,float/*Sig*/,float/*g_pt*/, float/*g_eta*/, float/*g_phi*/, int/*njets*/, float* /*jets_pt*/, float* /*jets_eta*/, float* /*jet_phi*/);

};

///Yields class is the data structure for the weighter class; it contains different bins
///of event yields e.g. used for the QCD weighting
///this class also defines the binning of the QCD weigting stuff (GetBin)
class Yields {
public:
    //Yields(){}
    std::string GetLabel() {
        return label_;
    }
    Yields(const std::string& s):label_(s) {
        /// ------------------------------------------------------------
        ///
        /// QCD Reweighting binning definition
        ///
        /// ------------------------------------------------------------

        AddBinning("#gamma p_{T}* (GeV)",      fak1p5_bins, n_fak1p5_bins+1, b_PtPhoton);
        AddBinning("Hadr. Recoil p_{T} (GeV)", fak1p5_bins, n_fak1p5_bins+1, b_PtRecoil);
        //AddBinning("HT (GeV)",           fak1p5_bins, n_fak1p5_bins+1, b_HT);


        //AddBinning("#gamma p_{T}* (GeV)",      bins_50_0_1500, n_50+1, b_PtPhoton);
        //AddBinning("Hadr. Recoil p_{T} (GeV)", bins_50_0_1500, n_50+1, b_PtRecoil);

        //AddBinning("#gamma p_{T}* (GeV)",      nu_bins, n_nu_bins+1, b_PtPhoton);
        //AddBinning("Hadr. Recoil p_{T} (GeV)", nu_bins, n_nu_bins+1, b_PtRecoil);


        /// ------------------------------------------------------------
        /// ------------------------------------------------------------
    }
    void AddBinning(const std::string& n, const double*bins, int nbins,
                    float(*eval)(float/*met*/,float/*metPhi*/,float/*ht*/,float/*Sig*/,float/*g_pt*/, float/*g_eta*/, float/*g_phi*/, int/*njets*/, float* /*jets_pt*/, float* /*jets_eta*/, float* /*jet_phi*/) ) {
        if (binning_.find(n)==binning_.end())
            binning_[n] = new Binnings(bins,nbins,eval);
        else throw "Yields::AddBinnings("+n+") added more than Once!";
    };
    void Add(Yields*r);
    std::map<int,Yield> * GetYields() {
        return &yield;
    }
    Yield * GetYield(int bin) {
        return &yield[bin];
    }
    virtual int GetBin(float met,float metPhi,float ht,float Sig,
                       float g_pt, float g_eta, float g_phi,
                       int njets, float *jets_pt, float *jets_eta, float *jets_phi)
    {
        int bin=0;
        int factor=1;
        for (std::map<std::string,Binnings*>::iterator it=binning_.begin(); it!=binning_.end(); ++it) {
            bin += factor * it->second->GetBin( met,metPhi,ht,Sig,g_pt,g_eta,g_phi,
                                                njets,jets_pt,jets_eta,jets_phi );
            factor *= it->second->GetNBins();
        }
        return bin;
        //return 0;
    }
    virtual int GetNBins() {
        int n=1;
        for (std::map<std::string,Binnings*>::iterator it=binning_.begin(); it!=binning_.end(); ++it)
            n *= it->second->GetNBins();
        return n;
    }
    int WeightsDimension() {
        return binning_.size();
    }
    std::map<std::string,Binnings*> * GetBinning() {
        return &binning_;
    }

    double Weighted(int b) {
        return yield[b].weighted();
    }
    double WeightError(int b) {
        return yield[b].weighterror();
    }
    unsigned Unweighted(int b) {
        return yield[b].unweighted();
    }
    double Error(int b) {
        return yield[b].error();
    }
    double Integral() {
        double res=0;
        for(std::map<int,Yield>::iterator it=yield.begin(); it!=yield.end(); ++it)res+=it->second.weighted();
        return res;
    }

protected:
    std::string label_;
    std::map<int,Yield> yield;
    std::map<std::string,Binnings*> binning_;
};

///class to collect event yields used for the QCD weighting
template<typename T>
class Weighter : public Processor<T> {
public:
    Weighter(std::string n):Processor<T>(n) {
        yields_=new Yields("QCD weighting: "+n);
    }
    //virtual void Init(){};
    virtual bool Process(T*t,Long64_t i,Long64_t n,double);
    virtual void Terminate();

    Yields* GetYields() {
        return yields_;
    }

private:
    Yields* yields_;
};

template<typename T>
bool Weighter<T>::Process(T*t,Long64_t i,Long64_t n,double w)
{
    w *= t->weight;

    float mht = Mht(t->ThePhotonPt,t->ThePhotonEta,t->ThePhotonPhi, t->jets_pt, t->jets_eta, t->jets_phi, t->jets_ );
    ROOT::Math::PtEtaPhiEVector recoil = Recoil(t->ThePhotonPt, t->ThePhotonEta, t->ThePhotonPhi, t->jets_pt, t->jets_eta, t->jets_phi, t->jets_ );
    float r_ht =  Recoil_ht(t->ThePhotonPt, t->ThePhotonEta, t->ThePhotonPhi, t->jets_pt, t->jets_eta, t->jets_phi, t->jets_ );
    float r_pt = Recoil_pt(  &recoil );
    float r_phi = Recoil_phi( &recoil );
    float phi_r_g = DeltaPhi( Recoil_phi( &recoil ), t->ThePhotonPhi);

    if (t->met<100.)
        yields_->GetYield(
            yields_->GetBin(
                t->met,t->metPhi,t->ht,t->metSig,
                t->ThePhotonPt, t->ThePhotonEta, t->ThePhotonPhi,
                t->jets_, t->jets_pt, t->jets_eta, t->jets_phi
            )
        )->Add( 1, w, 0, 0);

    return true;
}

template<typename T>
void Weighter<T>::Terminate()
{
    std::cout << "  Summary Weighter '"<<Processor<T>::name_<<"': "<<yields_->Weighted(0)<<" +- "<<yields_->Error(0)<<"  ("<<yields_->Unweighted(0)<<")"<<std::endl;
};


///Closure test class ==============================================================================
///event yields, binning etc for one single histogram
class YieldDataClass : public Yields
{
public:
    YieldDataClass(const std::string& s,const std::string & xaxis,const std::string yaxis):Yields(s),title_(s),xaxis_(xaxis),yaxis_(yaxis),corr_(false) { }
    virtual ~YieldDataClass() {
        delete binning_;
    }
    void SetBinning(const double *b,int n) {
        binning_ = new std::vector<double>(b,b+n);
	
	Yield * last = 0;
	for (int i=0;i<=n;++i){
	  yield[i] = Yield( last );
	  last = &yield[i];
	}  
	
    }
    virtual int GetNBins() {
        return binning_->size();
    }
    virtual double GetBinBorder(int v) {
        return binning_->at(v);
    }
    virtual int GetBin(double v) {
        //std::cout << "GetBin:: met = "<<v<<" in bin "<<  std::upper_bound(binning_->begin(),binning_->end(),v)-binning_->begin()-1<<std::endl;
        return std::upper_bound(binning_->begin(),binning_->end(),v)-binning_->begin();
    }
    void SetCorrelation(bool corr) {
        corr_=corr;
    }
    bool GetCorrelation() {
        return corr_;
    }
    std::string GetXaxisTitle() {
        return xaxis_;
    }
    std::string GetYaxisTitle() {
        return yaxis_;
    }

protected:
    std::string title_,xaxis_,yaxis_;
    std::vector<double>* binning_;
    bool corr_;
};

///collects the event yields of various histograms
class MyYields
{
public:
    MyYields(const std::string& s):label_(s) {}

    void Add(const std::string& s, YieldDataClass* d) {
        y_[s]=d;
    }
    void Add(const std::string& s, int bin, int n, double w, double we, int wbin, const std::string&str, double wght) {
        y_[s]->GetYield(bin)->Add(n,w,we,wbin,str,wght);
    }
    void AddRef(std::map<std::string, YieldDataClass*>* ref) {
        for (std::map<std::string, YieldDataClass*>::iterator it=ref->begin(); it!=ref->end(); ++it) {
            y_[it->first]->Add( it->second );
        }
    }
    void SetBinning(const std::string& s,const double *b,int n) {
        y_[s]->SetBinning(b,n);
    }
    YieldDataClass* GetYieldsRef(const std::string& s) {
        return y_[s];
    }
    std::map<std::string, YieldDataClass*> * GetRef() {
        return &y_;
    }
    TH1 *  GetWeightErrorPlot(const std::string& s, TH1*h=0);
    TH1 *  GetPlot(       const std::string& s);
    int    GetNBins(      const std::string& s)          {
        return y_[s]->GetNBins();
    }
    int    GetBin(        const std::string& s, double v) {
        return y_[s]->GetBin(v);
    }
    double GetBinBorder(  const std::string& s, int v)   {
        return y_[s]->GetBinBorder(v);
    }
    double Weighted(      const std::string& s, int b)   {
        return y_[s]->Weighted(b);
    }
    double WeightError(   const std::string& s, int b)   {
        return y_[s]->WeightError(b);
    }
    int    Unweighted(    const std::string& s, int b)   {
        return y_[s]->Unweighted(b);
    }
    double Integral(      const std::string& s)          {
        return y_[s]->Integral();
    }
    double Error(         const std::string& s, int b)   {
        return y_[s]->Error(b);
    }
    void   SetCorrelation(const std::string& s, bool corr) {
        y_[s]->SetCorrelation(corr);
    }
    bool   GetCorrelation(const std::string& s)          {
        return y_[s]->GetCorrelation();
    }
    std::string Name() {
        return label_;
    }
    std::string ResultName() {
        return resultlabel_;
    }
    void SetResultName(const std::string&s) {
        resultlabel_=s;
    }
    int FillColor(int c=-1) {
        return fillcolor_=(c>=0?c:fillcolor_);
    }
    int LineColor(int c=-1) {
        return linecolor_=(c>=0?c:linecolor_);
    }

protected:
    std::string label_, resultlabel_;
    std::map<std::string, YieldDataClass*> y_;
    int fillcolor_, linecolor_;
};

///Class for the closure
template<typename T>
class Closure : public Processor<T> {
public:
    Closure(std::string d,std::string n,std::string titel=""):Processor<T>(n),dir_(d),titel_(titel),denominator_(0),nominator_(0),weighterror_(0) {    }
    virtual void Init();
    virtual bool Process(T*t,Long64_t i,Long64_t n,double);
    virtual void Write();
    virtual void Book();
    virtual void Terminate() {
        std::cout << "  Summary Closure '"<<Processor<T>::name_<<std::endl;
    };

    void AddRef(std::map<std::string,MyYields*> *y) {
        for (std::map<std::string,MyYields*>::iterator it=y->begin(); it!=y->end(); ++it)
            yields_[it->first]->AddRef( it->second->GetRef() );
    }

    void SetDenominator(Yields*y) {
        denominator_=y;
    }
    void SetNominator(Yields*y) {
        nominator_=y;
    }
    void AddDirectYields(std::map<std::string,MyYields*> *y) {
        if (!direct_.size()) direct_= *y;
        else if (y) for (std::map<std::string,MyYields*>::iterator it=y->begin(); it!=y->end(); ++it)
                direct_[it->first]->AddRef( it->second->GetRef() );
    }
    void AddSignalYields(std::map<std::string,MyYields*> *y) {
        for (std::map<std::string,MyYields*>::iterator it=y->begin(); it!=y->end(); ++it) {
            signal_[it->first].push_back( it->second );
        }
    }
    void AddOtherYields(std::map<std::string,MyYields*> *y) {
        for (std::map<std::string,MyYields*>::iterator it=y->begin(); it!=y->end(); ++it)
            other_[it->first].push_back( it->second );
    }
    std::map<std::string,MyYields*> * GetYields() {
        return &yields_;
    }
    void FillColor(int c) {
        for(std::map<std::string,MyYields*>::iterator it=yields_.begin(); it!=yields_.end(); ++it)it->second->FillColor(c);
    }
    void LineColor(int c) {
        for(std::map<std::string,MyYields*>::iterator it=yields_.begin(); it!=yields_.end(); ++it)it->second->LineColor(c);
    }
    void ResultName(const std::string&c) {
        for(std::map<std::string,MyYields*>::iterator it=yields_.begin(); it!=yields_.end(); ++it)it->second->SetResultName(c);
    }
    void SetLegTitel(std::string titel) {
        legtitel_=titel;
    };

protected:
    double weighterror_;

private:
    std::string dir_, titel_, legtitel_;
    Yields * denominator_, //Nenner: MET<100, loose isolated
           * nominator_;   //Zähler: MET<100, tight isolated
    std::vector<double> weights_,weighterrors_; //lookuptable for weights to speed up process

    std::map<std::string,MyYields*> yields_, direct_;
    std::map<std::string,std::vector<MyYields*> > signal_, other_;
    double weight_;

    void BookHistogram(const std::string& s, const std::string& x, const std::string& y, const std::string title, const double * bins, int nbins) {
        yields_[s] = new MyYields(title);
        yields_[s]->Add(s, new YieldDataClass(s,x,y));
        yields_[s]->SetBinning(s, bins, nbins);
    }
    void BookCorrHistogram(const std::string& s, const std::string& x, const std::string& y, const std::string title, const double * bins, int nbins) {
        yields_[s] = new MyYields(title);
        yields_[s]->Add(s, new YieldDataClass(s,x,y));
        yields_[s]->SetBinning(s, bins, nbins);
        yields_[s]->SetCorrelation(s,true);
    }
    void Fill(const std::string& s, double var, double w, double we, int wbin, const std::string& str, double wght) {
        yields_[s]->Add(s, yields_[s]->GetBin(s,var), 1, w, we, wbin, str, wght );
    }
};


template<typename T>
void Closure<T>::Book()
{
    //Plotter<T>::Book();

    BookHistogram("n_jet", "n_{jet}","Events", titel_,bins_7_2_8, 8);
    BookHistogram("njet_met0_30", "n_{jet}","Events", titel_,bins_7_2_8, 8);
    BookHistogram("njet_met30_60", "n_{jet}","Events", titel_,bins_7_2_8, 8);
    BookHistogram("njet_met60_100", "n_{jet}","Events", titel_,bins_7_2_8, 8);
    BookHistogram("n_loose",  "loose photon multiplicity","Events", titel_,bins_11_0_10, 12);
    BookHistogram("n_tight",  "tight photon multiplicity","Events", titel_,bins_11_0_10, 12);
    BookHistogram("n_mu",  "muon multiplicity","Events", titel_,bins_11_0_10, 12);
    BookHistogram("n_e",  "electron multiplicity","Events", titel_,bins_11_0_10, 12);
    BookHistogram("n_l",  "lepton multiplicity","Events", titel_,bins_11_0_10, 12);
    BookHistogram("met", "#slash{E}_{T} (GeV)","Events", titel_,metbins, n_metbins+1);
    BookHistogram("met_arrow", "#slash{E}_{T} (GeV)","Events", titel_,metbins, n_metbins+1);
    BookHistogram("met_new", "#slash{E}_{T} (GeV)","Events", titel_,newmetbins, n_newmetbins+1);
    BookHistogram("met_fibo", "#slash{E}_{T} (GeV)","Events", titel_,fibonacci, n_fibonacci+1);
    BookHistogram("met_optim", "#slash{E}_{T} (GeV)","Events", titel_,met_optim, n_met_optim+1);
    BookHistogram("met_systerr", "syst. unc. vs #slash{E}_{T} (GeV)","Events", titel_,metbins, n_metbins+1);
    BookHistogram("met_trans", "transversal #slash{E}_{T} (GeV)","Events", titel_,metbins, n_metbins+1);
    BookHistogram("met_paral", "parallel #slash{E}_{T} (GeV)","Events", titel_,metbins, n_metbins+1);
    BookHistogram("ht",  "H_{T} (GeV)","Events", titel_, htbins,  n_htbins+1);
    BookHistogram("ht_const",  "H_{T} (GeV)","Events", titel_, htbins2,  n_htbins2+1);
    BookHistogram("met_const",  "#slash{E}_{T} (GeV)","Events", titel_, bins_50_0_500, n_50+1);
    BookHistogram("met_corr",  "corrected #slash{E}_{T} (GeV)","Events", titel_, bins_50_0_1000, n_50+1);
    BookHistogram("met_phi",  "#phi_{#slash{E}_{T}}","Events", titel_, bins_64_nPi_Pi, n_64+1);
    BookHistogram("met_signif",  "#slash{E}_{T} Significance","Events", titel_, bins_50_0_100, n_50+1);
    BookHistogram("mht",  "#slash{H}_{T} (GeV)","Events", titel_, bins_50_0_1000, n_50+1);
    BookHistogram("mht_trans",  "Transverse #slash{H}_{T} (GeV)","Events", titel_, bins_50_0_1000, n_50+1);
    BookHistogram("mht_paral",  "Parallel #slash{H}_{T} (GeV)","Events", titel_, bins_50_0_1000, n_50+1);
    BookHistogram("mht_phi",  "#phi_{#slash{H}_{T}}","Events", titel_, bins_64_nPi_Pi, n_64+1);
    BookHistogram("jet1_pt",  "Leading Jet p_{T} (GeV)","Events", titel_, bins_50_0_1000, n_50+1);
    BookHistogram("jet2_pt",  "Next-to-leading Jet p_{T} (GeV)","Events", titel_, bins_50_0_1000, n_50+1);
    BookHistogram("em1_pt",  "p_{T} (GeV)","Events", titel_, bins_50_0_1000, n_50+1);
    BookHistogram("em1_thePt",  "p_{T^{*}} (GeV)","Events", titel_, bins_20_110_1160, 21);
    BookHistogram("em1_ptstar",  "p_{T,jet matched to #gamma} (GeV)","Events", titel_, bins_50_0_1000, n_50+1);
    BookHistogram("em1_phi",  "#Phi Photon","Events", titel_, bins_64_nPi_Pi, n_64+1);
    BookHistogram("weight",  "weight","Events", titel_, weightbins, n_weightbins+1 );
    BookHistogram("phi_met_em1",  "#phi(#slash{E}_{T}, #gamma)","Events", titel_, bins_64_nPi_Pi, n_64+1);
    BookHistogram("phi_mht_em1",  "#phi(#slash{H}_{T}, #gamma)","Events", titel_, bins_64_nPi_Pi, n_64+1);
    BookHistogram("phi_mht_recoil",  "#phi(#slash{H}_{T}, Recoil)","Events", titel_, bins_64_nPi_Pi, n_64+1);
    BookHistogram("phi_met_recoil",  "#phi(#slash{E}_{T}, Recoil)","Events", titel_, bins_64_nPi_Pi, n_64+1);
    BookHistogram("phi_recoil_em1",  "#phi(Recoil, Photon)","Events", titel_, bins_64_nPi_Pi, n_64+1);
    BookHistogram("recoil_ht",  "Recoil H_{T} (GeV)","Events", titel_,bins_50_0_1500, n_50+1);
    BookHistogram("recoil_pt",  "Recoil p_{T} (GeV)","Events", titel_,bins_50_0_1500, n_50+1);
    BookHistogram("recoil_phi",  "#phi Recoil","Events", titel_,bins_64_nPi_Pi, n_64+1);
    BookHistogram("kinematicClosureMet",  "#vec{p}_{T} recoil + #vec{p}_{T} #gamma + #slash{E}_{T}","Events",     titel_, bins_50_0_500, n_50+1);
    BookHistogram("kinematicClosureMht",  "#vec{p}_{T} recoil + #vec{p}_{T} #gamma + #slash{H}_{T}","Events",     titel_, bins_50_0_500, n_50+1);
    BookHistogram("kinematicClosureMet_Met",  "#vec{p}_{T} recoil + #vec{p}_{T} #gamma + #slash{E}_{T}","Events", titel_, bins_50_n200_200, n_50+1);
    BookHistogram("kinematicClosureMet_G",  "#vec{p}_{T} recoil + #vec{p}_{T} #gamma + #slash{E}_{T}","Events",   titel_, bins_50_n200_200, n_50+1);
    BookHistogram("kinematicClosureMet_R",  "#vec{p}_{T} recoil + #vec{p}_{T} #gamma + #slash{E}_{T}","Events",   titel_, bins_50_n200_200, n_50+1);
    BookHistogram("kinematicClosureMht_Mht",  "#vec{p}_{T} recoil + #vec{p}_{T} #gamma + #slash{H}_{T}","Events", titel_, bins_50_n200_200, n_50+1);
    BookHistogram("kinematicClosureMht_G",  "#vec{p}_{T} recoil + #vec{p}_{T} #gamma + #slash{H}_{T}","Events",   titel_, bins_50_n200_200, n_50+1);
    BookHistogram("kinematicClosureMht_R",  "#vec{p}_{T} recoil + #vec{p}_{T} #gamma + #slash{H}_{T}","Events",   titel_, bins_50_n200_200, n_50+1);

    BookHistogram("PtEm1_Over_Ptrecoil",  "p_{T} Photon / p_{T} Recoil","Events", titel_,bins_50_0_5, n_50+1);
    BookHistogram("PtEm1_Over_MHT",  "p_{T} Photon / #slash{H}_{T}","Events", titel_,bins_50_0_5, n_50+1);
    BookHistogram("PtEm1_Over_MET",  "p_{T} Photon / #slash{E}_{T}","Events", titel_,bins_50_0_5, n_50+1);
    BookHistogram("Ptrecoil_Over_MHT",  "p_{T} Recoil / #slash{H}_{T}","Events", titel_,bins_50_0_5, n_50+1);
    BookHistogram("Ptrecoil_Over_PhiMhtEm1",  "p_{T} Recoil / #phi(#slash{H}_{T},photon)","Events", titel_,bins_50_0_500, n_50+1);
    BookHistogram("Ptrecoil_Over_PhiEm1Recoil",  "p_{T} Recoil / #phi(photon,recoil)","Events", titel_,bins_50_0_500, n_50+1);
    BookHistogram("Ptrecoil_Over_PhiMhtRecoil",  "p_{T} Recoil / #phi(#slash{H}_{T},recoil)","Events", titel_,bins_50_0_500, n_50+1);
    BookHistogram("PtEm1_Over_PhiMhtEm1",     "PtEm1_Over_PhiMhtEm1","Events", titel_,bins_50_0_100, n_50+1);
    BookHistogram("PtEm1_Over_PhiEm1Recoil",  "PtEm1_Over_PhiEm1Recoil","Events", titel_,bins_50_0_100, n_50+1);
    BookHistogram("PtEm1_Over_PhiMhtRecoil",  "PtEm1_Over_PhiMhtRecoil","Events", titel_,bins_50_0_100, n_50+1);
    BookHistogram("Mht_Over_PhiMhtEm1",       "Mht_Over_PhiMhtEm1","Events", titel_,bins_50_0_100, n_50+1);
    BookHistogram("Mht_Over_PhiEm1Recoil",    "Mht_Over_PhiEm1Recoil","Events", titel_,bins_50_0_100, n_50+1);
    BookHistogram("Mht_Over_PhiMhtRecoil",    "Mht_Over_PhiMhtRecoil","Events", titel_,bins_50_0_100, n_50+1);
    BookHistogram("PhiMhtEm1_Over_PhiMhtRecoil",  "PhiMhtEm1_Over_PhiMhtRecoil","Events", titel_,bins_50_0_5, n_50+1);
    BookHistogram("PhiMhtEm1_Over_PhiEm1Recoil",  "PhiMhtEm1_Over_PhiEm1Recoil","Events", titel_,bins_50_0_5, n_50+1);

    BookHistogram("PtEm1_Over_PtEm1Gen", "p_{T} Photon / p_{T} Generator-Photon","Events", titel_,bins_50_0_5, n_50+1);
    BookHistogram("DR_PtEm1_PtEm1Gen",   "#Delta R(#gamma, #gamma_{GEN})","Events", titel_,bins_50_0_5, n_50+1);

    BookCorrHistogram("corr_PtEm1_Over_Ptrecoil_vs_MHT", "#slash{H}_{T} (GeV)", "EM1 Pt / Recoil Pt", titel_,bins_50_0_1000, n_50+1);
    BookCorrHistogram("corr_PtEm1_Over_MHT_vs_MHT",      "#slash{H}_{T} (GeV)", "EM1 Pt / #slash{H}_{T}", titel_,bins_50_0_1000, n_50+1);
    BookCorrHistogram("corr_Ptrecoil_Over_MHT_vs_MHT",   "#slash{H}_{T} (GeV)", "Recoil Pt / #slash{H}_{T}", titel_,bins_50_0_1000, n_50+1);
    BookCorrHistogram("corr_PtEm1_Over_Ptrecoil_vs_MET", "#slash{E}_{T} (GeV)", "EM1 Pt / Recoil Pt", titel_,bins_50_0_1000, n_50+1);
    BookCorrHistogram("corr_PtEm1_Over_MHT_vs_MET",      "#slash{E}_{T} (GeV)", "EM1 Pt / #slash{H}_{T}", titel_,bins_50_0_1000, n_50+1);
    BookCorrHistogram("corr_Ptrecoil_Over_MHT_vs_MET",   "#slash{E}_{T} (GeV)", "Recoil Pt / #slash{H}_{T}", titel_,bins_50_0_1000, n_50+1);
    BookCorrHistogram("corr_PtEm1_Over_Ptrecoil_vs_recoil", "Recoil Pt (GeV)", "EM1 Pt / Recoil Pt", titel_,bins_50_0_1500, n_50+1);
    BookCorrHistogram("corr_PtEm1_Over_MHT_vs_recoil",      "Recoil Pt (GeV)", "EM1 Pt / #slash{H}_{T}", titel_,bins_50_0_1500, n_50+1);
    BookCorrHistogram("corr_Ptrecoil_Over_MHT_vs_recoil",   "Recoil Pt (GeV)", "Recoil Pt / #slash{H}_{T}", titel_,bins_50_0_1500, n_50+1);
    BookCorrHistogram("corr_PtEm1_Over_Ptrecoil_vs_em1pt", "EM1 Pt (GeV)", "EM1 Pt / Recoil Pt", titel_,bins_50_0_1000, n_50+1);
    BookCorrHistogram("corr_PtEm1_Over_MHT_vs_em1pt",      "EM1 Pt (GeV)", "EM1 Pt / #slash{H}_{T}", titel_,bins_50_0_1000, n_50+1);
    BookCorrHistogram("corr_Ptrecoil_Over_MHT_vs_em1pt",   "EM1 Pt (GeV)", "Recoil Pt / #slash{H}_{T}", titel_,bins_50_0_1000, n_50+1);

    /*
      BookCorrHistogram("corr_Ptrecoil_Over_PhiMhtEm1", "#slash{H}_{T} (GeV)", titel_,bins_50_0_1000, n_50+1);
      BookCorrHistogram("corr_Ptrecoil_Over_PhiEm1Recoil", "#slash{H}_{T} (GeV)", titel_,bins_50_0_1000, n_50+1);
      BookCorrHistogram("corr_Ptrecoil_Over_PhiMhtRecoil", "#slash{H}_{T} (GeV)", titel_,bins_50_0_1000, n_50+1);
      BookCorrHistogram("corr_PtEm1_Over_PhiMhtEm1", "#slash{H}_{T} (GeV)", titel_,bins_50_0_1000, n_50+1);
      BookCorrHistogram("corr_PtEm1_Over_PhiEm1Recoil", "#slash{H}_{T} (GeV)", titel_,bins_50_0_1000, n_50+1);
      BookCorrHistogram("corr_PtEm1_Over_PhiMhtRecoil", "#slash{H}_{T} (GeV)", titel_,bins_50_0_1000, n_50+1);
      BookCorrHistogram("corr_Mht_Over_PhiMhtEm1", "#slash{H}_{T} (GeV)", titel_,bins_50_0_1000, n_50+1);
      BookCorrHistogram("corr_Mht_Over_PhiEm1Recoil", "#slash{H}_{T} (GeV)", titel_,bins_50_0_1000, n_50+1);
      BookCorrHistogram("corr_Mht_Over_PhiMhtRecoil", "#slash{H}_{T} (GeV)", titel_,bins_50_0_1000, n_50+1);
      BookCorrHistogram("corr_PhiMhtEm1_Over_PhiMhtRecoil", "#slash{H}_{T} (GeV)", titel_,bins_50_0_1000, n_50+1);
      BookCorrHistogram("corr_PhiMhtEm1_Over_PhiEm1Recoil", "#slash{H}_{T} (GeV)", titel_,bins_50_0_1000, n_50+1);
    */

    ///To save time, pre-calculate the weights once:
    if (!denominator_ || !nominator_) return;
    for (int b=0; b<nominator_->GetNBins(); ++b) {
        double d_int = denominator_->Integral( );
        double n_int = nominator_->Integral( );
        double d = denominator_->Weighted( b ); //loose
        double de = denominator_->Error( b );   //loose stat. error
        double n = nominator_->Weighted( b );   //tight
        double ne = nominator_->Error( b );     //tight stat. error
        weights_.push_back( (d*n ==0?n_int/d_int :n / d) );
        weighterrors_.push_back( (d*n ==0?n_int/d_int:
                                  sqrt( ne*ne/(d*d) + de*de*n*n/(d*d*d*d) ) ) );

        //if (d) std::cout << "QCD-rewighting bin "<<b
        //          << ": weight = "<<n<<" +- "<<ne<< " / "<<d<<" +- "<<de<<" = "<<n/d<<" +- "<<sqrt( ne*ne/(d*d) + de*de*n*n/(d*d*d*d) )
        //	      <<std::endl;
    }
}

template<typename T>
void Closure<T>::Init() {}


template<typename T>
bool Closure<T>::Process(T*t,Long64_t i,Long64_t n,double w)
{
    std::stringstream ss;
    ss << "Sample_w_"<<w;
    std::string samplestr = ss.str();

    double we = weighterror_;
    int bin = 0;
    if (denominator_ && nominator_) {
        bin = nominator_->GetBin(
                  t->met,t->metPhi,t->ht,t->metSig,
                  t->ThePhotonPt, t->ThePhotonEta, t->ThePhotonPhi,
                  t->jets_, t->jets_pt, t->jets_eta, t->jets_phi
              );
        weight_ = weights_[bin];
        we = weighterrors_[bin];
    } else
        weight_ = 1.0;


    //int bin = yields_->GetBin( t->met,0,0 );
    //yields_->GetYield( bin )->Add( 1, Plotter<T>::weight_ * w );

    //int mybin = myYields_->GetBin("met",t->met);
    //myYields_->Add("met", myYields_->GetBin("met",t->met), 1, Plotter<T>::weight_ * w  );

    double weight = weight_ * w ;
    weight *= t->weight;
    we *= w * t->weight;

//  std::cout << "Closure<T>::Process(T*t,Long64_t i="<<i<<",Long64_t n="<<n<<",double w="<<w<<")"
//            << "  weight = "<<weight<<", we = "<<we
//    <<std::endl;


    if (t->photons_>t->kMaxphotons) std::cerr<<"t->photons_="<<t->photons_<<" > t->kMaxphotons="<<t->kMaxphotons<<std::endl;
    if (t->jets_>t->kMaxjets) std::cerr<<"t->jets_="<<t->jets_<<" > t->kMaxjets="<<t->kMaxjets<<std::endl;
    if (t->ThePhoton<0||t->ThePhoton>t->photons_) std::cerr<<"t->ThePhoton="<<t->ThePhoton<<" but t->photons_="<<t->photons_<<std::endl;


    //std::cout<<"met="<<t->met<< ", w="<<weight<<", we="<< we<<", b="<<bin<<std::endl;

    Fill("met",       t->met, weight, we, bin, samplestr, w);
    Fill("met_arrow", t->met, weight, we, bin, samplestr, w);
    Fill("met_new",   t->met, weight, we, bin, samplestr, w);
    Fill("met_fibo",  t->met, weight, we, bin, samplestr, w);
    Fill("met_optim", t->met, weight, we, bin, samplestr, w);
    Fill("met_const", t->met, weight, we, bin, samplestr, w);
    Fill("ht",        t->ht,  weight, we, bin, samplestr, w);
    Fill("ht_const",  t->ht,  weight, we, bin, samplestr, w);
    Fill("met_signif",t->metSig, weight, we, bin, samplestr, w);
    Fill("em1_pt",    t->photons_pt[t->ThePhoton], weight, we, bin, samplestr, w);
    Fill("em1_thePt", t->ThePhotonPt, weight, we, bin, samplestr, w);
    Fill("em1_ptstar",t->photons__ptJet[t->ThePhoton], weight, we, bin, samplestr, w);
    Fill("em1_phi",   t->ThePhotonPhi, weight, we, bin, samplestr, w);
    Fill("weight",    weight, 1., we, bin , samplestr, w);
    Fill("phi_met_em1", DeltaPhi(t->metPhi-kPI, t->ThePhotonPhi), weight, we, bin, samplestr, w);
    Fill("met_phi",   t->metPhi-kPI, weight, we, bin , samplestr, w);


    float g_pt  = t->ThePhotonPt;
    float g_eta = t->ThePhotonEta;
    float g_phi = t->ThePhotonPhi;
    float mht = Mht(g_pt,g_eta,g_phi, t->jets_pt, t->jets_eta, t->jets_phi, t->jets_ );
    float mht_phi = MhtPhi(g_pt,g_eta,g_phi, t->jets_pt, t->jets_eta, t->jets_phi, t->jets_ );
    ROOT::Math::PtEtaPhiEVector recoil = Recoil(t->ThePhotonPt, t->ThePhotonEta, t->ThePhotonPhi, t->jets_pt, t->jets_eta, t->jets_phi, t->jets_ );
    float recoil_pt =  Recoil_pt(  &recoil );
    float phi_recoil_em1 = DeltaPhi( Recoil_phi( &recoil ), t->ThePhotonPhi);
    float phi_mht_em1    = DeltaPhi(mht_phi, g_phi);
    float phi_mht_recoil = DeltaPhi(mht_phi, Recoil_phi( &recoil ));
    float phi_met_recoil = DeltaPhi(t->metPhi, Recoil_phi( &recoil ));

    Fill("jet1_pt",   FirstJet(g_pt,g_eta,g_phi, t->jets_pt, t->jets_eta, t->jets_phi, t->jets_ ), weight, we, bin, samplestr, w);
    Fill("jet2_pt",   SecondJet(g_pt,g_eta,g_phi, t->jets_pt, t->jets_eta, t->jets_phi, t->jets_ ), weight, we, bin, samplestr, w);
    Fill("recoil_ht",   Recoil_ht(g_pt, g_eta, g_phi, t->jets_pt, t->jets_eta, t->jets_phi, t->jets_ ), weight, we, bin , samplestr, w);
    Fill("recoil_pt",   recoil_pt, weight, we, bin , samplestr, w);
    Fill("recoil_phi",  Recoil_phi( &recoil ), weight, we, bin , samplestr, w);
    Fill("phi_recoil_em1", phi_recoil_em1, weight, we, bin, samplestr, w);
    Fill("phi_mht_em1",    phi_mht_em1, weight, we, bin, samplestr, w);
    Fill("phi_mht_recoil", phi_mht_recoil, weight, we, bin, samplestr, w);
    Fill("phi_met_recoil", phi_met_recoil, weight, we, bin, samplestr, w);
    //Fill("met_corr",    CorectedMet(t->met,t->metPhi-kPI,t->photons_pt[t->ThePhoton], t->photons_eta[t->ThePhoton], t->photons_phi[t->ThePhoton], g_pt ,g_eta, g_phi ), weight, we, bin, samplestr, w);

    Fill("mht",mht, weight, we, bin , samplestr, w);
    Fill("mht_phi",mht_phi, weight, we, bin , samplestr, w);
    Fill("mht_trans", CalcTransMet(mht,mht_phi,g_phi), weight, we, bin, samplestr, w);
    Fill("mht_paral", CalcParalMet(mht,mht_phi,g_phi), weight, we, bin, samplestr, w);

    Fill("kinematicClosureMet", KinematicClosure(t->met,t->metPhi-kPI,g_pt, g_phi, recoil_pt, Recoil_phi( &recoil )), weight, we, bin, samplestr, w);
    Fill("kinematicClosureMht", KinematicClosure(mht,mht_phi,g_pt, g_phi, recoil_pt, Recoil_phi( &recoil )), weight, we, bin, samplestr, w);

    Fill("kinematicClosureMet_Met",VecKinematicClosure(t->met,t->metPhi-kPI,g_pt, g_phi, recoil_pt, Recoil_phi( &recoil )), weight, we, bin, samplestr, w);
    Fill("kinematicClosureMet_G",  VecKinematicClosure(g_pt, g_phi, t->met,t->metPhi-kPI,recoil_pt, Recoil_phi( &recoil )), weight, we, bin, samplestr, w);
    Fill("kinematicClosureMet_R", VecKinematicClosure(recoil_pt, Recoil_phi( &recoil ), t->met,t->metPhi-kPI,g_pt, g_phi), weight, we, bin, samplestr, w);
    Fill("kinematicClosureMht_Mht",VecKinematicClosure(mht   ,mht_phi,  g_pt, g_phi, recoil_pt, Recoil_phi( &recoil )), weight, we, bin, samplestr, w);
    Fill("kinematicClosureMht_G",  VecKinematicClosure(g_pt, g_phi, mht   ,mht_phi,  recoil_pt, Recoil_phi( &recoil )), weight, we, bin, samplestr, w);
    Fill("kinematicClosureMht_R",  VecKinematicClosure(recoil_pt, Recoil_phi( &recoil ), mht   ,mht_phi,  g_pt, g_phi ), weight, we, bin, samplestr, w);

    //std::cout << "evt nr:" << t->eventNumber << ", rn: " <<t->runNumber<<", lbnr: "<< t->luminosityBlockNumber
    //          << ", recoilPt: " << recoil_pt << ", gPt: "<<g_pt << ", weight: "<< weight << ", we: "<< we << std::endl;

//std::cout<<"4"<<std::endl;

    /*
      if (t->genPhotons_){
      Fill("PtEm1_Over_PtEm1Gen",   (t->genPhotons_pt[0]==0?1.: g_pt/t->genPhotons_pt[0]), weight, we, bin, samplestr, w);
      Fill("DR_PtEm1_PtEm1Gen",   	deltaR(g_eta,g_phi,t->genPhotons_eta[0],t->genPhotons_phi[0]), weight, we, bin, samplestr, w);
      }
    */
    /*
      Fill("PtEm1_Over_Ptrecoil",   	(recoil_pt==0?1.: g_pt/recoil_pt), weight, we, bin, samplestr, w);
      Fill("PtEm1_Over_MHT",		(mht==0?1.: g_pt/mht), weight, we, bin, samplestr, w);
      Fill("PtEm1_Over_MET",		(t->met==0?1.: g_pt/t->met), weight, we, bin, samplestr, w);
      Fill("Ptrecoil_Over_MHT",		(mht==0?1.: recoil_pt/mht), weight, we, bin, samplestr, w);
      Fill("Ptrecoil_Over_PhiMhtEm1",	(phi_mht_em1==0?1.:    recoil_pt/phi_mht_em1), weight, we, bin, samplestr, w);
      Fill("Ptrecoil_Over_PhiEm1Recoil",	(phi_recoil_em1==0?1.: recoil_pt/phi_recoil_em1), weight, we, bin, samplestr, w);
      Fill("Ptrecoil_Over_PhiMhtRecoil",	(phi_mht_recoil==0?1.: recoil_pt/phi_mht_recoil), weight, we, bin, samplestr, w);
      Fill("PtEm1_Over_PhiMhtEm1",		(phi_mht_em1==0?1.:    g_pt/phi_mht_em1), weight, we, bin, samplestr, w);
      Fill("PtEm1_Over_PhiEm1Recoil",	(phi_recoil_em1==0?1.: g_pt/phi_recoil_em1), weight, we, bin, samplestr, w);
      Fill("PtEm1_Over_PhiMhtRecoil",	(phi_mht_recoil==0?1.: g_pt/phi_mht_recoil), weight, we, bin, samplestr, w);
      Fill("Mht_Over_PhiMhtEm1",		(phi_mht_em1==0?1.:    mht/phi_mht_em1), weight, we, bin, samplestr, w);
      Fill("Mht_Over_PhiEm1Recoil",		(phi_recoil_em1==0?1.: mht/phi_recoil_em1), weight, we, bin, samplestr, w);
      Fill("Mht_Over_PhiMhtRecoil",		(phi_mht_recoil==0?1.: mht/phi_mht_recoil), weight, we, bin, samplestr, w);
      Fill("PhiMhtEm1_Over_PhiMhtRecoil",	(phi_mht_recoil==0?1.: phi_mht_em1/phi_mht_recoil), weight, we, bin, samplestr, w);
      Fill("PhiMhtEm1_Over_PhiEm1Recoil",	(phi_recoil_em1==0?1.: phi_mht_em1/phi_recoil_em1), weight, we, bin, samplestr, w);
    */
    /*
      Fill("corr_PtEm1_Over_Ptrecoil_vs_MHT",   	mht, (recoil_pt==0?1.: g_pt/recoil_pt), 0, 0, samplestr, w);
      Fill("corr_PtEm1_Over_MHT_vs_MHT",		mht, (mht==0?1.: g_pt/recoil_pt), 0, 0, samplestr, w);
      Fill("corr_Ptrecoil_Over_MHT_vs_MHT",		mht, (mht==0?1.: recoil_pt/mht), 0, 0, samplestr, w);
      Fill("corr_PtEm1_Over_Ptrecoil_vs_MET",   	t->met, (recoil_pt==0?1.: g_pt/recoil_pt), 0, 0, samplestr, w);
      Fill("corr_PtEm1_Over_MHT_vs_MET",		t->met, (mht==0?1.: g_pt/recoil_pt), 0, 0, samplestr, w);
      Fill("corr_Ptrecoil_Over_MHT_vs_MET",		t->met, (mht==0?1.: recoil_pt/mht), 0, 0, samplestr, w);
      Fill("corr_PtEm1_Over_Ptrecoil_vs_recoil",   	recoil_pt, (recoil_pt==0?1.: g_pt/recoil_pt), 0, 0, samplestr, w);
      Fill("corr_PtEm1_Over_MHT_vs_recoil",		recoil_pt, (mht==0?1.: g_pt/recoil_pt), 0, 0, samplestr, w);
      Fill("corr_Ptrecoil_Over_MHT_vs_recoil",	recoil_pt, (mht==0?1.: recoil_pt/mht), 0, 0, samplestr, w);
      Fill("corr_PtEm1_Over_Ptrecoil_vs_em1pt",   	g_pt, (recoil_pt==0?1.: g_pt/recoil_pt), 0, 0, samplestr, w);
      Fill("corr_PtEm1_Over_MHT_vs_em1pt",		g_pt, (mht==0?1.: g_pt/recoil_pt), 0, 0, samplestr, w);
      Fill("corr_Ptrecoil_Over_MHT_vs_em1pt",	g_pt, (mht==0?1.: recoil_pt/mht), 0, 0, samplestr, w);
    */
    /*
      Fill("corr_Ptrecoil_Over_PhiMhtEm1",		mht, (phi_mht_em1==0?1.:    recoil_pt/phi_mht_em1), samplestr, w);
      Fill("corr_Ptrecoil_Over_PhiEm1Recoil",	mht, (phi_recoil_em1==0?1.: recoil_pt/phi_recoil_em1), samplestr, w);
      Fill("corr_Ptrecoil_Over_PhiMhtRecoil",	mht, (phi_mht_recoil==0?1.: recoil_pt/phi_mht_recoil), samplestr, w);
      Fill("corr_PtEm1_Over_PhiMhtEm1",		mht, (phi_mht_em1==0?1.:    g_pt/phi_mht_em1), samplestr, w);
      Fill("corr_PtEm1_Over_PhiEm1Recoil",		mht, (phi_recoil_em1==0?1.: g_pt/phi_recoil_em1), samplestr, w);
      Fill("corr_PtEm1_Over_PhiMhtRecoil",		mht, (phi_mht_recoil==0?1.: g_pt/phi_mht_recoil), samplestr, w);
      Fill("corr_Mht_Over_PhiMhtEm1",		mht, (phi_mht_em1==0?1.:    mht/phi_mht_em1), samplestr, w);
      Fill("corr_Mht_Over_PhiEm1Recoil",		mht, (phi_recoil_em1==0?1.: mht/phi_recoil_em1), samplestr, w);
      Fill("corr_Mht_Over_PhiMhtRecoil",		mht, (phi_mht_recoil==0?1.: mht/phi_mht_recoil), samplestr, w);
      Fill("corr_PhiMhtEm1_Over_PhiMhtRecoil",	mht, (phi_mht_recoil==0?1.: phi_mht_em1/phi_mht_recoil), samplestr, w);
      Fill("corr_PhiMhtEm1_Over_PhiEm1Recoil",	mht, (phi_recoil_em1==0?1.: phi_mht_em1/phi_recoil_em1), samplestr, w);
    */

    int njet = JetMult(  g_pt, g_eta, g_phi, t->jets_pt, t->jets_eta, t->jets_phi, t->jets_);
    Fill("n_jet",       njet, weight, we, bin, samplestr, w);
    Fill("n_loose",     LooseMult(t->photons_,t->photons_pt, t->photons__ptJet, t->photons_phi, t->photons_eta,t->photons_hadTowOverEm,t->photons_sigmaIetaIeta,t->photons_chargedIso,t->photons_neutralIso,t->photons_photonIso,t->photons_pixelseed), weight, we, bin, samplestr, w);
    Fill("n_tight",     TightMult(t->photons_,t->photons_pt, t->photons__ptJet, t->photons_phi, t->photons_eta,t->photons_hadTowOverEm,t->photons_sigmaIetaIeta,t->photons_chargedIso,t->photons_neutralIso,t->photons_photonIso,t->photons_pixelseed), weight, we, bin, samplestr, w);

    Fill("n_mu",       t->muons_, weight, we, bin, samplestr, w);
    Fill("n_e",        t->electrons_, weight, we, bin, samplestr, w);
    Fill("n_l",        t->muons_+t->electrons_, weight, we, bin, samplestr, w);

    if (t->met<30)       Fill("njet_met0_30",       njet, weight, we, bin, samplestr, w);
    else if (t->met<60)  Fill("njet_met30_60",      njet, weight, we, bin, samplestr, w);
    else if (t->met<100) Fill("njet_met60_100",     njet, weight, we, bin, samplestr, w);

//  if (we * w * t->weight) Fill("met_systerr", t->met,  we * w * t->weight, 0, 0);

//  std::cout << "Closure<T>::Process(T*t,Long64_t i="<<i<<",Long64_t n="<<n<<",double w="<<w<<") Done!"<<std::endl;

    return true;//Processor<T>::Process(t,i,n,w);
}


void RatioPlot(TH1*a, TH1*b, TH1*c, std::vector<TH1*> *sig, std::vector<TH1*> *other, const std::string& dir, const std::string& file, const std::string& t);
void Add2(TH1*h1, TH1*h2);
void PrintResults(const std::string& dir, std::string file, std::string name, MyYields* direct, MyYields* yields, std::vector<MyYields*>* other );

template<typename T>
void Closure<T>::Write()
{

    for (std::map<std::string,MyYields*>::iterator it=yields_.begin(); it!=yields_.end(); ++it) {
        TH1 * pred = it->second->GetPlot(it->first);
        TH1 * we   = it->second->GetWeightErrorPlot(it->first);
        //if (we) RatioPlot(0, we, 0, 0, 0, dir_, Closure<T>::name_+"_"+it->first+"_QCD_error", legtitel_);
        pred->SetTitle( it->second->Name().c_str() );
        TH1 * direct = 0;
        std::vector<TH1*> signal, other;
        if (direct_[it->first]) {
            direct = direct_[it->first]->GetPlot( it->first );
            direct->SetTitle( direct_[it->first]->Name().c_str() );
            if (signal_[it->first].size()) {
                for (std::vector<MyYields*>::iterator s=signal_[it->first].begin(); s!=signal_[it->first].end(); ++s) {
                    if (! *s) continue;
                    TH1 * sig = (*s)->GetPlot( it->first );
                    if (!sig) continue;
                    sig->SetLineColor( (*s)->LineColor() );
                    sig->SetLineWidth( 3 );
                    sig->SetTitle( (*s)->Name().c_str() );
                    signal.push_back( sig );
                }
            }
            if (other_[it->first].size()) {
                for (std::vector<MyYields*>::iterator s=other_[it->first].begin(); s!=other_[it->first].end(); ++s) {
                    TH1* oth = (*s)->GetPlot( it->first );
                    if (!oth) continue;
                    TH1 * oe = (*s)->GetWeightErrorPlot( it->first );
                    //RatioPlot(0, oe, 0, 0, 0, dir_, Closure<T>::name_+"_"+it->first+"_"+(*s)->Name().c_str()+"_error", legtitel_);
                    Add2( we, oe );
                    oth->SetLineColor( (*s)->FillColor()  );
                    oth->SetFillColor( (*s)->FillColor()  );
                    oth->SetTitle( (*s)->Name().c_str() );
                    other.push_back( oth);
                }
            }
            //if (we) RatioPlot(0, we, 0, 0, 0, dir_, Closure<T>::name_+"_"+it->first+"_Combined_error", legtitel_);
            RatioPlot(direct, pred, we, &signal, &other, dir_, Closure<T>::name_+"_"+it->first, legtitel_);
            PrintResults( dir_, Closure<T>::name_+"_"+it->first, it->first, direct_[it->first], yields_[it->first], &other_[it->first] );
        }
    }

    if (!denominator_ || !nominator_) return;

    std::map<std::string,Binnings*> * binning = denominator_->GetBinning();
    std::string axis[3];
    int i=0;
    for (std::map<std::string,Binnings*>::iterator it=binning->begin(); (it!=binning->end()&&i!=3); ++it)
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
        for (int x=0; x<(*binning)[axis[0]]->GetNBins(); ++x) {
            w->SetBinContent(x,weights_[x] );
            w->SetBinError(x,weighterrors_[x] );
        }
        struct stat st= {0};
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
        for (int i=0; i<n1; ++i) {
            a1[i]=(*v1)[i];
        }
        std::vector<double>* v2 = (*binning)[axis[1]]->GetArray();
        int n2=v2->size();
        double a2[n2];
        for (int i=0; i<n2; ++i) {
            a2[i]=(*v2)[i];
        }

        TH2F * w = new TH2F( ss.str().c_str(),
                             ((std::string)"QCD weighting;"+axis[0]+";"+axis[1]+";weight").c_str(),
                             n1-1,a1,n2-1,a2
                           );
        TH1F * corr_x = new TH1F(((std::string)"h_corr_x_"+Processor<T>::name_).c_str(),
                                 ((std::string)"Correlation;"+axis[0]+";"+axis[1]).c_str(),
                                 n1-1,a1);
        TH1F * corr_y = new TH1F(((std::string)"h_corr_y_"+Processor<T>::name_).c_str(),
                                 ((std::string)"Correlation;"+axis[1]+";"+axis[0]).c_str(),
                                 n2-1,a2);
        TH2F * we = (TH2F*)w->Clone();
        TH2F * nom = (TH2F*)w->Clone();
        //nom->SetTitle( nominator_->GetLabel().c_str() );
        TH2F * denom = (TH2F*)w->Clone();
        //denom->SetTitle( denominator_->GetLabel().c_str() );
        nom->SetStats(0);
        denom->SetStats(0);
        for (int x=0; x<(*binning)[axis[0]]->GetNBins(); ++x) {
            Yield yn, yd;
            for (int y=0; y<(*binning)[axis[1]]->GetNBins(); ++y) {
                w->SetBinContent(x,y,weights_[x+y*(*binning)[axis[0]]->GetNBins() ]);
                we->SetBinContent(x,y,weighterrors_[x+y*(*binning)[axis[0]]->GetNBins()] / weights_[x+y*(*binning)[axis[0]]->GetNBins() ]);
                nom->SetBinContent(x,y,nominator_->Weighted(x+y*(*binning)[axis[0]]->GetNBins() ));
                denom->SetBinContent(x,y,denominator_->Weighted(x+y*(*binning)[axis[0]]->GetNBins() ));

                yn.AddWeight( nominator_->GetYield(  x+y*(*binning)[axis[0]]->GetNBins() )->GetWeights() );
                yd.AddWeight( denominator_->GetYield(x+y*(*binning)[axis[0]]->GetNBins() )->GetWeights() );
            }
            float n=yn.weighted();
            float d=yd.weighted();
            float ne=yn.error();
            float de=yd.error();
            corr_x->SetBinContent( x, (d==0?1.0:n / d) );
            corr_x->SetBinError(   x, (d==0?1.0:sqrt( ne*ne/(d*d) + de*de*n*n/(d*d*d*d) ) ) );
        }
        for (int y=0; y<(*binning)[axis[1]]->GetNBins(); ++y) {
            Yield yn, yd;
            for (int x=0; x<(*binning)[axis[0]]->GetNBins(); ++x) {
                yn.AddWeight( nominator_->GetYield(  x+y*(*binning)[axis[0]]->GetNBins() )->GetWeights() );
                yd.AddWeight( denominator_->GetYield(x+y*(*binning)[axis[0]]->GetNBins() )->GetWeights() );
            }
            float n=yn.weighted();
            float d=yd.weighted();
            float ne=yn.error();
            float de=yd.error();
            corr_y->SetBinContent( y, (d==0?1.0:n / d) );
            corr_y->SetBinError(   y, (d==0?1.0: sqrt( ne*ne/(d*d) + de*de*n*n/(d*d*d*d) ) ) );
        }
        struct stat st= {0};
        if(stat(dir_.c_str(),&st)==-1)
            mkdir(dir_.c_str(), 0700);
        if(stat(((std::string)dir_+"/log/").c_str(),&st)==-1)
            mkdir(((std::string)dir_+"/log/").c_str(), 0700);
        TCanvas * c1 = new TCanvas("cw1","cw1",600,600);
        c1->cd();
        TPad *pad = new TPad(((std::string)"padc_"+ss.str()).c_str(),"padc",0,0,1,1);
        //pad->SetBottomMargin(0.1);
        pad->SetLeftMargin(0.12);
        pad->SetRightMargin(0.15);
        pad->SetLogy(0);
        pad->SetLogz(1);
        pad->Draw();
        pad->cd();
        w->SetStats(0);
        we->SetStats(0);
        w->SetTitle("");
        we->SetTitle("");
        nom->SetTitle("");
        denom->SetTitle("");
        gStyle->SetPalette(53,0);
        gStyle->SetNumberContours(512);
        w->GetZaxis()->SetRangeUser(0.8,20);
        we->GetZaxis()->SetRangeUser(0.005,1);
        w->GetYaxis()->SetTitleOffset(1.7);
        we->GetYaxis()->SetTitleOffset(1.7);
        w->GetZaxis()->SetTitleOffset(1.3);
        we->GetZaxis()->SetTitleOffset(1.3);
        we->GetZaxis()->SetTitle("weight rel. stat. uncertainty");
        nom->GetZaxis()->SetTitle("Events");
        denom->GetZaxis()->SetTitle("Events");
        w->Draw("Colz");
        c1->SaveAs(((std::string)"plots/"+dir_+"/log/h2_weight_"+Processor<T>::name_+".pdf").c_str());
        we->Draw("Colz");
        pad->RedrawAxis();
        c1->SaveAs(((std::string)"plots/"+dir_+"/log/h2_weighterror_"+Processor<T>::name_+".pdf").c_str());

        gStyle->SetPalette(55,0);
        gStyle->SetNumberContours(512);

        nom->GetYaxis()->SetTitleOffset(1.4);
        denom->GetYaxis()->SetTitleOffset(1.4);
        nom->Draw("Colz");
        c1->SaveAs(((std::string)"plots/"+dir_+"/log/h2_nominator_"+Processor<T>::name_+".pdf").c_str());
        denom->Draw("Colz");
        c1->SaveAs(((std::string)"plots/"+dir_+"/log/h2_denominator_"+Processor<T>::name_+".pdf").c_str());

        corr_x->Draw("pe");
        c1->SaveAs(((std::string)"plots/"+dir_+"/log/h1_correlationX_"+Processor<T>::name_+".pdf").c_str());
        corr_y->Draw("pe");
        c1->SaveAs(((std::string)"plots/"+dir_+"/log/h1_correlationY_"+Processor<T>::name_+".pdf").c_str());

        delete pad;
        delete we;
        delete w;
        delete nom;
        delete denom;
        delete corr_x;
        delete corr_y;
        delete c1;
        gStyle->SetPalette(1,0);
    }

}







///Class for the closure
template<typename T>
class EWK_prediction : public Closure<T> {
public:
    EWK_prediction(std::string d,std::string n,std::string titel=""):Closure<T>(d,n,titel) {    }
    virtual bool Process(T*t,Long64_t i,Long64_t n,double w) {
        double weight = w * (1. - 0.993 * (1. - std::pow(t->photons_pt[0] / 2.9 + 1.,-2.4))*
                             (1. - 0.23 * std::exp(-0.2777 * t->nTracksPV))*
                             (1. - 5.66e-4 * t->nVertex) );
        Closure<T>::weighterror_ = 0.11;
        return Closure<T>::Process(t,i,n,weight);
    }
};

template<typename T>
class ISR_prediction : public Closure<T> {
public:
    ISR_prediction(std::string d,std::string n,std::string titel=""):Closure<T>(d,n,titel) {    }
    virtual bool Process(T*t,Long64_t i,Long64_t n,double w) {
        Closure<T>::weighterror_ = 0.5; //50% uncertainty on the x-section
        return Closure<T>::Process(t,i,n,w);
    }
};

////Cutter
template<typename T>
class Cutter : public Processor<T> {
public:
    Cutter(std::string n):Processor<T>(n),d_tot(0),d_pass(0),i_tot(0), i_pass(0) {}
    //virtual void Init(){};
    virtual bool Process(T*t,Long64_t i,Long64_t n,double w) {
        ++i_tot;
        d_tot += w;
        //ROOT::Math::PtEtaPhiEVector recoil = Recoil(t->ThePhotonPt, t->ThePhotonEta, t->ThePhotonPhi, t->jets_pt, t->jets_eta, t->jets_phi, t->jets_ );
        //float ptrecoil = Recoil_pt (&recoil);
        //||
        // || t->photons__ptJet[t->ThePhoton]<=0
        // Recoil_pt(  &recoil )<150.
        if ( t->ThePhotonPt<110.
           ) {
            return false;
        }
        ++i_pass;
        d_pass += w;
        return true;
    }
    virtual void Terminate() {
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
class FinalCuts : public Processor<T> {
public:
    FinalCuts(std::string n):Processor<T>(n),d_tot(0),d_pass(0),i_tot(0), i_pass(0) {}
    //virtual void Init(){};
    virtual bool Process(T*t,Long64_t i,Long64_t n,double w) {
        ++i_tot;
        d_tot += w;

        if ( t->met<100.
           ) {
            return false;
        }
        ++i_pass;
        d_pass += w;
        return true;
    }
    virtual void Terminate() {
        std::cout << "  Summary Final Cuts '"<<Processor<T>::name_<<"' Terminate: "<<d_pass<<" ("<<i_pass<<") / "
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
    Cutter_looseID(std::string n):Cutter<T>(n) {}
    //virtual void Init(){};
    virtual bool Process(T*t,Long64_t i,Long64_t n,double w) {
        ++Cutter<T>::i_tot;
        Cutter<T>::d_tot += w;

        /*
              if (!t->photons_) return false;
              t->ThePhotonPt  = (t->photons__ptJet[0]>0?t->photons__ptJet[0]:t->photons_pt[0]);
              t->ThePhotonPhi = t->photons_phi[0];
              t->ThePhotonEta = t->photons_eta[0];
              t->ThePhoton = 0;
              t->metPhi=0;
              t->metSig=0;
        */
        if (fabs(t->metPhi)>2*kPI) t->metPhi=0;
//      t->metSig=0;

        double found_pt = 0;
        for (int i=0; i<t->photons_; ++i) {
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
    Cutter_tightID(std::string n):Cutter<T>(n) {}
    //virtual void Init(){};
    virtual bool Process(T*t,Long64_t i,Long64_t n,double w) {
        ++Cutter<T>::i_tot;
        Cutter<T>::d_tot += w;
        /*
              if (!t->photons_) return false;
              t->ThePhotonPt  = (t->photons__ptJet[0]>0?t->photons__ptJet[0]:t->photons_pt[0]);
              t->ThePhotonPhi = t->photons_phi[0];
              t->ThePhotonEta = t->photons_eta[0];
              t->ThePhoton = 0;
              t->metPhi=0;
              t->metSig=0;
        */
        if (fabs(t->metPhi)>2*kPI) t->metPhi=0;
        //t->metSig=0;

        double found_pt = 0;
        for (int i=0; i<t->photons_; ++i) {
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
        //Debugging for Knut:
        //if (t->ThePhoton!=0)
        //  std::cout << "evt nr:" << t->eventNumber << ", rn: " <<t->runNumber<<", lbnr: "<< t->luminosityBlockNumber <<std::endl;

        ++Cutter<T>::i_pass;
        Cutter<T>::d_pass += w;
        return true;
    }
};


////Cutter
template<typename T>
class Cutter_tightID_SansLeptonVeto : public Cutter<T> {
public:
    Cutter_tightID_SansLeptonVeto(std::string n):Cutter<T>(n) {}
    //virtual void Init(){};
    virtual bool Process(T*t,Long64_t i,Long64_t n,double w) {
        ++Cutter<T>::i_tot;
        Cutter<T>::d_tot += w;

        if (fabs(t->metPhi)>2*kPI) t->metPhi=0;
        //t->metSig=0;

        double found_pt = 0;
        for (int i=0; i<t->photons_; ++i) {
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

        if (found_pt==0)
            return false;
        //Debugging for Knut:
        //if (t->ThePhoton!=0)
        //  std::cout << "evt nr:" << t->eventNumber << ", rn: " <<t->runNumber<<", lbnr: "<< t->luminosityBlockNumber <<std::endl;

        ++Cutter<T>::i_pass;
        Cutter<T>::d_pass += w;
        return true;
    }
};



////Cutter
template<typename T>
class Cutter_electronID : public Cutter<T> {
public:
    Cutter_electronID(std::string n):Cutter<T>(n) {}
    //virtual void Init(){};
    virtual bool Process(T*t,Long64_t i,Long64_t n,double w) {
        ++Cutter<T>::i_tot;
        Cutter<T>::d_tot += w;

        //Nothing to do here: Knut says first entry of photon array (of photonElectronTree) is a good electron with tight ID except picexl seed veto
        if (t->photons_<=0) return false;

        t->ThePhotonPt  = (t->photons__ptJet[0]>0?t->photons__ptJet[0]:t->photons_pt[0]);;
        t->ThePhotonPhi = t->photons_phi[0];
        t->ThePhotonEta = t->photons_eta[0];
        t->ThePhoton = 0;

        ++Cutter<T>::i_pass;
        Cutter<T>::d_pass += w;

        if (!LeptonVeto(t->electrons_, t->electrons_pt, t->electrons_eta,t->muons_, t->muons_pt, t->muons_eta)   )
            return false;

        return true;
    }
};


///Doublicate Event Filter -------------------------------------------------------------------------
///Helper class for doublicate event filter
struct EventId {
    EventId(UInt_t nr, UInt_t lumi, UInt_t run):nr(nr),lumi(lumi),run(run) {}
    UInt_t nr, lumi, run;
    bool operator<(const EventId& rh) const {
        return (nr!=rh.nr? nr<rh.nr : (lumi!=rh.lumi? lumi<rh.lumi : run<rh.run) );
    }
};


////Doublecate event filter
template<typename T>
class DoubleCountFilter : public Processor<T> {
public:

    DoubleCountFilter(std::string n):Processor<T>(n),d_tot(0),d_pass(0),i_tot(0), i_pass(0) {}
    //virtual void Init(){};
    virtual bool Process(T*t,Long64_t i,Long64_t n,double w) {
        ++i_tot;
        d_tot += w;
        if (evts_.insert( EventId (t->eventNumber, t->runNumber, t->luminosityBlockNumber) ).second) {
            ++i_pass;
            d_pass += w;
            return true;
        }
        else {
            //Debugging for Knut:
            //std::cout << "evt nr:" << t->eventNumber << ", rn: " <<t->runNumber<<", lbnr: "<< t->luminosityBlockNumber <<std::endl;
            return false;
        }
    }
    virtual void Terminate() {
        std::cout << "  Doublicate Event Filter '"<<Processor<T>::name_<<"' Terminate: "<<d_pass<<" ("<<i_pass<<") / "
                  << d_tot << " ("<<i_tot<<") passed all cuts";
        if (d_tot&&i_tot) std::cout << ", i.e. "<< d_pass/d_tot*100.<<"% ("<<100.*i_pass/i_tot<<"%)."<<std::endl;
    }

    std::set<EventId> * Get() {
        return &evts_;
    }
    void Set(std::set<EventId>*l) {
        evts_.insert(l->begin(),l->end());
    }
protected:
    std::set<EventId> evts_;
    double d_tot, d_pass;
    int    i_tot, i_pass;
};


#endif
