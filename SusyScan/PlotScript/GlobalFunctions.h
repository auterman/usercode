#include "SusyScan.h"
#include "GeneratorMasses.h"

#include "TGraph.h"
#include "TGraphErrors.h"
#include "TSpline.h"

#include <cmath>
#include <iostream>


double Luminosity = 36.3; //[pb^-1]
double Mzero(const SusyScan* p){ return p->Mzero; }
double Mhalf(const SusyScan* p){ return p->Mhalf; }
double MGluino(const SusyScan* p){ return p->MGL; }
double MSquarkL(const SusyScan* p){ return p->MUL; }
double MSquarkR(const SusyScan* p){ return p->MUR; }
double MChi1(const SusyScan* p){ return p->MZ1; }
double MChi2(const SusyScan* p){ return p->MZ2; }
double MChi3(const SusyScan* p){ return p->MZ3; }
double MChi4(const SusyScan* p){ return p->MZ4; }
double MCha1(const SusyScan* p){ return p->MW1; }
double MCha2(const SusyScan* p){ return p->MW2; }
double Xsection(const SusyScan* p){ return p->Xsection; }
double ExpXsecLimit(const SusyScan* p){ return p->ExpXsecLimit; }
double ObsXsecLimit(const SusyScan* p){ return p->ObsXsecLimit; }
double Signal(const SusyScan* p){ return p->signal; }
double SignalUncertainty(const SusyScan* p){ return p->signal_uncertainty; }
double SignalRelUncertainty(const SusyScan* p){ return p->signal_uncertainty/p->signal; }
double ExpExclusion(const SusyScan* p){ return (ExpXsecLimit(p)<Xsection(p)&&ExpXsecLimit(p)>0.01?1:0.01); }
double ObsExclusion(const SusyScan* p){ return (ObsXsecLimit(p)<Xsection(p)&&ObsXsecLimit(p)>0.01?1:0.01); }
double ExpExclCL(const SusyScan* p){ return (p->CLs_b_xsec<=0.05 ? 1:0.01); }
double ExpExclCLm2sigma(const SusyScan* p){ return (p->CLs_b_n2_xsec<=0.05 ? 1:0.01); }
double ExpExclCLm1sigma(const SusyScan* p){ return (p->CLs_b_n1_xsec<=0.05 ? 1:0.01); }
double ExpExclCLp1sigma(const SusyScan* p){ return (p->CLs_b_p1_xsec<=0.05 ? 1:0.01); }
double ExpExclCLp2sigma(const SusyScan* p){ return (p->CLs_b_p2_xsec<=0.05 ? 1:0.01); }
double ObsExclCL(const SusyScan* p){ return (p->CLs_xsec<=0.05 ? 1:0.01); }
double SoverSqrtB(const SusyScan* p){ return p->signal/(sqrt(p->background)+p->background_uncertainty+p->signal_uncertainty); }
double XsecOverObserved(const SusyScan* p){ return (ObsXsecLimit(p)==0 ? 9999. : Xsection(p)/ObsXsecLimit(p)); }
double XsecOverExpected(const SusyScan* p){ return (ExpXsecLimit(p)==0 ? 9999. : Xsection(p)/ExpXsecLimit(p)); }
double SignalAcceptance(const SusyScan* p){ return  p->signal / (Luminosity*Xsection(p)); }
double ExpNSignLimit(const SusyScan* p){ return  p->ExpNsigLimit; }
double ObsNSignLimit(const SusyScan* p){ return  p->ObsNsigLimit; }
double PLExpNSignLimit(const SusyScan* p){ return  p->PLExpNsigLimit; }
double PLObsNSignLimit(const SusyScan* p){ return  p->PLObsNsigLimit; }
double PLExpXsecLimit(const SusyScan* p){ return p->PLExpXsecLimit; }
double PLObsXsecLimit(const SusyScan* p){ return p->PLObsXsecLimit; }
double PLExpExclusion(const SusyScan* p){ return (PLExpXsecLimit(p)<Xsection(p)&&PLExpXsecLimit(p)>0.01?1:0.01); }
double PLObsExclusion(const SusyScan* p){ return (PLObsXsecLimit(p)<Xsection(p)&&PLObsXsecLimit(p)>0.01?1:0.01); }
double FCExpNSignLimit(const SusyScan* p){ return  p->FCExpNsigLimit; }
double FCObsNSignLimit(const SusyScan* p){ return  p->FCObsNsigLimit; }
double FCExpXsecLimit(const SusyScan* p){ return p->FCExpXsecLimit; }
double FCObsXsecLimit(const SusyScan* p){ return p->FCObsXsecLimit; }
double FCExpExclusion(const SusyScan* p){ return (FCExpXsecLimit(p)<Xsection(p)&&FCExpXsecLimit(p)>0.01?1:0.01); }
double FCObsExclusion(const SusyScan* p){ return (FCObsXsecLimit(p)<Xsection(p)&&FCObsXsecLimit(p)>0.01?1:0.01); }
double MCMCExpNSignLimit(const SusyScan* p){ return  p->MCMCExpNsigLimit; }
double MCMCObsNSignLimit(const SusyScan* p){ return  p->MCMCObsNsigLimit; }
double MCMCExpXsecLimit(const SusyScan* p){ return p->MCMCExpXsecLimit; }
double MCMCObsXsecLimit(const SusyScan* p){ return p->MCMCObsXsecLimit; }
double MCMCExpExclusion(const SusyScan* p){ return (MCMCExpXsecLimit(p)<Xsection(p)&&MCMCExpXsecLimit(p)>0.01?1:0.01); }
double MCMCObsExclusion(const SusyScan* p){ return (MCMCObsXsecLimit(p)<Xsection(p)&&MCMCObsXsecLimit(p)>0.01?1:0.01); }
double SignalContamination(const SusyScan* p){return p->signal_contamination; }

double Mzero(const GeneratorMasses* p){ return p->Mzero; }
double Mhalf(const GeneratorMasses* p){ return p->Mhalf; }
double MGluino(const GeneratorMasses* p){ return p->MGL; }
double MSquarkL(const GeneratorMasses* p){ return p->MUL; }
double MSquarkR(const GeneratorMasses* p){ return p->MUR; }
double MChi1(const GeneratorMasses* p){ return p->MZ1; }
double MChi2(const GeneratorMasses* p){ return p->MZ2; }
double MChi3(const GeneratorMasses* p){ return p->MZ3; }
double MChi4(const GeneratorMasses* p){ return p->MZ4; }
double MCha1(const GeneratorMasses* p){ return p->MW1; }
double MCha2(const GeneratorMasses* p){ return p->MW2; }


//RA1 Contours
TGraph* set_sneutrino_d0_1(){
  double sn_m0[9]={0,0,55,90,100,110,100,55,0};
  double sn_m12[9]={0,140,220,240,240,230,210,150,0};

  TGraph* sn_d0_gr = new TGraph(9,sn_m0,sn_m12);

  sn_d0_gr->SetFillColor(kGreen+3);
  sn_d0_gr->SetFillStyle(1001);


  return sn_d0_gr;
}

TGraph* set_sneutrino_d0_2(){
  double sn_m0[5]={0,50,105,190,0};
  double sn_m12[5]={0,140,205,140,0};

  TGraph* sn_d0_gr_2 = new TGraph(5,sn_m0,sn_m12);

  sn_d0_gr_2->SetFillColor(kGreen+3);
  sn_d0_gr_2->SetFillStyle(1001);


  return sn_d0_gr_2;
}



TGraph* set_lep_ch(){

  double ch_m0[5];
  double ch_m12[5];

 
    ch_m0[0] = 0;
    ch_m0[1] = 0;
    ch_m0[2] = 600;
    ch_m0[3] = 1000;
    ch_m0[4] = 1000;

    ch_m12[0] = 0;
    ch_m12[1] = 130;
    ch_m12[2] = 120;
    ch_m12[3] = 113;
    ch_m12[4] = 0;

  
  TGraph* ch_gr = new TGraph(5,ch_m0,ch_m12);

  ch_gr->SetFillColor(3);
  ch_gr->SetFillStyle(1001);

  return ch_gr;

}


TGraph* set_lep_sl(){



  double sl_m0[] = {0,0,30,50,60,75,80,90,100};
  double sl_m12[] = {0,245,240,220,200,150,100,50,0}; 
  
  TGraph* lep_sl = new TGraph(9,sl_m0,sl_m12);

  lep_sl->SetFillColor(5);
  lep_sl->SetFillStyle(1001);
  
  return lep_sl;
}

TGraph* set_tev_sg_cdf(){

  double sg_m0[] = {0,50,100,150,200,250,300,350,400,450,500,550,600,600};
  double sg_m12[] = {0,170,160,155,150,122,116,112,110,106,105,100,98,0};
  TGraph* sg_gr = new TGraph(14,sg_m0,sg_m12);

  sg_gr->SetFillColor(2);
  sg_gr->SetFillStyle(1001); 

  return sg_gr;

}

TGraph* set_tev_sg_d0(){
  double sgd_m0[] = {0,50,100,150,200,250,300,350,400,450,500,550,600,600};
  double sgd_m12[] = {0,173,170,168,160,150,140,130,125,120,120,120,120,0};
  TGraph* sgd_gr = new TGraph(14,sgd_m0,sgd_m12);

  sgd_gr->SetFillColor(41);
  sgd_gr->SetFillStyle(1001);

  return sgd_gr;

}

TGraph* set_tev_tlp_cdf(){
  double tlp1_m0[] = {0,20,40,60,70,80,90,80,70,60};
  double tlp1_m12[] = {170,185,200,215,220,215,210,190,175,160};
  TGraph* tlp1_gr = new TGraph(10,tlp1_m0,tlp1_m12);

  tlp1_gr->SetFillColor(4);
  tlp1_gr->SetFillStyle(1001);

  return tlp1_gr;
}

TGraph* set_tev_tlp_d0(){
  double tlp2_m0[] = {70,80,90,100,105,110,120,130,140};
  double tlp2_m12[] = {160,172,184,196,205,195,185,173,160};
  TGraph* tlp2_gr = new TGraph(9,tlp2_m0,tlp2_m12);

  tlp2_gr->SetFillColor(4);
  tlp2_gr->SetFillStyle(1001); 

  return tlp2_gr;

}

TGraph* set_tev_stau(){
  double st_m0[] = {0,30,200,0,0};
  double st_m12[] = {230,240,1000,1000,230};
  TGraph* st_gr = new TGraph(5,st_m0,st_m12);

  st_gr->SetFillColor(40);
  st_gr->SetFillStyle(1001);


  return st_gr;

}


TGraphErrors* getLO_signalCont(){



  Int_t nl = 9;
  Double_t xl[9];
  Double_t yl[9];
  Double_t exl[9];
  Double_t eyl[9];
  
  // cout << " n " << hist->GetXaxis()->GetNbins() << endl;

  for(int i = 0; i < nl; i++){
    exl[i] = 5;
    eyl[i] = 5;

  }
  
  xl[0] = 0;
  yl[0] = 265;
  xl[1] = 100;
  yl[1] = 258;
  xl[2] = 200;
  yl[2] = 250;
  xl[3] = 250;
  yl[3] = 240;
  xl[4] = 300;
  yl[4] = 210;
  xl[5] = 340;
  yl[5] = 177;
  xl[6] = 400;
  yl[6] = 140;
  xl[7] = 460;
  yl[7] = 120;
  xl[8] = 490;
  yl[8] =100;


 
  
  
  
  TGraphErrors* gr1 = new TGraphErrors(nl,xl,yl,exl,eyl);
  gr1->SetMarkerColor(kGreen+2);
  gr1->SetMarkerStyle(21);
  
  
  //gr1->Draw("LP");

  TSpline3 *s = new TSpline3("grs",gr1);
  s->SetLineColor(kGreen+2);
  s->SetLineStyle(4);
  s->SetLineWidth(3);
  

  return gr1;



}


TGraphErrors* getExpected_NLOunc(){

 Int_t nl = 11;
  Double_t xl[11];
  Double_t yl[11];
  Double_t exl[11];
  Double_t eyl[11];
  
  // cout << " n " << hist->GetXaxis()->GetNbins() << endl;
  
  for(int i = 0; i < nl; i++){
    exl[i] = 5;
    eyl[i] = 5;

  }

  xl[0] = 35;
  yl[0] = 283;
  xl[1] = 100;
  yl[1] = 280;
  xl[2] = 150;
  yl[2] = 279;
  xl[3] = 200;
  yl[3] = 275;
  xl[4] = 250;
  yl[4] = 270;
  xl[5] = 300;
  yl[5] = 255;
  xl[6] = 350;
  yl[6] = 225;
  xl[7] = 400;
  yl[7] = 195;
  xl[8] = 450;
  yl[8] = 175;
  xl[9] = 500;
  yl[9] = 155;
  xl[10] = 550;
  yl[10] = 50;
  
  
  
  TGraphErrors* gr1 = new TGraphErrors(nl,xl,yl,exl,eyl);
  gr1->SetMarkerColor(kWhite);
 

  return gr1;





}


TGraphErrors* getObserved_NLOunc(){

  Int_t nl = 11;
  Double_t xl[11];
  Double_t yl[11];
  Double_t exl[11];
  Double_t eyl[11];
  
  // cout << " n " << hist->GetXaxis()->GetNbins() << endl;
  
  for(int i = 0; i < nl; i++){
    exl[i] = 5;
    eyl[i] = 5;

  }

  xl[0] = 35;
  yl[0] = 272;
  xl[1] = 100;
  yl[1] = 270;
  xl[2] = 150;
  yl[2] = 268;
  xl[3] = 200;
  yl[3] = 265;
  xl[4] = 250;
  yl[4] = 255;
  xl[5] = 300;
  yl[5] = 236;
  xl[6] = 350;
  yl[6] = 198;
  xl[7] = 400;
  yl[7] = 168;
  xl[8] = 450;
  yl[8] = 150;
  xl[9] = 490;
  yl[9] = 135;
  xl[10] = 503;
  yl[10] = 120;
  
  
  
  TGraphErrors* gr1 = new TGraphErrors(nl,xl,yl,exl,eyl);
  gr1->SetMarkerColor(kWhite);
  gr1->SetLineColor(kWhite);
  // gr1->SetMarkerStyle(21);
  
  
  //gr1->Draw("LP");

  TSpline3 *s = new TSpline3("grs",gr1);
  s->SetLineColor(kRed);
  //  s->SetLineStyle(2);
  s->SetLineWidth(3);
  

  return gr1;





}

