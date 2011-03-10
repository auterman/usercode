#include "plot.h"
#include "SusyScan.h"
#include "GeneratorMasses.h"
#include "PlotTools.h"
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

   /*
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
   */
   util::StyleSettings::paper();
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
   //genpoints->Fill("limits_MHT_tb50_20110303/filelist.txt"); 
   genpoints->Fill("limits_MHT_20110310/filelist_tb50.txt"); 

   TheLimits * genpointsHT = new TheLimits();
   //genpointsHT->Fill("limits_HT_tb50_20110303/filelist.txt"); 
   genpointsHT->Fill("limits_HT_20110310/filelist_tb50.txt"); 

   
   //Replace read limits with specific numbers
   //genpoints->OverwriteLimits("ABCD_MHT");
   
   genpoints->FillGeneratorMasses("GenScan_tb50.dat");
   genpoints->match();
   genpointsHT->FillGeneratorMasses("GenScan_tb50.dat");
   genpointsHT->match();

   //the plotting ----------------------------------------------------------------------
   //plotting helper functions
   PlotTools<SusyScan> * plotTools = new PlotTools<SusyScan>(genpoints->GetScan());
   PlotTools<SusyScan> * plotToolsHT = new PlotTools<SusyScan>(genpointsHT->GetScan());
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
                     40,200,600,26,140,400);
   plotTools->Area(hxsec, Mzero, Mhalf, Xsection);
   hxsec->SetMinimum(0.01);
   //sq500->Draw();
   //gl500->Draw();
   hxsec->Draw("colz");
   c1->SaveAs("results_tb50/Xsection_m0_m12_tb50.pdf");
   std::string wait;
   //std::cin>>wait;

   // Observed Limit in M0 - M1/2
   TH2F*hobslimit = new TH2F("obslimit",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Observed Limit [pb]",
                     40,200,600,26,140,400);
   plotTools->Area(hobslimit, Mzero, Mhalf, ObsXsecLimit);
   hobslimit->SetMinimum(0.01);
   hobslimit->Draw("colz");
   c1->SaveAs("results_tb50/ObsLimit_m0_m12_tb50.pdf");
   
   // Expected Limit in M0 - M1/2
   TH2F*hexplimit = new TH2F("explimit",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Expected Limit [pb]",
                     40,200,600,26,140,400);
   plotTools->Area(hexplimit, Mzero, Mhalf, ExpXsecLimit);
   hexplimit->SetMinimum(0.01);
   hexplimit->Draw("colz");
   c1->SaveAs("results_tb50/ExpLimit_m0_m12_tb50.pdf");
   
   // Signal Acceptance in M0 - M1/2   
   TH2F*hsigacc = new TH2F("sigacc",";m_{0} [GeV]; m_{1/2} [GeV]; Signal Acceptance",
                     40,200,600,26,140,400);
   plotTools->Area(hsigacc, Mzero, Mhalf, SignalAcceptance);
   hsigacc->SetMinimum(0.01);
   hsigacc->SetMaximum(1.0);
   hsigacc->Draw("colz");
   chi100->Draw();
   cha200->Draw();
   gl500 ->Draw();
   sq500 ->Draw();
   c1->SaveAs("results_tb50/SigAcc_m0_m12_tb50.pdf");
   
   // Exp. Limit on Number of Signal Events in M0 - M1/2
   c1->SetLogz(0);
   TH2F*hexplimitnsig = new TH2F("explimitnsig",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL exp. limit signal events [# ]",
                     40,200,600,26,140,400);
   plotTools->Area(hexplimitnsig, Mzero, Mhalf, ExpNSignLimit);
   hexplimitnsig->SetMinimum(0.0);
   hexplimitnsig->SetMaximum(20);
   hexplimitnsig->Draw("colz");
   c1->SaveAs("results_tb50/ExpLimitOnNSig_m0_m12_tb50.pdf");
   
   // Charged LSP in M0 - M1/2
   c1->SetRightMargin ( 0.15 );
   c1->SetLogz(0);
   TH2F*hchargedLSP = new TH2F("explimitnsig",";m_{0} [GeV]; m_{1/2} [GeV]; Charged LSP",
                     40,200,600,26,140,400);
   plotTools->Area(hchargedLSP, Mzero, Mhalf, ChargedLSP);
   //hchargedLSP->SetMinimum(-0.01);
   //hchargedLSP->SetMaximum(1.1);
   hchargedLSP->Draw("colz");
   c1->SaveAs("results_tb50/ChargedLSP_m0_m12_tb50.pdf");
   
   TH2F*hnLSP = new TH2F("explimitnsig",";m_{0} [GeV]; m_{1/2} [GeV]; Neutral LSP",
                     40,200,600,26,140,400);
   plotTools->Area(hnLSP, Mzero, Mhalf, MChi1);
   //hchargedLSP->SetMinimum(-0.01);
   //hchargedLSP->SetMaximum(1.1);
   hnLSP->Draw("colz");
   c1->SaveAs("results_tb50/NeutralLSP_m0_m12_tb50.pdf");
   
   // Obs. Limit on Number of Signal Events in M0 - M1/2
   TH2F*hobslimitnsig = new TH2F("obslimitnsig",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL obs. limit signal events [# ]",
                     40,200,600,26,140,400);
   plotTools->Area(hobslimitnsig, Mzero, Mhalf, ObsNSignLimit);
   hobslimitnsig->SetMinimum(0.0);
   hobslimitnsig->SetMaximum(20);
   hobslimitnsig->Draw("colz");
   c1->SaveAs("results_tb50/ObsLimitOnNSig_m0_m12_tb50.pdf");
   
   c1->SetLogz(0);
   // Expected Exclusion in M0 - M1/2
   TH2F*hexpexcl = new TH2F("expexcl",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Expected Exclusion",
                     40,200,600,26,140,400);
   plotTools->Area(hexpexcl, Mzero, Mhalf, ExpExclCL);
   hexpexcl->Draw("colz");
   c1->SaveAs("results_tb50/ExpExclusion_m0_m12_tb50.pdf");
   
   // Observed Exclusion in M0 - M1/2
   TH2F*hobsexcl = new TH2F("obsexcl",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Observed Exclusion",
                     40,200,600,26,140,400);
   plotTools->Area(hobsexcl, Mzero, Mhalf, ObsExclCL);
   hobsexcl->Draw("colz");
   c1->SaveAs("results_tb50/ObsExclusion_m0_m12_tb50.pdf");

   // Observed Exclusion in M0 - M1/2
   TH2F*hPLobsexcl = new TH2F("plobsexcl",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Observed Exclusion",
                     40,200,600,26,140,400);
   plotTools->Area(hPLobsexcl, Mzero, Mhalf, PLObsExclusion);
   hPLobsexcl->Draw("colz");
   c1->SaveAs("results_tb50/PL_ObsExclusion_m0_m12_tb50.pdf");

   c1->SetRightMargin ( 0.1 );
   // TestContours in M0 - M1/2
   TH2F*texcl = new TH2F("texcl",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Expected Exclusion",
                     40,200,600,26,140,400);
   TH2F*texpexcl=(TH2F*)texcl->Clone();
   plotToolsHT->Area(texpexcl, Mzero, Mhalf, NLOExpExclCLp1sigma);
   std::vector<TGraph*> contours = plotTools->GetContours(texpexcl,3); 
   texpexcl->Draw("colz");
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
   c1->SaveAs("results_tb50/ExclusionTestContours_m0_m12_tb50.pdf");


   // Exclusion in M0 - M1/2
   TH2F*hexcl = new TH2F("hexcl",";m_{0} [GeV]; m_{1/2} [GeV]; 95% CL Expected Exclusion",
                     40,200,600,26,140,450);
   TH2F*hs = new TH2F("hs","",40,200,600,26,140,400);
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
   TGraph * gCLsObsExclHT    = plotToolsHT->GetContour(hs,Mzero,Mhalf,NLOObsExclCL,	  3,0, 1,1); 
   TGraph * gCLsExpExclHT_LO = plotToolsHT->GetContour(hs,Mzero,Mhalf,ExpExclCL,	  3,0, 1,4); 
   TGraph * gCLsObsExclHT_LO = plotToolsHT->GetContour(hs,Mzero,Mhalf,ObsExclCL,	  3,0, 2,1); 
   TGraph * gCLsExpExclHTm1  = plotToolsHT->GetContour(hs,Mzero,Mhalf,NLOExpExclCLm1sigma,3,0, 5,2); 
   TGraph * gCLsExpExclHTp1  = plotToolsHT->GetContour(hs,Mzero,Mhalf,NLOExpExclCLp1sigma,3,0, 5,2); 
   TGraph * gCLsExpExclMHTm1 = plotTools  ->GetContour(hs,Mzero,Mhalf,NLOExpExclCLm1sigma,  3,0, 5,2); 
   TGraph * gCLsExpExclMHTp1 = plotTools  ->GetContour(hs,Mzero,Mhalf,NLOExpExclCLp1sigma,  3,0, 5,2); 
   TGraph * gCheck1 = (TGraph*)gobsexcl->Clone();
   TGraph * gCheck2 = (TGraph*)gexpexcl->Clone();
   Smooth( gCLsObsExclHT, 27); gCLsObsExclHT->SetLineWidth( 3 );
   Smooth( gCLsExpExclHT, 27 ); gCLsExpExclHT->SetLineWidth( 3 );
   Smooth( gCLsObsExclHT_LO, 27); gCLsObsExclHT_LO->SetLineWidth( 2 );
   Smooth( gCLsExpExclHT_LO, 27 ); gCLsExpExclHT_LO->SetLineWidth( 3 );
   Smooth( gexpexcl, 27 );
   Smooth( gobsexcl_LO, 27 );gobsexcl_LO->SetLineWidth( 2 );
   //Smooth( gobsexcl, 27 );
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
   TGraph * CLsObsNLO = plotToolsHT->ChooseBest(gCLsObsExclHT,gobsexcl, gCLsObsExclHT,gobsexcl);
   TGraph * CLsObsLO  = plotToolsHT->ChooseBest(gCLsObsExclHT_LO,gobsexcl_LO, gCLsObsExclHT_LO,gobsexcl_LO);
   TGraph * CLsExpNLO = plotToolsHT->ChooseBest(gCLsExpExclHT,gexpexcl, gCLsExpExclHT,gexpexcl);
   TGraph * CLsExpLO  = plotToolsHT->ChooseBest(gCLsExpExclHT_LO,gexpexcl_LO, gCLsExpExclHT_LO,gexpexcl_LO);
   TGraph * FCExpLO   = plotToolsHT->ChooseBest(gFCexpexclHT_LO,gFCexpexclMHT_LO,gFCexpexclHT_LO,gFCexpexclMHT_LO);
   TGraph * FCExpNLO  = plotToolsHT->ChooseBest(gFCexpexclHT,gFCexpexclMHT,gFCexpexclHT,gFCexpexclMHT);
   TGraph * FCObsNLO  = plotToolsHT->ChooseBest(gFCobsexclHT,gFCobsexclMHT,gFCobsexclHT,gFCobsexclMHT);
   TGraph * gCLsExpExclp1 = plotToolsHT->ChooseBest(gCLsExpExclHTp1,gCLsExpExclMHTp1, gCLsExpExclHTp1,gCLsExpExclMHTp1);
   TGraph * gCLsExpExclm1 = plotToolsHT->ChooseBest(gCLsExpExclHTm1,gCLsExpExclMHTm1, gCLsExpExclHTm1,gCLsExpExclMHTm1);
   TGraph * gCLsExp1Sigma    = MakeBand(gCLsExpExclp1, gCLsExpExclm1);
   hexcl->GetYaxis()->SetTitleOffset(1.3);
   hexcl->GetXaxis()->SetTitleOffset(0.92);
   hexcl->Draw("colz");
   //gCLsExpExclHTp1->Draw("l");
   //gCLsExpExclMHTp1->Draw("l");
   //gCLsExpExclHTm1->Draw("l");
   //gCLsExpExclMHTm1->Draw("l");
   gCLsExp1Sigma->Draw("f");
   //set old exclusion Limits
   TGraph* Atlas = Atlas_m0_m12_tb3_obs();
   Atlas->SetLineColor( 28 );
   TGraph* LEP_ch = set_lep_ch(50);
   TGraph* LEP_sl = set_lep_sl(50);//slepton curve
   TGraph* TEV_sg_cdf = set_tev_sg_cdf(50);//squark gluino cdf
   TGraph* TEV_sg_d0 = set_tev_sg_d0(50);//squark gluino d0
   //TGraph* TEV_tlp_cdf = set_tev_tlp_cdf(10);//trilepton cdf
   //TGraph* TEV_tlp_d0 = set_tev_tlp_d0(10);//trilepton d0
   TGraph* stau = set_tev_stau(50);//stau 
   TGraph* TEV_sn_d0_1 = set_sneutrino_d0_1(50);
   TGraph* TEV_sn_d0_2 = set_sneutrino_d0_2(50);
   TGraphErrors* First  = getObserved_NLO_tanBeta50();
   TGraphErrors* Second = getExpected_NLO_tanBeta50();//getLO_jetMultis();
   TGraphErrors* Third  = getLO_tanBeta50();
   First->GetXaxis()->SetRangeUser(0,505);
   First->GetYaxis()->SetRangeUser(80,500);
   First->GetXaxis()->SetTitle("m_{0} (GeV)");
   First->GetYaxis()->SetTitle("m_{1/2} (GeV)");
   TSpline3 *sFirst = new TSpline3("sFirst",First);
   sFirst->SetLineColor(kBlue);
   //sFirst->SetLineWidth(3);
   TSpline3 *sSecond = new TSpline3("sSecond",Second);
   sSecond->SetLineColor(kBlue);
   sSecond->SetLineStyle(2);
   //sSecond->SetLineWidth(3);
   TSpline3 *sThird = new TSpline3("sThird",Third);
   sThird->SetLineColor(kBlue);
   sThird->SetLineStyle(4);
   //sThird->SetLineWidth(3);
   //TEV_sn_d0_1->Draw("fsame");
   //TEV_sn_d0_2->Draw("fsame"); //only for tb=3
   LEP_ch->Draw("fsame");
   LEP_sl->Draw("fsame");
   //TEV_sg_cdf->Draw("fsame");
   //TEV_sg_cdf->Draw("lsame");
   //TEV_sg_d0->Draw("fsame");
   sFirst->Draw("same");
   //sSecond->Draw("same");
   //sThird->Draw("same");
   Atlas->Draw("c,same");
   TLatex b; b.SetTextSize(0.02); b.SetTextColor(1);
   TLatex ms; ms.SetTextSize(0.025); ms.SetTextFont(42);//ms.SetTextColor(12);
   ms.DrawLatex(490,458,"tan#beta=50, #mu>0, A_{0}=0"); 
   TLegend* legexp = new TLegend(0.69,0.77,0.93,0.92,NULL,"brNDC");
   legexp->SetFillColor(0);legexp->SetShadowColor(0);legexp->SetFillStyle(4000);legexp->SetTextFont(42);legexp->SetTextSize(0.025);legexp->SetBorderSize(0);
   //TEV_sg_cdf.SetLineColor(1);  
   legexp->SetHeader("CMS preliminary");
   //legexp->AddEntry(TEV_sg_cdf,"CDF  #tilde{#font[12]{g}}, #tilde{#font[12]{q}}, #scale[0.8]{tan#beta=5, #mu<0}","f"); 
   //legexp->AddEntry(TEV_sg_d0,"D0   #tilde{#font[12]{g}}, #tilde{#font[12]{q}}, #scale[0.8]{tan#beta=3, #mu<0}","f");  
   //ch_gr.SetLineColor(1); 
   legexp->AddEntry(LEP_ch,"LEP2   #tilde{#chi}_{1}^{#pm}","f");   
   //sl_gr.SetLineColor(1); 
   //legexp->AddEntry(LEP_sl,"LEP2   #tilde{#font[12]{l}}^{#pm}","f"); //NOT FOR tb=50!
   //if(tanbeta == 3) 
   //legexp->AddEntry(TEV_sn_d0_1,"D0  #chi^{#pm}_{1}, #chi^{0}_{2}","f");  
   legexp->AddEntry(sFirst, "CMS #alpha_{T}");
   legexp->AddEntry(Atlas,  "Atlas, #scale[0.8]{tan#beta=3}","l");
   legexp->Draw();
   
   CLsObsNLO->Draw("l");
   CLsObsLO->Draw("l");
   CLsExpNLO->Draw("l");
   //FCExpNLO->Draw("l");
   stau->Draw("fsame");
   b.DrawLatex( 220,420,"#tilde{#tau} LSP"); 

   //constant ssqquark and gluino lines
   TF1* lnsq[4];
   TF1* lngl[4];
   TLatex sqt; sqt.SetTextSize(0.02); sqt.SetTextAngle(-22);sqt.SetTextColor(kGray+2);
   sqt.DrawLatex(270,180,"#font[92]{#tilde{q}(500)GeV}");
   sqt.DrawLatex(327,255,"#font[92]{#tilde{q}(650)GeV}");
   sqt.DrawLatex(390,348,"#font[92]{#tilde{q}(800)GeV}");
   TLatex glt; glt.SetTextSize(0.02); sqt.SetTextAngle(-4); glt.SetTextColor(kGray+2);
   glt.DrawLatex(528,195,"#font[92]{#tilde{g}(500)GeV}");
   glt.DrawLatex(528,258,"#font[92]{#tilde{g}(650)GeV}");
   glt.DrawLatex(528,322,"#font[92]{#tilde{g}(800)GeV}");
   int tanBeta_=50;
   for(int i = 0; i < 4; i++){
    lnsq[i] = constant_squark(tanBeta_,i);
    lngl[i] = constant_gluino(tanBeta_,i);
    lngl[i]->Draw("same");   
    lnsq[i]->Draw("same");
   }
   TLegend * leg = new TLegend(0.38,0.8,0.73,0.92);
   leg->SetBorderSize(0);leg->SetFillColor(0);leg->SetFillStyle(4000);leg->SetTextFont(42);legexp->SetTextSize(0.025);
   TGraph * expLeg = (TGraph*)CLsExpNLO->Clone();expLeg->SetFillStyle(gCLsExp1Sigma->GetFillStyle());expLeg->SetFillColor(gCLsExp1Sigma->GetFillColor());
   leg->SetHeader("L_{int} = 36/pb, #sqrt{s} = 7 TeV");
   leg->AddEntry(CLsObsNLO,"Observed, NLO","l");
   leg->AddEntry(CLsObsLO, "Observed, LO","l");
   leg->AddEntry(expLeg,   "Expected #pm 1#sigma, NLO","lf");
   //leg->AddEntry(FCExpLO,  "Expected, FC, NLO","l");
   leg->Draw();
   gPad->RedrawAxis();
   c1->SaveAs("results/Exclusion_m0_m12_tb50.pdf");


   hexcl->Draw("");
   gCLsExp1Sigma->SetName("CLsNLO_Exp1SigmaUncertaintyTb50");
   CLsObsNLO->SetName("CLsNLO_ObservedTb50");
   CLsExpNLO->SetName("CLsNLO_ExpectedTb50");
   CLsExpLO->SetName("CLsLO_ExpectedTb50");
   gCLsExp1Sigma->Draw("l");
   CLsObsNLO->Draw("l");
   CLsExpNLO->Draw("l");
   CLsObsLO->Draw("l");
   c1->SaveAs("results/LimitContours_tb50.C");



   // Signal Contamination in M0 - M1/2
   c1->SetLogz(0);
   TH2F*hsigcont = new TH2F("sigcont",";m_{0} [GeV]; m_{1/2}; number of signal in bkg yield [events]",
                     40,200,600,26,140,400);
   plotToolsHT->Area(hsigcont, Mzero, Mhalf, SignalContamination);
   hsigcont->SetMinimum(0.01);
   hsigcont->SetMaximum(20);
   hsigcont->Draw("colz");
   if (gCLsObsExclHT)    gCLsObsExclHT->Draw("l");
   c1->SaveAs("results_tb50/SignalContamination_HT_m0_m12_tb50.pdf");
   
   // Signal Contamination in M0 - M1/2
   c1->SetLogz(0);
   TH2F*hsigcontMHT = new TH2F("sigcontMHT",";m_{0} [GeV]; m_{1/2}; number of signal in bkg yield [events]",
                     40,200,600,26,140,400);
   plotTools->Area(hsigcontMHT, Mzero, Mhalf, SignalContamination);
   hsigcontMHT->SetMinimum(0.01);
   hsigcontMHT->SetMaximum(20);
   hsigcontMHT->Draw("colz");
   if (gobsexcl)    gobsexcl->Draw("l");
   c1->SaveAs("results_tb50/SignalContamination_MHT_m0_m12_tb50.pdf");
   
   // Signal JEC Uncertainty  MHT   
   c1->SetLogz(0);
   TH2F*hsig_jec_mht = new TH2F("sigjecmht",";m_{0} [GeV]; m_{1/2} [GeV]; Signal JEC uncertainty",
                     40,200,600,26,140,400);
   plotTools->Area(hsig_jec_mht, Mzero, Mhalf, SignalUncertJEC);
   hsig_jec_mht->SetMinimum(0.0);
   hsig_jec_mht->SetMaximum(0.25);
   hsig_jec_mht->SetContour(10);
   hsig_jec_mht->Draw("colz");
   if (gobsexcl)    gobsexcl->Draw("l");
   c1->SaveAs("results_tb50/SigJEC_MHT_m0_m12_tb50.pdf");
   
   // Signal JEC Uncertainty  HT   
   c1->SetLogz(0);
   TH2F*hsig_jec_ht = new TH2F("sigjecht",";m_{0} [GeV]; m_{1/2} [GeV]; Signal JEC uncertainty",
                     40,200,600,26,140,400);
   plotTools->Area(hsig_jec_ht, Mzero, Mhalf, SignalUncertJEC);
   hsig_jec_ht->SetMinimum(0.0);
   hsig_jec_ht->SetMaximum(0.25);
   hsig_jec_ht->SetContour(10);
   hsig_jec_ht->Draw("colz");
   if (gCLsObsExclHT)    gCLsObsExclHT->Draw("l");
   c1->SaveAs("results_tb50/SigJEC_HT_m0_m12_tb50.pdf");
   

   // Signal MuIso Uncertainty  MHT   
   c1->SetLogz(0);
   TH2F*hsig_MuIso_mht = new TH2F("sigMuIsomht",";m_{0} [GeV]; m_{1/2} [GeV]; Signal #mu_{iso} uncertainty",
                     40,200,600,26,140,400);
   plotTools->Area(hsig_MuIso_mht, Mzero, Mhalf, SignalUncertMuIso);
   hsig_MuIso_mht->SetMinimum(0.0);
   hsig_MuIso_mht->SetMaximum(0.05);
   hsig_MuIso_mht->SetContour(5);
   hsig_MuIso_mht->Draw("colz");
   if (gobsexcl)    gobsexcl->Draw("l");
   c1->SaveAs("results_tb50/SigMuIso_MHT_m0_m12_tb50.pdf");
   
   // Signal MuIso Uncertainty  HT   
   c1->SetLogz(0);
   TH2F*hsig_MuIso_ht = new TH2F("sigMuIsoht",";m_{0} [GeV]; m_{1/2} [GeV]; Signal #mu_{iso} uncertainty",
                     40,200,600,26,140,400);
   plotTools->Area(hsig_MuIso_ht, Mzero, Mhalf, SignalUncertMuIso);
   hsig_MuIso_ht->SetMinimum(0.0);
   hsig_MuIso_ht->SetMaximum(0.05);
   hsig_MuIso_ht->SetContour(5);
   hsig_MuIso_ht->Draw("colz");
   if (gCLsObsExclHT)    gCLsObsExclHT->Draw("l");
   c1->SaveAs("results_tb50/SigMuIso_HT_m0_m12_tb50.pdf");
   

   // Signal kFactor Uncertainty  MHT   
   c1->SetLogz(0);
   TH2F*hsig_kFactor_mht = new TH2F("sigkFactormht",";m_{0} [GeV]; m_{1/2} [GeV]; Signal #mu_{iso} uncertainty",
                     40,200,600,26,140,400);
   plotTools->Area(hsig_kFactor_mht, Mzero, Mhalf, SignalUncertKfactor);
   hsig_kFactor_mht->SetMinimum(0.1);
   hsig_kFactor_mht->SetMaximum(0.2);
   hsig_kFactor_mht->SetContour(10);
   hsig_kFactor_mht->Draw("colz");
   if (gobsexcl)    gobsexcl->Draw("l");
   c1->SaveAs("results_tb50/SigkFactor_MHT_m0_m12_tb50.pdf");
   
   // Signal kFactor Uncertainty  HT   
   c1->SetLogz(0);
   TH2F*hsig_kFactor_ht = new TH2F("sigkFactorht",";m_{0} [GeV]; m_{1/2} [GeV]; Signal #mu_{iso} uncertainty",
                     40,200,600,26,140,400);
   plotTools->Area(hsig_kFactor_ht, Mzero, Mhalf, SignalUncertKfactor);
   hsig_kFactor_ht->SetMinimum(0.1);
   hsig_kFactor_ht->SetMaximum(0.2);
   hsig_kFactor_ht->SetContour(10);
   hsig_kFactor_ht->Draw("colz");
   if (gCLsObsExclHT)    gCLsObsExclHT->Draw("l");
   c1->SaveAs("results_tb50/SigkFactor_HT_m0_m12_tb50.pdf");
   

   //-----------------------------------------------------------------------------------
   c1->SetLogz(1);

   TGraph * mz500 = plotMasses->Line( MSquarkL, MGluino, Mzero, 500, 1);
   TGraph * mh250 = plotMasses->Line( MSquarkL, MGluino, Mhalf, 250, 1);

   // cross-section in squark - gluino mass
   TH2F*hxsec_qg = new TH2F("xsec_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; cross section [pb]",
                     40,200,900,30,200,900);
   plotTools->Area(hxsec_qg, MSquarkL, MGluino, Xsection);
   hxsec_qg->SetMinimum(0.01);
   hxsec_qg->Draw("colz");
   mz500->Draw();
   mh250->Draw();
   c1->SaveAs("results_tb50/Xsection_mSql_mGl_tb50.pdf");
   
   // Observed Limit in squark - gluino mass
   TH2F*hobslimit_qg = new TH2F("obslimit_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Observed Limit [pb]",
                     40,200,900,30,200,900);
   plotTools->Area(hobslimit_qg, MSquarkL, MGluino, ObsXsecLimit);
   hobslimit_qg->SetMinimum(0.01);
   hobslimit_qg->Draw("colz");
   c1->SaveAs("results_tb50/ObsLimit_mSql_mGl_tb50.pdf");
   
   // Expected Limit in squark - gluino mass
   TH2F*hexplimit_qg = new TH2F("explimit_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Expected Limit [pb]",
                     40,200,900,30,200,900);
   plotTools->Area(hexplimit_qg, MSquarkL, MGluino, ExpXsecLimit);
   hexplimit_qg->SetMinimum(0.01);
   hexplimit_qg->Draw("colz");
   c1->SaveAs("results_tb50/ExpLimit_mSql_mGl_tb50.pdf");
   
   // Signal Acceptance in squark - gluino mass
   TH2F*hsigacc_qg = new TH2F("sigacc_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; Signal Acceptance",
                     40,200,900,30,200,900);
   plotTools->Area(hsigacc_qg, MSquarkL, MGluino, SignalAcceptance);
   hsigacc_qg->SetMinimum(0.01);
   hsigacc_qg->SetMaximum(1.0);
   hsigacc_qg->Draw("colz");
   c1->SaveAs("results_tb50/SigAcc_mSql_mGl_tb50.pdf");
   
   // Exp. Limit on Number of Signal Events in squark - gluino mass
   c1->SetLogz(0);
   TH2F*hexplimitnsig_qg = new TH2F("explimitnsig_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL exp. limit signal events [# ]",
                     40,200,900,30,200,900);
   plotTools->Area(hexplimitnsig_qg, MSquarkL, MGluino, ExpNSignLimit);
   hexplimitnsig_qg->SetMinimum(0.0);
   hexplimitnsig_qg->SetMaximum(20);
   hexplimitnsig_qg->Draw("colz");
   c1->SaveAs("results_tb50/ExpLimitOnNSig_mSql_mGl_tb50.pdf");
   
   // Obs. Limit on Number of Signal Events in squark - gluino mass
   TH2F*hobslimitnsig_qg = new TH2F("obslimitnsig_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL obs. limit signal events [# ]",
                     40,200,900,30,200,900);
   plotTools->Area(hobslimitnsig_qg, MSquarkL, MGluino, ObsNSignLimit);
   hobslimitnsig_qg->SetMinimum(0.0);
   hobslimitnsig_qg->SetMaximum(20);
   hobslimitnsig_qg->Draw("colz");
   c1->SaveAs("results_tb50/ObsLimitOnNSig_mSql_mGl_tb50.pdf");
   
   c1->SetLogz(0);
   // Expected Exclusion in squark - gluino mass
   TH2F*hexpexcl_qg = new TH2F("expexcl_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Expected Exclusion",
                     40,200,900,30,200,900);
   plotTools->Area(hexpexcl_qg, MSquarkL, MGluino, ExpExclCL);
   hexpexcl_qg->Draw("colz");
   c1->SaveAs("results_tb50/ExpExclusion_mSql_mGl_tb50.pdf");
   
   // Observed Exclusion in squark - gluino mass
   TH2F*hobsexcl_qg = new TH2F("obsexcl_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Observed Exclusion",
                     40,200,900,30,200,900);
   plotTools->Area(hobsexcl_qg, MSquarkL, MGluino, ObsExclCL);
   //std::vector<TGraph *> vobsexcl_qg = plotTools->GetContours(hobsexcl_qg);
   hobsexcl_qg->Draw("colz");
   //for (std::vector<TGraph *>::iterator g=vobsexcl_qg.begin();g!=vobsexcl_qg.end();++g){
   //  if (! *g) continue;
   // // (*g)->Draw("l");
   //}
   c1->SaveAs("results_tb50/ObsExclusion_mSql_mGl_tb50.pdf");
   
   // TestContours in M0 - M1/2
   TH2F*texcl_qg = new TH2F("texcl_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Observed Exclusion",
                     40,200,900,30,200,900);
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
   c1->SaveAs("results_tb50/ExclusionTestContours_mSql_mGl_tb50.pdf");

   // Observed Exclusion in squark - gluino mass
   TH2F*hPLobsexcl_qg = new TH2F("plobsexcl_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Observed Exclusion",
                     40,200,900,30,200,900);
   plotTools->Area(hPLobsexcl_qg, MSquarkL, MGluino, PLObsExclusion);
   //hPLobsexcl_qg->Draw("colz");
   //c1->SaveAs("results_tb50/PL_ObsExclusion_mSql_mGl_tb50.pdf");
   


   // Exclusion in squark - gluino mass ----------------------------------------
   TH2F*hexcl_qg = new TH2F("hexcl_qg",";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; 95% CL Observed Exclusion",
                     40,200,900,30,200,900);
   TGraph * gexpexcl_qg         = plotTools->GetContour(hexcl_qg,MSquarkL,MGluino,  NLOExpExclCL,       3,0, 2,2); 
   TGraph * gexpexcl_qg_LO      = plotTools->GetContour(hexcl_qg,MSquarkL,MGluino,  ExpExclCL,       3,0, 2,4); 
   TGraph * gobsexcl_qg         = plotTools->GetContour(hexcl_qg,MSquarkL,MGluino,  NLOObsExclCL,       3,0, 2,1);
   TGraph * gMCMCexpexcl_qg     = plotToolsHT->GetContour(hexcl_qg,MSquarkL,MGluino,MCMCExpExclusion,3,0, 3,2); 
   TGraph * gMCMCobsexcl_qg     = plotToolsHT->GetContour(hexcl_qg,MSquarkL,MGluino,MCMCObsExclusion,3,0, 3,1); 
   TGraph * gCLsExpExclHT_qg    = plotToolsHT->GetContour(hexcl_qg,MSquarkL,MGluino,NLOExpExclCL,       3,0, 1,2); 
   TGraph * gCLsExpExclHT_qg_LO = plotToolsHT->GetContour(hexcl_qg,MSquarkL,MGluino,ExpExclCL,       3,0, 1,4); 
   TGraph * gCLsObsExclHT_qg    = plotToolsHT->GetContour(hexcl_qg,MSquarkL,MGluino,NLOObsExclCL,       3,0, 1,1); 
   TGraph * gCLsExpExclHTm1_qg  = plotToolsHT->GetContour(hexcl_qg,MSquarkL,MGluino,NLOExpExclCLm1sigma,3,0, 5,2); 
   TGraph * gCLsExpExclHTp1_qg  = plotToolsHT->GetContour(hexcl_qg,MSquarkL,MGluino,NLOExpExclCLp1sigma,3,0, 5,2); 
   TGraph * gCLsExp1Sigma_qg    = MakeBand(gCLsExpExclHTp1_qg, gCLsExpExclHTm1_qg);
   hexcl_qg->Draw("");
   //
   if (gCLsExp1Sigma_qg) gCLsExp1Sigma_qg->Draw("lf");   
   if (gobsexcl_qg)      gobsexcl_qg->Draw("l");
   if (gCLsObsExclHT_qg) gCLsObsExclHT_qg->Draw("l");
   if (gexpexcl_qg)      gexpexcl_qg->Draw("l");
   if (gexpexcl_qg_LO)   gexpexcl_qg_LO->Draw("l");
   if (gCLsExpExclHT_qg) gCLsExpExclHT_qg->Draw("l");
   if (gCLsExpExclHT_qg_LO) gCLsExpExclHT_qg_LO->Draw("l");
   //
   TLegend * leg_qg = new TLegend(0.45,0.78,0.85,0.89);
   leg_qg->SetBorderSize(0);leg_qg->SetFillColor(0);leg->SetTextFont(42);
   TGraph * expLeg_qg = (TGraph*)gCLsExpExclHT_qg->Clone();expLeg_qg->SetFillStyle(gCLsExp1Sigma_qg->GetFillStyle());expLeg_qg->SetFillColor(gCLsExp1Sigma_qg->GetFillColor());
   if (gobsexcl_qg) leg_qg->AddEntry(gobsexcl_qg,"NLO Observed (MHT, CLs)","l");
   if (gexpexcl_qg) leg_qg->AddEntry(gexpexcl_qg,"NLO Expected (MHT, CLs)","l");
   if (gexpexcl_qg_LO) leg_qg->AddEntry(gexpexcl_qg_LO,"LO Expected (MHT, CLs)","l");
   if (gCLsObsExclHT_qg) leg_qg->AddEntry(gCLsObsExclHT_qg,"NLO Observed (HT, CLs)","l");
   if (gCLsExpExclHT_qg) leg_qg->AddEntry(expLeg_qg,"NLO Expected (HT, CLs)","lf");
   if (gCLsExpExclHT_qg_LO) leg_qg->AddEntry(gCLsExpExclHT_qg_LO,"LO Expected (HT, CLs)","l");
   //if (gPLobsexcl_qg) leg_qg->AddEntry(gPLobsexcl_qg,"Observed (PL, RooStat)","l");
   //if (gPLexpexcl_qg) leg_qg->AddEntry(gPLexpexcl_qg,"Expected (PL, RooStat)","l");
   leg_qg->Draw();
   c1->SaveAs("results_tb50/Exclusion_mSql_mGl_tb50.pdf");
   //c1->SaveAs("plot_tb10.pdf");


   
   //c1->SaveAs("plot_tb50.pdf");
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
   c1->SaveAs("results_tb50/UncertaintyScan.pdf");

   //theApp.Run();
}


int main(int argc, char** argv)
{
  return plot(argc, argv);
}
