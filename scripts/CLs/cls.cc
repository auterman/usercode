//system
#include <iostream>
#include <cmath>
//root
#include "TCanvas.h"
#include "TH1F.h"
#include "TF1.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TRandom3.h"
#include "TGraph.h"
#include "TLatex.h"
#include "TLine.h"
#include "TLimit.h"
#include "TLimitDataSource.h"
#include "TConfidenceLevel.h"

int _plotindex=0;
TH1F * MakeBackground(int evts=1000, int bins=40, double min=100, double max=500)
{
  char * name = new char[100];
  sprintf(name,"backgd%d",_plotindex++);
  TH1F * result = new TH1F(name,";(pseudo) MET [GeV];(pseudo) events",bins, min,max);
  result->Sumw2();
  TF1  *fa = new TF1("fa","1.0*exp(-0.01*x)",100,500);
  for (int i=0; i<evts; ++i)
    result->Fill( fa->GetRandom(), 0.1 );
  return result;    
}

TH1F * MakeSignal(double mass, double width, int evts=100, int bins=40, double min=100, double max=500)
{
  char * name = new char[100];
  sprintf(name,"signal_m%f_%d",mass,_plotindex++);
  TH1F * result = new TH1F(name,";(pseudo) MET [GeV];(pseudo) events",bins, min,max);
  result->Sumw2();
  sprintf(name,"fb_m%f",mass);
  TF1  *fb = new TF1(name,"gaus(0)",100,500);
  fb->SetParameter(0,1);
  fb->SetParameter(1, mass );
  fb->SetParameter(2, width );
  for (int i=0; i<evts; ++i)
    result->Fill( fb->GetRandom(), 0.1 );
  return result;    

}

TH1F * MakeData(TH1F*b, TH1F*s=0)
{
  char * name = new char[100];
  sprintf(name,"data%d",_plotindex++);
  TH1F * result = new TH1F(name,";(pseudo) MET [GeV];(pseudo) events",
                           b->GetNbinsX(), b->GetXaxis()->GetXmin(),b->GetXaxis()->GetXmax());
  TRandom3 * rand = new TRandom3(0);
  for (int i=0; i<=b->GetNbinsX(); ++i){
    double entry;
    if (s==0) 
      entry = rand->Poisson( b->GetBinContent(i) );
    else  
      entry = rand->Poisson( b->GetBinContent(i)+s->GetBinContent(i) );
    result->SetBinContent(i,entry);      
    result->SetBinError(i,sqrt(entry));      
  }      
  return result;    
}

int main()
{
   gStyle->SetHistFillColor(0);
   gStyle->SetPalette(1);
   //gStyle->SetFillColor(0);
   //gStyle->SetOptStat(kFALSE);
   gStyle->SetCanvasColor(0);
   gStyle->SetCanvasBorderMode(0);
   gStyle->SetPadColor(0);
   gStyle->SetPadBorderMode(0);
   gStyle->SetFrameBorderMode(0);

   gStyle->SetTitleFillColor(0);
   gStyle->SetTitleBorderSize(0);
   gStyle->SetTitleX(0.10);
   gStyle->SetTitleY(0.98);
   gStyle->SetTitleW(0.8);
   gStyle->SetTitleH(0.06);

   gStyle->SetErrorX(0);
   gStyle->SetStatColor(0);
   gStyle->SetStatBorderSize(0);
   gStyle->SetStatX(0);
   gStyle->SetStatY(0);
   gStyle->SetStatW(0);
   gStyle->SetStatH(0);

   gStyle->SetTitleFont(22);
   gStyle->SetLabelFont(22,"X");
   gStyle->SetLabelFont(22,"Y");
   gStyle->SetLabelFont(22,"Z");
   gStyle->SetLabelSize(0.03,"X");
   gStyle->SetLabelSize(0.03,"Y");
   gStyle->SetLabelSize(0.03,"Z");

   TH1F * backgd = MakeBackground(5000);
   TH1F * signal = MakeSignal(300,sqrt(300), 200);
   TH1F * data   = MakeData( backgd );//, signal );
   
   //Draw MET for background, signal & data:
   TCanvas * c1 = new TCanvas("","",600,600);
   signal->SetLineColor(2);
   data->SetMarkerStyle(8);
   backgd->Draw("h");
   signal->Draw("h,same");
   data->Draw("pe,same");
   c1->SetLogy(1);
   c1->SaveAs("backgd.eps");
   
   //cout the CL's:
   int fNMC = 50000;
   TLimitDataSource* mydatasource = new TLimitDataSource(signal,backgd,data);
   TConfidenceLevel *myconfidence = TLimit::ComputeLimit(mydatasource,fNMC);
   std::cout << "  CLs    : " << myconfidence->CLs()  << "\n"
             << "  CLsb   : " << myconfidence->CLsb() << "\n"
             << "  CLb    : " << myconfidence->CLb()  << "\n"
             << "< CLs >  : " << myconfidence->GetExpectedCLs_b()  << "\n"
             << "< CLsb > : " << myconfidence->GetExpectedCLsb_b() << "\n"
             << "< CLb >  : " << myconfidence->GetExpectedCLb_b()  << "\n"
	     << " -2 ln Q : " << myconfidence->GetStatistic() << "\n"
	     << std::endl;
   
   //Draw the expected and observed test statistics:
   TH1F h("TConfidenceLevel_Draw","",50,0,0);
   Int_t i;
   double * fTSB = myconfidence->GetTestStatistic_B();
   double * fTSS = myconfidence->GetTestStatistic_SB();
   for (i=0; i<fNMC; i++) {
      h.Fill(-2*(fTSB[i]-myconfidence->GetStot() ));
      h.Fill(-2*(fTSS[i]-myconfidence->GetStot() ));
   }
   TH1F* b_hist  = new TH1F("b_hist", ";-2 ln Q;normalized p.d.f.",50,h.GetXaxis()->GetXmin(),h.GetXaxis()->GetXmax());
   TH1F* sb_hist = new TH1F("sb_hist",";-2 ln Q;normalized p.d.f.",50,h.GetXaxis()->GetXmin(),h.GetXaxis()->GetXmax());
   for (i=0; i<fNMC; i++) {
      b_hist->Fill(-2*(fTSB[i]-myconfidence->GetStot() ));
      sb_hist->Fill(-2*(fTSS[i]-myconfidence->GetStot() ));
   }
   b_hist->Scale(1.0/b_hist->Integral());
   sb_hist->Scale(1.0/sb_hist->Integral());
   b_hist->GetYaxis()->SetTitleOffset(1.3);
   b_hist->SetMinimum(0.001);
   b_hist->SetMaximum(3*b_hist->GetMaximum());
   b_hist->SetLineWidth(3);
   sb_hist->SetLineWidth(3);
   b_hist->SetLineColor(kBlue);
   sb_hist->SetLineColor( 28 );
   TLine * line = new TLine(myconfidence->GetStatistic(),b_hist->GetMinimum(),
                            myconfidence->GetStatistic(),0.6*b_hist->GetMaximum() );
   line->SetLineWidth(3);			    
   TLegend * leg = new TLegend(0.11,0.78,0.48,0.89);
   leg->SetBorderSize(0);
   leg->SetFillColor(0);
   leg->AddEntry(line,"Observed ","l");
   leg->AddEntry(b_hist,"Expected background-only ","l");
   leg->AddEntry(sb_hist,"Expected signal+background ","l");
   b_hist->Draw("h");
   line->Draw();
   leg->Draw("same");
   sb_hist->Draw("h,same");
   c1->SaveAs("conf.eps");

   //CLs vs mass:
   int nmass = 40;
   TH1F * cls_obs = new TH1F("cls_obs", ";(pseudo) mass [GeV];CLs",nmass,100,500);
   TH1F * cls_exp = new TH1F("cls_exp", ";(pseudo) mass [GeV];CLs",nmass,100,500);
   TH1F * lnQ_obs    = new TH1F("lnQ_obs",    ";(pseudo) mass [GeV];-2 ln Q",nmass,100,500);
   TH1F * lnQ_exp_b  = new TH1F("lnQ_exp_b",  ";(pseudo) mass [GeV];-2 ln Q",nmass,100,500);
   TH1F * lnQ_exp_sb = new TH1F("lnQ_exp_sb", ";(pseudo) mass [GeV];-2 ln Q",nmass,100,500);
   double y_cls_exp1[nmass*2];
   double x_cls_exp[nmass*2];
   double y_cls_exp2[nmass*2];
   double y_lnQ_exp1[nmass*2];
   double y_lnQ_exp2[nmass*2];
   fNMC = 2000;

   for (int i=0; i<=nmass; ++i){
   
     TH1F sig( *signal );
     sig.Scale( 4*exp(-i/8.) );
     std::cout << i*10 << " GeV:  n_signal = "<<sig.Integral()<<std::endl;
  
     TLimitDataSource * source = new TLimitDataSource(&sig,backgd,data);
     TConfidenceLevel * conf = TLimit::ComputeLimit(source,fNMC);
    
     cls_obs->SetBinContent(i, conf->CLs() );
     cls_exp->SetBinContent(i, conf->GetExpectedCLs_b() );
     y_cls_exp1[i] = conf->GetExpectedCLs_b( 1);
     y_cls_exp1[nmass*2-i] = conf->GetExpectedCLs_b(-1);
     y_cls_exp2[i] = conf->GetExpectedCLs_b( 2);
     y_cls_exp2[nmass*2-i] = conf->GetExpectedCLs_b(-2);
     x_cls_exp[i] = 100+10*i;
     x_cls_exp[nmass*2-i] = 100+10*i;
 
     lnQ_obs->SetBinContent( i, conf->GetStatistic() );
     lnQ_exp_b->SetBinContent( i, conf->GetExpectedStatistic_b() );
     lnQ_exp_sb->SetBinContent( i, conf->GetExpectedStatistic_sb() );
     y_lnQ_exp1[i] = conf->GetExpectedStatistic_b(1);
     y_lnQ_exp1[nmass*2-i] = conf->GetExpectedStatistic_b(-1);
     y_lnQ_exp2[i] = conf->GetExpectedStatistic_b(2);
     y_lnQ_exp2[nmass*2-i] = conf->GetExpectedStatistic_b(-2);
   
     delete conf;
     delete source;
   }
   TGraph * cls_exp1 = new TGraph(2*nmass,x_cls_exp,y_cls_exp1);
   TGraph * cls_exp2 = new TGraph(2*nmass,x_cls_exp,y_cls_exp2);
   cls_exp2->SetLineColor(5);
   cls_exp2->SetFillColor(5);
   cls_exp1->SetLineColor(3);
   cls_exp1->SetFillColor(3);
   cls_obs->SetLineColor(kRed);
   cls_exp->SetLineColor(kBlue);
   cls_exp->SetLineStyle(3);
   cls_exp->SetLineWidth(3);
   cls_obs->SetLineWidth(3);
   cls_obs->SetMinimum(0.00001);
   TLegend * lg = new TLegend(0.5,0.28,0.89,0.4);
   lg->SetBorderSize(0);
   lg->SetFillColor(0);
   lg->AddEntry(cls_obs,"Observed ","l");
   lg->AddEntry(cls_exp,"Expected background-only ","l");
   cls_obs->Draw("c");
   lg->Draw("same");
   cls_exp2->Draw("lf,same");
   cls_exp1->Draw("lf,same");
   cls_exp->Draw("c,same");
   cls_obs->Draw("c,same");
   line->SetLineWidth(2);
   line->DrawLine(100,0.05,500,0.05);
   c1->SaveAs("cls.eps");

   
   //-2 ln Q vs mass:
   TGraph * lnQ_exp1 = new TGraph(2*nmass,x_cls_exp,y_lnQ_exp1);
   TGraph * lnQ_exp2 = new TGraph(2*nmass,x_cls_exp,y_lnQ_exp2);
   lnQ_exp2->SetLineColor(5);
   lnQ_exp2->SetFillColor(5);
   lnQ_exp1->SetLineColor(3);
   lnQ_exp1->SetFillColor(3);
   c1->SetLogy(0);
   lnQ_obs->SetLineColor(kRed);
   lnQ_obs->SetLineWidth(3);
   lnQ_exp_b->SetLineColor(kBlue);
   lnQ_exp_b->SetLineStyle(3);
   lnQ_exp_b->SetLineWidth(3);
   lnQ_exp_sb->SetLineColor( 28 );
   lnQ_exp_sb->SetLineStyle(3);
   lnQ_exp_sb->SetLineWidth(3);
   lnQ_obs->SetMinimum(-20);
   line->SetLineWidth(1);
   lnQ_obs->Draw("c");
   lnQ_exp2->Draw("lf,same");
   lnQ_exp1->Draw("lf,same");
   lnQ_obs->Draw("c,same");
   line->DrawLine(100,0.0,500,0.0);
   lnQ_exp_b->Draw("c,same");
   lnQ_exp_sb->Draw("c,same");
   c1->SaveAs("lnQ.eps");
   
   return 0;
}
