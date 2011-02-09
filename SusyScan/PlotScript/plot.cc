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
#include "TLegend.h"
#include "TLatex.h"
#include "TMarker.h"

#include <string>
#include <cmath>
#include <stdio.h>

int plot(int argc, char** argv)
{
   //interactive root session
   //TApplication theApp("App", 0, 0);
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
<<<<<<< plot.cc
   //genpoints->Fill(argc, argv); 
   genpoints->Fill("limits_MHT_Fit_20110127/filelist.txt"); 

   TheLimits * genpointsHT = new TheLimits();
   genpointsHT->Fill("limits_HT_Fit_20110127/filelist.txt"); 

   
   //Replace read limits with specific numbers
   //genpoints->OverwriteLimits("ABCD_MHT");
   
=======
   //genpoints->Fill(argc, argv); 
   genpoints->Fill("limits_MHT/filelist.txt"); 

   TheLimits * genpointsHT = new TheLimits();
   genpointsHT->Fill("limits_HT/filelist.txt"); 

   TheLimits * genpointsHT_SC = new TheLimits();
   genpointsHT_SC->Fill("limits_HT_SigCont/filelist.txt"); 

   TheLimits * genpointsMHT_SC = new TheLimits();
   genpointsMHT_SC->Fill("limits_MHT_SigCont/filelist.txt"); 

   
   //Replace read limits with specific numbers
   //genpoints->OverwriteLimits("ABCD_MHT");
   
>>>>>>> 1.12
   genpoints->FillGeneratorMasses("tb10_mu1_a0_massscan.dat");
   genpoints->match();
<<<<<<< plot.cc
   genpointsHT->FillGeneratorMasses("tb10_mu1_a0_massscan.dat");
   genpointsHT->match();
=======
   genpointsHT->FillGeneratorMasses("tb10_mu1_a0_massscan.dat");
   genpointsHT->match();
   genpointsHT_SC->FillGeneratorMasses("tb10_mu1_a0_massscan.dat");
   genpointsHT_SC->match();
<<<<<<< plot.cc
>>>>>>> 1.12
=======
   genpointsMHT_SC->FillGeneratorMasses("tb10_mu1_a0_massscan.dat");
   genpointsMHT_SC->match();
>>>>>>> 1.13

   //the plotting ----------------------------------------------------------------------
   //plotting helper functions
   PlotTools<SusyScan> * plotTools = new PlotTools<SusyScan>(genpoints->GetScan());
<<<<<<< plot.cc
   PlotTools<SusyScan> * plotToolsHT = new PlotTools<SusyScan>(genpointsHT->GetScan());
=======
   PlotTools<SusyScan> * plotToolsHT = new PlotTools<SusyScan>(genpointsHT->GetScan());
   PlotTools<SusyScan> * plotToolsHT_SC = new PlotTools<SusyScan>(genpointsHT_SC->GetScan());
<<<<<<< plot.cc
>>>>>>> 1.12
=======
   PlotTools<SusyScan> * plotToolsMHT_SC = new PlotTools<SusyScan>(genpointsHT_SC->GetScan());
>>>>>>> 1.13
   PlotTools<GeneratorMasses> * plotMasses = new PlotTools<GeneratorMasses>(genpoints->GetGeneratorMasses());

   //iso mass lines
   TGraph * gl500 = plotMasses->Line(Mzero, Mhalf, MGluino, 500);
   TGraph * sq500 = plotMasses->Line(Mzero, Mhalf, MSquarkL, 500, 10);
   sq500->SetLineWidth(2); sq500->SetLineColor(7);

   TGraph * chi100 = plotMasses->Line(Mzero, Mhalf, MChi1, 50, 20);
   TGraph * cha200 = plotMasses->Line(Mzero, Mhalf, MCha1, 200, 20);
   cha200->SetLineColor(2);

   //the histograms
   c1->SetLogz(1);
   //h->SetMaximum(27);
   //h->SetMinimum(0.01);

/**/
   // cross-section in M0 - M1/2
   TH2F*hxsec = new TH2F("xsec",";m_{0} [GeV]; m_{1/2} [GeV]; cross section [pb]",
                     100,0,1009.9,50,0,500);
   plotTools->Area(hxsec, Mzero, Mhalf, Xsection);
   hxsec->SetMinimum(0.01);
   //sq500->Draw();
   //gl500->Draw();
   hxsec->Draw("colz");
   c1->SaveAs("results/Xsection_m0_m12_tb10.pdf");
   std::string wait;
   //std::cin>>wait;

   // Observed Limit in M0 - M1/2
   TH2F*hobslimit = new TH2F("obslimit",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Observed Limit [pb]",
                     100,0,1009.9,50,0,500);
   plotTools->Area(hobslimit, Mzero, Mhalf, ObsXsecLimit);
   hobslimit->SetMinimum(0.01);
   hobslimit->Draw("colz");
<<<<<<< plot.cc
   c1->SaveAs("results/ObsLimit_m0_m12_tb10.pdf");
=======
   c1->SaveAs("results/ObsLimit_m0_m12_tb10.pdf");
   
<<<<<<< plot.cc
   // Signal Contamination in M0 - M1/2
   TH2F*hsigcont = new TH2F("sigcont",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Observed Limit [pb]",
                     100,0,1009.9,50,0,500);
   plotToolsHT_SC->Area(hsigcont, Mzero, Mhalf, SignalContamination);
   //hsigcont->SetMinimum(0.01);
   hsigcont->Draw("colz");
   c1->SaveAs("results/SignalContamination_m0_m12_tb10.pdf");
>>>>>>> 1.12
   
=======
>>>>>>> 1.13
   // Expected Limit in M0 - M1/2
   TH2F*hexplimit = new TH2F("explimit",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Expected Limit [pb]",
                     100,0,1009.9,50,0,500);
   plotTools->Area(hexplimit, Mzero, Mhalf, ExpXsecLimit);
   hexplimit->SetMinimum(0.01);
   hexplimit->Draw("colz");
   c1->SaveAs("results/ExpLimit_m0_m12_tb10.pdf");
   
   // Signal Acceptance in M0 - M1/2   
   TH2F*hsigacc = new TH2F("sigacc",";m_{0} [GeV]; m_{1/2} [GeV]; Signal Acceptance",
                     100,0,1009.9,50,0,500);
   plotTools->Area(hsigacc, Mzero, Mhalf, SignalAcceptance);
   hsigacc->SetMinimum(0.01);
   hsigacc->SetMaximum(1.0);
   hsigacc->Draw("colz");
<<<<<<< plot.cc
   //chi100->Draw();
   //cha200->Draw();
   //gl500 ->Draw();
   // sq500 ->Draw();
   c1->SaveAs("results/SigAccMHT_m0_m12_tb10.pdf");
   
   // Signal Acceptance in M0 - M1/2
   TH2F*hsigaccHT = new TH2F("sigaccHT",";m_{0} [GeV]; m_{1/2} [GeV]; Signal Acceptance",
                     100,0,1009.9,50,0,500);
   plotToolsHT->Area(hsigaccHT, Mzero, Mhalf, SignalAcceptance);
   hsigaccHT->SetMinimum(0.01);
   hsigaccHT->SetMaximum(1.0);
   hsigaccHT->Draw("colz");
   //chi100->Draw();
   //cha200->Draw();
   //gl500 ->Draw();
   // sq500 ->Draw();
   c1->SaveAs("results/SigAccHT_m0_m12_tb10.pdf");
=======
   chi100->Draw();
   cha200->Draw();
   gl500 ->Draw();
    sq500 ->Draw();
   c1->SaveAs("results/SigAcc_m0_m12_tb10.pdf");
>>>>>>> 1.12
   
   // Exp. Limit on Number of Signal Events in M0 - M1/2
   c1->SetLogz(0);
   TH2F*hexplimitnsig = new TH2F("explimitnsig",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL exp. limit signal events [# ]",
                     100,0,1009.9,50,0,500);
   plotTools->Area(hexplimitnsig, Mzero, Mhalf, ExpNSignLimit);
   hexplimitnsig->SetMinimum(0.0);
   hexplimitnsig->SetMaximum(20);
   hexplimitnsig->Draw("colz");
   c1->SaveAs("results/ExpLimitOnNSig_m0_m12_tb10.pdf");
   
   // Obs. Limit on Number of Signal Events in M0 - M1/2
   TH2F*hobslimitnsig = new TH2F("obslimitnsig",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL obs. limit signal events [# ]",
                     100,0,1009.9,50,0,500);
   plotTools->Area(hobslimitnsig, Mzero, Mhalf, ObsNSignLimit);
   hobslimitnsig->SetMinimum(0.0);
   hobslimitnsig->SetMaximum(20);
   hobslimitnsig->Draw("colz");
   c1->SaveAs("results/ObsLimitOnNSig_m0_m12_tb10.pdf");
   
   c1->SetLogz(0);
   // Expected Exclusion in M0 - M1/2
   TH2F*hexpexcl = new TH2F("expexcl",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Expected Exclusion",
                     100,0,1009.9,35,100,450);
   plotTools->Area(hexpexcl, Mzero, Mhalf, ExpExclCL);
   hexpexcl->Draw("colz");
   c1->SaveAs("results/ExpExclusion_m0_m12_tb10.pdf");
   
   // Observed Exclusion in M0 - M1/2
   TH2F*hobsexcl = new TH2F("obsexcl",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Observed Exclusion",
                     100,0,1009.9,35,100,450);
   plotTools->Area(hobsexcl, Mzero, Mhalf, ObsExclCL);
   hobsexcl->Draw("colz");
   c1->SaveAs("results/ObsExclusion_m0_m12_tb10.pdf");

   // Observed Exclusion in M0 - M1/2
   TH2F*hPLobsexcl = new TH2F("plobsexcl",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Observed Exclusion",
                     100,0,1009.9,35,100,450);
   plotTools->Area(hPLobsexcl, Mzero, Mhalf, PLObsExclusion);
   hPLobsexcl->Draw("colz");
   c1->SaveAs("results/PL_ObsExclusion_m0_m12_tb10.pdf");

   // TestContours in M0 - M1/2
<<<<<<< plot.cc
   TH2F*texcl = new TH2F("texcl",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Expected Exclusion",
                     100,0,1009.9,35,100,450);
   TH2F*tPLexpexcl=(TH2F*)texcl->Clone();
   plotTools->Area(tPLexpexcl, Mzero, Mhalf, PLExpExclusion);
   TH2F*tFCexpexcl=(TH2F*)texcl->Clone();
   plotTools->Area(tFCexpexcl, Mzero, Mhalf, FCExpExclusion);
   TH2F*tFCobsexcl=(TH2F*)texcl->Clone();
   plotTools->Area(tFCobsexcl, Mzero, Mhalf, FCObsExclusion);
   std::vector<TGraph*> contours = plotTools->GetContours(hexpexcl,3); 
   //std::vector<TGraph*> contours = plotTools->GetContours(hobsexcl,3);
   //std::vector<TGraph*> contours = plotTools->GetContours(tPLexpexcl,3);
   //std::vector<TGraph*> contours = plotTools->GetContours(hPLobsexcl,3);
=======
   TH2F*texcl = new TH2F("texcl",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Expected Exclusion",
                     100,0,1009.9,35,100,450);
   TH2F*texpexcl=(TH2F*)texcl->Clone();
   plotTools->Area(texpexcl, Mzero, Mhalf, MCMCExpExclusion);
   TH2F*tobsexcl=(TH2F*)texcl->Clone();
   plotTools->Area(tobsexcl, Mzero, Mhalf, MCMCObsExclusion);
   std::vector<TGraph*> contours = plotTools->GetContours(texpexcl,3); 
   //std::vector<TGraph*> contours = plotTools->GetContours(tobsexcl,3);
>>>>>>> 1.12
   //hPLexpexcl
   //hexcl->Draw("colz");
   //hexpexcl->Draw("colz");
   //hobsexcl->Draw("colz");
<<<<<<< plot.cc
   hPLobsexcl->Draw("colz");
   int col=kBlue-10;
=======
   texpexcl->Draw("colz");
   int col=kBlue-10;
>>>>>>> 1.12
   for (std::vector<TGraph*>::iterator cont=contours.begin(); cont!=contours.end(); ++cont){
     if (! *cont) continue;
     double x, y;
     (*cont)->GetPoint(0, x, y);
     (*cont)->SetLineColor(col);
     (*cont)->Draw("l");
     TLatex l; l.SetTextSize(0.04); l.SetTextColor(col++);
     char val[20];
     sprintf(val,"%d",(int)(cont-contours.begin()));
     l.DrawLatex(x,y,val); 
     //if (cont-contours.begin()>3) break;
   }
   c1->SaveAs("results/ExclusionTestContours_m0_m12_tb10.pdf");


   // Exclusion in M0 - M1/2
   TH2F*hexcl = new TH2F("hexcl",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Expected Exclusion",
                     100,0,1009.9,35,100,450);
<<<<<<< plot.cc
<<<<<<< plot.cc
   TGraph * gexpexcl = plotTools->GetContour(hexpexcl,3,0); 
   TGraph * gobsexcl = plotTools->GetContour(hobsexcl,3,0);
   TGraph * gPLobsexcl = plotTools->GetContour(hPLobsexcl,3,0); 
   TH2F*hPLexpexcl=(TH2F*)hexcl->Clone();
   TH2F*hFCexpexcl=(TH2F*)hexcl->Clone();
   TH2F*hFCobsexcl=(TH2F*)hexcl->Clone();
   TH2F*hMCMCexpexcl=(TH2F*)hexcl->Clone();
   TH2F*hMCMCobsexcl=(TH2F*)hexcl->Clone();
   TH2F*hCLsExpHT=(TH2F*)hexcl->Clone();
   TH2F*hCLsObsHT=(TH2F*)hexcl->Clone();
   plotTools->Area(hPLexpexcl, Mzero, Mhalf, PLExpExclusion);
   plotToolsHT->Area(hCLsExpHT, Mzero, Mhalf, ExpExclCL);
   plotToolsHT->Area(hCLsObsHT, Mzero, Mhalf, ObsExclCL);
   TGraph * gPLexpexcl = plotTools->GetContour(hPLexpexcl,3,0); 
   TGraph * gFCexpexcl = plotTools->GetContour(hFCexpexcl,3,0); 
   TGraph * gFCobsexcl = plotTools->GetContour(hFCobsexcl,3,0); 
   TGraph * gMCMCexpexcl = plotTools->GetContour(hMCMCexpexcl,3,0); 
   TGraph * gMCMCobsexcl = plotTools->GetContour(hMCMCobsexcl,3,0); 
   TGraph * gCLsExpExclHT = plotTools->GetContour(hCLsExpHT,3,0); 
   TGraph * gCLsObsExclHT = plotTools->GetContour(hCLsObsHT,3,0); 
=======
   TGraph * gexpexcl = plotTools->GetContour(hexpexcl,3,0); 
   TGraph * gobsexcl = plotTools->GetContour(hobsexcl,3,0);
   TGraph * gPLobsexcl = plotTools->GetContour(hPLobsexcl,3,0); 
   TH2F*hPLexpexcl=(TH2F*)hexcl->Clone();
   TH2F*hFCexpexcl=(TH2F*)hexcl->Clone();
   TH2F*hFCobsexcl=(TH2F*)hexcl->Clone();
   TH2F*hMCMCexpexcl=(TH2F*)hexcl->Clone();
   TH2F*hMCMCobsexcl=(TH2F*)hexcl->Clone();
   TH2F*hCLsExpHT=(TH2F*)hexcl->Clone();
   TH2F*hCLsExpHT_SC=(TH2F*)hexcl->Clone();
   TH2F*hCLsObsHT=(TH2F*)hexcl->Clone();
   TH2F*hCLsExpHTp1sigma=(TH2F*)hexcl->Clone();
   TH2F*hCLsExpHTm1sigma=(TH2F*)hexcl->Clone();
   plotTools->Area(hPLexpexcl, Mzero, Mhalf, PLExpExclusion);
   plotToolsHT_SC->Area(hCLsExpHT_SC, Mzero, Mhalf, ExpExclCL);
   plotToolsHT->Area(hMCMCexpexcl, Mzero, Mhalf, MCMCExpExclusion);
   plotToolsHT->Area(hMCMCobsexcl, Mzero, Mhalf, MCMCObsExclusion);
   plotToolsHT->Area(hCLsExpHT, Mzero, Mhalf, ExpExclCL);
   plotToolsHT->Area(hCLsObsHT, Mzero, Mhalf, ObsExclCL);
   plotToolsHT->Area(hCLsExpHTm1sigma, Mzero, Mhalf, ExpExclCLm1sigma);
   plotToolsHT->Area(hCLsExpHTp1sigma, Mzero, Mhalf, ExpExclCLp1sigma);
   TGraph * gPLexpexcl = plotTools->GetContour(hPLexpexcl,3,0); 
   TGraph * gFCexpexcl = plotTools->GetContour(hFCexpexcl,3,0); 
   TGraph * gFCobsexcl = plotTools->GetContour(hFCobsexcl,3,0); 
   TGraph * gMCMCexpexcl = plotToolsHT->GetContour(hMCMCexpexcl,3,0); 
   TGraph * gMCMCobsexcl = plotToolsHT->GetContour(hMCMCobsexcl,3,0); 
   TGraph * gCLsExpExclHT_SC = plotToolsHT->GetContour(hCLsExpHT_SC,3,0); 
   TGraph * gCLsExpExclHT = plotToolsHT->GetContour(hCLsExpHT,3,0); 
   TGraph * gCLsObsExclHT = plotToolsHT->GetContour(hCLsObsHT,3,0); 
   TGraph * gCLsExpExclHTm1 = plotToolsHT->GetContour(hCLsExpHTm1sigma,3,0); 
   TGraph * gCLsExpExclHTp1 = plotToolsHT->GetContour(hCLsExpHTp1sigma,3,0); 
>>>>>>> 1.12
   hexcl->Draw("colz");
=======
   TGraph * gexpexcl         = plotTools     ->GetContour(hexcl,Mzero,Mhalf,ExpExclCL,       3,0, 2,2); 
   TGraph * gobsexcl         = plotTools     ->GetContour(hexcl,Mzero,Mhalf,ObsExclCL,       3,0, 2,1);
   TGraph * gMCMCexpexcl     = plotToolsHT   ->GetContour(hexcl,Mzero,Mhalf,MCMCExpExclusion,3,0, 3,2); 
   TGraph * gMCMCobsexcl     = plotToolsHT   ->GetContour(hexcl,Mzero,Mhalf,MCMCObsExclusion,3,0, 3,1); 
   TGraph * gCLsExpExclHT_SC = plotToolsHT_SC->GetContour(hexcl,Mzero,Mhalf,ExpExclCL,       3,0, 4,2); 
   TGraph * gCLsExpExclHT    = plotToolsHT   ->GetContour(hexcl,Mzero,Mhalf,ExpExclCL,       3,0, 1,2); 
   TGraph * gCLsObsExclHT    = plotToolsHT   ->GetContour(hexcl,Mzero,Mhalf,ObsExclCL,       3,0, 1,1); 
   TGraph * gCLsExpExclHTm1  = plotToolsHT   ->GetContour(hexcl,Mzero,Mhalf,ExpExclCLm1sigma,3,0, 5,2); 
   TGraph * gCLsExpExclHTp1  = plotToolsHT   ->GetContour(hexcl,Mzero,Mhalf,ExpExclCLp1sigma,3,0, 5,2); 
   TGraph * gCLsExp1Sigma    = MakeBand(gCLsExpExclHTp1, gCLsExpExclHTm1);
   hexcl->Draw("colz");   
   if (gCLsExp1Sigma)    gCLsExp1Sigma->Draw("lf");
   if (gexpexcl)         gexpexcl->Draw("l");
   if (gCLsExpExclHT_SC) gCLsExpExclHT_SC->Draw("l");
   if (gCLsExpExclHT)    gCLsExpExclHT->Draw("l");
   if (gMCMCexpexcl)     gMCMCexpexcl->Draw("l");
   if (gCLsObsExclHT)    gCLsObsExclHT->Draw("l");
   //if (gobsexcl)       gobsexcl->Draw("l");
   //if (gPLobsexcl)     gPLobsexcl->Draw("l");
   //if (gFCobsexcl)     gFCobsexcl->Draw("l");
   //if (gMCMCobsexcl)   gMCMCobsexcl->Draw("l");
   //if (gFCexpexcl)     gFCexpexcl->Draw("l");
   //if (gPLexpexcl)     gPLexpexcl->Draw("l");
   //if (gFCexpexcl)     gFCexpexcl->Draw("l");
>>>>>>> 1.13
   //set old exclusion Limits
   TGraph* LEP_ch = set_lep_ch();
   TGraph* LEP_sl = set_lep_sl();//slepton curve
   TGraph* TEV_sg_cdf = set_tev_sg_cdf();//squark gluino cdf
   TGraph* TEV_sg_d0 = set_tev_sg_d0();//squark gluino d0
   TGraph* TEV_tlp_cdf = set_tev_tlp_cdf();//trilepton cdf
   TGraph* TEV_tlp_d0 = set_tev_tlp_d0();//trilepton d0
   TGraph* stau = set_tev_stau();//stau 
   TGraph* TEV_sn_d0_1 = set_sneutrino_d0_1();
   TGraph* TEV_sn_d0_2 = set_sneutrino_d0_2();
   TGraphErrors* First = getObserved_NLOunc();
   TGraphErrors* Second = getExpected_NLOunc();//getLO_jetMultis();
   TGraphErrors* Third = getLO_signalCont();
   First->GetXaxis()->SetRangeUser(0,505);
   First->GetYaxis()->SetRangeUser(80,500);
   First->GetXaxis()->SetTitle("m_{0} (GeV)");
   First->GetYaxis()->SetTitle("m_{1/2} (GeV)");
   TSpline3 *sFirst = new TSpline3("sFirst",First);
   sFirst->SetLineColor(kBlue);
   sFirst->SetLineWidth(3);
   TSpline3 *sSecond = new TSpline3("sSecond",Second);
   sSecond->SetLineColor(kBlue);
   sSecond->SetLineStyle(2);
   sSecond->SetLineWidth(3);
   TSpline3 *sThird = new TSpline3("sThird",Third);
   sThird->SetLineColor(kGreen+2);
   sThird->SetLineStyle(4);
   sThird->SetLineWidth(3);
   TEV_sn_d0_1->Draw("fsame");
   TEV_sn_d0_2->Draw("fsame");
   TEV_sg_d0->Draw("fsame");
   TEV_sg_cdf->Draw("fsame");
   LEP_ch->Draw("fsame");
   LEP_sl->Draw("fsame");
   stau->Draw("fsame");
   sFirst->Draw("same");
   sSecond->Draw("same");
   TLatex b; b.SetTextSize(0.02); b.SetTextColor(1);
   b.DrawLatex( 10,420,"#tilde{#tau} LSP"); 
   b.DrawLatex(  6,150,"LEP2"); 
   b.DrawLatex( 22,138,"#tilde{l}^{#pm}"); 
   b.DrawLatex(360,110,"LEP2 #tilde{#chi}^{#pm}"); 
   b.DrawLatex(100,140,"CDF"); 
   b.DrawLatex(248,129,"D0 #tilde{g}, #tilde{q}"); 
   b.DrawLatex( 80,180,"D0 #tilde{#nu}"); 
<<<<<<< plot.cc
   
   if (gexpexcl) gexpexcl->SetLineStyle(2);
<<<<<<< plot.cc
   if (gPLobsexcl) gPLobsexcl->SetLineColor(2);
   if (gPLexpexcl) {gPLexpexcl->SetLineStyle(2);gPLexpexcl->SetLineColor(2);}
   if (gFCobsexcl) gFCobsexcl->SetLineColor(3);
   if (gFCexpexcl) {gFCexpexcl->SetLineStyle(2);gFCexpexcl->SetLineColor(3);}
   if (gMCMCobsexcl) gMCMCobsexcl->SetLineColor(4);
   if (gMCMCexpexcl) {gMCMCexpexcl->SetLineStyle(2);gMCMCexpexcl->SetLineColor(4);}
   if (gCLsObsExclHT) gCLsObsExclHT->SetLineColor(2);
   if (gCLsExpExclHT) {gCLsExpExclHT->SetLineStyle(2);gCLsExpExclHT->SetLineColor(2);}
   if (gobsexcl) gobsexcl->Draw("l");
   if (gCLsObsExclHT) gCLsObsExclHT->Draw("l");
   //if (gPLobsexcl) gPLobsexcl->Draw("l");
   //if (gFCobsexcl) gFCobsexcl->Draw("l");
   //if (gMCMCobsexcl) gMCMCobsexcl->Draw("l");
   //if (gFCexpexcl) gFCexpexcl->Draw("l");
   //if (gPLexpexcl) gPLexpexcl->Draw("l");
   //if (gMCMCexpexcl) gMCMCexpexcl->Draw("l");
   if (gexpexcl) gexpexcl->Draw("l");
   //if (gFCexpexcl) gFCexpexcl->Draw("l");
   if (gCLsExpExclHT) gCLsExpExclHT->Draw("l");
   TLegend * leg = new TLegend(0.45,0.7,0.85,0.89);
=======
   if (gexpexcl) gexpexcl->Draw("l");

   if (gCLsExpExclHT_SC) {gCLsExpExclHT_SC->SetLineStyle(2);gCLsExpExclHT_SC->SetLineColor(7);}
   if (gCLsExpExclHT_SC) gCLsExpExclHT_SC->Draw("l");

   if (gCLsExpExclHT) {gCLsExpExclHT->SetLineStyle(2);gCLsExpExclHT->SetLineWidth(2);gCLsExpExclHT->SetLineColor(2);}
   if (gCLsExpExclHT)  gCLsExpExclHT->Draw("l");
   if (gCLsExpExclHTm1) {gCLsExpExclHTm1->SetLineColor(kYellow); }
   if (gCLsExpExclHTm1) gCLsExpExclHTm1->Draw("l");
   if (gCLsExpExclHTp1) {gCLsExpExclHTp1->SetLineColor(kYellow); }
   if (gCLsExpExclHTp1) gCLsExpExclHTp1->Draw("l");

   if (gMCMCexpexcl) {gMCMCexpexcl->SetLineStyle(2);gMCMCexpexcl->SetLineColor(6);}
   if (gMCMCexpexcl)  gMCMCexpexcl->Draw("l");

   //if (gPLobsexcl) gPLobsexcl->SetLineColor(2);
   //if (gPLexpexcl) {gPLexpexcl->SetLineStyle(2);gPLexpexcl->SetLineColor(2);}
   if (gFCobsexcl) gFCobsexcl->SetLineColor(3);
   if (gFCexpexcl) {gFCexpexcl->SetLineStyle(2);gFCexpexcl->SetLineColor(3);}
   if (gMCMCobsexcl) gMCMCobsexcl->SetLineColor(5);

   if (gCLsObsExclHT) gCLsObsExclHT->SetLineColor(2); 
   if (gCLsObsExclHT) gCLsObsExclHT->Draw("l");

   //if (gobsexcl) gobsexcl->Draw("l");
   //if (gPLobsexcl) gPLobsexcl->Draw("l");
   //if (gFCobsexcl) gFCobsexcl->Draw("l");
   //if (gMCMCobsexcl) gMCMCobsexcl->Draw("l");
   //if (gFCexpexcl) gFCexpexcl->Draw("l");
   //if (gPLexpexcl) gPLexpexcl->Draw("l");
   //if (gFCexpexcl) gFCexpexcl->Draw("l");

=======
>>>>>>> 1.13
   TLegend * leg = new TLegend(0.45,0.7,0.85,0.89);
>>>>>>> 1.12
   leg->SetBorderSize(0);leg->SetFillColor(0);
<<<<<<< plot.cc
   if (gobsexcl) leg->AddEntry(gobsexcl,"Observed (MHT, CLs, TLimit)","l");
   if (gexpexcl) leg->AddEntry(gexpexcl,"Expected (MHT, CLs, TLimit)","l");
   if (gCLsObsExclHT) leg->AddEntry(gCLsObsExclHT,"Observed (HT, CLs, TLimit)","l");
   if (gCLsExpExclHT) leg->AddEntry(gCLsExpExclHT,"Expected (HT, CLs, TLimit)","l");
   //if (gPLobsexcl) leg->AddEntry(gPLobsexcl,"Observed (PL, RooStat)","l");
   //if (gPLexpexcl) leg->AddEntry(gPLexpexcl,"Expected (PL, RooStat)","l");
   //if (gFCobsexcl) leg->AddEntry(gFCobsexcl,"Observed (FC, RooStat)","l");
   //if (gFCexpexcl) leg->AddEntry(gFCexpexcl,"Expected (FC, RooStat)","l");
   //if (gMCMCobsexcl) leg->AddEntry(gMCMCobsexcl,"Observed (MCMC, RooStat)","l");
   //if (gMCMCexpexcl) leg->AddEntry(gMCMCexpexcl,"Expected (MCMC, RooStat)","l");
   if (sFirst)  leg->AddEntry(sFirst, "RA1 NLO Observed");
   if (sSecond) leg->AddEntry(sSecond,"RA1 NLO Expected");
=======
   //if (gobsexcl) leg->AddEntry(gobsexcl,"Observed (MHT, CLs, TLimit)","l");
   if (gexpexcl) leg->AddEntry(gexpexcl,"LO Expected (MHT, CLs, TLimit)","l");
   if (gCLsObsExclHT) leg->AddEntry(gCLsObsExclHT,"LO Observed (HT, CLs, TLimit)","l");
   if (gCLsExpExclHT) leg->AddEntry(gCLsExpExclHT,"LO Expected (HT, CLs, TLimit)","l");
   if (gCLsExpExclHT_SC) leg->AddEntry(gCLsExpExclHT_SC,"LO Expected (HT, CLs, sig cont)","l");
   //if (gPLobsexcl) leg->AddEntry(gPLobsexcl,"Observed (PL, RooStat)","l");
   //if (gPLexpexcl) leg->AddEntry(gPLexpexcl,"Expected (PL, RooStat)","l");
   //if (gFCobsexcl) leg->AddEntry(gFCobsexcl,"Observed (FC, RooStat)","l");
   //if (gFCexpexcl) leg->AddEntry(gFCexpexcl,"Expected (FC, RooStat)","l");
   //if (gMCMCobsexcl) leg->AddEntry(gMCMCobsexcl,"Observed (MHT, MCMC, RooStat)","l");
   if (gMCMCexpexcl) leg->AddEntry(gMCMCexpexcl,"LO Expected (HT, MCMC, RooStat)","l");
   if (sFirst)  leg->AddEntry(sFirst, "RA1 NLO Observed");
   if (sSecond) leg->AddEntry(sSecond,"RA1 NLO Expected");
>>>>>>> 1.12
   leg->Draw();
<<<<<<< plot.cc
   gPad->RedrawAxis();
   c1->SaveAs("results/Exclusion_m0_m12_tb10.pdf");
=======
   gPad->RedrawAxis();
<<<<<<< plot.cc
   c1->SaveAs("results/Exclusion_m0_m12_tb10.png");
>>>>>>> 1.12
=======
   c1->SaveAs("results/Exclusion_m0_m12_tb10.pdf");
>>>>>>> 1.13

   // Signal Contamination in M0 - M1/2
   c1->SetLogz(0);
   TH2F*hsigcont = new TH2F("sigcont",";m_{0} [GeV]; m_{1/2}; number of signal in bkg yield [events]",
                     100,0,1009.9,50,0,500);
   plotToolsHT_SC->Area(hsigcont, Mzero, Mhalf, SignalContamination);
   hsigcont->SetMinimum(0.01);
   hsigcont->SetMaximum(20);
   hsigcont->Draw("colz");
   if (gCLsObsExclHT)    gCLsObsExclHT->Draw("l");
   c1->SaveAs("results/SignalContamination_HT_m0_m12_tb10.pdf");
   
   // Signal Contamination in M0 - M1/2
   c1->SetLogz(0);
   TH2F*hsigcontMHT = new TH2F("sigcontMHT",";m_{0} [GeV]; m_{1/2}; number of signal in bkg yield [events]",
                     100,0,1009.9,50,0,500);
   plotToolsMHT_SC->Area(hsigcontMHT, Mzero, Mhalf, SignalContamination);
   hsigcontMHT->SetMinimum(0.01);
   hsigcontMHT->SetMaximum(20);
   hsigcontMHT->Draw("colz");
   if (gobsexcl)    gobsexcl->Draw("l");
   c1->SaveAs("results/SignalContamination_MHT_m0_m12_tb10.pdf");
   


   //-----------------------------------------------------------------------------------
   c1->SetLogz(1);

   TGraph * mz500 = plotMasses->Line( MSquarkL, MGluino, Mzero, 500, 1);
   TGraph * mh250 = plotMasses->Line( MSquarkL, MGluino, Mhalf, 250, 1);

   // cross-section in squark - gluino mass
   TH2F*hxsec_qg = new TH2F("xsec_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; cross section [pb]",
                     60,200,1400,50,200,1200);
   plotTools->Area(hxsec_qg, MSquarkL, MGluino, Xsection);
   hxsec_qg->SetMinimum(0.01);
   hxsec_qg->Draw("colz");
   mz500->Draw();
   mh250->Draw();
   c1->SaveAs("results/Xsection_mSql_mGl_tb10.pdf");
   
   // Observed Limit in squark - gluino mass
   TH2F*hobslimit_qg = new TH2F("obslimit_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Observed Limit [pb]",
                     60,200,1400,50,200,1200);
   plotTools->Area(hobslimit_qg, MSquarkL, MGluino, ObsXsecLimit);
   hobslimit_qg->SetMinimum(0.01);
   hobslimit_qg->Draw("colz");
   c1->SaveAs("results/ObsLimit_mSql_mGl_tb10.pdf");
   
   // Expected Limit in squark - gluino mass
   TH2F*hexplimit_qg = new TH2F("explimit_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Expected Limit [pb]",
                     60,200,1400,50,200,1200);
   plotTools->Area(hexplimit_qg, MSquarkL, MGluino, ExpXsecLimit);
   hexplimit_qg->SetMinimum(0.01);
   hexplimit_qg->Draw("colz");
   c1->SaveAs("results/ExpLimit_mSql_mGl_tb10.pdf");
   
   // Signal Acceptance in squark - gluino mass
   TH2F*hsigacc_qg = new TH2F("sigacc_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; Signal Acceptance",
                     60,200,1400,50,200,1200);
   plotTools->Area(hsigacc_qg, MSquarkL, MGluino, SignalAcceptance);
   hsigacc_qg->SetMinimum(0.01);
   hsigacc_qg->SetMaximum(1.0);
   hsigacc_qg->Draw("colz");
<<<<<<< plot.cc
   c1->SaveAs("results/SigAccMHT_mSql_mGl_tb10.pdf");
   
   // Signal Acceptance in squark - gluino mass
   TH2F*hsigaccHT_qg = new TH2F("sigaccHT_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; Signal Acceptance",
                     60,200,1400,50,200,1200);
   plotToolsHT->Area(hsigaccHT_qg, MSquarkL, MGluino, SignalAcceptance);
   hsigaccHT_qg->SetMinimum(0.01);
   hsigaccHT_qg->SetMaximum(1.0);
   hsigaccHT_qg->Draw("colz");
   c1->SaveAs("results/SigAccHT_mSql_mGl_tb10.pdf");
=======
   c1->SaveAs("results/SigAcc_mSql_mGl_tb10.pdf");
>>>>>>> 1.12
   
   // Exp. Limit on Number of Signal Events in squark - gluino mass
   c1->SetLogz(0);
   TH2F*hexplimitnsig_qg = new TH2F("explimitnsig_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL exp. limit signal events [# ]",
                     60,200,1400,50,200,1200);
   plotTools->Area(hexplimitnsig_qg, MSquarkL, MGluino, ExpNSignLimit);
   hexplimitnsig_qg->SetMinimum(0.0);
   hexplimitnsig_qg->SetMaximum(20);
   hexplimitnsig_qg->Draw("colz");
   c1->SaveAs("results/ExpLimitOnNSig_mSql_mGl_tb10.pdf");
   
   // Obs. Limit on Number of Signal Events in squark - gluino mass
   TH2F*hobslimitnsig_qg = new TH2F("obslimitnsig_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL obs. limit signal events [# ]",
                     60,200,1400,50,200,1200);
   plotTools->Area(hobslimitnsig_qg, MSquarkL, MGluino, ObsNSignLimit);
   hobslimitnsig_qg->SetMinimum(0.0);
   hobslimitnsig_qg->SetMaximum(20);
   hobslimitnsig_qg->Draw("colz");
   c1->SaveAs("results/ObsLimitOnNSig_mSql_mGl_tb10.pdf");
   
   c1->SetLogz(0);
   // Expected Exclusion in squark - gluino mass
   TH2F*hexpexcl_qg = new TH2F("expexcl_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Expected Exclusion",
                     60,200,1400,50,200,1200);
   plotTools->Area(hexpexcl_qg, MSquarkL, MGluino, ExpExclCL);
   hexpexcl_qg->Draw("colz");
   c1->SaveAs("results/ExpExclusion_mSql_mGl_tb10.pdf");
   
   // Observed Exclusion in squark - gluino mass
   TH2F*hobsexcl_qg = new TH2F("obsexcl_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Observed Exclusion",
                     60,200,1400,50,200,1200);
   plotTools->Area(hobsexcl_qg, MSquarkL, MGluino, ObsExclCL);
   //std::vector<TGraph *> vobsexcl_qg = plotTools->GetContours(hobsexcl_qg);
   hobsexcl_qg->Draw("colz");
   //for (std::vector<TGraph *>::iterator g=vobsexcl_qg.begin();g!=vobsexcl_qg.end();++g){
   //  if (! *g) continue;
   // // (*g)->Draw("l");
   //}
   c1->SaveAs("results/ObsExclusion_mSql_mGl_tb10.pdf");
   
   // TestContours in M0 - M1/2
   TH2F*texcl_qg = new TH2F("texcl_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Observed Exclusion",
                     60,200,1400,50,200,1200);
   TH2F*tPLobsexcl_qg=(TH2F*)texcl_qg->Clone();
   plotTools->Area(tPLobsexcl_qg, MSquarkL, MGluino, PLObsExclusion);
   TH2F*tPLexpexcl_qg=(TH2F*)texcl_qg->Clone();
   plotTools->Area(tPLexpexcl_qg, MSquarkL, MGluino, PLExpExclusion);
   //std::vector<TGraph*> contours_qg = plotTools->GetContours(hexpexcl_qg,3); 
   //std::vector<TGraph*> contours_qg = plotTools->GetContours(hobsexcl_qg,3);
   std::vector<TGraph*> contours_qg = plotTools->GetContours(tPLobsexcl_qg,3);
   //std::vector<TGraph*> contours_qg = plotTools->GetContours(tPLexpexcl_qg,3);
   //hPLexpexcl_qg
   //hexcl_qg->Draw("colz");
   //hexpexcl_qg->Draw("colz");
   //hobsexcl_qg->Draw("colz");
   tPLobsexcl_qg->Draw("colz");
   int col_gl=kBlue-10;
   for (std::vector<TGraph*>::iterator cont=contours_qg.begin(); cont!=contours_qg.end(); ++cont){
     if (! *cont) continue;
     double x, y;
     (*cont)->GetPoint(0, x, y);
     (*cont)->SetLineColor(col_gl);(*cont)->SetLineWidth(3);
     (*cont)->Draw("l");
     TLatex l; l.SetTextSize(0.04); l.SetTextColor(col_gl++);
     char val[20];
     sprintf(val,"%d",(int)(cont-contours_qg.begin()));
     l.DrawLatex(x,y,val); 
     if (cont-contours_qg.begin()>3) break;
   }
   c1->SaveAs("results/ExclusionTestContours_mSql_mGl_tb10.pdf");

   // Observed Exclusion in squark - gluino mass
   TH2F*hPLobsexcl_qg = new TH2F("plobsexcl_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Observed Exclusion",
                     60,200,1400,50,200,1200);
   plotTools->Area(hPLobsexcl_qg, MSquarkL, MGluino, PLObsExclusion);
   //hPLobsexcl_qg->Draw("colz");
   //c1->SaveAs("results/PL_ObsExclusion_mSql_mGl_tb10.pdf");
   




   // Exclusion in squark - gluino mass ----------------------------------------
   TH2F*hexcl_qg = new TH2F("hexcl_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Observed Exclusion",
                     60,200,1400,50,200,1200);
   TGraph * gexpexcl_qg = plotTools->GetContour(hexpexcl_qg,3,0); 
   if (gexpexcl_qg) gexpexcl_qg->SetLineStyle(2);
   TGraph * gobsexcl_qg = plotTools->GetContour(hobsexcl_qg,3,0);
   TH2F*hPLexpexcl_qg=(TH2F*)hexcl_qg->Clone();
   plotTools->Area(hPLexpexcl_qg, MSquarkL, MGluino, PLExpExclusion);   
   TGraph * gPLexpexcl_qg = plotTools->GetContour(hPLexpexcl_qg,3,0); 
   if (gPLexpexcl_qg)   {gPLexpexcl_qg->SetLineStyle(2);gPLexpexcl_qg->SetLineColor(2);}
   TGraph * gPLobsexcl_qg = plotTools->GetContour(hPLobsexcl_qg,3,0); 
   if (gPLobsexcl_qg) gPLobsexcl_qg->SetLineColor(2);
   TH2F*hCLsExpHT_qg=(TH2F*)hexcl_qg->Clone();
   TH2F*hCLsObsHT_qg=(TH2F*)hexcl_qg->Clone();
   plotToolsHT->Area(hCLsExpHT_qg, MSquarkL, MGluino, ExpExclCL);
   plotToolsHT->Area(hCLsObsHT_qg, MSquarkL, MGluino, ObsExclCL);
   TGraph * gCLsExpExclHT_qg = plotTools->GetContour(hCLsExpHT_qg,3,0); 
   TGraph * gCLsObsExclHT_qg = plotTools->GetContour(hCLsObsHT_qg,3,0); 
   if (gCLsObsExclHT_qg) gCLsObsExclHT_qg->SetLineColor(2);
   if (gCLsExpExclHT_qg) {gCLsExpExclHT_qg->SetLineStyle(2);gCLsExpExclHT_qg->SetLineColor(2);}
   hexcl_qg->Draw("");
   if (gobsexcl_qg) gobsexcl_qg->Draw("l");
   if (gCLsObsExclHT_qg) gCLsObsExclHT_qg->Draw("l");
   if (gexpexcl_qg) gexpexcl_qg->Draw("l");
   //if (gPLobsexcl_qg) gPLobsexcl_qg->Draw("l");
   //if (gPLexpexcl_qg) gPLexpexcl_qg->Draw("l");
   if (gCLsExpExclHT_qg) gCLsExpExclHT_qg->Draw("l");
   TLegend * leg_qg = new TLegend(0.45,0.78,0.85,0.89);
   leg_qg->SetBorderSize(0);leg_qg->SetFillColor(0);
   if (gobsexcl_qg) leg_qg->AddEntry(gobsexcl_qg,"Observed (MHT, CLs, TLimit)","l");
   if (gexpexcl_qg) leg_qg->AddEntry(gexpexcl_qg,"Expected (MHT, CLs, TLimit)","l");
   if (gCLsObsExclHT_qg) leg_qg->AddEntry(gCLsObsExclHT_qg,"Observed (HT, CLs, TLimit)","l");
   if (gCLsExpExclHT_qg) leg_qg->AddEntry(gCLsExpExclHT_qg,"Expected (HT, CLs, TLimit)","l");
   //if (gPLobsexcl_qg) leg_qg->AddEntry(gPLobsexcl_qg,"Observed (PL, RooStat)","l");
   //if (gPLexpexcl_qg) leg_qg->AddEntry(gPLexpexcl_qg,"Expected (PL, RooStat)","l");
   leg_qg->Draw();
   c1->SaveAs("results/Exclusion_mSql_mGl_tb10.pdf");
   
   //c1->SaveAs("plot_tb10.pdf");

<<<<<<< plot.cc
   c1->SetLogy(1);
   c1->SetLogx(1);
   TH2F*hUncertScan = new TH2F("hUncertScan",";relative signal uncertainty; number of signal events",
                     10,0.1,0.6,10,0.9,200);
   //hUncertScan->GetXaxis()->SetNdivisions(505, false);		     
   //hUncertScan->SetMinimum(1);	     
   TGraph * gCLsExpUncert = new TGraph(0);
   TGraph * gCLsObsUncert = new TGraph(0);
   TGraph * gPLExpUncert  = new TGraph(0);
   TGraph * gPLObsUncert  = new TGraph(0);
   TGraph * gFCExpUncert  = new TGraph(0);
   TGraph * gFCObsUncert  = new TGraph(0);
   TGraph * gMCMCExpUncert= new TGraph(0);
   TGraph * gMCMCObsUncert= new TGraph(0);
   plotTools->Graph(gCLsExpUncert, SignalRelUncertainty, ExpNSignLimit,1); 
   plotTools->Graph(gCLsObsUncert, SignalRelUncertainty, ObsNSignLimit,1); 
   plotTools->Graph(gPLExpUncert,  SignalRelUncertainty, PLExpNSignLimit,1); 
   plotTools->Graph(gPLObsUncert,  SignalRelUncertainty, PLObsNSignLimit, 1); 
   plotTools->Graph(gFCExpUncert,  SignalRelUncertainty, FCExpNSignLimit,1); 
   plotTools->Graph(gFCObsUncert,  SignalRelUncertainty, FCObsNSignLimit, 1); 
   plotTools->Graph(gMCMCExpUncert,SignalRelUncertainty, MCMCExpNSignLimit,1); 
   plotTools->Graph(gMCMCObsUncert,SignalRelUncertainty, MCMCObsNSignLimit, 1); 
   gCLsExpUncert->SetLineStyle(8);
   gPLObsUncert->SetLineColor(2);
   gPLExpUncert->SetLineColor(2); gPLExpUncert->SetLineStyle(8);
   gFCObsUncert->SetLineColor(3);
   gFCExpUncert->SetLineColor(3); gFCExpUncert->SetLineStyle(8);
   gMCMCObsUncert->SetLineColor(4);
   gMCMCExpUncert->SetLineColor(4); gMCMCExpUncert->SetLineStyle(8);
   hUncertScan->Draw("h");
   gCLsExpUncert->Draw("l");
   gCLsObsUncert->Draw("l");
   //gPLObsUncert->Draw("l");
   gPLExpUncert->Draw("l");
   //gFCObsUncert->Draw("l");
   //gFCExpUncert->Draw("l");
   //gMCMCObsUncert->Draw("l");
   //gMCMCExpUncert->Draw("l");
   TLatex l; l.SetTextSize(0.03); l.SetTextColor(1);
   l.DrawLatex(0.025,25,"excluded"); 
   TMarker m; m.SetMarkerStyle(29); m.SetMarkerColor(2);
   m.DrawMarker(0.128, 28.5);l.DrawLatex(0.14,28.5,"LM1");
   m.DrawMarker(0.147, 77.4);l.DrawLatex(0.16,77.4,"LM0");
 
   TLegend * leg_S = new TLegend(0.5,0.14,0.85,0.38);
   leg_S->SetBorderSize(0);leg_S->SetFillColor(0);
   leg_S->SetHeader("95% CL upper limits");
   leg_S->AddEntry(gCLsExpUncert,"Expected (CLs, TLimit)","l");
   leg_S->AddEntry(gCLsObsUncert,"Observed (CLs, TLimit)","l");
   //leg_S->AddEntry(gPLExpUncert,"Expected (PL, RooStat)","l");
   //leg_S->AddEntry(gPLObsUncert,"Observed (PL, RooStat)","l");
   //leg_S->AddEntry(gFCObsUncert,"Observed (FC, RooStat)","l");
   //leg_S->AddEntry(gFCExpUncert,"Expected (FC, RooStat)","l");
   //leg_S->AddEntry(gMCMCObsUncert,"Observed (MCMC, RooStat)","l");
   //leg_S->AddEntry(gMCMCExpUncert,"Expected (MCMC, RooStat)","l");
   leg_S->Draw();
   c1->SaveAs("results/UncertaintyScan.pdf");
=======
   c1->SetLogy(1);
   c1->SetLogx(1);
   TH2F*hUncertScan = new TH2F("hUncertScan",";relative signal uncertainty; number of signal events",
                     10,0.1,0.6,10,0.9,200);
   //hUncertScan->GetXaxis()->SetNdivisions(505, false);		     
   //hUncertScan->SetMinimum(1);	     
   TGraph * gCLsExpUncert = new TGraph(0);
   TGraph * gCLsObsUncert = new TGraph(0);
   TGraph * gPLExpUncert  = new TGraph(0);
   TGraph * gPLObsUncert  = new TGraph(0);
   TGraph * gFCExpUncert  = new TGraph(0);
   TGraph * gFCObsUncert  = new TGraph(0);
   TGraph * gMCMCExpUncert= new TGraph(0);
   TGraph * gMCMCObsUncert= new TGraph(0);
   plotTools->Graph(gCLsExpUncert, SignalRelUncertainty, ExpNSignLimit,1); 
   plotTools->Graph(gCLsObsUncert, SignalRelUncertainty, ObsNSignLimit,1); 
   plotTools->Graph(gPLExpUncert,  SignalRelUncertainty, PLExpNSignLimit,1); 
   plotTools->Graph(gPLObsUncert,  SignalRelUncertainty, PLObsNSignLimit, 1); 
   plotTools->Graph(gFCExpUncert,  SignalRelUncertainty, FCExpNSignLimit,1); 
   plotTools->Graph(gFCObsUncert,  SignalRelUncertainty, FCObsNSignLimit, 1); 
   plotTools->Graph(gMCMCExpUncert,SignalRelUncertainty, MCMCExpNSignLimit,1); 
   plotTools->Graph(gMCMCObsUncert,SignalRelUncertainty, MCMCObsNSignLimit, 1); 
   gCLsExpUncert->SetLineStyle(8);
   gPLObsUncert->SetLineColor(2);
   gPLExpUncert->SetLineColor(2); gPLExpUncert->SetLineStyle(8);
   gFCObsUncert->SetLineColor(3);
   gFCExpUncert->SetLineColor(3); gFCExpUncert->SetLineStyle(8);
   gMCMCObsUncert->SetLineColor(4);
   gMCMCExpUncert->SetLineColor(4); gMCMCExpUncert->SetLineStyle(8);
   hUncertScan->Draw("h");
   gCLsExpUncert->Draw("l");
   gCLsObsUncert->Draw("l");
   gPLObsUncert->Draw("l");
   gPLExpUncert->Draw("l");
   gFCObsUncert->Draw("l");
   gFCExpUncert->Draw("l");
   gMCMCObsUncert->Draw("l");
   gMCMCExpUncert->Draw("l");
   TLatex l; l.SetTextSize(0.03); l.SetTextColor(1);
   l.DrawLatex(0.025,25,"excluded"); 
   TMarker m; m.SetMarkerStyle(29); m.SetMarkerColor(2);
   m.DrawMarker(0.128, 28.5);l.DrawLatex(0.14,28.5,"LM1");
   m.DrawMarker(0.147, 77.4);l.DrawLatex(0.16,77.4,"LM0");
 
   TLegend * leg_S = new TLegend(0.5,0.14,0.85,0.38);
   leg_S->SetBorderSize(0);leg_S->SetFillColor(0);
   leg_S->SetHeader("95% CL upper limits");
   leg_S->AddEntry(gCLsExpUncert,"Expected (CLs, TLimit)","l");
   leg_S->AddEntry(gCLsObsUncert,"Observed (CLs, TLimit)","l");
   leg_S->AddEntry(gPLExpUncert,"Expected (PL, RooStat)","l");
   leg_S->AddEntry(gPLObsUncert,"Observed (PL, RooStat)","l");
   leg_S->AddEntry(gFCObsUncert,"Observed (FC, RooStat)","l");
   leg_S->AddEntry(gFCExpUncert,"Expected (FC, RooStat)","l");
   leg_S->AddEntry(gMCMCObsUncert,"Observed (MCMC, RooStat)","l");
   leg_S->AddEntry(gMCMCExpUncert,"Expected (MCMC, RooStat)","l");
   leg_S->Draw();
   c1->SaveAs("results/UncertaintyScan.pdf");
>>>>>>> 1.12

   //theApp.Run();
}


int main(int argc, char** argv)
{
  return plot(argc, argv);
}
