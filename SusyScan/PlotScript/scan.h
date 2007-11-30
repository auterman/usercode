#ifndef SCAN_H
#define SCAN_H

#include <vector>

#include <TH2.h>
#include <TGraph.h>
#include <TGraph2D.h>

class MyTGraph : public TGraph{
 public:
   MyTGraph(int b):TGraph(b){};
   void Add(double x,double y){ this->SetPoint( this->GetN(),x,y); };
   void ls(){  
      double x,y;
      for (int i=0; i<this->GetN(); ++i){
        this->GetPoint(i,x,y);
        std::cout << i << "th: "<<x<<", " <<y<<std::endl;
      }};
};

class SUSY_POINT {
 public:
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


class TScan {
 public:
   TScan();
   ~TScan();

   MyTGraph * IsoMassLine(bool(*func)(SUSY_POINT,double), double);
   MyTGraph * GetContour(TH2F*, int flag=0);
   TH2F * Area(bool(*func)(SUSY_POINT));
   TH2F * Area51();
   //static bool ul(const SUSY_POINT point, const double mass);
 
   int DoStuff();
 
 private:
   void ReadGeneratedMasses(std::string file);

   int plot_id, bins_x, bins_y;
   double min_x, max_x, min_y, max_y;
   std::vector<SUSY_POINT> points;

};


#endif
