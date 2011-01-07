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

#include <string>
#include <cmath>


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
   for (int i = 1; i<argc; ++i)
   {
     genpoints->add( new SusyScan(argv[i]) );
   }
   genpoints->FillGeneratorMasses("tb10_mu1_a0_massscan.dat");
   genpoints->match();


   //the plotting ----------------------------------------------------------------------
   //plotting helper functions
   PlotTools<SusyScan> * plotTools = new PlotTools<SusyScan>(genpoints->GetScan());
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
   
   // cross-section in M0 - M1/2
   TH2F*hxsec = new TH2F("xsec",";m_{0} [GeV]; m_{1/2} [GeV]; cross section [pb]",
                     100,0,1009.9,50,0,500);
   plotTools->Area(hxsec, Mzero, Mhalf, Xsection);
   hxsec->SetMinimum(0.01);
   //sq500->Draw();
   //gl500->Draw();
   hxsec->Draw("colz");
   c1->SaveAs("results/Xsection_m0_m12.png");
   std::string wait;
   //std::cin>>wait;
   
   // Observed Limit in M0 - M1/2
   TH2F*hobslimit = new TH2F("obslimit",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Observed Limit [pb]",
                     100,0,1009.9,50,0,500);
   plotTools->Area(hobslimit, Mzero, Mhalf, ObsXsecLimit);
   hobslimit->SetMinimum(0.01);
   hobslimit->Draw("colz");
   c1->SaveAs("results/ObsLimit_m0_m12.png");
   
   // Expected Limit in M0 - M1/2
   TH2F*hexplimit = new TH2F("explimit",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Expected Limit [pb]",
                     100,0,1009.9,50,0,500);
   plotTools->Area(hexplimit, Mzero, Mhalf, ExpXsecLimit);
   hexplimit->SetMinimum(0.01);
   hexplimit->Draw("colz");
   c1->SaveAs("results/ExpLimit_m0_m12.png");
   
   // Signal Acceptance in M0 - M1/2
   TH2F*hsigacc = new TH2F("sigacc",";m_{0} [GeV]; m_{1/2} [GeV]; Signal Acceptance",
                     100,0,1009.9,50,0,500);
   plotTools->Area(hsigacc, Mzero, Mhalf, SignalAcceptance);
   hsigacc->SetMinimum(0.01);
   hsigacc->SetMaximum(1.0);
   hsigacc->Draw("colz");
   chi100->Draw();
   cha200->Draw();
   gl500 ->Draw();
    sq500 ->Draw();
   c1->SaveAs("results/SigAcc_m0_m12.png");
   
   // Exp. Limit on Number of Signal Events in M0 - M1/2
   c1->SetLogz(0);
   TH2F*hexplimitnsig = new TH2F("explimitnsig",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL exp. limit signal events [# ]",
                     100,0,1009.9,50,0,500);
   plotTools->Area(hexplimitnsig, Mzero, Mhalf, ExpNSignLimit);
   //hexplimitnsig->SetMinimum(0.01);
   hexplimitnsig->Draw("colz");
   c1->SaveAs("results/ExpLimitOnNSig_m0_m12.png");
   
   // Obs. Limit on Number of Signal Events in M0 - M1/2
   TH2F*hobslimitnsig = new TH2F("obslimitnsig",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL obs. limit signal events [# ]",
                     100,0,1009.9,50,0,500);
   plotTools->Area(hobslimitnsig, Mzero, Mhalf, ObsNSignLimit);
   //hobslimitnsig->SetMinimum(0.01);
   hobslimitnsig->Draw("colz");
   c1->SaveAs("results/ObsLimitOnNSig_m0_m12.png");
   
   c1->SetLogz(0);
   // Expected Exclusion in M0 - M1/2
   TH2F*hexpexcl = new TH2F("expexcl",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Expected Exclusion",
                     100,0,1009.9,35,100,450);
   plotTools->Area(hexpexcl, Mzero, Mhalf, ExpExclusion);
   hexpexcl->Draw("colz");
   c1->SaveAs("results/ExpExclusion_m0_m12.png");
   
   // Observed Exclusion in M0 - M1/2
   TH2F*hobsexcl = new TH2F("obsexcl",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Observed Exclusion",
                     100,0,1009.9,35,100,450);
   plotTools->Area(hobsexcl, Mzero, Mhalf, ObsExclusion);
   hobsexcl->Draw("colz");
   c1->SaveAs("results/ObsExclusion_m0_m12.png");

   // Observed Exclusion in M0 - M1/2
   TH2F*hPLobsexcl = new TH2F("plobsexcl",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Observed Exclusion",
                     100,0,1009.9,35,100,450);
   plotTools->Area(hPLobsexcl, Mzero, Mhalf, PLObsExclusion);
   hPLobsexcl->Draw("colz");
   c1->SaveAs("results/PL_ObsExclusion_m0_m12.png");

   // Exclusion in M0 - M1/2
   TH2F*hexcl = new TH2F("hexcl",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Expected Exclusion",
                     100,0,1009.9,35,100,450);
   TGraph * gexpexcl = plotTools->GetContour(hexpexcl,3,1); 
   TGraph * gobsexcl = plotTools->GetContour(hobsexcl,3,1);
   //TH2F*hPLobsexcl=(TH2F*)hexcl->Clone();
   //plotTools->Area(hPLobsexcl, Mzero, Mhalf, PLObsExclusion);   
   TGraph * gPLobsexcl = plotTools->GetContour(hPLobsexcl,3,1); 
   TH2F*hPLexpexcl=(TH2F*)hexcl->Clone();
   plotTools->Area(hPLexpexcl, Mzero, Mhalf, PLExpExclusion);
   TGraph * gPLexpexcl = plotTools->GetContour(hPLexpexcl,3,1); 
   hexcl->Draw("colz");
   if (gobsexcl) gobsexcl->Draw("l");
   if (gexpexcl) gexpexcl->SetLineStyle(2);
   if (gexpexcl) gexpexcl->Draw("l");
   if (gPLobsexcl) gPLobsexcl->SetLineColor(2);
   if (gPLobsexcl) gPLobsexcl->Draw("l");
   if (gPLexpexcl) {gPLexpexcl->SetLineStyle(2);gPLexpexcl->SetLineColor(2);}
   if (gPLexpexcl) gPLexpexcl->Draw("l");
   TLegend * leg = new TLegend(0.45,0.78,0.85,0.89);
   leg->SetBorderSize(0);leg->SetFillColor(0);
   leg->AddEntry(gobsexcl,"Observed (TLimit, No Error)","l");
   leg->AddEntry(gexpexcl,"Expected (TLimit, No Error)","l");
   leg->AddEntry(gPLobsexcl,"Observed (ProfileLikelihood)","l");
   leg->AddEntry(gPLexpexcl,"Expected (ProfileLikelihood)","l");
   leg->Draw();
   c1->SaveAs("results/Exclusion_m0_m12.png");

   //plotTools->Area(h, Mzero, Mhalf, XsecOverObserved);
   //plotTools->Area(h, Mzero, Mhalf, XsecOverExpected);


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
   c1->SaveAs("results/Xsection_mSql_mGl.png");
   
   // Observed Limit in squark - gluino mass
   TH2F*hobslimit_qg = new TH2F("obslimit_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Observed Limit [pb]",
                     60,200,1400,50,200,1200);
   plotTools->Area(hobslimit_qg, MSquarkL, MGluino, ObsXsecLimit);
   hobslimit_qg->SetMinimum(0.01);
   hobslimit_qg->Draw("colz");
   c1->SaveAs("results/ObsLimit_mSql_mGl.png");
   
   // Expected Limit in squark - gluino mass
   TH2F*hexplimit_qg = new TH2F("explimit_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Expected Limit [pb]",
                     60,200,1400,50,200,1200);
   plotTools->Area(hexplimit_qg, MSquarkL, MGluino, ExpXsecLimit);
   hexplimit_qg->SetMinimum(0.01);
   hexplimit_qg->Draw("colz");
   c1->SaveAs("results/ExpLimit_mSql_mGl.png");
   
   // Signal Acceptance in squark - gluino mass
   TH2F*hsigacc_qg = new TH2F("sigacc_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; Signal Acceptance",
                     60,200,1400,50,200,1200);
   plotTools->Area(hsigacc_qg, MSquarkL, MGluino, SignalAcceptance);
   hsigacc_qg->SetMinimum(0.01);
   hsigacc_qg->SetMaximum(1.0);
   hsigacc_qg->Draw("colz");
   c1->SaveAs("results/SigAcc_mSql_mGl.png");
   
   // Exp. Limit on Number of Signal Events in squark - gluino mass
   c1->SetLogz(0);
   TH2F*hexplimitnsig_qg = new TH2F("explimitnsig_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL exp. limit signal events [# ]",
                     60,200,1400,50,200,1200);
   plotTools->Area(hexplimitnsig_qg, MSquarkL, MGluino, ExpNSignLimit);
   //hexplimitnsig_qg->SetMinimum(0.01);
   hexplimitnsig_qg->Draw("colz");
   c1->SaveAs("results/ExpLimitOnNSig_mSql_mGl.png");
   
   // Obs. Limit on Number of Signal Events in squark - gluino mass
   TH2F*hobslimitnsig_qg = new TH2F("obslimitnsig_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL obs. limit signal events [# ]",
                     60,200,1400,50,200,1200);
   plotTools->Area(hobslimitnsig_qg, MSquarkL, MGluino, ObsNSignLimit);
   //hobslimitnsig_qg->SetMinimum(0.01);
   hobslimitnsig_qg->Draw("colz");
   c1->SaveAs("results/ObsLimitOnNSig_mSql_mGl.png");
   
   c1->SetLogz(0);
   // Expected Exclusion in squark - gluino mass
   TH2F*hexpexcl_qg = new TH2F("expexcl_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Expected Exclusion",
                     60,200,1400,50,200,1200);
   plotTools->Area(hexpexcl_qg, MSquarkL, MGluino, ExpExclusion);
   hexpexcl_qg->Draw("colz");
   c1->SaveAs("results/ExpExclusion_mSql_mGl.png");
   
   // Observed Exclusion in squark - gluino mass
   TH2F*hobsexcl_qg = new TH2F("obsexcl_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Observed Exclusion",
                     60,200,1400,50,200,1200);
   plotTools->Area(hobsexcl_qg, MSquarkL, MGluino, ObsExclusion);
   //std::vector<TGraph *> vobsexcl_qg = plotTools->GetContours(hobsexcl_qg);
   hobsexcl_qg->Draw("colz");
   //for (std::vector<TGraph *>::iterator g=vobsexcl_qg.begin();g!=vobsexcl_qg.end();++g){
   //  if (! *g) continue;
   // // (*g)->Draw("l");
   //}
   c1->SaveAs("results/ObsExclusion_mSql_mGl.png");
   
   // Observed Exclusion in squark - gluino mass
   TH2F*hPLobsexcl_qg = new TH2F("plobsexcl_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Observed Exclusion",
                     60,200,1400,50,200,1200);
   plotTools->Area(hPLobsexcl_qg, MSquarkL, MGluino, PLObsExclusion);
   hPLobsexcl_qg->Draw("colz");
   c1->SaveAs("results/PL_ObsExclusion_mSql_mGl.png");
   
   // Exclusion in squark - gluino mass
   TH2F*hexcl_qg = new TH2F("hexcl_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Observed Exclusion",
                     60,200,1400,50,200,1200);
   TGraph * gexpexcl_qg = plotTools->GetContour(hexpexcl_qg,3,1); 
   if (gexpexcl_qg) gexpexcl_qg->SetLineStyle(2);
/*   TGraph * gobsexcl_qg = plotTools->GetContour(hobsexcl_qg,3,1);
   TH2F*hPLexpexcl_qg=(TH2F*)hexcl_qg->Clone();
   plotTools->Area(hPLexpexcl_qg, Mzero, Mhalf, PLExpExclusion);   
   TGraph * gPLexpexcl_qg = plotTools->GetContour(hPLexpexcl_qg,3,1); gPLexpexcl_qg->SetLineStyle(2);gPLexpexcl_qg->SetLineColor(2);
   //TH2F*hPLobsexcl_qg=(TH2F*)hexcl_qg->Clone();
   //plotTools->Area(hPLobsexcl_qg, Mzero, Mhalf, PLObsExclusion);
   TGraph * gPLobsexcl_qg = plotTools->GetContour(hPLobsexcl_qg,3,1); gPLobsexcl_qg->SetLineColor(2);
   hexcl_qg->Draw("colz");
   gobsexcl_qg->Draw("l");
   gexpexcl_qg->Draw("l");
   gPLobsexcl_qg->Draw("l");
   gPLexpexcl_qg->Draw("l");
   TLegend * leg_qg = new TLegend(0.45,0.78,0.85,0.89);
   leg_qg->SetBorderSize(0);leg_qg->SetFillColor(0);
   leg_qg->AddEntry(gobsexcl_qg,"Observed (TLimit, No Error)","l");
   leg_qg->AddEntry(gexpexcl_qg,"Expected (TLimit, No Error)","l");
   leg_qg->AddEntry(gPLobsexcl_qg,"Observed (ProfileLikelihood)","l");
   leg_qg->AddEntry(gPLexpexcl_qg,"Expected (ProfileLikelihood)","l");
   leg_qg->Draw();
*/
   c1->SaveAs("results/Exclusion_mSql_mGl.png");
   
   //c1->SaveAs("plot.pdf");


   //theApp.Run();
}


int main(int argc, char** argv)
{
  return plot(argc, argv);
}
