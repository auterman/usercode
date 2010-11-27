#ifndef ISOMASSLINES_H
#define ISOMASSLINES_H

#include <vector>

class TGraph;
class SusyScan;

template<class T>
class IsoMassLine {
 public:
  IsoMassLine(std::vector<T*> * scan):_scan(scan){}
  TGraph * get( double(*x)(const T*), double(*y)(const T*), 
                double(*func)(const T*), const double mass, const double diff=5.);
  
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
