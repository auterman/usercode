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

const static bool plotPNG = false;
const static bool plotC   = false;
const static bool plotPrelim = false;
TCanvas * c1 = 0;

void DrawPlot2D(PlotTools *PlotTool, TCanvas*canvas, TH2* h, const std::string& flag, const string& x, const std::string& y, const std::string& var, 
                const std::string& ztitel, double zmin=-999, double zmax=-999 )
{
   TH2F *plot2D = (TH2F*)h->Clone();
   if (zmin!=-999) plot2D->SetMinimum(zmin);
   if (zmax!=-999) plot2D->SetMaximum(zmax);
   plot2D->GetZaxis()->SetTitle(ztitel.c_str());
   PlotTool->Area(plot2D, x, y, var);
   plot2D->GetZaxis()->SetTitleOffset(1.5);
   plot2D->Draw("colz"); 
   double tx=400;
   double ty=1.045*(plot2D->GetYaxis()->GetXmax()-plot2D->GetYaxis()->GetXmin())+plot2D->GetYaxis()->GetXmin();  
   TLatex tex(tx,ty,"#font[42]{CMS preliminary, 4.62 fb^{-1}, #sqrt{s} = 7 TeV}");
   tex.SetTextSize(0.035);
   tex.Draw();
   string namePlot = "results/" + flag + "_"+x+"_"+y+"_"+var;
   if (plotPrelim) canvas->SaveAs((namePlot + "_prelim.pdf").c_str());
   if (plotPNG && plotPrelim) canvas->SaveAs((namePlot + "_prelim.png").c_str());
   //
   plot2D->Draw("colz"); 
   TLatex tex2(tx,ty,"#font[42]{CMS, 4.62 fb^{-1}, #sqrt{s} = 7 TeV}");
   tex2.SetTextSize(0.035);
   tex2.Draw();
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

   DrawHist1D(pt,c1,flag,x,y,"ObsRmM2", "ObsR - ExpR-2sigma", 20);
   DrawHist1D(pt,c1,flag,x,y,"ObsRmP2", "ObsR - ExpR+2sigma", 20);
   DrawHist1D(pt,c1,flag,x,y,"ObsRdM2", "ObsR / ExpR-2sigma", 20);
   DrawHist1D(pt,c1,flag,x,y,"ObsRdP2", "ObsR / ExpR+2sigma", 20);
}

void DrawStandardLimitPlots(PlotTools *pt, const std::string& flag, const std::string& x, const std::string& y, TH2*h)
{

   c1->SetRightMargin(0.18);
   
   //Log z-scale
   c1->SetLogz(1);
   DrawPlot2D(pt,c1,h,flag,x,y,"ObsXsecLimit",      "Observed cross section limit [pb]");
   DrawPlot2D(pt,c1,h,flag,x,y,"ExpXsecLimit",      "Expected cross section limit [pb]");
   DrawPlot2D(pt,c1,h,flag,x,y,"ObsNsignalLimit",   "Observed limit on number signal events");
   DrawPlot2D(pt,c1,h,flag,x,y,"ExpNsignalLimit",   "Expected limit on number signal events");
   DrawPlot2D(pt,c1,h,flag,x,y,"ObsAsympXsecLimit", "Observed asympt. cross section limit [pb]");
   DrawPlot2D(pt,c1,h,flag,x,y,"ExpAsympXsecLimit", "Expected asympt. cross section limit [pb]");

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
   hxsec->SetMinimum(-0);hxsec->SetMaximum(1.);
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
   TGraph * gCLsObsTheom1   = PlotTool->GetContour(hs, x, y, "ObsRTheoM1", 3, 0, 1, 4);
   TGraph * gCLsObsTheop1   = PlotTool->GetContour(hs, x, y, "ObsRTheoP1", 3, 0, 1, 4);
   TGraph * gCLsExpTheom1   = PlotTool->GetContour(hs, x, y, "ExpRTheoM1", 3, 0, 1, 3);
   TGraph * gCLsExpTheop1   = PlotTool->GetContour(hs, x, y, "ExpRTheoP1", 3, 0, 1, 3);
   gCLsObsExcl->SetLineWidth(2);
   gCLsExpExcl->SetLineWidth(2);

   Cut(gCLsExpExclm1,'y','<',260);
   Cut(gCLsExpExclp1,'y','<',200);
   Cut(gCLsExpExcl,'y','<',240);
   Cut(gCLsObsExcl,'y','<',260);
   Cut(gCLsExpExcl,'x','>',2970);
   Cut(gCLsObsExcl,'x','>',2970);
   Cut(gCLsExpExclp1,'x','>',2780);

   Cut(gCLsExpTheom1,'y','<',240);
   Cut(gCLsObsTheom1,'y','<',260);
   Cut(gCLsExpTheom1,'x','>',2970);
   Cut(gCLsObsTheom1,'x','>',2970);
   Cut(gCLsExpTheop1,'y','<',240);
   Cut(gCLsObsTheop1,'y','<',260);
   Cut(gCLsExpTheop1,'x','>',2970);
   Cut(gCLsObsTheop1,'x','>',2970);

   Smooth(gCLsObsExcl,     30);
   Smooth(gCLsExpExcl,     30);
   Smooth(gCLsExpExclm1,   30);
   Smooth(gCLsExpExclp1,   30);

   Smooth(gCLsObsTheom1,     30);
   Smooth(gCLsExpTheom1,     30);
   Smooth(gCLsObsTheop1,     30);
   Smooth(gCLsExpTheop1,     30);

   TGraph * gCLs1Sigma = MakeBand(gCLsExpExclm1, gCLsExpExclp1);
   gCLs1Sigma->SetFillStyle(3001);
   gCLs1Sigma->SetFillColor(5);
   hplot->GetYaxis()->SetTitleOffset(1.55);
   hplot->GetXaxis()->SetTitleOffset(1.1);
   //Drawing the contours
   TGraph * RA2_36pb = RA2Observed_36pb();
   TGraph * RA2_1fb = RA2Observed_1fb();
   TGraph * Atlas = Atlas0l24j_1fb();
   RA2_36pb->Draw("l");
   Atlas->Draw("l");
   gCLs1Sigma->Draw("f");
   gCLsObsExcl->Draw("l");
   gCLsExpExcl->Draw("l");
//   gCLsExpTheom1->Draw("l");
   gCLsObsTheom1->Draw("l");
//   gCLsExpTheop1->Draw("l");
   gCLsObsTheop1->Draw("l");

   //Legends
   TLegend * leg = new TLegend(0.35, 0.6, 0.7, 0.8);
   leg->SetBorderSize(0);
   leg->SetFillColor(0);
   //leg->SetFillStyle(4000);
   leg->SetTextSize(0.035);
   leg->SetTextFont(42);
   leg->SetHeader("#bf{CMS, 4.62 fb^{-1}, #sqrt{s} = 7 TeV}");
   leg->AddEntry(gCLsObsExcl, "Obs. limit", "l");
   leg->AddEntry(gCLsObsTheop1, "Obs. limit #pm1#sigma signal theory", "l");
   TH1F* legExp = (TH1F*)gCLs1Sigma->Clone();
   legExp->SetLineStyle(gCLsExpExcl->GetLineStyle());
   legExp->SetLineColor(gCLsExpExcl->GetLineColor());
   legExp->SetLineWidth(gCLsExpExcl->GetLineWidth());
   leg->AddEntry(legExp, "Exp. limit #pm1#sigma exp.", "lf");
   //leg->AddEntry(gCLsExpTheop1, "Exp. limit #pm1#sigma signal theory", "l");
   leg->AddEntry(Atlas, "Atlas, 1.04 fb^{-1}", "l");
   leg->AddEntry(RA2_36pb, "CMS, 36 pb^{-1}", "l");
   leg->Draw();
   TMarker lm5(230,360,29);
   lm5.SetMarkerColor(14);
   lm5.Draw("same");
   TLatex tex(245,365,"#font[42]{LM5}");
   tex.SetTextColor(12);
   tex.SetTextSize(0.03);
   tex.Draw();

   gPad->RedrawAxis();
   string nameExcl = "results/"+ flag + "_"+x+"_"+y+"_Exclusion_";
   c1->SaveAs((nameExcl + ".pdf").c_str());
   if (plotPNG) c1->SaveAs((nameExcl + ".png").c_str());
   if (plotC  ) {
     h->GetZaxis()->SetTitle("");
     h->Draw("h");
     gCLsObsExcl->SetName("Observed limit");
     gCLsObsExcl->SetTitle("Observed limit");
     gCLsObsExcl->Draw("l");
     c1->SaveAs((nameExcl + ".C").c_str());
   }

   ///------------------------------------------------------------------------------------
   /// same, but preliminary:
   RA2_36pb->Draw("l");
   gCLs1Sigma->Draw("f");
   gCLsObsExcl->Draw("l");
   gCLsExpExcl->Draw("l");
   gCLsExpTheom1->Draw("l");
   gCLsObsTheom1->Draw("l");
   gCLsExpTheop1->Draw("l");
   gCLsObsTheop1->Draw("l");
   leg->SetHeader("#bf{CMS preliminary, 4.62 fb^{-1}, #sqrt{s} = 7 TeV}");
   leg->Draw();
   lm5.Draw("same");
   tex.Draw();
   gPad->RedrawAxis();
   nameExcl = "results/"+ flag + "_"+x+"_"+y+"_Exclusion_prelim";
   if (plotPrelim) c1->SaveAs((nameExcl + ".pdf").c_str());

   
   ///------------------------------------------------------------------------------------
   //Draw Exclusion Plot again, but now without asymptotic limits, but with old RA2 limits
   //Drawing the contours
   //TCanvas * c3 = GetLimitTemplateCanvas("GridTanb10_v1.root","GridCanvas");
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
   leg->SetHeader("#bf{4.62 fb^{-1}, #sqrt{s}=7 TeV, #sqrt{s} = 7 TeV}");
   leg->AddEntry(gCLsObsExcl, "Observed", "l");
   TH1F* legExp = (TH1F*)gCLs1Sigma->Clone();legExp->SetLineStyle(gCLsExpExcl->GetLineStyle());
   legExp->SetLineColor(gCLsExpExcl->GetLineColor());legExp->SetLineWidth(gCLsExpExcl->GetLineWidth());
   leg->AddEntry(legExp, "Expected #pm 1#sigma exp. unc.", "lf");
   leg->Draw();
   }
   gPad->RedrawAxis();
   nameExcl = "results/" + flag + "_"+x+"_"+y+"_Exclusion_woObsBand";
   c1->SaveAs((nameExcl + ".pdf").c_str());
   if (plotPNG) c1->SaveAs((nameExcl + ".png").c_str());
   //delete c3;
   
   }
   c1->cd();
}





void GetPlotTools(PlotTools*& plotTools, std::string filename, std::string GeneratorFile, unsigned factor)
{
  Events * events = new Events();
  ReadEvents(*events, filename);
  
  //plotting helper functions
  plotTools = new PlotTools(events);

  //Require an observed CLs limit:
  plotTools->Remove("ObsR", Compare::less, 0.0);


  plotTools->FillEmptyPointsByInterpolation("gluino", "squark");


  //Make grid in Mzero, Mhalf finer by factors of 2 by linear interpolation
  for (int i=2; i<=factor; i*=2)
    plotTools->ExpandGrid("gluino", "squark");
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

  TCanvas* c_square    = new TCanvas("c_squ", "c_squ", 800, 800);
  c1 = c_square;
  c1->cd();

  {
  //Bino gl-sq //////////////////////////////////////////////////////////////////////////////
    PlotTools * PlotTool;
    GetPlotTools(PlotTool, "2012-05-08-22-30-GMSBBino375Neutr2j/filelist.txt");

    TH2F h_glsq("h",               ";#tilde{g} [GeV]; #tilde{q} [GeV]; cross section [pb]", 21 ,360, 2040, 21, 360, 2040);
    TH2F h_glsq_exclusion("h_excl",";#tilde{g} [GeV]; #tilde{q} [GeV]; cross section [pb]", 21 ,360, 2040, 21, 360, 2040);

    //PlotTool->Remove("ObsR", Compare::less, 0.000001);
    //PlotTool->FillEmptyPointsByInterpolation("gluino", "squark");

    DrawStandardPlots(      PlotTool, "GMSB", "gluino", "squark", &h_glsq);
    //DrawStandardPlotsPerBin(PlotTool, "GMSB", "gluino", "squark", &h_plot);
    DrawStandardLimitPlots(  PlotTool, "GMSB", "gluino", "squark", &h_glsq);
    DrawExclusion(PlotTool,"GMSB","gluino", "squark",&h_glsq,&h_glsq_exclusion); //removes points, which have no limits and fills the gaps by interpolation
  }

}

int main(int argc, char** argv) {
	return plot(argc, argv);
}
