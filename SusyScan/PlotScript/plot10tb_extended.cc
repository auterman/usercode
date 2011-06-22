#include "plot.h"
#include "SusyScan.h"
#include "GeneratorMasses.h"
#include "PlotTools.h"
#include "PrintTools.h"
#include "TheLimits.h"
#include "GlobalFunctions.h"
#include "StyleSettings.h"

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
#include "TPaveText.h"

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
   /*
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
   */
   util::StyleSettings::paperNoTitle();
   gStyle->SetPadBottomMargin(0.18);
 
   //gROOT->SetStyle("MyStyle");
   TCanvas * c1 = new TCanvas("c1","c1",600,600);
   //c1->SetFillStyle   ( 4000 );
   //c1->SetLeftMargin  ( 0.15 );
   //c1->SetRightMargin ( 0.15 );
   //c1->SetBottomMargin( 0.10 );
   c1->cd();
   
   //Get limits and generator masses ---------------------------------------------------
   TheLimits * genpoints = new TheLimits();
   //genpoints->Fill(argc, argv); 
   //genpoints->Fill("limits_MHT_tb10_20110303/filelist.txt"); 
   genpoints->Fill("limits_MHT_20110609/filelist_tb10.txt"); 

   TheLimits * genpointsHT = new TheLimits();
   //genpointsHT->Fill("limits_HT_tb10_20110303/filelist.txt"); 
   genpointsHT->Fill("limits_HT_20110609/filelist_tb10.txt"); 

   //TheLimits * genpointsHT = new TheLimits();
   //genpointsHT->Fill("limits_HT_SigCont/filelist.txt"); 

   //TheLimits * genpointsMHT = new TheLimits();
   //genpointsMHT->Fill("limits_MHT_SigCont/filelist.txt"); 

   
   //Replace read limits with specific numbers
   //genpoints->OverwriteLimits("ABCD_MHT");
   
   genpoints->FillGeneratorMasses("GenScan_tb10.dat");
   genpoints->match();
   genpointsHT->FillGeneratorMasses("GenScan_tb10.dat");
   genpointsHT->match();
   //genpointsHT->FillGeneratorMasses("GenScan_tb10.dat");
   //genpointsHT->match();
   //genpointsMHT->FillGeneratorMasses("GenScan_tb10.dat");
   //genpointsMHT->match();

   //genpoints->ExpandGrid(1);
   //genpoints->ExpandGrid(1);
   //genpoints->ExpandGrid(1);
   //genpointsHT->ExpandGrid(1);
   //genpointsHT->ExpandGrid(1);
   //genpointsHT->ExpandGrid(1);

   //the plotting ----------------------------------------------------------------------
   //plotting helper functions
   PlotTools<SusyScan> * plotTools = new PlotTools<SusyScan>(genpoints->GetScan());
   PlotTools<SusyScan> * plotToolsHT = new PlotTools<SusyScan>(genpointsHT->GetScan());
   //PlotTools<SusyScan> * plotToolsHT = new PlotTools<SusyScan>(genpointsHT->GetScan());
   //PlotTools<SusyScan> * plotToolsMHT = new PlotTools<SusyScan>(genpointsMHT->GetScan());
   PlotTools<GeneratorMasses> * plotMasses = new PlotTools<GeneratorMasses>(genpoints->GetGeneratorMasses());

   //iso mass lines
   TGraph * gl500 = plotMasses->Line(Mzero, Mhalf, MGluino, 500, 0.8 );
   TGraph * gl600 = plotMasses->Line(Mzero, Mhalf, MGluino, 600, 0.8 );
   TGraph * gl700 = plotMasses->Line(Mzero, Mhalf, MGluino, 700, 0.8 );
   TGraph * gl800 = plotMasses->Line(Mzero, Mhalf, MGluino, 800, 0.8 );
   TGraph * gl900 = plotMasses->Line(Mzero, Mhalf, MGluino, 900, 0.8 );
   TGraph * gl1000 = plotMasses->Line(Mzero, Mhalf, MGluino, 1000, 0.8);
   TGraph * sq500 = plotMasses->Line(Mzero, Mhalf, MSquarkL, 500, 1);
   TGraph * sq600 = plotMasses->Line(Mzero, Mhalf, MSquarkL, 600, 1);
   TGraph * sq700 = plotMasses->Line(Mzero, Mhalf, MSquarkL, 700, 1);
   TGraph * sq800 = plotMasses->Line(Mzero, Mhalf, MSquarkL, 800, 1);
   TGraph * sq900 = plotMasses->Line(Mzero, Mhalf, MSquarkL, 900, 1);
   TGraph * sq1000 = plotMasses->Line(Mzero, Mhalf, MSquarkL, 1000, 1);

   TGraph * chi100 = plotMasses->Line(Mzero, Mhalf, MChi1, 50, 20);
   TGraph * cha200 = plotMasses->Line(Mzero, Mhalf, MCha1, 200, 20);
   cha200->SetLineColor(2);

   //the histograms
   c1->SetLogz(1);
   //h->SetMaximum(27);
   //h->SetMinimum(0.01);

/**/
   // cross-section in M0 - M1/2
   c1->SetRightMargin ( 0.2 );
   TH2F*hxsec = new TH2F("xsec",";m_{0} [GeV]; m_{1/2} [GeV]; cross section [pb]",
                     200,0,2009.9,35,100,450);
   plotTools->Area(hxsec, Mzero, Mhalf, Xsection);
   hxsec->SetMinimum(0.01);
   //sq500->Draw();
   //gl500->Draw();
   hxsec->Draw("colz");
   c1->SaveAs("results_tb10/Xsection_m0_m12_tb10.pdf");
   std::string wait;
   //std::cin>>wait;

   // Observed Limit in M0 - M1/2
   TH2F*hobslimit = new TH2F("obslimit",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Observed Limit [pb]",
                     200,0,2009.9,35,100,450);
   plotTools->Area(hobslimit, Mzero, Mhalf, ObsXsecLimit);
   hobslimit->SetMinimum(0.01);
   hobslimit->Draw("colz");
   c1->SaveAs("results_tb10/ObsLimit_m0_m12_tb10.pdf");
   

   // Expected Limit in M0 - M1/2
   TH2F*hexplimit = new TH2F("explimit",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Expected Limit [pb]",
                     200,0,2009.9,35,100,450);
   plotTools->Area(hexplimit, Mzero, Mhalf, ExpXsecLimit);
   hexplimit->SetMinimum(0.01);
   hexplimit->Draw("colz");
   c1->SaveAs("results_tb10/ExpLimit_m0_m12_tb10.pdf");
   
   // Signal Acceptance in M0 - M1/2   
   c1->SetRightMargin ( 0.2 );
   c1->SetTopMargin ( 0.11 );
   c1->SetLogz(0);
   TH2F*hsigacc = new TH2F("sigacc",";m_{0} [GeV]; m_{1/2} [GeV]; Signal Acceptance",
                     100,0,2009.9,35,100,450);
   hsigacc->SetNdivisions(505);		     
   plotTools->Area(hsigacc, Mzero, Mhalf, NLOSignalAcceptance);
   hsigacc->SetMinimum(0.0);
   hsigacc->SetMaximum(0.35);
   hsigacc->SetContour(14);
   hsigacc->GetZaxis()->SetTitleOffset(1.5);
   hsigacc->Draw("colz");
   TLatex as; as.SetTextSize(0.025); as.SetTextFont(42);//ms.SetTextColor(12);
   as.DrawLatex(520,461,"MHT selection, tan#beta=10, #mu>0, A_{0}=0");
   as.SetTextSize(0.04);  
   as.DrawLatex( 50,461,"CMS"); 
   c1->SaveAs("results_tb10/SigAccMHT_m0_m12_tb10.pdf");

   
   // Signal Acceptance in M0 - M1/2   
   TH2F*hsigaccHT = new TH2F("sigaccHT",";m_{0} [GeV]; m_{1/2} [GeV]; Signal Acceptance",
                     100,0,2009.9,35,100,450);
   hsigaccHT->SetNdivisions(505);		     
   plotToolsHT->Area(hsigaccHT, Mzero, Mhalf, NLOSignalAcceptance);
   hsigaccHT->SetMinimum(0.0);
   hsigaccHT->SetMaximum(0.35);
   hsigaccHT->SetContour(14);
   hsigaccHT->GetZaxis()->SetTitleOffset(1.5);
   hsigaccHT->Draw("colz");
   as.SetTextSize(0.025); as.SetTextFont(42);//ms.SetTextColor(12);
   as.DrawLatex(525,461,"HT selection, tan#beta=10, #mu>0, A_{0}=0");
   as.SetTextSize(0.04);  
   as.DrawLatex( 50,461,"CMS"); 
   c1->SaveAs("results_tb10/SigAccHT_m0_m12_tb10.pdf");
   c1->SetTopMargin ( 0.1 );
 
   
   // Exp. Limit on Number of Signal Events in M0 - M1/2
   c1->SetLogz(0);
   TH2F*hexplimitnsig = new TH2F("explimitnsig",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL exp. limit signal events [# ]",
                     200,0,2009.9,35,100,450);
   plotTools->Area(hexplimitnsig, Mzero, Mhalf, ExpNSignLimit);
   hexplimitnsig->SetMinimum(0.0);
   hexplimitnsig->SetMaximum(20);
   hexplimitnsig->Draw("colz");
   c1->SaveAs("results_tb10/ExpLimitOnNSig_m0_m12_tb10.pdf");
   
   // Obs. Limit on Number of Signal Events in M0 - M1/2
   TH2F*hobslimitnsig = new TH2F("obslimitnsig",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL obs. limit signal events [# ]",
                     200,0,2009.9,35,100,450);
   plotTools->Area(hobslimitnsig, Mzero, Mhalf, ObsNSignLimit);
   hobslimitnsig->SetMinimum(0.0);
   hobslimitnsig->SetMaximum(20);
   hobslimitnsig->Draw("colz");
   c1->SaveAs("results_tb10/ObsLimitOnNSig_m0_m12_tb10.pdf");
   
   c1->SetLogz(0);
   // Expected Exclusion in M0 - M1/2
   TH2F*hexpexcl = new TH2F("expexcl",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Expected Exclusion",
                     200,0,2009.9,35,100,450);
   plotTools->Area(hexpexcl, Mzero, Mhalf, NLOExpExclCL);
   hexpexcl->Draw("colz");
   c1->SaveAs("results_tb10/ExpExclusionMHT_m0_m12_tb10.pdf");
   
   // Expected Exclusion in M0 - M1/2
   TH2F*hexpexclHT = new TH2F("expexclHT",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Expected Exclusion",
                     200,0,2009.9,35,100,450);
   plotToolsHT->Area(hexpexclHT, Mzero, Mhalf, NLOExpExclCL);
   hexpexclHT->Draw("colz");
   c1->SaveAs("results_tb10/ExpExclusionHT_m0_m12_tb10.pdf");
   
   // Observed Exclusion in M0 - M1/2
   TH2F*hobsexcl = new TH2F("obsexcl",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Observed Exclusion",
                     200,0,2009.9,35,100,450);
   plotTools->Area(hobsexcl, Mzero, Mhalf, NLOObsExclCL);
   hobsexcl->Draw("colz");
   c1->SaveAs("results_tb10/ObsExclusionMHT_m0_m12_tb10.pdf");

   // Observed Exclusion in M0 - M1/2
   TH2F*hobsexclHT = new TH2F("obsexclHT",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Observed Exclusion",
                     200,0,2009.9,35,100,450);
   plotToolsHT->Area(hobsexclHT, Mzero, Mhalf, NLOObsExclCL);
   hobsexclHT->Draw("colz");
   c1->SaveAs("results_tb10/ObsExclusionHT_m0_m12_tb10.pdf");



   // TestContours in M0 - M1/2 /////////////////////////////////////////////////////////////////////
   c1->SetRightMargin ( 0.1 );
   TH2F*texcl = new TH2F("texcl",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Expected Exclusion",
                     200,0,2009.9,35,100,450);
   TH2F*testExclold=(TH2F*)texcl->Clone();
   TH2F*testExclnew=(TH2F*)texcl->Clone();
   plotTools->Area(testExclold, Mzero, Mhalf, NLOHybridObsExclCL);
   plotTools->Area(testExclnew, Mzero, Mhalf, NLOHybridObsCL );
   
   //std::vector<TGraph*> contours = plotTools->GetContours005(testExclnew,3); 
   std::vector<TGraph*> contours = plotToolsHT->GetContours(testExclold,3); 
   testExclold->SetNdivisions(505);		     
   testExclold->Draw("colz");
   //testExclnew->Draw("colz");
   int col=kBlue-10;
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
   c1->SaveAs("results_tb10/ExclusionTestContours_m0_m12_tb10.pdf");


   // Exclusion in M0 - M1/2
   TH2F*hexcl = new TH2F("hexcl",";m_{0} (GeV); m_{1/2} (GeV); 95% CL Exclusion",
                     200,0,2009.9,35,110,450);
   hexcl->SetNdivisions(505);		     
   TH2F*hs = new TH2F("hs","",200,0,2009.9,35,100,450);
   TGraph * gexpexcl         = plotTools  ->GetContour(hs,Mzero,Mhalf,NLOExpExclCL,       3,0, 2,2); 
   TGraph * gexpexcl_LO      = plotTools  ->GetContour(hs,Mzero,Mhalf,ExpExclCL,          3,0, 2,4); 
   TGraph * gobsexcl         = plotTools  ->GetContour(hs,Mzero,Mhalf,NLOObsExclCL,       3,0, 2,1);
   TGraph * gobsexcl_LO      = plotTools  ->GetContour(hs,Mzero,Mhalf,ObsExclCL,          3,0, 2,1);
   TGraph * gMCMCexpexcl     = plotToolsHT->GetContour(hs,Mzero,Mhalf,MCMCExpExclusion,   3,0, 3,2); 
   TGraph * gMCMCobsexcl     = plotToolsHT->GetContour(hs,Mzero,Mhalf,MCMCObsExclusion,   3,0, 3,1); 
   TGraph * gFCexpexclHT     = plotToolsHT->GetContour(hs,Mzero,Mhalf,NLOFCExpExclusion,  3,0, 3,2); 
   TGraph * gFCexpexclHT_LO  = plotToolsHT->GetContour(hs,Mzero,Mhalf,FCExpExclusion,     3,0, 3,4); 
   TGraph * gFCexpexclMHT    = plotTools->GetContour(hs,Mzero,Mhalf,NLOFCExpExclusion,    3,0, 3,2); 
   TGraph * gFCexpexclMHT_LO = plotTools->GetContour(hs,Mzero,Mhalf,FCExpExclusion,       3,0, 3,4); 
   TGraph * gFCobsexclHT     = plotToolsHT->GetContour(hs,Mzero,Mhalf,NLOFCObsExclusion,  3,0, 3,1); 
   TGraph * gFCobsexclMHT    = plotToolsHT->GetContour(hs,Mzero,Mhalf,NLOFCObsExclusion,  3,0, 3,1); 
   TGraph * gCLsExpExclHT    = plotToolsHT->GetContour(hs,Mzero,Mhalf,NLOExpExclCL,	  3,0, 1,2); 
   //TGraph * gCLsExpNoSExclMHT= plotTools  ->GetContour(hs,Mzero,Mhalf,NLOExpNoSigExclCL,  3,0, 3,2); 
   //TGraph * gCLsExpNoSExclHT = plotToolsHT->GetContour(hs,Mzero,Mhalf,NLOExpNoSigExclCL,  3,0, 3,2); 
   TGraph * gCLsObsExclHT    = plotToolsHT->GetContour(hs,Mzero,Mhalf,NLOObsExclCL,	  3,0, 1,1); 
   TGraph * gCLsExpExclHT_LO = plotToolsHT->GetContour(hs,Mzero,Mhalf,ExpExclCL,	  3,0, 1,4); 
   TGraph * gCLsObsExclHT_LO = plotToolsHT->GetContour(hs,Mzero,Mhalf,ObsExclCL,	  3,0, 2,1); 
   TGraph * gCLsExpExclHTm1  = plotToolsHT->GetContour(hs,Mzero,Mhalf,NLOExpExclCLm1sigma,3,0, 5,2); 
   TGraph * gCLsExpExclHTp1  = plotToolsHT->GetContour(hs,Mzero,Mhalf,NLOExpExclCLp1sigma,3,0, 5,2); 
   TGraph * gCLsExpExclMHTm1 = plotTools  ->GetContour(hs,Mzero,Mhalf,NLOExpExclCLm1sigma,  3,0, 5,2); 
   TGraph * gCLsExpExclMHTp1 = plotTools  ->GetContour(hs,Mzero,Mhalf,NLOExpExclCLp1sigma,  3,0, 5,2); 
   TGraph * gHybridCLsObsExclHT = plotToolsHT->GetContour(hs,Mzero,Mhalf,NLOHybridObsExclCL,	  3,1, 6,1); 
   TGraph * gHybridCLsObsExclMHT= plotTools  ->GetContour(hs,Mzero,Mhalf,NLOHybridObsExclCL,	  3,1, 6,1); 
   TGraph * gCheck1 = (TGraph*)gCLsObsExclHT->Clone();
   TGraph * gCheck2 = (TGraph*)gCLsExpExclHT->Clone();
   Smooth( gCLsObsExclHT, 27); gCLsObsExclHT->SetLineWidth( 3 );
   //Smooth( gCLsExpNoSExclMHT, 27 ); gCLsExpNoSExclMHT->SetLineWidth( 3 );
   //Smooth( gCLsExpNoSExclHT, 27 );  gCLsExpNoSExclHT->SetLineWidth( 3 );
   Smooth( gCLsExpExclHT, 27 ); gCLsExpExclHT->SetLineWidth( 3 );
   Smooth( gCLsObsExclHT_LO, 27); gCLsObsExclHT_LO->SetLineWidth( 2 );
   Smooth( gCLsExpExclHT_LO, 27 ); gCLsExpExclHT_LO->SetLineWidth( 3 );
   Smooth( gexpexcl, 27 );
   Smooth( gobsexcl_LO, 27 );
   Smooth( gobsexcl, 27 );
   Smooth( gexpexcl_LO, 27 );
   Smooth( gFCexpexclHT, 27 );
   Smooth( gFCexpexclHT_LO, 27 );gFCexpexclHT_LO->SetLineWidth( 3 );
   Smooth( gFCexpexclMHT, 27 );
   Smooth( gFCexpexclMHT_LO, 27 );
   Smooth( gFCobsexclHT, 27 );gFCobsexclHT->SetLineWidth( 3 );
   Smooth( gFCobsexclMHT, 27 );
   Smooth( gCLsExpExclHTm1, 27 );
   Smooth( gCLsExpExclHTp1, 27 );
   Smooth( gCLsExpExclMHTm1, 27 );
   Smooth( gCLsExpExclMHTp1, 27 );
//   Smooth( gHybridCLsObsExclHT, 27); gHybridCLsObsExclHT->SetLineWidth( 3 );
//   Smooth( gHybridCLsObsExclMHT, 27);gHybridCLsObsExclMHT->SetLineWidth( 3 );
   TGraph * CLsObsNLO = plotToolsHT->ChooseBest(gCLsObsExclHT,gobsexcl, gCLsObsExclHT,gobsexcl);
   TGraph * CLsObsLO  = plotToolsHT->ChooseBest(gCLsObsExclHT_LO,gobsexcl_LO, gCLsObsExclHT_LO,gobsexcl_LO);
   TGraph * CLsExpNLO = plotToolsHT->ChooseBest(gCLsExpExclHT,gexpexcl, gCLsExpExclHT,gexpexcl);
   //TGraph * CLsExpNoSNLO = plotToolsHT->ChooseBest(gCLsExpNoSExclHT,gCLsExpNoSExclMHT, gCLsExpNoSExclHT,gCLsExpNoSExclMHT);
   TGraph * CLsExpLO  = plotToolsHT->ChooseBest(gCLsExpExclHT_LO,gexpexcl_LO, gCLsExpExclHT_LO,gexpexcl_LO);
   TGraph * FCExpLO   = plotToolsHT->ChooseBest(gFCexpexclHT_LO,gFCexpexclMHT_LO,gFCexpexclHT_LO,gFCexpexclMHT_LO);
   TGraph * FCExpNLO  = plotToolsHT->ChooseBest(gFCexpexclHT,gFCexpexclMHT,gFCexpexclHT,gFCexpexclMHT);
   TGraph * FCObsNLO  = plotToolsHT->ChooseBest(gFCobsexclHT,gFCobsexclMHT,gFCobsexclHT,gFCobsexclMHT);
   TGraph * HybridCLsObsNLO = plotToolsHT->ChooseBest(gHybridCLsObsExclHT,gHybridCLsObsExclMHT,   gHybridCLsObsExclHT,gHybridCLsObsExclMHT);
   TGraph * gCLsExpExclp1 = plotToolsHT->ChooseBest(gCLsExpExclHTp1,gCLsExpExclMHTp1, gCLsExpExclHTp1,gCLsExpExclMHTp1);
   TGraph * gCLsExpExclm1 = plotToolsHT->ChooseBest(gCLsExpExclHTm1,gCLsExpExclMHTm1, gCLsExpExclHTm1,gCLsExpExclMHTm1);
   TGraph * gCLsExp1Sigma = MakeBand(gCLsExpExclp1, gCLsExpExclm1);gCLsExp1Sigma->SetFillStyle(4010);
   hexcl->GetYaxis()->SetTitleOffset(1.3);
   hexcl->GetXaxis()->SetTitleOffset(0.92);
   hexcl->Draw("colz");
   //set old exclusion Limits
   TGraph* LEP_ch = set_lep_ch(10);
   TGraph* LEP_sl = set_lep_sl(10);//slepton curve
   TGraph* TEV_sg_cdf = set_tev_sg_cdf(10);//squark gluino cdf
   TGraph* TEV_sg_d0 = set_tev_sg_d0(10);//squark gluino d0
   //TGraph* TEV_tlp_cdf = set_tev_tlp_cdf(10);//trilepton cdf
   //TGraph* TEV_tlp_d0 = set_tev_tlp_d0(10);//trilepton d0
   TGraph* Atlas = Atlas_m0_m12_tb3_obs();
   Atlas->SetLineColor( 28 );
   TGraph* stau = set_tev_stau(10);//stau 
   TGraph* TEV_sn_d0_1 = set_sneutrino_d0_1(10);
   TGraph* TEV_sn_d0_2 = set_sneutrino_d0_2(10);
   TGraphErrors* First  = getObserved_NLO_tanBeta10();
   TGraphErrors* Second = getExpected_NLO_tanBeta10();//getLO_jetMultis();
   TGraphErrors* Third  = getLO_tanBeta10();
   First->GetXaxis()->SetRangeUser(0,505);
   First->GetYaxis()->SetRangeUser(80,500);
   First->GetXaxis()->SetTitle("m_{0} (GeV)");
   First->GetYaxis()->SetTitle("m_{1/2} (GeV)");
   TSpline3 *sFirst = new TSpline3("sFirst",First); sFirst->SetLineColor(kBlue);sFirst->SetLineWidth(1);
   TSpline3 *sSecond = new TSpline3("sSecond",Second);
   sSecond->SetLineColor(kBlue); sSecond->SetLineStyle(2); sSecond->SetLineWidth(1);
   TSpline3 *sThird = new TSpline3("sThird",Third);
   sThird->SetLineColor(kBlue); sThird->SetLineStyle(4); sThird->SetLineWidth(1);
   //TEV_sn_d0_1->Draw("fsame");
   //TEV_sn_d0_2->Draw("fsame"); //only for tb=3
   LEP_ch->Draw("fsame");
   LEP_sl->Draw("fsame");
   TEV_sg_cdf->Draw("fsame");
   TEV_sg_cdf->Draw("lsame");
   TEV_sg_d0->Draw("fsame");
   TEV_sg_d0->Draw("lsame");
   TLatex b; b.SetTextSize(0.02); b.SetTextColor(1);
   //b.DrawLatex(  6,150,"LEP2"); 
   //b.DrawLatex( 22,138,"#tilde{l}^{#pm}"); 
   //b.DrawLatex(360,110,"LEP2 #tilde{#chi}^{#pm}"); 
   //b.DrawLatex(90,160,"CDF"); 
   //b.DrawLatex(248,129,"D0 #tilde{g}, #tilde{q}"); 
   //b.DrawLatex( 80,180,"D0 #tilde{#nu}"); 
//   if (gCLsExp1Sigma)    gCLsExp1Sigma->Draw("lf");
   sFirst->Draw("same");
   //sSecond->Draw("same");
   //sThird->Draw("same");
   Atlas->Draw("c,same");
   TLatex ms; ms.SetTextSize(0.025); ms.SetTextFont(42);//ms.SetTextColor(12);
   ms.DrawLatex(750,458,"tan#beta=10, #mu>0, A_{0}=0"); 
 
  //gCheck1->Draw("same");
  //gCheck2->Draw("same");
  
   //LM points
   TMarker* LM0 = new TMarker(200.,160.,20);
   TMarker* LM1 = new TMarker(60.,250.,20);
   LM0->SetMarkerSize(0.7);
   LM1->SetMarkerSize(0.7);
   TLatex* tLM0 = new TLatex(205.,160.,"LM0");
   tLM0->SetTextSize(0.02);
   TLatex* tLM1 = new TLatex(65.,243.,"LM1");
   tLM1->SetTextSize(0.02);
   LM0->Draw("same");   
   tLM0->Draw("same");
   LM1->Draw("same");   
   tLM1->Draw("same");
   TLegend* legexp = new TLegend(0.61,0.61,0.93,0.88,NULL,"brNDC");
   legexp->SetFillColor(0);legexp->SetShadowColor(0);legexp->SetFillStyle(4000);legexp->SetTextFont(42);legexp->SetTextSize(0.025);legexp->SetBorderSize(0);
   //TEV_sg_cdf.SetLineColor(1);  
   legexp->SetHeader("CMS");
   legexp->AddEntry(TEV_sg_cdf,"CDF  #tilde{#font[12]{g}}, #tilde{#font[12]{q}}, #scale[0.8]{tan#beta=5, #mu<0}","f"); 
   legexp->AddEntry(TEV_sg_d0,"D0   #tilde{#font[12]{g}}, #tilde{#font[12]{q}}, #scale[0.8]{tan#beta=3, #mu<0}","f");  
   //ch_gr.SetLineColor(1); 
   legexp->AddEntry(LEP_ch,"LEP2   #tilde{#chi}_{1}^{#pm}","f");   
   //sl_gr.SetLineColor(1); 
   legexp->AddEntry(LEP_sl,"LEP2   #tilde{#font[12]{l}}^{#pm}","f"); //NOT FOR tb=50!
   //if(tanbeta == 3) 
   //legexp->AddEntry(TEV_sn_d0_1,"D0  #chi^{#pm}_{1}, #chi^{0}_{2}","f");  
   legexp->AddEntry(sFirst, "CMS #alpha_{T}");
   //legexp->AddEntry(Atlas,  "Atlas, #scale[0.8]{tan#beta=3}","l");
   legexp->Draw();
   
   CLsObsNLO->Draw("l");
   CLsObsLO->Draw("l");
   CLsExpNLO->Draw("l");
   //FCExpNLO->Draw("l");
   //CLsExpNoSNLO->Draw("l");
//   HybridCLsObsNLO->Draw("l");
   //gobsexcl->Draw("l");
   //gHybridCLsObsExclMHT->Draw("l");

   
   stau->Draw("fsame");
   b.DrawLatex( 10,380,"#tilde{#tau} LSP"); 
   //constant ssqquark and gluino lines
   TF1* lnsq[4];
   TF1* lngl[4];
   TLatex sqt; sqt.SetTextSize(0.02); sqt.SetTextAngle(-22);sqt.SetTextColor(kGray+2);
   sqt.DrawLatex(148,220,"#font[92]{#tilde{q}(500)GeV}");
   sqt.DrawLatex(390,350,"#font[92]{#tilde{q}(800)GeV}");
   TLatex glt; glt.SetTextSize(0.02); sqt.SetTextAngle(-4); glt.SetTextColor(kGray+2);
   glt.DrawLatex(840,172,"#font[92]{#tilde{g}(500)GeV}");
   glt.DrawLatex(840,235,"#font[92]{#tilde{g}(650)GeV}");
   glt.DrawLatex(838,297,"#font[92]{#tilde{g}(800)GeV}");
   int tanBeta_=10;
   for(int i = 0; i < 4; i++){
    lnsq[i] = constant_squark(tanBeta_,i);
    lngl[i] = constant_gluino(tanBeta_,i);
    lngl[i]->Draw("same");   
    lnsq[i]->Draw("same");
   }
   TLegend * leg = new TLegend(0.25,0.76,0.6,0.88);
   leg->SetBorderSize(0);leg->SetFillColor(0);leg->SetFillStyle(4000);leg->SetTextFont(42);legexp->SetTextSize(0.025);
   TGraph * expLeg = (TGraph*)CLsExpNLO->Clone();expLeg->SetFillStyle(gCLsExp1Sigma->GetFillStyle());expLeg->SetFillColor(gCLsExp1Sigma->GetFillColor());
   leg->SetHeader("L_{int} = 36/pb, #sqrt{s} = 7 TeV");
   leg->AddEntry(CLsObsNLO,"Observed, NLO","l");
   leg->AddEntry(CLsObsLO, "Observed, LO","l");
   leg->AddEntry(expLeg,   "Expected #pm 1#sigma, NLO","lf");
   //leg->AddEntry(CLsExpNoSNLO,   "Expected, no-signal hyp., NLO","l");
   leg->Draw();
   gPad->RedrawAxis();
   c1->SaveAs("results_tb10/Exclusion_m0_m12_tb10.pdf");


   //plotToolsHT->Print(NLOSignal, Mzero,Mhalf, gCLsObsExclHT, 5);
//   plotToolsHT->Print(NLOXsection, Mzero,Mhalf, MGluino, MSquarkL, gCLsObsExclHT, 5);
   
   hexcl->Draw("");
   gCLsExp1Sigma->SetName("CLsNLO_Exp1SigmaUncertaintyTb10");
   CLsObsNLO->SetName("CLsNLO_ObservedTb10");
   CLsExpNLO->SetName("CLsNLO_ExpectedTb10");
   CLsExpLO->SetName("CLsLO_ExpectedTb10");
   gCLsExp1Sigma->Draw("l");
   CLsObsNLO->Draw("l");
   CLsExpNLO->Draw("l");
   CLsObsLO->Draw("l");
   c1->SaveAs("results_tb10/LimitContours_tb10.C");
   // ===============================================================================================================


   //The RA1-style Expected limits - no-signal hypothesis *only* for pseudo data ///////////////////////////////
   TGraph * gCLsExpNoSExclMHT   = plotTools  ->GetContour(hs,Mzero,Mhalf,NLOExpNoSigExclCL,  3,0, 1,2); 
   TGraph * gCLsExpNoSExclHT    = plotToolsHT->GetContour(hs,Mzero,Mhalf,NLOExpNoSigExclCL,  3,0, 1,2); 
   TGraph * gCLsExpNoSExclHTm1  = plotToolsHT->ModifyExpSigma(gCLsExpExclHTm1, gCLsExpExclHT, gCLsExpNoSExclHT); 
   TGraph * gCLsExpNoSExclHTp1  = plotToolsHT->ModifyExpSigma(gCLsExpExclHTp1, gCLsExpExclHT, gCLsExpNoSExclHT);
   TGraph * gCLsExpNoSExclMHTm1 = plotTools  ->ModifyExpSigma(gCLsExpExclMHTm1,gCLsExpExclHT, gCLsExpNoSExclHT); 
   TGraph * gCLsExpNoSExclMHTp1 = plotTools  ->ModifyExpSigma(gCLsExpExclMHTp1,gCLsExpExclHT, gCLsExpNoSExclHT); 
   Smooth( gCLsExpNoSExclMHT, 27 ); gCLsExpNoSExclMHT->SetLineWidth( 3 );
   Smooth( gCLsExpNoSExclHT, 27 );  gCLsExpNoSExclHT->SetLineWidth( 3 );
   Smooth( gCLsExpNoSExclHTm1, 27 );
   Smooth( gCLsExpNoSExclHTp1, 27 );
   Smooth( gCLsExpNoSExclMHTm1, 27 );
   Smooth( gCLsExpNoSExclMHTp1, 27 );
   TGraph * CLsExpNoSNLO = plotToolsHT->ChooseBest(gCLsExpNoSExclHT,gCLsExpNoSExclMHT, gCLsExpNoSExclHT,gCLsExpNoSExclMHT);
   TGraph * gCLsExpNoSExclp1 = plotToolsHT->ChooseBest(gCLsExpNoSExclHTp1,gCLsExpNoSExclMHTp1, gCLsExpNoSExclHTp1,gCLsExpNoSExclMHTp1);
   TGraph * gCLsExpNoSExclm1 = plotToolsHT->ChooseBest(gCLsExpNoSExclHTm1,gCLsExpNoSExclMHTm1, gCLsExpNoSExclHTm1,gCLsExpNoSExclMHTm1);
   TGraph * gCLsExpNoS1Sigma = MakeBand(gCLsExpNoSExclp1, gCLsExpNoSExclm1);gCLsExpNoS1Sigma->SetFillStyle(4010);
   hexcl->Draw("colz");
   LEP_ch->Draw("fsame");
   LEP_sl->Draw("fsame");
   TEV_sg_cdf->Draw("fsame");
   TEV_sg_cdf->Draw("lsame");
   TEV_sg_d0->Draw("fsame");
   TEV_sg_d0->Draw("lsame");
   ms.DrawLatex(1500,458,"tan#beta=10, #mu>0, A_{0}=0"); 
   LM0->SetMarkerSize(0.7);
   LM1->SetMarkerSize(0.7);
   tLM0->SetTextSize(0.02);
   tLM1->SetTextSize(0.02);
   LM0->Draw("same");   
   tLM0->Draw("same");
   LM1->Draw("same");   
   tLM1->Draw("same");
   gCLsExpNoS1Sigma->Draw("lf,same");
   sFirst->Draw("same");
   //Atlas->Draw("c,same");
   CLsObsNLO->Draw("l,same");
   CLsObsLO->Draw("l,same");
   CLsExpNoSNLO->Draw("l,same");
   //FCExpNLO->Draw("l");
   stau->Draw("fsame");
   b.DrawLatex( 10,380,"#tilde{#tau} LSP"); 
   //constant ssqquark and gluino lines
   sqt.DrawLatex(148,220,"#font[92]{#tilde{q}(500)GeV}");
   sqt.DrawLatex(390,350,"#font[92]{#tilde{q}(800)GeV}");
   glt.DrawLatex(840,172,"#font[92]{#tilde{g}(500)GeV}");
   glt.DrawLatex(840,235,"#font[92]{#tilde{g}(650)GeV}");
   glt.DrawLatex(838,297,"#font[92]{#tilde{g}(800)GeV}");
   for(int i = 0; i < 4; i++){
    lngl[i]->Draw("same");   
    lnsq[i]->Draw("same");
   }
   legexp->Draw();
   leg->Draw();
   gPad->RedrawAxis();
   c1->SaveAs("results_tb10/Exclusion_m0_m12_tb10_NoSigHypPseudoData.pdf");


   hexcl->Draw("");
   gCLsExp1Sigma->SetName("CLsNLO_Exp1SigmaUncertaintyTb10");
   CLsObsNLO->SetName("CLsNLO_ObservedTb10");
   CLsExpNoSNLO->SetName("CLsNLO_ExpectedTb10");
   CLsObsLO->SetName("CLsLO_ObservedTb10");
   //gCLsExpNoS1Sigma->Draw("l");
   CLsObsNLO->Draw("l,same");
   //CLsObsLO->Draw("l,same");
   CLsExpNoSNLO->Draw("l,same");
   c1->SaveAs("results_tb10/LimitContours_tb10_NoSigHypPseudoData.C");

   hexcl->Draw("colz");
   LEP_ch->Draw("fsame");
   LEP_sl->Draw("fsame");
   TEV_sg_cdf->Draw("fsame");
   TEV_sg_cdf->Draw("lsame");
   TEV_sg_d0->Draw("fsame");
   TEV_sg_d0->Draw("lsame");
   gCLsExpNoS1Sigma->Draw("lf,same");
   sFirst->Draw("same");
   Atlas->Draw("c,same");
   CLsObsNLO->Draw("l,same");
   CLsExpNoSNLO->Draw("l,same");
   ms.DrawLatex(750,458,"tan#beta=10, #mu>0, A_{0}=0"); 
   LM0->SetMarkerSize(0.7);
   LM1->SetMarkerSize(0.7);
   tLM0->SetTextSize(0.02);
   tLM1->SetTextSize(0.02);
   LM0->Draw("same");   
   tLM0->Draw("same");
   LM1->Draw("same");   
   tLM1->Draw("same");
   stau->Draw("fsame");
   b.DrawLatex( 10,380,"#tilde{#tau} LSP"); 
   //Jim's limits///////////////////////////////////////////////////////////////////////////////
   TGraph* JimObsHT  = Jim_ht_tb10(0); 
   TGraph* JimObsMHT = Jim_mht_tb10(0);
   TGraph* JimExpHT  = Jim_ht_tb10(1);
   TGraph* JimExpMHT = Jim_mht_tb10(1); 
   TGraph* JimExpHTup  = Jim_ht_tb10(2); 
   TGraph* JimExpMHTup = Jim_mht_tb10(2); 
   TGraph* JimExpHTdn  = Jim_ht_tb10(3); 
   TGraph* JimExpMHTdn = Jim_mht_tb10(3); 
   Smooth(JimObsHT, 2);
   Smooth(JimObsMHT,2);
   Smooth(JimExpHT, 2);
   Smooth(JimExpMHT,2);
   Smooth(JimExpHTup, 2);
   Smooth(JimExpMHTup,2);
   Smooth(JimExpHTdn, 2);
   Smooth(JimExpMHTdn,2);
   TGraph * JimObs = plotToolsHT->ChooseBest(JimObsHT,JimObsMHT,JimObsHT,JimObsMHT);
   TGraph * JimExp = plotToolsHT->ChooseBest(JimExpHT,JimExpMHT,JimExpHT,JimExpMHT);
   TGraph * JimExpup = plotToolsHT->ChooseBest(JimExpHTup,JimExpMHTup,JimExpHTup,JimExpMHTup);
   TGraph * JimExpdn = plotToolsHT->ChooseBest(JimExpHTdn,JimExpMHTdn,JimExpHTdn,JimExpMHTdn);
   TGraph * JimExp1Sigma = MakeBand(JimExpup, JimExpdn);JimExp1Sigma->SetFillStyle(3001);
   TGraph * JimLeg = (TGraph*)JimExp->Clone();JimLeg->SetFillStyle(JimExp1Sigma->GetFillStyle());JimLeg->SetFillColor(JimExp1Sigma->GetFillColor());
   JimExp1Sigma->Draw("f");
   JimObs->Draw("c");
   JimExp->Draw("c");
   stau->Draw("fsame");
   b.DrawLatex( 10,380,"#tilde{#tau} LSP"); 
   //constant ssqquark and gluino lines
   sqt.DrawLatex(148,220,"#font[92]{#tilde{q}(500)GeV}");
   sqt.DrawLatex(390,350,"#font[92]{#tilde{q}(800)GeV}");
   glt.DrawLatex(840,172,"#font[92]{#tilde{g}(500)GeV}");
   glt.DrawLatex(840,235,"#font[92]{#tilde{g}(650)GeV}");
   glt.DrawLatex(838,297,"#font[92]{#tilde{g}(800)GeV}");
   for(int i = 0; i < 4; i++){
    lngl[i]->Draw("same");   
    lnsq[i]->Draw("same");
   }
   legexp->Draw();
   TLegend * legBayes = new TLegend(0.25,0.730,0.6,0.88);
   legBayes->SetBorderSize(0);legBayes->SetFillColor(0);legBayes->SetFillStyle(4000);legBayes->SetTextFont(42);
   legBayes->SetHeader("L_{int} = 36/pb, #sqrt{s} = 7 TeV");
   legBayes->AddEntry(JimObs,   "Observed, Bayes","l");
   legBayes->AddEntry(JimLeg,   "Expected #pm 1#sigma, Bayes","lf");
   legBayes->AddEntry(CLsObsNLO,"Observed, CLs","l");
   legBayes->AddEntry(expLeg,   "Expected #pm 1#sigma, CLs","lf");
   legBayes->Draw();
   gPad->RedrawAxis();
   c1->SaveAs("results_tb10/Exclusion_m0_m12_tb10_Bayes.pdf");



   // Signal Contamination in M0 - M1/2
   c1->SetLogz(0);
   c1->SetRightMargin ( 0.2 );
   TH2F*hsigcont = new TH2F("sigcont",";m_{0} [GeV]; m_{1/2}; number of signal in bkg yield [events]",
                     200,0,2009.9,35,100,450);
   plotToolsHT->Area(hsigcont, Mzero, Mhalf, SignalContamination);
   hsigcont->SetMinimum(0.01);
   hsigcont->SetMaximum(20);
   hsigcont->Draw("colz");
   if (gCLsObsExclHT)    gCLsObsExclHT->Draw("l");
   c1->SaveAs("results_tb10/SignalContamination_HT_m0_m12_tb10.pdf");
   
   // Signal Contamination in M0 - M1/2
   c1->SetLogz(0);
   TH2F*hsigcontMHT = new TH2F("sigcontMHT",";m_{0} [GeV]; m_{1/2}; number of signal in bkg yield [events]",
                     200,0,2009.9,35,100,450);
   plotTools->Area(hsigcontMHT, Mzero, Mhalf, SignalContamination);
   hsigcontMHT->SetMinimum(0.01);
   hsigcontMHT->SetMaximum(20);
   hsigcontMHT->Draw("colz");
   if (gobsexcl)    gobsexcl->Draw("l");
   c1->SaveAs("results_tb10/SignalContamination_MHT_m0_m12_tb10.pdf");
   
   // Signal JEC Uncertainty  MHT   
   c1->SetLogz(0);
   TH2F*hsig_jec_mht = new TH2F("sigjecmht",";m_{0} [GeV]; m_{1/2} [GeV]; Signal JEC uncertainty",
                     200,0,2009.9,35,100,450);
   plotTools->Area(hsig_jec_mht, Mzero, Mhalf, SignalUncertJEC);
   hsig_jec_mht->SetMinimum(0.0);
   hsig_jec_mht->SetMaximum(0.25);
   hsig_jec_mht->SetContour(10);
   hsig_jec_mht->Draw("colz");
   if (gobsexcl)    gobsexcl->Draw("l");
   c1->SaveAs("results_tb10/SigJEC_MHT_m0_m12_tb10.pdf");
   
   // Signal JEC Uncertainty  HT   
   c1->SetLogz(0);
   TH2F*hsig_jec_ht = new TH2F("sigjecht",";m_{0} [GeV]; m_{1/2} [GeV]; Signal JEC uncertainty",
                     200,0,2009.9,35,100,450);
   plotToolsHT->Area(hsig_jec_ht, Mzero, Mhalf, SignalUncertJEC);
   hsig_jec_ht->SetMinimum(0.0);
   hsig_jec_ht->SetMaximum(0.25);
   hsig_jec_ht->SetContour(10);
   hsig_jec_ht->Draw("colz");
   if (gCLsObsExclHT)    gCLsObsExclHT->Draw("l");
   c1->SaveAs("results_tb10/SigJEC_HT_m0_m12_tb10.pdf");
   

   // Signal MuIso Uncertainty  MHT   
   c1->SetLogz(0);
   TH2F*hsig_MuIso_mht = new TH2F("sigMuIsomht",";m_{0} [GeV]; m_{1/2} [GeV]; Signal #mu_{iso} uncertainty",
                     200,0,2009.9,35,100,450);
   plotTools->Area(hsig_MuIso_mht, Mzero, Mhalf, SignalUncertMuIso);
   hsig_MuIso_mht->SetMinimum(0.0);hsig_MuIso_mht->GetZaxis()->SetTitleOffset(1.3);
   hsig_MuIso_mht->SetMaximum(0.05);
   hsig_MuIso_mht->SetContour(5);
   hsig_MuIso_mht->Draw("colz");
   if (gobsexcl)    gobsexcl->Draw("l");
   c1->SaveAs("results_tb10/SigMuIso_MHT_m0_m12_tb10.pdf");
   
   // Signal MuIso Uncertainty  HT   
   c1->SetLogz(0);
   TH2F*hsig_MuIso_ht = new TH2F("sigMuIsoht",";m_{0} [GeV]; m_{1/2} [GeV]; Signal #mu_{iso} uncertainty",
                     200,0,2009.9,35,100,450);
   plotToolsHT->Area(hsig_MuIso_ht, Mzero, Mhalf, SignalUncertMuIso);
   hsig_MuIso_ht->SetMinimum(0.0);hsig_MuIso_ht->GetZaxis()->SetTitleOffset(1.3);
   hsig_MuIso_ht->SetMaximum(0.05);
   hsig_MuIso_ht->SetContour(5);
   hsig_MuIso_ht->Draw("colz");
   if (gCLsObsExclHT)    gCLsObsExclHT->Draw("l");
   c1->SaveAs("results_tb10/SigMuIso_HT_m0_m12_tb10.pdf");
   


   // Signal PDF Uncertainty  MHT   
   c1->SetLogz(0);
   TH2F*hsig_PDF_mht = new TH2F("sigPDFmht",";m_{0} [GeV]; m_{1/2} [GeV]; Signal PDF uncertainty",
                     200,0,2009.9,35,100,450);
   plotTools->Area(hsig_PDF_mht, Mzero, Mhalf, SignalUncertPDF);
   hsig_PDF_mht->SetMinimum(0.0);hsig_PDF_mht->GetZaxis()->SetTitleOffset(1.3);
   hsig_PDF_mht->SetMaximum(0.20);
   hsig_PDF_mht->SetContour(10);
   hsig_PDF_mht->Draw("colz");
   if (gobsexcl)    gobsexcl->Draw("l");
   c1->SaveAs("results_tb10/SigPDF_MHT_m0_m12_tb10.pdf");
   
   // Signal PDF Uncertainty  HT   
   c1->SetLogz(0);
   TH2F*hsig_PDF_ht = new TH2F("sigPDFht",";m_{0} [GeV]; m_{1/2} [GeV]; Signal PDF uncertainty",
                     200,0,2009.9,35,100,450);
   plotToolsHT->Area(hsig_PDF_ht, Mzero, Mhalf, SignalUncertPDF);
   hsig_PDF_ht->SetMinimum(0.0);hsig_PDF_ht->GetZaxis()->SetTitleOffset(1.3);
   hsig_PDF_ht->SetMaximum(0.20);
   hsig_PDF_ht->SetContour(10);
   hsig_PDF_ht->Draw("colz");
   if (gCLsObsExclHT)    gCLsObsExclHT->Draw("l");
   c1->SaveAs("results_tb10/SigPDF_HT_m0_m12_tb10.pdf");
   

   // Signal kFactor Uncertainty  MHT   
   c1->SetLogz(0);
   TH2F*hsig_kFactorUncert_mht = new TH2F("sigkFactorUncertmht",";m_{0} [GeV]; m_{1/2} [GeV]; signal NLO scale uncertainty",
                     200,0,2009.9,35,100,450);
   plotTools->Area(hsig_kFactorUncert_mht, Mzero, Mhalf, SignalUncertKfactor);
   hsig_kFactorUncert_mht->SetMinimum(0.1);hsig_kFactorUncert_mht->GetZaxis()->SetTitleOffset(1.3);
   hsig_kFactorUncert_mht->SetMaximum(0.2);
   hsig_kFactorUncert_mht->SetContour(10);
   hsig_kFactorUncert_mht->Draw("colz");
   if (gobsexcl)    gobsexcl->Draw("l");
   c1->SaveAs("results_tb10/SigkFactorUncert_MHT_m0_m12_tb10.pdf");
   
   // Signal kFactor Uncertainty  HT   
   c1->SetLogz(0);
   TH2F*hsig_kFactorUncert_ht = new TH2F("sigkFactorUncertht",";m_{0} [GeV]; m_{1/2} [GeV]; signal NLO scale uncertainty",
                     200,0,2009.9,35,100,450);
   plotToolsHT->Area(hsig_kFactorUncert_ht, Mzero, Mhalf, SignalUncertKfactor);
   hsig_kFactorUncert_ht->SetMinimum(0.1);hsig_kFactorUncert_ht->GetZaxis()->SetTitleOffset(1.3);
   hsig_kFactorUncert_ht->SetMaximum(0.2);
   hsig_kFactorUncert_ht->SetContour(10);
   hsig_kFactorUncert_ht->Draw("colz");
   if (gCLsObsExclHT)    gCLsObsExclHT->Draw("l");
   c1->SaveAs("results_tb10/SigkFactorUncert_HT_m0_m12_tb10.pdf");
   
   // Signal kFactor HT   
   c1->SetLogz(0);
   TH2F*hsig_kFactor_ht = new TH2F("sigkFactorht",";m_{0} [GeV]; m_{1/2} [GeV]; NLO signal k-factor",
                     200,0,2009.9,35,100,450);
   plotToolsHT->Area(hsig_kFactor_ht, Mzero, Mhalf, SignalKfactor);
   hsig_kFactor_ht->SetMinimum(1.0);hsig_kFactor_ht->GetZaxis()->SetTitleOffset(1.3);
   hsig_kFactor_ht->SetMaximum(2.4);
   hsig_kFactor_ht->SetContour(7);
   hsig_kFactor_ht->Draw("colz");
   if (gCLsObsExclHT)    gCLsObsExclHT->Draw("l");
   c1->SaveAs("results_tb10/SigkFactor_HT_m0_m12_tb10.pdf");
   
   // Signal kFactor MHT   
   c1->SetLogz(0);
   TH2F*hsig_kFactor_mht = new TH2F("sigkFactormht",";m_{0} [GeV]; m_{1/2} [GeV]; NLO signal k-factor",
                     200,0,2009.9,35,100,450);
   plotTools->Area(hsig_kFactor_mht, Mzero, Mhalf, SignalKfactor);
   hsig_kFactor_mht->SetMinimum(1.0);hsig_kFactor_ht->GetZaxis()->SetTitleOffset(1.3);
   hsig_kFactor_mht->SetMaximum(2.4);
   hsig_kFactor_mht->SetContour(7);
   hsig_kFactor_mht->Draw("colz");
   if (gobsexcl)    gobsexcl->Draw("l");
   c1->SaveAs("results_tb10/SigkFactor_MHT_m0_m12_tb10.pdf");
   


 


   //theApp.Run();
}


int main(int argc, char** argv)
{
  return plot(argc, argv);
}
