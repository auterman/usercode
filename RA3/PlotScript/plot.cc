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

void DrawPlots(TCanvas * c1,PlotTools < SusyScan > *plotTools,PlotTools < SusyScan > *plotToolsInterpol,string nameappendix){
	c1->SetLogz(1);

		// cross-section in M0 - M1/2
		c1->SetRightMargin(0.2);
		TH2F*hxsec = new TH2F("xsec",
				";m_{squark} [GeV]; m_{gluino} [GeV]; cross section [pb]", 20, 400,
				2000, 20, 400, 2000);
		plotTools->Area(hxsec, Msquark, Mgluino, Xsection);
		hxsec->SetMinimum(0.0001);
		//sq500->Draw();   // m(squark) = 500 GeV line
		//gl500->Draw();   // m(gluino) = 500 GeV line
		hxsec->Draw("colz");
		drawCmsPrel();
		string nameXsPlot="results/"+nameappendix+"Xsection_3jet";
		c1->SaveAs((nameXsPlot+".pdf").c_str());
		c1->SaveAs((nameXsPlot+".eps").c_str());

		// Signal Acceptance in M0 - M1/2
		c1->SetRightMargin(0.2);
		c1->SetTopMargin(0.11);
		c1->SetLogz(0);
		TH2F*hsigacc = new TH2F("sigacc",
				";m_{squark} [GeV]; m_{gluino} [GeV]; Signal Acceptance [%]", 20,
				400, 2000, 20, 400, 2000);
		plotTools->Area(hsigacc, Msquark, Mgluino, SignalAcceptance);
		if(nameappendix=="Wino"){
			hsigacc->SetMinimum(0.0);
					hsigacc->SetMaximum(0.20);
		}else{
			hsigacc->SetMinimum(0.0);
					hsigacc->SetMaximum(0.55);
		}

		hsigacc->SetContour(14);
		hsigacc->GetZaxis()->SetTitleOffset(1.5);
		hsigacc->Draw("colz");
		drawCmsPrel();
		string nameSigAcc="results/"+nameappendix+"SigAcc3jet";
		c1->SaveAs((nameSigAcc+".pdf").c_str());
		c1->SaveAs((nameSigAcc+".eps").c_str());

		// Signal Contamination in M0 - M1/2
		c1->SetRightMargin(0.2);
		c1->SetTopMargin(0.11);
		c1->SetLogz(0);
		TH2F*hsigcont = new TH2F("sigcont",
				";m_{squark} [GeV]; m_{gluino} [GeV]; Signal Contamination [%]",
				20, 400, 2000, 20, 400, 2000);
		plotTools->Area(hsigcont, Msquark, Mgluino, SignalContamination);
		hsigcont->SetMinimum(0.0);
		hsigcont->SetMaximum(1);
		hsigcont->SetContour(14);
		hsigcont->GetZaxis()->SetTitleOffset(1.5);
		hsigcont->Draw("colz");
		drawCmsPrel();
		string nameSigCont="results/"+nameappendix+"SigCont3jet";
		c1->SaveAs((nameSigCont+".pdf").c_str());
		c1->SaveAs((nameSigCont+".eps").c_str());

		// Observed Limit in M0 - M1/2
		c1->SetRightMargin(0.2);
		c1->SetTopMargin(0.11);
		c1->SetLogz(0);
		TH2F*hobslimit = new TH2F("obslimit",
				";m_{squark} [GeV]; m_{gluino} [GeV]; 95% CL Observed Limit [pb]",
				20, 400, 2000, 20, 400, 2000);
		plotTools->Area(hobslimit, Msquark, Mgluino, ObsXsecLimit);
		if(nameappendix=="Wino"){
			hobslimit->SetMinimum(0.05);
				}else{
					hobslimit->SetMinimum(0.03);
				}
		hobslimit->Draw("colz");
		drawCmsPrel();
		string nameObs="results/"+nameappendix+"ObsLimit_3jet";
		c1->SaveAs((nameObs+".pdf").c_str());
		c1->SaveAs((nameObs+".eps").c_str());

		// Expected Limit in M0 - M1/2
		c1->SetRightMargin(0.2);
		c1->SetTopMargin(0.11);
		c1->SetLogz(0);
		TH2F*hexplimit = new TH2F("explimit",
				";m_{squark} [GeV]; m_{gluino} [GeV]; 95% CL Expected Limit [pb]",
				20, 400, 2000, 20, 400, 2000);
		plotTools->Area(hexplimit, Msquark, Mgluino, ExpXsecLimit);
		if(nameappendix=="Wino"){
			hexplimit->SetMinimum(0.05);
						}else{
							hexplimit->SetMinimum(0.03);
						}
		hexplimit->Draw("colz");
		drawCmsPrel();
		string nameExp="results/"+nameappendix+"ExpLimit_3jet";
		c1->SaveAs((nameExp+".pdf").c_str());
		c1->SaveAs((nameExp+".eps").c_str());

		// Observed Limit in M0 - M1/2
		c1->SetRightMargin(0.25);
		c1->SetTopMargin(0.11);
		c1->SetLogz(0);
		TH2F*hobslimitSigCont = new TH2F("obslimitSigCont",
				";m_{squark} [GeV]; m_{gluino} [GeV]; 95% CL Observed Limit [pb]",
				20, 400, 2000, 20, 400, 2000);
		plotTools->Area(hobslimitSigCont, Msquark, Mgluino, ObsXsecLimitSigCont);
		if(nameappendix=="Wino"){
			hobslimitSigCont->SetMinimum(0.05);
							}else{
								hobslimitSigCont->SetMinimum(0.03);
							}
		hobslimitSigCont->GetZaxis()->SetTitleOffset(1.5);
		hobslimitSigCont->Draw("colz");
		drawCmsPrel();
		string nameObsSigCont="results/"+nameappendix+"ObsLimitSigCont_3jet";
		c1->SaveAs((nameObsSigCont+".pdf").c_str());
		c1->SaveAs((nameObsSigCont+".eps").c_str());

		// Expected Limit in M0 - M1/2
		c1->SetRightMargin(0.25);
		c1->SetTopMargin(0.11);
		c1->SetLogz(0);
		TH2F*hexplimitSigCont = new TH2F("explimitSigCont",
				";m_{squark} [GeV]; m_{gluino} [GeV]; 95% CL Expected Limit [pb]",
				20, 400, 2000, 20, 400, 2000);
		plotTools->Area(hexplimitSigCont, Msquark, Mgluino, ExpXsecLimitSigCont);
		if(nameappendix=="Wino"){
			hexplimitSigCont->SetMinimum(0.05);
									}else{
										hexplimitSigCont->SetMinimum(0.03);
									}
		hexplimitSigCont->GetZaxis()->SetTitleOffset(1.5);
		hexplimitSigCont->Draw("colz");
		drawCmsPrel();
		string nameExpSigCont="results/"+nameappendix+"ExpLimitSigCont_3jet";
		c1->SaveAs((nameExpSigCont+".pdf").c_str());
		c1->SaveAs((nameExpSigCont+".eps").c_str());

		// TestContours in M0 - M1/2 /////////////////////////////////////////////////////////////////////
		c1->SetRightMargin(0.2);
		c1->SetTopMargin(0.11);
		c1->SetLogz(0);
		TH2F*TestCont = new TH2F("texcl",
				";m_{squark} [GeV]; m_{gluino} [GeV]; 95% CL Expected Exclusion",
				20, 400, 2000, 20, 400, 2000);
		/// Find the correct contour for any of the exclusion plots--->get right contour number!::::::::::::::::::::::
		plotToolsInterpol->Area(TestCont, Msquark, Mgluino, ExpExclusionM1);
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
		string nameTestCont="results/"+nameappendix+"ExclusionTestContoursExp_3jet.pdf";
		c1->SaveAs(nameTestCont.c_str());
		TFile * output = new TFile("output.root", "Update");
		//  //////////////////////////////////////////////////////////////////////////////////////////////
		//
		// Exclusion in M0 - M1/2 =================================================================================
		// hexcl is drawn to define axis range and labels:
		c1->SetRightMargin(0.08);
		c1->SetTopMargin(0.11);
		c1->SetLogz(0);
		TH2F*hexcl = new TH2F("hexcl",
				";m_{squark} [GeV]; m_{gluino} [GeV]; 95% CL Exclusion", 80, 400,
				2000, 80, 400, 2000);
		// hs defined range in which the contrours are calculated, the binning matters! binning should be equal to signal-scan
		TH2F*hs = new TH2F("hs", "", 80, 400, 2000, 80, 400, 2000);
		// usage: GetContour( <range-hist>, <x-var>, <y-var>, <limit-function>, <contour-levels>, <contour index>, <color>, <style> )

		TGraph * gCLsExpExcl_LO = plotToolsInterpol->GetContour(hs, Msquark,
				Mgluino, ExpExclusion, 3, 1, 2, 4);
		TGraph * gCLsObsExcl_LO = plotToolsInterpol->GetContour(hs, Msquark,
				Mgluino, ObsExclusion, 3, 1, 2, 1);
		TGraph * gCLsExpExclm1 = plotToolsInterpol->GetContour(hs, Msquark,
				Mgluino, ExpExclusionM1, 3, 0, 5, 2);
		TGraph * gCLsExpExclp1 = plotToolsInterpol->GetContour(hs, Msquark,
				Mgluino, ExpExclusionP1, 3, 1, 5, 2);

		TGraph * gCLsExpExcl_LOSigCont = plotToolsInterpol->GetContour(hs, Msquark,
				Mgluino, ExpExclusionSigCont, 3, 1, kRed - 2, 4);
		TGraph * gCLsObsExcl_LOSigCont = plotToolsInterpol->GetContour(hs, Msquark,
				Mgluino, ObsExclusionSigCont, 3, 1, kRed - 2, 1);
		TGraph * gCLsExpExclm1SigCont = plotToolsInterpol->GetContour(hs, Msquark,
				Mgluino, ExpExclusionM1SigCont, 3, 0, kOrange, 2);
		TGraph * gCLsExpExclp1SigCont = plotToolsInterpol->GetContour(hs, Msquark,
				Mgluino, ExpExclusionP1SigCont, 3, 1, kOrange, 2);

		TGraph * gCLsObsExcl_LO_Smooth = (TGraph *) gCLsObsExcl_LO->Clone("1");
		TGraph * gCLsExpExcl_LO_Smooth = (TGraph *) gCLsExpExcl_LO->Clone("2");
		TGraph * gCLsExpExclm1_Smooth = (TGraph *) gCLsExpExclm1->Clone("3");
		TGraph * gCLsExpExclp1_Smooth = (TGraph *) gCLsExpExclp1->Clone("4");

		TGraph * gCLsObsExcl_LO_SmoothSigCont =
				(TGraph *) gCLsObsExcl_LOSigCont->Clone("1");
		TGraph * gCLsExpExcl_LO_SmoothSigCont =
				(TGraph *) gCLsExpExcl_LOSigCont->Clone("2");
		TGraph * gCLsExpExclm1_SmoothSigCont =
				(TGraph *) gCLsExpExclm1SigCont->Clone("3");
		TGraph * gCLsExpExclp1_SmoothSigCont =
				(TGraph *) gCLsExpExclp1SigCont->Clone("4");

		//smooth contours (2D Gaussian smoothing)
		Smooth(gCLsObsExcl_LO_Smooth, 12);
		Smooth(gCLsExpExcl_LO_Smooth, 12);

		Smooth(gCLsExpExclm1_Smooth, 12);
		Smooth(gCLsExpExclp1_Smooth, 12);

		Smooth(gCLsObsExcl_LO_SmoothSigCont, 12);
		Smooth(gCLsExpExcl_LO_SmoothSigCont, 12);

		Smooth(gCLsExpExclm1_SmoothSigCont, 12);
		Smooth(gCLsExpExclp1_SmoothSigCont, 12);

		TGraph * gCLsExp1SigmaSigCont = MakeBand(gCLsExpExclm1_SmoothSigCont,
				gCLsExpExclp1_SmoothSigCont);
		gCLsExp1SigmaSigCont->SetFillStyle(3001);
		gCLsExp1SigmaSigCont->SetFillColor(kOrange);

		TGraph * gCLsExp1Sigma = MakeBand(gCLsExpExclm1_Smooth,
				gCLsExpExclp1_Smooth);
		gCLsExp1Sigma->SetFillStyle(3001);
		gCLsExp1Sigma->SetFillColor(5);
		//gCLsExp1Sigma->SetLineWidth(1004);

		hexcl->GetYaxis()->SetTitleOffset(1.4);
		hexcl->GetXaxis()->SetTitleOffset(0.95);
		hexcl->Draw("colz");

		gCLsExp1Sigma->Draw("f");
		gCLsExp1SigmaSigCont->Draw("f");

		gCLsExpExcl_LO_Smooth->Draw("l");
		gCLsExpExcl_LO_Smooth->SetFillStyle(3001);
		gCLsExpExcl_LO_Smooth->SetFillColor(5);
		gCLsObsExcl_LO_Smooth->Draw("l");

		gCLsExpExcl_LO_SmoothSigCont->Draw("l");
		gCLsExpExcl_LO_SmoothSigCont->SetFillStyle(3001);
		gCLsExpExcl_LO_SmoothSigCont->SetFillColor(kOrange);
		gCLsObsExcl_LO_SmoothSigCont->Draw("l");

		TLegend * leg = new TLegend(0.51, 0.51, 0.93, 0.88);
				leg->SetBorderSize(0);
				leg->SetFillColor(0);
				leg->SetFillStyle(4000);
				leg->SetTextSize(0.025);

		//leg->AddEntry(CLsObsNLO,"Observed, NLO","l");
		leg->AddEntry(gCLsObsExcl_LO_Smooth, "Observed, LO", "l");
		leg->AddEntry(gCLsExpExcl_LO_Smooth, "Expected #pm 1#sigma, LO", "lf");
		leg->AddEntry(gCLsObsExcl_LO_SmoothSigCont, "Observed, LO, - Sig. Cont. ",
				"l");
		leg->AddEntry(gCLsExpExcl_LO_SmoothSigCont,
				"Expected #pm 1#sigma, LO, - Sig. Cont.", "lf");

		//leg->AddEntry(CLsExpNoSNLO,   "Expected, no-signal hyp., NLO","l");
		leg->Draw();
		gPad->RedrawAxis();
		drawCmsPrel();
		string nameExcl="results/"+nameappendix+"Exclusion_3jet";
		c1->SaveAs((nameExcl+".pdf").c_str());
		c1->SaveAs((nameExcl+".eps").c_str());
		output->WriteTObject(c1, 0, "overwrite");
		//create clone of result plot but only signal contamination corrected limit drawn
		hexcl->Draw("colz");


			gCLsExp1SigmaSigCont->Draw("f");
			gCLsObsExcl_LO_SmoothSigCont->Draw("l");
			gCLsExpExcl_LO_SmoothSigCont->Draw("l");
			gCLsExpExcl_LO_SmoothSigCont->SetFillStyle(3001);
			gCLsExpExcl_LO_SmoothSigCont->SetFillColor(kOrange);


			TLegend * leg2 = new TLegend(0.51, 0.51, 0.93, 0.88);
			leg2->SetBorderSize(0);
			leg2->SetFillColor(0);
			leg2->SetFillStyle(4000);
			leg2->SetTextSize(0.025);

			//leg->AddEntry(CLsObsNLO,"Observed, NLO","l");
			leg2->SetHeader("Signal Contamination subtracted");
			leg2->AddEntry(gCLsObsExcl_LO_SmoothSigCont, "Observed, LO",
					"l");
			leg2->AddEntry(gCLsExpExcl_LO_SmoothSigCont,
					"Expected #pm 1#sigma, LO", "lf");
			TH1F * dummy= new TH1F("dummy","dummy",5,0,5);
			dummy->SetLineColor(0);
			leg2->AddEntry(dummy, " ",
							"l");
			leg2->AddEntry(dummy, " ",
							"l");

			//leg->AddEntry(CLsExpNoSNLO,   "Expected, no-signal hyp., NLO","l");
			leg2->Draw();
			gPad->RedrawAxis();
			drawCmsPrel();
			string nameExclSigCont="results/"+nameappendix+"ExclusionOnlySigCont_3jet";
			c1->SaveAs((nameExclSigCont+".pdf").c_str());
			c1->SaveAs((nameExclSigCont+".eps").c_str());
			output->WriteTObject(c1, 0, "overwrite");
}

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
	///////////////BINO LIMITS///////////////////////////////////////////////////
	//Get limits from signal scan ---------------------------------------------------
	TheLimits * genpointsInterpolBino = new TheLimits();
	genpointsInterpolBino->Fill("limits_GMSB/resultfilesbino200.txt");
	TheLimits * genpointsBino = new TheLimits();
	genpointsBino->Fill("limits_GMSB/resultfilesbino200.txt");

	//Make grid in Msquark, Mgluino finer by factor of 4 by linear interpolation
	genpointsInterpolBino->ExpandGrid<SusyScan> (Msquark, Mgluino);
	genpointsInterpolBino->ExpandGrid<SusyScan> (Msquark, Mgluino);
	// New 'pseudo' points are added, therefore the binning of all plots has to be made finer by a factor
	// of 2 in x and y for each "ExpandGrid
	//the plotting ----------------------------------------------------------------------
	//plotting helper functions
	PlotTools < SusyScan > *plotToolsBino = new PlotTools<SusyScan> (
			genpointsBino->GetScan());

	PlotTools < SusyScan > *plotToolsInterpolBino = new PlotTools<SusyScan> (
			genpointsInterpolBino->GetScan());

	//the histograms ---------------------------------------------------------------------
	DrawPlots(c1,plotToolsBino,plotToolsInterpolBino,"Bino");
	///////////////WINO LIMITS///////////////////////////////////////////////////
		//Get limits from signal scan ---------------------------------------------------
		TheLimits * genpointsInterpolWino = new TheLimits();
		genpointsInterpolWino->Fill("limits_GMSB/resultfileswino200.txt");
		TheLimits * genpointsWino = new TheLimits();
		genpointsWino->Fill("limits_GMSB/resultfileswino200.txt");

		//Make grid in Msquark, Mgluino finer by factor of 4 by linear interpolation
		genpointsInterpolWino->ExpandGrid<SusyScan> (Msquark, Mgluino);
		genpointsInterpolWino->ExpandGrid<SusyScan> (Msquark, Mgluino);
		// New 'pseudo' points are added, therefore the binning of all plots has to be made finer by a factor
		// of 2 in x and y for each "ExpandGrid
		//the plotting ----------------------------------------------------------------------
		//plotting helper functions
		PlotTools < SusyScan > *plotToolsWino = new PlotTools<SusyScan> (
				genpointsWino->GetScan());

		PlotTools < SusyScan > *plotToolsInterpolWino = new PlotTools<SusyScan> (
				genpointsInterpolWino->GetScan());

		//the histograms ---------------------------------------------------------------------
		DrawPlots(c1,plotToolsWino,plotToolsInterpolWino,"Wino");


}

int main(int argc, char** argv) {
	return plot(argc, argv);
}
