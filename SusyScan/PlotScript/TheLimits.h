#ifndef THELIMITS_H
#define THELIMITS_H

#include <vector>

class SusyScan;
class TH2;
class GeneratorMasses;

class TheLimits{
 public:
  TheLimits(){}

  void add(SusyScan*s){_scan.push_back(s);}
  void plot(TH2*, double(*x)(const SusyScan*), double(*y)(const SusyScan*), double(*f)(const SusyScan*) );
  void match(const std::vector<GeneratorMasses>);
  
  std::vector<SusyScan*> * GetScan(){return &_scan;}
  
 private:
  std::vector<SusyScan*> _scan;
};


#endif
