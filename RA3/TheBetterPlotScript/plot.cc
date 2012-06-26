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

#include "StyleSettings_SinglePhoton7TeV.h"
#include "StyleSettings_SinglePhoton8TeV.h"

const static bool plotPNG = false;
const static bool plotC   = true;
const static bool plotROOT = true;
const static bool plotPrelim = false;
TCanvas * c1 = 0;

void DrawPlot2D(PlotTools *PlotTool, TCanvas*canvas, TH2* h, const std::string& flag, const string& x, const std::string& y, const std::string& var, 
                const std::string& ztitel, double zmin=-999, double zmax=-999, style*s=0 )
{
   TH2F *plot2D = (TH2F*)h->Clone();
   plot2D->GetZaxis()->SetTitle(ztitel.c_str());
   PlotTool->Area(plot2D, x, y, var);
   plot2D->GetZaxis()->SetTitleOffset(1.5);
   if (zmin!=-999) plot2D->SetMinimum(zmin);
   if (zmax!=-999) plot2D->SetMaximum(zmax);
   if (zmin==-999&&zmax==-999) SetZRange(plot2D);
   plot2D->Draw("colz"); 
   double tx=400;
   double ty=1.045*(plot2D->GetYaxis()->GetXmax()-plot2D->GetYaxis()->GetXmin())+plot2D->GetYaxis()->GetXmin();  

   if (s&&s->cmsprelimTemperaturePlot) s->cmsprelimTemperaturePlot->Draw();
   if (s&&s->lumiTemperaturePlot) s->lumiTemperaturePlot->Draw();
   gPad->RedrawAxis();
   string namePlot = "results/" + flag + "_"+x+"_"+y+"_"+var;
   if (plotPrelim) canvas->SaveAs((namePlot + "_prelim.pdf").c_str());
   if (plotPNG && plotPrelim) canvas->SaveAs((namePlot + "_prelim.png").c_str());
   if (plotC   && plotPrelim) canvas->SaveAs((namePlot + "_prelim.C").c_str());
   //
   plot2D->Draw("colz"); 
   if (s&&s->cmsTemperaturePlot) s->cmsTemperaturePlot->Draw();
   if (s&&s->lumiTemperaturePlot) s->lumiTemperaturePlot->Draw();
   gPad->RedrawAxis();
   canvas->SaveAs((namePlot + ".pdf").c_str());
   if (plotPNG) canvas->SaveAs((namePlot + ".png").c_str());
   if (plotC)   canvas->SaveAs((namePlot + ".C").c_str());
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


void DrawStandardUncertaintyPlots(PlotTools *pt, const std::string& flag, const std::string& x, const std::string& y, style*s, TH2*h)
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

void DrawStandardPlots(PlotTools *pt, const std::string& flag, const std::string& x, const std::string& y, style*s, TH2*h)
{
   gStyle->SetPadRightMargin(0.2);
   gStyle->SetPadLeftMargin(0.2);
   gStyle->SetTitleOffset(1.3, "xyz");
   gStyle->SetTitleOffset(1.9, "y");
   gStyle->SetNdivisions(505);
   gStyle->SetTitleFont(43, "xyz");
   gStyle->SetTitleSize(32, "xyz");
   c1->UseCurrentStyle();
   
   //Log z-scale
   c1->SetLogz(1);
   DrawPlot2D(pt,c1,h,flag,x,y,"Xsection",          "NLO cross section [pb]");

   //Linear z-scale
   c1->SetLogz(0);
   DrawPlot2D(pt,c1,h,flag,x,y,"Acceptance",        "Acceptance", s->MinAccZ, s->MaxAccZ, s );
   DrawPlot2D(pt,c1,h,flag,x,y,"AcceptancePercent", "Acceptance [%]", s->MinAccZ*100., s->MaxAccZ*100., s );
   DrawPlot2D(pt,c1,h,flag,x,y,"AcceptanceCorr",    "Acceptance corr. f. sig. cont. [%]", s->MinAccZ*100., s->MaxAccZ*100., s );

   //1D Histograms
   DrawHist1D(pt,c1,flag,x,y,"SignalStatUnc",	  "Rel. Signal Statistical uncertainty", 20);
   DrawHist1D(pt,c1,flag,x,y,"SignalSysUnc",	  "Rel. Signal Systematic uncertainty", 20);
   DrawHist1D(pt,c1,flag,x,y,"SignalPDFAccUnc",   "Rel. Signal PDF Accept. uncertainty", 20);
   DrawHist1D(pt,c1,flag,x,y,"SignalPDFXsecUnc",  "Rel. Signal PDF cross-section uncertainty", 20);
   DrawHist1D(pt,c1,flag,x,y,"SignalScaleUnc",    "Rel. Signal Scale uncertainty", 20);

   //DrawHist1D(pt,c1,flag,x,y,"ObsRmM2", "ObsR - ExpR-2sigma", 20);
   //DrawHist1D(pt,c1,flag,x,y,"ObsRmP2", "ObsR - ExpR+2sigma", 20);
   //DrawHist1D(pt,c1,flag,x,y,"ObsRdM2", "ObsR / ExpR-2sigma", 20);
   //DrawHist1D(pt,c1,flag,x,y,"ObsRdP2", "ObsR / ExpR+2sigma", 20);
}

void DrawStandardLimitPlots(PlotTools *pt, const std::string& flag, const std::string& x, const std::string& y, style*s, TH2*h)
{
   gStyle->SetPadRightMargin(0.2);
   gStyle->SetPadLeftMargin(0.2);
   gStyle->SetTitleOffset(1.3, "xyz");
   gStyle->SetTitleOffset(1.9, "y");
   gStyle->SetNdivisions(505);
   gStyle->SetTitleFont(43, "xyz");
   gStyle->SetTitleSize(32, "xyz");
   c1->UseCurrentStyle();
   
   //Log z-scale
   c1->SetLogz(1);
   //DrawPlot2D(pt,c1,h,flag+"_FixedBinning",x,y,"ObsXsecLimit",      "Observed cross section limit [pb]",0.001,0.02);
   DrawPlot2D(pt,c1,h,flag,x,y,"ObsXsecLimit",      "Observed cross section limit [pb]", s->MinXsecZ, s->MaxXsecZ, s );
   DrawPlot2D(pt,c1,h,flag,x,y,"ExpXsecLimit",      "Expected cross section limit [pb]", s->MinXsecZ, s->MaxXsecZ, s);
   //DrawPlot2D(pt,c1,h,flag,x,y,"ObsNsignalLimit",   "Observed limit on number signal events");
   //DrawPlot2D(pt,c1,h,flag,x,y,"ExpNsignalLimit",   "Expected limit on number signal events");
   DrawPlot2D(pt,c1,h,flag,x,y,"ObsXsecLimitasym",  "Observed asympt. cross section limit [pb]", s->MinXsecZ, s->MaxXsecZ, s);
   DrawPlot2D(pt,c1,h,flag,x,y,"ExpXsecLimitasym",  "Expected asympt. cross section limit [pb]", s->MinXsecZ, s->MaxXsecZ, s);

   //Linear z-scale
   c1->SetLogz(0);
   DrawPlot2D(pt,c1,h,flag,x,y,"ObsR",              "Observed R", 0.5, 2.);
   DrawPlot2D(pt,c1,h,flag,x,y,"ExpR",              "Expected R", 0.5, 2.);
   //DrawPlot2D(pt,c1,h,flag,x,y,"ObsDivExp",         "Observed / Expected", 0.0, 2.);
}


void DrawStandardPlotsPerBin(PlotTools *pt, const std::string& flag, const std::string& x, const std::string& y, style*s, TH2*h)
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

TGraph * InOutPlot(PlotTools *PlotTool, std::string flag, const std::string& x, const std::string& y, const std::string& R, TH2*h, unsigned idx=0, int color=0, int style=0)
{
   PlotTool->InOutFromR(h, x, y, R, 3);
   FillEmptyPoints(h,0.5);
   h->GetZaxis()->SetTitleOffset(1.5);
   h->SetMinimum(-0);h->SetMaximum(1.);
   std::vector<TGraph*> contours = GetContours(h, 3);
   /// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
   h->Draw("colz");
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
   string nameXsPlot = "results/" + flag + "_"+x+"_"+y+"_"+"InOut"+R;
   c1->SaveAs((nameXsPlot + ".pdf").c_str());
   if (plotPNG) c1->SaveAs((nameXsPlot + ".png").c_str());
   delete h;
   TGraph * res = (TGraph*)contours[idx]->Clone();
   res->SetLineColor(color);
   res->SetLineStyle(style);
   return res;
}

void DrawExclusion(PlotTools *PlotTool, std::string flag, const std::string& x, const std::string& y, 
                   TH1*hp, TH1*h, style*s)
{
   //Require an observed CLs limit:
   //PlotTool->Remove("ObsR", Compare::less, 0.0);
   //PlotTool->FillEmptyPointsByInterpolation1D(x, y);
 
   TH2F *hs = (TH2F*)h->Clone();
   TH2F *hplot = (TH2F*)hp->Clone();
   hs->GetZaxis()->SetTitle("");

std::cout << "s->iCLsObsExcl  =  "<<s->iCLsObsExcl<<", iCLsExpExcl = "<<s->iCLsExpExcl<<std::endl;

   //TGraph * gCLsObsExcl     = PlotTool->GetContour(hs, x, y, "ObsR",       3, s->iCLsObsExcl  , kBlue, 1);
   //TGraph * gCLsExpExcl     = PlotTool->GetContour(hs, x, y, "ExpR",       3, s->iCLsExpExcl  , kOrange - 3, 9);
   //TGraph * gCLsExpExclm1   = PlotTool->GetContour(hs, x, y, "ExpRM1",     3, s->iCLsExpExclm1, kOrange - 3, 1);
   //TGraph * gCLsExpExclp1   = PlotTool->GetContour(hs, x, y, "ExpRP1",     3, s->iCLsExpExclp1, kOrange - 3, 1);
   //TGraph * gCLsObsTheom1   = PlotTool->GetContour(hs, x, y, "ObsRTheoM1", 3, s->iCLsObsTheom1, kBlue, 3);
   //TGraph * gCLsObsTheop1   = PlotTool->GetContour(hs, x, y, "ObsRTheoP1", 3, s->iCLsObsTheop1, kBlue, 3);
   //TGraph * gCLsExpTheom1   = PlotTool->GetContour(hs, x, y, "ExpRTheoM1", 3, s->iCLsExpTheom1, 1, 3);
   //TGraph * gCLsExpTheop1   = PlotTool->GetContour(hs, x, y, "ExpRTheoP1", 3, s->iCLsExpTheop1, 1, 3);

   //In/Out Plot
   hplot->GetZaxis()->SetTitle("Observed in/out");
   TGraph * gCLsObsExcl	  = InOutPlot(PlotTool,flag,x,y,"ObsR",(TH2F*)h->Clone(), s->iCLsObsExcl, kBlue, 1);

   hplot->GetZaxis()->SetTitle("Expected in/out");
   TGraph * gCLsExpExcl   = InOutPlot(PlotTool,flag,x,y,"ExpR",(TH2F*)h->Clone(), s->iCLsExpExcl, kOrange - 3, 9);

   hplot->GetZaxis()->SetTitle("Expected -1 #sigma_{experimental} in/out");
   TGraph * gCLsExpExclm1 = InOutPlot(PlotTool,flag,x,y,"ExpRM1",(TH2F*)h->Clone(), s->iCLsExpExclm1, kOrange - 3, 1);

   hplot->GetZaxis()->SetTitle("Expected +1 #sigma_{experimental} in/out");
   TGraph * gCLsExpExclp1 = InOutPlot(PlotTool,flag,x,y,"ExpRP1",(TH2F*)h->Clone(), s->iCLsExpExclp1, kOrange - 3, 1);

   hplot->GetZaxis()->SetTitle("Observed -1 #sigma_{theory} in/out");
   TGraph * gCLsObsTheom1 = InOutPlot(PlotTool,flag,x,y,"ObsRTheoM1",(TH2F*)h->Clone(), s->iCLsObsTheom1, kBlue, 3);

   hplot->GetZaxis()->SetTitle("Observed +1 #sigma_{theory} in/out");
   TGraph * gCLsObsTheop1 = InOutPlot(PlotTool,flag,x,y,"ObsRTheoP1",(TH2F*)h->Clone(), s->iCLsObsTheop1, kBlue, 3);


   {//Exclusion Contours
   gStyle->SetPadRightMargin(0.08);
   gStyle->SetPadLeftMargin(0.2);
   gStyle->SetTitleOffset(1.3, "xyz");
   gStyle->SetTitleOffset(1.9, "y");
   gStyle->SetNdivisions(505);
   gStyle->SetTitleFont(43, "xyz");
   gStyle->SetTitleSize(32, "xyz");
   c1->UseCurrentStyle();
   c1->SetLogz(0);
   c1->SetTopMargin(0.11);
   
   gCLsObsExcl->SetLineWidth(3);
   gCLsExpExcl->SetLineWidth(3);
   

   Smooth(gCLsObsExcl,     s->smooth_points, s->smooth_flag);
   Smooth(gCLsExpExcl,     s->smooth_points, s->smooth_flag);
   Smooth(gCLsExpExclm1,   s->smooth_points, s->smooth_flag);
   Smooth(gCLsExpExclp1,   s->smooth_points, s->smooth_flag);
   Smooth(gCLsObsTheom1,     s->smooth_points, s->smooth_flag);
   Smooth(gCLsObsTheop1,     s->smooth_points, s->smooth_flag);

   TGraph * gCLs1Sigma = MakeBand(gCLsExpExclm1, gCLsExpExclp1);
   gCLs1Sigma->SetFillStyle(3001);
   gCLs1Sigma->SetFillColor(5);
   hplot->GetYaxis()->SetTitleOffset(1.65);
   hplot->GetXaxis()->SetTitleOffset(1.1);
   //Drawing the contours
   hplot->Draw("h");
   gCLs1Sigma->Draw("f");
   gCLsObsExcl->Draw("l");
   gCLsExpExcl->Draw("l");
//   gCLsExpTheom1->Draw("l");
   gCLsObsTheom1->Draw("l");
//   gCLsExpTheop1->Draw("l");
   gCLsObsTheop1->Draw("l");

   //Legends
   TLegend* leg = new TLegend(s->LegendMinX,  s->LegendMinY, s->LegendMaxX,s->LegendMaxY);
   TH1F * legdummy = 0;
   //leg->AddEntry(legdummy,  ("m_{#tilde{#chi}^{0}} = "+neutralinomass+" [GeV]").c_str(), "l");
   leg->SetBorderSize(0);
   leg->SetLineColor(0);
   leg->SetFillColor(10);
   leg->SetFillStyle(1001);
   leg->SetTextFont(42);
   leg->SetTextSize(0.03);
   leg->SetHeader(s->LegendTitel.c_str());
   leg->AddEntry(gCLsObsExcl, "Observed", "l");
   leg->AddEntry(gCLsObsTheop1, "Observed #pm1#sigma signal theory", "l");
   TH1F* legExp = (TH1F*)gCLs1Sigma->Clone();
   legExp->SetLineStyle(gCLsExpExcl->GetLineStyle());
   legExp->SetLineColor(gCLsExpExcl->GetLineColor());
   legExp->SetLineWidth(gCLsExpExcl->GetLineWidth());
   leg->AddEntry(legExp, "Expected #pm1#sigma exp.", "lf");
   //leg->AddEntry(gCLsExpTheop1, "Exp. limit #pm1#sigma signal theory", "l");
   if (s->excluded) s->excluded->Draw();
   if (s->coverUp) s->coverUp();   
   leg->Draw();
   s->lumi->Draw();
   s->cms->Draw();

   gPad->RedrawAxis();
   string nameExcl = "results/"+ flag + "_"+x+"_"+y+"_Exclusion_";
   c1->SaveAs((nameExcl + ".pdf").c_str());
   if (plotPNG) c1->SaveAs((nameExcl + ".png").c_str());
   if (plotROOT) c1->SaveAs((nameExcl + "canvas.C").c_str());
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
   gCLs1Sigma->Draw("f");
   gCLsObsExcl->Draw("l");
   gCLsExpExcl->Draw("l");
   gCLsObsTheom1->Draw("l");
   gCLsObsTheop1->Draw("l");
   leg->SetHeader("#bf{CMS preliminary, 4.62 fb^{-1}, #sqrt{s} = 7 TeV}");
   leg->Draw();
   s->lumi->Draw();
   s->cmsprelim->Draw();
   if (s->coverUp) s->coverUp();   
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
   s->lumi->Draw();
   s->cms->Draw();
   if (s->coverUp) s->coverUp();   
   gPad->RedrawAxis();
   nameExcl = "results/" + flag + "_"+x+"_"+y+"_Exclusion_woObsBand";
   c1->SaveAs((nameExcl + ".pdf").c_str());
   if (plotPNG) c1->SaveAs((nameExcl + ".png").c_str());
   //delete c3;
   
   }
   c1->cd();
}





void GetPlotTools(PlotTools*& plotTools, std::string filename, const std::string& x, const std::string& y, std::string GeneratorFile, unsigned factor)
{
  Events * events = new Events();
  ReadEvents(*events, filename);
  
  //plotting helper functions
  plotTools = new PlotTools(events);

  //Require an observed CLs limit:
  plotTools->Remove("ObsR", Compare::less, 0.0);
  plotTools->Remove("Xsection", Compare::less, 0.0);

  //Fill the holes by 2D interpolation in gl-sq
  plotTools->FillEmptyPointsByInterpolation(x, y);

  //Make grid in Mzero, Mhalf finer by factors of 2 by linear interpolation
  for (int i=2; i<=factor; i*=2)
    plotTools->ExpandGrid(x, y);
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

void DoPlotsFor(const std::string& x, const std::string& y, const std::string& flag, const std::string& file, style*s, int factor=0, TH2*plot_range=0, TH2*plot_excl=0)  {
    PlotTools * PlotTool;
    GetPlotTools(PlotTool, file, x, y, "", 0);
    GetInfo("squark")->SetLabel("m(#tilde{q}) [GeV]");
    GetInfo("gluino")->SetLabel("m(#tilde{g}) [GeV]");
    GetInfo("chi1")->SetLabel("m(#tilde{#chi}^{0}_{1}) [GeV]");
    GetInfo("cha1")->SetLabel("m(#tilde{#chi}^{#pm}_{1}) [GeV]");
    if (!plot_range) plot_range = PlotTool->GetHist(x,y);
    if (!plot_excl)  plot_excl = plot_range;
    DrawStandardPlots(PlotTool, flag, x,y, s, plot_range);
    //DrawStandardPlotsPerBin(PlotTool, "GMSB", x,y, &plot_range);
    DrawStandardLimitPlots(PlotTool, flag, x,y, s, plot_range);
  
    for (int i=2; i<=factor; i*=2)
      PlotTool->ExpandGrid(x, y);

    if (plot_excl == plot_range) {
      delete plot_range; 
      plot_range = PlotTool->GetHist(x,y);
      plot_excl = plot_range;
    } else {
      delete plot_range; 
      plot_range = PlotTool->GetHist(x,y);
    }  
    DrawExclusion(PlotTool,flag,x,y,plot_range,plot_excl,s); //removes points, which have no limits and fills the gaps by interpolation
}

void PlotAll(std::vector<LimitGraphs*>& lg, const std::string& flag, const std::string& limit, TH2*h=0)
{
  if (!h) h = (TH2F*)lg.front()->GetHist()->Clone();
  h->Draw();
  TLegend* leg = new TLegend(0.55,0.52,0.88,0.83,NULL,"brNDC");
  leg->SetFillColor(0);leg->SetShadowColor(0);
  leg->SetTextFont(42);leg->SetTextSize(0.025);leg->SetBorderSize(1);
  leg->SetHeader("CMS, L_{int} = 4.7 fb^{-1}, #sqrt{s} = 7 TeV");
  for (std::vector<LimitGraphs*>::iterator it=lg.begin();it!=lg.end();++it) {
    TGraph * g = (*it)->Limit(limit);
    g->Draw("l");
    leg->AddEntry(g,(*it)->Name().c_str(),"l");
  }
  leg->Draw();
  gPad->RedrawAxis();
  c1->SaveAs(("results/"+flag +"_"+limit+".pdf").c_str());
  if (plotPNG) c1->SaveAs(("results/"+flag+limit+".png").c_str());
}


void MultipleChannels(const std::string& x, const std::string& y, const std::string& flag, const std::string& dir, TH2*plot_range=0)  
{
  //read all channels
  std::vector<LimitGraphs*> lg;  
  lg.push_back(new LimitGraphs(dir+"/filelist.txt",    "", 1, x, y, "combined", 15, 1, 2, 1, 0, 0, plot_range) );
  lg.push_back(new LimitGraphs(dir+"/filelist_ch0.txt","", 1, x, y, "Bin 0",    15, 2, 0, 0, 0, 0, plot_range) );
  lg.push_back(new LimitGraphs(dir+"/filelist_ch1.txt","", 1, x, y, "Bin 1",    15, 3, 0, 0, 0, 0, plot_range) );
  lg.push_back(new LimitGraphs(dir+"/filelist_ch2.txt","", 1, x, y, "Bin 2",    15, 4, 0, 0, 0, 0, plot_range) );
  lg.push_back(new LimitGraphs(dir+"/filelist_ch3.txt","", 1, x, y, "Bin 3",    15, 5, 0, 0, 0, 0, plot_range) );
  lg.push_back(new LimitGraphs(dir+"/filelist_ch4.txt","", 1, x, y, "Bin 4",    15, 6, 2, 0, 0, 0, plot_range) );
  lg.push_back(new LimitGraphs(dir+"/filelist_ch5.txt","", 1, x, y, "Bin 5",    15, 7, 2, 0, 0, 0, plot_range) );

  //define plot range and labels
  GetInfo("squark")->SetLabel("m(#tilde{q}) [GeV]");
  GetInfo("gluino")->SetLabel("m(#tilde{g}) [GeV]");
  GetInfo("chi1"  )->SetLabel("m(#tilde{#chi}^{0}_{1}) [GeV]");
  if (!plot_range) plot_range = (TH2F*)lg.front()->GetHist()->Clone();
  plot_range->GetXaxis()->SetTitle(GetInfo(x)->GetLabel().c_str());
  plot_range->GetYaxis()->SetTitle(GetInfo(y)->GetLabel().c_str());

  //plotting
  PlotAll(lg,flag,x+"_"+y+"_allObsAsym",plot_range);
  PlotAll(lg,flag,x+"_"+y+"_allExpAsym",plot_range);
  c1->SetRightMargin(0.2);

  for (std::vector<LimitGraphs*>::iterator it=lg.begin();it!=lg.end();++it) {
      std::string l=flag+"_"+(*it)->Name();
      std::replace(l.begin(),l.end(),' ','_');
      c1->SetLogz(0);
      DrawPlot2D((*it)->GetPlot(),c1,(*it)->GetHist(),l,x,y,"Acceptance",        "Acceptance");
      c1->SetLogz(1);
      DrawPlot2D((*it)->GetPlot(),c1,(*it)->GetHist(),l,x,y,"ObsXsecLimitasym", "Observed asympt. cross section limit [pb]");
      DrawPlot2D((*it)->GetPlot(),c1,(*it)->GetHist(),l,x,y,"ExpXsecLimitasym", "Expected asympt. cross section limit [pb]");
      DrawPlot2D((*it)->GetPlot(),c1,(*it)->GetHist(),l,x,y,"ObsRasym", "Asymptotic Observed R ");
      //In/Out plot 
      {
      TH2F *hplot = (TH2F*)(*it)->GetHist()->Clone();
      hplot->GetZaxis()->SetTitle("Observed in/out");
      InOutPlot((*it)->GetPlot(),l,x,y,"ObsRasym",hplot);
      delete hplot;
      }
      {
      TH2F *hplot = (TH2F*)(*it)->GetHist()->Clone();
      hplot->GetZaxis()->SetTitle("Expected in/out");
      InOutPlot((*it)->GetPlot(),l,x,y,"ExpRasym",hplot);
      delete hplot;
      }
  }
}


int plot(int argc, char** argv) {
  util::StyleSettings::paperNoTitle();
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadBottomMargin(0.18);
  gStyle->SetPadRightMargin(0.18);

  TCanvas* c_square    = new TCanvas("c_squ", "c_squ", 800, 800);
  c1 = c_square;
  c1->cd();

  ///Single-Photon 7 TeV Paper
  if (0){
  DoPlotsFor("cha1",  "chi1",  "GMSB_7TeV_WinoBino2j","2012-06-19-13-43-GMSB_WinoBino_7TeV_2j/filelist.txt",GetWinoBinoPlotStyle(),0);
  DoPlotsFor("squark","gluino","GMSB_7TeV_Wino2j",    "2012-06-23-13-27-GMSB_sqgWino_7TeV_2j/filelist.txt", GetSqGlWinoStyle(),4);
  DoPlotsFor("squark","gluino","GMSB_7TeV_Bino2j",    "2012-06-18-16-57-GMSB_sqgBino_7TeV_2j/filelist.txt", GetSqGlBinoStyle(),4);
  DoPlotsFor("cha1",  "gluino","GMSB_7TeV_Wino2j",    "2012-06-19-13-41-GMSB_gWino_7TeV_2j/filelist.txt",   Get_GlWino_PlotStyle(),4);
  DoPlotsFor("chi1",  "gluino","GMSB_7TeV_Bino2j",    "2012-06-22-09-36-GMSB_gBino_7TeV_2j/filelist.txt",   Get_GlBino_PlotStyle(),4);
  DoPlotsFor("chi1",  "gluino","T1gg2j",	      "2012-05-22-21-38-GMSB_T1gg2j/filelist.txt",GetSMST1ggStyle(),2);
  DoPlotsFor("chi1",  "gluino","T1lg2j",              "2012-05-22-21-45-GMSB_T1lg2j/filelist.txt",GetSMST1lgStyle(),2);
  //MultipleChannels("squark","gluino","GMSB_SingleChannels_Bino2j", "2012-05-11-21-38-GMSBBino375NeutrSingleChannels2j");
  //MultipleChannels("squark","gluino","GMSB_SingleChannels_Wino2j", "2012-05-11-21-38-GMSBWino375NeutrSingleChannels2j");
  }
  //DoPlotsFor("squark","gluino","GMSB_Wino2j","2012-05-09-22-24-GMSBWino375Neutr2j/filelist.txt",GetSqGlWinoStyle(),4);
  //DoPlotsFor("squark","gluino","GMSB_Bino2j","2012-05-09-21-44-GMSBBino375Neutr2j/filelist.txt",GetSqGlBinoStyle(),4);
  //DoPlotsFor("chi1",  "gluino","GMSB_Bino2j","2012-05-09-22-33-GMSB_SquarkGluino_vs_Neutralino2j/filelist.txt",GetGlChiPlotStyle("bino","2500"),4);
  //DoPlotsFor("chi1",  "gluino","T1gg2j",     "2012-05-22-21-38-GMSB_T1gg2j/filelist.txt",GetSMST1ggStyle(),2);
  //DoPlotsFor("chi1",  "gluino","T1lg2j",     "2012-05-22-21-45-GMSB_T1lg2j/filelist.txt",GetSMST1lgStyle(),2);
  //MultipleChannels("squark","gluino","GMSB_SingleChannels_Bino2j", "2012-05-11-21-38-GMSBBino375NeutrSingleChannels2j");
  //MultipleChannels("squark","gluino","GMSB_SingleChannels_Wino2j", "2012-05-11-21-38-GMSBWino375NeutrSingleChannels2j");

  ///Single Photon 8 TeV Summer12 ICHEP
  if (0){ // 4.02 fb-1
  DoPlotsFor("squark","gluino","GMSB_8TeV_Bino2j","2012-06-21-17-05-GMSB_sqgBino375_8TeV_2j/filelist.txt",GetSqGlBinoStyle8TeV(", #geq2 jets"),4);
  DoPlotsFor("squark","gluino","GMSB_8TeV_Wino2j","2012-06-21-17-06-GMSB_sqgWino375_8TeV_2j/filelist.txt",GetSqGlWinoStyle8TeV(", #geq2 jets"),4);
  }
  
  
  
}

int main(int argc, char** argv) {
  return plot(argc, argv);
}
