#ifndef PLOTTOOLS_H
#define PLOTTOOLS_H

#include <vector>

class TGraph;
class SusyScan;
class TH2;
class TCanvas;

TGraph * MakeBand(TGraph *g1, TGraph *g2, bool b=false);
TGraph * GetNeutrNNLSP();
TGraph * GetExcludedRegion(TGraph * lowerLimit, double min1,double min2, double max1,double max2);
void Smooth(TGraph * g, int n=3);
void Smooth2D(TGraph * g, int n=3);
TGraph* Close2D(TGraph * g);
void drawCmsPrel(double lumi,double METCut, bool onlyChannelInfo=false, int noJets=3,bool isBestjet=false);
void drawCmsPrelInCanvas(double intLumi);
//void MergeBins(TH1*);

template<class T>
class PlotTools {
 public:
  PlotTools(std::vector<T*> * scan):_scan(scan){}
  
  TGraph * Line( double(*x)(const T*), double(*y)(const T*), 
                 double(*func)(const T*), const double mass, const double diff=5.);
  
  void Area(  TH2*h, double(*x)(const T*), double(*y)(const T*), 
              double(*func)(const T*));
  double SingleValue( double(*func)(const T*) );
  
  void Graph(  TGraph*g, double(*x)(const T*), double(*y)(const T*), double ymin=-999. );
  
  TGraph * GetContour(TH2*, int ncont=20, int flag=0);
  std::vector<TGraph *> GetContours(TH2*, int ncont=20);

  TGraph * GetContour005(TH2*, int ncont=20, int flag=0);
  std::vector<TGraph *> GetContours005(TH2*, int ncont=20);

  TGraph * GetContour(TH2*,double(*x)(const T*), double(*y)(const T*), 
                      double(*func)(const T*), int ncont=20, int flag=0,
		      int color=1, int style=1);

  TGraph * GetContour005(TH2*,double(*x)(const T*), double(*y)(const T*), 
                      double(*func)(const T*), int ncont=20, int flag=0,
		      int color=1, int style=1);

  void Print(double(*x)(const T*), double(*x2)(const T*), double(*y)(const T*),
             TGraph*, double p=10.);
  void Print(double(*x)(const T*), double(*x2)(const T*), double(*y)(const T*), double(*x3)(const T*), double(*y2)(const T*),
             TGraph*, double p=10.);

  TGraph * ChooseBest(TGraph*,TGraph*,TGraph*,TGraph*,double x=0,double y=0);
  
  TH2 * BinWiseOr(TH2*, TH2*);
  
  TGraph * ModifyExpSigma(TGraph*, TGraph*, TGraph*);
  TGraph * ModifyExpSigmaY(TGraph*, TGraph*, TGraph*);

 private:
  std::vector<T*> * _scan; 
  
 class sort_by{
  public:
   sort_by(double(*x)(const T*)):_f(x){}
   bool operator()(const T*a, const T*b){ return _f(a)<_f(b); }
  private:
   double(*_f)(const T*);
 };
 
 class sort_TGraph{
  public:
   sort_TGraph(){}
   bool operator()(const TGraph*g1, const TGraph*g2);
 };
 
};

#endif
