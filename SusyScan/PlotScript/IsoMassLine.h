#ifndef ISOMASSLINES_H
#define ISOMASSLINES_H

#include <vector>

class TGraph;
class SusyScan;

class IsoMassLine {
 public:
  IsoMassLine(std::vector<SusyScan*> * scan):_scan(scan){}
  TGraph * get( double(*x)(const SusyScan*), double(*y)(const SusyScan*), 
                double(*func)(const SusyScan*), double mass, double diff=5.);
  
 private:
  std::vector<SusyScan*> * _scan; 
};

#endif
