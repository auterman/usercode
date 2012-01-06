#include "plot.h"

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
#include <sstream>
#include <cmath>
#include <stdio.h>


namespace filenames {
//ATTENTION: do not forget to add new filenames in main function!!

string file_wino3j = "2012-01-06-00-07-GMSBWino375Neutr";
string file_wino2j = "2012-01-06-00-08-GMSBWino375Neutr2j";

string file_bino3j = "2012-01-06-00-03-GMSBBino375Neutr";
string file_bino2j = "2012-01-06-00-06-GMSBBino375Neutr2j";

string file_binoNeutr3j = "2012-01-06-00-09-GMSB_SquarkGluino_vs_Neutralino";
string file_binoNeutr2j = "2012-01-06-00-10-GMSB_SquarkGluino_vs_Neutralino2j";

string file_wino3j_MergedBins = "2011-11-27-16-33-GMSBWino375NeutrMerged";

string file_wino3j_SingleBins = "2011-12-19-21-50-GMSBWino375NeutrSingleChannels";
string file_bino3j_SingleBins = "2011-12-19-21-49-GMSBBino375NeutrSingleChannels";
string file_binoNeutr3j_SingleBins = "2011-12-19-21-51-GMSB_SquarkGluino_vs_NeutralinoSingleChannels";
string file_wino2j_SingleBins = "2011-12-19-21-50-GMSBWino375NeutrSingleChannels2j";
string file_bino2j_SingleBins = "2011-12-19-21-49-GMSBBino375NeutrSingleChannels2j";
string file_binoNeutr2j_SingleBins = "2011-12-19-21-51-GMSB_SquarkGluino_vs_NeutralinoSingleChannels2j";

string file_bino3j_DemoPoint = "2011-12-19-21-53-DemoPoint";



//2012-01-06-00-03-GMSBBino375Neutr/                   2012-01-06-00-07-GMSBWino375Neutr/                   2012-01-06-00-09-GMSB_SquarkGluino_vs_Neutralino/
//2012-01-06-00-06-GMSBBino375Neutr2j/                 2012-01-06-00-08-GMSBWino375Neutr2j/                 2012-01-06-00-10-GMSB_SquarkGluino_vs_Neutralino2j/





//Remove files with crashed expected/observed limits?
//find . | grep -l "CLs observed asymptotic = $" 2011-11-2*/* | while read line; do echo rm $line; done
//Remove files where no CLs observed was written in file
//find . | grep -L "CLs observed = " 2011-12-14*/*txt.result.txt | while read line; do rm $line; done


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
namespace styles {
PlotStyles ret;
PlotStyles retWino2j;
PlotStyles retWino3j;
PlotStyles retWinoMerged3j;
PlotStyles retWinoBestJet;

PlotStyles retBino2j;
PlotStyles retBino3j;
PlotStyles retBinoBestJet;

PlotStyles retBinoNeutr_Gluino3j;
PlotStyles retBinoNeutr_Gluino2j;
PlotStyles retBinoNeutr_GluinoBestJet;
PlotStyles retBinoNeutr_Squark;

void setStyles(PlotTools<SusyScan>*& PlotTool) {
	ret.flag = "";
	ret.lumi = PlotTool->SingleValue(Luminosity);
	ret.METCut = "100";
	ret.isBestJet = false;
	ret.isBestJetFile1 = "";
	ret.isBestJetFile2 = "";
	ret.noJet = -1;
	ret.zaxistitleoffset = 1.5;
	ret.neutralinotype = "";
	ret.neutralinomass = "375";
	ret.flagForComp = "";
	ret.drawObsNLOXSErrorBand = true;
	ret.drawGluinoNLSPExclusionRegion = false;

	retWino2j = ret;
	retWino2j.flag = "Wino2j";
	retWino2j.noJet = 2;
	retWino2j.neutralinotype = "wino";

	retWino3j = ret;
	retWino3j.flag = "Wino3j";
	retWino3j.noJet = 3;
	retWino3j.neutralinotype = "wino";

	retWinoMerged3j = ret;
	retWinoMerged3j.flag = "Wino3jMergedBin";
	retWinoMerged3j.noJet = 3;
	retWinoMerged3j.METCut = "200";
	retWinoMerged3j.neutralinotype = "wino";

	retWinoBestJet = ret;
	retWinoBestJet.flag = "WinoBestJet";
	retWinoBestJet.noJet = 0;
	retWinoBestJet.METCut = "100";
	retWinoBestJet.isBestJet = true;
	retWinoBestJet.isBestJetFile1 = "2 jet";
	retWinoBestJet.isBestJetFile2 = "3 jet";
	retWinoBestJet.neutralinotype = "wino";
	retWinoBestJet.flagForComp = "Wino_2_vs_3_jet";

	retBino2j = ret;
	retBino2j.flag = "Bino2j";
	retBino2j.noJet = 2;
	retBino2j.neutralinotype = "bino";

	retBino3j = ret;
	retBino3j.flag = "Bino3j";
	retBino3j.noJet = 3;
	retBino3j.neutralinotype = "bino";

	retBinoBestJet = retWinoBestJet;
	retBinoBestJet.flag = "BinoBestJet";
	retBinoBestJet.neutralinotype = "bino";
	retBinoBestJet.flagForComp = "Bino_2_vs_3_jet";

	retBinoNeutr_Gluino3j = ret;
	retBinoNeutr_Gluino3j.flag = "BinoNeutr_gl_3j";
	retBinoNeutr_Gluino3j.noJet = 3;
	retBinoNeutr_Gluino3j.neutralinotype = "bino";
	retBinoNeutr_Gluino3j.drawGluinoNLSPExclusionRegion = true;

	retBinoNeutr_Gluino2j = retBinoNeutr_Gluino3j;
	retBinoNeutr_Gluino2j.flag = "BinoNeutr_gl_2j";
	retBinoNeutr_Gluino2j.noJet = 2;

	retBinoNeutr_GluinoBestJet = retWinoBestJet;
	retBinoNeutr_GluinoBestJet.flag = "BinoNeutrBestJet";
	retBinoNeutr_GluinoBestJet.neutralinotype = "bino";
	retBinoNeutr_GluinoBestJet.flagForComp = "BinoNeutr_2_vs_3_jet";
	retBinoNeutr_GluinoBestJet.drawGluinoNLSPExclusionRegion = true;

	retBinoNeutr_Squark = retBinoNeutr_Gluino3j;
	retBinoNeutr_Squark.flag = "BinoNeutr_sq_3j";
}

}
using namespace styles;

//redo filelists
//ls 2011-11-25-15-00-GMSBBino375Neutr/* > 2011-11-25-15-00-GMSBBino375Neutr/filelist.txt
void redoFileList(vector<string> filenames) {
	for (int i = 0; i < (int) filenames.size(); ++i) {

		std::system(("ls " + filenames.at(i) + "/*.txt > " + filenames.at(i) + "/filelist.txt").c_str());

		std::system(("ls " + filenames.at(i) + "/*_bin0*.txt > " + filenames.at(i) + "/filelist_ch0.txt").c_str());
		std::system(("ls " + filenames.at(i) + "/*_bin1*.txt > " + filenames.at(i) + "/filelist_ch1.txt").c_str());
		std::system(("ls " + filenames.at(i) + "/*_bin2*.txt > " + filenames.at(i) + "/filelist_ch2.txt").c_str());
		std::system(("ls " + filenames.at(i) + "/*_bin3*.txt > " + filenames.at(i) + "/filelist_ch3.txt").c_str());
		std::system(("ls " + filenames.at(i) + "/*_bin4*.txt > " + filenames.at(i) + "/filelist_ch4.txt").c_str());
		std::system(("ls " + filenames.at(i) + "/*_bin5*.txt > " + filenames.at(i) + "/filelist_ch5.txt").c_str());
		std::system(("ls " + filenames.at(i) + "/*_bin6*.txt > " + filenames.at(i) + "/filelist_ch6.txt").c_str());
		std::system(("ls " + filenames.at(i) + "/*_bin7*.txt > " + filenames.at(i) + "/filelist_ch7.txt").c_str());
		std::system(("ls " + filenames.at(i) + "/*_bin8*.txt > " + filenames.at(i) + "/filelist_ch8.txt").c_str());
	}
}

struct ExclusionCurves {
	std::string Name;
	TGraph * obs;
	TGraph * obs_asym;
	TGraph * exp_asym;
	TGraph * exp;
	TGraph * onesigband;
	TGraph * onesigbandNLOXS;
	TGraph * p1sigma;
	TGraph * p1sigmaNLOCX;

};
void setExclusionStyles(ExclusionCurves * excl, bool isSecondCurve) {
	excl->exp->SetLineWidth(2);
	excl->exp->SetLineStyle(2);
	excl->obs->SetLineWidth(2);
	excl->onesigband->SetLineWidth(0);
	excl->onesigbandNLOXS->SetLineWidth(0);
	if (!isSecondCurve) {
		excl->onesigband->SetLineColor(42);
		excl->onesigbandNLOXS->SetLineColor(42);
		excl->onesigband->SetFillColor(42);
		excl->onesigbandNLOXS->SetFillColor(42);
	} else {
		excl->onesigband->SetFillColor(kRed - 5);
		excl->onesigband->SetFillStyle(3004);
		excl->onesigband->SetLineColor(kRed - 5);
		excl->onesigbandNLOXS->SetFillColor(kRed - 5);
		excl->onesigbandNLOXS->SetFillStyle(3004);
		excl->onesigbandNLOXS->SetLineColor(kRed - 5);
		excl->exp->SetLineColor(kRed);
		excl->obs->SetLineColor(kRed);
	}
}
void SetZRange(TH2F * h) {
	//cout<<"Find optimal z range..."<<endl;
	double maxValue = 0, minValue = 0;

	maxValue = h->GetBinContent(h->GetMaximumBin());
	minValue = h->GetBinContent(h->GetMinimumBin());
	if (minValue == 0) {
		minValue = 9999;
		for (int x = 1; x <= h->GetNbinsX() + 1; x++) {
			for (int y = 1; y <= h->GetNbinsY() + 1; y++) {
				//cout<<"x:"<<x<<endl;
				//cout<<"y:"<<y<<endl;
				double bincontent = h->GetBinContent(x, y);
				//cout<<"bincontent:"<<bincontent<<endl;

				if (minValue > bincontent && bincontent > 0) {

					minValue = bincontent;
				}
			}
		}

	}
	minValue = minValue * 0.9;
	maxValue = maxValue * 1.1;
	//	cout<<"maximum value:"<<maxValue<<endl;
	//	cout<<"minimum value:"<<minValue<<endl;
	h->GetZaxis()->SetRangeUser(minValue, maxValue);

}
template<class T>
void CreateSamplePointTexFile(PlotTools<SusyScan> *PlotTool, double(*x)(const T*), double(*y)(const T*), double xValue, double yValue, string filename, string name, bool onlyAsympAvailable = false) {
	std::cout << "==========SAMPLE POINT TEX DEFS===============" << endl;
	ofstream ofile;
	ofile.open(filename.c_str(), ios::out | ios::app);
	ofile << "\\def\\" << name << "squark{" << xValue  << "}" << endl;
	ofile << "\\def\\" << name << "gluino{" << yValue << "}" << endl;
	ofile << "\\def\\" << name << "neutralino{" <<  PlotTool->SingleValue(Mchi1, x, y, xValue, yValue) << "}" << endl;
	if (!onlyAsympAvailable) {
		ofile << "\\def\\" << name << "NLOXS{" << PlotTool->roundDouble(PlotTool->SingleValue(NLOXsection, x, y, xValue, yValue),2) << "}" << endl;
		ofile << "\\def\\" << name << "NLOXSUp{" << PlotTool->roundDouble(PlotTool->SingleValue(NLOXSUncertaintyUpAbs, x, y, xValue, yValue),2) << "}" << endl;
		ofile << "\\def\\" << name << "NLOXSDn{" << PlotTool->roundDouble(PlotTool->SingleValue(NLOXSUncertaintyDownAbs, x, y, xValue, yValue),2) << "}" << endl;
		ofile << "\\def\\" << name << "PDFXSErr{" << PlotTool->roundDouble(PlotTool->SingleValue(PDFXSUncertaintyAbs, x, y, xValue, yValue),2) << "}" << endl;
		ofile << "\\def\\" << name << "ObsXS{" << PlotTool->roundDouble( PlotTool->SingleValue(ObsXsecLimit, x, y, xValue, yValue),2) << "}" << endl;
		ofile << "\\def\\" << name << "ExpXS{" <<  PlotTool->roundDouble(PlotTool->SingleValue(ExpXsecLimit, x, y, xValue, yValue),2) << "}" << endl;
		ofile << "\\def\\" << name << "NLOXSM{" << PlotTool->roundDouble(PlotTool->SingleValue(NLOXsectionM1, x, y, xValue, yValue),2) << "}" << endl;
		ofile << "\\def\\" << name << "NLOXSP{" << PlotTool->roundDouble(PlotTool->SingleValue(NLOXsectionP1, x, y, xValue, yValue),2) << "}" << endl;

		ofile << "\\def\\" << name << "lumi{" << PlotTool->roundDouble(0.001 * PlotTool->SingleValue(Luminosity, x, y, xValue, yValue), 2) << "}" << endl;
		ofile << "\\def\\" << name << "lumiErr{" << PlotTool->roundDouble(0.001 * PlotTool->SingleValue(LuminosityErr, x, y, xValue, yValue), 2) << "}" << endl;
	} else {
		ofile << "\\def\\" << name << "ObsXSAsym{" << PlotTool->SingleValue(ObsXsecLimitAsym, x, y, xValue, yValue) << "}" << endl;
		ofile << "\\def\\" << name << "ExpXSAsym{" << PlotTool->SingleValue(ExpXsecLimitAsym, x, y, xValue, yValue) << "}" << endl;
	}
	ofile << "\\def\\" << name << "signalacceptance{" << PlotTool->roundDouble(PlotTool->SingleValue(SignalAcceptanceAbs, x, y, xValue, yValue),2) << "}" << endl;

	ofile << "\\def\\" << name << "signalacceptanceuncertainty{" <<PlotTool->roundDouble(PlotTool->SingleValue(SignalAcceptanceUncert, x, y, xValue, yValue),0) << "}" << endl;

	ofile << "\\def\\" << name << "expSignalEvts{" << PlotTool->roundDouble(PlotTool->SingleValue(SignalExpectation, x, y, xValue, yValue), 0) << "}" << endl;
	ofile << "\\def\\" << name << "expSignalEvtsErr{" << PlotTool->roundDouble(PlotTool->SingleValue(SignalExpectationErr, x, y, xValue, yValue), 0) << "}" << endl;
	ofile << "\\def\\" << name << "expBkgContEvts{" << PlotTool->roundDouble(PlotTool->SingleValue(SignalContaminationAbs, x, y, xValue, yValue), 0) << "}" << endl;

	ofile.close();
}
template<class T>
void CreateResultPointTexFile(TGraph * limit, PlotTools<SusyScan> *PlotTool, double(*x)(const T*), double(*y)(const T*), double xValue, string name, string filename) {
	std::cout << "==========CREATE RESULT POINT TEX DEFS" << name << "===============" << endl;
	ofstream ofile;
	ofile.open(filename.c_str(), ios::out | ios::app);
	ofile << "\\def\\res" << name << "xValue{" << xValue << "}" << endl;
	//Find corresponding y Value - where is the observed limit for given x Value?
	double yValue = PlotTool->LimitValue(x, y, limit, xValue);
	ofile << "\\def\\res" << name << "yValue{" << yValue << "}" << endl;
	ofile << "\\def\\res" << name << "obsXSLimit{" << PlotTool->roundDouble(PlotTool->SingleValue(ObsXsecLimit, x, y, xValue, yValue),2) << "}" << endl;
	ofile << "\\def\\res" << name << "acceptance{" << PlotTool->roundDouble(PlotTool->SingleValue(SignalAcceptance, x, y, xValue, yValue), 0) << "}" << endl;
	ofile << "\\def\\res" << name << "expSignalEvts{" << PlotTool->roundDouble(PlotTool->SingleValue(SignalExpectation, x, y, xValue, yValue), 0) << "}" << endl;

	std::pair<double, double> minLimits = PlotTool->MinLimitValues(x, y, limit);
	std::pair<double, double> maxLimits = PlotTool->MaxLimitValues(x, y, limit);
	ofile << "\\def\\res" << name << "minY{" << minLimits.second << "}" << endl;
	ofile << "\\def\\res" << name << "minX{" << minLimits.first << "}" << endl;
	ofile << "\\def\\res" << name << "maxX{" << maxLimits.first << "}" << endl;
	ofile.close();
}

template<class T>
void DrawStandardPlots(PlotTools<SusyScan> *PlotTool, PlotStyles style, double(*x)(const T*), double(*y)(const T*), TH1*h) {
	std::system(("mkdir results/" + style.flag).c_str());

	//	gStyle->SetPadTopMargin(0.1);
	//	gStyle->SetPadBottomMargin(0.15);
	gStyle->SetPadRightMargin(0.2);
	gStyle->SetPadLeftMargin(0.2);
	gStyle->SetTitleOffset(1.3, "xyz");
	gStyle->SetTitleOffset(1.9, "y");
	gStyle->SetNdivisions(505);
	gStyle->SetTitleFont(43, "xyz");
	gStyle->SetTitleSize(32, "xyz");

	c1->UseCurrentStyle();
	h->UseCurrentStyle();

	//Xsection
	{
		TH2F *hxsec = (TH2F*) h->Clone();
		hxsec->GetZaxis()->SetTitle("NLO cross section [pb]");
		c1->SetLogz(1);
		PlotTool->Area(hxsec, x, y, NLOXsection);
		SetZRange(hxsec);
		cout << "setrange done" << endl;
		hxsec->GetZaxis()->SetTitleOffset(1.5);
		hxsec->Draw("colz");
		cout << "histo drawn" << endl;
		if (style.drawGluinoNLSPExclusionRegion)
			DrawNeutrNNLSP();
		cout << "now cms pre" << endl;
		drawCmsPrel(style.lumi, style.METCut, style.noJet, style.isBestJet, "", false);
		cout << "done" << endl;
		string nameXsPlot = "results/" + style.flag + "/Xsection";
		c1->SaveAs((nameXsPlot + ".pdf").c_str());
		//c1->SaveAs((nameXsPlot + ".root").c_str());
		c1->SaveAs((nameXsPlot + ".png").c_str());
	}
	//Acceptance
	{
		TH2F *hxsec = (TH2F*) h->Clone("bla");
		hxsec->GetZaxis()->SetTitle("Acceptance [%]");
		c1->SetLogz(0);
		PlotTool->Area(hxsec, x, y, SignalAcceptance);
		SetZRange(hxsec);
		hxsec->GetZaxis()->SetTitleOffset(1.5);
		hxsec->Draw("colz");
		if (style.drawGluinoNLSPExclusionRegion)
			DrawNeutrNNLSP();
		drawCmsPrel(style.lumi, style.METCut, style.noJet, style.isBestJet);
		string nameXsPlot = "results/" + style.flag + "/Acceptance";
		c1->SaveAs((nameXsPlot + ".pdf").c_str());
		c1->SaveAs((nameXsPlot + ".png").c_str());
		delete hxsec;
	}
	//SignalContamination
	{
		TH2F *hxsec = (TH2F*) h->Clone("bla");
		hxsec->GetZaxis()->SetTitle("Signal Contamination [%]");
		c1->SetLogz(0);
		PlotTool->Area(hxsec, x, y, SignalContamination);
		SetZRange(hxsec);
		hxsec->GetZaxis()->SetTitleOffset(1.5);
		hxsec->Draw("colz");
		if (style.drawGluinoNLSPExclusionRegion)
			DrawNeutrNNLSP();
		drawCmsPrel(style.lumi, style.METCut, style.noJet, style.isBestJet);
		string nameXsPlot = "results/" + style.flag + "/SigCont";
		c1->SaveAs((nameXsPlot + ".pdf").c_str());
		c1->SaveAs((nameXsPlot + ".png").c_str());
		delete hxsec;
	}
	//SignalExpectation
	{
		TH2F *hxsec = (TH2F*) h->Clone("bla");
		hxsec->GetZaxis()->SetTitle("Expected Signal Events");
		c1->SetLogz(1);
		PlotTool->Area(hxsec, x, y, SignalExpectation);
		SetZRange(hxsec);
		hxsec->GetZaxis()->SetTitleOffset(1.5);
		hxsec->Draw("colz");
		if (style.drawGluinoNLSPExclusionRegion)
			DrawNeutrNNLSP();
		drawCmsPrel(style.lumi, style.METCut, style.noJet, style.isBestJet);
		string nameXsPlot = "results/" + style.flag + "/ExpectedSignal";
		c1->SaveAs((nameXsPlot + ".pdf").c_str());
		c1->SaveAs((nameXsPlot + ".png").c_str());
		delete hxsec;
	}
	//PDF Acc uncertainty
	{
		TH2F *hxsec = (TH2F*) h->Clone("bla");
		hxsec->GetZaxis()->SetTitle("PDF uncertainty on acceptance[%]");
		c1->SetLogz(0);
		PlotTool->Area(hxsec, x, y, PDFAccUncertainty);
		SetZRange(hxsec);
		hxsec->GetZaxis()->SetTitleOffset(1.5);
		hxsec->Draw("colz");
		if (style.drawGluinoNLSPExclusionRegion)
			DrawNeutrNNLSP();
		drawCmsPrel(style.lumi, style.METCut, style.noJet, style.isBestJet, "", false);
		string nameXsPlot = "results/" + style.flag + "/PDFAccUncert";
		c1->SaveAs((nameXsPlot + ".pdf").c_str());
		c1->SaveAs((nameXsPlot + ".png").c_str());
		delete hxsec;
	}
	//PDF XS uncertainty
	{
		TH2F *hxsec = (TH2F*) h->Clone("bla");
		hxsec->GetZaxis()->SetTitle("PDF XS uncertainty [%]");
		c1->SetLogz(0);
		PlotTool->Area(hxsec, x, y, PDFXSUncertainty);
		SetZRange(hxsec);
		hxsec->GetZaxis()->SetTitleOffset(1.5);
		hxsec->Draw("colz");
		if (style.drawGluinoNLSPExclusionRegion)
			DrawNeutrNNLSP();
		drawCmsPrel(style.lumi, style.METCut, style.noJet, style.isBestJet, "", false);
		string nameXsPlot = "results/" + style.flag + "/PDFXSUncert";
		c1->SaveAs((nameXsPlot + ".pdf").c_str());
		c1->SaveAs((nameXsPlot + ".png").c_str());
		delete hxsec;
	}
	//NLO XS uncertainty
	{
		TH2F *hxsec = (TH2F*) h->Clone("bla");
		hxsec->GetZaxis()->SetTitle("NLO XS uncertainty [%]");
		c1->SetLogz(0);
		PlotTool->Area(hxsec, x, y, NLOXSUncertaintyUp);
		SetZRange(hxsec);
		hxsec->GetZaxis()->SetTitleOffset(1.5);
		hxsec->Draw("colz");
		if (style.drawGluinoNLSPExclusionRegion)
			DrawNeutrNNLSP();
		drawCmsPrel(style.lumi, style.METCut, style.noJet, style.isBestJet, "", false);
		string nameXsPlot = "results/" + style.flag + "/NLOXSUncert";
		c1->SaveAs((nameXsPlot + ".pdf").c_str());
		c1->SaveAs((nameXsPlot + ".png").c_str());
		delete hxsec;
	}
	//Observed X-section limit
	{
		TH2F *hxsec = (TH2F*) h->Clone();
		hxsec->GetZaxis()->SetTitle("(Observed) 95% CL Upper Limit [pb]");
		c1->SetLogz(1);
		PlotTool->Area(hxsec, x, y, ObsXsecLimit);
		TH2F *hacc = (TH2F*) h->Clone();
		PlotTool->Area(hacc, x, y, SignalAcceptance);
		fillXSLimitAboveForInvalidResultPoints(hxsec,hacc);
		SetZRange(hxsec);
		hxsec->GetZaxis()->SetTitleOffset(1.5);
		hxsec->Draw("colz");
		if (style.drawGluinoNLSPExclusionRegion)
			DrawNeutrNNLSP();
		drawCmsPrel(style.lumi, style.METCut, style.noJet, style.isBestJet);
		string nameXsPlot = "results/" + style.flag + "/ObsXsecLimit";
		c1->SaveAs((nameXsPlot + ".pdf").c_str());
		c1->SaveAs((nameXsPlot + ".png").c_str());
		c1->SaveAs((nameXsPlot + ".root").c_str());
	}
	//Expected X-section limit
	{
		TH2F *hxsec = (TH2F*) h->Clone();
		hxsec->GetZaxis()->SetTitle("(Expected) 95% CL Upper Limit [pb]");
		c1->SetLogz(1);
		PlotTool->Area(hxsec, x, y, ExpXsecLimit);
		SetZRange(hxsec);
		hxsec->GetZaxis()->SetTitleOffset(1.5);
		hxsec->Draw("colz");
		if (style.drawGluinoNLSPExclusionRegion)
			DrawNeutrNNLSP();
		drawCmsPrel(style.lumi, style.METCut, style.noJet, style.isBestJet);
		string nameXsPlot = "results/" + style.flag + "/ExpXsecLimit";
		c1->SaveAs((nameXsPlot + ".pdf").c_str());
		c1->SaveAs((nameXsPlot + ".png").c_str());
	}
	//Observed asymptotoc X-section limit
	{
		TH2F *hxsec = (TH2F*) h->Clone();
		hxsec->GetZaxis()->SetTitle("(Observed asympt.) 95% CL Upper Limit [pb]");
		c1->SetLogz(1);
		PlotTool->Area(hxsec, x, y, ObsXsecLimitAsym);
		SetZRange(hxsec);
		hxsec->GetZaxis()->SetTitleOffset(1.5);
		hxsec->Draw("colz");
		if (style.drawGluinoNLSPExclusionRegion)
			DrawNeutrNNLSP();
		drawCmsPrel(style.lumi, style.METCut, style.noJet, style.isBestJet);
		string nameXsPlot = "results/" + style.flag + "/ObsAsympXsecLimit";
		c1->SaveAs((nameXsPlot + ".pdf").c_str());
		c1->SaveAs((nameXsPlot + ".png").c_str());
	}
	//Expected asymptotic X-section limit
	{
		TH2F *hxsec = (TH2F*) h->Clone();
		hxsec->GetZaxis()->SetTitle("(Expected asympt.) 95% CL Upper Limit [pb]");
		c1->SetLogz(1);
		PlotTool->Area(hxsec, x, y, ExpXsecLimitAsym);
		SetZRange(hxsec);
		hxsec->GetZaxis()->SetTitleOffset(1.5);
		hxsec->Draw("colz");
		if (style.drawGluinoNLSPExclusionRegion)
			DrawNeutrNNLSP();
		drawCmsPrel(style.lumi, style.METCut, style.noJet, style.isBestJet);
		string nameXsPlot = "results/" + style.flag + "/ExpAsympXsecLimit";
		c1->SaveAs((nameXsPlot + ".pdf").c_str());
		c1->SaveAs((nameXsPlot + ".png").c_str());
	}
	c1->SetLogz(0);
	//Only if "Best Limit" is drawn, i.e. 2 or 3 jets selection
	{
		if (style.isBestJet) {
			TH2F *hxsec = (TH2F*) h->Clone();
			hxsec->GetZaxis()->SetTitle("");
			c1->SetLogz(0);
			PlotTool->Area(hxsec, x, y, BestLimitFromFile1or2);
			hxsec->Draw("col");
			if (style.drawGluinoNLSPExclusionRegion)
				DrawNeutrNNLSP();
			drawCmsPrel(style.lumi, style.METCut, style.noJet, style.isBestJet);
			TLatex as;
			as.SetNDC(true);
			as.SetTextFont(62);
			as.SetTextSize(0.035);
			as.SetTextColor(1);
			as.DrawLatex(0.81, 0.83, "Selection for");
			as.DrawLatex(0.81, 0.8, "best expected");
			as.DrawLatex(0.81, 0.77, "limit:");
			as.SetTextColor(2);
			std::string legendRed = style.isBestJetFile2;
			as.DrawLatex(0.83, 0.72, legendRed.c_str());
			as.SetTextColor(4);
			std::string legendBlue = style.isBestJetFile1;
			as.DrawLatex(0.83, 0.67, legendBlue.c_str());

			string nameXsPlot = "results/" + style.flag + "/BestLimitFromFile1or2";
			c1->SaveAs((nameXsPlot + ".pdf").c_str());
			c1->SaveAs((nameXsPlot + ".png").c_str());
		}
	}

}

void PlotAllObserved(std::vector<ExclusionCurves*> limits, PlotStyles * style, TH1*h) {
	gStyle->SetPadRightMargin(0.08);
				gStyle->SetPadLeftMargin(0.2);
				gStyle->SetTitleOffset(1.3, "xyz");
				gStyle->SetTitleOffset(1.9, "y");
				gStyle->SetNdivisions(505);
				gStyle->SetTitleFont(43, "xyz");
				gStyle->SetTitleSize(32, "xyz");
	c1->UseCurrentStyle();
		h->UseCurrentStyle();
	{

		h->Draw();
		string neutralinotype = style->neutralinotype;
		string neutralinomass = style->neutralinomass;
		string legtitle = "#splitline{GGM " + neutralinotype + "-like #tilde{#Chi}^{0}}{#slash{E}_{T} [GeV]:}";

		TLegend * leg = new TLegend(0.62, 0.52, 0.95, 0.87, "");
		leg->SetBorderSize(0);
		leg->SetLineColor(0);
		leg->SetFillColor(10);
		leg->SetFillStyle(1001);
		leg->SetTextFont(42);
		leg->SetTextSize(0.035);
		leg->SetHeader(legtitle.c_str());
		int color = 1;
		for (std::vector<ExclusionCurves*>::iterator it = limits.begin(); it != limits.end(); ++it) {
			TGraph * g = (*it)->obs_asym;
			if (color == 5) color=kOrange-3;
			g->SetLineColor(color);
			g->SetLineStyle(1);

			if (color == 0) {
				g->SetLineWidth(3);
				g->Draw("l");
			} else {
				g->Draw("lsame");
			}
			leg->AddEntry(g, ((*it)->Name).c_str(), "l");
			color++;
		}

		if (style->drawGluinoNLSPExclusionRegion) {
			DrawNeutrNNLSP();
		}

		//draw heading
		drawCmsPrel(style->lumi, style->METCut, style->noJet, style->isBestJet);
		leg->Draw();
		gPad->RedrawAxis();
		c1->SaveAs(("results/" + style->flag + "/AllLimitsPerBin.pdf").c_str());
		c1->SaveAs(("results/" + style->flag + "/AllLimitsPerBin.png").c_str());
	}
	{
			h->Draw();
			string neutralinotype = style->neutralinotype;
			string neutralinomass = style->neutralinomass;
			string legtitle = "#splitline{GGM " + neutralinotype + "-like #tilde{#Chi}^{0}}{#slash{E}_{T} [GeV]:}";
			TLegend * leg = new TLegend(0.62, 0.52, 0.95, 0.87, "");
			leg->SetBorderSize(0);
			leg->SetLineColor(0);
			leg->SetFillColor(10);
			leg->SetFillStyle(1001);
			leg->SetTextFont(42);
			leg->SetTextSize(0.035);
			leg->SetHeader(legtitle.c_str());
			int color = 1;
			for (std::vector<ExclusionCurves*>::iterator it = limits.begin(); it != limits.end(); ++it) {
				TGraph * g = (*it)->exp_asym;
				if (color == 5) color=kOrange-3;
				g->SetLineColor(color);
				g->SetLineStyle(1);
				if (color == 0) {
					g->SetLineWidth(3);

					g->Draw("l");
				} else {
					g->Draw("lsame");
				}
				leg->AddEntry(g, ((*it)->Name).c_str(), "l");
				color++;
			}

			if (style->drawGluinoNLSPExclusionRegion) {
				DrawNeutrNNLSP();
			}

			//draw heading
			drawCmsPrel(style->lumi, style->METCut, style->noJet, style->isBestJet);
			leg->Draw();
			gPad->RedrawAxis();
			c1->SaveAs(("results/" + style->flag + "/AllLimitsPerBinExp.pdf").c_str());
			c1->SaveAs(("results/" + style->flag + "/AllLimitsPerBinExp.png").c_str());
		}

}

void DrawExclusion(ExclusionCurves * exclA, PlotStyles * style, TH1*h, ExclusionCurves * exclB = 0) {
	if (exclA == 0) {
		cout << "No Exclusion Cruves To Draw...exclA is 0" << endl;
		return;
	};
	if (exclA->exp == 0 || exclA->obs == 0 || exclA->onesigband == 0 || exclA->onesigbandNLOXS == 0) {
		cout << "No Exclusion Cruves To Draw...exclA is not properly filled" << endl;
		return;
	}
	bool drawComp = false;
	if (exclB != 0) {
		drawComp = true;
	}
	if (!drawComp)
		std::system(("mkdir results/" + style->flag).c_str());
	else {
		std::system(("mkdir results/Comp_" + style->flagForComp).c_str());
	}
	//set styles
	//	gStyle->SetPadTopMargin(0.1);
	//	gStyle->SetPadBottomMargin(0.15);
	gStyle->SetPadRightMargin(0.08);
	gStyle->SetPadLeftMargin(0.2);
	gStyle->SetTitleOffset(1.3, "xyz");
	gStyle->SetTitleOffset(1.9, "y");
	gStyle->SetNdivisions(505);
	gStyle->SetTitleFont(43, "xyz");
	gStyle->SetTitleSize(32, "xyz");



	//set styles of Graphs
	setExclusionStyles(exclA, false);
	if (drawComp) {
		setExclusionStyles(exclB, true);
	}

	c1->UseCurrentStyle();
		h->UseCurrentStyle();
	{//Exclusion Contours
		TH2F *hs = (TH2F*) h->Clone();
		hs->GetZaxis()->SetTitle("");

		TGraph * gCLsObsExcl = exclA->obs;
		TGraph * gCLsExpExcl = exclA->exp;
		TGraph * gCLs1Sigma = exclA->onesigband;
		if (style->drawObsNLOXSErrorBand)
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

		//Draw Legend
		string neutralinotype = style->neutralinotype;
		string neutralinomass = style->neutralinomass;
		string legtitle = "#splitline{GGM " + neutralinotype + "-like #tilde{#Chi}^{0}}{m_{#tilde{#Chi}^{0}} = " + neutralinomass + " GeV}";
		if (style->drawGluinoNLSPExclusionRegion) {
			legtitle = "#splitline{GGM " + neutralinotype + "-like #tilde{#Chi}^{0}}{m_{#tilde{q}} = 2500 GeV}";
		}

		TLegend * leg = new TLegend(0.55, 0.57, 0.95, 0.87);
		if (drawComp) {
			leg = new TLegend(0.6, 0.5, 0.95, 0.87);
		}

		TH1F * legdummy = 0;
		//leg->AddEntry(legdummy,  ("m_{#tilde{#Chi}^{0}} = "+neutralinomass+" [GeV]").c_str(), "l");
		leg->SetBorderSize(0);
		leg->SetLineColor(0);
		leg->SetFillColor(10);
		leg->SetFillStyle(1001);
		leg->SetTextFont(62);
		leg->SetTextSize(0.04);
		leg->SetHeader(legtitle.c_str());

		if (!drawComp) {
			leg->AddEntry(gCLsObsExcl, "Obs., NLO", "l");
			leg->AddEntry(gCLsExpExcl, "Exp., NLO", "l");
			leg->AddEntry(gCLs1Sigma, "#pm 1#sigma, NLO", "lf");
		} else {
			leg->AddEntry(exclA->obs, (style->isBestJetFile1 + ", Obs.").c_str(), "l");
			leg->AddEntry(exclA->exp, (style->isBestJetFile1 + ", Exp.").c_str(), "l");
			leg->AddEntry(exclA->onesigband, (style->isBestJetFile1 + ", #pm 1#sigma").c_str(), "f");
			leg->AddEntry(exclB->obs, (style->isBestJetFile2 + ", Obs.").c_str(), "l");
			leg->AddEntry(exclB->exp, (style->isBestJetFile2 + ", Exp.").c_str(), "l");
			leg->AddEntry(exclB->onesigband, (style->isBestJetFile2 + ", #pm 1#sigma").c_str(), "f");
		}
		leg->Draw();
		gPad->RedrawAxis();

		if (!drawComp) {
			//Draw Excluded region
			double min1 = hs->GetXaxis()->GetBinLowEdge(0);
			double min2 = hs->GetYaxis()->GetBinLowEdge(0);
			double max2 = hs->GetYaxis()->GetBinLowEdge(hs->GetNbinsX() + 1);
			double max1 = hs->GetXaxis()->GetBinLowEdge(hs->GetNbinsX() + 1);
			TGraph * edgeOfExclRegion = exclA->p1sigma;
			if (style->drawObsNLOXSErrorBand)
				edgeOfExclRegion = exclA->p1sigmaNLOCX;
			TGraph * excludedRegion = GetExcludedRegion(edgeOfExclRegion, min1, min2, max1, max2);
			excludedRegion->Draw("f");
			TLatex* lat3 = new TLatex(0.24, 0.3, "Excluded");
			lat3->SetNDC(true);
			lat3->SetTextFont(62);
			lat3->SetTextSize(0.05);

			lat3->Draw();
		}
		if (style->drawGluinoNLSPExclusionRegion) {
			DrawNeutrNNLSP();
		}

		//draw heading
		drawCmsPrel(style->lumi, style->METCut, style->noJet, style->isBestJet);

		string nameExcl = "results/" + style->flag + "/Exclusion_";
		if (drawComp) {
			nameExcl = "results/Comp_" + style->flagForComp + "/Exclusion_";
		}
		c1->SaveAs((nameExcl + ".pdf").c_str());
		c1->SaveAs((nameExcl + ".png").c_str());
	}

}

template<class T>
ExclusionCurves GetExclusionContours(PlotTools<SusyScan> *PlotTool, PlotStyles style, double(*x)(const T*), double(*y)(const T*), TH1*h, bool excludeBelowExcludedRegion = false, string altFlagName = "", string name = "") {
	//Xsection
	std::system(("mkdir results/" + style.flag).c_str());
	{
		TH2F *hxsec = (TH2F*) h->Clone();
		hxsec->GetZaxis()->SetTitle("Observed in/out");
		PlotTool->Area(hxsec, x, y, ExpExclusionAsym);
		hxsec->GetZaxis()->SetTitleOffset(1.5);
		std::vector<TGraph*> contours = PlotTool->GetContours(hxsec, 3, excludeBelowExcludedRegion);
		/// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		hxsec->Draw("colz");
		int col = kBlue - 10;
		for (std::vector<TGraph*>::iterator cont = contours.begin(); cont != contours.end(); ++cont) {
			if (!*cont)
				continue;
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
		drawCmsPrel(style.lumi, style.METCut, style.noJet, style.isBestJet);
		string nameXsPlot = "results/" + style.flag + "/ObservedInOut" + name;
		c1->SaveAs((nameXsPlot + ".pdf").c_str());
		c1->SaveAs((nameXsPlot + ".png").c_str());
	}

	{//Exclusion Contours
		TH2F *hs = (TH2F*) h->Clone();
		hs->GetZaxis()->SetTitle("");
		TGraph * gCLsObsExcl = PlotTool->GetContour(hs, x, y, ObsExclusion, 3, 0, 1, 1, excludeBelowExcludedRegion);
		TGraph * gCLsObsExclAsym = PlotTool->GetContour(hs, x, y, ObsExclusionAsym, 3, 0, 1, 1, excludeBelowExcludedRegion);
		TGraph * gCLsExpExcl = PlotTool->GetContour(hs, x, y, ExpExclusion, 3, 0, 1, 2, excludeBelowExcludedRegion);
		TGraph * gCLsExpExclAsym = PlotTool->GetContour(hs, x, y, ExpExclusionAsym, 3, 0, 1, 2, excludeBelowExcludedRegion);
		TGraph * gCLsExpExclm1 = PlotTool->GetContour(hs, x, y, ExpExclusionM1, 3, 1, 5, 2, excludeBelowExcludedRegion);
		TGraph * gCLsExpExclp1 = PlotTool->GetContour(hs, x, y, ExpExclusionP1, 3, 0, 5, 2, excludeBelowExcludedRegion);
		TGraph * gCLsObsExclm1 = PlotTool->GetContour(hs, x, y, ObsExclusionM1, 3, 1, 5, 2, excludeBelowExcludedRegion);
		TGraph * gCLsObsExclp1 = PlotTool->GetContour(hs, x, y, ObsExclusionP1, 3, 0, 5, 2, excludeBelowExcludedRegion);
		Smooth(gCLsObsExcl,15);
		Smooth(gCLsObsExclAsym,15);
		Smooth(gCLsExpExcl,15);
		Smooth(gCLsExpExclAsym,15);
		Smooth(gCLsExpExclm1,15);
		Smooth(gCLsExpExclp1,15);
		Smooth(gCLsObsExclm1,15);
		Smooth(gCLsObsExclp1,15);
		TGraph * gCLs1Sigma = MakeBand(gCLsExpExclm1, gCLsExpExclp1);
		TGraph * gCLs1SigmaObsNLO = MakeBand(gCLsObsExclm1, gCLsObsExclp1);
		ExclusionCurves ret;
		ret.exp = gCLsExpExcl;
		ret.exp_asym = gCLsExpExclAsym;
		ret.obs = gCLsObsExcl;
		ret.obs_asym = gCLsObsExclAsym;
		ret.onesigband = gCLs1Sigma;
		ret.p1sigma = gCLsExpExclp1;
		ret.onesigbandNLOXS = gCLs1SigmaObsNLO;
		ret.p1sigmaNLOCX = gCLsObsExclp1;
		ret.Name = altFlagName;
		return ret;
	}

}
void interpolateAndFillPoints(TheLimits * genpointsInterpol, TheLimits * genpoints, bool isNeutrGluinoScan = false) {
	if (isNeutrGluinoScan) {
		cout << "!!!!!!!!!FillNeutrGluinoSCAN!!!!!!!!!!!!" << endl;
		genpoints->FillEmptyPointsByInterpolation(Mchi1, Mgluino);
		genpoints->FillEmptyPointsByInterpolation(Mgluino, Mchi1);

		genpointsInterpol->ExpandGrid<SusyScan> (Mchi1, Mgluino);
		genpointsInterpol->ExpandGrid<SusyScan> (Mchi1, Mgluino);

		genpointsInterpol->FillEmptyPointsNeutralinoScan(Mchi1, Mgluino);
		genpointsInterpol->FillEmptyPointsByInterpolation(Mchi1, Mgluino);
		genpointsInterpol->FillEmptyPointsByInterpolation(Mgluino, Mchi1);

	} else {
		//Make grid in Msquark, Mgluino finer by factor of 4 by linear interpolation
		genpoints->FillEmptyPointsByInterpolation(Msquark, Mgluino);
		genpointsInterpol->FillEmptyPointsByInterpolation(Msquark, Mgluino);
		genpoints->FillEmptyPointsByInterpolation(Mgluino, Msquark);
		genpointsInterpol->FillEmptyPointsByInterpolation(Mgluino, Msquark);
		genpointsInterpol->ExpandGrid<SusyScan> (Msquark, Mgluino);
		genpointsInterpol->ExpandGrid<SusyScan> (Msquark, Mgluino);
	}

}
void GetBestExpectedPlotTools(PlotTools<SusyScan>*& plotTools, PlotTools<SusyScan>*& plotToolsInterpol, std::string filename, std::string filename2, bool isNeutrGluinoScan = false) {
	filename = filename + "/filelist.txt";
	filename2 = filename2 + "/filelist.txt";
	TheLimits * genpointsInterpol = new TheLimits();
	TheLimits * genpoints = new TheLimits();
	genpointsInterpol->FillBestExpectedLimit(filename, filename2);
	genpoints->FillBestExpectedLimit(filename, filename2);

	interpolateAndFillPoints(genpointsInterpol, genpoints, isNeutrGluinoScan);

	plotTools = new PlotTools<SusyScan> (genpoints->GetScan());
	//std::cout<<"done -try to get next scan"<<std::endl;
	plotToolsInterpol = new PlotTools<SusyScan> (genpointsInterpol->GetScan());
	//std::cout<<"done - return"<<std::endl;
}

void GetPlotTools(PlotTools<SusyScan>*& plotTools, PlotTools<SusyScan>*& plotToolsInterpol, std::string filename, bool isNeutrGluinoScan = false, bool isNeutrSquarkScan = false, string fileappendix = "/filelist.txt") {
	filename = filename + fileappendix;
	TheLimits * genpointsInterpol = new TheLimits();
	TheLimits * genpoints = new TheLimits();
	cout << "Fill interpol:" << filename << endl;
	//
	genpointsInterpol->Fill(filename);

	//cout << "Fill " << endl;
	genpoints->Fill(filename);

	//cout << "interpol..." << endl;
	interpolateAndFillPoints(genpointsInterpol, genpoints, isNeutrGluinoScan);

	plotTools = new PlotTools<SusyScan> (genpoints->GetScan());
	plotToolsInterpol = new PlotTools<SusyScan> (genpointsInterpol->GetScan());
}
template<class T>
void PlotBinComparison(PlotTools<SusyScan>*& Scan, PlotTools<SusyScan>*& ScanInterpol, double(*x)(const T*), double(*y)(const T*), string filename, string filenameSingleBins, PlotStyles ret, TH1 * hi, TH1 * h, bool excludeBelowExcludedRegion = false, string filenameforresulttex = "") {
	bool doResultTex = false;
	if (filenameforresulttex != "") {
		doResultTex = true;
	}
	GetPlotTools(Scan, ScanInterpol, filename, excludeBelowExcludedRegion, false);
	ExclusionCurves exclBinoCombinedChannels = GetExclusionContours(ScanInterpol, ret, x, y, hi, excludeBelowExcludedRegion, ">100", "comb");
	if (doResultTex)
		CreateSamplePointTexFile(ScanInterpol,x,  y, 720.,800., filenameforresulttex, "");

	GetPlotTools(Scan, ScanInterpol, filenameSingleBins, excludeBelowExcludedRegion, false, "/filelist_ch0.txt");
	ExclusionCurves exclBinoChannel0 = GetExclusionContours(ScanInterpol, ret, x, y, hi, excludeBelowExcludedRegion, "100-120", "bin1");
	if (doResultTex)
		CreateSamplePointTexFile(ScanInterpol, x,y, 720.,800., filenameforresulttex, "binA");
	GetPlotTools(Scan, ScanInterpol, filenameSingleBins, excludeBelowExcludedRegion, false, "/filelist_ch1.txt");
	ExclusionCurves exclBinoChannel1 = GetExclusionContours(ScanInterpol, ret, x, y, hi, excludeBelowExcludedRegion, "120-160", "bin2");
	if (doResultTex)
			CreateSamplePointTexFile(ScanInterpol, x,y, 720.,800., filenameforresulttex, "binB");

	GetPlotTools(Scan, ScanInterpol, filenameSingleBins, excludeBelowExcludedRegion, false, "/filelist_ch2.txt");
	ExclusionCurves exclBinoChannel2 = GetExclusionContours(ScanInterpol, ret, x, y, hi, excludeBelowExcludedRegion, "160-200", "bin3");
	if (doResultTex)
			CreateSamplePointTexFile(ScanInterpol, x,y, 720.,800., filenameforresulttex, "binC");

	GetPlotTools(Scan, ScanInterpol, filenameSingleBins, excludeBelowExcludedRegion, false, "/filelist_ch3.txt");
	ExclusionCurves exclBinoChannel3 = GetExclusionContours(ScanInterpol, ret, x, y, hi, excludeBelowExcludedRegion, "200-270", "bin4");
	if (doResultTex)
			CreateSamplePointTexFile(ScanInterpol, x,y, 720.,800., filenameforresulttex, "binD");

	GetPlotTools(Scan, ScanInterpol, filenameSingleBins, excludeBelowExcludedRegion, false, "/filelist_ch4.txt");
	ExclusionCurves exclBinoChannel4 = GetExclusionContours(ScanInterpol, ret, x, y, hi, excludeBelowExcludedRegion, "270-350", "bin5");
	if (doResultTex)
			CreateSamplePointTexFile(ScanInterpol, x,y, 720.,800., filenameforresulttex, "binE");

	GetPlotTools(Scan, ScanInterpol, filenameSingleBins, excludeBelowExcludedRegion, false, "/filelist_ch5.txt");
	ExclusionCurves exclBinoChannel5 = GetExclusionContours(ScanInterpol, ret, x, y, hi, excludeBelowExcludedRegion, ">350", "bin6");
	if (doResultTex)
			CreateSamplePointTexFile(ScanInterpol, x,y, 720.,800., filenameforresulttex, "binF");
	if(!doResultTex){
	setStyles(Scan);
	PlotStyles retHighest=ret;
	retHighest.flag=ret.flag+"HighestBin";
	DrawStandardPlots(Scan, retHighest, x, y, h);


	std::vector<ExclusionCurves*> limitvector;
	limitvector.push_back(&exclBinoCombinedChannels);
//	limitvector.push_back(&exclBinoChannel0);
//	limitvector.push_back(&exclBinoChannel1);
	limitvector.push_back(&exclBinoChannel2);
	limitvector.push_back(&exclBinoChannel3);
	limitvector.push_back(&exclBinoChannel4);
	limitvector.push_back(&exclBinoChannel5);
	PlotAllObserved(limitvector, &ret, hi);
	}
}
int plot(int argc, char** argv) {
	//util::StyleSettings::paperNoTitle();
	setMyTDRStyle();
	gROOT->SetStyle("mytdrStyle");
	c1 = new TCanvas("c1", "c1");
	c1->cd();
	gStyle->SetTextFont(43);
	gStyle->SetTextSize(32);

	TH2F h("h", ";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; cross section [pb]", 21, 360, 2040, 21, 360, 2040);
	TH2F hi("hi", ";m_{#tilde{q}} [GeV]; m_{#tilde{g}} [GeV]; cross section [pb]", 81, 390, 2010, 81, 390, 2010);

	TH2F hNeutrGluino("hNeutrGluino", ";m_{#chi^{0}_{1}} [GeV]; m_{#tilde{g}} [GeV]; cross section [pb]", 10, 100, 1100, 21, 360, 2040);

	TH2F hNeutrGluinoi("hNeutrGluinoi", ";m_{#chi^{0}_{1}} [GeV]; m_{#tilde{g}} [GeV]; cross section [pb]", 37, 125, 1075, 93, 150, 2010);

	TH2F hNeutrSquark("hNeutrSquark", ";m_{#chi^{0}_{1}} [GeV]; m_{#tilde{q}} [GeV]; cross section [pb]", 10, 100, 1100, 21, 360, 2040);

	TH2F hNeutrSquarki("hNeutrSquarki", ";m_{#chi^{0}_{1}} [GeV]; m_{#tilde{q}} [GeV]; cross section [pb]", 37, 125, 1075, 93, 150, 2010);

	PlotTools<SusyScan> *Scan, *ScanInterpol;

	///==================Bino-Neutr vs Gluino Limits 3j================================
	GetPlotTools(Scan, ScanInterpol, filenames::file_binoNeutr3j, true);
	setStyles(Scan);
	DrawStandardPlots(Scan, retBinoNeutr_Gluino3j, Mchi1, Mgluino, &hNeutrGluino);
	ExclusionCurves exclBinoNeutr_gl_3j = GetExclusionContours(ScanInterpol, retBinoNeutr_Gluino3j, Mchi1, Mgluino, &hNeutrGluinoi, true);
	DrawExclusion(&exclBinoNeutr_gl_3j, &retBinoNeutr_Gluino3j, &hNeutrGluinoi);

	///==================Bino-Neutr vs Gluino Limits 3j================================
	GetPlotTools(Scan, ScanInterpol, filenames::file_binoNeutr2j, true);
	setStyles(Scan);
	DrawStandardPlots(Scan, retBinoNeutr_Gluino2j, Mchi1, Mgluino, &hNeutrGluino);
	ExclusionCurves exclBinoNeutr_gl_2j = GetExclusionContours(ScanInterpol, retBinoNeutr_Gluino2j, Mchi1, Mgluino, &hNeutrGluinoi, true);
	DrawExclusion(&exclBinoNeutr_gl_2j, &retBinoNeutr_Gluino2j, &hNeutrGluinoi);

	///==================Bino-Neutr Limits 2/3j - Best Expected ================================
		//cout << "Get Best Expected!!" << endl;
		GetBestExpectedPlotTools(Scan, ScanInterpol, filenames::file_binoNeutr2j, filenames::file_binoNeutr3j);
		//cout << "Get Best Expected 2!!" << endl;
		setStyles(Scan);
		DrawStandardPlots(Scan, retBinoNeutr_GluinoBestJet, Mchi1, Mgluino, &hNeutrGluino);
		ExclusionCurves exclBinoNeutrBestJet = GetExclusionContours(ScanInterpol, retBinoNeutr_GluinoBestJet, Mchi1, Mgluino, &hNeutrGluinoi);
		DrawExclusion(&exclBinoNeutrBestJet, &retBinoNeutr_GluinoBestJet, &hi);
	//Compare Exclusion between 2/3 jet
	DrawExclusion(&exclBinoNeutr_gl_2j, &retBinoNeutr_GluinoBestJet, &hNeutrGluinoi, &exclBinoNeutr_gl_3j);

	///==================Bino Limits 3j================================
	//cout << "Get Bino 3j!!" << endl;
	GetPlotTools(Scan, ScanInterpol, filenames::file_bino3j);
	//cout << "Get PlotTools done!!" << endl;
	setStyles(Scan);
	//cout << "Draw plots" << endl;
	DrawStandardPlots(Scan, retBino3j, Msquark, Mgluino, &h);
	//cout << "GetExclusion" << endl;
	ExclusionCurves exclBino3j = GetExclusionContours(ScanInterpol, retBino3j, Msquark, Mgluino, &hi);
	//cout << "Draw Exclusion" << endl;
	DrawExclusion(&exclBino3j, &retBino3j, &hi);
	///==================Bino Limits 2j================================
	GetPlotTools(Scan, ScanInterpol, filenames::file_bino2j);
	setStyles(Scan);
	DrawStandardPlots(Scan, retBino2j, Msquark, Mgluino, &h);
	ExclusionCurves exclBino2j = GetExclusionContours(ScanInterpol, retBino2j, Msquark, Mgluino, &hi);
	DrawExclusion(&exclBino2j, &retBino2j, &hi);

	///==================Bino Limits 2/3j - Best Expected ================================
	//cout << "Get Best Expected!!" << endl;
	GetBestExpectedPlotTools(Scan, ScanInterpol, filenames::file_bino2j, filenames::file_bino3j);
	//cout << "Get Best Expected 2!!" << endl;
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
	ExclusionCurves exclWino2j = GetExclusionContours(ScanInterpol, retWino2j, Msquark, Mgluino, &hi);
	DrawExclusion(&exclWino2j, &retWino2j, &hi);

	///==================Wino Limits 2/3j - Best Expected ================================
	//cout<<"Get Best Expected!!"<<endl;
	GetBestExpectedPlotTools(Scan, ScanInterpol, filenames::file_wino2j, filenames::file_wino3j);

	setStyles(Scan);
	DrawStandardPlots(Scan, retWinoBestJet, Msquark, Mgluino, &h);
	ExclusionCurves exclWinoBestJet = GetExclusionContours(ScanInterpol, retWinoBestJet, Msquark, Mgluino, &hi);
	DrawExclusion(&exclWinoBestJet, &retWinoBestJet, &hi);
	//Compare Exclusion between 2/3 jet
	DrawExclusion(&exclWino2j, &retWinoBestJet, &hi, &exclWino3j);

	///==================Wino Limits >200 (bin4-7 gemerged)================================
	GetPlotTools(Scan, ScanInterpol, filenames::file_wino3j_MergedBins);
	setStyles(Scan);
	DrawStandardPlots(Scan, retWinoMerged3j, Msquark, Mgluino, &h);
	ExclusionCurves exclWino1Bin = GetExclusionContours(ScanInterpol, retWinoMerged3j, Msquark, Mgluino, &hi);
	DrawExclusion(&exclWino1Bin, &retWinoMerged3j, &hi);

	///==================CREATE TEX DEFINITIONS FOR SAMPLE POINT/RESULTING LIMITS================================
	string filenameforresulttex = "results/interpretationresults.tex";
	std::system(("rm " + filenameforresulttex).c_str());
	GetPlotTools(Scan, ScanInterpol, filenames::file_bino3j);
	//CreateSamplePointTexFile(ScanInterpol, Msquark, Mgluino, 720., 800., filenameforresulttex);
	CreateResultPointTexFile(exclBino3j.obs, ScanInterpol, Msquark, Mgluino, 1000, "Bino", filenameforresulttex);
	GetPlotTools(Scan, ScanInterpol, filenames::file_wino3j);
	CreateResultPointTexFile(exclWino3j.obs, ScanInterpol, Msquark, Mgluino, 1000, "Wino", filenameforresulttex);
	GetPlotTools(Scan, ScanInterpol, filenames::file_binoNeutr3j);
	CreateResultPointTexFile(exclBinoNeutr_gl_3j.obs, ScanInterpol, Mchi1, Mgluino, 150, "BinoNeutrMin", filenameforresulttex);
	CreateResultPointTexFile(exclBinoNeutr_gl_3j.obs, ScanInterpol, Mchi1, Mgluino, 650, "BinoNeutrMax", filenameforresulttex);

	///==================Limits - which channel is most sensitive? ---3 jet
	PlotBinComparison(Scan, ScanInterpol, Msquark, Mgluino, filenames::file_wino3j, filenames::file_wino3j_SingleBins, retWino3j, &hi, &h);
	PlotBinComparison(Scan, ScanInterpol, Msquark, Mgluino, filenames::file_bino3j, filenames::file_bino3j_SingleBins, retBino3j, &hi, &h, false);
	PlotBinComparison(Scan, ScanInterpol, Mchi1, Mgluino, filenames::file_binoNeutr3j, filenames::file_binoNeutr3j_SingleBins, retBinoNeutr_Gluino3j, &hNeutrGluinoi,&hNeutrGluino, true);
	///==================Limits - which channel is most sensitive? ---2 jet
	PlotBinComparison(Scan, ScanInterpol, Msquark, Mgluino, filenames::file_wino2j, filenames::file_wino2j_SingleBins, retWino2j, &hi, &h);
	PlotBinComparison(Scan, ScanInterpol, Msquark, Mgluino, filenames::file_bino2j, filenames::file_bino2j_SingleBins, retBino2j, &hi, &h);
	PlotBinComparison(Scan, ScanInterpol, Mchi1, Mgluino, filenames::file_binoNeutr2j, filenames::file_binoNeutr2j_SingleBins, retBinoNeutr_Gluino2j, &hNeutrGluinoi,&hNeutrGluino, true);

	///==================Limits - Demopoint (bino,3 jet)
	PlotBinComparison(Scan, ScanInterpol, Msquark, Mgluino, filenames::file_bino3j, filenames::file_bino3j_DemoPoint, retBino3j, &hi, &h, false, filenameforresulttex);

}

int main(int argc, char** argv) {
	vector < string > filenames;
	filenames.push_back(filenames::file_wino3j);
	filenames.push_back(filenames::file_wino2j);
	filenames.push_back(filenames::file_wino3j_MergedBins);
	filenames.push_back(filenames::file_bino3j);
	filenames.push_back(filenames::file_bino2j);
	filenames.push_back(filenames::file_binoNeutr3j);
	filenames.push_back(filenames::file_binoNeutr2j);

	filenames.push_back(filenames::file_wino3j_SingleBins);
	filenames.push_back(filenames::file_bino3j_SingleBins);
	filenames.push_back(filenames::file_binoNeutr3j_SingleBins);
	filenames.push_back(filenames::file_wino2j_SingleBins);
	filenames.push_back(filenames::file_bino2j_SingleBins);
	filenames.push_back(filenames::file_binoNeutr2j_SingleBins);
	filenames.push_back(filenames::file_bino3j_DemoPoint);
	redoFileList(filenames);
	return plot(argc, argv);
}
