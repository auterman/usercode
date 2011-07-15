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

#include <string>
#include <cmath>
#include <stdio.h>

int plot(int argc, char** argv) {
	//interactive root session
	//TApplication theApp("App", 0, 0);
	if (gROOT->IsBatch()) {
		fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
		return 1;
	}

	util::StyleSettings::paperNoTitle();
	gStyle->SetPadBottomMargin(0.18);

	TCanvas * c1 = new TCanvas("c1", "c1", 600, 600);
	c1->cd();

	//Get limits from signal scan ---------------------------------------------------
	TheLimits * genpoints = new TheLimits();
	genpoints->Fill("limits_GMSB/resultfiles.txt");
	
	//Make grid in Msquark, Mgluino finer by factor of 4 by linear interpolation
	genpoints->ExpandGrid<SusyScan>(Msquark, Mgluino); 
	genpoints->ExpandGrid<SusyScan>(Msquark, Mgluino); 
	// New 'pseudo' points are added, therefore the binning of all plots has to be made finer by a factor 
	// of 2 in x and y for each "ExpandGrid"

	//the plotting ----------------------------------------------------------------------
	//plotting helper functions
	PlotTools < SusyScan > *plotTools = new PlotTools<SusyScan> ( genpoints->GetScan() );


	//the histograms ---------------------------------------------------------------------
	c1->SetLogz(1);

	// cross-section in M0 - M1/2
	c1->SetRightMargin(0.2);
	TH2F*hxsec = new TH2F("xsec",
			";m_{squark} [GeV]; m_{gluino} [GeV]; cross section [pb]", 
			20, 400, 2000, 20, 400, 2000);
	plotTools->Area(hxsec, Msquark, Mgluino, Xsection);
	hxsec->SetMinimum(0.0001);
	//sq500->Draw();   // m(squark) = 500 GeV line
	//gl500->Draw();   // m(gluino) = 500 GeV line
	hxsec->Draw("colz");
	c1->SaveAs("results/Xsection_3jet.pdf");
	c1->SaveAs("results/Xsection_3jet.eps");

	// Signal Acceptance in M0 - M1/2
	c1->SetRightMargin(0.2);
	c1->SetTopMargin(0.11);
	c1->SetLogz(0);
	TH2F*hsigacc = new TH2F("sigacc",
			";m_{squark} [GeV]; m_{gluino} [GeV]; Signal Acceptance", 
			20, 400, 2000, 20, 400, 2000);
	plotTools->Area(hsigacc, Msquark, Mgluino, SignalAcceptance);
	hsigacc->SetMinimum(0.0);
	hsigacc->SetMaximum(0.55);
	hsigacc->SetContour(14);
	hsigacc->GetZaxis()->SetTitleOffset(1.5);
	hsigacc->Draw("colz");
	TLatex as;
	as.SetTextSize(0.025);
	as.SetTextFont(42);//ms.SetTextColor(12);
	as.DrawLatex(680, 2060, "=1#gamma, >=3 jets, MET>100");
	as.SetTextSize(0.04);
	as.DrawLatex(50, 2060, "CMS preliminary");
	c1->SaveAs("results/SigAcc3jet.pdf");
	c1->SaveAs("results/SigAcc3jet.eps");

	// Signal Contamination in M0 - M1/2
	c1->SetRightMargin(0.2);
	c1->SetTopMargin(0.11);
	c1->SetLogz(0);
	TH2F*hsigcont = new TH2F("sigacc",
			";m_{squark} [GeV]; m_{gluino} [GeV]; Signal Contamination", 
			20, 400, 2000, 20, 400, 2000);
	plotTools->Area(hsigcont, Msquark, Mgluino, SignalContamination);
	hsigcont->SetMinimum(0.0);
	hsigcont->SetMaximum(1);
	hsigcont->SetContour(14);
	hsigcont->GetZaxis()->SetTitleOffset(1.5);
	hsigcont->Draw("colz");

	c1->SaveAs("results/SigCont3jet.pdf");
	c1->SaveAs("results/SigCont3jet.eps");


	// Observed Limit in M0 - M1/2
	TH2F*hobslimit = new TH2F("obslimit",
			";m_{squark} [GeV]; m_{gluino} [GeV]; 95% CL Observed Limit [pb]",
			20, 400, 2000, 20, 400, 2000);
	plotTools->Area(hobslimit, Msquark, Mgluino, ObsXsecLimit);
	hobslimit->SetMinimum(0.03);
	hobslimit->Draw("colz");
	c1->SaveAs("results/ObsLimit_3jet.pdf");
	c1->SaveAs("results/ObsLimit_3jet.eps");

	// Expected Limit in M0 - M1/2
	TH2F*hexplimit = new TH2F("explimit",
			";m_{squark} [GeV]; m_{gluino} [GeV]; 95% CL Expected Limit [pb]",
			20, 400, 2000, 20, 400, 2000);
	plotTools->Area(hexplimit, Msquark, Mgluino, ExpXsecLimit);
	hexplimit->SetMinimum(0.03);
	hexplimit->Draw("colz");
	c1->SaveAs("results/ExpLimit_3jet.pdf");
	c1->SaveAs("results/ExpLimit_3jet.eps");

	// Observed Limit in M0 - M1/2
	TH2F*hobslimitSigCont = new TH2F("obslimitSigCont",
			";m_{squark} [GeV]; m_{gluino} [GeV]; 95% CL Observed Limit [pb]",
			20, 400, 2000, 20, 400, 2000);
	plotTools->Area(hobslimitSigCont, Msquark, Mgluino, ObsXsecLimitSigCont);
	hobslimitSigCont->SetMinimum(0.03);
	hobslimitSigCont->Draw("colz");
	c1->SaveAs("results/ObsLimitSigCont_3jet.pdf");
	c1->SaveAs("results/ObsLimitSigCont_3jet.eps");

	// Expected Limit in M0 - M1/2
	TH2F*hexplimitSigCont = new TH2F("explimitSigCont",
			";m_{squark} [GeV]; m_{gluino} [GeV]; 95% CL Expected Limit [pb]",
			20, 400, 2000, 20, 400, 2000);
	plotTools->Area(hexplimitSigCont, Msquark, Mgluino, ExpXsecLimitSigCont);
	hexplimitSigCont->SetMinimum(0.03);
	hexplimitSigCont->Draw("colz");
	c1->SaveAs("results/ExpLimitSigCont_3jet.pdf");
	c1->SaveAs("results/ExpLimitSigCont_3jet.eps");

	// TestContours in M0 - M1/2 /////////////////////////////////////////////////////////////////////
	c1->SetRightMargin(0.1);
	TH2F*TestCont = new TH2F("texcl",
			";m_{squark} [GeV]; m_{gluino} [GeV]; 95% CL Expected Exclusion",
			80, 400, 2000, 80, 400, 2000);
	/// Find the correct contour for any of the exclusion plots--->get right contour number!::::::::::::::::::::::
	plotTools->Area(TestCont, Msquark, Mgluino, ExpExclusionM1);
	/// try "3" contour levels:
	std::vector<TGraph*> contours = plotTools->GetContours(TestCont, 3);
	/// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	TestCont->Draw("colz");
	int col = kBlue - 10;
	for (std::vector<TGraph*>::iterator cont = contours.begin(); cont
			!= contours.end(); ++cont) {
		if (!*cont)
			continue;
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
	c1->SaveAs("results/ExclusionTestContoursExp_3jet.pdf");
	TFile * output = new TFile("output.root", "Update");
	//  //////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Exclusion in M0 - M1/2 =================================================================================
	// hexcl is drawn to define axis range and labels:
	TH2F*hexcl = new TH2F("hexcl",
			";m_{squark} [GeV]; m_{gluino} [GeV]; 95% CL Exclusion", 
			80, 400, 2000, 80, 400, 2000);
	// hs defined range in which the contrours are calculated, the binning matters! binning should be equal to signal-scan
	TH2F*hs = new TH2F("hs", "", 80, 400, 2000, 80, 400, 2000);
	// usage: GetContour( <range-hist>, <x-var>, <y-var>, <limit-function>, <contour-levels>, <contour index>, <color>, <style> )

	TGraph * gCLsExpExclMHT_LO = plotTools->GetContour(hs, Msquark, Mgluino,
			ExpExclusion, 3, 2, 2, 4);
	TGraph * gCLsObsExclMHT_LO = plotTools->GetContour(hs, Msquark, Mgluino,
			ObsExclusion, 3, 2, 2, 1);
	TGraph * gCLsExpExclMHTm1 = plotTools->GetContour(hs, Msquark, Mgluino,
			ExpExclusionM1, 3, 2, 5, 2);
	TGraph * gCLsExpExclMHTp1 = plotTools->GetContour(hs, Msquark, Mgluino,
			ExpExclusionP1, 3, 0, 5, 2);

	TGraph * gCLsObsExclMHT_LO_Smooth=(TGraph *)gCLsObsExclMHT_LO->Clone("1");
	TGraph * gCLsExpExclMHT_LO_Smooth=(TGraph *)gCLsExpExclMHT_LO->Clone("2");
	TGraph * gCLsExpExclMHTm1_Smooth=(TGraph *)gCLsExpExclMHTm1->Clone("3");
	TGraph * gCLsExpExclMHTp1_Smooth=(TGraph *)gCLsExpExclMHTp1->Clone("4");
	//smooth contours (2D Gaussian smoothing)
	Smooth(gCLsObsExclMHT_LO_Smooth, 8);
	Smooth(gCLsExpExclMHT_LO_Smooth, 8);
	//Smooth(gCLsObsExcl_LO_SmoothSigCont, 5);
	//Smooth(gCLsExpExcl_LO_SmoothSigCont, 5);

	TGraph * gCLsExp1Sigma = MakeBand(gCLsExpExclMHTm1_Smooth,gCLsExpExclMHTp1_Smooth);
	//gCLsExp1Sigma->SetFillStyle(3001);
	//gCLsExp1Sigma->SetFillColor(5);
	//gCLsExp1Sigma->SetLineWidth(1004);

	hexcl->GetYaxis()->SetTitleOffset(1.4);
	hexcl->GetXaxis()->SetTitleOffset(0.95);
	hexcl->Draw("colz");

	gCLsExp1Sigma->Draw("f");
//	gCLsObsExclMHT_LO->Draw("l");
//	gCLsExpExclMHT_LO->Draw("l");
//	gCLsExpExclMHTm1->Draw("l");
//	gCLsExpExclMHTp1->Draw("l");


	gCLsExpExclMHT_LO_Smooth->Draw("l");
	gCLsObsExclMHT_LO_Smooth->Draw("l");
	//gCLsExpExclMHTm1_Smooth->Draw("l");
	//gCLsExpExclMHTp1_Smooth->Draw("l");

	TLegend * leg = new TLegend(0.61, 0.66, 0.93, 0.83);
	leg->SetBorderSize(0);
	leg->SetFillColor(0);
	leg->SetFillStyle(4000);
	leg->SetTextFont(42);
	TGraph * expLeg = (TGraph*) gCLsObsExclMHT_LO_Smooth->Clone();
	expLeg->SetFillStyle(gCLsExp1Sigma->GetFillStyle());
	expLeg->SetFillColor(gCLsExp1Sigma->GetFillColor());
	leg->SetHeader("L_{int} = 1.09 fb^{-1}, #sqrt{s} = 7 TeV");
	//leg->AddEntry(CLsObsNLO,"Observed, NLO","l");
	leg->AddEntry(gCLsObsExclMHT_LO_Smooth, "Observed, LO", "l");
	leg->AddEntry(expLeg, "Expected #pm 1#sigma, LO", "lf");
	as.SetTextSize(0.03);
	as.DrawLatex(1370, 1890, "CMS preliminary");

	//leg->AddEntry(CLsExpNoSNLO,   "Expected, no-signal hyp., NLO","l");
	leg->Draw();
	gPad->RedrawAxis();
	c1->SaveAs("results/Exclusion_3jet.pdf");
	c1->SaveAs("results/Exclusion_3jet.eps");
	output->WriteTObject(c1, 0, "overwrite");
}

int main(int argc, char** argv) {
	return plot(argc, argv);
}
