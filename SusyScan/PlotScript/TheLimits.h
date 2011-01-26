#ifndef THELIMITS_H
#define THELIMITS_H

#include <vector>
#include <string>

class SusyScan;
class TH2;
class GeneratorMasses;

class TheLimits{
 public:
  TheLimits(){}

  void add(SusyScan*s){_scan.push_back(s);}
  void FillGeneratorMasses(std::string file);

  void match();
  
  std::vector<SusyScan*> * GetScan(){return &_scan;}
  std::vector<GeneratorMasses*> * GetGeneratorMasses(){return &_masses;}
  
  void OverwriteLimits(std::string flag);
  
 private:
  std::vector<SusyScan*> _scan; 
  std::vector<GeneratorMasses*> _masses;
};


#endif
