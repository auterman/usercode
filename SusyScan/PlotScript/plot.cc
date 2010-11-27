#include "plot.h"
#include "SusyScan.h"
#include "GeneratorMasses.h"
#include "PlotTools.h"
#include "TheLimits.h"
#include "GlobalFunctions.h"

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


int plot(int argc, char** argv)
{
   //interactive root session
   TApplication theApp("App", 0, 0);
   if (gROOT->IsBatch()) {
      fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
      return 1;
   }   

   //Style stuff
   gStyle->SetHistFillColor(0);
   gStyle->SetPalette(1);
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
   
   //Get limits and generator masses ---------------------------------------------------
   TheLimits * genpoints = new TheLimits();
   for (int i = 1; i<argc; ++i)
   {
     genpoints->add( new SusyScan(argv[i]) );
   }
   genpoints->FillGeneratorMasses("tb10_mu1_a0_massscan.dat");
   genpoints->match();


   //the plotting ----------------------------------------------------------------------
   //c1->SetLogz(1);
   PlotTools<SusyScan> * plotTools = new PlotTools<SusyScan>(genpoints->GetScan());
   PlotTools<GeneratorMasses> * plotMasses = new PlotTools<GeneratorMasses>(genpoints->GetGeneratorMasses());

   TH2F*h = new TH2F("xsec",";m_{0} [GeV]; m_{1/2} [GeV]; x-section [pb]",
                     100,0,1009.9,50,0,500);
   //plotTools->Area(h, Mzero, Mhalf, ObsXsecLimit);
   //plotTools->Area(h, Mzero, Mhalf, Xsection);
   //plotTools->Area(h, Mzero, Mhalf, XsecOverObserved);
   //plotTools->Area(h, Mzero, Mhalf, XsecOverExpected);
   //plotTools->Area(h, Mzero, Mhalf, SignalAcceptance);
   //plotTools->Area(h, Mzero, Mhalf, NSignExpLimit);
   //plotTools->Area(h, Mzero, Mhalf, ObsExclusion);
   plotTools->Area(h, Mzero, Mhalf, ExpExclusion);
   //h->SetMaximum(27);
   //h->SetMinimum(0.01);
   //c1->SetLogz(1);
   h->GetYaxis()->SetTitleOffset(1.3);
   h->GetZaxis()->SetTitleOffset(1.2);
   h->Draw("colz");
   
   TGraph * gl500 = plotTools->Line(Mzero, Mhalf, MGluino, 500);
   gl500->Draw();

   TGraph * sq500 = plotTools->Line(Mzero, Mhalf, MSquarkL, 500, 8);
   sq500->SetLineWidth(2);
   sq500->Draw();

   TGraph * sq500_m = plotMasses->Line(Mzero, Mhalf, MSquarkL, 500, 10);
   sq500_m->SetLineWidth(2); sq500_m->SetLineColor(7);
   sq500_m->Draw();
   

   TH2F*h_qg = new TH2F("AccMGMSQ",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; signal acceptance",
                     60,200,1400,50,200,1200);
   plotTools->Area(h_qg, MSquarkL, MGluino, SignalAcceptance);
   plotTools->Area(h_qg, MSquarkL, MGluino, NSignExpLimit);
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
  return plot(argc, argv);
}
