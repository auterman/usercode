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
   DrawPlot2D(pt,c1,h,flag,x,y,"Acceptance",        "Acceptance", 0.0, 30.);

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
      DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_theory_UP",	"Signal Theory UP [%]"+bl, 0.01, 100.);	
      DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_theory_DN",	"Signal Theory DN [%]"+bl, 0.01, 100.);	
      DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_trigger_UP",	"Signal trigger UP [%]"+bl, 0.01, 100.);   
      DrawPlot2D(pt,c1,h,flag,x,y,"signal_"+b+"_trigger_DN", 	"Signal trigger DN [%]"+bl, 0.01, 100.);   

      //1D Histograms
      DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_contamination", "Signal contamination [%]"+bl, 20);
      DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_stat_UP",       "Signal Stat. UP [%]"+bl, 20);  
      DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_stat_DN",       "Signal Stat. DN [%]"+bl, 20);  
      DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_syst_UP",       "Signal Syst. UP [%]"+bl, 20);  
      DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_syst_DN",       "Signal Syst. DN [%]"+bl, 20);  
      DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_theory_UP",     "Signal Acc. PDF UP [%]"+bl, 20);       
      DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_theory_DN",     "Signal Acc. PDF DN [%]"+bl, 20);       
      DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_trigger_UP",    "Signal trigger UP [%]"+bl, 20);   
      DrawHist1D(pt,c1,flag,x,y,"signal_"+b+"_trigger_DN",    "Signal trigger DN [%]"+bl, 20);   
   }
   
}

void DrawExclusion(PlotTools *PlotTool, std::string flag, const std::string& x, const std::string& y, 
                   TH1*hp, TH1*h)
{
   //Require an observed CLs limit:
   PlotTool->Remove("ObsR", Compare::less, 0.0);
   PlotTool->FillEmptyPointsByInterpolation1D(x, y);
 
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
   TGraph * gCLsExpExcl     = PlotTool->GetContour(hs, x, y, "ExpR", 3, 0, 1, 2);
   TGraph * gCLsExpExclm1   = PlotTool->GetContour(hs, x, y, "ExpRM1", 3, 0, 5, 2);
   TGraph * gCLsExpExclp1   = PlotTool->GetContour(hs, x, y, "ExpRP1", 3, 0, 5, 2);

   //The limit calculated *with* the theoretical uncertainties on the signal cross-section:
   PlotTools *  PT_withTheoUnc;
   GetPlotTools(PT_withTheoUnc, "2012-03-28-00-09-cMSSM_V9/filelist.txt");
   PT_withTheoUnc->Remove("ObsR", Compare::less, 0.0);
   PT_withTheoUnc->FillEmptyPointsByInterpolation(x, y);
   TGraph * gCLsObsTheory = PT_withTheoUnc->GetContour(hs, x, y, "ObsR", 3, 0, 1, 1);

   gCLsObsExcl->SetLineWidth(1);
   gCLsExpExcl->SetLineWidth(2);
   gCLsObsTheory->SetLineWidth(3);

   Cut(gCLsExpExclm1,'y','<',260);
   Cut(gCLsExpExclp1,'y','<',200);
   Cut(gCLsExpExcl,'y','<',240);
   Cut(gCLsObsExcl,'y','<',260);
   Cut(gCLsExpExcl,'x','>',2970);
   Cut(gCLsObsExcl,'x','>',2970);
   Cut(gCLsExpExclp1,'x','>',2780);
   Cut(gCLsObsTheory,'y','<',260);

   Smooth(gCLsObsExcl,     30);
   Smooth(gCLsExpExcl,     30);
   Smooth(gCLsExpExclm1,   30);
   Smooth(gCLsExpExclp1,   30);
   Smooth(gCLsObsTheory,   30);

   TGraph * gCLs1Sigma = MakeBand(gCLsExpExclm1, gCLsExpExclp1);
   TGraph * gCLsObs    = MakeBand(gCLsObsExcl,   gCLsObsTheory);
   gCLs1Sigma->SetFillStyle(3001);
   gCLs1Sigma->SetFillColor(5);
   gCLsObs->SetFillStyle(3001);
   gCLsObs->SetFillColor(kCyan+1);
   hplot->GetYaxis()->SetTitleOffset(1.55);
   hplot->GetXaxis()->SetTitleOffset(1.1);
   //Drawing the contours
   //TCanvas * c2 = GetLimitTemplateCanvas("GridTanb10_v1.root","GridCanvas");
   TCanvas * c2 = GetLimitTemplateCanvas(c1);
   c2->cd();
   TGraph * RA2_36pb = RA2Observed_36pb();
   TGraph * RA2_1fb = RA2Observed_1fb();
   RA2_36pb->Draw("l");
   //hplot->Draw("");
   gCLs1Sigma->Draw("f");
   gCLsObs->Draw("f");
   gCLsObsExcl->Draw("l");
   gCLsObsTheory->Draw("l");
   gCLsExpExcl->Draw("l");
   //gCLsObsAsym->Draw("l");
   //gCLsExpAsym->Draw("l");
   //Legends
   {TLegend * leg = new TLegend(0.35, 0.66, 0.7, 0.8);
   leg->SetBorderSize(0);
   leg->SetFillColor(0);
   //leg->SetFillStyle(4000);
   leg->SetTextSize(0.035);
   leg->SetTextFont(42);
   leg->SetHeader("#bf{CMS, 4.98 fb^{-1}, #sqrt{s} = 7 TeV}");
   TH1F* legObs = (TH1F*)gCLsObs->Clone();
   legObs->SetLineStyle(gCLsObsExcl->GetLineStyle());
   legObs->SetLineColor(gCLsObsExcl->GetLineColor());
   legObs->SetLineWidth(gCLsObsExcl->GetLineWidth());
   leg->AddEntry(legObs, "Obs. limit w. +1#sigma signal theory", "lf");
   TH1F* legExp = (TH1F*)gCLs1Sigma->Clone();
   legExp->SetLineStyle(gCLsExpExcl->GetLineStyle());
   legExp->SetLineColor(gCLsExpExcl->GetLineColor());
   legExp->SetLineWidth(gCLsExpExcl->GetLineWidth());
   leg->AddEntry(legExp, "Exp. limit #pm 1#sigma exp. (w/o signal theory)", "lf");
   leg->AddEntry(RA2_36pb, "Observed 36 pb^{-1}", "l");
   leg->Draw();
   }
   TMarker lm5(230,360,29);
   lm5.SetMarkerColor(14);
   lm5.Draw("same");
   TLatex tex(245,365,"#font[42]{LM5}");
   tex.SetTextColor(12);
   tex.SetTextSize(0.03);
   tex.Draw();

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
   gCLsObsExcl->SetLineWidth(3);
   gCLsObsExcl->Draw("l");
   gCLsExpExcl->Draw("l");

   //The Legends
   {TLegend * leg = new TLegend(0.40, 0.58, 0.73, 0.81);
   leg->SetBorderSize(0);leg->SetFillColor(0);
   //leg->SetFillStyle(4000);
   leg->SetTextFont(42);leg->SetTextSize(0.03);
   leg->SetHeader("#bf{4.98 fb^{-1}, #sqrt{s}=7 TeV, #sqrt{s} = 7 TeV}");
   leg->AddEntry(gCLsObsExcl, "Observed", "l");
   TH1F* legExp = (TH1F*)gCLs1Sigma->Clone();legExp->SetLineStyle(gCLsExpExcl->GetLineStyle());
   legExp->SetLineColor(gCLsExpExcl->GetLineColor());legExp->SetLineWidth(gCLsExpExcl->GetLineWidth());
   leg->AddEntry(legExp, "Expected #pm 1#sigma exp. unc.", "lf");
   leg->Draw();
   }
   gPad->RedrawAxis();
   nameExcl = "results/" + flag + "_"+x+"_"+y+"_Exclusion_woObsBand";
   c3->SaveAs((nameExcl + ".pdf").c_str());
   if (plotPNG) c3->SaveAs((nameExcl + ".png").c_str());
   //delete c3;
   
   }
   c1->cd();
}

void DrawExclusionGlSq(PlotTools *PlotTool, std::string flag, const std::string& x, const std::string& y, TH1*hp,TH1*h)
{
   //Xsection
   {TH2F *hxsec = (TH2F*)h->Clone();
   hxsec->GetZaxis()->SetTitle("Observed in/out");
   PlotTool->InOutFromR(hxsec, x, y, "ObsR", 3);

   hxsec->GetZaxis()->SetTitleOffset(1.5);
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
   string nameXsPlot = "results/"  + flag + "_"+x+"_"+y+"_ObservedInOut";
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
   TGraph * gCLsExpExcl     = PlotTool->GetContour(hs, x, y, "ExpR", 3, 0, 1, 2);
   TGraph * gCLsExpExclm1   = PlotTool->GetContour(hs, x, y, "ExpRM1", 3, 0, 5, 2);
   TGraph * gCLsExpExclp1   = PlotTool->GetContour(hs, x, y, "ExpRP1", 3, 0, 5, 2);
   //The limit calculated *with* the theoretical uncertainties on the signal cross-section:
   PlotTools *  PT_withTheoUnc;
   GetPlotTools(PT_withTheoUnc, "2012-03-28-00-09-cMSSM_V9/filelist.txt", "masses_cMSSM_tb10_A0_Mu1.dat");
   PT_withTheoUnc->Remove("ObsR", Compare::less, 0.0);
   PT_withTheoUnc->FillEmptyPointsByInterpolation("Mzero","Mhalf");
   TGraph * gCLsObsTheory = PT_withTheoUnc->GetContour(hs, x, y, "ObsR", 3, 0, 1, 1);

   gCLsExpExclp1->SetLineColor(5);
   gCLsObsExcl->SetLineWidth(1);
   gCLsObsTheory->SetLineWidth(3);
   gCLsExpExcl->SetLineWidth(3);   
   TGraph * gCLsObsExcl_unsmoothed = (TGraph*)gCLsObsExcl->Clone();

   Smooth(gCLsObsTheory, 15);
   Smooth(gCLsObsExcl,   15);
   Smooth(gCLsExpExcl,   15);
   Smooth(gCLsExpExclm1, 15);
   Smooth(gCLsExpExclp1, 15);

   Cut(gCLsObsExcl,  'y','<',1000);
   Cut(gCLsExpExcl,  'y','<',1000);
   Cut(gCLsObsTheory,'y','<',1000);
   Cut(gCLsExpExclp1,'y','<',1000);
   Cut(gCLsExpExclm1,'y','<',1000);
   
   TGraph * gCLs1Sigma = MakeBand(gCLsExpExclm1, gCLsExpExclp1);
   gCLs1Sigma->SetFillStyle(3001);
   gCLs1Sigma->SetFillColor(5);
   TGraph * gCLsObs    = MakeBand(gCLsObsExcl,   gCLsObsTheory);
   gCLs1Sigma->SetFillStyle(3001);
   gCLs1Sigma->SetFillColor(5);
   gCLsObs->SetFillStyle(3001);
   gCLsObs->SetFillColor(kCyan+1);
   hplot->GetYaxis()->SetTitleOffset(1.55);
   hplot->GetXaxis()->SetTitleOffset(1.1);
      
   //Drawing the contours
   hplot->Draw("");
   gCLs1Sigma->Draw("f");
   gCLsObs->Draw("f");
   //gCLsExpExclm1->SetLineColor(7);gCLsExpExclm1->Draw("l");
   //gCLsExpExclp1->SetLineColor(3);gCLsExpExclp1->Draw("l");
   gCLsObsExcl->Draw("l");
//   gCLsObsExcl_unsmoothed->Draw("l");
   gCLsObsTheory->Draw("l");
   gCLsExpExcl->Draw("l");
   //cOVER-UP
   TGraph *CoverUp = new TGraph(4);
   CoverUp->SetPoint(0,0,0);
   CoverUp->SetPoint(1,550,0);
   CoverUp->SetPoint(2,550,1900);
   CoverUp->SetPoint(3,0,1900);
   CoverUp->SetPoint(4,0,0);
   CoverUp->SetFillColor(0);
   CoverUp->Draw("f");


////////////////////////////////////////////
   TGraph* lep= gl_LEP();
   TGraph* tev= sq_TEV();
   TGraph* cdf= sq_CDF();
   TGraph* dez= sq_DEZ();
   TGraph* nosol= glsq_NoSol();
   TGraph* nosol_aux= glsq_NoSol_aux();
   nosol_aux->Draw("f");
   dez->Draw("f");
   cdf->Draw("f");
   tev->Draw("f");
   lep->Draw("f");
   nosol->Draw("f");nosol->Draw("l");   
   //LepLimit->Draw("l");

   //Legends
   TLatex ms; ms.SetTextSize(0.035); ms.SetTextFont(42);
   //ms.DrawLatex(10,1613,"L_{int} = 4.98 fb^{-1}, #sqrt{s} = 7 TeV"); 
   ms.DrawLatex(1200,1800,"tan#beta=10, #mu>0, A_{0}=0"); 
   {
   TLegend* legexp_gq = new TLegend(0.2,0.38,0.43,0.69,NULL,"brNDC");
   legexp_gq->SetFillColor(0);legexp_gq->SetShadowColor(0);
   legexp_gq->SetTextFont(42);legexp_gq->SetTextSize(0.033);legexp_gq->SetBorderSize(0);
   //TEV_sg_cdf.SetLineColor(1);  
   legexp_gq->SetHeader("#bf{CMS, 4.98 fb^{-1}, #sqrt{s} = 7 TeV}");
   TH1F* legObs = (TH1F*)gCLsObs->Clone();
   legObs->SetLineStyle(gCLsObsExcl->GetLineStyle());
   legObs->SetLineColor(gCLsObsExcl->GetLineColor());
   legObs->SetLineWidth(gCLsObsExcl->GetLineWidth());
   legexp_gq->AddEntry(legObs, "Obs. limit w. +1#sigma signal theory", "lf");
   TH1F* legExp = (TH1F*)gCLs1Sigma->Clone();
   legExp->SetLineStyle(gCLsExpExcl->GetLineStyle());
   legExp->SetLineColor(gCLsExpExcl->GetLineColor());
   legExp->SetLineWidth(gCLsExpExcl->GetLineWidth());
   legexp_gq->AddEntry(legExp, "Exp. limit #pm1#sigma exp. (w/o signal theory)", "lf");
   //legexp_gq->AddEntry(gCLsObsAsym, "Obs. CLs asymptotic, NLO", "l");
   //legexp_gq->AddEntry(gCLsExpAsym, "Exp. CLs asymptotic, NLO", "l");
   legexp_gq->AddEntry(tev,"Tevatron RunI","f"); 
   legexp_gq->AddEntry(cdf,"CDF RunII","f");  
   legexp_gq->AddEntry(dez,"D0 RunII","f");   
   legexp_gq->AddEntry(lep,"LEP2","f"); //NOT FOR tb=50!
   legexp_gq->Draw();
   }
   TPaveText tpave(0.6,0.33,0.75,0.4,"NDC");
   tpave.AddText("no CMSSM");tpave.AddText("solution");
   tpave.SetBorderSize(0);tpave.SetFillColor(0);tpave.SetTextFont(42);
   tpave.Draw();
   gPad->RedrawAxis();
   string nameExcl = "results/"  + flag + "_"+x+"_"+y+ "_Exclusion_";
   c1->SaveAs((nameExcl + ".pdf").c_str());
   if (plotPNG) c1->SaveAs((nameExcl + ".png").c_str());
   
   
   //Draw Exclusion Plot again, but now without asymptotic limits, but with old RA2 limits
   //Drawing the contours
   hplot->Draw("");
   gCLs1Sigma->Draw("f");
   gCLsObsExcl->Draw("l");
   gCLsExpExcl->Draw("l");
   CoverUp->Draw("f");
   //Previous experiments
   nosol_aux->Draw("f");
   dez->Draw("f");
   cdf->Draw("f");
   tev->Draw("f");
   lep->Draw("f");
   //LepLimit->Draw("l");
   //The Legends
   ms.DrawLatex(1200,1800,"tan#beta=10, #mu>0, A_{0}=0"); 
   {
   TLegend* legexp_gq = new TLegend(0.2,0.35,0.55,0.66,NULL,"brNDC");
   legexp_gq->SetFillColor(0);legexp_gq->SetShadowColor(0);
   legexp_gq->SetTextFont(42);legexp_gq->SetTextSize(0.033);legexp_gq->SetBorderSize(0);
   //TEV_sg_cdf.SetLineColor(1);  
   legexp_gq->SetHeader("#bf{CMS, 4.98 fb^{-1}, #sqrt{s} = 7 TeV}");
   TH1F* legExp = (TH1F*)gCLs1Sigma->Clone();
   legExp->SetLineStyle(gCLsExpExcl->GetLineStyle());
   legExp->SetLineColor(gCLsExpExcl->GetLineColor());
   legExp->SetLineWidth(gCLsExpExcl->GetLineWidth());
   legexp_gq->AddEntry(gCLsObsExcl,"Observed, NLO","l");
   legexp_gq->AddEntry(legExp,     "Expected #pm 1#sigma exp. unc.","lf");
   legexp_gq->AddEntry(tev,"Tevatron RunI","f"); 
   legexp_gq->AddEntry(cdf,"CDF RunII","f");  
   legexp_gq->AddEntry(dez,"D0 RunII","f");   
   legexp_gq->AddEntry(lep,"LEP2","f"); //NOT FOR tb=50!
   legexp_gq->Draw();
   }
   nosol->Draw("f");nosol->Draw("l");   

   tpave.Draw();
   gPad->RedrawAxis();
   nameExcl = "results/"  + flag + "_"+x+"_"+y+ "_Exclusion_withOldRA2";
   c1->SaveAs((nameExcl + ".pdf").c_str());
   if (plotPNG) c1->SaveAs((nameExcl + ".png").c_str());
   
   
   }
}


void GetPlotTools(PlotTools*& plotTools, std::string filename, std::string GeneratorFile, unsigned factor)
{
  Events * events = new Events();
  ReadEvents(*events, filename);
  
  //plotting helper functions
  plotTools = new PlotTools(events);

  //Require an observed CLs limit:
  plotTools->Remove("ObsR", Compare::less, 0.0);


  //plotTools->FillEmptyPointsByInterpolation("Mzero", "Mhalf");


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

void AddEvents(PlotTools*& plotTools, std::string filename, std::string GeneratorFile)
{
  Events * additionalEvents = new Events;
  ReadEvents(*additionalEvents, filename);
  plotTools->addEvents( additionalEvents );
  if (GeneratorFile!="") {
    std::vector<GeneratorMasses> GenMasses;
    ReadGeneratorMasses(GenMasses, GeneratorFile);
    Match( GenMasses, *additionalEvents);
  }  
  delete additionalEvents;
}

int plot(int argc, char** argv) {
  util::StyleSettings::paperNoTitle();
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadBottomMargin(0.18);
  gStyle->SetPadRightMargin(0.18);

  TCanvas* c_rectangle = new TCanvas("c_rec", "c_rec", 800, 600);
  TCanvas* c_square    = new TCanvas("c_squ", "c_squ", 800, 800);

  //m0 m1/2 stuff //////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////
  PlotTools * PlotTool;
  //GetPlotTools(PlotTool, "2012-03-28-00-09-cMSSM_V9/filelist.txt", "masses_cMSSM_tb10_A0_Mu1.dat");
  GetPlotTools(PlotTool, "2012-04-16-10-49-cMSSM_V9b/filelist.txt");
  //GetPlotTools(PlotTool, "cMSSM/filelist.txt");

  TH2F h_plot("h",";m_{0} (GeV/c^{2}); m_{1/2} (GeV/c^{2}); cross section [pb]",
          148,50, 3010, 47, 90, 1030);


  //PlotTool->Remove("ObsR", Compare::less, 0.000001);
  //PlotTool->FillEmptyPointsByInterpolation("Mzero", "Mhalf");

  c1=c_square; c1->cd();
  DrawStandardPlots(      PlotTool, "cMSSM", "Mzero", "Mhalf", &h_plot);
  DrawStandardPlotsPerBin(PlotTool, "cMSSM", "Mzero", "Mhalf", &h_plot);


  TH2F h1("h1",";m_{0} (GeV/c^{2}); m_{1/2} (GeV/c^{2}); cross section [pb]",
          145,110, 3010, 45, 90, 990);

  c1=c_square; c1->cd();
  DrawStandardLimitPlots(  PlotTool, "cMSSM", "Mzero", "Mhalf", &h_plot);


  c1=c_rectangle; c1->cd();
  AddEvents(PlotTool, "2012-04-16-10-49-cMSSM_V9b/filelist_additional.txt", "masses_cMSSM_tb10_A0_Mu1.dat");
  DrawExclusion(PlotTool,"cMSSM","Mzero","Mhalf",&h_plot,&h1); //removes points, which have no limits and fills the gaps by interpolation

  
  //TH1F sc("sc",";m_{0} (GeV/c^{2}); relative to background expectation",100,50, 3000);
  //PlotTool->Keep(ExpRasym, 1.0, 0.05);
  //Draw1DSignalContamination(PlotTool,"PlotTool",Mzero,&sc);




  //msq, mgl stuff /////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////
  PlotTools *CMSSM_gl_sq;
  
  //standard plots in gl-sq --------------------------------------------------------------------
  if(1){ 
  TH2F h_sq_gl("h_sq_gl","; m_{#tilde{g}} [GeV]; m_{#tilde{q}} [GeV]; cross section [pb]",
          25, 300, 1800, 75, 500, 2000);
  GetPlotTools(CMSSM_gl_sq, "2012-04-16-10-49-cMSSM_V9b/filelist.txt", "masses_cMSSM_tb10_A0_Mu1.dat", 1);
  CMSSM_gl_sq->Remove("ObsR", Compare::less, 0.0);
  CMSSM_gl_sq->FillEmptyPointsByInterpolation("Mzero", "Mhalf");
  c1=c_square; c1->cd();
  DrawStandardPlots(CMSSM_gl_sq,"cMSSM","MGluino","MSquark",&h_sq_gl);
  DrawStandardLimitPlots(CMSSM_gl_sq,"cMSSM","MGluino","MSquark",&h_sq_gl);
  }

  //exclusion contours in gl-sq ----------------------------------------------------------------
  if(1){ 
  //AddEvents(CMSSM_gl_sq, "2012-04-16-10-49-cMSSM_V9b/filelist.txt", "masses_cMSSM_tb10_A0_Mu1.dat");
  TH2F h_sq_gl("h_sq_gl","; m_{#tilde{g}} [GeV]; m_{#tilde{q}} [GeV]; cross section [pb]",
          30, 0, 1800, 90, 0, 2000);
  TH2F h("hqg",";gluino;squark; dummy",
          25, 600, 1800, 60, 900, 2100);
  c1=c_rectangle; c1->cd();
  DrawExclusionGlSq(CMSSM_gl_sq,"cMSSM","MGluino","MSquark",&h_sq_gl,&h);
  }


}

int main(int argc, char** argv) {
	return plot(argc, argv);
}
