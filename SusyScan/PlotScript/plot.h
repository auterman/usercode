#ifndef PLOT_H
#define PLOT_H

#include <vector>

class SusyScan;
class TH2;

class TheLimits{
 public:
  TheLimits(){}

  void add(SusyScan*s){_scan.push_back(s);}
  void plot(TH2*, double(*x)(const SusyScan*), double(*y)(const SusyScan*), double(*f)(const SusyScan*) );
  
 private:
  std::vector<SusyScan*> _scan;
};


#endif
