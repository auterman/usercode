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

#include <vector>
#include <iostream>
#include <stdlib.h>
using namespace std;
#include <string>
#include <cmath>
#include <stdio.h>

namespace filenames{
//ATTENTION: do not forget to add new filenames in main function!!

string file_wino3j="2011-11-25-15-03-GMSBWino375Neutr";
string file_wino2j="2011-11-25-15-05-GMSBWino375Neutr2j";
string file_wino3j_MergedBins="2011-11-27-16-33-GMSBWino375NeutrMerged";

string file_bino3j="2011-11-25-15-00-GMSBBino375Neutr";
string file_bino2j="2011-11-25-15-03-GMSBBino375Neutr2j";

string file_binoNeutr3j="2011-11-25-16-29-GMSB_SquarkGluino_vs_Neutralino";
string file_binoNeutr2j="2011-11-25-16-36-GMSB_SquarkGluino_vs_Neutralino2j";


//Remove files with crashed expected/observed limits?
//find . | grep -l "CLs observed = $" 2011-11-2*/* | while read line; do echo rm $line; done


}

TCanvas * c1;

struct PlotStyles {
	//identifier for signal scan, e.g Wino/Bino/BinoNeutr
	std::string flag;
	std::string flagForComp;
	int noJet;
	bool isBestJet;
	bool drawObsNLOXSErrorBand;
	bool drawGluinoNLSPExclusionRegion;
	string isBestJetFile1;
	string isBestJetFile2;
	std::string METCut;
	string neutralinotype;
	string neutralinomass;
	double lumi;

	double zaxistitleoffset;


};
namespace styles{
PlotStyles ret;
PlotStyles retWino2j;
PlotStyles retWino3j;
PlotStyles retWinoMerged3j;
PlotStyles retWinoBestJet;

PlotStyles retBino2j;
PlotStyles retBino3j;
PlotStyles retBinoBestJet;

PlotStyles retBinoNeutr_Gluino;
PlotStyles retBinoNeutr_Squark;

void setStyles(PlotTools<SusyScan>*& PlotTool){
	ret.flag = "";
	ret.lumi = PlotTool->SingleValue(Luminosity);
	ret.METCut = "100";
	ret.isBestJet = false;
	ret.isBestJetFile1 = "";
	ret.isBestJetFile2 = "";
	ret.noJet = -1;
	ret.zaxistitleoffset = 1.5;
	ret.neutralinotype="";
	ret.neutralinomass="375";
	ret.flagForComp="";
	ret.drawObsNLOXSErrorBand=false;
  ret.drawGluinoNLSPExclusionRegion=false;

	retWino2j = ret;
	retWino2j.flag = "Wino2j";
	retWino2j.noJet = 2;
	retWino2j.neutralinotype="wino";

	retWino3j = ret;
	retWino3j.flag = "Wino3j";
	retWino3j.noJet = 3;
	retWino3j.neutralinotype="wino";

	retWinoMerged3j = ret;
	retWinoMerged3j.flag = "Wino3jMergedBin";
	retWinoMerged3j.noJet = 3;
	retWinoMerged3j.METCut = "200";
	retWinoMerged3j.neutralinotype="wino";

	retWinoBestJet = ret;
	retWinoBestJet.flag = "WinoBestJet";
	retWinoBestJet.noJet = 0;
	retWinoBestJet.METCut = "100";
	retWinoBestJet.isBestJet = true;
	retWinoBestJet.isBestJetFile1 = "3 jet";
	retWinoBestJet.isBestJetFile2 = "2 jet";
	retWinoBestJet.neutralinotype="wino";
	retWinoBestJet.flagForComp="Wino_2_vs_3_jet";

	retBino2j = ret;
	retBino2j.flag = "Bino2j";
	retBino2j.noJet = 2;
	retBino2j.neutralinotype="bino";

	retBino3j = ret;
	retBino3j.flag = "Bino3j";
	retBino3j.noJet = 3;
	retBino3j.neutralinotype="bino";

	retBinoBestJet =retWinoBestJet;
	retBinoBestJet.flag = "BinoBestJet";
	retBinoBestJet.neutralinotype="bino";
	retBinoBestJet.flagForComp="Bino_2_vs_3_jet";

	retBinoNeutr_Gluino=ret;
	retBinoNeutr_Gluino.flag = "BinoNeutr_gl_3j";
	retBinoNeutr_Gluino.noJet = 3;
	retBinoNeutr_Gluino.neutralinotype="bino";
	retBinoNeutr_Gluino.drawGluinoNLSPExclusionRegion=true;

	retBinoNeutr_Squark=retBinoNeutr_Gluino;
	retBinoNeutr_Squark.flag = "BinoNeutr_sq_3j";
}

}
using namespace styles;

//redo filelists
//ls 2011-11-25-15-00-GMSBBino375Neutr/* > 2011-11-25-15-00-GMSBBino375Neutr/filelist.txt
void redoFileList(vector<string> filenames){
	for(int i=0;i<(int)filenames.size();++i){
	std::system(("ls "+filenames.at(i)+"/* > "+filenames.at(i)+"/filelist.txt").c_str());
	}
}

struct ExclusionCurves {
	TGraph * obs;

	TGraph * exp;
	TGraph * onesigband;
	TGraph * onesigbandNLOXS;

};
void setExclusionStyles(ExclusionCurves * excl, bool isSecondCurve) {
	excl->exp->SetLineWidth(2);
	excl->exp->SetLineStyle(2);
	excl->obs->SetLineWidth(2);
	excl->onesigband->SetLineWidth(2);
	if (!isSecondCurve) {
		excl->onesigband->SetFillColor(42);
		excl->onesigband->SetLineColor(42);
	} else {
		excl->onesigband->SetFillColor(5);
		excl->onesigband->SetFillStyle(3004);
		excl->onesigband->SetLineColor(5);
		excl->exp->SetLineColor(2);
		excl->obs->SetLineColor(2);
	}
}
template<class T>
void DrawStandardPlots(PlotTools<SusyScan> *PlotTool, PlotStyles style, double(*x)(const T*), double(*y)(const T*), TH1*h)
{
	std::system(("mkdir results/" + style.flag).c_str());

	gStyle->SetPadTopMargin(0.1);
	gStyle->SetPadBottomMargin(0.15);
	gStyle->SetPadRightMargin(0.2);
	gStyle->SetTitleOffset(1.3,"xyz");
	gStyle->SetTitleOffset(2.1,"y");
	gStyle->SetNdivisions(505);
	gStyle->SetTitleSize(0.04,"xyz");


	c1->UseCurrentStyle();
	h->UseCurrentStyle();

   //Xsection
   {TH2F *hxsec = (TH2F*)h->Clone();
   hxsec->GetZaxis()->SetTitle("NLO cross section [pb]");
   c1->SetLogz(1);
      hxsec->GetZaxis()->SetRangeUser(0.001,100);
   PlotTool->Area(hxsec, x, y, NLOXsection);
   hxsec->GetZaxis()->SetTitleOffset(1.5);
   hxsec->Draw("colz");
   if (style.drawGluinoNLSPExclusionRegion) DrawNeutrNNLSP();
   drawCmsPrel(style.lumi,style.METCut,style.noJet,style.isBestJet);
   string nameXsPlot = "results/" + style.flag + "/Xsection";
   c1->SaveAs((nameXsPlot + ".pdf").c_str());
   //c1->SaveAs((nameXsPlot + ".root").c_str());
   c1->SaveAs((nameXsPlot + ".png").c_str());
   }
   //Acceptance
   {TH2F *hxsec = (TH2F*)h->Clone("bla");
   hxsec->GetZaxis()->SetTitle("Acceptance [%]");
   c1->SetLogz(0);
   PlotTool->Area(hxsec, x, y, SignalAcceptance);
   hxsec->GetZaxis()->SetTitleOffset(1.5);
   hxsec->Draw("colz");
   if (style.drawGluinoNLSPExclusionRegion) DrawNeutrNNLSP();
   drawCmsPrel(style.lumi,style.METCut,style.noJet,style.isBestJet);
   string nameXsPlot = "results/" + style.flag + "/Acceptance";
   c1->SaveAs((nameXsPlot + ".pdf").c_str());
   c1->SaveAs((nameXsPlot + ".png").c_str());
   delete hxsec;
   }
   //Observed X-section limit
   {TH2F *hxsec = (TH2F*)h->Clone();
   hxsec->GetZaxis()->SetTitle("Observed cross section limit [pb]");
   c1->SetLogz(1);
   hxsec->GetZaxis()->SetRangeUser(0.001,100);
   PlotTool->Area(hxsec, x, y, ObsXsecLimit);
   hxsec->GetZaxis()->SetTitleOffset(1.5);
   hxsec->Draw("colz");
   if (style.drawGluinoNLSPExclusionRegion) DrawNeutrNNLSP();
   drawCmsPrel(style.lumi,style.METCut,style.noJet,style.isBestJet);
   string nameXsPlot = "results/" + style.flag + "/ObsXsecLimit";
   c1->SaveAs((nameXsPlot + ".pdf").c_str());
   c1->SaveAs((nameXsPlot + ".png").c_str());
   c1->SaveAs((nameXsPlot + ".root").c_str());
   }
   //Expected X-section limit
   {TH2F *hxsec = (TH2F*)h->Clone();
   hxsec->GetZaxis()->SetTitle("Expected cross section limit [pb]");
   c1->SetLogz(1);
      hxsec->GetZaxis()->SetRangeUser(0.001,100);
   PlotTool->Area(hxsec, x, y, ExpXsecLimit);
   hxsec->GetZaxis()->SetTitleOffset(1.5);
   hxsec->Draw("colz");
   if (style.drawGluinoNLSPExclusionRegion) DrawNeutrNNLSP();
   drawCmsPrel(style.lumi,style.METCut,style.noJet,style.isBestJet);
   string nameXsPlot = "results/" + style.flag + "/ExpXsecLimit";
   c1->SaveAs((nameXsPlot + ".pdf").c_str());
   c1->SaveAs((nameXsPlot + ".png").c_str());
   }
   //Observed asymptotoc X-section limit
   {TH2F *hxsec = (TH2F*)h->Clone();
   hxsec->GetZaxis()->SetTitle("Observed asympt. cross section limit [pb]");
   c1->SetLogz(1);
      hxsec->GetZaxis()->SetRangeUser(0.001,100);
   PlotTool->Area(hxsec, x, y, ObsXsecLimitAsym);
   hxsec->GetZaxis()->SetTitleOffset(1.5);
   hxsec->Draw("colz");
   if (style.drawGluinoNLSPExclusionRegion) DrawNeutrNNLSP();
   drawCmsPrel(style.lumi,style.METCut,style.noJet,style.isBestJet);
   string nameXsPlot = "results/" + style.flag + "/ObsAsympXsecLimit";
   c1->SaveAs((nameXsPlot + ".pdf").c_str());
   c1->SaveAs((nameXsPlot + ".png").c_str());
   }
   //Expected asymptotic X-section limit
   {TH2F *hxsec = (TH2F*)h->Clone();
   hxsec->GetZaxis()->SetTitle("Expected asympt. cross section limit [pb]");
   c1->SetLogz(1);
      hxsec->GetZaxis()->SetRangeUser(0.001,100);
   PlotTool->Area(hxsec, x, y, ExpXsecLimitAsym);
   hxsec->GetZaxis()->SetTitleOffset(1.5);
   hxsec->Draw("colz");
   if (style.drawGluinoNLSPExclusionRegion) DrawNeutrNNLSP();
   drawCmsPrel(style.lumi,style.METCut,style.noJet,style.isBestJet);
   string nameXsPlot = "results/" + style.flag + "/ExpAsympXsecLimit";
   c1->SaveAs((nameXsPlot + ".pdf").c_str());
   c1->SaveAs((nameXsPlot + ".png").c_str());
   }
   c1->SetLogz(0);
   //Only if "Best Limit" is drawn, i.e. 2 or 3 jets selection
	{if (style.isBestJet) {
      TH2F *hxsec = (TH2F*) h->Clone();
      hxsec->GetZaxis()->SetTitle("");
      c1->SetLogz(0);
      PlotTool->Area(hxsec, x, y, BestLimitFromFile1or2);
      hxsec->Draw("col");
      if (style.drawGluinoNLSPExclusionRegion) DrawNeutrNNLSP();
      drawCmsPrel(style.lumi, style.METCut, style.noJet, style.isBestJet);
      TLatex as;
      	as.SetNDC(true);
      	as.SetTextFont(62);
      	as.SetTextSize(0.032);
      	as.SetTextColor(1);
      	as.DrawLatex(0.81, 0.83,"Selection for");
      	as.DrawLatex(0.81, 0.8,"best expected");
      	as.DrawLatex(0.81, 0.77,"limit:");
      	as.SetTextColor(2);
      	std::string legendRed = style.isBestJetFile2;
      	as.DrawLatex(0.83, 0.72,legendRed.c_str());
      	as.SetTextColor(4);
      	std::string legendBlue = style.isBestJetFile1;
      	as.DrawLatex(0.83, 0.67,legendBlue.c_str());

      string nameXsPlot = "results/" + style.flag + "/BestLimitFromFile1or2";
      c1->SaveAs((nameXsPlot + ".pdf").c_str());
      c1->SaveAs((nameXsPlot + ".png").c_str());
	 }
	}

}


void DrawExclusion(ExclusionCurves * exclA,PlotStyles * style, TH1*h,ExclusionCurves * exclB=0)
{
	if (exclA == 0) {
		cout << "No Exclusion Cruves To Draw...exclA is 0" << endl;
		return;
	};
	if (exclA->exp == 0 || exclA->obs == 0 || exclA->onesigband == 0|| exclA->onesigbandNLOXS == 0) {
		cout << "No Exclusion Cruves To Draw...exclA is not properly filled" << endl;
				return;
	}
	bool drawComp = false;
	if (exclB != 0){
		drawComp = true;
	}
	if (!drawComp)
		std::system(("mkdir results/" + style->flag).c_str());
	else {
		std::system(("mkdir results/Comp_" + style->flagForComp ).c_str());
	}
	//set styles
	gStyle->SetPadTopMargin(0.1);
	gStyle->SetPadBottomMargin(0.15);
	gStyle->SetPadRightMargin(0.08);
	gStyle->SetTitleOffset(1.3, "xyz");
	gStyle->SetTitleOffset(2.1, "y");
	gStyle->SetNdivisions(505);
	gStyle->SetTitleSize(0.04, "xyz");

	c1->UseCurrentStyle();
	h->UseCurrentStyle();

	//set styles of Graphs
	setExclusionStyles(exclA, false);
	if (drawComp) {
		setExclusionStyles(exclB, true);
	}
   {//Exclusion Contours
   TH2F *hs = (TH2F*)h->Clone();
   hs->GetZaxis()->SetTitle("");

   TGraph * gCLsObsExcl   = exclA->obs;
   TGraph * gCLsExpExcl   = exclA->exp;
   TGraph * gCLs1Sigma = exclA->onesigband;
   if(style->drawObsNLOXSErrorBand)
	   gCLs1Sigma = exclA->onesigbandNLOXS;
   //Draw Graphs
   hs->Draw("");
   gCLs1Sigma->Draw("f");
   gCLsObsExcl->Draw("l");
   gCLsExpExcl->Draw("l");
   //Draw additional graphs
   if (drawComp) {

			if (style->drawObsNLOXSErrorBand) {
				exclB->onesigbandNLOXS->Draw("f");
			} else {
				exclB->onesigband->Draw("f");
			}
			exclB->obs->Draw("l");
			exclB->exp->Draw("l");
		}

   if (style->drawGluinoNLSPExclusionRegion) {
  	 DrawNeutrNNLSP();
		}
   //Draw Legend
   string neutralinotype=style->neutralinotype;
   string neutralinomass=style->neutralinomass;
   string legtitle="#splitline{GGM "+neutralinotype
		+ "-like #tilde{#Chi}^{0}}{m_{#tilde{#Chi}^{0}} = "+neutralinomass+" (GeV/c^{2})}";
   TLegend * leg = new TLegend(0.57, 0.6, 0.89, 0.87,(legtitle).c_str());
   TH1F * legdummy=0;
   //leg->AddEntry(legdummy,  ("m_{#tilde{#Chi}^{0}} = "+neutralinomass+" (GeV/c^{2})").c_str(), "l");
   leg->SetBorderSize(0);
   leg->SetFillColor(0);
   leg->SetFillStyle(4000);
   leg->SetTextSize(0.035);

   if (!drawComp) {
    leg->AddEntry(gCLsObsExcl, "Observed, NLO", "l");
    leg->AddEntry(gCLsExpExcl, "Expected, NLO", "l");
    leg->AddEntry(gCLs1Sigma, "#pm 1#sigma, NLO", "lf");
   }
   else{
	   leg->AddEntry(exclA->obs, (style->isBestJetFile1+", Observed, NLO").c_str(), "l");
	   leg->AddEntry(exclA->exp, (style->isBestJetFile1+", Expected, NLO").c_str(), "l");
	   leg->AddEntry(exclA->onesigband, (style->isBestJetFile1+", #pm 1#sigma, NLO").c_str(), "f");
	   leg->AddEntry(exclB->obs, (style->isBestJetFile2+", Observed, NLO").c_str(), "l");
	   leg->AddEntry(exclB->exp, (style->isBestJetFile2+", Expected, NLO").c_str(), "l");
	   leg->AddEntry(exclB->onesigband, (style->isBestJetFile2+", #pm 1#sigma, NLO").c_str(), "f");
   }
   leg->Draw();
   gPad->RedrawAxis();

   if(!drawComp){
    //Draw Excluded region
    double min1 = hs->GetXaxis()->GetBinLowEdge(0);
    double min2 = hs->GetYaxis()->GetBinLowEdge(0);
    double max2 = hs->GetYaxis()->GetBinLowEdge(hs->GetNbinsX() + 1);
    double max1 = hs->GetXaxis()->GetBinLowEdge(hs->GetNbinsX() + 1);
    TGraph * excludedRegion = GetExcludedRegion(exclA->obs, min1, min2, max1, max2);
    excludedRegion->Draw("f");
    TLatex* lat3 = new TLatex(0.2, 0.25, "Excluded");
    lat3->SetNDC(true);
    lat3->Draw();
   }

   //draw heading
   drawCmsPrel(style->lumi,style->METCut,style->noJet,style->isBestJet);

   string nameExcl = "results/" + style->flag + "/Exclusion_";
   if (drawComp) {
    nameExcl="results/Comp_" + style->flagForComp + "/Exclusion_";
   }
   c1->SaveAs((nameExcl + ".pdf").c_str());
   c1->SaveAs((nameExcl + ".png").c_str());
   }

}

template<class T>
ExclusionCurves GetExclusionContours(PlotTools<SusyScan> *PlotTool,PlotStyles  style, double(*x)(const T*), double(*y)(const T*), TH1*h)
{
   //Xsection
	std::system(("mkdir results/" + style.flag).c_str());
   {TH2F *hxsec = (TH2F*)h->Clone();
   hxsec->GetZaxis()->SetTitle("Observed in/out");
   PlotTool->Area(hxsec, x, y, ExpExclusion);
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
   	   l.SetTextSize(0.035);
   	   l.SetTextColor(col++);
   	   char val[20];
   	   sprintf(val, "%d", (int) (cont - contours.begin()));
   	   l.DrawLatex(x, y, val);
   	   //if (cont-contours.begin()>3) break;
   }
   drawCmsPrel(style.lumi,style.METCut,style.noJet,style.isBestJet);
   string nameXsPlot = "results/" + style.flag + "/ObservedInOut";
   c1->SaveAs((nameXsPlot + ".pdf").c_str());
   c1->SaveAs((nameXsPlot + ".png").c_str());
   }

   {//Exclusion Contours
   TH2F *hs = (TH2F*)h->Clone();
   hs->GetZaxis()->SetTitle("");
   TGraph * gCLsObsExcl   = PlotTool->GetContour(hs, x, y, ObsExclusion, 3, 0, 1, 1);
   TGraph * gCLsExpExcl   = PlotTool->GetContour(hs, x, y, ExpExclusion, 3, 0, 1, 2);
   TGraph * gCLsExpExclm1 = PlotTool->GetContour(hs, x, y, ExpExclusionM1, 3, 0, 5, 2);
   TGraph * gCLsExpExclp1 = PlotTool->GetContour(hs, x, y, ExpExclusionP1, 3, 0, 5, 2);
   TGraph * gCLsObsExclm1 = PlotTool->GetContour(hs, x, y, ObsExclusionM1, 3, 0, 5, 2);
   TGraph * gCLsObsExclp1 = PlotTool->GetContour(hs, x, y, ObsExclusionP1, 3, 0, 5, 2);
   Smooth(gCLsObsExcl,   12);
   Smooth(gCLsExpExcl,   12);
   Smooth(gCLsExpExclm1, 12);
   Smooth(gCLsExpExclp1, 12);
   Smooth(gCLsObsExclm1, 12);
   Smooth(gCLsObsExclp1, 12);
   TGraph * gCLs1Sigma = MakeBand(gCLsExpExclm1, gCLsExpExclp1);
   TGraph * gCLs1SigmaObsNLO = MakeBand(gCLsObsExclm1, gCLsObsExclp1);
   ExclusionCurves ret;
   ret.exp=gCLsExpExcl;

   ret.obs=gCLsObsExcl;
   ret.onesigband=gCLs1Sigma;
   ret.onesigbandNLOXS=gCLs1SigmaObsNLO;
   return ret;
   }

}
void GetBestExpectedPlotTools(PlotTools<SusyScan>*& plotTools, PlotTools<SusyScan>*& plotToolsInterpol, std::string filename, std::string filename2, bool isNeutrGluinoScan=false)
{
	filename=filename+"/filelist.txt";
	filename2=filename2+"/filelist.txt";
  TheLimits * genpointsInterpol = new TheLimits();
  TheLimits * genpoints = new TheLimits();
  //std::cout<<"fill interpol"<<std::endl;
  genpointsInterpol->FillBestExpectedLimit(filename,filename2);
 // std::cout<<"fill non interpol"<<std::endl;
  genpoints->FillBestExpectedLimit(filename,filename2);
  //std::cout<<"interpol..."<<std::endl;
  if (isNeutrGluinoScan) {
		genpointsInterpol->ExpandGrid<SusyScan> (Mchi1, Mgluino);
		genpointsInterpol->ExpandGrid<SusyScan> (Mchi1, Mgluino);
	} else {
		//Make grid in Msquark, Mgluino finer by factor of 4 by linear interpolation
		genpointsInterpol->ExpandGrid<SusyScan> (Msquark, Mgluino);
		genpointsInterpol->ExpandGrid<SusyScan> (Msquark, Mgluino);
	}
  //std::cout<<"done"<<std::endl;
  // New 'pseudo' points are added, therefore the binning of all plots has to be made finer by a factor
  // of 2 in x and y for each "ExpandGrid
  //the plotting ----------------------------------------------------------------------
  //plotting helper functions
  //std::cout<<"try to get scan"<<std::endl;
  plotTools = new PlotTools<SusyScan> (genpoints->GetScan());
  //std::cout<<"done -try to get next scan"<<std::endl;
  plotToolsInterpol = new PlotTools<SusyScan> (genpointsInterpol->GetScan());
  //std::cout<<"done - return"<<std::endl;
}

void GetPlotTools(PlotTools<SusyScan>*& plotTools, PlotTools<SusyScan>*& plotToolsInterpol, std::string filename, bool isNeutrGluinoScan=false, bool isNeutrSquarkScan=false)
{
	filename=filename+"/filelist.txt";
  TheLimits * genpointsInterpol = new TheLimits();
  TheLimits * genpoints = new TheLimits();
  cout << "Fill interpol" << endl;
  genpointsInterpol->Fill(filename);
  cout << "Fill " << endl;
  genpoints->Fill(filename);
  cout << "interpol..." << endl;
	if (isNeutrGluinoScan) {
		genpointsInterpol->ExpandGrid<SusyScan> (Mchi1, Mgluino);
		genpointsInterpol->ExpandGrid<SusyScan> (Mchi1, Mgluino);
	}else if (isNeutrSquarkScan) {
		genpointsInterpol->ExpandGrid<SusyScan> (Mchi1, Msquark);
		genpointsInterpol->ExpandGrid<SusyScan> (Mchi1, Msquark);
	} else {
		//Make grid in Msquark, Mgluino finer by factor of 4 by linear interpolation
		genpointsInterpol->ExpandGrid<SusyScan> (Msquark, Mgluino);
		genpointsInterpol->ExpandGrid<SusyScan> (Msquark, Mgluino);
	}
	cout << "done" << endl;

  // New 'pseudo' points are added, therefore the binning of all plots has to be made finer by a factor
  // of 2 in x and y for each "ExpandGrid
  //the plotting ----------------------------------------------------------------------
  //plotting helper functions
  plotTools = new PlotTools<SusyScan> (genpoints->GetScan());
  plotToolsInterpol = new PlotTools<SusyScan> (genpointsInterpol->GetScan());
} 

int plot(int argc, char** argv) {
  util::StyleSettings::paperNoTitle();



  c1 = new TCanvas("c1", "c1", 600, 600);
  c1->cd();
  TH2F h("h", ";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); cross section [pb]", 21, 360, 2040, 21, 360, 2040);
	TH2F hi("hi", ";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); cross section [pb]", 81, 380, 2020, 81, 380, 2020);
	//TODO binning korrigieren!!!
	TH2F hNeutrGluino("hNeutrGluino", ";m_{#chi^{0}_{1}} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); cross section [pb]", 9, 150, 1050.1, 24, 160, 2000.1);

	TH2F hNeutrGluinoi("hNeutrGluinoi", ";m_{#chi^{0}_{1}} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); cross section [pb]", 33, 150, 1050.1, 93, 160, 2000.1);

	TH2F hNeutrSquark("hNeutrSquark", ";m_{#chi^{0}_{1}} (GeV/c^{2}); m_{squark} (GeV/c^{2}); cross section [pb]", 9, 150, 1050.1, 24, 160, 2000.1);

	TH2F hNeutrSquarki("hNeutrSquarki", ";m_{#chi^{0}_{1}} (GeV/c^{2}); m_{squark} (GeV/c^{2}); cross section [pb]", 33, 150, 1050.1, 93, 160, 2000.1);


  PlotTools<SusyScan> *Scan, *ScanInterpol;


  ///==================Bino-Neutr vs Gluino Limits 3j================================
	GetPlotTools(Scan, ScanInterpol, filenames::file_binoNeutr3j, true);
	setStyles(Scan);
	DrawStandardPlots(Scan, retBinoNeutr_Gluino, Mchi1, Mgluino, &hNeutrGluino);
	ExclusionCurves exclBinoNeutr_gl_3j = GetExclusionContours(ScanInterpol, retBinoNeutr_Gluino, Mchi1, Mgluino, &hNeutrGluinoi);
	DrawExclusion(&exclBinoNeutr_gl_3j, &retBinoNeutr_Gluino, &hNeutrGluinoi);

//	///==================Bino-Neutr vs Squark Limits 3j================================
//	GetPlotTools(Scan, ScanInterpol, filenames::file_binoNeutr3j, false, true);
//	setStyles(Scan);
//	DrawStandardPlots(Scan, retBinoNeutr_Squark, Mchi1, Msquark, &hNeutrSquark);
//	ExclusionCurves exclBinoNeutr_sq_3j = GetExclusionContours(ScanInterpol, retBinoNeutr_Squark, Mchi1, Msquark, &hNeutrSquark);
//	DrawExclusion(&exclBinoNeutr_sq_3j, &retBinoNeutr_Squark, &hNeutrSquarki);

	///==================Bino Limits 3j================================
	cout << "Get Bino 3j!!" << endl;
	GetPlotTools(Scan, ScanInterpol, filenames::file_bino3j);
	cout << "Get PlotTools done!!" << endl;
	setStyles(Scan);
	cout << "Draw plots" << endl;
	DrawStandardPlots(Scan, retBino3j, Msquark, Mgluino, &h);
	cout << "GetExclusion" << endl;
	ExclusionCurves exclBino3j = GetExclusionContours(ScanInterpol, retBino3j, Msquark, Mgluino, &hi);
	cout << "Draw Exclusion" << endl;
	DrawExclusion(&exclBino3j, &retBino3j, &hi);

	///==================Bino Limits 2j================================
	GetPlotTools(Scan, ScanInterpol, filenames::file_bino2j);
	setStyles(Scan);
	DrawStandardPlots(Scan, retBino2j, Msquark, Mgluino, &h);
	ExclusionCurves exclBino2j = GetExclusionContours(ScanInterpol, retBino2j, Msquark, Mgluino, &hi);
	DrawExclusion(&exclBino2j, &retBino2j, &hi);

	///==================Bino Limits 2/3j - Best Expected ================================
	cout << "Get Best Expected!!" << endl;
	GetBestExpectedPlotTools(Scan, ScanInterpol, filenames::file_bino3j, filenames::file_bino2j);
	cout << "Get Best Expected 2!!" << endl;
	setStyles(Scan);
	DrawStandardPlots(Scan, retBinoBestJet, Msquark, Mgluino, &h);
	ExclusionCurves exclBinoBestJet = GetExclusionContours(ScanInterpol, retBinoBestJet, Msquark, Mgluino, &hi);
	DrawExclusion(&exclBinoBestJet, &retBinoBestJet, &hi);
	//Compare Exclusion between 2/3 jet
	DrawExclusion(&exclBino2j, &retBinoBestJet, &hi, &exclBino3j);

	///==================Wino Limits 3j================================
	GetPlotTools(Scan, ScanInterpol, filenames::file_wino3j);
	setStyles(Scan);
	DrawStandardPlots(Scan, retWino3j, Msquark, Mgluino, &h);
	ExclusionCurves exclWino3j = GetExclusionContours(ScanInterpol, retWino3j, Msquark, Mgluino, &hi);
	DrawExclusion(&exclWino3j, &retWino3j, &hi);

	///==================Wino Limits 2j================================
	GetPlotTools(Scan, ScanInterpol, filenames::file_wino2j);
	setStyles(Scan);
	DrawStandardPlots(Scan, retWino2j, Msquark, Mgluino, &h);
	ExclusionCurves exclWino2j = GetExclusionContours(ScanInterpol, retWino2j, Msquark, Mgluino,
														&hi);
	DrawExclusion(&exclWino2j, &retWino2j, &hi);

	///==================Wino Limits 2/3j - Best Expected ================================
	//cout<<"Get Best Expected!!"<<endl;
	GetBestExpectedPlotTools(Scan, ScanInterpol, filenames::file_wino3j, filenames::file_wino2j);

	setStyles(Scan);
	DrawStandardPlots(Scan, retWinoBestJet, Msquark, Mgluino, &h);
	ExclusionCurves exclWinoBestJet = GetExclusionContours(ScanInterpol, retWinoBestJet, Msquark,
														Mgluino, &hi);
	DrawExclusion(&exclWinoBestJet, &retWinoBestJet, &hi);
	//Compare Exclusion between 2/3 jet
	DrawExclusion(&exclWino2j, &retWinoBestJet, &hi,&exclWino3j);

	///==================Wino Limits >200 (bin4-7 gemerged)================================
	GetPlotTools(Scan, ScanInterpol, filenames::file_wino3j_MergedBins);
	setStyles(Scan);
	DrawStandardPlots(Scan, retWinoMerged3j, Msquark, Mgluino, &h);
	ExclusionCurves exclWino1Bin = GetExclusionContours(ScanInterpol, retWinoMerged3j, Msquark,
														Mgluino, &hi);
	DrawExclusion(&exclWino1Bin, &retWinoMerged3j, &hi);



}

int main(int argc, char** argv) {
	vector<string> filenames;
	filenames.push_back(filenames::file_wino3j);
	filenames.push_back(filenames::file_wino2j);
	filenames.push_back(filenames::file_wino3j_MergedBins);
	filenames.push_back(filenames::file_bino3j);
	filenames.push_back(filenames::file_bino2j);
	filenames.push_back(filenames::file_binoNeutr3j);
	filenames.push_back(filenames::file_binoNeutr2j);

	redoFileList(filenames);
	return plot(argc, argv);
}
