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

class SUSY_XSECS {
 public:
   double MZERO;
   double MHALF;
   double TANB;
   double SGNMU;
   double AZERO;
   double MTOP;
   double XS0;
   double XS201;
   double XS202;
   double XS204;
   double XS205;
   double XS207;
   double XS208;
   double XS209;
   double XS210;
   double XS211;
   double XS212;
   double XS213;
   double XS214;
   double XS216;
   double XS217;
   double XS218;
   double XS219;
   double XS220;
   double XS221;
   double XS222;
   double XS223;
   double XS224;
   double XS225;
   double XS226;
   double XS227;
   double XS228;
   double XS229;
   double XS230;
   double XS231;
   double XS232;
   double XS233;
   double XS234;
   double XS235;
   double XS236;
   double XS237;
   double XS238;
   double XS239;
   double XS240;
   double XS241;
   double XS242;
   double XS243;
   double XS244;
   double XS246;
   double XS247;
   double XS248;
   double XS249;
   double XS250;
   double XS251;
   double XS252;
   double XS253;
   double XS254;
   double XS256;
   double XS258;
   double XS259;
   double XS261;
   double XS262;
   double XS263;
   double XS264;
   double XS265;
   double XS271;
   double XS272;
   double XS273;
   double XS274;
   double XS275;
   double XS276;
   double XS277;
   double XS278;
   double XS279;
   double XS280;
   double XS281;
   double XS282;
   double XS283;
   double XS284;
   double XS285;
   double XS286;
   double XS287;
   double XS288;
   double XS289;
   double XS290;
   double XS291;
   double XS292;
   double XS293;
   double XS294;
   double XS295;
   double XS296;
   double XS297;
   double XS298;
   double XS299;
   double XS300;
   double XS301;
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
   void ReadGeneratedXsects(std::string file);

   int plot_id, bins_x, bins_y;
   double min_x, max_x, min_y, max_y;
   std::vector<SUSY_POINT> points;
   std::vector<SUSY_XSECS> xsects;

};


#endif
