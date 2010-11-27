#ifndef PLOTTOOLS_H
#define PLOTTOOLS_H

#include <vector>

class TGraph;
class SusyScan;
class TH2;

template<class T>
class PlotTools {
 public:
  PlotTools(std::vector<T*> * scan):_scan(scan){}
  
  TGraph * Line( double(*x)(const T*), double(*y)(const T*), 
                double(*func)(const T*), const double mass, const double diff=5.);
  
  void Area(  TH2*h, double(*x)(const T*), double(*y)(const T*), 
              double(*func)(const T*));
  
  TGraph * GetContour(TH2*, int flag=0);

 private:
  std::vector<T*> * _scan; 
  
 class sort_by{
  public:
   sort_by(double(*x)(const T*)):_f(x){}
   bool operator()(const T*a, const T*b){ return _f(a)<_f(b); }
  private:
   double(*_f)(const T*);
 };
};

#endif
