#include "plot.h"

#include "TRint.h"
#include "TROOT.h"
#include "TObjArray.h"
#include "TStyle.h"
#include "TChain.h"
#include "TFile.h"
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
#include "TGraph.h"

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>

#include "StyleSettings.h"
#include "PlotTools.h"
#include "GeneratorMasses.h"
#include "TemplateTanb10.h"

const static bool plotPNG = false;
TCanvas * c1 = 0;

void DrawPlot2D(PlotTools *PlotTool, TCanvas*canvas, TH2* h, const std::string& flag, const string& x, const std::string& y, const std::string& var, 
                const std::string& ztitel, double zmin, double zmax )
{
   TH2F *plot2D = (TH2F*)h->Clone();
   plot2D->SetMinimum(zmin);
   plot2D->SetMaximum(zmax);
   plot2D->GetZaxis()->SetTitle(ztitel.c_str());
   PlotTool->Area(plot2D, x, y, var);
   plot2D->GetZaxis()->SetTitleOffset(1.5);
   plot2D->Draw("colz");
   string namePlot = "results/" + flag + "_"+x+"_"+y+"_"+var;
   canvas->SaveAs((namePlot + ".pdf").c_str());
   if (plotPNG) canvas->SaveAs((namePlot + ".png").c_str());
 }
 
void DrawHist1D(PlotTools *PlotTool, TCanvas*canvas, const std::string& flag, const string& x, const std::string& y, const std::string& var, 
                const std::string& titel, int n)
{
   std::vector<double> vec;
   PlotTool->VectorOf(vec, var);
   if (vec.size()<=0) return;
   std::sort(vec.begin(), vec.end());
   double median  = vec[vec.size()/2];
   double quant16 = vec[vec.size()/6.25];
   double quant84 = vec[vec.size()/1.2];
   double xmin    = vec.front();
   double xmax    = vec.back();
   std::string name = flag+"_"+x+"_"+y+"_"+var+"_1D";
   TH1F *h1D = new TH1F(name.c_str(),";;number of cMSSM points", n, xmin-sqrt(fabs(xmin)), xmax+sqrt(fabs(xmax)));
   h1D->GetXaxis()->SetTitle(titel.c_str());
   PlotTool->Hist(h1D, var);
   TLegend * leg = new TLegend(0.19, 0.7, 0.4, 0.88);
   leg->SetBorderSize(0);
   leg->SetFillColor(0);
   leg->SetTextSize(0.025);
   { std::stringstream ss; ss << "16% quantile: " << setprecision(3) << quant16;
   leg->AddEntry((TObject*)0,ss.str().c_str(),""); }
   { std::stringstream ss; ss << "median : " << setprecision(3) << median;
   leg->AddEntry((TObject*)0,ss.str().c_str(),""); }
   { std::stringstream ss; ss << "84% quantile: " << setprecision(3) << quant84;
   leg->AddEntry((TObject*)0,ss.str().c_str(),""); }
   h1D->Draw("h");
   leg->Draw("same");
   std::string namePlot = "results/" + flag +"_"+x+"_"+y+"_"+var+"_1D";
   c1->SaveAs((namePlot + ".pdf").c_str());
   if (plotPNG) c1->SaveAs((namePlot + ".png").c_str());
}


void DrawStandardUncertaintyPlots(PlotTools *pt, const std::string& flag, const std::string& x, const std::string& y, TH2*h)
{
   c1->SetRightMargin(0.18);  
   //Log z-scale
   c1->SetLogz(1);
   DrawPlot2D(pt,c1,h,flag,x,y,"Luminosity",        "Luminosity [pb^{-1}]", 0, 6000.);
   DrawPlot2D(pt,c1,h,flag,x,y,"SignalStatUnc",     "Rel. Signal Statistical uncertainty", 0.01, 100.);
   DrawPlot2D(pt,c1,h,flag,x,y,"SignalSysUnc",      "Rel. Signal Systematic uncertainty", 0.01, 100.);
   DrawPlot2D(pt,c1,h,flag,x,y,"SignalPDFAccUnc",   "Rel. Signal PDF Accept. uncertainty", 0.01, 100.);
   DrawPlot2D(pt,c1,h,flag,x,y,"SignalPDFXsecUnc",  "Rel. Signal PDF cross-section uncertainty", 0.01, 100.);
   DrawPlot2D(pt,c1,h,flag,x,y,"SignalScaleUnc",    "Rel. Signal Scale uncertainty", 0.01, 100.);
   DrawPlot2D(pt,c1,h,flag,x,y,"SignalJesUpUnc",    "Rel. Signal JES (up) uncertainty", 0.01, 100.);
   DrawPlot2D(pt,c1,h,flag,x,y,"SignalJesDnUnc",    "Rel. Signal JES (dn) uncertainty", 0.01, 100.);
   DrawPlot2D(pt,c1,h,flag,x,y,"SignalJerUpUnc",    "Rel. Signal JER (up) uncertainty", 0.01, 100.);
   DrawPlot2D(pt,c1,h,flag,x,y,"SignalJerDnUnc",    "Rel. Signal JER (dn) uncertainty", 0.01, 100.);
   DrawPlot2D(pt,c1,h,flag,x,y,"SignalTotalUnc",    "Rel. Signal Total uncertainty", 0.01, 100.);
}

void DrawStandardPlots(PlotTools *pt, const std::string& flag, const std::string& x, const std::string& y, TH2*h)
{
   c1->SetRightMargin(0.18);
   
   //Log z-scale
   c1->SetLogz(1);
   DrawPlot2D(pt,c1,h,flag,x,y,"Xsection",          "NLO cross section [pb]", 0.0001, 100.);

   //Linear z-scale
   c1->SetLogz(0);
   DrawPlot2D(pt,c1,h,flag,x,y,"Acceptance",        "Acceptance", 0.0, 25.);

   //1D Histograms
   DrawHist1D(pt,c1,flag,x,y,"SignalStatUnc",	  "Rel. Signal Statistical uncertainty", 20);
   DrawHist1D(pt,c1,flag,x,y,"SignalSysUnc",	  "Rel. Signal Systematic uncertainty", 20);
   DrawHist1D(pt,c1,flag,x,y,"SignalPDFAccUnc",   "Rel. Signal PDF Accept. uncertainty", 20);
   DrawHist1D(pt,c1,flag,x,y,"SignalPDFXsecUnc",  "Rel. Signal PDF cross-section uncertainty", 20);
   DrawHist1D(pt,c1,flag,x,y,"SignalScaleUnc",    "Rel. Signal Scale uncertainty", 20);
   DrawHist1D(pt,c1,flag,x,y,"SignalJesUpUnc",    "Rel. Signal JES (up) uncertainty", 20);
   DrawHist1D(pt,c1,flag,x,y,"SignalJesDnUnc",    "Rel. Signal JES (dn) uncertainty", 20);
   DrawHist1D(pt,c1,flag,x,y,"SignalJerUpUnc",    "Rel. Signal JER (up) uncertainty", 20);
   DrawHist1D(pt,c1,flag,x,y,"SignalJerDnUnc",    "Rel. Signal JER (dn) uncertainty", 20);
   DrawHist1D(pt,c1,flag,x,y,"SignalTotalUnc",    "Rel. Signal Total uncertainty", 20);
}

void DrawStandardLimitPlots(PlotTools *pt, const std::string& flag, const std::string& x, const std::string& y, TH2*h)
{
   c1->SetRightMargin(0.18);
   
   //Log z-scale
   c1->SetLogz(1);
   DrawPlot2D(pt,c1,h,flag,x,y,"ObsXsecLimit",      "Observed cross section limit [pb]", 0.01, 100.);
   DrawPlot2D(pt,c1,h,flag,x,y,"ExpXsecLimit",      "Expected cross section limit [pb]", 0.01, 100.);
   DrawPlot2D(pt,c1,h,flag,x,y,"ObsNsignalLimit",   "Observed limit on number signal events", 9, 300.);
   DrawPlot2D(pt,c1,h,flag,x,y,"ExpNsignalLimit",   "Expected limit on number signal events", 9, 300.);
   DrawPlot2D(pt,c1,h,flag,x,y,"ObsAsympXsecLimit", "Observed asympt. cross section limit [pb]", 0.01, 100.);
   DrawPlot2D(pt,c1,h,flag,x,y,"ExpAsympXsecLimit", "Expected asympt. cross section limit [pb]", 0.01, 100.);

   //Linear z-scale
   c1->SetLogz(0);
   DrawPlot2D(pt,c1,h,flag,x,y,"ObsR",              "Observed R", 0.0, 2.);
   DrawPlot2D(pt,c1,h,flag,x,y,"ObsDivExp",         "Observed / Expected", 0.0, 2.);
}


void DrawStandardPlotsPerBin(PlotTools *pt, const std::string& flag, const std::string& x, const std::string& y, TH2*h)
{
   for (int bin=0; bin<nchannels; ++bin) {
      std::stringstream binlabel, binl;
      binlabel << ", bin "<<bin;
      binl     << bin;
      const std::string bl = binlabel.str();
      const std::string b  = binl.str();
      c1->SetLogz(1);
  
      DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b,          	"Signal event yield"+bl, 0.01, 100.);
      DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_acceptance", 	"Acceptance [%]"+bl, 0.01, 100.);
      DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_contamination", "Signal contamination [%]"+bl, 0.01, 100.);
      DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_stat_UP",	"Signal Stat. UP [%]"+bl, 0.01, 100.);	
      DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_stat_DN",	"Signal Stat. DN [%]"+bl, 0.01, 100.);	
      DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_syst_UP",	"Signal Syst. UP [%]"+bl, 0.01, 100.);	
      DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_syst_DN",	"Signal Syst. DN [%]"+bl, 0.01, 100.);	
      DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_AccPDF_UP",	"Signal Acc. PDF UP [%]"+bl, 0.01, 100.);	
      DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_AccPDF_DN",	"Signal Acc. PDF DN [%]"+bl, 0.01, 100.);	
      DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_ScaleUP",	"Signal Scale UP [%]"+bl, 0.01, 100.);	
      DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_Scale_DN",	"Signal Scale DN [%]"+bl, 0.01, 100.);	
      DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_XsecPDF_UP",	"Signal Xsec. PDF UP [%]"+bl, 0.01, 100.);   
      DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_XsecPDF_DN",	"Signal Xsec. PDF DN [%]"+bl, 0.01, 100.);   
      DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_trigger_UP",	"Signal trigger UP [%]"+bl, 0.01, 100.);   
      DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_trigger_DN", 	"Signal trigger DN [%]"+bl, 0.01, 100.);   
      DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_JES_UP",	"Signal JES UP [%]"+bl, 0.01, 100.);   
      DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_JES_DN", 	"Signal JES DN [%]"+bl, 0.01, 100.);   
      DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_JER_UP",	"Signal JER UP [%]"+bl, 0.01, 100.);   
      DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_JER_DN", 	"Signal JER DN [%]"+bl, 0.01, 100.);   

      //1D Histograms
      DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_contamination", "Signal contamination [%]"+bl, 20);
      DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_stat_UP",       "Signal Stat. UP [%]"+bl, 20);  
      DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_stat_DN",       "Signal Stat. DN [%]"+bl, 20);  
      DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_syst_UP",       "Signal Syst. UP [%]"+bl, 20);  
      DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_syst_DN",       "Signal Syst. DN [%]"+bl, 20);  
      DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_AccPDF_UP",     "Signal Acc. PDF UP [%]"+bl, 20);       
      DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_AccPDF_DN",     "Signal Acc. PDF DN [%]"+bl, 20);       
      DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_ScaleUP",       "Signal Scale UP [%]"+bl, 20);  
      DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_Scale_DN",      "Signal Scale DN [%]"+bl, 20);  
      DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_XsecPDF_UP",    "Signal Xsec. PDF UP [%]"+bl, 20);   
      DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_XsecPDF_DN",    "Signal Xsec. PDF DN [%]"+bl, 20);   
      DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_trigger_UP",    "Signal trigger UP [%]"+bl, 20);   
      DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_trigger_DN",    "Signal trigger DN [%]"+bl, 20);   
      DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_JES_UP",        "Signal JES UP [%]"+bl, 20);   
      DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_JES_DN",        "Signal JES DN [%]"+bl, 20);   
      DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_JER_UP",        "Signal JER UP [%]"+bl, 20);   
      DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_JER_DN",        "Signal JER DN [%]"+bl, 20);   
   }
   
}

void DrawExclusion(PlotTools *PlotTool, std::string flag, const std::string& x, const std::string& y, 
                   TH1*hp, TH1*h)
{
   //Require an observed CLs limit:
   PlotTool->Remove("ObsR", Compare::less, 0.0);
   PlotTool->FillEmptyPointsByInterpolation(x, y);
 
   //Xsection
   {TH2F *hxsec = (TH2F*)h->Clone();
   hxsec->GetZaxis()->SetTitle("Observed in/out");
   PlotTool->InOutFromR(hxsec, x, y, "ObsR", 3);
   FillEmptyPoints(hxsec,0.5);
   hxsec->GetZaxis()->SetTitleOffset(1.5);
   hxsec->SetMinimum(-0.5);hxsec->SetMaximum(1.5);
   std::vector<TGraph*> contours = GetContours(hxsec, 3);
   /// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
   hxsec->Draw("colz");
   int col = kBlue - 10;
   for (std::vector<TGraph*>::iterator cont = contours.begin(); cont != contours.end(); ++cont) {
   	   if (!*cont) continue;
   	   double x, y;
   	   (*cont)->GetPoint(0, x, y);
   	   (*cont)->SetLineColor(col);
   	   (*cont)->Draw("l");
   	   TLatex l;
   	   l.SetTextSize(0.04);
   	   l.SetTextColor(col++);
   	   char val[20];
   	   sprintf(val, "%d", (int) (cont - contours.begin()));
   	   l.DrawLatex(x, y, val);
   	   if (cont-contours.begin()>13) break;
   }
   //drawCmsPrel(PlotTool->SingleValue(Luminosity), METCut);
   string nameXsPlot = "results/" + flag + "_"+x+"_"+y+"_ObservedInOut";
   c1->SaveAs((nameXsPlot + ".pdf").c_str());
   if (plotPNG) c1->SaveAs((nameXsPlot + ".png").c_str());
   }

   {//Exclusion Contours
   c1->SetLogz(0);
   c1->SetRightMargin(0.08);
   c1->SetTopMargin(0.11);
   TH2F *hs = (TH2F*)h->Clone();
   TH2F *hplot = (TH2F*)hp->Clone();
   hs->GetZaxis()->SetTitle("");

   TGraph * gCLsObsExcl     = PlotTool->GetContour(hs, x, y, "ObsR", 3, 0, 1, 1);
   TGraph * gCLsObsAsym     = PlotTool->GetContour(hs, x, y, "ObsRasym", 3, 0, 2, 1);
   TGraph * gCLsExpAsym     = PlotTool->GetContour(hs, x, y, "ExpRasym", 3, 0, 2, 2);
   TGraph * gCLsExpExcl     = PlotTool->GetContour(hs, x, y, "ExpR", 3, 0, 1, 2);
   TGraph * gCLsExpExclm1   = PlotTool->GetContour(hs, x, y, "ExpRM1", 3, 0, 5, 2);
   TGraph * gCLsExpExclp1   = PlotTool->GetContour(hs, x, y, "ExpRP1", 3, 0, 5, 2);
   TGraph * gCLsObsTheoryp1 = PlotTool->GetContour(hs, x, y, "ObsRTheoryP1", 3, 0, 1, 3);
   TGraph * gCLsObsTheorym1 = PlotTool->GetContour(hs, x, y, "ObsRTheoryM1", 3, 0, 1, 3);
   TGraph * gCLsExpTheoryp1 = PlotTool->GetContour(hs, x, y, "ExpRTheoryP1", 3, 0, 1, 4);
   TGraph * gCLsExpTheorym1 = PlotTool->GetContour(hs, x, y, "ExpRTheoryM1", 3, 0, 1, 4);
   gCLsObsExcl->SetLineWidth(3);
   gCLsExpExcl->SetLineWidth(3);


   Cut(gCLsObsTheorym1,'y','<',260);
   Cut(gCLsObsTheoryp1,'y','<',220);
   Cut(gCLsExpTheorym1,'y','<',240);
   Cut(gCLsExpTheoryp1,'y','<',200);
   Cut(gCLsExpExclm1,'y','<',260);
   Cut(gCLsExpExclp1,'y','<',200);
   Cut(gCLsExpExcl,'y','<',240);
   Cut(gCLsObsExcl,'y','<',260);
   Cut(gCLsExpAsym,'y','<',200);
   Cut(gCLsObsAsym,'y','<',200);
   Cut(gCLsExpExcl,'x','>',2970);
   Cut(gCLsObsExcl,'x','>',2970);
   Cut(gCLsExpExclp1,'x','>',2780);
   Cut(gCLsObsTheoryp1,'x','>',2900);
   Cut(gCLsExpTheoryp1,'x','>',2800);

   Smooth(gCLsObsAsym,     30);
   Smooth(gCLsExpAsym,     30);
   Smooth(gCLsObsExcl,     30);
   Smooth(gCLsExpExcl,     30);
   Smooth(gCLsExpExclm1,   30);
   Smooth(gCLsExpExclp1,   30);
   Smooth(gCLsObsTheoryp1, 30);
   Smooth(gCLsExpTheoryp1, 30);
   Smooth(gCLsObsTheorym1, 30);
   Smooth(gCLsExpTheorym1, 30);

   TGraph * gCLs1Sigma = MakeBand(gCLsExpExclm1, gCLsExpExclp1);
   gCLs1Sigma->SetFillStyle(3001);
   gCLs1Sigma->SetFillColor(5);
   hplot->GetYaxis()->SetTitleOffset(1.55);
   hplot->GetXaxis()->SetTitleOffset(1.1);
   //Drawing the contours
   //TCanvas * c2 = GetLimitTemplateCanvas("GridTanb10_v1.root","GridCanvas");
   TCanvas * c2 = GetLimitTemplateCanvas(c1);
   c2->cd();
   hplot->Draw("");
   gCLs1Sigma->Draw("f");
   gCLsObsExcl->Draw("l");
   gCLsExpExcl->Draw("l");
   //gCLsObsAsym->Draw("l");
   //gCLsExpAsym->Draw("l");
   //Legends
   {TLegend * leg = new TLegend(0.55, 0.66, 0.93, 0.8);
   leg->SetBorderSize(0);
   leg->SetFillColor(0);
   //leg->SetFillStyle(4000);
   leg->SetTextSize(0.03);
   leg->SetHeader("CMS, 4.98 fb^{-1}");
   leg->AddEntry(gCLsObsExcl, "Observed, NLO", "l");
   TH1F* legExp = (TH1F*)gCLs1Sigma->Clone();
   legExp->SetLineStyle(gCLsExpExcl->GetLineStyle());
   legExp->SetLineColor(gCLsExpExcl->GetLineColor());
   legExp->SetLineWidth(gCLsExpExcl->GetLineWidth());
   leg->AddEntry(legExp, "Expected #pm 1#sigma exp. unc.", "lf");
   //leg->AddEntry(gCLsObsAsym, "Obs. CLs asymptotic, NLO", "l");
   //leg->AddEntry(gCLsExpAsym, "Exp. CLs asymptotic, NLO", "l");
   leg->Draw();
   }
   gPad->RedrawAxis();
   string nameExcl = "results/"+ flag + "_"+x+"_"+y+"_Exclusion_";
   c2->SaveAs((nameExcl + ".pdf").c_str());
   if (plotPNG) c2->SaveAs((nameExcl + ".png").c_str());
   //delete c2;
   
   //Draw Exclusion Plot again, but now without asymptotic limits, but with old RA2 limits
   //Drawing the contours
   //TCanvas * c3 = GetLimitTemplateCanvas("GridTanb10_v1.root","GridCanvas");
   TCanvas * c3 = GetLimitTemplateCanvas(c1);
   c3->cd();
   //hplot->Draw("");
   gCLs1Sigma->Draw("f");
   gCLsObsExcl->Draw("l");
   gCLsExpExcl->Draw("l");
   //gCLsObsTheoryp1->Draw("l");
   //gCLsExpTheoryp1->Draw("l");
   //gCLsObsTheorym1->Draw("l");
   //gCLsExpTheorym1->Draw("l");

   TGraph * RA2_36pb = RA2Observed_36pb();
   TGraph * RA2_1fb = RA2Observed_1fb();
   RA2_36pb->Draw("l");
   //RA2_1fb->Draw("l");

   //The Legends
   {TLegend * leg = new TLegend(0.40, 0.58, 0.73, 0.81);
   leg->SetBorderSize(0);leg->SetFillColor(0);
   //leg->SetFillStyle(4000);
   leg->SetTextFont(42);leg->SetTextSize(0.03);
   leg->SetHeader("4.98 fb^{-1}, #sqrt{s}=7 TeV");
   leg->AddEntry(gCLsObsExcl, "Observed", "l");
   leg->AddEntry(gCLsObsTheoryp1, "Observed #pm 1#sigma theo. unc.", "l");
   TH1F* legExp = (TH1F*)gCLs1Sigma->Clone();legExp->SetLineStyle(gCLsExpExcl->GetLineStyle());
   legExp->SetLineColor(gCLsExpExcl->GetLineColor());legExp->SetLineWidth(gCLsExpExcl->GetLineWidth());
   leg->AddEntry(legExp, "Expected #pm 1#sigma exp. unc.", "lf");
   leg->AddEntry(gCLsExpTheoryp1, "Expected #pm 1#sigma theo. unc.", "l");
   //leg->AddEntry(RA2_1fb,  "Observed 1 fb^{-1}", "l");
   leg->AddEntry(RA2_36pb, "Observed 36 pb^{-1}", "l");
   leg->Draw();
   }
   gPad->RedrawAxis();
   nameExcl = "results/" + flag + "_"+x+"_"+y+"_Exclusion_withOldRA2";
   c3->SaveAs((nameExcl + ".pdf").c_str());
   if (plotPNG) c3->SaveAs((nameExcl + ".png").c_str());
   //delete c3;
   
   }
   c1->cd();
}

void GetPlotTools(PlotTools*& plotTools, std::string filename, std::string GeneratorFile="", unsigned factor=0)
{
  Events * events = new Events();
  ReadEvents(*events, filename);
  
  //plotting helper functions
  plotTools = new PlotTools(events);

  //Require an observed CLs limit:
  //plotTools->Remove("ObsR", Compare::less, 0.0);


//  plotTools->FillEmptyPointsByInterpolation("Mzero", "Mhalf");


  //Make grid in Mzero, Mhalf finer by factors of 2 by linear interpolation
  for (int i=2; i<=factor; i*=2)
    plotTools->ExpandGrid("Mzero", "Mhalf");
  // New 'pseudo' points are added, therefore the binning of all plots has to be made finer by a factor
  // of 2 in x and y for each "ExpandGrid
  
  //Add generator information of particles masses if a file is given
  if (GeneratorFile!="") {
    std::vector<GeneratorMasses> GenMasses;
    ReadGeneratorMasses(GenMasses, GeneratorFile);
    Match( GenMasses, *events);
  }  
} 


int plot(int argc, char** argv) {
  util::StyleSettings::paperNoTitle();
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadBottomMargin(0.18);
  gStyle->SetPadRightMargin(0.18);

  c1 = new TCanvas("c1", "c1", 800, 600);
  c1->cd();


  //m0 m1/2 stuff //////////////////////////////////////////////////////////////////////////////
  PlotTools * PlotTool;
  GetPlotTools(PlotTool, "2012-03-28-00-09-cMSSM_V9/filelist.txt", "masses_cMSSM_tb10_A0_Mu1.dat");
  //GetPlotTools(PlotTool, "cMSSM/filelist.txt");

  TH2F h_plot("h",";m_{0} (GeV/c^{2}); m_{1/2} (GeV/c^{2}); cross section [pb]",
          148,50, 3010, 47, 90, 1030);

  DrawStandardPlots(      PlotTool, "cMSSM", "Mzero", "Mhalf", &h_plot);
  //DrawStandardPlotsPerBin(PlotTool, "cMSSM", "Mzero", "Mhalf", &h_plot);


  TH2F h1("h1",";m_{0} (GeV/c^{2}); m_{1/2} (GeV/c^{2}); cross section [pb]",
          145,110, 3010, 45, 90, 990);

  DrawExclusion(PlotTool,"cMSSM","Mzero","Mhalf",&h_plot,&h1); //removes points, which have no limits and fills the gaps by interpolation
  DrawStandardLimitPlots(      PlotTool, "cMSSM", "Mzero", "Mhalf", &h_plot);

/* 
  
  //TH1F sc("sc",";m_{0} (GeV/c^{2}); relative to background expectation",100,50, 3000);
  //PlotTool->Keep(ExpRasym, 1.0, 0.05);
  //Draw1DSignalContamination(PlotTool,"PlotTool",Mzero,&sc);


  //msq, mgl stuff /////////////////////////////////////////////////////////////////////////////
  PlotTools<SusyScan> *PlotTool_2;
  //GetPlotTools(PlotTool_2, "2011-11-29-21-44-PlotTool_Nov29/filelist.txt", "NewGenScan_tb10.dat", 4);
  //GetPlotTools(PlotTool_2, "2012-02-18-21-27-PlotTool_NonClosure2Pfit/filelist.txt", "masses_PlotTool_tb10_A0_Mu1.dat", 2);
  GetPlotTools(PlotTool_2, "2012-02-18-21-29-PlotTool_NonClosure2Pfit/filelist.txt", "masses_PlotTool_tb10_A0_Mu1.dat", 1);

  TH2F h_sq_gl("h_sq_gl","; m_{#tilde{g}} (GeV); m_{#tilde{q}} (GeV); cross section [pb]",
          30, 0, 1600, 90, 0, 1600);
  
  TH2F h("hqg",";m_{#tilde{g}} (GeV); m_{#tilde{q}} (GeV); cross section [pb]",
          30, 200, 1600, 80, 200, 1900);
  DrawStandardPlots(PlotTool_2,"PlotTool_GluinoSquark",Mgluino,Msquark,&h_sq_gl);
  DrawExclusionGlSq(PlotTool_2,"PlotTool_GluinoSquark",Mgluino,Msquark,&h_sq_gl,&h);


*/


}

int main(int argc, char** argv) {
	return plot(argc, argv);
}
