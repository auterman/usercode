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
#include "TPaveText.h"

#include <iostream>
#include <stdlib.h>
using namespace std;
#include <string>
#include <cmath>
#include <stdio.h>

TCanvas * c1;

template<class T>
void DrawStandardPlots(PlotTools<SusyScan> *PlotTool, std::string flag, double(*x)(const T*), double(*y)(const T*), TH1*h)
{
   //Xsection
   int METCut = 200;
   {TH2F *hxsec = (TH2F*)h->Clone();
   hxsec->GetZaxis()->SetTitle("NLO cross section [pb]");
   PlotTool->Area(hxsec, x, y, NLOXsection);
   hxsec->GetZaxis()->SetTitleOffset(1.5);
   hxsec->Draw("colz");
   drawCmsPrel(PlotTool->SingleValue(Luminosity), METCut);
   string nameXsPlot = "results/" + flag + "_Xsection";
   c1->SaveAs((nameXsPlot + ".pdf").c_str());
   c1->SaveAs((nameXsPlot + ".png").c_str());
   }
   //Acceptance
   {TH2F *hxsec = (TH2F*)h->Clone();
   hxsec->GetZaxis()->SetTitle("Acceptance");
   PlotTool->Area(hxsec, x, y, SignalAcceptance);
   hxsec->GetZaxis()->SetTitleOffset(1.5);
   hxsec->Draw("colz");
   drawCmsPrel(PlotTool->SingleValue(Luminosity), METCut);
   string nameXsPlot = "results/" + flag + "_Acceptance";
   c1->SaveAs((nameXsPlot + ".pdf").c_str());
   c1->SaveAs((nameXsPlot + ".png").c_str());
   }
   //Observed X-section limit
   {TH2F *hxsec = (TH2F*)h->Clone();
   hxsec->GetZaxis()->SetTitle("Observed cross section limit [pb]");
   PlotTool->Area(hxsec, x, y, ObsXsecLimit);
   hxsec->GetZaxis()->SetTitleOffset(1.5);
   hxsec->Draw("colz");
   drawCmsPrel(PlotTool->SingleValue(Luminosity), METCut);
   string nameXsPlot = "results/" + flag + "_ObsXsecLimit";
   c1->SaveAs((nameXsPlot + ".pdf").c_str());
   c1->SaveAs((nameXsPlot + ".png").c_str());
   }
   //Expected X-section limit
   {TH2F *hxsec = (TH2F*)h->Clone();
   hxsec->GetZaxis()->SetTitle("Expected cross section limit [pb]");
   PlotTool->Area(hxsec, x, y, ObsXsecLimit);
   hxsec->GetZaxis()->SetTitleOffset(1.5);
   hxsec->Draw("colz");
   drawCmsPrel(PlotTool->SingleValue(Luminosity), METCut);
   string nameXsPlot = "results/" + flag + "_ExpXsecLimit";
   c1->SaveAs((nameXsPlot + ".pdf").c_str());
   c1->SaveAs((nameXsPlot + ".png").c_str());
   }
   //Observed asymptotoc X-section limit
   {TH2F *hxsec = (TH2F*)h->Clone();
   hxsec->GetZaxis()->SetTitle("Observed asympt. cross section limit [pb]");
   PlotTool->Area(hxsec, x, y, ObsXsecLimitAsym);
   hxsec->GetZaxis()->SetTitleOffset(1.5);
   hxsec->Draw("colz");
   drawCmsPrel(PlotTool->SingleValue(Luminosity), METCut);
   string nameXsPlot = "results/" + flag + "_ObsAsympXsecLimit";
   c1->SaveAs((nameXsPlot + ".pdf").c_str());
   c1->SaveAs((nameXsPlot + ".png").c_str());
   }
   //Expected asymptotic X-section limit
   {TH2F *hxsec = (TH2F*)h->Clone();
   hxsec->GetZaxis()->SetTitle("Expected asympt. cross section limit [pb]");
   PlotTool->Area(hxsec, x, y, ObsXsecLimitAsym);
   hxsec->GetZaxis()->SetTitleOffset(1.5);
   hxsec->Draw("colz");
   drawCmsPrel(PlotTool->SingleValue(Luminosity), METCut);
   string nameXsPlot = "results/" + flag + "_ExpAsympXsecLimit";
   c1->SaveAs((nameXsPlot + ".pdf").c_str());
   c1->SaveAs((nameXsPlot + ".png").c_str());
   }
   
}

template<class T>
void DrawExclusion(PlotTools<SusyScan> *PlotTool, std::string flag, double(*x)(const T*), double(*y)(const T*), TH1*h)
{
   //Xsection
   int METCut = 200;
   {TH2F *hxsec = (TH2F*)h->Clone();
   hxsec->GetZaxis()->SetTitle("Observed in/out");
   PlotTool->Area(hxsec, x, y, ObsExclusion);
   hxsec->GetZaxis()->SetTitleOffset(1.5);
   std::vector<TGraph*> contours = PlotTool->GetContours(hxsec, 3);
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
   	   //if (cont-contours.begin()>3) break;
   }
   drawCmsPrel(PlotTool->SingleValue(Luminosity), METCut);
   string nameXsPlot = "results/" + flag + "ObservedInOut";
   c1->SaveAs((nameXsPlot + ".pdf").c_str());
   c1->SaveAs((nameXsPlot + ".png").c_str());
   }

   {//Exclusion Contours
   c1->SetLogz(0);
   c1->SetRightMargin(0.08);
   c1->SetTopMargin(0.11);
   TH2F *hs = (TH2F*)h->Clone();
   hs->GetZaxis()->SetTitle("");
   TGraph * gCLsObsExcl   = PlotTool->GetContour(hs, Msquark, Mgluino, ObsExclusion, 3, 0, 1, 1);
   TGraph * gCLsExpExcl   = PlotTool->GetContour(hs, Msquark, Mgluino, ExpExclusion, 3, 0, 1, 2);
   TGraph * gCLsExpExclm1 = PlotTool->GetContour(hs, Msquark, Mgluino, ExpExclusionM1, 3, 0, 5, 2);
   TGraph * gCLsExpExclp1 = PlotTool->GetContour(hs, Msquark, Mgluino, ExpExclusionP1, 3, 0, 5, 2);
   gCLsObsExcl->SetLineWidth(2);
   gCLsExpExcl->SetLineWidth(2);
   Smooth(gCLsObsExcl,   12);
   Smooth(gCLsExpExcl,   12);
   Smooth(gCLsExpExclm1, 12);
   Smooth(gCLsExpExclp1, 12);
   TGraph * gCLs1Sigma = MakeBand(gCLsExpExclm1, gCLsExpExclp1);
   gCLs1Sigma->SetFillStyle(3001);
   gCLs1Sigma->SetFillColor(5);
   hs->GetYaxis()->SetTitleOffset(1.55);
   hs->GetXaxis()->SetTitleOffset(1.1);
   hs->Draw("");
   gCLs1Sigma->Draw("f");
   gCLsObsExcl->Draw("l");
   gCLsExpExcl->Draw("l");
   TLegend * leg = new TLegend(0.49, 0.6, 0.93, 0.9);
   leg->SetBorderSize(0);
   leg->SetFillColor(0);
   leg->SetFillStyle(4000);
   leg->SetTextSize(0.025);
   leg->SetHeader("m_{#tilde{#Chi}^{0}} = 375 GeV");
   if (flag == "Wino150" || flag == "Wino100150" || flag == "Bino150") {
   	   leg->SetHeader("m_{#tilde{#Chi}^{0}} = 150 GeV");
   }
   leg->AddEntry(gCLsObsExcl, "Observed, NLO", "l");
   TH1F* legExp = (TH1F*)gCLs1Sigma->Clone();
   legExp->SetLineStyle(gCLsExpExcl->GetLineStyle());
   legExp->SetLineColor(gCLsExpExcl->GetLineColor());
   legExp->SetLineWidth(gCLsExpExcl->GetLineWidth());
   leg->AddEntry(legExp, "Expected #pm 1#sigma, NLO", "lf");
   //leg->AddEntry(gCLs1Sigma, "Expected #pm 1#sigma, NLO", "lf");
   //leg->AddEntry(gCLsExpExcl, "Expected, NLO", "l");
   leg->Draw();
   gPad->RedrawAxis();
   bool bestLimit = false;
   int noJet = 3;
   drawCmsPrel(PlotTool->SingleValue(Luminosity), METCut, false, noJet, bestLimit);
   string nameExcl = "results/" + flag + "_Exclusion_";
   c1->SaveAs((nameExcl + ".pdf").c_str());
   c1->SaveAs((nameExcl + ".png").c_str());
   }

}

void GetPlotTools(PlotTools<SusyScan>*& plotTools, PlotTools<SusyScan>*& plotToolsInterpol, std::string filename)
{
  TheLimits * genpointsInterpol = new TheLimits();
  TheLimits * genpoints = new TheLimits();

  genpointsInterpol->Fill(filename);
  genpoints->Fill(filename);

  //Make grid in Msquark, Mgluino finer by factor of 4 by linear interpolation
  genpointsInterpol->ExpandGrid<SusyScan> (Msquark, Mgluino);
  genpointsInterpol->ExpandGrid<SusyScan> (Msquark, Mgluino);

  // New 'pseudo' points are added, therefore the binning of all plots has to be made finer by a factor
  // of 2 in x and y for each "ExpandGrid
  //the plotting ----------------------------------------------------------------------
  //plotting helper functions
  plotTools = new PlotTools<SusyScan> (genpoints->GetScan());
  plotToolsInterpol = new PlotTools<SusyScan> (genpointsInterpol->GetScan());
} 

int plot(int argc, char** argv) {
  util::StyleSettings::paperNoTitle();
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadBottomMargin(0.18);
  gStyle->SetPadRightMargin(0.18);

  c1 = new TCanvas("c1", "c1", 600, 600);
  c1->cd();

  PlotTools<SusyScan> *Wino, *WinoInterpol;
  GetPlotTools(Wino, WinoInterpol, "2011-11-15-20-21-limits/filelist.txt");

  TH2F h("h",";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); cross section [pb]",
          21, 400, 2020, 21, 400, 2020);
  TH2F hi("hi",";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); cross section [pb]",
          81, 400, 2020, 81, 400, 2020);
  DrawStandardPlots(Wino,"Wino",Msquark,Mgluino,&h);
  DrawExclusion(WinoInterpol,"Wino",Msquark,Mgluino,&hi);
			  
}

int main(int argc, char** argv) {
	return plot(argc, argv);
}
