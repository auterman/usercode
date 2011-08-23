#include "SusyScan.h"
#include "GeneratorMasses.h"
#include "ExclusionPlot.h"

#include "TGraph.h"
#include "TLatex.h"
#include "TF1.h"
#include "TH1F.h"
#include "TGraphErrors.h"
#include "TSpline.h"
#include "TStyle.h"
#include "TMath.h"
#include "TRint.h"
#include "TROOT.h"
#include "TPad.h"

#include <cmath>
#include <iostream>

double Luminosity = 1143.0; //[pb^-1]
double Mgluino(const SusyScan* p) {
	return p->Mgluino;
}
double Msquark(const SusyScan* p) {
	return p->Msquark;
}
double Mchi1(const SusyScan* p) {
	return p->Mchi1;
}
//std::cout<<Mgluino(p)<<"-"<<Msquark(p)<<"===="<<p->signal_acceptance<<std::endl;
double SignalAcceptance(const SusyScan* p) {
	if (false)
		std::cout << Mgluino(p) << "-" << Msquark(p) << "==!!!!!!!!SigAcc=="
				<< p->signal_acceptance << std::endl;
	return 100 * p->signal_acceptance;
}
double SignalContamination(const SusyScan* p) {
	return 100 * (p->signal_contamination * p->NLOXsection * p->Luminosity
			/ p->background);
}
double TriggerEfficiency(const SusyScan* p) {
	return 100 * p->triggerEff;
}
double TriggerEfficiencyN90Hits(const SusyScan* p) {
	return 100 * p->triggerEffN90Hits;
}
double NLOXsection(const SusyScan* p) {
	return p->NLOXsection;
}
//TODO add ISR uncerstainty on XS?
double NLOXsectionErrUp(const SusyScan* p) {
	return sqrt(p->PDFXsectionErr * p->PDFXsectionErr + (p->NLOXSecUp
			/ p->NLOXsection) * (p->NLOXSecUp / p->NLOXsection));
}
double NLOXsectionErrDown(const SusyScan* p) {
	return sqrt(p->PDFXsectionErr * p->NLOXsection * p->PDFXsectionErr
			* p->NLOXsection + p->NLOXSecDown / p->NLOXsection * p->NLOXSecDown
			/ p->NLOXsection);
}

double NLOXsectionM1(const SusyScan* p) {
	return NLOXsection(p) - NLOXsection(p) * NLOXsectionErrDown(p);
}
double NLOXsectionM2(const SusyScan* p) {
	return NLOXsection(p) - 2 * (NLOXsection(p) * NLOXsectionErrDown(p));
}
double NLOXsectionP1(const SusyScan* p) {
	return NLOXsection(p) + NLOXsection(p) * NLOXsectionErrUp(p);
}
double NLOXsectionP2(const SusyScan* p) {
	return NLOXsection(p) + 2 * (NLOXsection(p) * NLOXsectionErrUp(p));
}

double ExpXsecLimit(const SusyScan* p) {
	return p->ExpXsecLimit;
}
double ObsXsecLimit(const SusyScan* p) {
	return p->ObsXsecLimit;
}

double ExpExclusion(const SusyScan* p) {
	return (ExpXsecLimit(p) > NLOXsection(p)?1:0.01);
}
double ObsExclusion(const SusyScan* p) {
	return (ObsXsecLimit(p) > NLOXsection(p)?1:0.01);
}

double ObsExclusionM1(const SusyScan* p) {
	return (ObsXsecLimit(p) > NLOXsectionM1(p)?1:0.01);
}
double ObsExclusionP1(const SusyScan* p) {
	return (ObsXsecLimit(p) > NLOXsectionP1(p)?1:0.01);
}
double ObsExclusionM2(const SusyScan* p) {
	return (ObsXsecLimit(p) > NLOXsectionM2(p)?1:0.01);
}
double ObsExclusionP2(const SusyScan* p) {
	return (ObsXsecLimit(p) > NLOXsectionP2(p)?1:0.01);
}

double ExpExclusionM1(const SusyScan* p) {
	return (p->ExpXsecLimitM1>NLOXsection(p)?1:0.01);
}
double ExpExclusionP1(const SusyScan* p) {
	return (p->ExpXsecLimitP1>NLOXsection(p)?1:0.01);
}
double ExpExclusionM2(const SusyScan* p) {
	return (p->ExpXsecLimitM2>NLOXsection(p)?1:0.01);
}
double ExpExclusionP2(const SusyScan* p) {
	return (p->ExpXsecLimitP2>NLOXsection(p)?1:0.01);
}

double ExpXsecLimitSigCont(const SusyScan* p) {
	return p->ExpXsecLimitSigCont;
}
double ObsXsecLimitSigCont(const SusyScan* p) {
	return p->ObsXsecLimitSigCont;
}

double ExpExclusionSigCont(const SusyScan* p) {
	return (ExpXsecLimitSigCont(p) > NLOXsection(p)?1:0.01);
}
double ObsExclusionSigCont(const SusyScan* p) {
	return (ObsXsecLimitSigCont(p) > NLOXsection(p)?1:0.01);
}

double ObsExclusionM1SigCont(const SusyScan* p) {

	return (ObsXsecLimitSigCont(p) > NLOXsectionM1(p)?1:0.01);
}
double ObsExclusionP1SigCont(const SusyScan* p) {

	return (ObsXsecLimitSigCont(p) > NLOXsectionP1(p)?1:0.01);
}
double ObsExclusionM2SigCont(const SusyScan* p) {

	return (ObsXsecLimitSigCont(p) > NLOXsectionM2(p)?1:0.01);
}
double ObsExclusionP2SigCont(const SusyScan* p) {

	return (ObsXsecLimitSigCont(p) > NLOXsectionP2(p)?1:0.01);
}

double ExpExclusionM1SigCont(const SusyScan* p) {
	return (p->ExpXsecLimitM1SigCont>NLOXsection(p)?1:0.01);
}
double ExpExclusionP1SigCont(const SusyScan* p) {
	return (p->ExpXsecLimitP1SigCont>NLOXsection(p)?1:0.01);
}
double ExpExclusionM2SigCont(const SusyScan* p) {
	return (p->ExpXsecLimitM2SigCont>NLOXsection(p)?1:0.01);
}
double ExpExclusionP2SigCont(const SusyScan* p) {
	return (p->ExpXsecLimitP2SigCont>NLOXsection(p)?1:0.01);
}

void setPlottingStyle(TH1F& hsig) {

	hsig.SetStats(kFALSE);

	hsig.SetAxisRange(80, 500, "Y");
	hsig.SetAxisRange(0, 520, "X");
	hsig.SetAxisRange(200, 520, "X");

	hsig.GetXaxis()->SetTitle("m_{0} (GeV)");
	hsig.GetYaxis()->SetTitle("m_{1/2} (GeV)");
	hsig.GetYaxis()->SetTitleOffset(0.8);
	hsig.GetYaxis()->SetTitleSize(0.06);
	hsig.GetYaxis()->SetLabelSize(0.06);
	hsig.GetXaxis()->SetTitleOffset(0.9);
	hsig.GetXaxis()->SetTitleSize(0.06);
	hsig.GetXaxis()->SetLabelSize(0.06);

	hsig.SetLineWidth(1);
	hsig.SetLineColor(kBlue);

}

TGraph* set_sneutrino_d0_1(Int_t tanBeta) {
	double sn_m0[14] = { 0, 0, 48, 55, 80, 90, 100, 105, 109, 105, 100, 72, 55,
			0 };
	double sn_m12[14] = { 0, 140, 210, 220, 237, 241, 242, 241, 230, 220, 210,
			170, 150, 0 };

	TGraph* sn_d0_gr = new TGraph(14, sn_m0, sn_m12);

	sn_d0_gr->SetFillColor(kGreen + 3);
	sn_d0_gr->SetFillStyle(3001);

	return sn_d0_gr;
}

TGraph* set_sneutrino_d0_2(Int_t tanBeta) {
	double sn_m0[9] = { 0, 45, 75, 115, 130, 150, 163, 185, 0 };
	double sn_m12[9] = { 0, 140, 170, 213, 202, 183, 168, 140, 0 };

	TGraph* sn_d0_gr_2 = new TGraph(9, sn_m0, sn_m12);

	sn_d0_gr_2->SetFillColor(kGreen + 3);
	sn_d0_gr_2->SetFillStyle(3001);

	return sn_d0_gr_2;
}

TGraph* set_lep_ch(Int_t tanBeta) {
	if (tanBeta == 3)
		return set_lep_ch_tanBeta3();
	if (tanBeta == 10)
		return set_lep_ch_tanBeta10();
	if (tanBeta == 50)
		return set_lep_ch_tanBeta50();
}

TGraph* set_lep_ch_tanBeta10() {

	double ch_m0[12];
	double ch_m12[12];

	ch_m0[0] = 0;
	ch_m0[1] = 100;
	ch_m0[2] = 200;
	ch_m0[3] = 300;
	ch_m0[4] = 400;
	ch_m0[5] = 500;
	ch_m0[6] = 600;
	ch_m0[7] = 700;
	ch_m0[8] = 800;
	ch_m0[9] = 1000;
	ch_m0[10] = 1000;
	ch_m0[11] = 0;

	ch_m12[0] = 163;
	ch_m12[1] = 162;
	ch_m12[2] = 161;
	ch_m12[3] = 160;
	ch_m12[4] = 159;
	ch_m12[5] = 158;
	ch_m12[6] = 157;
	ch_m12[7] = 156;
	ch_m12[8] = 155.4;
	ch_m12[9] = 155.05;
	ch_m12[10] = 0;
	ch_m12[11] = 0;

	TGraph* ch_gr = new TGraph(12, ch_m0, ch_m12);

	ch_gr->SetFillColor(3);
	ch_gr->SetLineColor(3);
	//  ch_gr->SetLineWidth(3);
	ch_gr->SetFillStyle(3001);

	return ch_gr;

}

TGraph* set_lep_ch_tanBeta3() {

	double ch_m0[17];
	double ch_m12[17];

	ch_m0[0] = 0;
	ch_m0[1] = 100;
	ch_m0[2] = 150;
	ch_m0[3] = 200;
	ch_m0[4] = 250;
	ch_m0[5] = 300;
	ch_m0[6] = 350;
	ch_m0[7] = 400;
	ch_m0[8] = 450;
	ch_m0[9] = 500;
	ch_m0[10] = 550;
	ch_m0[11] = 600;
	ch_m0[12] = 650;
	ch_m0[13] = 700;
	ch_m0[14] = 750;
	ch_m0[15] = 750;
	ch_m0[16] = 0;

	ch_m12[0] = 170;
	ch_m12[1] = 168;
	ch_m12[2] = 167;
	ch_m12[3] = 165;
	ch_m12[4] = 163;
	ch_m12[5] = 161;
	ch_m12[6] = 158;
	ch_m12[7] = 156;
	ch_m12[8] = 154;
	ch_m12[9] = 152;
	ch_m12[10] = 150;
	ch_m12[11] = 148;
	ch_m12[12] = 147;
	ch_m12[13] = 145;
	ch_m12[14] = 144;
	ch_m12[15] = 0;
	ch_m12[16] = 0;

	TGraph* ch_gr = new TGraph(17, ch_m0, ch_m12);

	ch_gr->SetFillColor(3);
	ch_gr->SetLineColor(3);
	// ch_gr->SetLineWidth(3);
	ch_gr->SetFillStyle(3001);

	return ch_gr;

}

TGraph* set_lep_ch_tanBeta50() {

	double ch_m0[21];
	double ch_m12[21];

	ch_m0[0] = 200;
	ch_m0[1] = 250;
	ch_m0[2] = 300;
	ch_m0[3] = 350;
	ch_m0[4] = 400;
	ch_m0[5] = 450;
	ch_m0[6] = 500;
	ch_m0[7] = 550;
	ch_m0[8] = 600;
	ch_m0[9] = 650;
	ch_m0[10] = 700;
	ch_m0[11] = 750;
	ch_m0[12] = 800;
	ch_m0[13] = 850;
	ch_m0[14] = 900;
	ch_m0[15] = 950;
	ch_m0[16] = 1000;
	ch_m0[17] = 1050;
	ch_m0[18] = 1100;
	ch_m0[19] = 1100;
	ch_m0[20] = 200;

	ch_m12[0] = 157;
	ch_m12[1] = 156;
	ch_m12[2] = 156;
	ch_m12[3] = 155;
	ch_m12[4] = 155;
	ch_m12[5] = 154;
	ch_m12[6] = 154;
	ch_m12[7] = 153;
	ch_m12[8] = 153;
	ch_m12[9] = 152;
	ch_m12[10] = 152;
	ch_m12[11] = 152;
	ch_m12[12] = 152;
	ch_m12[13] = 152;
	ch_m12[14] = 152;
	ch_m12[15] = 153;
	ch_m12[16] = 153;
	ch_m12[17] = 153;
	ch_m12[18] = 154;
	ch_m12[19] = 0;
	ch_m12[20] = 0;

	TGraph* ch_gr = new TGraph(21, ch_m0, ch_m12);

	ch_gr->SetFillColor(3);
	ch_gr->SetLineColor(3);
	ch_gr->SetFillStyle(3001);

	return ch_gr;

}

TGraph* set_lep_sl(Int_t tanBeta) {

	// CMS SUSY Summer2010 implementation
	//  double sl_m0[] =  {0,  0, 30, 50, 60, 75, 80,90,100};
	//  double sl_m12[] = {0,245,240,220,200,150,100,50,0};

	//contour from D0 trilepton paper (PLB 680 (2009) 34-43)
	TGraph* lep_sl = 0;
	if (tanBeta == 3) {
		double sl_m0[] = { 0, 0, 10, 20, 30, 40, 50, 60, 70, 77, 88, 95 };
		double sl_m12[] = { 0, 245, 242, 239, 232, 222, 209, 189, 165, 140, 60,
				0 };
		int n = 12;
		lep_sl = new TGraph(n, sl_m0, sl_m12);
	}
	//CMS PTDR-II
	//* Selectron_R line mass=99, ISASUGRA7.69, A0=0, m_top=175, tan(beta]=10
	if (tanBeta == 10 || tanBeta == 50) {
		double sl_m0[] = { 0, 0, 11, 20, 24, 49, 70, 82, 88, 90 };
		double sl_m12[] = { 0, 240, 237, 233, 230, 200, 150, 100, 50, 0 };
		int n = 10;
		lep_sl = new TGraph(n, sl_m0, sl_m12);
	}

	lep_sl->SetFillColor(5);
	lep_sl->SetLineColor(5);
	lep_sl->SetFillStyle(3001);

	return lep_sl;
}

TGraph* set_tev_sg_cdf(Int_t tanBeta) {

	//  double sg_m0[] =  {0,  0, 20, 50,100,150,200,250,300,350,400,450,500,550,600,600};
	//  double sg_m12[] = {0,160,169,170,160,155,150,122,116,112,110,106,105,100, 98,  0};
	//  int np=16;
	//New CHF from CDF plot in ICHEP2010 talk (E. Halkiadakis)
	double sg_m0[] = { 0, 0, 30, 75, 150, 185, 225, 310, 360, 400, 430, 500,
			600, 600 };
	double sg_m12[] = { 0, 162, 168, 170, 160, 150, 130, 120, 109, 108, 100,
			96, 95, 0 };
	int np = 14;

	TGraph* sg_gr = new TGraph(np, sg_m0, sg_m12);

	//  gStyle->SetHatchesLineWidth(3);

	sg_gr->SetFillColor(2);
	sg_gr->SetLineColor(2);
	//  sg_gr->SetLineWidth(3);
	sg_gr->SetFillStyle(3001);

	return sg_gr;

}

TGraph* set_tev_sg_d0(Int_t tanBeta) {

	//official D0 contour from P. Verdier
	double sgd_m0[] = { 0, 0., 25., 80., 100., 150., 192., 250., 300., 350.,
			400., 450., 500., 600., 600., 0. };
	double sgd_m12[] = { 0, 167., 167., 163., 162., 157., 149., 136., 125.5,
			116., 109., 106.5, 105., 105., 0., 0. };
	int npd = 16;

	TGraph* sgd_gr = new TGraph(npd, sgd_m0, sgd_m12);

	gStyle->SetHatchesLineWidth(3);

	sgd_gr->SetFillColor(kMagenta + 1);
	sgd_gr->SetLineColor(kMagenta + 1);
	sgd_gr->SetLineWidth(3);
	sgd_gr->SetFillStyle(3335);
	//sgd_gr->SetFillStyle(3001);

	return sgd_gr;

}

// TGraph* set_tev_tlp_cdf(Int_t tanBeta){
//   double tlp1_m0[] = {   0, 20, 40, 60, 70, 80, 90, 80, 70, 60};
//   double tlp1_m12[] = {170,185,200,215,220,215,210,190,175,160};
//   TGraph* tlp1_gr = new TGraph(10,tlp1_m0,tlp1_m12);

//   tlp1_gr->SetFillColor(4);
//   tlp1_gr->SetLineColor(4);
//   tlp1_gr->SetFillStyle(1001);

//   return tlp1_gr;
// }

// TGraph* set_tev_tlp_d0(Int_t tanBeta){
//   double tlp2_m0[] = {  70, 80, 90,100,105,110,120,130,140};
//   double tlp2_m12[] = {160,172,184,196,205,195,185,173,160};
//   TGraph* tlp2_gr = new TGraph(9,tlp2_m0,tlp2_m12);

//   tlp2_gr->SetFillColor(4);
//   tlp2_gr->SetFillStyle(1001); 

//   return tlp2_gr;

// }


TGraph* set_tev_stau(Int_t tanBeta) {

	double st_m0_tanBeta3[] = { 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 0 };
	double st_m12_tanBeta3[] = { 337, 341, 356, 378, 406, 439, 473, 510, 548,
			587, 626, 626 };

	double st_m0_tanBeta10[] =
			{ 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 0 };
	double st_m12_tanBeta10[] = { 213, 220, 240, 275, 312, 351, 393, 435, 476,
			518, 559, 559 };

	double st_m0_tanBeta50[] = { 200, 210, 220, 230, 240, 250, 260, 270, 280,
			290, 310, 325, 200, 200 };
	double st_m12_tanBeta50[] = { 206, 226, 246, 267, 288, 310, 332, 354, 376,
			399, 450, 500, 500, 206 };

	TGraph* st_gr_tanBeta3 = new TGraph(12, st_m0_tanBeta3, st_m12_tanBeta3);
	TGraph* st_gr_tanBeta10 = new TGraph(12, st_m0_tanBeta10, st_m12_tanBeta10);
	TGraph* st_gr_tanBeta50 = new TGraph(14, st_m0_tanBeta50, st_m12_tanBeta50);

	st_gr_tanBeta3->SetFillColor(40);
	st_gr_tanBeta3->SetFillStyle(1001);

	st_gr_tanBeta50->SetFillColor(40);
	st_gr_tanBeta50->SetFillStyle(1001);

	st_gr_tanBeta10->SetFillColor(40);
	st_gr_tanBeta10->SetFillStyle(1001);

	if (tanBeta == 3)
		return st_gr_tanBeta3;
	if (tanBeta == 10)
		return st_gr_tanBeta10;
	if (tanBeta == 50)
		return st_gr_tanBeta50;

}

TF1* constant_squark(int tanBeta, int i) {
	//---lines of constant gluino/squark
	double coef1 = 0.35;
	double coef2[] = { 5, 5, 4.6, 4.1 };

	char hname[200];

	sprintf(hname, "lnsq_%i", i);

	TF1* lnsq = new TF1(hname, "sqrt([0]-x*x*[1]-[2])", 0, 1000);
	lnsq->SetParameter(0, (500 + 150 * (i - 1)) * (500 + 150 * (i - 1))
			/ coef2[i]);
	lnsq->SetParameter(1, 1. / coef2[i]);
	lnsq->SetParameter(2, -coef1 * 91 * 91 * (2 * TMath::Cos(TMath::ATan(
			tanBeta))) / coef2[i]);//--tanbeta=10 --> cos2beta = -99/101
	lnsq->SetLineWidth(1);

	lnsq->SetLineColor(kGray);

	return lnsq;
}

TF1* constant_gluino(int tanBeta, int i) {
	//---lines of constant gluino/squark
	double coef1 = 0.35;
	double coef2[] = { 5, 5, 4.6, 4.1 };

	char hname[200];

	sprintf(hname, "lngl_%i", i);

	TF1* lngl = new TF1(hname, "[0]+x*[1]", 0, 1000);
	lngl->SetParameter(0, (500 + 150. * (i - 1)) / 2.4);
	lngl->SetParameter(1, -40. / 1400);
	lngl->SetLineWidth(1);
	lngl->SetLineColor(kGray);

	return lngl;
}

TLatex* constant_squark_text(Int_t it, TF1& lnsq, Int_t tanBeta_) {
	char legnm[200];

	sprintf(legnm, "#font[92]{#tilde{q}(%i)GeV}", 500 + 150 * (it - 1));
	Double_t place_x = 170;
	if (tanBeta_ == 50)
		place_x = 290;
	TLatex* t3 = new TLatex(place_x + 10 * (it - 1), lnsq.Eval(place_x + 10
			* (it - 1)) + 5, legnm);
	t3->SetTextSize(0.02);
	t3->SetTextAngle(-8);
	t3->SetTextColor(kGray + 2);

	return t3;
}

TLatex* constant_gluino_text(Int_t it, TF1& lngl) {
	char legnm[200];

	sprintf(legnm, "#font[12]{#tilde{g}}#font[92]{(%i)GeV}", 500 + 150 * (it
			- 1));
	TLatex* t4 = new TLatex(423, 18 + lngl.Eval(480), legnm);
	t4->SetTextSize(0.02);
	t4->SetTextAlign(13);
	t4->SetTextColor(kGray + 2);

	return t4;
}

TLegend* makeStauLegend(Double_t txtsz, Int_t tanBeta_) {
	Double_t ypos_1 = 0.86;
	Double_t ypos_2 = 0.88;
	Double_t xpos_1 = 0.16;
	Double_t xpos_2 = 0.17;
	if (tanBeta_ == 50) {
		xpos_1 = 0.17;
		xpos_2 = 0.18;
		ypos_1 = 0.76;
		ypos_2 = 0.78;

	}
	TLegend* legst = new TLegend(xpos_1, ypos_1, xpos_2, ypos_2);
	legst->SetHeader("#tilde{#tau} = LSP");
	legst->SetFillStyle(0);
	legst->SetBorderSize(0);
	legst->SetTextSize(0.03);

	return legst;
}

TLegend* makeExpLegend(TGraph& sg_gr, TGraph& sgd_gr, TGraph& ch_gr,
		TGraph& sl_gr, TGraph& tev_sn, Double_t txtsz, Int_t tanbeta) {
	TLegend* legexp = new TLegend(0.64, 0.65, 0.99, 0.9, NULL, "brNDC");
	legexp->SetFillColor(0);
	legexp->SetShadowColor(0);
	legexp->SetTextSize(txtsz);
	legexp->SetBorderSize(0);

	sg_gr.SetLineColor(1);
	legexp->AddEntry(
			&sg_gr,
			"CDF  #tilde{#font[12]{g}}, #tilde{#font[12]{q}}, #scale[0.8]{tan#beta=5, #mu<0}",
			"f");
	//  sgd_gr.SetLineColor(1);
	//  sgd_gr.SetLineWidth(1);

	legexp->AddEntry(
			&sgd_gr,
			"D0   #tilde{#font[12]{g}}, #tilde{#font[12]{q}}, #scale[0.8]{tan#beta=3, #mu<0}",
			"f");
	ch_gr.SetLineColor(1);
	legexp->AddEntry(&ch_gr, "LEP2   #tilde{#chi}_{1}^{#pm}", "f");

	sl_gr.SetLineColor(1);
	if (tanbeta != 50)
		legexp->AddEntry(&sl_gr, "LEP2   #tilde{#font[12]{l}}^{#pm}", "f");
	if (tanbeta == 3)
		legexp->AddEntry(&tev_sn, "D0  #chi^{#pm}_{1}, #chi^{0}_{2}", "f");

	return legexp;

}

TGraph* getGraph(TH2F* h1, double level) {

	h1->SetContour(1);
	h1->SetContourLevel(0, level);
	h1->Draw("CONT LIST");
	gPad->Update();

	TObjArray* contours = (TObjArray*) gROOT->GetListOfSpecials()->FindObject(
			"contours");

	std::cout << "contours: " << contours << std::endl;
	// Draw contours
	TList* graphList = (TList*) (contours->At(0));
	std::cout << "number of graphs: " << graphList->GetSize() << std::endl;
	TGraph* myGraph = 0;
	for (int igraph = 0; igraph < graphList->GetSize(); ++igraph) {
		myGraph = (TGraph*) graphList->At(igraph);

		std::cout << " - graph " << igraph << " has " << myGraph->GetN()
				<< " points" << std::endl;
		if (myGraph->GetN() > 50) {
			std::cout << "Drawing " << myGraph->GetN() << " points"
					<< std::endl;
			//      myGraph->Print("all");
			//      myGraph->SetLineColor(9);
			//      myGraph->SetLineColor(46);
			//myGraph->SetLineWidth(3);
			//      myGraph->SetLineStyle(2);
			//      myGraph->SetMarkerStyle(20);
			//      myGraph->SetMarkerColor(9);

			//      myGraph->Draw("C");
			//      cvsSys->Update();

			//    TString graphName("graph"+name+"_");
			//    graphName += igraph;
			//    myGraph->SetName(graphName);
			break;
		}
	}
	return myGraph;
}

TH2F* fillHoles(TH2F* h2) {

	int nx = h2->GetNbinsX();
	int ny = h2->GetNbinsY();
	std::cout << "Nbins: " << nx << " " << ny << std::endl;

	for (int i = 1; i < nx + 1; i++) {
		for (int j = ny; j > 0; j--) {
			int pos = j;
			int count = 0;
			if (h2->GetBinContent(i, j) == 0 && h2->GetBinContent(i - 1, j)
					== -1 && h2->GetBinContent(i + 1, j) == -1)
				h2->SetBinContent(i, j, -1.);

			if (h2->GetBinContent(i, j) == 1 && h2->GetBinContent(i - 1, j)
					== -1 && h2->GetBinContent(i + 1, j) == -1
					&& h2->GetBinContent(i, j + 1) == -1 && h2->GetBinContent(
					i, j - 1) == -1)
				h2->SetBinContent(i, j, -1.);

			//      if (h2->GetBinContent(i,j)<0){
			if (h2->GetBinContent(i, j) < 0 && h2->GetBinContent(i, j - 1) < 0) {
				//      cout <<" bin content " << i <<" " <<  j<< " " << pos << " " << h2->GetBinContent(i,j) <<endl;
				for (int k = pos; k > 0; k--) {
					h2->SetBinContent(i, k, -1.);
				}
				break;
			}

		}
	}
	return h2;
}

//old------------------------------------------------------------------------

TGraph* sq_LEP() {//sq-gl
	double sq[] = { 0, 0, 100, 100 };
	double gl[] = { 0, 2000, 2000, 0 };
	TGraph* res = new TGraph(4, sq, gl);
	res->SetFillColor(kBlue);
	return res;
}
TGraph* gl_TEV() {//sq-gl
	double sq[] = { 0, 2000, 2000, 0 };
	double gl[] = { 0, 0, 190, 190 };
	TGraph* res = new TGraph(4, sq, gl);
	res->SetFillColor(kGreen + 2);
	return res;
}

TGraph* gl_CDF() {//sq-gl
	double sq[] = { 0, 2000, 2000, 0 };
	double gl[] = { 190, 190, 230, 230 };
	TGraph* res = new TGraph(4, sq, gl);
	res->SetFillColor(kOrange + 5);
	return res;
}

TGraph* gl_DEZ() {//sq-gl
	double sq[] = { 0, 2000, 2000, 0 };
	double gl[] = { 230, 230, 255, 255 };
	TGraph* res = new TGraph(4, sq, gl);
	res->SetFillColor(kYellow - 5);
	return res;
}

TGraph* gl_WHT() {//sq-gl
	double sq[] = { 101, 2000, 2000, 101 };
	double gl[] = { 256, 256, 400, 400 };
	TGraph* res = new TGraph(4, sq, gl);
	res->SetFillColor(kWhite);
	return res;
}

////////////////////////////////////////////
TGraph* gl_LEP() {//gl-sq
	double sq[] = { 0, 0, 100, 100 };
	double gl[] = { 0, 2000, 2000, 0 };
	TGraph* res = new TGraph(4, gl, sq);
	res->SetFillColor(kBlue);
	return res;
}

TGraph* sq_TEV() {//gl-sq
	double sq[] = { 0, 2000, 2000, 330, 250, 300, 200, 150, 100, 0 };
	double gl[] = { 0, 0, 190, 190, 260, 300, 500, 560, 500, 500 };

	TGraph* res = new TGraph(10, gl, sq);
	res->SetFillColor(kGreen + 2);
	return res;
}

TGraph* sq_CDF() {//gl-sq
	double sq[] = { 0, 2000, 2000, 480, 460, 420, 410, 380, 390, 290, 0 };
	double gl[] = { 0, 0, 280, 280, 300, 310, 330, 340, 440, 320, 320 };
	TGraph* res = new TGraph(11, gl, sq);
	res->SetFillColor(kOrange + 5);
	return res;
}
TGraph* sq_DEZ() {//gl-sq
	double sq[] = { 0, 2000, 2000, 460, 430, 400, 390, 290, 0 };
	double gl[] = { 0, 0, 305, 305, 320, 350, 440, 320, 320 };
	TGraph* res = new TGraph(9, gl, sq);
	res->SetFillColor(kYellow - 5);
	return res;
}

TGraph* glsq_NoSol() {//sq-gl
	gStyle->SetHatchesSpacing(2.0);
	gStyle->SetHatchesLineWidth(1);
	double sq[] = { 83, 83, 110, 1297.6, 0, 0 };
	double gl[] = { 0, 63, 120, 1466, 1466, 0 };
	TGraph* res = new TGraph(6, gl, sq);
	res->SetLineColor(1);
	res->SetLineWidth(2);
	res->SetFillStyle(3354);
	return res;
}

TGraph * Atlas_m0_m12_tb3_obs() {
	TGraph *graph = new TGraph(71);
	graph->SetLineWidth(2);
	graph->SetPoint(0, 54, 357.341);
	graph->SetPoint(1, 82, 358.4171);
	graph->SetPoint(2, 110, 359.8179);
	graph->SetPoint(3, 110.934, 359.875);
	graph->SetPoint(4, 138, 361.5302);
	graph->SetPoint(5, 166, 362.5696);
	graph->SetPoint(6, 189.0676, 359.875);
	graph->SetPoint(7, 194, 359.1415);
	graph->SetPoint(8, 222, 354.5211);
	graph->SetPoint(9, 248.1714, 351.625);
	graph->SetPoint(10, 250, 351.4583);
	graph->SetPoint(11, 278, 349.4991);
	graph->SetPoint(12, 306, 343.4051);
	graph->SetPoint(13, 306.1313, 343.375);
	graph->SetPoint(14, 334, 335.3855);
	graph->SetPoint(15, 334.7966, 335.125);
	graph->SetPoint(16, 350.8793, 326.875);
	graph->SetPoint(17, 362, 319.7823);
	graph->SetPoint(18, 364.6568, 318.625);
	graph->SetPoint(19, 383.5969, 310.375);
	graph->SetPoint(20, 390, 308.6978);
	graph->SetPoint(21, 415.0931, 302.125);
	graph->SetPoint(22, 418, 300.2992);
	graph->SetPoint(23, 423.9359, 293.875);
	graph->SetPoint(24, 431.0348, 285.625);
	graph->SetPoint(25, 441.6066, 277.375);
	graph->SetPoint(26, 446, 275.3629);
	graph->SetPoint(27, 468.0485, 269.125);
	graph->SetPoint(28, 474, 267.2393);
	graph->SetPoint(29, 486.3632, 260.875);
	graph->SetPoint(30, 495.8534, 252.625);
	graph->SetPoint(31, 502, 244.7684);
	graph->SetPoint(32, 502.3699, 244.375);
	graph->SetPoint(33, 508.424, 236.125);
	graph->SetPoint(34, 514.4781, 227.875);
	graph->SetPoint(35, 518.8059, 219.625);
	graph->SetPoint(36, 521.5056, 211.375);
	graph->SetPoint(37, 524.324, 203.125);
	graph->SetPoint(38, 527.1424, 194.875);
	graph->SetPoint(39, 530, 193.5052);
	graph->SetPoint(40, 552.5782, 186.625);
	graph->SetPoint(41, 558, 185.8767);
	graph->SetPoint(42, 586, 180.1476);
	graph->SetPoint(43, 592.2836, 178.375);
	graph->SetPoint(44, 614, 171.422);
	graph->SetPoint(45, 617.5927, 170.125);
	graph->SetPoint(46, 627.5396, 161.875);
	graph->SetPoint(47, 642, 157.9876);
	graph->SetPoint(48, 658.2277, 153.625);
	graph->SetPoint(49, 670, 151.1225);
	graph->SetPoint(50, 698, 146.9886);
	graph->SetPoint(51, 711.5425, 145.375);
	graph->SetPoint(52, 726, 143.6935);
	graph->SetPoint(53, 754, 140.0648);
	graph->SetPoint(54, 782, 137.4563);
	graph->SetPoint(55, 783.0426, 137.125);
	graph->SetPoint(56, 796.0326, 128.875);
	graph->SetPoint(57, 810, 125.0328);
	graph->SetPoint(58, 826.0235, 120.625);
	graph->SetPoint(59, 838, 117.3305);
	graph->SetPoint(60, 866, 116.7462);
	graph->SetPoint(61, 894, 116.1044);
	graph->SetPoint(62, 922, 114.6645);
	graph->SetPoint(63, 950, 116.937);
	graph->SetPoint(64, 978, 117.6956);
	graph->SetPoint(65, 1006, 115.198);
	graph->SetPoint(66, 1034, 113.1166);
	graph->SetPoint(67, 1038.967, 112.375);
	graph->SetPoint(68, 1062, 108.0997);
	graph->SetPoint(69, 1090, 104.373);
	graph->SetPoint(70, 1096.429, 104.125);
	graph->SetFillColor(0);
	return graph;
}
TGraph * Atlas_mGl_mSq_obs() {
	//x:gluino mass, y:squark mass
	TGraph *graph = new TGraph(64);
	graph->SetLineWidth(2);
	graph->SetPoint(0, 511.2569, 1976.25);
	graph->SetPoint(1, 513.125, 1936.181);
	graph->SetPoint(2, 513.4714, 1928.75);
	graph->SetPoint(3, 515.6859, 1881.25);
	graph->SetPoint(4, 517.9004, 1833.75);
	graph->SetPoint(5, 520.1148, 1786.25);
	graph->SetPoint(6, 522.3293, 1738.75);
	graph->SetPoint(7, 524.5438, 1691.25);
	graph->SetPoint(8, 526.1095, 1643.75);
	graph->SetPoint(9, 526.5267, 1596.25);
	graph->SetPoint(10, 526.9924, 1548.75);
	graph->SetPoint(11, 527.2745, 1501.25);
	graph->SetPoint(12, 526.9487, 1453.75);
	graph->SetPoint(13, 526.6228, 1406.25);
	graph->SetPoint(14, 532.902, 1358.75);
	graph->SetPoint(15, 540.182, 1311.25);
	graph->SetPoint(16, 547.4619, 1263.75);
	graph->SetPoint(17, 558.0438, 1216.25);
	graph->SetPoint(18, 561.875, 1203.002);
	graph->SetPoint(19, 579.4475, 1168.75);
	graph->SetPoint(20, 603.8166, 1121.25);
	graph->SetPoint(21, 610.625, 1107.979);
	graph->SetPoint(22, 630.8311, 1073.75);
	graph->SetPoint(23, 658.8712, 1026.25);
	graph->SetPoint(24, 659.375, 1025.691);
	graph->SetPoint(25, 703.8609, 978.75);
	graph->SetPoint(26, 708.125, 975.5966);
	graph->SetPoint(27, 756.875, 945.0332);
	graph->SetPoint(28, 778.5082, 931.25);
	graph->SetPoint(29, 805.625, 916.3566);
	graph->SetPoint(30, 847.5791, 883.75);
	graph->SetPoint(31, 854.375, 879.4212);
	graph->SetPoint(32, 903.125, 848.0476);
	graph->SetPoint(33, 927.7959, 836.25);
	graph->SetPoint(34, 951.875, 823.2943);
	graph->SetPoint(35, 1000.625, 790.1523);
	graph->SetPoint(36, 1004.488, 788.75);
	graph->SetPoint(37, 1049.375, 774.2142);
	graph->SetPoint(38, 1098.125, 755.1404);
	graph->SetPoint(39, 1130.904, 741.25);
	graph->SetPoint(40, 1146.875, 736.2226);
	graph->SetPoint(41, 1195.625, 720.3208);
	graph->SetPoint(42, 1244.375, 701.1234);
	graph->SetPoint(43, 1266.694, 693.75);
	graph->SetPoint(44, 1293.125, 685.9941);
	graph->SetPoint(45, 1341.875, 668.9126);
	graph->SetPoint(46, 1390.625, 652.8072);
	graph->SetPoint(47, 1439.375, 647.4204);
	graph->SetPoint(48, 1448.107, 646.25);
	graph->SetPoint(49, 1488.125, 638.5908);
	graph->SetPoint(50, 1536.875, 620.8084);
	graph->SetPoint(51, 1585.625, 603.026);
	graph->SetPoint(52, 1597.347, 598.75);
	graph->SetPoint(53, 1634.375, 581.9133);
	graph->SetPoint(54, 1683.125, 559.7463);
	graph->SetPoint(55, 1701.81, 551.25);
	graph->SetPoint(56, 1731.875, 537.5793);
	graph->SetPoint(57, 1780.625, 515.4123);
	graph->SetPoint(58, 1806.273, 503.75);
	graph->SetPoint(59, 1829.375, 493.2101);
	graph->SetPoint(60, 1878.125, 471.0784);
	graph->SetPoint(61, 1910.736, 456.25);
	graph->SetPoint(62, 1926.875, 448.241);
	graph->SetPoint(63, 1975.625, 426.7444);
	return graph;
}

//- Jim Lungu ----------------------------------------------

TGraph* Jim_mht_tb3(int mode) {
	Int_t n = 10;
	Double_t x[10] = { 15, 65, 115, 165, 215, 265, 315, 365, 415, 465 };
	Double_t yobs[10] = { 330, 330, 340, 340, 330, 320, 310, 300, 280, 260 };
	Double_t yexp[10] = { 330, 320, 320, 320, 320, 310, 300, 290, 270, 250 };
	Double_t yexpp[10] = { 350, 350, 350, 340, 340, 330, 330, 310, 300, 280 };
	Double_t yexpn[10] = { 310, 310, 310, 300, 300, 300, 280, 270, 240, 240 };
	Double_t y[10];
	for (int i = 0; i < n; i++) {
		if (mode == 0)
			y[i] = yobs[i];
		if (mode == 1)
			y[i] = yexp[i];
		if (mode == 2)
			y[i] = yexpp[i];
		if (mode == 3)
			y[i] = yexpn[i];
	}
	TGraph* gr = new TGraph(n, x, y);
	gr->SetLineWidth(2);
	if (mode == 1)
		gr->SetLineStyle(2);
	if (mode > 2) {
		gr->SetFillColor(8);
		gr->SetLineColor(8);
	} else
		gr->SetLineColor(2);
	return gr;
}

TGraph* Jim_ht_tb3(int mode) {
	Int_t n = 10;
	Double_t x[10] = { 15, 65, 115, 165, 215, 265, 315, 365, 415, 465 };
	Double_t yobs[10] = { 300, 300, 290, 290, 300, 290, 280, 270, 260, 250 };
	Double_t yexp[10] = { 300, 300, 290, 300, 300, 290, 280, 270, 260, 250 };
	Double_t yexpp[10] = { 320, 320, 320, 320, 310, 310, 300, 290, 280, 270 };
	Double_t yexpn[10] = { 290, 290, 280, 290, 280, 280, 270, 250, 250, 240 };
	Double_t y[10];
	for (int i = 0; i < n; i++) {
		if (mode == 0)
			y[i] = yobs[i];
		if (mode == 1)
			y[i] = yexp[i];
		if (mode == 2)
			y[i] = yexpp[i];
		if (mode == 3)
			y[i] = yexpn[i];
	}
	TGraph* gr = new TGraph(n, x, y);
	gr->SetLineWidth(2);
	if (mode == 1)
		gr->SetLineStyle(2);
	if (mode > 2) {
		gr->SetFillColor(8);
		gr->SetLineColor(8);
	} else
		gr->SetLineColor(2);
	return gr;
}

TGraph* Jim_mht_tb50(int mode) {
	Int_t n = 9;
	Double_t x[9] = { 205, 255, 305, 355, 405, 455, 505, 555, 605 };
	Double_t yobs[9] = { 330, 330, 310, 300, 300, 270, 250, 240, 210 };
	Double_t yexp[9] = { 320, 320, 300, 290, 280, 250, 240, 230, 210 };
	Double_t yexpp[9] = { 350, 340, 330, 320, 310, 290, 260, 250, 230 };
	Double_t yexpn[9] = { 300, 300, 290, 280, 260, 240, 230, 200, 190 };
	Double_t y[9];
	for (int i = 0; i < n; i++) {
		if (mode == 0)
			y[i] = yobs[i];
		if (mode == 1)
			y[i] = yexp[i];
		if (mode == 2)
			y[i] = yexpp[i];
		if (mode == 3)
			y[i] = yexpn[i];
	}
	TGraph* gr = new TGraph(n, x, y);
	gr->SetLineWidth(2);
	if (mode == 1)
		gr->SetLineStyle(2);
	if (mode > 2) {
		gr->SetFillColor(8);
		gr->SetLineColor(8);
	} else
		gr->SetLineColor(2);
	return gr;
}

TGraph* Jim_ht_tb50(int mode) {
	Int_t n = 9;
	Double_t x[9] = { 205, 255, 305, 355, 405, 455, 505, 555, 605 };
	Double_t yobs[9] = { 300, 290, 280, 280, 260, 250, 230, 220, 220 };
	Double_t yexp[9] = { 300, 290, 280, 280, 260, 250, 240, 230, 220 };
	Double_t yexpp[9] = { 320, 310, 300, 290, 290, 270, 260, 250, 240 };
	Double_t yexpn[9] = { 280, 270, 270, 260, 250, 240, 230, 220, 210 };
	Double_t y[10];
	for (int i = 0; i < n; i++) {
		if (mode == 0)
			y[i] = yobs[i];
		if (mode == 1)
			y[i] = yexp[i];
		if (mode == 2)
			y[i] = yexpp[i];
		if (mode == 3)
			y[i] = yexpn[i];
	}
	TGraph* gr = new TGraph(n, x, y);
	gr->SetLineWidth(2);
	if (mode == 1)
		gr->SetLineStyle(2);
	if (mode > 2) {
		gr->SetFillColor(8);
		gr->SetLineColor(8);
	} else
		gr->SetLineColor(2);
	return gr;
}

TGraph* Jim_mht_tb10(int mode) {
	Int_t n = 20;
	Double_t x[20] = { 15, 65, 115, 165, 215, 265, 315, 365, 415, 465, 515,
			565, 615, 665, 715, 765, 815, 865, 915, 965 };
	Double_t yobs[20] = { 330, 340, 330, 330, 330, 320, 310, 280, 280, 260,
			240, 230, 220, 220, 210, 190, 190, 180, 180, 180 };
	Double_t yexp[20] = { 330, 320, 320, 320, 320, 310, 300, 280, 270, 250,
			240, 220, 220, 200, 190, 190, 180, 160, 170, 160 };
	Double_t yexpp[20] = { 330, 340, 350, 340, 330, 330, 330, 310, 300, 280,
			270, 240, 240, 230, 220, 210, 200, 180, 190, 180 };
	Double_t yexpn[20] = { 290, 280, 310, 300, 300, 290, 280, 270, 250, 240,
			220, 210, 200, 190, 160, 170, 160, 150, 150, 140 };
	Double_t y[20];
	for (int i = 0; i < n; i++) {
		if (mode == 0)
			y[i] = yobs[i];
		if (mode == 1)
			y[i] = yexp[i];
		if (mode == 2)
			y[i] = yexpp[i];
		if (mode == 3)
			y[i] = yexpn[i];
	}
	TGraph* gr = new TGraph(n, x, y);
	gr->SetLineWidth(2);
	if (mode == 1)
		gr->SetLineStyle(2);
	if (mode > 2) {
		gr->SetFillColor(8);
		gr->SetLineColor(8);
	} else
		gr->SetLineColor(2);
	return gr;
}

TGraph* Jim_ht_tb10(int mode) {
	Int_t n = 20;
	Double_t x[20] = { 15, 65, 115, 165, 215, 265, 315, 365, 415, 465, 515,
			565, 615, 665, 715, 765, 815, 865, 915, 965 };
	Double_t yobs[20] = { 290, 280, 300, 290, 300, 290, 280, 270, 250, 250,
			240, 230, 220, 220, 210, 190, 200, 180, 190, 180 };
	Double_t yexp[20] = { 290, 280, 290, 300, 300, 290, 280, 270, 250, 250,
			240, 240, 220, 220, 210, 190, 200, 180, 190, 180 };
	Double_t yexpp[20] = { 330, 320, 310, 310, 310, 310, 300, 280, 280, 260,
			260, 250, 240, 230, 220, 220, 210, 210, 200, 190 };
	Double_t yexpn[20] = { 290, 280, 280, 280, 280, 280, 270, 260, 240, 240,
			230, 220, 200, 200, 190, 190, 180, 180, 170, 170 };
	Double_t y[20];
	for (int i = 0; i < n; i++) {
		if (mode == 0)
			y[i] = yobs[i];
		if (mode == 1)
			y[i] = yexp[i];
		if (mode == 2)
			y[i] = yexpp[i];
		if (mode == 3)
			y[i] = yexpn[i];
	}
	TGraph* gr = new TGraph(n, x, y);
	gr->SetLineWidth(2);
	if (mode == 1)
		gr->SetLineStyle(2);
	if (mode > 2) {
		gr->SetFillColor(8);
		gr->SetLineColor(8);
	} else
		gr->SetLineColor(2);
	return gr;
}

