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

struct ExclusionCurves {
	TGraph * obs;
	TGraph * exp;
	TGraph * onesigband;

};
struct ExclCurvesContainer {
	double lumi;
	ExclusionCurves jet2;
	ExclusionCurves jet3;
	ExclusionCurves best;
};

void DrawPlot_XS1D(TCanvas * c1, PlotTools<SusyScan> *plotTools,
	PlotTools<SusyScan> *plotToolsInterpol, string nameappendix, bool inclEdges, double METCut,
	bool bandAroundExpLimit) {
	TH1F *hobslimitaxes = new TH1F("xsec", ";m_{squark} (GeV/c^{2}); cross section [pb]", 5, 400,
									700);
	TGraph *hObsLimit1D = new TGraph(21);

	plotTools->Graph(hObsLimit1D, MSquarkGluino1600, ObsXsecLimitSigCont, 0);
	TGraph *hNLOXS = new TGraph(21);
	plotTools->Graph(hNLOXS, MSquarkGluino1600, NLOXsection, 0);
	TGraph *hNLOXSM1 = new TGraph(21);
	plotTools->Graph(hNLOXSM1, MSquarkGluino1600, NLOXsectionM1, 0);
	TGraph *hNLOXSP1 = new TGraph(21);
	plotTools->Graph(hNLOXSP1, MSquarkGluino1600, NLOXsectionP1, 0);
	hobslimitaxes->SetMaximum(0.5);
	hobslimitaxes->Draw("");
	hObsLimit1D->Draw("*");
	hNLOXS->SetLineColor(42);
	hNLOXS->Draw("C");
	hNLOXSM1->SetLineColor(42);
	hNLOXSM1->Draw("C");
	hNLOXSP1->SetLineColor(42);
	hNLOXSP1->Draw("C");
	TLegend * leg1DGl1600 = new TLegend(0.51, 0.51, 0.93, 0.88);
	leg1DGl1600->SetBorderSize(0);
	leg1DGl1600->SetFillColor(0);
	leg1DGl1600->SetFillStyle(4000);
	leg1DGl1600->SetTextSize(0.025);
	leg1DGl1600->SetHeader("guino mass=1600 GeV");
	leg1DGl1600->AddEntry(hObsLimit1D, "Observed", "p");
	leg1DGl1600->AddEntry(hNLOXS, "XS#pm1#sigma", "l");
	leg1DGl1600->Draw("same");
	drawCmsPrel(plotTools->SingleValue(Luminosity), METCut);
	string nameLimit1DPlot = "results/" + nameappendix + "/ObsLimit1DGluino1600_3jet";
	c1->SaveAs((nameLimit1DPlot + ".pdf").c_str());
	c1->SaveAs((nameLimit1DPlot + ".png").c_str());
	//observed limit 1D
	c1->SetTopMargin(0.11);
	c1->SetRightMargin(0.2);
	TH1F *hobslimitaxes2 = new TH1F("xsec", ";m_{squark} (GeV/c^{2}); cross section [pb]", 5, 400,
									700);
	TGraph *hObsLimit1D2 = new TGraph(21);
	plotTools->Graph(hObsLimit1D2, MSquarkGluino1360, ObsXsecLimitSigCont, 0);
	TGraph *hNLOXS2 = new TGraph(21);
	plotTools->Graph(hNLOXS2, MSquarkGluino1360, NLOXsection, 0);
	TGraph *hNLOXSM12 = new TGraph(21);
	plotTools->Graph(hNLOXSM12, MSquarkGluino1360, NLOXsectionM1, 0);
	TGraph *hNLOXSP12 = new TGraph(21);
	plotTools->Graph(hNLOXSP12, MSquarkGluino1360, NLOXsectionP1, 0);
	hobslimitaxes2->SetMaximum(0.5);
	hobslimitaxes2->Draw("");
	hObsLimit1D2->Draw("*");
	hNLOXS2->SetLineColor(42);
	hNLOXS2->Draw("C");
	hNLOXSM12->SetLineColor(42);
	hNLOXSM12->Draw("C");
	hNLOXSP1->SetLineColor(42);
	hNLOXSP1->Draw("C");
	TLegend * leg1DGl1360 = new TLegend(0.51, 0.51, 0.93, 0.88);
	leg1DGl1360->SetBorderSize(0);
	leg1DGl1360->SetFillColor(0);
	leg1DGl1360->SetFillStyle(4000);
	leg1DGl1360->SetTextSize(0.025);
	leg1DGl1360->SetHeader("guino mass=1360 GeV");
	leg1DGl1360->AddEntry(hObsLimit1D, "Observed", "p");
	leg1DGl1360->AddEntry(hNLOXS, "XS#pm1#sigma", "l");
	leg1DGl1360->Draw("same");
	drawCmsPrel(plotTools->SingleValue(Luminosity), METCut);
	string nameLimit1DPlot2 = "results/" + nameappendix + "/ObsLimit1DGluino1360_3jet";
	c1->SaveAs((nameLimit1DPlot2 + ".pdf").c_str());
	c1->SaveAs((nameLimit1DPlot2 + ".png").c_str());
}
void DrawPlot_XS(TCanvas * c1, PlotTools<SusyScan> *plotTools,
	PlotTools<SusyScan> *plotToolsInterpol, string nameappendix, bool inclEdges = false,
	double METCut = 200, bool bandAroundExpLimit = false) {
	TH2F *hxsec = new TH2F("xsec",
							";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); cross section [pb]",
							21, 400, 2020, 21, 400, 2020);
	if (nameappendix == "Wino150" || nameappendix == "Wino100150" || nameappendix == "Bino150") {
		hxsec = new TH2F("xsec",
							";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); cross section [pb]",
							24, 160, 2020, 24, 160, 2020);
	}
	if (nameappendix == "Chi1_SQ2500") {
		hxsec
			= new TH2F(
						"xsec",
						";m_{#chi^{0}_{1}} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); cross section [pb]",
						9, 150, 1050.1, 24, 160, 2000.1);
	}
	if (nameappendix == "Chi1_SQ2500") {
		plotTools->Area(hxsec, Mchi1, Mgluino, NLOXsection);
	} else
		plotTools->Area(hxsec, Msquark, Mgluino, NLOXsection);

	//hxsec->SetMinimum(0.00001);
	hxsec->GetZaxis()->SetTitleOffset(1.5);
	//sq500->Draw();   // m(squark) = 500 GeV line
	//gl500->Draw();   // m(gluino) = 500 GeV line
	hxsec->Draw("colz");
	drawCmsPrel(plotTools->SingleValue(Luminosity), METCut);
	string nameXsPlot = "results/" + nameappendix + "/Xsection_3jet";
	c1->SaveAs((nameXsPlot + ".pdf").c_str());
	c1->SaveAs((nameXsPlot + ".png").c_str());
}
void DrawPlot_SigAccAndCont(TCanvas * c1, PlotTools<SusyScan> *plotTools,
	PlotTools<SusyScan> *plotToolsInterpol, string nameappendix, bool inclEdges, double METCut,
	bool bandAroundExpLimit, bool dijet = false) {
	string jetapp = "3jet";
	int noJet = 3;
	if (dijet) {
		jetapp = "2jet";
		noJet = 2;
	}
	c1->SetLogz(0);
	TH2F *hsigacc =
		new TH2F("sigacc",
					";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); Signal Acceptance [%]", 21,
					400, 2080, 12, 400, 2080);//1360);

	if (nameappendix == "Wino150" || nameappendix == "Wino100150" || nameappendix == "Bino150") {
		hsigacc
			= new TH2F("sigacc",
						";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); Signal Acceptance [%]",
						24, 160, 2020, 24, 160, 2020);
	}
	if (nameappendix == "Chi1_SQ2500") {
		hsigacc
			= new TH2F(
						"sigacc",
						";m_{#chi^{0}_{1}} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); Signal Acceptance [%]",
						9, 150, 1050.1, 24, 160, 2000.1);

	}
	if (dijet) {
		if (nameappendix == "Chi1_SQ2500")
			plotTools->Area(hsigacc, Mchi1, Mgluino, SignalAcceptance2j);
		else
			plotTools->Area(hsigacc, Msquark, Mgluino, SignalAcceptance2j);
	} else {
		if (nameappendix == "Chi1_SQ2500")
			plotTools->Area(hsigacc, Mchi1, Mgluino, SignalAcceptance);
		else
			plotTools->Area(hsigacc, Msquark, Mgluino, SignalAcceptance);
	}
	if (nameappendix == "Wino" || nameappendix == "WinoLow" || nameappendix == "WinoHigh") {
		hsigacc->SetMinimum(0.0);
		//hsigacc->SetMaximum(8);
	} else if (nameappendix == "Wino150" || nameappendix == "Wino100150") {
		hsigacc->SetMinimum(0.0);
		//hsigacc->SetMaximum(5);
	} else {
		hsigacc->SetMinimum(0.0);
		//hsigacc->SetMaximum(55);
	}

	hsigacc->SetContour(14);
	hsigacc->GetZaxis()->SetTitleOffset(1.5);
	hsigacc->Draw("colz");
	drawCmsPrel(plotTools->SingleValue(Luminosity), METCut, false, noJet);

	string nameSigAcc = "results/" + nameappendix + "/SigAcc" + jetapp;
	c1->SaveAs((nameSigAcc + ".pdf").c_str());
	c1->SaveAs((nameSigAcc + ".png").c_str());

	// Signal Contamination in M0 - M1/2
	c1->SetRightMargin(0.2);
	c1->SetTopMargin(0.11);
	c1->SetLogz(0);
	TH2F *hsigcont =
		new TH2F("sigcont",
					";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); Signal Contamination [%]",
					21, 400, 2020, 21, 400, 2020);
	if (nameappendix == "Wino150" || nameappendix == "Wino100150" || nameappendix == "Bino150") {
		hsigcont
			= new TH2F(
						"sigcont",
						";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); Signal Contamination [%]",
						24, 160, 2020, 24, 160, 2020);
	}
	if (nameappendix == "Chi1_SQ2500") {
		hsigcont
			= new TH2F(
						"sigcont",
						";m_{#chi^{0}_{1}} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); Signal Acceptance [%]",
						9, 150, 1050.1, 24, 160, 2000.1);
	}
	if (dijet) {
		if (nameappendix == "Chi1_SQ2500")
			plotTools->Area(hsigcont, Mchi1, Mgluino, SignalContamination2j);
		else
			plotTools->Area(hsigcont, Msquark, Mgluino, SignalContamination2j);
	} else {
		if (nameappendix == "Chi1_SQ2500")
			plotTools->Area(hsigcont, Mchi1, Mgluino, SignalContamination);
		else
			plotTools->Area(hsigcont, Msquark, Mgluino, SignalContamination);
	}
	hsigcont->SetMinimum(0.0);
	hsigcont->SetMaximum(100);
	hsigcont->SetContour(14);
	hsigcont->GetZaxis()->SetTitleOffset(1.5);
	hsigcont->Draw("colz");
	drawCmsPrel(plotTools->SingleValue(Luminosity), METCut, false, noJet);
	string nameSigCont = "results/" + nameappendix + "/SigCont" + jetapp;
	c1->SaveAs((nameSigCont + ".pdf").c_str());
	c1->SaveAs((nameSigCont + ".png").c_str());
}
void DrawPlot_TrigEff(TCanvas * c1, PlotTools<SusyScan> *plotTools,
	PlotTools<SusyScan> *plotToolsInterpol, string nameappendix, bool inclEdges = false,
	double METCut = 200, bool bandAroundExpLimit = false) {
	c1->SetLogz(0);
	TH2F *htrigeff =
		new TH2F("trigeff",
					";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); Trigger efficiency [%]", 21,
					400, 2020, 21, 400, 2020);
	if (nameappendix == "Wino150" || nameappendix == "Wino100150" || nameappendix == "Bino150") {
		htrigeff
			= new TH2F("trigeff",
						";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); Trigger efficiency [%]",
						24, 160, 2020, 24, 160, 2020);
	}
	plotTools->Area(htrigeff, Msquark, Mgluino, TriggerEfficiency);
	htrigeff->SetMinimum(0.0);
	if (nameappendix == "Wino" || nameappendix == "WinoLow" || nameappendix == "WinoHigh"
		|| nameappendix == "Wino150" || nameappendix == "Wino100150") {
		htrigeff->SetMaximum(30);
	} else
		htrigeff->SetMaximum(30);
	htrigeff->SetContour(14);
	htrigeff->GetZaxis()->SetTitleOffset(1.5);
	htrigeff->Draw("colz");
	drawCmsPrel(plotTools->SingleValue(Luminosity), METCut);
	string nameTrigAcc = "results/" + nameappendix + "/TrigEff3jet";
	c1->SaveAs((nameTrigAcc + ".pdf").c_str());
	c1->SaveAs((nameTrigAcc + ".png").c_str());

	// Trigger efficiencyN90Hits
	c1->SetRightMargin(0.2);
	c1->SetTopMargin(0.11);
	c1->SetLogz(0);
	TH2F
		*htrigeffN90Hits =
			new TH2F(
						"trigeffN90Hits",
						";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); Trigger efficiency (N90Hits) [%]",
						21, 400, 2020, 21, 400, 2020);
	if (nameappendix == "Wino150" || nameappendix == "Wino100150" || nameappendix == "Bino150") {
		htrigeffN90Hits
			= new TH2F(
						"trigeffN90Hits",
						";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); Trigger efficiency (N90Hits) [%]",
						24, 160, 2020, 24, 160, 2020);
	}
	plotTools->Area(htrigeffN90Hits, Msquark, Mgluino, TriggerEfficiencyN90Hits);
	htrigeffN90Hits->SetMinimum(0.0);
	htrigeffN90Hits->SetMaximum(30);
	htrigeffN90Hits->SetContour(14);
	htrigeffN90Hits->GetZaxis()->SetTitleOffset(1.5);
	htrigeffN90Hits->Draw("colz");
	drawCmsPrel(plotTools->SingleValue(Luminosity), METCut);
	string nameTrigAcc2 = "results/" + nameappendix + "/TrigEffN90Hits3jet";
	c1->SaveAs((nameTrigAcc2 + ".pdf").c_str());
	c1->SaveAs((nameTrigAcc2 + ".png").c_str());
}
void DrawPlot_XSLimit(TCanvas * c1, PlotTools<SusyScan> *plotTools,
	PlotTools<SusyScan> *plotToolsInterpol, string nameappendix, bool inclEdges = false,
	double METCut = 200, bool bandAroundExpLimit = false) {
	c1->SetLogz(0);
	TH2F
		*hobslimit =
			new TH2F(
						"obslimit",
						";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); (Observed) 95% CL Upper Limit, pb",
						20, 400, 2020, 20, 400, 2020);
	if (nameappendix == "Wino150" || nameappendix == "Wino100150" || nameappendix == "Bino150") {
		hobslimit
			= new TH2F(
						"obslimit",
						";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); (Observed) 95% CL Upper Limit, pb",
						24, 160, 2020, 24, 160, 2020);
	}
	if (nameappendix == "Chi1_SQ2500") {
		hobslimit
			= new TH2F(
						"obslimit",
						";m_{#chi^{0}_{1}} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); (Observed) 95% CL Upper Limit, pb",
						9, 150, 1050.1, 24, 160, 2000.1);
	}
	if (nameappendix == "Chi1_SQ2500")
		plotTools->Area(hobslimit, Mchi1, Mgluino, ObsXsecLimit);
	else
		plotTools->Area(hobslimit, Msquark, Mgluino, ObsXsecLimit);
	if (nameappendix == "Wino") {
		hobslimit->SetMinimum(0.05);
	} else {
		hobslimit->SetMinimum(0.005);
		//hobslimit->SetMaximum(0.2);
	}
	hobslimit->Draw("colz");
	hobslimit->GetZaxis()->SetTitleOffset(1.5);
	drawCmsPrel(plotTools->SingleValue(Luminosity), METCut);
	string nameObs = "results/" + nameappendix + "/ObsLimit_3jet";
	c1->SaveAs((nameObs + ".pdf").c_str());
	c1->SaveAs((nameObs + ".png").c_str());

	// Expected Limit in M0 - M1/2
	c1->SetRightMargin(0.2);
	c1->SetTopMargin(0.11);

	c1->SetLogz(0);
	TH2F
		*hexplimit =
			new TH2F(
						"explimit",
						";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); (Expected) 95% CL Upper Limit, pb",
						21, 400, 2020, 21, 400, 2020);
	if (nameappendix == "Wino150" || nameappendix == "Wino100150" || nameappendix == "Bino150") {
		hexplimit
			= new TH2F(
						"explimit",
						";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); (Expected) 95% CL Upper Limit, pb",
						24, 160, 2020, 24, 160, 2020);
	}
	if (nameappendix == "Chi1_SQ2500") {
		hexplimit
			= new TH2F(
						"explimit",
						";m_{#chi^{0}_{1}} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); (Expected) 95% CL Upper Limit, pb",
						9, 150, 1050.1, 24, 160, 2000.1);

	}
	if (nameappendix == "Chi1_SQ2500")
		plotTools->Area(hexplimit, Mchi1, Mgluino, ExpXsecLimit);
	else
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
	drawCmsPrel(plotTools->SingleValue(Luminosity), METCut);
	string nameExp = "results/" + nameappendix + "/ExpLimit_3jet";
	c1->SaveAs((nameExp + ".pdf").c_str());
	c1->SaveAs((nameExp + ".png").c_str());
}
void DrawPlot_XSLimitSigCont(TCanvas * c1, PlotTools<SusyScan> *plotTools,
	PlotTools<SusyScan> *plotToolsInterpol, string nameappendix, bool inclEdges = false,
	double METCut = 200, bool bandAroundExpLimit = false, bool dijet = false, bool bestLimit =
		false) {
	string jetapp = "3jet";
	int noJet = 3;
	if (dijet) {
		jetapp = "2jet";
		noJet = 2;
	}
	if (bestLimit)
		jetapp = "2OR3jet";
	c1->SetLogz(0);
	TH2F
		*hobslimitSigCont =
			new TH2F(
						"obslimitSigCont",
						";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2});(Observed) 95% CL Upper Limit, pb",
						21, 400, 2020, 12, 400, 2020);//1360);
	if (nameappendix == "Wino150" || nameappendix == "Wino100150" || nameappendix == "Bino150") {
		hobslimitSigCont
			= new TH2F(
						"obslimitSigCont",
						";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2});(Observed) 95% CL Upper Limit, pb",
						24, 160, 2020, 24, 160, 2020);
	}
	if (nameappendix == "Chi1_SQ2500") {
		hobslimitSigCont
			= new TH2F(
						"obslimitSigCont",
						";m_{#chi^{0}_{1}} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); (Observed) 95% CL Upper Limit, pb",
						9, 150, 1050.1, 24, 160, 2000.1);
	}
	if (nameappendix == "Chi1_SQ2500") {
		if (dijet)
			plotTools->Area(hobslimitSigCont, Mchi1, Mgluino, ObsXsecLimitSigCont2j);
		else if (!bestLimit)
			plotTools->Area(hobslimitSigCont, Mchi1, Mgluino, ObsXsecLimitSigCont);
		if (bestLimit)
			plotTools->Area(hobslimitSigCont, Mchi1, Mgluino, ObsXsecLimitSigContBest);
	} else {
		if (dijet)
			plotTools->Area(hobslimitSigCont, Msquark, Mgluino, ObsXsecLimitSigCont2j);
		else if (!bestLimit)
			plotTools->Area(hobslimitSigCont, Msquark, Mgluino, ObsXsecLimitSigCont);
		if (bestLimit)
			plotTools->Area(hobslimitSigCont, Msquark, Mgluino, ObsXsecLimitSigContBest);
	}

	if (nameappendix == "Wino") {
		hobslimitSigCont->SetMinimum(0.05);
	}
	else if (nameappendix == "Chi1_SQ2500") {
		hobslimitSigCont->SetMaximum(0.15);
	}
	else {
		hobslimitSigCont->SetMinimum(0.005);
		//hobslimitSigCont->SetMaximum(0.45);
	}
	hobslimitSigCont->GetZaxis()->SetTitleOffset(1.5);
	hobslimitSigCont->GetXaxis()->SetNdivisions(505);
	hobslimitSigCont->Draw("colz");
	TGraph*gluinoNLSP = 0;
		if (nameappendix == "Chi1_SQ2500") {
			gluinoNLSP = GetNeutrNNLSP();

				gluinoNLSP->Draw("f");
				TLatex tex;
					tex.SetTextSize(0.03);
					tex.DrawLatex(800, 400, "#tilde{g} NLSP");
			}
	drawCmsPrel(plotTools->SingleValue(Luminosity), METCut, false, noJet, bestLimit);
	string nameObsSigCont = "results/" + nameappendix + "/ObsLimitSigCont_" + jetapp;
	c1->SaveAs((nameObsSigCont + ".pdf").c_str());
	c1->SaveAs((nameObsSigCont + ".png").c_str());
	c1->SaveAs((nameObsSigCont + ".png").c_str());
	//c1->SaveAs((nameObsSigCont + ".C").c_str());
	// Expected Limit in M0 - M1/2
	c1->SetRightMargin(0.2);
	c1->SetTopMargin(0.11);
	c1->SetLogz(0);
	TH2F
		*hexplimitSigCont =
			new TH2F(
						"explimitSigCont",
						";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); (Expected) 95% CL Upper Limit, pb",
						21, 400, 2020, 21, 400, 2020);
	if (nameappendix == "Wino150" || nameappendix == "Wino100150" || nameappendix == "Bino150") {
		hexplimitSigCont
			= new TH2F(
						"explimitSigCont",
						";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); (Expected) 95% CL Upper Limit, pb",
						24, 160, 2020, 24, 160, 2020);
	}if (nameappendix == "Chi1_SQ2500") {
		hexplimitSigCont
					= new TH2F(
								"explimitSigCont",
						";m_{#chi^{0}_{1}} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); (Observed) 95% CL Upper Limit, pb",
						9, 150, 1050.1, 24, 160, 2000.1);
	}
	if (nameappendix == "Chi1_SQ2500") {
		if (dijet)
			plotTools->Area(hexplimitSigCont, Mchi1, Mgluino, ExpXsecLimitSigCont2j);
		else
			plotTools->Area(hexplimitSigCont, Mchi1, Mgluino, ExpXsecLimitSigCont);
		if (bestLimit)
			plotTools->Area(hexplimitSigCont, Mchi1, Mgluino, ExpXsecLimitSigContBest);

	} else {
		if (dijet)
			plotTools->Area(hexplimitSigCont, Msquark, Mgluino, ExpXsecLimitSigCont2j);
		else
			plotTools->Area(hexplimitSigCont, Msquark, Mgluino, ExpXsecLimitSigCont);
		if (bestLimit)
			plotTools->Area(hexplimitSigCont, Msquark, Mgluino, ExpXsecLimitSigContBest);
	}
	if (nameappendix == "Wino" || nameappendix == "WinoLow" || nameappendix == "WinoHigh") {
		hexplimitSigCont->SetMinimum(0.05);

	}else if (nameappendix == "Chi1_SQ2500") {
		hexplimitSigCont->SetMaximum(0.15);
	}
	if (nameappendix == "Wino150" || nameappendix == "Wino100150") {
		hexplimitSigCont->SetMinimum(0.05);
		hexplimitSigCont->SetMaximum(3);
	} else {
		hexplimitSigCont->SetMinimum(0.005);
	}
	hexplimitSigCont->GetZaxis()->SetTitleOffset(1.5);
	hexplimitSigCont->Draw("colz");
	if(gluinoNLSP!=0){gluinoNLSP->Draw("f");
	TLatex tex;
						tex.SetTextSize(0.03);
						tex.DrawLatex(800, 400, "#tilde{g} NLSP");}
	drawCmsPrel(plotTools->SingleValue(Luminosity), METCut, false, noJet, bestLimit);
	string nameExpSigCont = "results/" + nameappendix + "/ExpLimitSigCont_" + jetapp;
	c1->SaveAs((nameExpSigCont + ".pdf").c_str());
	c1->SaveAs((nameExpSigCont + ".png").c_str());
}
void DrawPlot_BestLimit20r3jet(TCanvas * c1, PlotTools<SusyScan> *plotTools,
	PlotTools<SusyScan> *plotToolsInterpol, string nameappendix, bool inclEdges, double METCut,
	bool bandAroundExpLimit) {
	c1->SetLogz(0);
	TH2F
		*bestLim =
			new TH2F(
						"texcl",
						";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); #Jets with best expected XS Limit",
						21, 400, 2020, 12, 400, 2020);
	if (nameappendix == "Wino150" || nameappendix == "Wino100150" || nameappendix == "Bino150") {
		bestLim
			= new TH2F(
						"texcl",
						";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); #Jets with best expected XS Limit",
						24, 160, 2020, 24, 160, 2020);
	}
	if (nameappendix == "Chi1_SQ2500") {
		bestLim
			= new TH2F("texcl",
						";m_{#chi^{0}_{1}} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); 95% CL Exclusion",
						9, 150, 1050.1, 24, 160, 2000.1);
	}
	if (nameappendix == "Chi1_SQ2500")
		plotTools->Area(bestLim, Mchi1, Mgluino, BestXSec);
	else
		plotTools->Area(bestLim, Msquark, Mgluino, BestXSec);
	bestLim->Draw("colz");
	if (nameappendix == "Chi1_SQ2500") {
			TGraph * gluinoNLSP = GetNeutrNNLSP();
			gluinoNLSP->Draw("f");
					TLatex tex;
						tex.SetTextSize(0.03);
						tex.DrawLatex(800, 400, "#tilde{g} NLSP");
		}
	string nameTestCont = "results/" + nameappendix + "ExclusionBestLimitFrom2or3jet.pdf";
	c1->SaveAs((nameTestCont + ".pdf").c_str());
		c1->SaveAs((nameTestCont + ".eps").c_str());

}
void DrawPlot_TestContour(TCanvas * c1, PlotTools<SusyScan> *plotTools,
	PlotTools<SusyScan> *plotToolsInterpol, string nameappendix, bool inclEdges = false,
	double METCut = 200, bool bandAroundExpLimit = false) {
	c1->SetLogz(0);
	TH2F *TestCont =
		new TH2F("texcl",
					";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); 95% CL Expected Exclusion",
					82, 380, 2020, 82, 380, 2020);
	//21, 400, 2020, 21, 400, 2020);
	if (nameappendix == "Wino150" || nameappendix == "Wino100150" || nameappendix == "Bino150") {
		TestCont
			= new TH2F(
						"texcl",
						";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); 95% CL Expected Exclusion",
						//24, 160, 2020, 24, 160, 2020);
						93, 160, 2020, 93, 160, 2020);
	}
	if (nameappendix == "Chi1_SQ2500") {
		TestCont
			= new TH2F(
						"texcl",
						";m_{#chi^{0}_{1}} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); 95% CL Expected Exclusion",
						33, 150, 1050.1, 93, 160, 2000.1);
	}
	/// Find the correct contour for any of the exclusion plots--->get right contour number!::::::::::::::::::::::
	if (nameappendix == "Chi1_SQ2500")
		plotToolsInterpol->Area(TestCont, Mchi1, Mgluino, ObsExclusionSigCont);
	else
		plotToolsInterpol->Area(TestCont, Msquark, Mgluino, ObsExclusionSigContBest);
	/// try "3" contour levels:
	std::vector<TGraph*> contours = plotToolsInterpol->GetContours(TestCont, 3);
	/// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	TestCont->Draw("colz");
	int col = kBlue - 10;
	for (std::vector<TGraph*>::iterator cont = contours.begin(); cont != contours.end(); ++cont) {
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
	string nameTestCont = "results/" + nameappendix + "ExclusionTestContoursExp_3jet.pdf";
	c1->SaveAs(nameTestCont.c_str());
	TFile * output = new TFile("output.root", "Update");
}

ExclusionCurves DrawPlot_ExclusionCurves(TCanvas * c1, PlotTools<SusyScan> *plotTools, PlotTools<
	SusyScan> *plotToolsInterpol, string nameappendix, bool inclEdges, double METCut,
	bool bandAroundExpLimit, bool dijet = false, bool bestLimit = false) {

	string jetapp = "3jet";
	int noJet = 3;
	if (dijet) {
		jetapp = "2jet";
		noJet = 2;
	}
	if (bestLimit)
		jetapp = "2OR3jet";
	c1->SetLogz(0);
	c1->SetRightMargin(0.08);
	c1->SetTopMargin(0.11);
	TH2F *hexcl =0;

	if (nameappendix == "Wino150" || nameappendix == "Wino100150" || nameappendix == "Bino150") {
		hexcl = new TH2F("hexcl",
							";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); 95% CL Exclusion",
							93, 160, 2020, 93, 160, 2020);
		//24,160, 2000, 24, 160, 2000);
	}
	else if (nameappendix == "Wino" || nameappendix == "Bino") {
		hexcl = new TH2F("hexcl",
							";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); 95% CL Exclusion",
							81, 400, 2020, 65, 400, 2020);//1360.1);
		//24,160, 2000, 24, 160, 2000);
	}
	else if (nameappendix == "Chi1_SQ2500") {
		hexcl
			= new TH2F("hexcl",
						";m_{#chi^{0}_{1}} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); 95% CL Exclusion",
						33, 150, 1050.1, 93, 160, 2000.1);
	}
	else{
		hexcl = new TH2F("hexcl",
									";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); 95% CL Exclusion",
									81, 400, 2020, 81, 400, 2020);
	}

	// hs defined range in which the contrours are calculated, the binning matters! binning should be equal to signal-scan
	TH2F * hs = 0;

	if (inclEdges) {
		hs = new TH2F("hs", "", 82, 380, 2020, 82, 380, 2020);
	}
	else if (nameappendix == "Wino150" || nameappendix == "Wino100150" || nameappendix == "Bino150") {
		hs = new TH2F("hs", "", 93, 160, 2020, 92, 160, 2020);
		//;24,160, 2020, 24, 160, 2020);
	}
	else if (nameappendix == "Chi1_SQ2500") {
		hs = new TH2F("hs", "", 33, 150, 1050.1, 93, 160, 2000.1);
	}
	else{
		hs=new TH2F("hs", "", 81, 400, 2020, 81, 400, 2020);
	}

	// usage: GetContour( <range-hist>, <x-var>, <y-var>, <limit-function>, <contour-levels>, <contour index>, <color>, <style> )

	TGraph * gCLsExpExcl_LO = plotToolsInterpol->GetContour(hs, Msquark, Mgluino, ExpExclusion, 3,
															0, 2, 4);
	TGraph * gCLsObsExcl_LO = plotToolsInterpol->GetContour(hs, Msquark, Mgluino, ObsExclusion, 3,
															0, 2, 1);
	TGraph * gCLsExpExclm1 = plotToolsInterpol->GetContour(hs, Msquark, Mgluino, ExpExclusionM1, 3,
															0, 5, 2);
	TGraph * gCLsExpExclp1 = plotToolsInterpol->GetContour(hs, Msquark, Mgluino, ExpExclusionP1, 3,
															0, 5, 2);
	TGraph * gCLsExpExclm2 = plotToolsInterpol->GetContour(hs, Msquark, Mgluino, ExpExclusionM2, 3,
															0, 5, 2);
	TGraph * gCLsExpExclp2 = plotToolsInterpol->GetContour(hs, Msquark, Mgluino, ExpExclusionP2, 3,
															0, 5, 2);
	TGraph * gCLsObsExclm1 = plotToolsInterpol->GetContour(hs, Msquark, Mgluino, ObsExclusionM1, 3,
															0, 5, 2);
	TGraph * gCLsObsExclp1 = plotToolsInterpol->GetContour(hs, Msquark, Mgluino, ObsExclusionP1, 3,
															0, 5, 2);
	TGraph * gCLsObsExclm2 = plotToolsInterpol->GetContour(hs, Msquark, Mgluino, ObsExclusionM2, 3,
															0, 5, 2);
	TGraph * gCLsObsExclp2 = plotToolsInterpol->GetContour(hs, Msquark, Mgluino, ObsExclusionP2, 3,
															0, 5, 2);

	TGraph * gCLsExpExcl_LOSigCont = 0;
	TGraph * gCLsObsExcl_LOSigCont =0;
	TGraph * gCLsExpExclm1SigCont = 0;
	TGraph * gCLsExpExclp1SigCont = 0;
	TGraph * gCLsExpExclm2SigCont = 0;
	TGraph * gCLsExpExclp2SigCont = 0;
	TGraph * gCLsObsExclm1SigCont = 0;
	TGraph * gCLsObsExclp1SigCont = 0;
	TGraph * gCLsObsExclm2SigCont = 0;
	TGraph * gCLsObsExclp2SigCont =0;
	if (nameappendix == "Chi1_SQ2500") {
		gCLsExpExcl_LOSigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																ExpExclusionSigCont, 3, 0,
																kRed - 2, 4);
		gCLsObsExcl_LOSigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																ObsExclusionSigCont, 3, 0,
																kRed - 2, 1);
		gCLsExpExclm1SigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																ExpExclusionM1SigCont, 3, 0,
																kOrange, 2);
		gCLsExpExclp1SigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																ExpExclusionP1SigCont, 3, 0,
																kOrange, 2);
		gCLsExpExclm2SigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																ExpExclusionM2SigCont, 3, 0,
																kOrange, 2);
		gCLsExpExclp2SigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																ExpExclusionP2SigCont, 3, 0,
																kOrange, 2);
		gCLsObsExclm1SigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																ObsExclusionM1SigCont, 3, 0,
																kOrange, 2);
		gCLsObsExclp1SigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																ObsExclusionP1SigCont, 3, 0,
																kOrange, 2);
		gCLsObsExclm2SigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																ObsExclusionM2SigCont, 3, 0,
																kOrange, 2);
		gCLsObsExclp2SigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																ObsExclusionP2SigCont, 3, 0,
																kOrange, 2);
	}
	else{
		gCLsExpExcl_LOSigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																			ExpExclusionSigCont, 3, 0, kRed
																				- 2, 4);
			gCLsObsExcl_LOSigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																			ObsExclusionSigCont, 3, 0, kRed
																				- 2, 1);
			gCLsExpExclm1SigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																			ExpExclusionM1SigCont, 3, 0,
																			kOrange, 2);
			gCLsExpExclp1SigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																			ExpExclusionP1SigCont, 3, 0,
																			kOrange, 2);
			gCLsExpExclm2SigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																			ExpExclusionM2SigCont, 3, 0,
																			kOrange, 2);
			gCLsExpExclp2SigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																			ExpExclusionP2SigCont, 3, 0,
																			kOrange, 2);
			gCLsObsExclm1SigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																			ObsExclusionM1SigCont, 3, 0,
																			kOrange, 2);
			gCLsObsExclp1SigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																			ObsExclusionP1SigCont, 3, 0,
																			kOrange, 2);
			gCLsObsExclm2SigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																			ObsExclusionM2SigCont, 3, 0,
																			kOrange, 2);
			gCLsObsExclp2SigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																			ObsExclusionP2SigCont, 3, 0,
																			kOrange, 2);
	}
	if (dijet) {
		if (nameappendix == "Chi1_SQ2500") {
			gCLsExpExcl_LOSigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																	ExpExclusionSigCont2j, 3, 0,
																	kRed - 2, 4);
			gCLsObsExcl_LOSigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																	ObsExclusionSigCont2j, 3, 0,
																	kRed - 2, 1);
			gCLsExpExclm1SigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																	ExpExclusionM1SigCont2j, 3, 0,
																	kOrange, 2);
			gCLsExpExclp1SigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																	ExpExclusionP1SigCont2j, 3, 0,
																	kOrange, 2);
			gCLsExpExclm2SigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																	ExpExclusionM2SigCont2j, 3, 0,
																	kOrange, 2);
			gCLsExpExclp2SigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																	ExpExclusionP2SigCont2j, 3, 0,
																	kOrange, 2);
			gCLsObsExclm1SigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																	ObsExclusionM1SigCont2j, 3, 0,
																	kOrange, 2);
			gCLsObsExclp1SigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																	ObsExclusionP1SigCont2j, 3, 0,
																	kOrange, 2);
			gCLsObsExclm2SigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																	ObsExclusionM2SigCont2j, 3, 0,
																	kOrange, 2);
			gCLsObsExclp2SigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																	ObsExclusionP2SigCont2j, 3, 0,
																	kOrange, 2);
		} else {
			gCLsExpExcl_LOSigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																	ExpExclusionSigCont2j, 3, 0,
																	kRed - 2, 4);
			gCLsObsExcl_LOSigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																	ObsExclusionSigCont2j, 3, 0,
																	kRed - 2, 1);
			gCLsExpExclm1SigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																	ExpExclusionM1SigCont2j, 3, 0,
																	kOrange, 2);
			gCLsExpExclp1SigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																	ExpExclusionP1SigCont2j, 3, 0,
																	kOrange, 2);
			gCLsExpExclm2SigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																	ExpExclusionM2SigCont2j, 3, 0,
																	kOrange, 2);
			gCLsExpExclp2SigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																	ExpExclusionP2SigCont2j, 3, 0,
																	kOrange, 2);
			gCLsObsExclm1SigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																	ObsExclusionM1SigCont2j, 3, 0,
																	kOrange, 2);
			gCLsObsExclp1SigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																	ObsExclusionP1SigCont2j, 3, 0,
																	kOrange, 2);
			gCLsObsExclm2SigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																	ObsExclusionM2SigCont2j, 3, 0,
																	kOrange, 2);
			gCLsObsExclp2SigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																	ObsExclusionP2SigCont2j, 3, 0,
																	kOrange, 2);
		}
	}
	if (bestLimit) {
		if (nameappendix == "Chi1_SQ2500") {
			gCLsExpExcl_LOSigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																	ExpExclusionSigContBest, 3, 0,
																	kRed - 2, 4);
			gCLsObsExcl_LOSigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																	ObsExclusionSigContBest, 3, 0,
																	kRed - 2, 1);
			gCLsExpExclm1SigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																	ExpExclusionM1SigContBest, 3,
																	0, kOrange, 2);
			gCLsExpExclp1SigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																	ExpExclusionP1SigContBest, 3,
																	0, kOrange, 2);
			gCLsExpExclm2SigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																	ExpExclusionM2SigContBest, 3,
																	0, kOrange, 2);
			gCLsExpExclp2SigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																	ExpExclusionP2SigContBest, 3,
																	0, kOrange, 2);
			gCLsObsExclm1SigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																	ObsExclusionM1SigContBest, 3,
																	0, kOrange, 2);
			gCLsObsExclp1SigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																	ObsExclusionP1SigContBest, 3,
																	0, kOrange, 2);
			gCLsObsExclm2SigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																	ObsExclusionM2SigContBest, 3,
																	0, kOrange, 2);
			gCLsObsExclp2SigCont = plotToolsInterpol->GetContour(hs, Mchi1, Mgluino,
																	ObsExclusionP2SigContBest, 3,
																	0, kOrange, 2);
		} else {
			gCLsExpExcl_LOSigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																	ExpExclusionSigContBest, 3, 0,
																	kRed - 2, 4);
			gCLsObsExcl_LOSigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																	ObsExclusionSigContBest, 3, 0,
																	kRed - 2, 1);
			gCLsExpExclm1SigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																	ExpExclusionM1SigContBest, 3,
																	0, kOrange, 2);
			gCLsExpExclp1SigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																	ExpExclusionP1SigContBest, 3,
																	0, kOrange, 2);
			gCLsExpExclm2SigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																	ExpExclusionM2SigContBest, 3,
																	0, kOrange, 2);
			gCLsExpExclp2SigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																	ExpExclusionP2SigContBest, 3,
																	0, kOrange, 2);
			gCLsObsExclm1SigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																	ObsExclusionM1SigContBest, 3,
																	0, kOrange, 2);
			gCLsObsExclp1SigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																	ObsExclusionP1SigContBest, 3,
																	0, kOrange, 2);
			gCLsObsExclm2SigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																	ObsExclusionM2SigContBest, 3,
																	0, kOrange, 2);
			gCLsObsExclp2SigCont = plotToolsInterpol->GetContour(hs, Msquark, Mgluino,
																	ObsExclusionP2SigContBest, 3,
																	0, kOrange, 2);
		}
	}

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

	TGraph * gCLsObsExcl_LO_SmoothSigCont = (TGraph *) gCLsObsExcl_LOSigCont->Clone("1");
	TGraph * gCLsExpExcl_LO_SmoothSigCont = (TGraph *) gCLsExpExcl_LOSigCont->Clone("2");
	TGraph * gCLsExpExclm1_SmoothSigCont = (TGraph *) gCLsExpExclm1SigCont->Clone("3");
	TGraph * gCLsExpExclp1_SmoothSigCont = (TGraph *) gCLsExpExclp1SigCont->Clone("4");
	TGraph * gCLsExpExclm2_SmoothSigCont = (TGraph *) gCLsExpExclm2SigCont->Clone("5");
	TGraph * gCLsExpExclp2_SmoothSigCont = (TGraph *) gCLsExpExclp2SigCont->Clone("6");
	TGraph * gCLsObsExclm1_SmoothSigCont = (TGraph *) gCLsObsExclm1SigCont->Clone("7");
	TGraph * gCLsObsExclp1_SmoothSigCont = (TGraph *) gCLsObsExclp1SigCont->Clone("8");
	TGraph * gCLsObsExclm2_SmoothSigCont = (TGraph *) gCLsObsExclm2SigCont->Clone("9");
	TGraph * gCLsObsExclp2_SmoothSigCont = (TGraph *) gCLsObsExclp2SigCont->Clone("10");

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

	TGraph * gCLs1SigmaSigCont = MakeBand(gCLsObsExclm1_SmoothSigCont, gCLsObsExclp1_SmoothSigCont);
	if (bandAroundExpLimit) {
		gCLs1SigmaSigCont = MakeBand(gCLsExpExclm1_SmoothSigCont, gCLsExpExclp1_SmoothSigCont);
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

	TGraph * gCLs2SigmaSigCont = MakeBand(gCLsObsExclm2_SmoothSigCont, gCLsObsExclp2_SmoothSigCont);
	if (bandAroundExpLimit) {
		gCLs2SigmaSigCont = MakeBand(gCLsExpExclm2_SmoothSigCont, gCLsExpExclp2_SmoothSigCont);
	}
	gCLs2SigmaSigCont->SetFillStyle(3001);
	gCLs2SigmaSigCont->SetFillColor(5);

	TGraph * gCLs2Sigma = MakeBand(gCLsObsExclm2_Smooth, gCLsObsExclp2_Smooth);
	if (bandAroundExpLimit) {
		gCLs2Sigma = MakeBand(gCLsExpExclm2_Smooth, gCLsExpExclp2_Smooth);
	}
	gCLs2Sigma->SetFillStyle(3001);
	gCLs2Sigma->SetFillColor(kYellow - 9);
	TGraph*gluinoNLSP = 0;
	if (nameappendix == "Chi1_SQ2500") {
		gluinoNLSP = GetNeutrNNLSP();
	}

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
	if (nameappendix == "Chi1_SQ2500"){
		gluinoNLSP->Draw("f");
		TLatex tex;
			tex.SetTextSize(0.03);
			tex.DrawLatex(800, 400, "#tilde{g} NLSP");
	}
	TLegend * leg = new TLegend(0.49, 0.49, 0.93, 0.9);
	leg->SetBorderSize(0);
	leg->SetFillColor(0);
	leg->SetFillStyle(4000);
	leg->SetTextSize(0.025);

	leg->SetHeader("m_{#tilde{#Chi}^{0}} = 375 GeV");
	if (nameappendix == "Wino150" || nameappendix == "Wino100150" || nameappendix == "Bino150") {
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
	leg->AddEntry(gCLsObsExcl_LO_SmoothSigCont, "Observed, NLO, - Sig. Cont. ", "l");
	if (bandAroundExpLimit) {
		leg->AddEntry(gCLs1SigmaSigCont, "Expected #pm 1#sigma, NLO, - Sig. Cont.", "lf");
	} else {
		leg->AddEntry(gCLs1SigmaSigCont, "Observed #pm 1#sigma, NLO, - Sig. Cont.", "lf");
	}
	leg->AddEntry(gCLsExpExcl_LO_SmoothSigCont, "Expected, NLO, - Sig. Cont. ", "l");

	//leg->AddEntry(CLsExpNoSNLO,   "Expected, no-signal hyp., NLO","l");
	leg->Draw();
	gPad->RedrawAxis();

	drawCmsPrel(plotTools->SingleValue(Luminosity), METCut, false, noJet, bestLimit);

	string nameExcl = "results/" + nameappendix + "/Exclusion_" + jetapp;
	if (!dijet && !bestLimit) {
		//currently only SigCont subtracted limit is available for dijet!
		c1->SaveAs((nameExcl + ".pdf").c_str());
		c1->SaveAs((nameExcl + ".png").c_str());
	}
	//create clone of result plot but only signal contamination corrected limit drawn
	hexcl->Draw("colz");
	double min1 = hs->GetXaxis()->GetBinLowEdge(0);
	double min2 = hs->GetYaxis()->GetBinLowEdge(0);
	double max2 = hs->GetYaxis()->GetBinLowEdge(hs->GetNbinsX() + 1);
	double max1 = hs->GetXaxis()->GetBinLowEdge(hs->GetNbinsX() + 1);
	TGraph * excludedRegion =
		GetExcludedRegion(gCLsObsExclm1_SmoothSigCont, min1, min2, max1, max2);
	excludedRegion->Draw("f");
	float yv = 0.25;
	if (nameappendix == "Wino150" || nameappendix == "Wino100150" || nameappendix == "Wino")
		yv = 0.22;
	if (nameappendix == "Chi1_SQ2500") {
		yv = 0.35;
	}
	TLatex* lat3 = new TLatex(0.25, yv, "Excluded");
	lat3->SetNDC(true);
	lat3->SetTextSize(0.04);
	lat3->Draw("same");

	gCLs1SigmaSigCont->Draw("f");
	gCLsObsExcl_LO_SmoothSigCont->SetLineWidth(3);
	gCLsObsExcl_LO_SmoothSigCont->SetLineStyle(1);
	gCLsObsExcl_LO_SmoothSigCont->SetLineColor(1);
	gCLsExpExcl_LO_SmoothSigCont->SetLineWidth(3);
	gCLsExpExcl_LO_SmoothSigCont->SetLineStyle(2);
	gCLsExpExcl_LO_SmoothSigCont->SetLineColor(1);
	gCLsObsExcl_LO_SmoothSigCont->Draw("l");
	gCLsExpExcl_LO_SmoothSigCont->Draw("l");
	if (nameappendix == "Chi1_SQ2500"){
			gluinoNLSP->Draw("f");
			TLatex tex;
				tex.SetTextSize(0.03);
				tex.DrawLatex(800, 400, "#tilde{g} NLSP");
		}

	drawCmsPrelInCanvas(plotTools->SingleValue(Luminosity));
	string bino = "bino";
	if (nameappendix == "Wino" || nameappendix == "Wino150") {
		bino = "wino";
	}
	TLegend* leg2 = new TLegend(0.46, 0.73, 0.95, 0.55, (bino
		+ "-like,  m_{#tilde{#Chi}^{0}} = 375 (GeV/c^{2})").c_str());
	//TLegend* leg2 = new TLegend(0.55,0.55,0.8,0.8,"m_{#tilde{#Chi}^{0}} = 375 (GeV/c^{2})");
	//TLegend * leg2 = new TLegend(0.51, 0.51, 0.93, 0.88);
	leg2->SetBorderSize(0);
	leg2->SetFillColor(0);
	leg2->SetFillStyle(4000);
	leg2->SetTextSize(0.03);
	//leg2->SetHeader("m_{#tilde{#Chi}^{0}} = 375 GeV");
	if (nameappendix == "Wino150" || nameappendix == "Wino100150" || nameappendix == "Bino150") {
		leg2->SetHeader((bino + "-like,  m_{#tilde{#Chi}^{0}} = 150 (GeV/c^{2})").c_str());

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
	drawCmsPrel(plotTools->SingleValue(Luminosity), METCut, true, noJet, bestLimit);
	string nameExclSigCont = "results/" + nameappendix + "/ExclusionOnlySigCont_" + jetapp;
	c1->SaveAs((nameExclSigCont + ".pdf").c_str());
	c1->SaveAs((nameExclSigCont + ".png").c_str());
	c1->SaveAs((nameExclSigCont + ".eps").c_str());
	c1->SaveAs((nameExclSigCont + ".C").c_str());

	ExclusionCurves exclCurv;
	exclCurv.exp = gCLsExpExcl_LO_SmoothSigCont;
	exclCurv.obs = gCLsObsExcl_LO_SmoothSigCont;
	exclCurv.onesigband = gCLs1SigmaSigCont;
	return exclCurv;
}
ExclCurvesContainer DrawPlots(TCanvas * c1, PlotTools<SusyScan> *plotTools,
	PlotTools<SusyScan> *plotToolsInterpol, string nameappendix, bool inclEdges = false,
	double METCut = 200, bool bandAroundExpLimit = false) {
	std::system(("mkdir results/" + nameappendix).c_str());
	std::cout << "Draw band around expected limit? " << bandAroundExpLimit << std::endl;
	c1->SetLogz(1);
	c1->SetTopMargin(0.11);
	c1->SetRightMargin(0.2);

	DrawPlot_XS1D(c1, plotTools, plotToolsInterpol, nameappendix, inclEdges, METCut,
					bandAroundExpLimit);

	DrawPlot_XS(c1, plotTools, plotToolsInterpol, nameappendix, inclEdges, METCut,
				bandAroundExpLimit);

	DrawPlot_SigAccAndCont(c1, plotTools, plotToolsInterpol, nameappendix, inclEdges, METCut,
							bandAroundExpLimit);
	//Dijet!
	DrawPlot_SigAccAndCont(c1, plotTools, plotToolsInterpol, nameappendix, inclEdges, METCut,
							bandAroundExpLimit, true);

	DrawPlot_TrigEff(c1, plotTools, plotToolsInterpol, nameappendix, inclEdges, METCut,
						bandAroundExpLimit);

	DrawPlot_XSLimit(c1, plotTools, plotToolsInterpol, nameappendix, inclEdges, METCut,
						bandAroundExpLimit);

	DrawPlot_XSLimitSigCont(c1, plotTools, plotToolsInterpol, nameappendix, inclEdges, METCut,
							bandAroundExpLimit);
	//Dijet!
	DrawPlot_XSLimitSigCont(c1, plotTools, plotToolsInterpol, nameappendix, inclEdges, METCut,
							bandAroundExpLimit, true);
	//Dijet OR Trijet - best!
	DrawPlot_XSLimitSigCont(c1, plotTools, plotToolsInterpol, nameappendix, inclEdges, METCut,
							bandAroundExpLimit, false, true);
	DrawPlot_TestContour(c1, plotTools, plotToolsInterpol, nameappendix, inclEdges, METCut,
							bandAroundExpLimit);
	DrawPlot_BestLimit20r3jet(c1, plotTools, plotToolsInterpol, nameappendix, inclEdges, METCut,
								bandAroundExpLimit);
	//Dijet!
	ExclusionCurves dijetExcl = DrawPlot_ExclusionCurves(c1, plotTools, plotToolsInterpol,
															nameappendix, inclEdges, METCut,
															bandAroundExpLimit, true);
	//Dijet OR Trijet - best!
	ExclusionCurves bestExcl = DrawPlot_ExclusionCurves(c1, plotTools, plotToolsInterpol,
														nameappendix, inclEdges, METCut,
														bandAroundExpLimit, true, true);
	ExclusionCurves trijetExcl = DrawPlot_ExclusionCurves(c1, plotTools, plotToolsInterpol,
															nameappendix, inclEdges, METCut,
															bandAroundExpLimit);
	ExclCurvesContainer exclCurvesCont;
	exclCurvesCont.best = bestExcl;
	exclCurvesCont.jet3 = trijetExcl;
	exclCurvesCont.jet2 = dijetExcl;
	exclCurvesCont.lumi = plotTools->SingleValue(Luminosity);
	return exclCurvesCont;

}
ExclCurvesContainer DrawPlotsChi1(TCanvas * c1, PlotTools<SusyScan> *plotTools,
	PlotTools<SusyScan> *plotToolsInterpol, string nameappendix, bool inclEdges = false,
	double METCut = 200, bool bandAroundExpLimit = false) {
	std::system(("mkdir results/" + nameappendix).c_str());
	c1->SetLogz(1);
	c1->SetTopMargin(0.11);
	c1->SetRightMargin(0.2);

	//XS
	DrawPlot_XS(c1, plotTools, plotToolsInterpol, nameappendix, inclEdges, METCut,
				bandAroundExpLimit);
	// Signal Acceptance in M0 - M1/2
	DrawPlot_SigAccAndCont(c1, plotTools, plotToolsInterpol, nameappendix, inclEdges, METCut,
							bandAroundExpLimit, false);
	//dijet
	DrawPlot_SigAccAndCont(c1, plotTools, plotToolsInterpol, nameappendix, inclEdges, METCut,
							bandAroundExpLimit, true);

	//XS limit, signal cont.  subtracted - 3jet
	DrawPlot_XSLimitSigCont(c1, plotTools, plotToolsInterpol, nameappendix, inclEdges, METCut,
							bandAroundExpLimit, false, false);
	//XS limit, signal cont.  subtracted - 2jet
	DrawPlot_XSLimitSigCont(c1, plotTools, plotToolsInterpol, nameappendix, inclEdges, METCut,
							bandAroundExpLimit, true, false);
	//XS limit, signal cont.  subtracted - Best
	DrawPlot_XSLimitSigCont(c1, plotTools, plotToolsInterpol, nameappendix, inclEdges, METCut,
							bandAroundExpLimit, false, true);

	//Test Contour
	DrawPlot_TestContour(c1, plotTools, plotToolsInterpol, nameappendix, inclEdges, METCut,
							bandAroundExpLimit);
	//Draw Limits - 3jet
	ExclusionCurves trijetExcl = DrawPlot_ExclusionCurves(c1, plotTools, plotToolsInterpol,
															nameappendix, inclEdges, METCut,
															bandAroundExpLimit, false, false);
	//Draw Limits - 2jet
	ExclusionCurves dijetExcl = DrawPlot_ExclusionCurves(c1, plotTools, plotToolsInterpol,
															nameappendix, inclEdges, METCut,
															bandAroundExpLimit, true, false);
	//Draw Limits - Best
	ExclusionCurves bestExcl = DrawPlot_ExclusionCurves(c1, plotTools, plotToolsInterpol,
														nameappendix, inclEdges, METCut,
														bandAroundExpLimit, false, true);

	DrawPlot_BestLimit20r3jet(c1, plotTools, plotToolsInterpol, nameappendix, inclEdges, METCut,
								bandAroundExpLimit);

	ExclCurvesContainer exclCurvesCont;
	exclCurvesCont.best = bestExcl;
	exclCurvesCont.jet3 = trijetExcl;
	exclCurvesCont.jet2 = dijetExcl;
	exclCurvesCont.lumi = plotTools->SingleValue(Luminosity);
	return exclCurvesCont;

}
void DrawNeutrMassComp(double lumi, TCanvas * c1, ExclusionCurves m1Excl, ExclusionCurves m2Excl,
	string prefix) {
	//compare Neutralino masses
	TGraph * m1Exp = m1Excl.exp;
	TGraph * m1Obs = m1Excl.obs;
	TGraph * m1OneSigBand = m1Excl.onesigband;

	TGraph * m2Exp = m2Excl.exp;
	TGraph * m2Obs = m2Excl.obs;
	TGraph * m2OneSigBand = m2Excl.onesigband;

	c1->SetLogz(1);
	c1->SetRightMargin(0.08);
	TH2F *hexclCompNeutr =
		new TH2F("hexclNeutrComp",
					";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); 95% CL Exclusion", 92, 160,
					2000, 92, 160, 2000);

	hexclCompNeutr->Draw("COLZ");

	m1OneSigBand->SetFillStyle(3001);
	m1OneSigBand->SetFillColor(kOrange);
	m1OneSigBand->Draw("f");
	m1Obs->SetLineWidth(2);
	m1Exp->SetLineWidth(2);
	m1Obs->Draw("l");
	m1Exp->Draw("l");

	m2OneSigBand->SetFillStyle(3001);
	m2OneSigBand->SetFillColor(kYellow);
	m2OneSigBand->Draw("f");
	m2Obs->SetLineWidth(2);
	m2Exp->SetLineWidth(2);
	m2Obs->Draw("l");
	m2Exp->Draw("l");

	TH1F * dummy = new TH1F("dummy", "dummy", 5, 0, 5);
	dummy->SetLineColor(0);

	TLegend * leg2 = new TLegend(0.41, 0.41, 0.93, 0.88);
	leg2->SetBorderSize(0);
	leg2->SetFillColor(0);
	leg2->SetFillStyle(4000);
	leg2->SetTextSize(0.025);
	//leg2->AddEntry(dummy, "m_{#tilde{#Chi}^{0}} = 375 GeV", "l");
	leg2->AddEntry(m1Obs, "Observed, m_{#tilde{#Chi}^{0}} = 375 GeV", "l");
	leg2->AddEntry(m1Exp, "Expected, m_{#tilde{#Chi}^{0}} = 375 GeV", "l");
	leg2->AddEntry(m1OneSigBand, "#pm 1#sigma, m_{#tilde{#Chi}^{0}} = 375 GeV", "f");

	//leg2->AddEntry(dummy, "m_{#tilde{#Chi}^{0}} = 150 GeV ", "l");
	leg2->AddEntry(m2Obs, "Observed, m_{#tilde{#Chi}^{0}} = 150 GeV", "l");
	leg2->AddEntry(m2Exp, "Expected, m_{#tilde{#Chi}^{0}} = 150 GeV", "l");
	leg2->AddEntry(m2OneSigBand, "#pm 1#sigma, m_{#tilde{#Chi}^{0}} = 150 GeV", "f");

	//leg->AddEntry(CLsExpNoSNLO,   "Expected, no-signal hyp., NLO","l");
	leg2->Draw();
	gPad->RedrawAxis();

	drawCmsPrel(lumi, 200);
	string nameNeutrCompPlot = "results/" + prefix + "COMPNeutr_3jet";
	c1->SaveAs((nameNeutrCompPlot + ".pdf").c_str());
	c1->SaveAs((nameNeutrCompPlot + ".png").c_str());
}
void DrawMetCutComp(double lumi, TCanvas * c1, TGraph * low, TGraph * high, TGraph * med,
	string prefix, bool dijet = false, bool bestLimit = false) {
	//compare MET cuts
	c1->SetLogz(1);
	c1->SetRightMargin(0.08);
	string jetapp = "3jet";
	int noJet = 3;
	if (dijet) {
		jetapp = "2jet";
		noJet = 2;
	}
	if (bestLimit)
		jetapp = "2OR3jet";
	TH2F *hexclComp =
		new TH2F("hexcl", ";m_{squark} (GeV/c^{2}); m_{gluino} (GeV/c^{2}); 95% CL Exclusion", 80,
					400, 2000, 80, 400, 2000);
	hexclComp->Draw("COLZ");
	med->SetLineColor(2);
	high->SetLineColor(2);
	low->SetLineColor(2);
	med->SetLineStyle(1);
	high->SetLineStyle(2);
	low->SetLineStyle(3);
	med->Draw("");
	high->Draw("");
	low->Draw("");
	TLegend * leg = new TLegend(0.41, 0.41, 0.93, 0.9);
	leg->SetBorderSize(0);
	leg->SetFillColor(0);
	leg->SetFillStyle(4000);
	leg->SetTextSize(0.025);

	leg->AddEntry(low, "Expected MET>100", "l");
	leg->AddEntry(med, "Expected, MET>200", "l");
	leg->AddEntry(high, "Expected, MET>350", "l");

	leg->Draw();

	drawCmsPrel(lumi, 0, false, noJet);
	string nameXsPlot = "results/" + prefix + "COMPMetSelection_" + jetapp;
	c1->SaveAs((nameXsPlot + ".pdf").c_str());
	c1->SaveAs((nameXsPlot + ".png").c_str());
}
ExclusionCurves makePlotsChi1(double met, string filename, bool pdfuncertinclInAcc, string prefix,
	TCanvas * c1) {
	///////////////NEUTRALINO LIMITS///////////////////////////////////////////////////
	//Get limits from signal scan ---------------------------------------------------
	TheLimits * genpointsChi1_sq2500 = new TheLimits();
	genpointsChi1_sq2500->Fill("limits_GMSB/" + filename);

	//	genpointsChi1->FillEmptyPointsByInterpolationInMchi1Mgl();
	//	genpointsChi1->FillEmptyPointsByInterpolationInMchi1Mgl();
	//Make grid in Msquark, Mgluino finer by factor of 4 by linear interpolation
	genpointsChi1_sq2500->ExpandGrid<SusyScan> (Mchi1, Mgluino);
	genpointsChi1_sq2500->ExpandGrid<SusyScan> (Mchi1, Mgluino);

	// New 'pseudo' points are added, therefore the binning of all plots has to be made finer by a factor
	// of 2 in x and y for each "ExpandGrid
	//the plotting ----------------------------------------------------------------------
	//plotting helper functions
	PlotTools<SusyScan> *plotToolsChi1_sq2500 =
		new PlotTools<SusyScan> (genpointsChi1_sq2500->GetScan());

	//the histograms ---------------------------------------------------------------------
	DrawPlotsChi1(c1, plotToolsChi1_sq2500, plotToolsChi1_sq2500, prefix);
}
ExclCurvesContainer makePlotsSqGl(double met, string filename, bool pdfuncertinclInAcc,
	string prefix, TCanvas * c1) {
	//Get limits from signal scan ---------------------------------------------------
	TheLimits * genpointsInterpol = new TheLimits();

	genpointsInterpol->Fill("limits_GMSB/" + filename);
	TheLimits * genpoints = new TheLimits();

	genpoints->Fill("limits_GMSB/" + filename);

	//Make grid in Msquark, Mgluino finer by factor of 4 by linear interpolation
	genpointsInterpol->ExpandGrid<SusyScan> (Msquark, Mgluino);
	genpointsInterpol->ExpandGrid<SusyScan> (Msquark, Mgluino);

	// New 'pseudo' points are added, therefore the binning of all plots has to be made finer by a factor
	// of 2 in x and y for each "ExpandGrid
	//the plotting ----------------------------------------------------------------------
	//plotting helper functions
	PlotTools<SusyScan> *plotTools = new PlotTools<SusyScan> (genpoints->GetScan());

	PlotTools<SusyScan> *plotToolsInterpol = new PlotTools<SusyScan> (genpointsInterpol->GetScan());

	//the histograms ---------------------------------------------------------------------
	ExclCurvesContainer excl;
	if (pdfuncertinclInAcc) {
		std::cout << "INCL!!:" << std::endl;
		excl = DrawPlots(c1, plotTools, plotToolsInterpol, prefix, true, met, true);

	} else {
		excl = DrawPlots(c1, plotTools, plotToolsInterpol, prefix, true, met);
	}
	return excl;
}
int plot(int argc, char** argv) {

	util::StyleSettings::paperNoTitle();
	gStyle->SetPadBottomMargin(0.18);

	TCanvas * c1 = new TCanvas("c1", "c1", 600, 600);
	c1->cd();

	bool pdfuncertinclInAcc = false;
	bool doBinoLimits = true;
	bool doWinoLimits = true;
	bool doNeutralinoLimits = true;

	if (doBinoLimits) {
		//Bino, Met200, Neutr375
		ExclCurvesContainer binoExcl;
		if (pdfuncertinclInAcc) {
			binoExcl = makePlotsSqGl(200, "resultfilesbinoPDFACC.txt", pdfuncertinclInAcc, "Bino",
										c1);
		} else
			binoExcl = makePlotsSqGl(200, "resultfilesbino200_Sep6.txt", pdfuncertinclInAcc,
										"Bino", c1);
		TGraph * bino = binoExcl.jet3.exp;
		//Bino, Met100, Neutr375
		ExclCurvesContainer binoLowExcl = makePlotsSqGl(100, "resultfilesbino100_Sep6.txt",
														pdfuncertinclInAcc, "BinoLow", c1);
		TGraph * binoLow = binoLowExcl.jet3.exp;
		//Bino, Met350, Neutr375
		ExclCurvesContainer binoHighExcl = makePlotsSqGl(350, "resultfilesbino350_Sep6.txt",
															pdfuncertinclInAcc, "BinoHigh", c1);
		TGraph * binoHigh = binoHighExcl.jet3.exp;
		//Bino, Met Comparison, Neutr375
		DrawMetCutComp(1714, c1, binoLow, binoHigh, bino, "Bino");

		//Bino, Met200, Neutr150
		ExclCurvesContainer bino150Excl;
		bino150Excl = makePlotsSqGl(200, "resultfilesbino200Neutr150_Sep6.txt", pdfuncertinclInAcc,
									"Bino150", c1);
		//Bino, Neutr mass comparison
		DrawNeutrMassComp(binoExcl.lumi, c1, binoExcl.jet3, bino150Excl.jet3, "Bino");

	}
	if (doWinoLimits) {
		//Wino, Met200, Neutr375
		ExclCurvesContainer winoExcl;
		if (pdfuncertinclInAcc) {
			winoExcl = makePlotsSqGl(200, "resultfileswinoPDFACC.txt", pdfuncertinclInAcc, "Wino",
										c1);
		} else
			winoExcl = makePlotsSqGl(200, "resultfileswino200_Sep15.txt", pdfuncertinclInAcc,
										"Wino", c1);
		TGraph * wino = winoExcl.jet3.exp;
		//Wino, Met100, Neutr375
		ExclCurvesContainer winoLowExcl = makePlotsSqGl(100, "resultfileswino100_Sep6.txt",
														pdfuncertinclInAcc, "WinoLow", c1);
		TGraph * winoLow = winoLowExcl.jet3.exp;
		//Wino, Met350, Neutr375
		ExclCurvesContainer winoHighExcl = makePlotsSqGl(350, "resultfileswino350_Sep6.txt",
															pdfuncertinclInAcc, "WinoHigh", c1);
		TGraph * winoHigh = winoHighExcl.jet3.exp;
		//Wino, Met Comparison, Neutr375
		DrawMetCutComp(winoExcl.lumi, c1, winoLow, winoHigh, wino, "Wino");
		DrawMetCutComp(winoExcl.lumi, c1, winoLowExcl.jet2.exp, winoHighExcl.jet2.exp,
						winoExcl.jet2.exp, "Wino", true);

		//Wino, Met200, Neutr150
		ExclCurvesContainer wino150Excl;
		wino150Excl = makePlotsSqGl(200, "resultfileswino200Neutr150_Sep6.txt", pdfuncertinclInAcc,
									"Wino150", c1);
		//Wino, Met100, Neutr150
		ExclCurvesContainer wino150LowExcl = makePlotsSqGl(100,
															"resultfilesbino100Neutr150_Sep6.txt",
															pdfuncertinclInAcc, "Wino150Low", c1);
		//Wino, Neutr mass comparison
		DrawNeutrMassComp(1714, c1, winoExcl.jet3, wino150Excl.jet3, "Wino");

	}
	if (doNeutralinoLimits) {

		makePlotsChi1(200, "resultfilesBino200NeutrScanSquark2500_Sep9.txt", pdfuncertinclInAcc,
						"Chi1_SQ2500", c1);
	}
}

int main(int argc, char** argv) {
	return plot(argc, argv);
}
