#ifndef PLOTTOOLS_H
#define PLOTTOOLS_H

#include <vector>

class TGraph;
class SusyScan;
class TH2;

TGraph * MakeBand(TGraph *g1, TGraph *g2, bool b=false);

void Smooth(TGraph * g, int n=3);

template<class T>
class PlotTools {
 public:
  PlotTools(std::vector<T*> * scan):_scan(scan){}
  
  TGraph * Line( double(*x)(const T*), double(*y)(const T*), 
                 double(*func)(const T*), const double mass, const double diff=5.);
  
  void Area(  TH2*h, double(*x)(const T*), double(*y)(const T*), 
              double(*func)(const T*));
  
  void Graph(  TGraph*g, double(*x)(const T*), double(*y)(const T*), double ymin=-999. );
  
  TGraph * GetContour(TH2*, int ncont=20, int flag=0);
  std::vector<TGraph *> GetContours(TH2*, int ncont=20);

  TGraph * GetContour(TH2*,double(*x)(const T*), double(*y)(const T*), 
                      double(*func)(const T*), int ncont=20, int flag=0,
		      int color=1, int style=1);

  void Print(double(*x)(const T*), double(*x)(const T*), double(*y)(const T*), 
             TGraph*, double p=10.);

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
