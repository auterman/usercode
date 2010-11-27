#ifndef GENERATORMASSES_H
#define GENERATORMASSES_H

#include <vector>
#include <string>

class GeneratorMasses {
 public:
//  GeneratorMasses(){}
  double MZERO;
  double MHALF;
  double TANB;
  double SGNMU;
  double AZERO;
  double MTOP;
  double muQ;
  double Q;
  double M1;
  double M2;
  double M3;
  double MGL;
  double MUL;
  double MB1;
  double MSN;
  double MNTAU;
  double MZ1;
  double MW1;
  double MHL;
  double MUR;
  double MB2;
  double MEL;
  double MTAU1;
  double MZ2;
  double MW2;
  double MHH;
  double MDL;
  double MT1;
  double MER;
  double MTAU2;
  double MZ3;
  double MHA;
  double MDR;
  double MT2;
  double MZ4;
  double MHp;
};

std::vector<GeneratorMasses> FillGeneratorMasses(std::string file);

#endif
