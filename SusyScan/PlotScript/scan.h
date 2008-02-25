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
   double XS0;         // I   0 All  // Included subprocesses // I 
   double XS201;       // I 201
   double XS202;       // I 202 f + fbar -> ~e_R + ~e_Rbar    // I 
   double XS204;       // I 204 f + fbar -> ~mu_L + ~mu_Lbar  // I 
   double XS205;       // I 205 f + fbar -> ~mu_R + ~mu_Rbar  // I 
   double XS207;       // I 207 f+fbar -> ~tau_1 + ~tau_1bar  // I 
   double XS208;       // I 208 f+fbar -> ~tau_2 + ~tau_2bar  // I 
   double XS209;       // I 209 f+fbar -> ~tau_1 + ~tau_2bar  // I 
   double XS210;       // I 210
   double XS211;       // I 211 q+qbar'-> ~tau_1 + ~nutaubar  // I     
   double XS212;       // I 212 q+qbar'-> ~tau_2 + ~nutaubar  // I 
   double XS213;       // I 213 f + fbar -> ~nul + ~nulbar    // I 
   double XS214;       // I 214 f+fbar -> ~nutau + ~nutaubar  // I 
   double XS216;       // I 216 f + fbar -> ~chi1 + ~chi1     // I 
   double XS217;       // I 217 f + fbar -> ~chi2 + ~chi2     // I 
   double XS218;       // I 218 f + fbar -> ~chi3 + ~chi3     // I 
   double XS219;       // I 219 f + fbar -> ~chi4 + ~chi4     // I 
   double XS220;       // I 220 f + fbar -> ~chi1 + ~chi2     // I 
   double XS221;       // I 221 f + fbar -> ~chi1 + ~chi3     // I 
   double XS222;       // I 222 f + fbar -> ~chi1 + ~chi4     // I 
   double XS223;       // I 223 f + fbar -> ~chi2 + ~chi3     // I 
   double XS224;       // I 224 f + fbar -> ~chi2 + ~chi4     // I 
   double XS225;       // I 225 f + fbar -> ~chi3 + ~chi4     // I 
   double XS226;       // I 226 f+fbar -> ~chi+-1 + ~chi-+1   // I 
   double XS227;       // I 227 f+fbar -> ~chi+-2 + ~chi-+2   // I 
   double XS228;       // I 228 f+fbar -> ~chi+-1 + ~chi-+2   // I 
   double XS229;       // I 229 q + qbar' -> ~chi1 + ~chi+-1  // I 
   double XS230;       // I 230 q + qbar' -> ~chi2 + ~chi+-1  // I 
   double XS231;       // I 231 q + qbar' -> ~chi3 + ~chi+-1  // I 
   double XS232;       // I 232 q + qbar' -> ~chi4 + ~chi+-1  // I 
   double XS233;       // I 233 q + qbar' -> ~chi1 + ~chi+-2  // I 
   double XS234;       // I 234 q + qbar' -> ~chi2 + ~chi+-2  // I 
   double XS235;       // I 235 q + qbar' -> ~chi3 + ~chi+-2  // I 
   double XS236;       // I 236 q + qbar' -> ~chi4 + ~chi+-2  // I 
   double XS237;       // I 237 q + qbar -> ~chi1 + ~g        // I 
   double XS238;       // I 238 q + qbar -> ~chi2 + ~g        // I 
   double XS239;       // I 239 q + qbar -> ~chi3 + ~g        // I 
   double XS240;       // I 240 q + qbar -> ~chi4 + ~g        // I 
   double XS241;       // I 241 q + qbar' -> ~chi+-1 + ~g     // I 
   double XS242;       // I 242 q + qbar' -> ~chi+-2 + ~g     // I 
   double XS243;       // I 243 q + qbar -> ~g + ~g	      // I 
   double XS244;       // I 244 g + g -> ~g + ~g	      // I 
   double XS246;       // I 246 qj + g -> ~qj_L + ~chi1       // I 
   double XS247;       // I 247 qj + g -> ~qj_R + ~chi1       // I 
   double XS248;       // I 248 qj + g -> ~qj_L + ~chi2       // I 
   double XS249;       // I 249 qj + g -> ~qj_R + ~chi2       // I 
   double XS250;       // I 250 qj + g -> ~qj_L + ~chi3       // I 
   double XS251;       // I 251 qj + g -> ~qj_R + ~chi3       // I 
   double XS252;       // I 252 qj + g -> ~qj_L + ~chi4       // I 
   double XS253;       // I 253 qj + g -> ~qj_R + ~chi4       // I 
   double XS254;       // I 254 qj + g -> ~qk_L + ~chi+-1     // I 
   double XS256;       // I 256 qj + g -> ~qk_L + ~chi+-2     // I 
   double XS258;       // I 258 qj + g -> ~qj_L + ~g	      // I 
   double XS259;       // I 259 qj + g -> ~qj_R + ~g	      // I 
   double XS261;       // I 261 f + fbar -> ~t_1 + ~t_1bar    // I 
   double XS262;       // I 262 f + fbar -> ~t_2 + ~t_2bar    // I 
   double XS263;       // I 263 f + fbar -> ~t_1 + ~t_2bar    // I 
   double XS264;       // I 264 g + g -> ~t_1 + ~t_1bar       // I 
   double XS265;       // I 265 g + g -> ~t_2 + ~t_2bar       // I 
   double XS271;       // I 271 qi + qj -> ~qi_L + ~qj_L      // I 
   double XS272;       // I 272 qi + qj -> ~qi_R + ~qj_R      // I 
   double XS273;       // I 273 qi + qj -> ~qi_L + ~qj_R      // I 
   double XS274;       // I 274 qi+qjbar -> ~qi_L + ~qj_Lbar  // I 
   double XS275;       // I 275 qi+qjbar -> ~qi_R + ~qj_Rbar  // I 
   double XS276;       // I 276 qi+qjbar -> ~qi_L + ~qj_Rbar  // I 
   double XS277;       // I 277 f + fbar -> ~qi_L + ~qi_Lbar  // I 
   double XS278;       // I 278 f + fbar -> ~qi_R + ~qi_Rbar  // I 
   double XS279;       // I 279 g + g -> ~qi_L + ~qi_Lbar     // I 
   double XS280;       // I 280 g + g -> ~qi_R + ~qi_Rbar     // I 
   double XS281;       // I 281 b + qj -> ~b_1 + ~qj_L        // I 
   double XS282;       // I 282 b + qj -> ~b_2 + ~qj_R        // I 
   double XS283;       // I 283 b + qj -> ~b_1 + ~qj_R        // I 
   double XS284;       // I 284 b + qjbar -> ~b_1 + ~qj_Lbar  // I 
   double XS285;       // I 285 b + qjbar -> ~b_2 + ~qj_Rbar  // I 
   double XS286;       // I 286 b + qjbar -> ~b_1 + ~qj_Rbar  // I 
   double XS287;       // I 287 f + fbar -> ~b_1 + ~b_1bar    // I 
   double XS288;       // I 288 f + fbar -> ~b_2 + ~b_2bar    // I 
   double XS289;       // I 289 g + g -> ~b_1 + ~b_1bar       // I 
   double XS290;       // I 290 g + g -> ~b_2 + ~b_2bar       // I 
   double XS291;       // I 291 b + b -> ~b_1 + ~b_1	      // I 
   double XS292;       // I 292 b + b -> ~b_2 + ~b_2	      // I 
   double XS293;       // I 293 b + b -> ~b_1 + ~b_2	      // I 
   double XS294;       // I 294 b + g -> ~b_1 + ~g	      // I 
   double XS295;       // I 295 b + g -> ~b_2 + ~g	      // I 
   double XS296;       // I 296 b + bbar -> ~b_1 + ~b_2bar    // I 
   double XS297;       // I 297 f + fbar' -> H+/- + h0        // I 
   double XS298;       // I 298 f + fbar -> H+/- + H0	      // I 
   double XS299;       // I 299 f + fbar -> A0 + h0	      // I 
   double XS300;       // I 300 f + fbar -> A0 + H0	      // I 
   double XS301;       // I 301 f + fbar -> H+ + H-	      // I 

   //total susy x-sec
   double total() const {return XS0*1.0E9;};
   
   //gluino-gluino prod. x-sec in pb:
   double sgsg() const {return (XS243+XS244)*1.0E9;};

   //squark-gluino prod. x-sec in pb:
   double sqsg() const {return (XS258+XS259+XS294+XS295)*1.0E9;};

   //squark-squark prod. x-sec in pb:
   double sqsq() const {return (XS261+XS262+XS263+XS264+XS265+XS271+XS272+XS273+XS274+XS275+XS276+XS277+XS278+XS279+XS280+XS281+XS282+XS283+XS284+XS285+XS286+XS287+XS288+XS289+XS290+XS291+XS292+XS293+XS296)*1.0E9;};
};

class TScan {
 public:
   TScan();
   ~TScan();

   template <class T> MyTGraph * IsoMassLine(std::vector<T>, bool(*func)(T,double), double);
   MyTGraph * GetContour(TH2F*, int flag=0);
   template <class T> TH2F * TScan::Area(std::vector<T>,  bool(*func)(T));
   template <class T> TH2F * TScan::Area(std::vector<T>,double(*func)(T));
   TH2F * Area51();
   //static bool ul(const SUSY_POINT point, const double mass);
 
   int DoStuff();
 
 private:
   void ReadGeneratedMasses(std::string file);
   void ReadGeneratedXsects(std::string file);
   void SetBins(int bx, int by){bins_x=bx; bins_y=by;};

   int plot_id, bins_x, bins_y;
   double min_x, max_x, min_y, max_y;
   std::vector<SUSY_POINT> points;
   std::vector<SUSY_XSECS> xsects;

};


#endif
