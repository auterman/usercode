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

struct ExclusionCurves {
	TGraph * obs;
	TGraph * exp;
	TGraph * onesigband;
};

ExclusionCurves DrawPlots(TCanvas * c1, PlotTools<SusyScan> *plotTools,
		PlotTools<SusyScan> *plotToolsInterpol, string nameappendix,
		bool inclEdges = false, double METCut = 200, bool bandAroundExpLimit =
				false) {
	std::cout << "Draw band around expected limit? " << bandAroundExpLimit
			<< std::endl;
	c1->SetLogz(1);

	// cross-section in M0 - M1/2
	c1->SetTopMargin(0.11);
	c1->SetRightMargin(0.2);
	TH2F*hxsec = new TH2F("xsec",
			";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); cross section [pb]", 21, 400,
			2000.1, 21, 400, 2000.1);
	if (nameappendix == "Wino150" || nameappendix == "Wino100150"
			|| nameappendix == "Bino150") {
		hxsec = new TH2F("xsec",
				";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); cross section [pb]", 24,
				160, 2000.1, 24, 160, 2000.1);
	}
	plotTools->Area(hxsec, Msquark, Mgluino, NLOXsection);
	hxsec->SetMinimum(0.00001);
	hxsec->GetZaxis()->SetTitleOffset(1.5);
	//sq500->Draw();   // m(squark) = 500 GeV line
	//gl500->Draw();   // m(gluino) = 500 GeV line
	hxsec->Draw("colz");
	drawCmsPrel(METCut);
	string nameXsPlot = "results/" + nameappendix + "Xsection_3jet";
	c1->SaveAs((nameXsPlot + ".pdf").c_str());
	c1->SaveAs((nameXsPlot + ".eps").c_str());

	// Signal Acceptance in M0 - M1/2
	c1->SetRightMargin(0.2);
	c1->SetTopMargin(0.11);
	c1->SetLogz(0);
	TH2F*hsigacc = new TH2F("sigacc",
			";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); Signal Acceptance [%]", 21,
			400, 2000.1, 21, 400, 2000.1);

	if (nameappendix == "Wino150" || nameappendix == "Wino100150"
			|| nameappendix == "Bino150") {
		hsigacc = new TH2F("sigacc",
				";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); Signal Acceptance [%]",
				24, 160, 2000.1, 24, 160, 2000.1);
	}
	plotTools->Area(hsigacc, Msquark, Mgluino, SignalAcceptance);
	if (nameappendix == "Wino" || nameappendix == "WinoLow" || nameappendix
			== "WinoHigh") {
		hsigacc->SetMinimum(0.0);
		hsigacc->SetMaximum(8);
	} else if (nameappendix == "Wino150" || nameappendix == "Wino100150") {
		hsigacc->SetMinimum(0.0);
		hsigacc->SetMaximum(5);
	} else {
		hsigacc->SetMinimum(0.0);
		hsigacc->SetMaximum(55);
	}

	hsigacc->SetContour(14);
	hsigacc->GetZaxis()->SetTitleOffset(1.5);
	hsigacc->Draw("colz");
	drawCmsPrel(METCut);
	string nameSigAcc = "results/" + nameappendix + "SigAcc3jet";
	c1->SaveAs((nameSigAcc + ".pdf").c_str());
	c1->SaveAs((nameSigAcc + ".eps").c_str());

	// Signal Contamination in M0 - M1/2
	c1->SetRightMargin(0.2);
	c1->SetTopMargin(0.11);
	c1->SetLogz(0);
	TH2F*hsigcont = new TH2F("sigcont",
			";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); Signal Contamination [%]",
			21, 400, 2000.1, 21, 400, 2000.1);
	if (nameappendix == "Wino150" || nameappendix == "Wino100150"
			|| nameappendix == "Bino150") {
		hsigcont
				= new TH2F(
						"sigcont",
						";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); Signal Contamination [%]",
						24, 160, 2000.1, 24, 160, 2000.1);
	}
	plotTools->Area(hsigcont, Msquark, Mgluino, SignalContamination);
	hsigcont->SetMinimum(0.0);
	hsigcont->SetMaximum(40);
	hsigcont->SetContour(14);
	hsigcont->GetZaxis()->SetTitleOffset(1.5);
	hsigcont->Draw("colz");
	drawCmsPrel(METCut);
	string nameSigCont = "results/" + nameappendix + "SigCont3jet";
	c1->SaveAs((nameSigCont + ".pdf").c_str());
	c1->SaveAs((nameSigCont + ".eps").c_str());

	// Trigger efficiency
	c1->SetRightMargin(0.2);
	c1->SetTopMargin(0.11);
	c1->SetLogz(0);
	TH2F*htrigeff = new TH2F("trigeff",
			";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); Trigger efficiency [%]", 21,
			400, 2000.1, 21, 400, 2000.1);
	if (nameappendix == "Wino150" || nameappendix == "Wino100150"
			|| nameappendix == "Bino150") {
		htrigeff = new TH2F("trigeff",
				";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); Trigger efficiency [%]",
				24, 160, 2000.1, 24, 160, 2000.1);
	}
	plotTools->Area(htrigeff, Msquark, Mgluino, TriggerEfficiency);
	htrigeff->SetMinimum(0.0);
	if (nameappendix == "Wino" || nameappendix == "WinoLow" || nameappendix
			== "WinoHigh" || nameappendix == "Wino150" || nameappendix
			== "Wino100150") {
		htrigeff->SetMaximum(30);
	} else
		htrigeff->SetMaximum(100);
	htrigeff->SetContour(14);
	htrigeff->GetZaxis()->SetTitleOffset(1.5);
	htrigeff->Draw("colz");
	drawCmsPrel(METCut);
	string nameTrigAcc = "results/" + nameappendix + "TrigEff3jet";
	c1->SaveAs((nameTrigAcc + ".pdf").c_str());
	c1->SaveAs((nameTrigAcc + ".eps").c_str());

	// Trigger efficiencyN90Hits
	c1->SetRightMargin(0.2);
	c1->SetTopMargin(0.11);
	c1->SetLogz(0);
	TH2F
			*htrigeffN90Hits =
					new TH2F(
							"trigeffN90Hits",
							";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); Trigger efficiency (N90Hits) [%]",
							21, 400, 2000.1, 21, 400, 2000.1);
	if (nameappendix == "Wino150" || nameappendix == "Wino100150"
			|| nameappendix == "Bino150") {
		htrigeffN90Hits
				= new TH2F(
						"trigeffN90Hits",
						";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); Trigger efficiency (N90Hits) [%]",
						24, 160, 2000.1, 24, 160, 2000.1);
	}
	plotTools->Area(htrigeffN90Hits, Msquark, Mgluino, TriggerEfficiencyN90Hits);
	htrigeffN90Hits->SetMinimum(0.0);
	htrigeffN90Hits->SetMaximum(30);
	htrigeffN90Hits->SetContour(14);
	htrigeffN90Hits->GetZaxis()->SetTitleOffset(1.5);
	htrigeffN90Hits->Draw("colz");
	drawCmsPrel(METCut);
	string nameTrigAcc2 = "results/" + nameappendix + "TrigEffN90Hits3jet";
	c1->SaveAs((nameTrigAcc2 + ".pdf").c_str());
	c1->SaveAs((nameTrigAcc2 + ".eps").c_str());

	// Observed Limit in M0 - M1/2
	c1->SetRightMargin(0.2);
	c1->SetTopMargin(0.11);
	c1->SetLogz(0);
	TH2F
			*hobslimit =
					new TH2F(
							"obslimit",
							";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); (Observed) 95% CL Upper Limit, pb",
							20, 400, 2000.1, 20, 400, 2000.1);
	if (nameappendix == "Wino150" || nameappendix == "Wino100150"
			|| nameappendix == "Bino150") {
		hobslimit
				= new TH2F(
						"obslimit",
						";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); (Observed) 95% CL Upper Limit, pb",
						24, 160, 2000.1, 24, 160, 2000.1);
	}
	plotTools->Area(hobslimit, Msquark, Mgluino, ObsXsecLimit);
	if (nameappendix == "Wino") {
		hobslimit->SetMinimum(0.05);
	} else {
		hobslimit->SetMinimum(0.005);
		//hobslimit->SetMaximum(0.2);
	}
	hobslimit->Draw("colz");
	hobslimit->GetZaxis()->SetTitleOffset(1.5);
	drawCmsPrel(METCut);
	string nameObs = "results/" + nameappendix + "ObsLimit_3jet";
	c1->SaveAs((nameObs + ".pdf").c_str());
	c1->SaveAs((nameObs + ".eps").c_str());

	// Expected Limit in M0 - M1/2
	c1->SetRightMargin(0.2);
	c1->SetTopMargin(0.11);

	c1->SetLogz(0);
	TH2F
			*hexplimit =
					new TH2F(
							"explimit",
							";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); (Expected) 95% CL Upper Limit, pb",
							21, 400, 2000.1, 21, 400, 2000.1);
	if (nameappendix == "Wino150" || nameappendix == "Wino100150"
			|| nameappendix == "Bino150") {
		hexplimit
				= new TH2F(
						"explimit",
						";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); (Expected) 95% CL Upper Limit, pb",
						24, 160, 2000.1, 24, 160, 2000.1);
	}
	plotTools->Area(hexplimit, Msquark, Mgluino, ExpXsecLimit);
	if (nameappendix == "Wino") {
		hexplimit->SetMinimum(0.05);
	} else {
		hexplimit->SetMinimum(0.005);
	}
	if (nameappendix == "Wino") {
		//hexplimit->SetMaximum(1);
	} else {
		hexplimit->SetMaximum(0.3);
	}
	hexplimit->Draw("colz");
	hexplimit->GetZaxis()->SetTitleOffset(1.5);
	drawCmsPrel(METCut);
	string nameExp = "results/" + nameappendix + "ExpLimit_3jet";
	c1->SaveAs((nameExp + ".pdf").c_str());
	c1->SaveAs((nameExp + ".eps").c_str());

	// Observed Limit in M0 - M1/2
	c1->SetRightMargin(0.2);
	c1->SetTopMargin(0.11);
	c1->SetLogz(0);
	TH2F
			*hobslimitSigCont =
					new TH2F(
							"obslimitSigCont",
							";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2});(Observed) 95% CL Upper Limit, pb",
							21, 400, 2000.001, 21, 400, 2000.001);
	if (nameappendix == "Wino150" || nameappendix == "Wino100150"
			|| nameappendix == "Bino150") {
		hobslimitSigCont
				= new TH2F(
						"obslimitSigCont",
						";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2});(Observed) 95% CL Upper Limit, pb",
						24, 160, 2000.1, 24, 160, 2000.1);
	}
	plotTools->Area(hobslimitSigCont, Msquark, Mgluino, ObsXsecLimitSigCont);
	if (nameappendix == "Wino") {
		hobslimitSigCont->SetMinimum(0.05);
	} else {
		hobslimitSigCont->SetMinimum(0.005);
		hobslimitSigCont->SetMaximum(0.45);
	}
	hobslimitSigCont->GetZaxis()->SetTitleOffset(1.5);
	hobslimitSigCont->GetXaxis()->SetNdivisions(505);
	hobslimitSigCont->Draw("colz");
	drawCmsPrel(METCut);
	string nameObsSigCont = "results/" + nameappendix + "ObsLimitSigCont_3jet";
	c1->SaveAs((nameObsSigCont + ".pdf").c_str());
	c1->SaveAs((nameObsSigCont + ".eps").c_str());

	// Expected Limit in M0 - M1/2
	c1->SetRightMargin(0.2);
	c1->SetTopMargin(0.11);
	c1->SetLogz(0);
	TH2F
			*hexplimitSigCont =
					new TH2F(
							"explimitSigCont",
							";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); (Expected) 95% CL Upper Limit, pb",
							21, 400, 2000.1, 21, 400, 2000.1);
	if (nameappendix == "Wino150" || nameappendix == "Wino100150"
			|| nameappendix == "Bino150") {
		hexplimitSigCont
				= new TH2F(
						"explimitSigCont",
						";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); (Expected) 95% CL Upper Limit, pb",
						24, 160, 2000.1, 24, 160, 2000.1);
	}
	plotTools->Area(hexplimitSigCont, Msquark, Mgluino, ExpXsecLimitSigCont);
	if (nameappendix == "Wino" || nameappendix == "WinoLow" || nameappendix
			== "WinoHigh") {
		hexplimitSigCont->SetMinimum(0.05);

	}
	if (nameappendix == "Wino150" || nameappendix == "Wino100150") {
		hexplimitSigCont->SetMinimum(0.05);
		hexplimitSigCont->SetMaximum(3);
	} else {
		hexplimitSigCont->SetMinimum(0.005);
	}
	hexplimitSigCont->GetZaxis()->SetTitleOffset(1.5);
	hexplimitSigCont->Draw("colz");
	drawCmsPrel(METCut);
	string nameExpSigCont = "results/" + nameappendix + "ExpLimitSigCont_3jet";
	c1->SaveAs((nameExpSigCont + ".pdf").c_str());
	c1->SaveAs((nameExpSigCont + ".eps").c_str());

	// TestContours in M0 - M1/2 /////////////////////////////////////////////////////////////////////
	c1->SetRightMargin(0.2);
	c1->SetTopMargin(0.11);
	c1->SetLogz(0);
	TH2F*TestCont = new TH2F("texcl",
			";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); 95% CL Expected Exclusion",
			82, 380, 2000.1, 82, 380, 2000.1);
	//21, 400, 2000.1, 21, 400, 2000.1);
	if (nameappendix == "Wino150" || nameappendix == "Wino100150"
			|| nameappendix == "Bino150") {
		TestCont
				= new TH2F(
						"texcl",
						";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); 95% CL Expected Exclusion",
						//24, 160, 2000.1, 24, 160, 2000.1);
						93, 160, 2000.1, 93, 160, 2000.1);
	}

	/// Find the correct contour for any of the exclusion plots--->get right contour number!::::::::::::::::::::::
	plotToolsInterpol->Area(TestCont, Msquark, Mgluino, ObsExclusionM1SigCont);
	/// try "3" contour levels:
	std::vector<TGraph*> contours = plotToolsInterpol->GetContours(TestCont, 3);
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
	string nameTestCont = "results/" + nameappendix
			+ "ExclusionTestContoursExp_3jet.pdf";
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
			";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); 95% CL Exclusion", 81, 400,
			2000.1, 81, 400, 2000.1);
	if (nameappendix == "Wino150" || nameappendix == "Wino100150"
			|| nameappendix == "Bino150") {
		hexcl = new TH2F("hexcl",
				";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); 95% CL Exclusion", 93,
				160, 2000.1, 93, 160, 2000.1);
		//24,160, 2000, 24, 160, 2000);
	}

	// hs defined range in which the contrours are calculated, the binning matters! binning should be equal to signal-scan
	TH2F * hs = new TH2F("hs", "", 81, 400, 2000.1, 81, 400, 2000.1);
	if (inclEdges) {
		hs = new TH2F("hs", "", 82, 380, 2000.1, 82, 380, 2000.1);
	}
	if (nameappendix == "Wino150" || nameappendix == "Wino100150"
			|| nameappendix == "Bino150") {
		hs = new TH2F("hs", "", 93, 160, 2000.1, 92, 160, 2000.1);
		//;24,160, 2000.1, 24, 160, 2000.1);
	}

	// usage: GetContour( <range-hist>, <x-var>, <y-var>, <limit-function>, <contour-levels>, <contour index>, <color>, <style> )

	TGraph * gCLsExpExcl_LO = plotToolsInterpol->GetContour(hs, Msquark,
			Mgluino, ExpExclusion, 3, 0, 2, 4);
	TGraph * gCLsObsExcl_LO = plotToolsInterpol->GetContour(hs, Msquark,
			Mgluino, ObsExclusion, 3, 0, 2, 1);
	TGraph * gCLsExpExclm1 = plotToolsInterpol->GetContour(hs, Msquark,
			Mgluino, ExpExclusionM1, 3, 0, 5, 2);
	TGraph * gCLsExpExclp1 = plotToolsInterpol->GetContour(hs, Msquark,
			Mgluino, ExpExclusionP1, 3, 0, 5, 2);
	TGraph * gCLsExpExclm2 = plotToolsInterpol->GetContour(hs, Msquark,
			Mgluino, ExpExclusionM2, 3, 0, 5, 2);
	TGraph * gCLsExpExclp2 = plotToolsInterpol->GetContour(hs, Msquark,
			Mgluino, ExpExclusionP2, 3, 0, 5, 2);
	TGraph * gCLsObsExclm1 = plotToolsInterpol->GetContour(hs, Msquark,
			Mgluino, ObsExclusionM1, 3, 0, 5, 2);
	if (nameappendix == "Bino150" ) {
		gCLsObsExclm1 = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
				ObsExclusionM1, 3, 3, 5, 2);
	}
	TGraph * gCLsObsExclp1 = plotToolsInterpol->GetContour(hs, Msquark,
			Mgluino, ObsExclusionP1, 3, 0, 5, 2);
	TGraph * gCLsObsExclm2 = plotToolsInterpol->GetContour(hs, Msquark,
			Mgluino, ObsExclusionM2, 3, 0, 5, 2);
	TGraph * gCLsObsExclp2 = plotToolsInterpol->GetContour(hs, Msquark,
			Mgluino, ObsExclusionP2, 3, 0, 5, 2);

	TGraph * gCLsExpExcl_LOSigCont = plotToolsInterpol->GetContour(hs, Msquark,
			Mgluino, ExpExclusionSigCont, 3, 0, kRed - 2, 4);
	TGraph * gCLsObsExcl_LOSigCont = plotToolsInterpol->GetContour(hs, Msquark,
			Mgluino, ObsExclusionSigCont, 3, 0, kRed - 2, 1);
	TGraph * gCLsExpExclm1SigCont = plotToolsInterpol->GetContour(hs, Msquark,
			Mgluino, ExpExclusionM1SigCont, 3, 0, kOrange, 2);
	TGraph * gCLsExpExclp1SigCont = plotToolsInterpol->GetContour(hs, Msquark,
			Mgluino, ExpExclusionP1SigCont, 3, 0, kOrange, 2);
	TGraph * gCLsExpExclm2SigCont = plotToolsInterpol->GetContour(hs, Msquark,
			Mgluino, ExpExclusionM2SigCont, 3, 0, kOrange, 2);
	TGraph * gCLsExpExclp2SigCont = plotToolsInterpol->GetContour(hs, Msquark,
			Mgluino, ExpExclusionP2SigCont, 3, 0, kOrange, 2);
	TGraph * gCLsObsExclm1SigCont = plotToolsInterpol->GetContour(hs, Msquark,
			Mgluino, ObsExclusionM1SigCont, 3, 0, kOrange, 2);
	if (nameappendix == "Bino150" ) {
		gCLsObsExclm1SigCont = plotToolsInterpol->GetContour(hs, Msquark,
				Mgluino, ObsExclusionM1SigCont, 3, 3, kOrange, 2);
	}
	TGraph * gCLsObsExclp1SigCont = plotToolsInterpol->GetContour(hs, Msquark,
			Mgluino, ObsExclusionP1SigCont, 3, 0, kOrange, 2);
	TGraph * gCLsObsExclm2SigCont = plotToolsInterpol->GetContour(hs, Msquark,
			Mgluino, ObsExclusionM2SigCont, 3, 0, kOrange, 2);
	TGraph * gCLsObsExclp2SigCont = plotToolsInterpol->GetContour(hs, Msquark,
			Mgluino, ObsExclusionP2SigCont, 3, 0, kOrange, 2);

	TGraph * gCLsObsExcl_LO_Smooth = (TGraph *) gCLsObsExcl_LO->Clone("1");
	TGraph * gCLsExpExcl_LO_Smooth = (TGraph *) gCLsExpExcl_LO->Clone("2");
	TGraph * gCLsExpExclm1_Smooth = (TGraph *) gCLsExpExclm1->Clone("3");
	TGraph * gCLsExpExclp1_Smooth = (TGraph *) gCLsExpExclp1->Clone("4");
	TGraph * gCLsExpExclm2_Smooth = (TGraph *) gCLsExpExclm1->Clone("5");
	TGraph * gCLsExpExclp2_Smooth = (TGraph *) gCLsExpExclp1->Clone("6");
	TGraph * gCLsObsExclm1_Smooth = (TGraph *) gCLsObsExclm1->Clone("7");
	TGraph * gCLsObsExclp1_Smooth = (TGraph *) gCLsObsExclp1->Clone("8");
	TGraph * gCLsObsExclm2_Smooth = (TGraph *) gCLsObsExclm1->Clone("9");
	TGraph * gCLsObsExclp2_Smooth = (TGraph *) gCLsObsExclp1->Clone("10");

	TGraph * gCLsObsExcl_LO_SmoothSigCont =
			(TGraph *) gCLsObsExcl_LOSigCont->Clone("1");
	TGraph * gCLsExpExcl_LO_SmoothSigCont =
			(TGraph *) gCLsExpExcl_LOSigCont->Clone("2");
	TGraph * gCLsExpExclm1_SmoothSigCont =
			(TGraph *) gCLsExpExclm1SigCont->Clone("3");
	TGraph * gCLsExpExclp1_SmoothSigCont =
			(TGraph *) gCLsExpExclp1SigCont->Clone("4");
	TGraph * gCLsExpExclm2_SmoothSigCont =
			(TGraph *) gCLsExpExclm2SigCont->Clone("5");
	TGraph * gCLsExpExclp2_SmoothSigCont =
			(TGraph *) gCLsExpExclp2SigCont->Clone("6");
	TGraph * gCLsObsExclm1_SmoothSigCont =
			(TGraph *) gCLsObsExclm1SigCont->Clone("7");
	TGraph * gCLsObsExclp1_SmoothSigCont =
			(TGraph *) gCLsObsExclp1SigCont->Clone("8");
	TGraph * gCLsObsExclm2_SmoothSigCont =
			(TGraph *) gCLsObsExclm2SigCont->Clone("9");
	TGraph * gCLsObsExclp2_SmoothSigCont =
			(TGraph *) gCLsObsExclp2SigCont->Clone("10");

	//smooth contours (2D Gaussian smoothing)
	Smooth(gCLsObsExcl_LO_Smooth, 12);
	Smooth(gCLsExpExcl_LO_Smooth, 12);

	Smooth(gCLsExpExclm1_Smooth, 12);
	Smooth(gCLsExpExclp1_Smooth, 12);
	Smooth(gCLsExpExclm2_Smooth, 12);
	Smooth(gCLsExpExclp2_Smooth, 12);
	Smooth(gCLsObsExclm1_Smooth, 12);
	Smooth(gCLsObsExclp1_Smooth, 12);
	Smooth(gCLsObsExclm2_Smooth, 12);
	Smooth(gCLsObsExclp2_Smooth, 12);

	Smooth(gCLsObsExcl_LO_SmoothSigCont, 12);
	Smooth(gCLsExpExcl_LO_SmoothSigCont, 12);

	Smooth(gCLsExpExclm1_SmoothSigCont, 12);
	Smooth(gCLsExpExclp1_SmoothSigCont, 12);
	Smooth(gCLsExpExclm2_SmoothSigCont, 12);
	Smooth(gCLsExpExclp2_SmoothSigCont, 12);
	Smooth(gCLsObsExclm1_SmoothSigCont, 12);
	Smooth(gCLsObsExclp1_SmoothSigCont, 12);
	Smooth(gCLsObsExclm2_SmoothSigCont, 12);
	Smooth(gCLsObsExclp2_SmoothSigCont, 12);

	TGraph * gCLs1SigmaSigCont = MakeBand(gCLsObsExclm1_SmoothSigCont,
			gCLsObsExclp1_SmoothSigCont);
	if (bandAroundExpLimit) {
		gCLs1SigmaSigCont = MakeBand(gCLsExpExclm1_SmoothSigCont,
				gCLsExpExclp1_SmoothSigCont);
	}
	//gCLs1SigmaSigCont->SetFillStyle(kSolid);
	gCLs1SigmaSigCont->SetFillColor(42);
	gCLs1SigmaSigCont->SetLineColor(42);

	TGraph * gCLs1Sigma = MakeBand(gCLsObsExclm1_Smooth, gCLsObsExclp1_Smooth);
	if (bandAroundExpLimit) {
		gCLs1Sigma = MakeBand(gCLsExpExclm1_Smooth, gCLsExpExclp1_Smooth);
	}
	gCLs1Sigma->SetFillStyle(3001);
	gCLs1Sigma->SetFillColor(5);

	TGraph * gCLs2SigmaSigCont = MakeBand(gCLsObsExclm2_SmoothSigCont,
			gCLsObsExclp2_SmoothSigCont);
	if (bandAroundExpLimit) {
		gCLs2SigmaSigCont = MakeBand(gCLsExpExclm2_SmoothSigCont,
				gCLsExpExclp2_SmoothSigCont);
	}
	gCLs2SigmaSigCont->SetFillStyle(3001);
	gCLs2SigmaSigCont->SetFillColor(5);

	TGraph * gCLs2Sigma = MakeBand(gCLsObsExclm2_Smooth, gCLsObsExclp2_Smooth);
	if (bandAroundExpLimit) {
		gCLs2Sigma = MakeBand(gCLsExpExclm2_Smooth, gCLsExpExclp2_Smooth);
	}
	gCLs2Sigma->SetFillStyle(3001);
	gCLs2Sigma->SetFillColor(kYellow - 9);

	hexcl->GetYaxis()->SetTitleOffset(1.55);
	hexcl->GetXaxis()->SetTitleOffset(1.1);
	hexcl->Draw("colz");

	gCLs1Sigma->Draw("f");
	gCLs1SigmaSigCont->Draw("f");

	//	gCLs2Sigma->Draw("f");
	//	gCLs2SigmaSigCont->Draw("f");

	gCLsExpExcl_LO_Smooth->Draw("l");
	gCLsObsExcl_LO_Smooth->Draw("l");

	gCLsExpExcl_LO_SmoothSigCont->Draw("l");
	gCLsObsExcl_LO_SmoothSigCont->Draw("l");

	TLegend * leg = new TLegend(0.51, 0.51, 0.93, 0.88);
	leg->SetBorderSize(0);
	leg->SetFillColor(0);
	leg->SetFillStyle(4000);
	leg->SetTextSize(0.025);

	leg->SetHeader("m_{#tilde{#Chi}^{0}} = 375 GeV");
	if (nameappendix == "Wino150" || nameappendix == "Wino100150"
			|| nameappendix == "Bino150") {
		leg->SetHeader("m_{#tilde{#Chi}^{0}} = 150 GeV");
	}
	leg->AddEntry(gCLsObsExcl_LO_Smooth, "Observed, NLO", "l");

	if (bandAroundExpLimit) {
		leg->AddEntry(gCLs1Sigma, "Expected #pm 1#sigma, NLO", "lf");
	} else {
		leg->AddEntry(gCLs1Sigma, "Observed #pm 1#sigma, NLO", "lf");
	}
	leg->AddEntry(gCLsExpExcl_LO_Smooth, "Expected, NLO", "l");
	///////////////////////////////////////
	leg->AddEntry(gCLsObsExcl_LO_SmoothSigCont, "Observed, NLO, - Sig. Cont. ",
			"l");
	if (bandAroundExpLimit) {
		leg->AddEntry(gCLs1SigmaSigCont,
				"Expected #pm 1#sigma, NLO, - Sig. Cont.", "lf");
	} else {
		leg->AddEntry(gCLs1SigmaSigCont,
				"Observed #pm 1#sigma, NLO, - Sig. Cont.", "lf");
	}
	leg->AddEntry(gCLsExpExcl_LO_SmoothSigCont, "Expected, NLO, - Sig. Cont. ",
			"l");

	//leg->AddEntry(CLsExpNoSNLO,   "Expected, no-signal hyp., NLO","l");
	leg->Draw();
	gPad->RedrawAxis();
	drawCmsPrel(METCut);
	string nameExcl = "results/" + nameappendix + "Exclusion_3jet";
	c1->SaveAs((nameExcl + ".pdf").c_str());
	c1->SaveAs((nameExcl + ".eps").c_str());
	output->WriteTObject(c1, 0, "overwrite");
	//create clone of result plot but only signal contamination corrected limit drawn
	hexcl->Draw("colz");
	double min1=hs->GetXaxis()->GetBinLowEdge(0);
	double min2=hs->GetYaxis()->GetBinLowEdge(0);
	TGraph * excludedRegion=GetExcludedRegion(gCLsObsExclm1_SmoothSigCont,min1,min2);
	excludedRegion->Draw("f");
	float yv = 0.25;
	if (nameappendix == "Wino150" || nameappendix == "Wino100150"
				|| nameappendix == "Wino")yv = 0.22;
	  TLatex* lat2 = new TLatex(0.25,yv,"Excluded");
	  lat2->SetNDC(true);
	  lat2->SetTextSize(0.04);
	  lat2->Draw("same");


	gCLs1SigmaSigCont->Draw("f");
	gCLsObsExcl_LO_SmoothSigCont->SetLineWidth(3);
	gCLsObsExcl_LO_SmoothSigCont->SetLineStyle(1);
	gCLsObsExcl_LO_SmoothSigCont->SetLineColor(1);
	gCLsExpExcl_LO_SmoothSigCont->SetLineWidth(3);
	gCLsExpExcl_LO_SmoothSigCont->SetLineStyle(2);
	gCLsExpExcl_LO_SmoothSigCont->SetLineColor(1);
	gCLsObsExcl_LO_SmoothSigCont->Draw("l");
	gCLsExpExcl_LO_SmoothSigCont->Draw("l");

	//gCLsObsExclp1_SmoothSigCont->Draw("l");
	//gCLsObsExclm1_SmoothSigCont->Draw("l");

	TLatex* lat = new TLatex(0.37,0.82,"CMS Preliminary 2011, 1.14 fb^{-1}");
	  lat->SetNDC(true);
	  lat->SetTextSize(0.04);
	  lat->Draw("same");


	 TLegend* leg2 = new TLegend(0.55,0.55,0.8,0.8,"m_{#tilde{#Chi}^{0}} = 375 (GeV/c^{2})");
	//TLegend * leg2 = new TLegend(0.51, 0.51, 0.93, 0.88);
	leg2->SetBorderSize(0);
	leg2->SetFillColor(0);
	leg2->SetFillStyle(4000);
	leg2->SetTextSize(0.03);
	//leg2->SetHeader("m_{#tilde{#Chi}^{0}} = 375 GeV");
	if (nameappendix == "Wino150" || nameappendix == "Wino100150"
			|| nameappendix == "Bino150") {
		leg2->SetHeader("m_{#tilde{#Chi}^{0}} = 150 GeV");
	}
	//leg->AddEntry(CLsObsNLO,"Observed, NLO","l");
	//leg2->SetHeader("Signal Contamination subtracted");
	leg2->AddEntry(gCLsObsExcl_LO_SmoothSigCont, "Observed, NLO", "l");
	leg2->AddEntry(gCLsExpExcl_LO_SmoothSigCont, "Expected, NLO", "l");
	if (bandAroundExpLimit) {
		leg2->AddEntry(gCLs1SigmaSigCont, "#pm 1#sigma, NLO", "f");
	} else {
		leg2->AddEntry(gCLs1SigmaSigCont, "#pm 1#sigma, NLO", "f");
	}

	//	leg2->AddEntry(gCLsExp2SigmaSigCont, "Expected #pm 2#sigma, NLO",
	//				"f");
	TH1F * dummy = new TH1F("dummy", "dummy", 5, 0, 5);
	dummy->SetLineColor(0);
	//leg2->AddEntry(dummy, " ", "l");
	//leg2->AddEntry(dummy, " ", "l");

	//leg->AddEntry(CLsExpNoSNLO,   "Expected, no-signal hyp., NLO","l");
	leg2->Draw();
	gPad->RedrawAxis();
	drawCmsPrel(METCut,true);
	string nameExclSigCont = "results/" + nameappendix
			+ "ExclusionOnlySigCont_3jet";
	c1->SaveAs((nameExclSigCont + ".pdf").c_str());
	c1->SaveAs((nameExclSigCont + ".eps").c_str());
	output->WriteTObject(c1, 0, "overwrite");

	ExclusionCurves exclCurv;
	exclCurv.exp = gCLsExpExcl_LO_SmoothSigCont;
	exclCurv.obs = gCLsObsExcl_LO_SmoothSigCont;
	exclCurv.onesigband = gCLs1SigmaSigCont;
	return exclCurv;
	//	return 0;

}

int plot(int argc, char** argv) {
	//interactive root session
	//TApplication theApp("App", 0, 0);
//	if (gROOT->IsBatch()) {
//		fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
//		return 1;
//	}

	util::StyleSettings::paperNoTitle();
	gStyle->SetPadBottomMargin(0.18);

	TCanvas * c1 = new TCanvas("c1", "c1", 600, 600);
	c1->cd();

	bool pdfuncertinclInAcc = false;

	//	///////////////BINO LIMITS///////////////////////////////////////////////////
	//Get limits from signal scan ---------------------------------------------------
	TheLimits * genpointsInterpolBino = new TheLimits();
	if (pdfuncertinclInAcc) {
		genpointsInterpolBino->Fill("limits_GMSB/resultfilesbinoPDFACC.txt");
	} else {
		genpointsInterpolBino->Fill("limits_GMSB/resultfilesbino200Aug18.txt");
	}
	TheLimits * genpointsBino = new TheLimits();
	if (pdfuncertinclInAcc) {
		genpointsBino->Fill("limits_GMSB/resultfilesbinoPDFACC.txt");
	} else {
		genpointsBino->Fill("limits_GMSB/resultfilesbino200Aug18.txt");
	}

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
	ExclusionCurves binoExcl;
	if (pdfuncertinclInAcc) {
		binoExcl = DrawPlots(c1, plotToolsBino, plotToolsInterpolBino, "Bino",
				true, 200, true);
	} else {
		binoExcl = DrawPlots(c1, plotToolsBino, plotToolsInterpolBino, "Bino",
				true, 200);
	}

	TGraph * bino = binoExcl.exp;
	///////////////BINO LIMITS, NEUTR 150///////////////////////////////////////////////////
	//Get limits from signal scan ---------------------------------------------------
	TheLimits * genpointsInterpolBino150 = new TheLimits();
	genpointsInterpolBino150->Fill("limits_GMSB/resultfilesBino200Neutr150.txt");
	TheLimits * genpointsBino150 = new TheLimits();
	genpointsBino150->Fill("limits_GMSB/resultfilesBino200Neutr150.txt");

	//Make grid in Msquark, Mgluino finer by factor of 4 by linear interpolation
	genpointsInterpolBino150->ExpandGrid<SusyScan> (Msquark, Mgluino);
	genpointsInterpolBino150->ExpandGrid<SusyScan> (Msquark, Mgluino);
	// New 'pseudo' points are added, therefore the binning of all plots has to be made finer by a factor
	// of 2 in x and y for each "ExpandGrid
	//the plotting ----------------------------------------------------------------------
	//plotting helper functions
	PlotTools < SusyScan > *plotToolsBino150 = new PlotTools<SusyScan> (
			genpointsBino150->GetScan());

	PlotTools < SusyScan > *plotToolsInterpolBino150
			= new PlotTools<SusyScan> (genpointsInterpolBino150->GetScan());

	//the histograms ---------------------------------------------------------------------
	ExclusionCurves bino150Excl = DrawPlots(c1, plotToolsBino150,
			plotToolsInterpolBino150, "Bino150", false, 200);

	//	///////////////BINO LIMITS LOW///////////////////////////////////////////////////
	//	//Get limits from signal scan ---------------------------------------------------
	//	TheLimits * genpointsInterpolBinoLow = new TheLimits();
	//	genpointsInterpolBinoLow->Fill("limits_GMSB/resultfilesbino100.txt");
	//	TheLimits * genpointsBinoLow = new TheLimits();
	//	genpointsBinoLow->Fill("limits_GMSB/resultfilesbino100.txt");
	//
	//	//Make grid in Msquark, Mgluino finer by factor of 4 by linear interpolation
	//	genpointsInterpolBinoLow->ExpandGrid<SusyScan> (Msquark, Mgluino);
	//	genpointsInterpolBinoLow->ExpandGrid<SusyScan> (Msquark, Mgluino);
	//	// New 'pseudo' points are added, therefore the binning of all plots has to be made finer by a factor
	//	// of 2 in x and y for each "ExpandGrid
	//	//the plotting ----------------------------------------------------------------------
	//	//plotting helper functions
	//	PlotTools < SusyScan > *plotToolsBinoLow = new PlotTools<SusyScan> (
	//			genpointsBinoLow->GetScan());
	//
	//	PlotTools < SusyScan > *plotToolsInterpolBinoLow
	//			= new PlotTools<SusyScan> (genpointsInterpolBinoLow->GetScan());
	//
	//	//the histograms ---------------------------------------------------------------------
	//	ExclusionCurves binoLowExcl = DrawPlots(c1, plotToolsBinoLow,
	//			plotToolsInterpolBinoLow, "BinoLow", true,100);
	//	TGraph * binoLow = binoLowExcl.exp;
	//
	//	///////////////BINO LIMITS HIGK///////////////////////////////////////////////////
	//	//Get limits from signal scan ---------------------------------------------------
	//	TheLimits * genpointsInterpolBinoHigh = new TheLimits();
	//	genpointsInterpolBinoHigh->Fill("limits_GMSB/resultfilesbino350.txt");
	//	TheLimits * genpointsBinoHigh = new TheLimits();
	//	genpointsBinoHigh->Fill("limits_GMSB/resultfilesbino350.txt");
	//
	//	//Make grid in Msquark, Mgluino finer by factor of 4 by linear interpolation
	//	genpointsInterpolBinoHigh->ExpandGrid<SusyScan> (Msquark, Mgluino);
	//	genpointsInterpolBinoHigh->ExpandGrid<SusyScan> (Msquark, Mgluino);
	//	// New 'pseudo' points are added, therefore the binning of all plots has to be made finer by a factor
	//	// of 2 in x and y for each "ExpandGrid
	//	//the plotting ----------------------------------------------------------------------
	//	//plotting helper functions
	//	PlotTools < SusyScan > *plotToolsBinoHigh = new PlotTools<SusyScan> (
	//			genpointsBinoHigh->GetScan());
	//
	//	PlotTools < SusyScan > *plotToolsInterpolBinoHigh
	//			= new PlotTools<SusyScan> (genpointsInterpolBinoHigh->GetScan());
	//
	//	//the histograms ---------------------------------------------------------------------
	//	ExclusionCurves binoHighExcl = DrawPlots(c1, plotToolsBinoHigh,
	//			plotToolsInterpolBinoHigh, "BinoHigh", true,350);
	//	TGraph * binoHigh = binoHighExcl.exp;
	//
	//	//compare MET cuts
	//	c1->SetLogz(1);
	//	c1->SetRightMargin(0.08);
	//	TH2F*hexclBinoComp = new TH2F("hexcl",
	//			";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); 95% CL Exclusion", 80, 400,
	//			2000, 80, 400, 2000);
	//	hexclBinoComp->Draw("COLZ");
	//	bino->SetLineColor(2);
	//	binoHigh->SetLineColor(2);
	//	binoLow->SetLineColor(2);
	//	bino->SetLineStyle(1);
	//	binoHigh->SetLineStyle(2);
	//	binoLow->SetLineStyle(3);
	//	bino->Draw("");
	//	binoHigh->Draw("");
	//	binoLow->Draw("");
	//	TLegend * legB = new TLegend(0.41, 0.41, 0.93, 0.9);
	//	legB->SetBorderSize(0);
	//	legB->SetFillColor(0);
	//	legB->SetFillStyle(4000);
	//	legB->SetTextSize(0.025);
	//
	//	//leg->AddEntry(CLsObsNLO,"Observed, NLO","l");
	//
	//	legB->AddEntry(binoLow, "Expected MET>100", "l");
	//	legB->AddEntry(bino, "Expected, MET>200", "l");
	//	legB->AddEntry(binoHigh, "Expected, MET>350", "l");
	//
	//	legB->Draw();
	//	drawCmsPrel(0);
	//	string nameXsPlotB = "results/BinoCOMPMetSelection_3jet";
	//	c1->SaveAs((nameXsPlotB + ".pdf").c_str());
	//	c1->SaveAs((nameXsPlotB + ".eps").c_str());
	//
	//compare Neutralino masses
	TGraph * binoExp = binoExcl.exp;
	TGraph * binoObs = binoExcl.obs;
	TGraph * binoOneSigBand = binoExcl.onesigband;

	TGraph * bino150Exp = bino150Excl.exp;
	TGraph * bino150Obs = bino150Excl.obs;
	TGraph * bino150OneSigBand = bino150Excl.onesigband;

	c1->SetLogz(1);
	c1->SetRightMargin(0.08);
	TH2F*hexclbinoCompNeutr = new TH2F("hexclNeutrComp",
			";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); 95% CL Exclusion", 80, 400,
			2000, 80, 400, 2000);
	hexclbinoCompNeutr->Draw("COLZ");

	binoOneSigBand->SetFillStyle(3001);
	binoOneSigBand->SetFillColor(kOrange);
	binoOneSigBand->Draw("f");
	binoObs->SetLineWidth(2);
	binoExp->SetLineWidth(2);
	binoObs->Draw("l");
	binoExp->Draw("l");

	bino150OneSigBand->SetFillStyle(3001);
	bino150OneSigBand->SetFillColor(kYellow);
	bino150OneSigBand->Draw("f");
	bino150Obs->SetLineWidth(2);
	bino150Exp->SetLineWidth(2);
	bino150Obs->Draw("l");
	bino150Exp->Draw("l");

	TH1F * dummybino = new TH1F("dummy", "dummy", 5, 0, 5);
	dummybino->SetLineColor(0);

	TLegend * leg2bino = new TLegend(0.41, 0.41, 0.93, 0.88);
	leg2bino->SetBorderSize(0);
	leg2bino->SetFillColor(0);
	leg2bino->SetFillStyle(4000);
	leg2bino->SetTextSize(0.025);
	//leg2bino->AddEntry(dummybino, "m_{#tilde{#Chi}^{0}} = 375 GeV", "l");
	leg2bino->AddEntry(binoObs, "Observed m_{#tilde{#Chi}^{0}} = 375 GeV", "l");
	leg2bino->AddEntry(binoExp, "Expected m_{#tilde{#Chi}^{0}} = 375 GeV", "l");
	leg2bino->AddEntry(binoOneSigBand,
			"#pm 1#sigma  m_{#tilde{#Chi}^{0}} = 375 GeV", "f");

	//leg2bino->AddEntry(dummybino, "m_{#tilde{#Chi}^{0}} = 150 GeV ", "l");
	leg2bino->AddEntry(bino150Obs, "Observed, m_{#tilde{#Chi}^{0}} = 150 GeV ",
			"l");
	leg2bino->AddEntry(bino150Exp, "Expected, m_{#tilde{#Chi}^{0}} = 150 GeV ",
				"l");
	leg2bino->AddEntry(bino150OneSigBand,
			"#pm 1#sigma, m_{#tilde{#Chi}^{0}} = 150 GeV ", "f");


	//leg->AddEntry(CLsExpNoSNLO,   "Expected, no-signal hyp., NLO","l");
	leg2bino->Draw();
	gPad->RedrawAxis();

	drawCmsPrel(200);
	string nameNeutrCompbinoPlot = "results/BinoCOMPNeutr_3jet";
	c1->SaveAs((nameNeutrCompbinoPlot + ".pdf").c_str());
	c1->SaveAs((nameNeutrCompbinoPlot + ".eps").c_str());
	//	///////////////////////////////////////////////////////////////////////////
	//
	///////////////WINO LIMITS///////////////////////////////////////////////////
	//Get limits from signal scan ---------------------------------------------------
	TheLimits * genpointsInterpolWino = new TheLimits();

	if (pdfuncertinclInAcc) {
		genpointsInterpolWino->Fill("limits_GMSB/resultfileswinoPDFACC.txt");
	} else
		genpointsInterpolWino->Fill("limits_GMSB/resultfileswino200Aug18.txt");
	TheLimits * genpointsWino = new TheLimits();
	if (pdfuncertinclInAcc) {
		genpointsWino->Fill("limits_GMSB/resultfileswinoPDFACC.txt");
	} else
		genpointsWino->Fill("limits_GMSB/resultfileswino200Aug18.txt");

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
	ExclusionCurves winoExcl;
	if (pdfuncertinclInAcc) {
		std::cout << "INCL!!:" << std::endl;
		winoExcl = DrawPlots(c1, plotToolsWino, plotToolsInterpolWino, "Wino",
				false, 200, true);

	} else {
		winoExcl = DrawPlots(c1, plotToolsWino, plotToolsInterpolWino, "Wino",
				false, 200);
	}
	TGraph * wino = winoExcl.exp;
		///////////////WINO LIMITS, NEUTR 150///////////////////////////////////////////////////
		//Get limits from signal scan ---------------------------------------------------
		TheLimits * genpointsInterpolWino150 = new TheLimits();
		genpointsInterpolWino150->Fill("limits_GMSB/resultfilesWino200Neutr150.txt");
		TheLimits * genpointsWino150 = new TheLimits();
		genpointsWino150->Fill("limits_GMSB/resultfilesWino200Neutr150.txt");

		//Make grid in Msquark, Mgluino finer by factor of 4 by linear interpolation
		genpointsInterpolWino150->ExpandGrid<SusyScan> (Msquark, Mgluino);
		genpointsInterpolWino150->ExpandGrid<SusyScan> (Msquark, Mgluino);
		// New 'pseudo' points are added, therefore the binning of all plots has to be made finer by a factor
		// of 2 in x and y for each "ExpandGrid
		//the plotting ----------------------------------------------------------------------
		//plotting helper functions
		PlotTools < SusyScan > *plotToolsWino150 = new PlotTools<SusyScan> (
				genpointsWino150->GetScan());

		PlotTools < SusyScan > *plotToolsInterpolWino150
				= new PlotTools<SusyScan> (genpointsInterpolWino150->GetScan());

		//the histograms ---------------------------------------------------------------------
		ExclusionCurves wino150Excl = DrawPlots(c1, plotToolsWino150,
				plotToolsInterpolWino150, "Wino150", false, 200);

		///////////////WINO LIMITS 100, NEUTR 150///////////////////////////////////////////////////
			//Get limits from signal scan ---------------------------------------------------
			TheLimits * genpointsInterpolWino100150 = new TheLimits();
			genpointsInterpolWino100150->Fill("limits_GMSB/resultfilesWino100Neutr150.txt");
			TheLimits * genpointsWino100150 = new TheLimits();
			genpointsWino100150->Fill("limits_GMSB/resultfilesWino100Neutr150.txt");

			//Make grid in Msquark, Mgluino finer by factor of 4 by linear interpolation
			genpointsInterpolWino100150->ExpandGrid<SusyScan> (Msquark, Mgluino);
			genpointsInterpolWino100150->ExpandGrid<SusyScan> (Msquark, Mgluino);
			// New 'pseudo' points are added, therefore the binning of all plots has to be made finer by a factor
			// of 2 in x and y for each "ExpandGrid
			//the plotting ----------------------------------------------------------------------
			//plotting helper functions
			PlotTools < SusyScan > *plotToolsWino100150 = new PlotTools<SusyScan> (
					genpointsWino100150->GetScan());

			PlotTools < SusyScan > *plotToolsInterpolWino100150
					= new PlotTools<SusyScan> (genpointsInterpolWino100150->GetScan());

			//the histograms ---------------------------------------------------------------------
			ExclusionCurves wino100150Excl = DrawPlots(c1, plotToolsWino100150,
					plotToolsInterpolWino100150, "Wino100150", false, 100);

	///////////////WINO LIMITS LOW///////////////////////////////////////////////////
	//		//Get limits from signal scan ---------------------------------------------------
	//		TheLimits * genpointsInterpolWinoLow = new TheLimits();
	//		genpointsInterpolWinoLow->Fill("limits_GMSB/resultfileswino100.txt");
	//		TheLimits * genpointsWinoLow = new TheLimits();
	//		genpointsWinoLow->Fill("limits_GMSB/resultfileswino100.txt");
	//
	//		//Make grid in Msquark, Mgluino finer by factor of 4 by linear interpolation
	//		genpointsInterpolWinoLow->ExpandGrid<SusyScan> (Msquark, Mgluino);
	//		genpointsInterpolWinoLow->ExpandGrid<SusyScan> (Msquark, Mgluino);
	//		// New 'pseudo' points are added, therefore the binning of all plots has to be made finer by a factor
	//		// of 2 in x and y for each "ExpandGrid
	//		//the plotting ----------------------------------------------------------------------
	//		//plotting helper functions
	//		PlotTools < SusyScan > *plotToolsWinoLow = new PlotTools<SusyScan> (
	//				genpointsWinoLow->GetScan());
	//
	//		PlotTools < SusyScan > *plotToolsInterpolWinoLow
	//				= new PlotTools<SusyScan> (genpointsInterpolWinoLow->GetScan());
	//
	//		//the histograms ---------------------------------------------------------------------
	//		ExclusionCurves winoLowExcl = DrawPlots(c1, plotToolsWinoLow,
	//				plotToolsInterpolWinoLow, "WinoLow",100);
	//		TGraph * winoLow = winoLowExcl.exp;
	//
	//	///////////////WINO LIMITS HIGH///////////////////////////////////////////////////
	//	//Get limits from signal scan ---------------------------------------------------
	//	TheLimits * genpointsInterpolWinoHigh = new TheLimits();
	//	genpointsInterpolWinoHigh->Fill("limits_GMSB/resultfileswino350.txt");
	//	TheLimits * genpointsWinoHigh = new TheLimits();
	//	genpointsWinoHigh->Fill("limits_GMSB/resultfileswino350.txt");
	//
	//	//Make grid in Msquark, Mgluino finer by factor of 4 by linear interpolation
	//	genpointsInterpolWinoHigh->ExpandGrid<SusyScan> (Msquark, Mgluino);
	//	genpointsInterpolWinoHigh->ExpandGrid<SusyScan> (Msquark, Mgluino);
	//	// New 'pseudo' points are added, therefore the binning of all plots has to be made finer by a factor
	//	// of 2 in x and y for each "ExpandGrid
	//	//the plotting ----------------------------------------------------------------------
	//	//plotting helper functions
	//	PlotTools < SusyScan > *plotToolsWinoHigh = new PlotTools<SusyScan> (
	//			genpointsWinoHigh->GetScan());
	//
	//	PlotTools < SusyScan > *plotToolsInterpolWinoHigh
	//			= new PlotTools<SusyScan> (genpointsInterpolWinoHigh->GetScan());
	//
	//	//the histograms ---------------------------------------------------------------------
	//	ExclusionCurves winoHighExcl = DrawPlots(c1, plotToolsWinoHigh,
	//			plotToolsInterpolWinoHigh, "WinoHigh",350);
	//	TGraph * winoHigh = winoHighExcl.exp;
	//
	//	//compare MET cuts
	//	c1->SetLogz(1);
	//	c1->SetRightMargin(0.08);
	//	TH2F*hexclWinoComp = new TH2F("hexcl",
	//			";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); 95% CL Exclusion", 80, 400,
	//			2000, 80, 400, 2000);
	//	hexclWinoComp->Draw("COLZ");
	//	wino->SetLineColor(2);
	//	winoHigh->SetLineColor(2);
	//	winoLow->SetLineColor(2);
	//	wino->SetLineStyle(1);
	//	winoHigh->SetLineStyle(2);
	//	winoLow->SetLineStyle(3);
	//	wino->Draw("");
	//	winoHigh->Draw("");
	//	winoLow->Draw("");
	//	TLegend * leg = new TLegend(0.41, 0.41, 0.93, 0.9);
	//	leg->SetBorderSize(0);
	//	leg->SetFillColor(0);
	//	leg->SetFillStyle(4000);
	//	leg->SetTextSize(0.025);
	//
	//	//leg->AddEntry(CLsObsNLO,"Observed, NLO","l");
	//
	//	leg->AddEntry(winoLow, "Expected MET>100", "l");
	//	leg->AddEntry(wino, "Expected, MET>200", "l");
	//	leg->AddEntry(winoHigh, "Expected, MET>350", "l");
	//
	//	leg->Draw();
	//	drawCmsPrel(0);
	//	string nameXsPlot = "results/WinoCOMPMetSelection_3jet";
	//	c1->SaveAs((nameXsPlot + ".pdf").c_str());
	//	c1->SaveAs((nameXsPlot + ".eps").c_str());
	//
	//compare Neutralino masses
	TGraph * winoExp = winoExcl.exp;
	TGraph * winoObs = winoExcl.obs;
	TGraph * winoOneSigBand = winoExcl.onesigband;

	TGraph * wino150Exp = wino150Excl.exp;
	TGraph * wino150Obs = wino150Excl.obs;
	TGraph * wino150OneSigBand = wino150Excl.onesigband;

	c1->SetLogz(1);
	c1->SetRightMargin(0.08);
	TH2F*hexclWinoCompNeutr = new TH2F("hexclNeutrComp",
			";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); 95% CL Exclusion", 92, 160,
			2000, 92, 160, 2000);

	hexclWinoCompNeutr->Draw("COLZ");

	winoOneSigBand->SetFillStyle(3001);
	winoOneSigBand->SetFillColor(kOrange);
	winoOneSigBand->Draw("f");
	winoObs->SetLineWidth(2);
	winoExp->SetLineWidth(2);
	winoObs->Draw("l");
	winoExp->Draw("l");

	wino150OneSigBand->SetFillStyle(3001);
	wino150OneSigBand->SetFillColor(kYellow);
	wino150OneSigBand->Draw("f");
	wino150Obs->SetLineWidth(2);
		wino150Exp->SetLineWidth(2);
	wino150Obs->Draw("l");
	wino150Exp->Draw("l");

	TH1F * dummy = new TH1F("dummy", "dummy", 5, 0, 5);
	dummy->SetLineColor(0);

	TLegend * leg2 = new TLegend(0.41, 0.41, 0.93, 0.88);
	leg2->SetBorderSize(0);
	leg2->SetFillColor(0);
	leg2->SetFillStyle(4000);
	leg2->SetTextSize(0.025);
	//leg2->AddEntry(dummy, "m_{#tilde{#Chi}^{0}} = 375 GeV", "l");
	leg2->AddEntry(winoObs, "Observed, m_{#tilde{#Chi}^{0}} = 375 GeV", "l");
	leg2->AddEntry(winoExp, "Expected, m_{#tilde{#Chi}^{0}} = 375 GeV", "l");
	leg2->AddEntry(winoOneSigBand, "#pm 1#sigma, m_{#tilde{#Chi}^{0}} = 375 GeV", "f");

	//leg2->AddEntry(dummy, "m_{#tilde{#Chi}^{0}} = 150 GeV ", "l");
	leg2->AddEntry(wino150Obs, "Observed, m_{#tilde{#Chi}^{0}} = 150 GeV", "l");
	leg2->AddEntry(wino150Exp, "Expected, m_{#tilde{#Chi}^{0}} = 150 GeV", "l");
	leg2->AddEntry(wino150OneSigBand, "#pm 1#sigma, m_{#tilde{#Chi}^{0}} = 150 GeV", "f");


	//leg->AddEntry(CLsExpNoSNLO,   "Expected, no-signal hyp., NLO","l");
	leg2->Draw();
	gPad->RedrawAxis();

	drawCmsPrel(200);
	string nameNeutrCompWinoPlot = "results/WinoCOMPNeutr_3jet";
	c1->SaveAs((nameNeutrCompWinoPlot + ".pdf").c_str());
	c1->SaveAs((nameNeutrCompWinoPlot + ".eps").c_str());

}

int main(int argc, char** argv) {
	return plot(argc, argv);
}
