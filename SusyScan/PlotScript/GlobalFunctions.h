#include "SusyScan.h"
#include "GeneratorMasses.h"

#include "TGraph.h"
#include "TLatex.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TSpline.h"
#include "TStyle.h"
#include "TMath.h"

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

TGraph* set_sneutrino_d0_1(Int_t tanBeta){
  double sn_m0[14]= {0,  0, 48, 55, 80, 90,100,105,109,105,100, 72, 55,0};
  double sn_m12[14]={0,140,210,220,237,241,242,241,230,220,210,170,150,0};

  TGraph* sn_d0_gr = new TGraph(14,sn_m0,sn_m12);

  sn_d0_gr->SetFillColor(kGreen+3);
  sn_d0_gr->SetFillStyle(1001);

  return sn_d0_gr;
}

TGraph* set_sneutrino_d0_2(Int_t tanBeta){
  double sn_m0[9]= {0, 45, 75,115,130,150,163,185,0};
  double sn_m12[9]={0,140,170,213,202,183,168,140,0};

  TGraph* sn_d0_gr_2 = new TGraph(9,sn_m0,sn_m12);

  sn_d0_gr_2->SetFillColor(kGreen+3);
  sn_d0_gr_2->SetFillStyle(1001);

  return sn_d0_gr_2;
}


TGraph* set_lep_ch_tanBeta10(){

  double ch_m0[11];
  double ch_m12[11];

  ch_m0[0] = 0;
  ch_m0[1] = 100;
  ch_m0[2] = 200;
  ch_m0[3] = 300;
  ch_m0[4] = 400;
  ch_m0[5] = 500;
  ch_m0[6] = 600;
  ch_m0[7] = 700;
  ch_m0[8] = 800; 
  ch_m0[9] = 800;
  ch_m0[10] = 0;

  ch_m12[0] = 163;
  ch_m12[1] = 162;
  ch_m12[2] = 161;
  ch_m12[3] = 160;
  ch_m12[4] = 159;
  ch_m12[5] = 158;
  ch_m12[6] = 157;
  ch_m12[7] = 156;
  ch_m12[8] = 155.4;
  ch_m12[9] = 0;
  ch_m12[10] = 0;
  
  
  TGraph* ch_gr = new TGraph(11,ch_m0,ch_m12);

  ch_gr->SetFillColor(3);
  ch_gr->SetLineColor(3);
  //  ch_gr->SetLineWidth(3);
  ch_gr->SetFillStyle(1001);

  return ch_gr;

}



TGraph* set_lep_ch_tanBeta3(){

  double ch_m0[17];
  double ch_m12[17];

  ch_m0[0] = 0;
  ch_m0[1] = 100;
  ch_m0[2] = 150;
  ch_m0[3] = 200;
  ch_m0[4] = 250;
  ch_m0[5] = 300;
  ch_m0[6] = 350;
  ch_m0[7] = 400;
  ch_m0[8] = 450;
  ch_m0[9] = 500;
  ch_m0[10] = 550;
  ch_m0[11] = 600;
  ch_m0[12] = 650;
  ch_m0[13] = 700;
  ch_m0[14] = 750;
  ch_m0[15] = 750;
  ch_m0[16] = 0;
  
  ch_m12[0] = 170;
  ch_m12[1] = 168;
  ch_m12[2] = 167;
  ch_m12[3] = 165;
  ch_m12[4] = 163;
  ch_m12[5] = 161;
  ch_m12[6] = 158;
  ch_m12[7] = 156;
  ch_m12[8] = 154;
  ch_m12[9] = 152;
  ch_m12[10] = 150;
  ch_m12[11] = 148;
  ch_m12[12] = 147;
  ch_m12[13] = 145;
  ch_m12[14] = 144;
  ch_m12[15] = 0;
  ch_m12[16] = 0;
  
  TGraph* ch_gr = new TGraph(17,ch_m0,ch_m12);

  ch_gr->SetFillColor(3);
  ch_gr->SetLineColor(3);
  // ch_gr->SetLineWidth(3);
  ch_gr->SetFillStyle(1001);

  return ch_gr;

}


TGraph* set_lep_ch_tanBeta50(){

  double ch_m0[21];
  double ch_m12[21];

  ch_m0[0] = 200;
  ch_m0[1] = 250;
  ch_m0[2] = 300;
  ch_m0[3] = 350;
  ch_m0[4] = 400;
  ch_m0[5] = 450;
  ch_m0[6] = 500;
  ch_m0[7] = 550;
  ch_m0[8] = 600;
  ch_m0[9] = 650;
  ch_m0[10] = 700;
  ch_m0[11] = 750;
  ch_m0[12] = 800;
  ch_m0[13] =850;
  ch_m0[14] = 900;
  ch_m0[15] = 950;
  ch_m0[16] = 1000;
  ch_m0[17] = 1050;
  ch_m0[18] = 1100;
  ch_m0[19] = 1100;
  ch_m0[20] = 200;
 
  ch_m12[0] = 157;
  ch_m12[1] = 156;
  ch_m12[2] = 156;
  ch_m12[3] = 155;
  ch_m12[4] = 155;
  ch_m12[5] = 154;
  ch_m12[6] = 154;
  ch_m12[7] = 153;
  ch_m12[8] = 153;
  ch_m12[9] = 152;
  ch_m12[10] = 152;
  ch_m12[11] = 152;
  ch_m12[12] = 152;
  ch_m12[13] = 152;
  ch_m12[14] = 152;
  ch_m12[15] = 153;
  ch_m12[16] = 153;
  ch_m12[17] = 153;
  ch_m12[18] = 154;
  ch_m12[19] = 0;
  ch_m12[20] = 0;
  
  
  TGraph* ch_gr = new TGraph(21,ch_m0,ch_m12);

  ch_gr->SetFillColor(3);
  ch_gr->SetLineColor(3);
  ch_gr->SetFillStyle(1001);

  return ch_gr;

}


TGraph* set_lep_ch(Int_t tanBeta){
  if(tanBeta == 3) return set_lep_ch_tanBeta3();
  if(tanBeta == 10) return set_lep_ch_tanBeta10();
  if(tanBeta == 50) return set_lep_ch_tanBeta50();
}


TGraph* set_lep_sl(Int_t tanBeta){

  // CMS SUSY Summer2010 implementation
  //  double sl_m0[] =  {0,  0, 30, 50, 60, 75, 80,90,100};
  //  double sl_m12[] = {0,245,240,220,200,150,100,50,0}; 
  
  //contour from D0 trilepton paper (PLB 680 (2009) 34-43)

  double *sl_m0 = 0;
  double *sl_m12 = 0;
  int n = 0;

  double sl_m0_3[] ={0,  0, 10, 20, 30, 40, 50, 60, 70, 77,88,95};
  double sl_m12_3[]={0,245,242,239,232,222,209,189,165,140,60,0};
  int n_3 = 12;

  double sl_m0_10[]={ 0,  0, 11, 20, 24, 49, 70, 82,88,90};
  double sl_m12_10[]={0,240,237,233,230,200,150,100,50,0};
  int n_10 = 10;

  if (tanBeta==3){
    sl_m0 = sl_m0_3;
    sl_m12 = sl_m12_3;
    n = n_3;
  }
  //CMS PTDR-II
  //* Selectron_R line mass=99, ISASUGRA7.69, A0=0, m_top=175, tan(beta]=10
  if (tanBeta==10 || tanBeta==50){
    sl_m0 = sl_m0_10;
    sl_m12 = sl_m12_10;
    n = n_10;
  }

  TGraph* lep_sl = new TGraph(n,sl_m0,sl_m12);

  lep_sl->SetFillColor(5);
  lep_sl->SetLineColor(5);
  lep_sl->SetFillStyle(1001);
  
  return lep_sl;
}


TGraph* set_tev_sg_cdf(Int_t tanBeta){

  //  double sg_m0[] =  {0,  0, 20, 50,100,150,200,250,300,350,400,450,500,550,600,600};
  //  double sg_m12[] = {0,160,169,170,160,155,150,122,116,112,110,106,105,100, 98,  0};
  //  int np=16;
  //New CHF from CDF plot in ICHEP2010 talk (E. Halkiadakis)
  double sg_m0[]= {0,  0, 30, 75,150,185,225,310,360,400,430,500,600,600};
  double sg_m12[]={0,162,168,170,160,150,130,120,109,108,100, 96, 95,  0};
  int np=14;

  TGraph* sg_gr = new TGraph(np,sg_m0,sg_m12);

  //  gStyle->SetHatchesLineWidth(3);

  sg_gr->SetFillColor(2);
  sg_gr->SetLineColor(2);
  //  sg_gr->SetLineWidth(3);
  sg_gr->SetFillStyle(1001); 

  return sg_gr;

}

TGraph* set_tev_sg_d0(Int_t tanBeta){

  //  double sgd_m0[] = {0, 0,  50, 100,150,200,250,300,350,400,450,500,550,600,600};
  //  double sgd_m12[] = {0,168,167,162,157,145,125,120,110,108,95, 94 ,94 ,93,0};
  //  int np=15;
  double sgd_m0[]= {0,  0, 30, 80,150,240,320,400,500,600,600,0};
  double sgd_m12[]={0,167,166,162,156,138,121,109,105,105,  0,0};
  int npd=12;

  TGraph* sgd_gr = new TGraph(npd,sgd_m0,sgd_m12);

  gStyle->SetHatchesLineWidth(3);

  sgd_gr->SetFillColor(kMagenta+3);
  sgd_gr->SetLineColor(kMagenta+3);
  sgd_gr->SetLineWidth(3);
  sgd_gr->SetFillStyle(3335);

  return sgd_gr;

}

// TGraph* set_tev_tlp_cdf(Int_t tanBeta){
//   double tlp1_m0[] = {   0, 20, 40, 60, 70, 80, 90, 80, 70, 60};
//   double tlp1_m12[] = {170,185,200,215,220,215,210,190,175,160};
//   TGraph* tlp1_gr = new TGraph(10,tlp1_m0,tlp1_m12);

//   tlp1_gr->SetFillColor(4);
//   tlp1_gr->SetLineColor(4);
//   tlp1_gr->SetFillStyle(1001);

//   return tlp1_gr;
// }

// TGraph* set_tev_tlp_d0(Int_t tanBeta){
//   double tlp2_m0[] = {  70, 80, 90,100,105,110,120,130,140};
//   double tlp2_m12[] = {160,172,184,196,205,195,185,173,160};
//   TGraph* tlp2_gr = new TGraph(9,tlp2_m0,tlp2_m12);

//   tlp2_gr->SetFillColor(4);
//   tlp2_gr->SetFillStyle(1001); 

//   return tlp2_gr;

// }





TGraph* set_tev_stau(Int_t tanBeta){

    double st_m0_tanBeta3[] = {0,10,20,30,40,50,60,70,80,90,100,0};
    double st_m12_tanBeta3[] = {337,341,356,378,406,439,473,510,548,587,626,626};   

    double st_m0_tanBeta10[] = {0,10,20,30,40,50,60,70,80,90,100,0};
    double st_m12_tanBeta10[] = {213,220,240,275,312,351,393,435,476,518,559,559};

    double st_m0_tanBeta50[] = {200,210,220,230,240,250,260,270,280,290,310,325,200,200};
    double st_m12_tanBeta50[] = {206,226,246,267,288,310,332,354,376,399,450,500,500,206};


    TGraph* st_gr_tanBeta3 = new TGraph(12,st_m0_tanBeta3,st_m12_tanBeta3);
    TGraph* st_gr_tanBeta10 = new TGraph(12,st_m0_tanBeta10,st_m12_tanBeta10);
    TGraph* st_gr_tanBeta50 = new TGraph(14,st_m0_tanBeta50,st_m12_tanBeta50);

    st_gr_tanBeta3->SetFillColor(40);
    st_gr_tanBeta3->SetFillStyle(1001);

    st_gr_tanBeta50->SetFillColor(40);
    st_gr_tanBeta50->SetFillStyle(1001);
    
    st_gr_tanBeta10->SetFillColor(40);
    st_gr_tanBeta10->SetFillStyle(1001);


    if(tanBeta == 3)return st_gr_tanBeta3;
    if(tanBeta == 10)return st_gr_tanBeta10;
    if(tanBeta == 50)return st_gr_tanBeta50;

}




TF1* constant_squark(int tanBeta,int i){
//---lines of constant gluino/squark
  double coef1 = 0.35;
  double coef2[] = {5,5,4.6,4.1};

  char hname[200];

  sprintf(hname,"lnsq_%i",i); 

  
  TF1* lnsq = new TF1(hname,"sqrt([0]-x*x*[1]-[2])",0,1000);
  lnsq->SetParameter(0,(500+150*(i-1))*(500+150*(i-1))/coef2[i]);
  lnsq->SetParameter(1,1./coef2[i]);
  lnsq->SetParameter(2,-coef1*91*91*(2*TMath::Cos(TMath::ATan(tanBeta)))/coef2[i]);//--tanbeta=10 --> cos2beta = -99/101
  lnsq->SetLineWidth(1);


  lnsq->SetLineColor(kGray);

  return lnsq;
}


TF1* constant_gluino(int tanBeta,int i){
//---lines of constant gluino/squark
  double coef1 = 0.35;
  double coef2[] = {5,5,4.6,4.1};

  char hname[200];

  sprintf(hname,"lngl_%i",i); 
    
  TF1* lngl = new TF1(hname,"[0]+x*[1]",0,1000);
  lngl->SetParameter(0,(500+150.*(i-1))/2.4);
  lngl->SetParameter(1,-40./1400);
  lngl->SetLineWidth(1);
  lngl->SetLineColor(kGray);

  return lngl;
}


TLatex* constant_squark_text(Int_t it,TF1& lnsq,Int_t tanBeta_){
  char legnm[200];

  sprintf(legnm,"#font[92]{#tilde{q}(%i)GeV}",500+150*(it-1));
  Double_t place_x = 170;
  if(tanBeta_ == 50)place_x = 290;
  TLatex* t3 = new TLatex(place_x+10*(it-1),lnsq.Eval(place_x+10*(it-1))+5,legnm);
  t3->SetTextSize(0.03);
  t3->SetTextAngle(-8);
  t3->SetTextColor(kGray+2);


  
  return t3;
}

TLatex* constant_gluino_text(Int_t it,TF1& lngl){
  char legnm[200];

  sprintf(legnm,"#font[12]{#tilde{g}}#font[92]{(%i)GeV}",500+150*(it-1));
  TLatex* t4 = new TLatex(423,18+lngl.Eval(480),legnm);
  t4->SetTextSize(0.03);
  t4->SetTextAlign(13);
  t4->SetTextColor(kGray+2);

  return t4;
}


TGraphErrors* getLO_tanBeta3(){



  Int_t nl = 9;
  Double_t xl[9];
  Double_t yl[9];
  Double_t exl[9];
  Double_t eyl[9];
  
  // cout << " n " << hist->GetXaxis()->GetNbins() << endl;
  
 
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
  xl[7] = 450;
  yl[7] = 120;
  xl[8] = 520;
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

TGraphErrors* getLO_tanBeta10(){



  Int_t nl = 10;
  Double_t xl[10];
  Double_t yl[10];
  Double_t exl[10];
  Double_t eyl[10];
  
  // cout << " n " << hist->GetXaxis()->GetNbins() << endl;
  
  xl[0] = 0;
  yl[0] = 270;
  xl[1] = 100;
  yl[1] = 260;
  xl[2] = 200;
  yl[2] = 250;
  xl[3] = 250;
  yl[3] = 240;
  xl[4] = 300;
  yl[4] = 210;
  xl[5] = 350;
  yl[5] = 174;
  xl[6] = 400;
  yl[6] = 147;
  xl[7] = 450;
  yl[7] = 127;
  xl[8] = 500;
  yl[8] =115;
  xl[9] = 520;
  yl[9] = 112;
  
  
  
  
  
  
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

TGraphErrors* getLO_tanBeta50(){



  Int_t nl = 10;
  Double_t xl[10];
  Double_t yl[10];
  Double_t exl[10];
  Double_t eyl[10];
  
  // cout << " n " << hist->GetXaxis()->GetNbins() << endl;
  

  xl[0] = 200;
  yl[0] = 239;
  xl[1] = 210;
  yl[1] = 249;
  xl[2] = 229;
  yl[2] = 260;
  xl[3] = 250;
  yl[3] = 245;
  xl[4] = 300;
  yl[4] = 210;
  xl[5] = 350;
  yl[5] = 180;
  xl[6] = 400;
  yl[6] = 160;
  xl[7] = 450;
  yl[7] = 150;
  xl[8] = 500;
  yl[8] =140;
  xl[9] = 520;
  yl[9] = 137;
  
  
  
  
  
  
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



TGraphErrors* getExpected_NLO_tanBeta3(){

 Int_t nl = 11;
  Double_t xl[11];
  Double_t yl[11];
  Double_t exl[11];
  Double_t eyl[11];
  
    xl[0] = 0;
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
  yl[10] = 150;

 
  
  TGraphErrors* gr1 = new TGraphErrors(nl,xl,yl,exl,eyl);
  gr1->SetMarkerColor(kBlue);
  gr1->SetMarkerStyle(21);
  
  
  //gr1->Draw("LP");

  TSpline3 *s = new TSpline3("grs",gr1);
  s->SetLineColor(kBlue);
  s->SetLineStyle(2);
  s->SetLineWidth(3);
  

  return gr1;





}

TGraphErrors* getExpected_NLO_tanBeta10(){

 Int_t nl = 11;
  Double_t xl[11];
  Double_t yl[11];
  Double_t exl[11];
  Double_t eyl[11];
  
  // cout << " n " << hist->GetXaxis()->GetNbins() << endl;
  
   xl[0] = 0;
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
  yl[9] = 165;
  xl[10] = 550;
  yl[10] = 150;



  
  
  TGraphErrors* gr1 = new TGraphErrors(nl,xl,yl,exl,eyl);
  gr1->SetMarkerColor(kBlue);
  gr1->SetMarkerStyle(21);
  
  
  //gr1->Draw("LP");

  TSpline3 *s = new TSpline3("grs",gr1);
  s->SetLineColor(kBlue);
  s->SetLineStyle(2);
  s->SetLineWidth(3);
  

  return gr1;





}


TGraphErrors* getExpected_NLO_tanBeta50(){

 Int_t nl = 10;
  Double_t xl[10];
  Double_t yl[10];
  Double_t exl[10];
  Double_t eyl[10];
  
  // cout << " n " << hist->GetXaxis()->GetNbins() << endl;
  
   xl[0] = 200;
  yl[0] = 287;
  xl[1] = 220;
  yl[1] = 287;
  xl[2] = 245;
  yl[2] = 287;
  xl[3] = 270;
  yl[3] = 265;
  xl[4] = 300;
  yl[4] = 245;
  xl[5] = 350;
  yl[5] = 222;
  xl[6] = 400;
  yl[6] = 197;
  xl[7] = 450;
  yl[7] = 180;
  xl[8] = 500;
  yl[8] = 168;
  xl[9] = 550;
  yl[9] = 145;



  
  
  TGraphErrors* gr1 = new TGraphErrors(nl,xl,yl,exl,eyl);
  gr1->SetMarkerColor(kBlue);
  gr1->SetMarkerStyle(21);
  
  
  //gr1->Draw("LP");

  TSpline3 *s = new TSpline3("grs",gr1);
  s->SetLineColor(kBlue);
  s->SetLineStyle(2);
  s->SetLineWidth(3);
  

  return gr1;





}


TGraphErrors* getObserved_NLO_tanBeta3(){

 Int_t nl = 11;
  Double_t xl[11];
  Double_t yl[11];
  Double_t exl[11];
  Double_t eyl[11];
  
  // cout << " n " << hist->GetXaxis()->GetNbins() << endl;
  

  
  xl[0] = 0;
  yl[0] = 274;
  xl[1] = 100;
  yl[1] = 270;
  xl[2] = 150;
  yl[2] = 268;
  xl[3] = 200;
  yl[3] = 265;
  xl[4] = 250;
  yl[4] = 255;
  xl[5] = 300;
  yl[5] = 230;
  xl[6] = 350;
  yl[6] = 195;
  xl[7] = 400;
  yl[7] = 160;
  xl[8] = 450;
  yl[8] = 140;
  xl[9] = 480;
  yl[9] = 130;
  xl[10] = 530;
  yl[10] = 120;
 
  
  
  
  TGraphErrors* gr1 = new TGraphErrors(nl,xl,yl,exl,eyl);
  gr1->SetMarkerColor(kBlue);
  gr1->SetMarkerStyle(21);
  
  
  //gr1->Draw("LP");

  TSpline3 *s = new TSpline3("grs",gr1);
  s->SetLineColor(kRed);
  // s->SetLineStyle(2);
  s->SetLineWidth(3);
  

  return gr1;





}



TGraphErrors* getObserved_NLO_tanBeta10(){

 Int_t nl = 11;
  Double_t xl[11];
  Double_t yl[11];
  Double_t exl[11];
  Double_t eyl[11];
  
  // cout << " n " << hist->GetXaxis()->GetNbins() << endl;
  
   xl[0] = 0;
  yl[0] = 278;
  xl[1] = 100;
  yl[1] = 270;
  xl[2] = 150;
  yl[2] = 267;
  xl[3] = 200;
  yl[3] = 262;
  xl[4] = 250;
  yl[4] = 250;
  xl[5] = 300;
  yl[5] = 225;
  xl[6] = 350;
  yl[6] = 192;
  xl[7] = 400;
  yl[7] = 163;
  xl[8] = 450;
  yl[8] = 148;
  xl[9] = 500;
  yl[9] = 140;
  xl[10] = 520;
  yl[10] = 137;
  
 
 
  
  
  TGraphErrors* gr1 = new TGraphErrors(nl,xl,yl,exl,eyl);
  gr1->SetMarkerColor(kBlue);
  gr1->SetMarkerStyle(21);
  
  
  //gr1->Draw("LP");

  TSpline3 *s = new TSpline3("grs",gr1);
  s->SetLineColor(kRed);
  //  s->SetLineStyle(2);
  s->SetLineWidth(3);
  

  return gr1;





}


TGraphErrors* getObserved_NLO_tanBeta50(){

 Int_t nl = 10;
  Double_t xl[10];
  Double_t yl[10];
  Double_t exl[10];
  Double_t eyl[10];
  
  // cout << " n " << hist->GetXaxis()->GetNbins() << endl;
  

  xl[0] = 200;
  yl[0] = 243;
  xl[1] = 220;
  yl[1] = 264;
  xl[2] = 235;
  yl[2] = 278;
  xl[3] = 250;
  yl[3] = 267;
  xl[4] = 300;
  yl[4] = 230;
  xl[5] = 350;
  yl[5] = 205;
  xl[6] = 400;
  yl[6] = 184;
  xl[7] = 450;
  yl[7] = 168;
  xl[8] = 500;
  yl[8] = 156;
  xl[9] = 520;
  yl[9] = 148;
  
 
 
  
  
  TGraphErrors* gr1 = new TGraphErrors(nl,xl,yl,exl,eyl);
  gr1->SetMarkerColor(kBlue);
  gr1->SetMarkerStyle(21);
  
  
  //gr1->Draw("LP");

  TSpline3 *s = new TSpline3("grs",gr1);
  s->SetLineColor(kRed);
  //  s->SetLineStyle(2);
  s->SetLineWidth(3);
  

  return gr1;





}

