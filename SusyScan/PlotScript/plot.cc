#include "plot.h"
#include "SusyScan.h"
#include "GeneratorMasses.h"
#include "IsoMassLine.h"
#include "TheLimits.h"

#include "TRint.h"
#include "TROOT.h"
#include "TObjArray.h"
#include "TStyle.h"

#include "TChain.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TH2F.h"
#include "TTree.h"
#include "TKey.h"
#include "Riostream.h"
#include "TCanvas.h"

#include <string>
#include <cmath>

const double Luminosity = 31.2; //[pb^-1]
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
double ExpExclusion(const SusyScan* p){ return (ExpXsecLimit(p)<Xsection(p)&&ExpXsecLimit(p)>0.01?1:-1); }
double ObsExclusion(const SusyScan* p){ return (ObsXsecLimit(p)<Xsection(p)&&ObsXsecLimit(p)>0.01?1:-1); }
double SoverSqrtB(const SusyScan* p){ return p->signal/(sqrt(p->background)+p->background_uncertainty+p->signal_uncertainty); }
double XsecOverObserved(const SusyScan* p){ return (ObsXsecLimit(p)==0 ? 9999. : Xsection(p)/ObsXsecLimit(p)); }
double XsecOverExpected(const SusyScan* p){ return (ObsXsecLimit(p)==0 ? 9999. : Xsection(p)/ObsXsecLimit(p)); }
double SignalAcceptance(const SusyScan* p){ return  p->signal / (Luminosity*Xsection(p)); }
double NSignExpLimit(const SusyScan* p){ return  p->signal * ExpXsecLimit(p)/Xsection(p); }
  

int plot(int argc, char** argv)
{
   TApplication theApp("App", 0, 0);
   if (gROOT->IsBatch()) {
      fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
      return 1;
   }   

   gStyle->SetHistFillColor(0);
   gStyle->SetPalette(1);
   //gStyle->SetFillColor(0);
   //gStyle->SetOptStat(kFALSE);
   gStyle->SetCanvasColor(0);
   gStyle->SetCanvasBorderMode(0);
   gStyle->SetPadColor(0);
   gStyle->SetPadBorderMode(0);
   gStyle->SetFrameBorderMode(0);

   gStyle->SetTitleFillColor(0);
   gStyle->SetTitleBorderSize(0);
   gStyle->SetTitleX(0.10);
   gStyle->SetTitleY(0.98);
   gStyle->SetTitleW(0.8);
   gStyle->SetTitleH(0.06);

   gStyle->SetErrorX(0);
   gStyle->SetStatColor(0);
   gStyle->SetStatBorderSize(0);
   gStyle->SetStatX(0);
   gStyle->SetStatY(0);
   gStyle->SetStatW(0);
   gStyle->SetStatH(0);

   gStyle->SetTitleFont(22);
   gStyle->SetLabelFont(22,"X");
   gStyle->SetLabelFont(22,"Y");
   gStyle->SetLabelFont(22,"Z");
   gStyle->SetLabelSize(0.03,"X");
   gStyle->SetLabelSize(0.03,"Y");
   gStyle->SetLabelSize(0.03,"Z");
 
   //gROOT->SetStyle("MyStyle");
   TCanvas * c1 = new TCanvas("c1","c1",600,600);
   c1->SetFillStyle   ( 4000 );
   c1->SetLeftMargin  ( 0.15 );
   c1->SetRightMargin ( 0.15 );
   c1->SetBottomMargin( 0.10 );
   c1->cd();
   
   std::vector<GeneratorMasses> genmasses = FillGeneratorMasses("tb10_mu1_a0_massscan.dat");
   TheLimits * doIt = new TheLimits();
   for (int i = 1; i<argc; ++i)
   {
     doIt->add( new SusyScan(argv[i]) );
   }
   doIt->match(genmasses);

   //c1->SetLogz(1);

   IsoMassLine * mline = new IsoMassLine(doIt->GetScan());

   TH2F*h = new TH2F("xsec",";m_{0} [GeV]; m_{1/2} [GeV]; x-section [pb]",
                     100,0,1009.9,50,0,500);
   //doIt->plot(h, Mzero, Mhalf, ObsXsecLimit);
   //doIt->plot(h, Mzero, Mhalf, Xsection);
   //doIt->plot(h, Mzero, Mhalf, XsecOverObserved);
   //doIt->plot(h, Mzero, Mhalf, XsecOverExpected);
   //doIt->plot(h, Mzero, Mhalf, SignalAcceptance);
   //doIt->plot(h, Mzero, Mhalf, NSignExpLimit);
   doIt->plot(h, Mzero, Mhalf, ObsExclusion);
   //h->SetMaximum(27);
   //h->SetMinimum(0.01);
   //c1->SetLogz(1);
   h->GetYaxis()->SetTitleOffset(1.3);
   h->GetZaxis()->SetTitleOffset(1.2);
   h->Draw("colz");
   
   TGraph * gl500 = mline->get(Mzero, Mhalf, MGluino, 500);
   gl500->Draw();

   TGraph * sq500 = mline->get(Mzero, Mhalf, MSquarkL, 500, 10);
   sq500->Draw();

   TH2F*h_qg = new TH2F("AccMGMSQ",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; signal acceptance",
                     60,200,1400,50,200,1200);
   doIt->plot(h_qg, MSquarkL, MGluino, SignalAcceptance);
   doIt->plot(h_qg, MSquarkL, MGluino, NSignExpLimit);
   //h_qg->SetMaximum(1.0);
   h_qg->SetMinimum(0.01);

   //c1->SetLogz(1);
   h_qg->GetYaxis()->SetTitleOffset(1.3);
   //h_qg->Draw("colz");
   
   
   
   //c1->SaveAs("plot.pdf");


   theApp.Run();
}


int main(int argc, char** argv)
{
 // TRint *theApp = new TRint("ROOT example", &argc, argv);
  
  return plot(argc, argv);
}
