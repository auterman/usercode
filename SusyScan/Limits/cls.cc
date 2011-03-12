//root
#include "TCanvas.h"
#include "TPostScript.h"
#include "TH1D.h"
#include "TF1.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TRandom3.h"
#include "TGraph.h"
#include "TLatex.h"
#include "TLine.h"
#include "TArrow.h"
#include "TLimit.h"
#include "TLimitDataSource.h"
#include "TConfidenceLevel.h"
#include "TVectorT.h"

//User
#include "cls.h"
#include "solve.h"
#include "table.h"
#include "significance.h"
#include "ConfigFile.h"
//system
#include <iostream>
#include <iomanip>
#include <cmath>
#include <assert.h>
#include <fstream>
#include <time.h>

cls::cls():
    plotindex_(0),fNMC_(5000),outputfilename_("cls.ps"),signal_(0),backgd_(0),data_(0)
{
  stat_=false;syst_=false;
  do1DScan_ = false;
  if (!data_ && signal_ && backgd_) {
    data_ = (TH1D*)MakePseudoData( backgd_ );//, signal_ );
    isPseudoData = true;
  } else 
    isPseudoData = false;
}

cls::cls(std::string n,TH1*s,TH1*b,TH1*d):
    plotindex_(0),fNMC_(50000),outputfilename_(n),signal_(s),backgd_(b),data_(d)
{
  stat_=false;syst_=false;
  do1DScan_ = false;
  if (!data_ && signal_ && backgd_) {
    data_ = (TH1D*)MakePseudoData( backgd_ );//, signal_ );
    isPseudoData = true;
  } else 
    isPseudoData = false;
}

cls::cls(std::string n,std::string ScanParName,std::vector<double>ScanPar,std::vector<TH1*>s,TH1*b,TH1*d):
    plotindex_(0),fNMC_(100000),outputfilename_(n),ScanParName_(ScanParName),ScanPar_(ScanPar),backgd_(b),data_(d)
{
  stat_=true;syst_=false;
  signals_  = s;
  signal_   = (signals_.size()>0 ? signals_[0] : 0);
  do1DScan_ = (signals_.size()>1 ? true : false);
  
  if (!data_ && signal_ && backgd_) {
    data_ = (TH1D*)MakePseudoData( backgd_ );//, signal_ );
    isPseudoData = true;
  } else 
    isPseudoData = false;
}


void cls::GetXandYbyInterpolation( const double x, TH1D *& sig)
{
   ///naive linear interpolation for each bin contents and bin error:
   sig=0;
   double xmin=-99999, xmax=999999;
   int imin, imax;
   for (std::vector<double>::const_iterator it=ScanPar_.begin();it!=ScanPar_.end();++it){
     if (*it<=x && xmin<*it) { xmin= *it; imin=it-ScanPar_.begin(); continue;}
     if (*it>=x && xmax>*it) { xmax= *it; imax=it-ScanPar_.begin(); }
   }
   if (xmin<0) return;
   sig = (TH1D*)signals_[imin]->Clone();
   if (xmax>99999||xmin==xmax) return;
   TH1D * SIG = (TH1D*)signals_[imax]->Clone();
   
   for (int i=1; i<=sig->GetXaxis()->GetNbins(); ++i){//avoid under- and overflow bins
//std::cout <<"sig_left: x="<<xmin<<", y="<<sig->GetBinContent(i);
     double k = (SIG->GetBinContent(i)-sig->GetBinContent(i))/(xmax-xmin);
     double kerr = (SIG->GetBinError(i)-sig->GetBinError(i))/(xmax-xmin);
     sig->SetBinContent(i,sig->GetBinContent(i)+k*(x-xmin));
     sig->SetBinError(i,sig->GetBinError(i)+kerr*(x-xmin));
//std::cout <<";  sig_new: x="<<x<<", y="<<sig->GetBinContent(i)
//          <<";  sig_right: x="<<xmax<<", y="<<SIG->GetBinContent(i)
//	  <<std::endl;
   }
   delete SIG;
}



double cls::operator()(const double x, const double * par)
{
   double cl;
   TH1D *sig=0;
   if (do1DScan_) {
     GetXandYbyInterpolation(x, sig);
     if (!sig) return 99999;
   }
   else {
     sig = (TH1D*)signal_->Clone();
     sig->Scale( x );
   }
   TLimitDataSource * source = new TLimitDataSource(sig,(TH1D*)backgd_,(TH1D*)data_);
   TConfidenceLevel * confl = TLimit::ComputeLimit(source,fNMC_,stat_);
   if (par[1]) cl = confl->CLs();
   else        cl = confl->GetExpectedCLs_b();
   delete confl;
   delete sig;
   delete source;
   return cl - par[0]; //par[0] is the requested Confidence Level (e.g. 0.05)
}

double cls::GetObservedXsecLimit(double cl, double min, double max)
{
  double par[] = {cl,true};
  return TSolve<cls>( this, &cls::operator(), par, min, max)();
}
double cls::GetExpectedXsecLimit(double cl, double min, double max)
{
  double par[] = {cl,false};
  return TSolve<cls>( this, &cls::operator(), par, min, max)();
}

TH1 * cls::MakePseudoData(TH1 const *b, TH1 const *s)
{
  char * name = new char[100];
  sprintf(name,"data%d",plotindex_++);
  char * title = new char[128];
  sprintf(title,";%s;events",b->GetXaxis()->GetTitle());
  TH1 * result = (TH1D*)b->Clone( title );
  TRandom3 * rand = new TRandom3(0);
  for (int i=0; i<=b->GetNbinsX(); ++i){
    double entry;
    double uncert;      
    if (s==0) {
      entry  = rand->Poisson( b->GetBinContent(i) );
      uncert = rand->Gaus(0, b->GetBinError(i));
    }  
    else {
      entry = rand->Poisson( b->GetBinContent(i)+s->GetBinContent(i) );
      uncert = rand->Gaus(0, b->GetBinError(i)+s->GetBinError(i));
    }  
    result->SetBinContent(i,entry+uncert);
    result->SetBinError(i,sqrt(entry+uncert));
  }
  delete name;
  delete title;
  delete rand;
  return result;
}

void cls::Draw(bool doeps)
{
  if (do1DScan_) DrawVsSignalParam(doeps);
  else DrawVsXsec(doeps);
}

void cls::Sort(double &v1, double &v2)
{
  if (v1>v2){
    double temp(v1);
    v1=v2;
    v2=temp;
  }
}

void cls::DrawVsSignalParam(bool doeps)
{
   gStyle->SetHistFillColor(0);
   gStyle->SetPalette(1);
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

   if (!backgd_ || !signal_) 
   {
     std::cerr << "ERROR! &(background histogram)="<<backgd_
               << ", &(signal histogram)="<<signal_<<std::endl;
     return;	       
   }

   //Draw MET for background, signal & data:
   TCanvas * c1 = new TCanvas("","",600,600);
   TPostScript * ps = new TPostScript( outputfilename_.c_str(),111);

   //CLs vs x-section:
   //int n_points = signals_.size();
   int n_points = 51;
   double min = 999999.;
   double max = -99999.;
   for (std::vector<double>::const_iterator it=ScanPar_.begin();it!=ScanPar_.end();++it){
     if (*it<min) min = *it;
     if (*it>max) max = *it;
   }  
   --n_points;

   TLine * line = new TLine( );
   
   char * title = new char[256];
   sprintf(title,";%s;CLs",ScanParName_.c_str());
   TH1D * axis_cls  = new TH1D("axis", title,0,min,max);
   TGraph * cls_obs = new TGraph(1);
   TGraph * cls_exp = new TGraph(1);
   sprintf(title,";%s;-2 ln Q",ScanParName_.c_str());
   TH1D * axis_lnQ     = new TH1D("-2lnQ_obs",    title,0,min,max);
   TGraph * lnQ_obs    = new TGraph(1);
   TGraph * lnQ_exp_b  = new TGraph(1);
   TGraph * lnQ_exp_sb = new TGraph(1);
   double y_cls_exp1[n_points*2+2];
   double x_cls[n_points*2+2];
   double y_cls_exp2[n_points*2+2];
   double y_lnQ_exp1[n_points*2+2];
   double y_lnQ_exp2[n_points*2+2];
   double x_lnQ[n_points*2+2];
   int i_cls=0; // i_lnQ=0;


   TH1D * backgd = (TH1D*)backgd_->Clone();
   TH1D * data   = (TH1D*)data_->Clone();
   data->SetMarkerStyle(8);
   backgd->SetTitle("");

   TTable limits("Limits of parameter scan","|");
   limits.AddColumn<double>(ScanParName_);
   limits.AddColumn<double>(" #s ");
   limits.AddColumn<double>("+-ds");
   limits.AddColumn<double>("CL_s ");limits.SetPrecision(3,5);
   limits.AddColumn<double>("CL_sb");limits.SetPrecision(4,5);
   limits.AddColumn<double>("CL_b ");limits.SetPrecision(5,5);
   limits.AddColumn<double>("<CL_sb>_b");limits.SetPrecision(6,5);
   limits.AddColumn<double>("<CL_b>_b");limits.SetPrecision(7,5);
   limits.AddColumn<double>("<CL_s>_b-2s");limits.SetPrecision(8,5);
   limits.AddColumn<double>("<CL_s>_b-1s");limits.SetPrecision(9,5);
   limits.AddColumn<double>("<CL_s>_b");limits.SetPrecision(10,5);
   limits.AddColumn<double>("<CL_s>_b+1s");limits.SetPrecision(11,5);
   limits.AddColumn<double>("<CL_s>_b+2s");limits.SetPrecision(12,5);
   limits.AddColumn<double>("-2lnQ");
   limits.AddColumn<double>("<lnQ_b>");
   limits.AddColumn<double>("<lnQ_sb>");

   std::cout << "Scanning " << ScanParName_ << " in " << n_points << " steps from " << min << " to "<< max << ":"<<std::endl;
   for (int i=0; i<=n_points; ++i){
     
     double x;
     TH1D * signal;
     
     if (n_points-1==(int)signals_.size()){
       x = ScanPar_[i];
       signal = (TH1D*)signals_[i]->Clone();
     } else {
       x = min + (max-min)/(double)(n_points)*(double)i;
       GetXandYbyInterpolation( x, signal);  
     }     
     
     if (!signal) { std::cout << "Error at x="<<x<<"; skipping..."<< std::endl; continue;}
     
     signal->SetLineColor(2);
     signal->SetLineWidth(3);
     c1->SetLogy(0);
     if (!i) backgd->GetXaxis()->SetTitle( signal->GetXaxis()->GetTitle() );
     backgd->Draw("h");
     signal->Draw("h,same");
     data->Draw("pe,same");
     c1->Update();
     ps->NewPage();

     //cout the CL's:
     TLimitDataSource* mydatasource = new TLimitDataSource(signal,backgd,data);
     TConfidenceLevel *myconfidence = TLimit::ComputeLimit(mydatasource,fNMC_,stat_);

     double cls = myconfidence->CLs();
     double cls_b = myconfidence->GetExpectedCLs_b();
     double cls_b_p1 = myconfidence->GetExpectedCLs_b(1);
     double cls_b_n1 = myconfidence->GetExpectedCLs_b(-1);
     double cls_b_p2 = myconfidence->GetExpectedCLs_b(2);
     double cls_b_n2 = myconfidence->GetExpectedCLs_b(-2);
     Sort(cls_b_n1, cls_b_p1);
     Sort(cls_b_n2, cls_b_p2);

     double ds=0, db=0;
     for (int a=0; a<signal->GetNbinsX()+1; ++a) ds+=signal->GetBinError(a);
     for (int a=0; a<backgd->GetNbinsX()+1; ++a) db+=backgd->GetBinError(a);

     limits.Fill(x,
                 signal->Integral(),ds,
                 backgd->Integral(),db,
		 cls,myconfidence->CLsb(),myconfidence->CLb(),
                 myconfidence->GetExpectedCLsb_b(),myconfidence->GetExpectedCLb_b(),
		 //myconfidence->GetExpectedStatistic_b(1),myconfidence->GetExpectedStatistic_b(-1),myconfidence->GetExpectedStatistic_b(2),myconfidence->GetExpectedStatistic_b(-2),		 
		 cls_b_n2,cls_b_n1,cls_b,cls_b_p1,cls_b_p2,
		 myconfidence->GetStatistic(),myconfidence->GetExpectedStatistic_b(),myconfidence->GetExpectedStatistic_sb()
                );

     //Draw the expected and observed test statistics:
     c1->SetLogy(1);
     TH1D h("TConfidenceLevel_Draw","",50,0,0);
     double * fTSB = myconfidence->GetTestStatistic_B();
     double * fTSS = myconfidence->GetTestStatistic_SB();
     for (int j=0; j<fNMC_; j++) {
	h.Fill(-2*(fTSB[j]-myconfidence->GetStot() ));
	h.Fill(-2*(fTSS[j]-myconfidence->GetStot() ));
     }
     sprintf(title,"b_hist_%d",plotindex_++);
     TH1D* b_hist  = new TH1D(title, ";-2 ln Q;normalized p.d.f.",50,h.GetXaxis()->GetXmin(),h.GetXaxis()->GetXmax());
     sprintf(title,"sb_hist_%d",plotindex_++);
     TH1D* sb_hist = new TH1D(title,";-2 ln Q;normalized p.d.f.",50,h.GetXaxis()->GetXmin(),h.GetXaxis()->GetXmax());
     for (int j=0; j<fNMC_; j++) {
	b_hist->Fill(-2*(fTSB[j]-myconfidence->GetStot() ));
	sb_hist->Fill(-2*(fTSS[j]-myconfidence->GetStot() ));
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
     line->SetLineWidth(3);			    
     TLegend * leg = new TLegend(0.11,0.78,0.48,0.89);
     leg->SetBorderSize(0);
     leg->SetFillColor(0);
     leg->AddEntry(line,"Observed ","l");
     leg->AddEntry(b_hist,"Expected background-only ","l");
     leg->AddEntry(sb_hist,"Expected signal+background ","l");
     b_hist->Draw("h");
     line->DrawLine(myconfidence->GetStatistic(),b_hist->GetMinimum(),
                    myconfidence->GetStatistic(),0.6*b_hist->GetMaximum());
     leg->Draw("same");
     sb_hist->Draw("h,same");
     c1->Update();
     ps->NewPage();

     //Fill Test-Statistics hists
     lnQ_obs->SetPoint( i, x, myconfidence->GetStatistic() );
     lnQ_exp_b->SetPoint( i, x, myconfidence->GetExpectedStatistic_b() );
     lnQ_exp_sb->SetPoint( i, x, myconfidence->GetExpectedStatistic_sb() );
     y_lnQ_exp1[i] = myconfidence->GetExpectedStatistic_b(1);
     y_lnQ_exp1[2*n_points-i+1] = myconfidence->GetExpectedStatistic_b(-1);
     y_lnQ_exp2[i] = myconfidence->GetExpectedStatistic_b(2);
     y_lnQ_exp2[2*n_points-i+1] = myconfidence->GetExpectedStatistic_b(-2);
     x_lnQ[i] = x;
     x_lnQ[2*n_points-i+1] = x;

     //Fill CLs hists:
     if (cls<=1. && cls_b<=1. && cls_b_p1<999. &&cls_b_n1<999. &&cls_b_p2<999. &&cls_b_n2<999. &&
         cls_b>0.) {
       cls_obs->SetPoint( i_cls, x, cls );
       cls_exp->SetPoint( i_cls, x, cls_b );
       y_cls_exp1[i_cls] = cls_b_p1;
       y_cls_exp1[2*n_points-i_cls+1] =cls_b_n1;
       y_cls_exp2[i_cls] = cls_b_p2;
       y_cls_exp2[2*n_points-i_cls+1] = cls_b_n2;
       x_cls[i_cls] = x;
       x_cls[2*n_points-i_cls+1] = x;

       ++i_cls;
     } 
 
     delete myconfidence;
     delete mydatasource;
     delete signal;
   }
   delete data;
   delete backgd;
   
   std::cout << limits << std::endl;
   
   //-2 ln Q vs mass:
   //c1->SetLogx(1);
   c1->SetLogy(0);
   TGraph * lnQ_exp1 = new TGraph(2*n_points+2,x_lnQ,y_lnQ_exp1);
   TGraph * lnQ_exp2 = new TGraph(2*n_points+2,x_lnQ,y_lnQ_exp2);
   lnQ_exp2->SetLineColor(5);
   lnQ_exp2->SetFillColor(5);
   lnQ_exp1->SetLineColor(3);
   lnQ_exp1->SetFillColor(3);
   lnQ_obs->SetLineColor(kRed);
   lnQ_obs->SetLineWidth(3);
   lnQ_exp_b->SetLineColor(kBlue);
   lnQ_exp_b->SetLineStyle(3);
   lnQ_exp_b->SetLineWidth(3);
   lnQ_exp_sb->SetLineColor( 28 );
   lnQ_exp_sb->SetLineStyle(3);
   lnQ_exp_sb->SetLineWidth(3);
   axis_lnQ->SetMaximum(  lnQ_exp2->GetYaxis()->GetXmax() );
   axis_lnQ->SetMinimum(  lnQ_exp_sb->GetYaxis()->GetXmin()  );
   line->SetLineWidth(1);
   TLegend * lgd = new TLegend(0.5,0.28,0.89,0.4);
   lgd->SetBorderSize(0);
   lgd->SetFillColor(0);
   if (isPseudoData) lgd->AddEntry(lnQ_obs,"Observed (pseudo data)","l");
   else              lgd->AddEntry(lnQ_obs,"Observed","l");
   lgd->AddEntry(lnQ_exp_b,"Expected background-only","l");
   lgd->AddEntry(lnQ_exp_sb,"Expected signal+background","l");
   axis_lnQ->Draw("");
   lnQ_exp2->Draw("lf,same");
   lnQ_exp1->Draw("lf,same");
   lnQ_obs->Draw("l,same");
   line->DrawLine(min,0.0,max,0.0);
   lnQ_exp_b->Draw("l,same");
   lnQ_exp_sb->Draw("l,same");
   lgd->Draw("same");
   if (doeps) c1->SaveAs("pictures/lnQ_vs_param.eps");
   c1->Update();
   ps->NewPage();

   //c1->SetLogx(1);
   double exp_xsec_limit=min;
   double obs_xsec_limit=min;
   std::cout<<"Limits on "<<ScanParName_<<":"<<std::endl;
   exp_xsec_limit = GetExpectedXsecLimit(0.05, min, max);
   std::cout<<"Expected: "<<exp_xsec_limit<<" @ 95%CL in " << ScanParName_ << std::endl;
   obs_xsec_limit = GetObservedXsecLimit(0.05, min, max);
   std::cout<<"Observed: "<<obs_xsec_limit<<" @ 95%CL in " << ScanParName_ << std::endl;
   
   //double par[] = {1.6449};//<->5%
   //TSignificance eSignif(backgd->Integral(), 0);
   //double exp_signif = TSolve<TSignificance>( &eSignif, &TSignificance::operator(), par, 0.1, 10.)();
   ////double obs_signif = exp_signif/(data->Integral()-backgd->Integral());
   //std::cout<<"Expected s: "<< exp_signif/signal->Integral() <<" * signal cross section @ 95%CL significance" << std::endl;
   ////std::cout<<"Observed s: "<< obs_signif <<" * signal cross section @ 95%CL" << std::endl;
 
   c1->SetLogy(1);
   //remove empty array elements, to get a smooth TGraph:
   --i_cls;
   double ny_cls_exp1[i_cls*2];
   double nx_cls[i_cls*2];
   double ny_cls_exp2[i_cls*2];
   for (int i=0; i<=i_cls; ++i) {
     ny_cls_exp1[i] = y_cls_exp1[i];
     ny_cls_exp2[i] = y_cls_exp2[i];
     nx_cls[i]      = x_cls[i];
     ny_cls_exp1[i_cls+i]   = y_cls_exp1[2*n_points-i_cls+i];
     ny_cls_exp2[i_cls+i] = y_cls_exp2[2*n_points-i_cls+i];
     nx_cls[i_cls+i]      = x_cls[2*n_points-i_cls+i];
   }

   TGraph * cls_exp1 = new TGraph(2*i_cls,nx_cls,ny_cls_exp1);
   TGraph * cls_exp2 = new TGraph(2*i_cls,nx_cls,ny_cls_exp2);
   cls_exp2->SetLineColor(5);
   cls_exp2->SetFillColor(5);
   cls_exp1->SetLineColor(3);
   cls_exp1->SetFillColor(3);
   cls_obs->SetLineColor(kRed);
   cls_exp->SetLineColor(kBlue);
   cls_exp->SetLineStyle(3);
   cls_exp->SetLineWidth(3);
   cls_obs->SetLineWidth(3);
   //axis_cls->SetMinimum( cls_exp2->GetYaxis()->GetXmin() );
   axis_cls->SetMinimum( 0.000001 );
   axis_cls->SetMaximum(1.1);
   TLegend * lg = new TLegend(0.5,0.28,0.89,0.4);
   lg->SetBorderSize(0);
   lg->SetFillColor(0);
   if (isPseudoData) lg->AddEntry(cls_obs,"Observed (pseudo data)","l");
   else              lg->AddEntry(cls_obs,"Observed","l");
   lg->AddEntry(cls_exp,"Expected background-only ","l");
   axis_cls->Draw("l");
   cls_exp2->Draw("lf,same");
   cls_exp1->Draw("lf,same");
   cls_exp->Draw("l,same");
   cls_obs->Draw("l,same");
   lg->Draw("same");
   line->SetLineWidth(1);
   line->DrawLine(min,0.05,max,0.05);
   TArrow * arrow = new TArrow();
   arrow->SetLineWidth(1);			    
   if (obs_xsec_limit>min && obs_xsec_limit<max)			    
   arrow->DrawArrow(obs_xsec_limit,0.05,obs_xsec_limit,axis_cls->GetMinimum(),0.03,">");
   arrow->SetLineStyle(7);			    
   if (exp_xsec_limit>min && exp_xsec_limit<max)
   arrow->DrawArrow(exp_xsec_limit,50.*axis_cls->GetMinimum(),
                    exp_xsec_limit,    axis_cls->GetMinimum(),0.03,">");

   if (doeps) c1->SaveAs("pictures/CLs_vs_param.eps");
   c1->Update();
   ps->Close();
}

void cls::DrawVsXsec(bool doeps)
{
   gStyle->SetHistFillColor(0);
   gStyle->SetPalette(1);
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

   if (!backgd_ || !signal_) 
   {
     std::cerr << "ERROR! &(background histogram)="<<backgd_
               << ", &(signal histogram)="<<signal_<<std::endl;
     return;	       
   }

   TH1D * backgd = (TH1D*)backgd_->Clone();
   TH1D * signal = (TH1D*)signal_->Clone();
   TH1D * data   = (TH1D*)data_->Clone();
   backgd->GetXaxis()->SetTitle( signal->GetXaxis()->GetTitle() );
   backgd->SetTitle("");
   
   std::cout << "DATA = " << data->Integral(0, data->GetXaxis()->GetNbins()+1) <<std::endl;
   
   //Draw MET for background, signal & data:
   TCanvas * c1 = new TCanvas("","",600,600);
   TPostScript * ps = new TPostScript( outputfilename_.c_str(),111);
   TLine * line = new TLine();
   signal->SetLineColor(2);
   signal->SetLineWidth(3);
   data->SetMarkerStyle(8);
   backgd->SetMinimum(0);
   backgd->Draw("he");
   signal->Draw("he,same");
   data->Draw("pe,same");
   c1->Update();
   ps->NewPage();
   
   //CLs vs x-section:
   int n_points = 10;
   double min = 0.01;
   double max = 1;
   
   //min *= signalxsec;
   //max *= signalxsec;
   
   TH1D * cls_obs = new TH1D("cls_obs", ";relative cross section k [k * #sigma];CLs",n_points,min,max);
//   TH1D * cls_obs = new TH1D("cls_obs", ";signal cross section #sigma;CLs",n_points,min,max);
   TH1D * cls_exp = new TH1D("cls_exp", ";relative cross section k [k * #sigma];CLs",n_points,min,max);
   TH1D * lnQ_obs    = new TH1D("lnQ_obs",    ";relative cross section k [k * #sigma];-2 ln Q",n_points,min,max);
   TH1D * lnQ_exp_b  = new TH1D("lnQ_exp_b",  ";relative cross section k [k * #sigma];-2 ln Q",n_points,min,max);
   TH1D * lnQ_exp_sb = new TH1D("lnQ_exp_sb", ";relative cross section k [k * #sigma];-2 ln Q",n_points,min,max);
   double y_cls_exp1[n_points*2+2];
   double x_cls_exp[n_points*2+2];
   double x_lnQ_exp[n_points*2+2];
   double y_cls_exp2[n_points*2+2];
   double y_lnQ_exp1[n_points*2+2];
   double y_lnQ_exp2[n_points*2+2];

   std::cout<<"Scanning signal coss-section from x = "<<(int)(min*100)<<"% till "<<(int)(max*100)<<"%:"<<std::endl;
   TTable limits("Limits of cross-section scan","|");
   //limits.AddColumn<int>("i");
   limits.AddColumn<double>("x/sigma");
   limits.AddColumn<double>(" #s ");
   limits.AddColumn<double>("+-ds");
   limits.AddColumn<double>(" #b ");
   limits.AddColumn<double>("+-db");
   limits.AddColumn<double>("CL_s");
   limits.AddColumn<double>("<CL_b>_b");limits.SetPrecision(6,5);
   limits.AddColumn<double>("<CL_sb>_b");limits.SetPrecision(7,5);
   limits.AddColumn<double>("<lnQ_b>");
   limits.AddColumn<double>("<lnQ_sb>");
   limits.AddColumn<double>("<CL_s-2s>_b");limits.SetPrecision(10,5);
   limits.AddColumn<double>("<CL_s-1s>_b");limits.SetPrecision(11,5);
   limits.AddColumn<double>("<CL_s>_b");limits.SetPrecision(12,5);
   limits.AddColumn<double>("<CL_s+1s>_b");limits.SetPrecision(13,5);
   limits.AddColumn<double>("<CL_s+2s>_b");limits.SetPrecision(14,5);
   char * title = new char[100];
   int i_cls=0;
   
   for (int i=0; i<=n_points; ++i){
     if ((int)(100*(float)i/n_points)%10==0) 
       std::cout << std::setw(2)<<100*i/n_points << "% done"<<std::endl;
    

     TH1D * sig = (TH1D*)signal_->Clone();
     double x = min + (max-min)*i/n_points;
     sig->Scale( x );
     
//     double x = min + (double)i/n_points*(max-min);
//     sig->Scale( x/signalxsec );
//     x *= signalxsec;

     TLimitDataSource * source = new TLimitDataSource();
     if (!syst_) source->AddChannel(sig,backgd,data);
     else        source->AddChannel(sig,backgd,data,(TH1D*)esup_,(TH1D*)esdn_,(TH1D*)ebup_,(TH1D*)ebdn_,names_);
     TConfidenceLevel * conf = TLimit::ComputeLimit(source,fNMC_,stat_);

     //Draw plot
     c1->SetLogy(0);
     sig->SetLineColor(2);
     sig->SetLineWidth(3);
     data->SetMarkerStyle(8);
     backgd->SetMinimum(0);
     if (sig->GetMaximum()>backgd->GetMaximum()) backgd->SetMaximum(sig->GetMaximum()*sqrt(sig->GetMaximum()));
     backgd->Draw("he");
     sig->Draw("he,same");
     data->Draw("pe,same");
     if (doeps) {
       sprintf(title,"pictures/N_for_x%.3f.eps",x);
       c1->SaveAs(title);
     }  
     c1->Update();
     ps->NewPage();

     //Draw the expected and observed test statistics:
     c1->SetLogy(1);
     sprintf(title,"xTConfidenceLevel_Draw_%i",plotindex_++);
     TH1D h(title,"",50,0,0);
     double * fTSB = conf->GetTestStatistic_B();
     double * fTSS = conf->GetTestStatistic_SB();
     for (int j=0; j<fNMC_; j++) {
	h.Fill(-2*(fTSB[j]-conf->GetStot() ));
	h.Fill(-2*(fTSS[j]-conf->GetStot() ));
     }
     sprintf(title,"xb_hist_%i",plotindex_++);
     TH1D* b_hist  = new TH1D(title, ";-2 ln Q;normalized p.d.f.",50,h.GetXaxis()->GetXmin(),h.GetXaxis()->GetXmax());
     sprintf(title,"xsb_hist_%i",plotindex_++);
     TH1D* sb_hist = new TH1D(title,";-2 ln Q;normalized p.d.f.",50,h.GetXaxis()->GetXmin(),h.GetXaxis()->GetXmax());
     for (int j=0; j<fNMC_; j++) {
	b_hist->Fill(-2*(fTSB[j]-conf->GetStot() ));
	sb_hist->Fill(-2*(fTSS[j]-conf->GetStot() ));
     }
     b_hist->Scale(1.0/b_hist->Integral());
     sb_hist->Scale(1.0/sb_hist->Integral());
     b_hist->GetYaxis()->SetTitleOffset(1.3);
     b_hist->SetMinimum( 1./fNMC_ );
     b_hist->SetMaximum(3*b_hist->GetMaximum());
     b_hist->SetLineWidth(3);
     sb_hist->SetLineWidth(3);
     b_hist->SetLineColor(kBlue);
     sb_hist->SetLineColor( 28 );
     line->DrawLine(conf->GetStatistic(),b_hist->GetMinimum(),
                        conf->GetStatistic(),0.6*b_hist->GetMaximum() );
     line->SetLineWidth(3);			    
     TLegend leg(0.11,0.78,0.48,0.89);
     leg.SetBorderSize(0);
     leg.SetFillColor(0);
     leg.AddEntry(line,"Observed ","l");
     leg.AddEntry(b_hist,"Expected background-only ","l");
     leg.AddEntry(sb_hist,"Expected signal+background ","l");
     sprintf(title,"#sigma_{s} #times %f",x);
     
     b_hist->Draw("h");
     line->Draw();
     leg.Draw("same");
     sb_hist->Draw("h,same");
     if (doeps) {
       sprintf(title,"pictures/lnQ_for_x%.3f.eps",x);
       c1->SaveAs(title);
     }  
     c1->Update();
     ps->NewPage();

     //Plot # of signal and backgd events of pseudo experiments:
     c1->SetLogy(0);
     double * psig = conf->GetSig();
     double * pbgd = conf->GetBgd();
     double ds=0, db=0;
     for (int a=0; a<sig->GetNbinsX()+1; ++a) ds+=sig->GetBinError(a);
     for (int a=0; a<backgd->GetNbinsX()+1; ++a) db+=backgd->GetBinError(a);
   
     double pmin=999999, pmax=-999999;
     for (int it=0; it<fNMC_; ++it){
       if (psig[it]<pmin) pmin = psig[it];
       if (psig[it]>pmax && psig[it]<99999.) pmax = psig[it];
     }  
     for (int it=0; it<fNMC_; ++it){
       if (pbgd[it]<pmin) pmin = pbgd[it];
       if (pbgd[it]>pmax && pbgd[it]<99999.) pmax = pbgd[it];
     }  
     sprintf(title,"pseudosig_%i",plotindex_++);
     TH1F * pseudosig = new TH1F(title,";# events;# pseudo experiments",100,pmin-5,pmax+5);
     sprintf(title,"pseudobgd_%i",plotindex_++);
     TH1F * pseudobgd = new TH1F(title,";# events;# pseudo experiments",100,pmin-5,pmax+5);
     for (int it=0; it<fNMC_; ++it){
       pseudosig->Fill(psig[it]); 
     }  
     for (int it=0; it<fNMC_; ++it){
       pseudobgd->Fill(pbgd[it]); 
     }  
     pseudosig->SetLineColor(2);
     pseudosig->SetLineWidth(3);
     pseudobgd->SetLineWidth(3);
     pseudosig->Draw("h");
     pseudobgd->Draw("h,same");
     TLegend pleg(0.31,0.8,0.68,0.89);
     pleg.SetBorderSize(0);
     pleg.SetFillColor(0);
     sprintf(title,"Signal %.2f#pm%.2f",sig->Integral(),ds);
     pleg.AddEntry(pseudosig,title,"l");
     sprintf(title,"Background %.2f#pm%.2f",backgd->Integral(),db);
     pleg.AddEntry(pseudobgd,title,"l");
     pleg.Draw("same");
     if (doeps) {
       sprintf(title,"pictures/pseudoN_for_x%.3f.eps",x);
       c1->SaveAs(title);
     }
     c1->Update();
     ps->NewPage();
     delete pseudosig;
     delete pseudobgd;
     
     //Calculate CLs and lnQ:
     double cls = conf->CLs();
     double cls_b = conf->GetExpectedCLs_b();
     double cls_b_p1 = conf->GetExpectedCLs_b(1);
     double cls_b_n1 = conf->GetExpectedCLs_b(-1);
     double cls_b_p2 = conf->GetExpectedCLs_b(2);
     double cls_b_n2 = conf->GetExpectedCLs_b(-2);
     Sort(cls_b_n1, cls_b_p1);
     Sort(cls_b_n2, cls_b_p2);
     if (cls_b>0 && cls_b<=1.) {
       cls_obs->SetBinContent(i_cls, cls );
       cls_exp->SetBinContent(i_cls, cls_b );
       y_cls_exp1[i_cls] = cls_b_p1;
       y_cls_exp1[n_points*2-i_cls+1] = cls_b_n1;
       y_cls_exp2[i_cls] = cls_b_p2;
       y_cls_exp2[n_points*2-i_cls+1] = cls_b_n2;
       x_cls_exp[i_cls] = x;
       x_cls_exp[n_points*2-i_cls+1] = x;
       ++i_cls;
     }
 
     lnQ_obs->SetBinContent( i, conf->GetStatistic() );
     lnQ_exp_b->SetBinContent( i, conf->GetExpectedStatistic_b() );
     lnQ_exp_sb->SetBinContent( i, conf->GetExpectedStatistic_sb() );
     y_lnQ_exp1[i] = conf->GetExpectedStatistic_b(1);
     y_lnQ_exp1[n_points*2-i+1] = conf->GetExpectedStatistic_b(-1);
     y_lnQ_exp2[i] = conf->GetExpectedStatistic_b(2);
     y_lnQ_exp2[n_points*2-i+1] = conf->GetExpectedStatistic_b(-2);
     x_lnQ_exp[i] = x;
     x_lnQ_exp[n_points*2-i+1] = x;

     limits.Fill(x,
		 sig->Integral(),ds,
		 backgd->Integral(),db,
                 cls,
		 conf->GetExpectedCLb_b(),conf->GetExpectedCLsb_b(),
                 conf->GetExpectedStatistic_b(),conf->GetExpectedStatistic_sb(),
		 cls_b_n2,cls_b_n1,
		 cls_b,
		 cls_b_p1,cls_b_p2
		);
     delete conf;
     delete source;
     delete sig;
   }
   std::cout << limits << std::endl;
   
   //-2 ln Q vs mass:
   //c1->SetLogx(1);
   c1->SetLogy(0);
   TGraph * lnQ_exp1 = new TGraph(2*n_points,x_lnQ_exp,y_lnQ_exp1);
   TGraph * lnQ_exp2 = new TGraph(2*n_points,x_lnQ_exp,y_lnQ_exp2);
   lnQ_exp2->SetLineColor(5);
   lnQ_exp2->SetFillColor(5);
   lnQ_exp1->SetLineColor(3);
   lnQ_exp1->SetFillColor(3);
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
   TLegend * lgd = new TLegend(0.5,0.28,0.89,0.4);
   lgd->SetBorderSize(0);
   lgd->SetFillColor(0);
   if (isPseudoData) lgd->AddEntry(lnQ_obs,"Observed (pseudo data)","l");
   else              lgd->AddEntry(lnQ_obs,"Observed","l");
   lgd->AddEntry(lnQ_exp_b,"Expected background-only","l");
   lgd->AddEntry(lnQ_exp_sb,"Expected signal+background","l");
   lnQ_obs->Draw("c");
   lnQ_exp2->Draw("lf,same");
   lnQ_exp1->Draw("lf,same");
   lnQ_obs->Draw("c,same");
   line->DrawLine(100,0.0,500,0.0);
   lnQ_exp_b->Draw("c,same");
   lnQ_exp_sb->Draw("c,same");
   lgd->Draw("same");
   //if (doeps) 
   c1->SaveAs("pictures/lnQvsxsec.eps");
   c1->Update();
   ps->NewPage();

   //c1->SetLogx(1);
   double exp_xsec_limit = 0;
   double obs_xsec_limit = 0;
   std::cout<<"Cross-section limit:"<<std::endl;
   //exp_xsec_limit = GetExpectedXsecLimit(0.05, min, max);
   //obs_xsec_limit = GetObservedXsecLimit(0.05, min, max);
   std::cout<<"Expected: "<<exp_xsec_limit<<" * signal cross section @ 95%CL" << std::endl;
   std::cout<<"Observed: "<<obs_xsec_limit<<" * signal cross section @ 95%CL" << std::endl;
   
   //double par[] = {1.6449};//<->5%
   //TSignificance eSignif(backgd->Integral(), 0);
   //double exp_signif = TSolve<TSignificance>( &eSignif, &TSignificance::operator(), par, 0.1, 10.)();
   ////double obs_signif = exp_signif/(data->Integral()-backgd->Integral());
   //std::cout<<"Expected s: "<< exp_signif/signal->Integral() <<" * signal cross section @ 95%CL significance" << std::endl;
   ////std::cout<<"Observed s: "<< obs_signif <<" * signal cross section @ 95%CL" << std::endl;
   //remove empty array elements, to get a smooth TGraph:

   --i_cls;
   double ny_cls_exp1[i_cls*2];
   double nx_cls[i_cls*2];
   double ny_cls_exp2[i_cls*2];
   for (int i=0; i<=i_cls; ++i) {
     ny_cls_exp1[i] = y_cls_exp1[i];
     ny_cls_exp2[i] = y_cls_exp2[i];
     nx_cls[i]      = x_cls_exp[i];
     ny_cls_exp1[i_cls+i]   = y_cls_exp1[2*n_points-i_cls+i];
     ny_cls_exp2[i_cls+i] = y_cls_exp2[2*n_points-i_cls+i];
     nx_cls[i_cls+i]      = x_cls_exp[2*n_points-i_cls+i];
   }
 
   c1->SetLogy(1);
   TGraph * cls_exp1 = new TGraph(2*i_cls,nx_cls,ny_cls_exp1);
   TGraph * cls_exp2 = new TGraph(2*i_cls,nx_cls,ny_cls_exp2);
   cls_exp2->SetLineColor(5);
   cls_exp2->SetFillColor(5);
   cls_exp1->SetLineColor(3);
   cls_exp1->SetFillColor(3);
   cls_obs->SetLineColor(kRed);
   cls_exp->SetLineColor(kBlue);
   cls_exp->SetLineStyle(3);
   cls_exp->SetLineWidth(3);
   cls_obs->SetLineWidth(3);
   //cls_obs->SetMinimum(0.00001);
   cls_exp->SetMaximum(1.1);
   TLegend * lg = new TLegend(0.5,0.28,0.89,0.4);
   lg->SetBorderSize(0);
   lg->SetFillColor(0);
   if (isPseudoData) lg->AddEntry(cls_obs,"Observed (pseudo data)","l");
   else              lg->AddEntry(cls_obs,"Observed","l");
   lg->AddEntry(cls_exp,"Expected background-only ","l");
   cls_exp->Draw("c");
   cls_exp2->Draw("lf,same");
   cls_exp1->Draw("lf,same");
   cls_obs->Draw("c,same");
   cls_exp->Draw("c,same");
   lg->Draw("same");
   line->SetLineWidth(1);
   line->DrawLine(min,0.05,max,0.05);
   TArrow * arrow = new TArrow();
   arrow->SetLineWidth(1);			    
   if (obs_xsec_limit) arrow->DrawArrow(obs_xsec_limit,0.05,obs_xsec_limit,cls_obs->GetYaxis()->GetXmin(),0.03,">");
   arrow->SetLineStyle(7);			    
   if (exp_xsec_limit) arrow->DrawArrow(exp_xsec_limit,50.*cls_obs->GetMinimum(),
                    exp_xsec_limit,    cls_obs->GetYaxis()->GetXmin(),0.03,">");

   //if (doeps) 
   c1->SaveAs("pictures/CLsvsxsec.eps");
   c1->Update();
   delete data;
   delete backgd;
   delete signal;

   ps->Close();
   
}

double cls::GetTotalStatError(const TH1* arg)
{
   if (!arg) return 0;
   TH1F * h = (TH1F*)arg->Clone();
   h->Rebin( h->GetXaxis()->GetNbins() ); 
   double result = sqrt(pow(h->GetBinError(0),2)+pow(h->GetBinError(1),2)+pow(h->GetBinError(2),2));
   delete h;
   return result;
}

void cls::WriteResult(ConfigFile * config, double xsec, const std::string s)
{
  double min=0.01; //min. limit on xsection in which CLs = 95% is searched for
  double max=1000; //max. limit on xsection in which CLs = 95% is searched for

  TH1D * backgd = (TH1D*)backgd_->Clone();
  TH1D * signal = (TH1D*)signal_->Clone();
  TH1D * data   = (TH1D*)data_->Clone();
  double nsignal = signal->Integral(0,signal->GetXaxis()->GetNbins()+1);

  //std::cout << "...calculating ExpectedXsecLimit" <<std::endl;
  double exp_xsec_limit = GetExpectedXsecLimit(0.05, min, max);
  //std::cout << "...calculating ObservedXsecLimit" <<std::endl;
  double obs_xsec_limit = GetObservedXsecLimit(0.05, min, max);

  config->add(s+"ExpXsecLimit", exp_xsec_limit * xsec);
  config->add(s+"ObsXsecLimit", obs_xsec_limit * xsec);
  config->add(s+"ExpNsigLimit", exp_xsec_limit * nsignal);
  config->add(s+"ObsNsigLimit", obs_xsec_limit * nsignal);
  config->add(s+"CLsUseStat",   stat_);
  config->add(s+"Xsection", xsec);

  if (obs_xsec_limit>=min && obs_xsec_limit<=max) {
    TH1D * sig = (TH1D*)signal_->Clone();
    sig->Scale( obs_xsec_limit );
    TLimitDataSource * source = new TLimitDataSource();
    if (!syst_) source->AddChannel(sig,backgd,data);
    else        source->AddChannel(sig,backgd,data,(TH1D*)esup_,(TH1D*)esdn_,(TH1D*)ebup_,(TH1D*)ebdn_,names_);
    TConfidenceLevel * conf = TLimit::ComputeLimit(source,fNMC_,stat_);
    config->add(s+"CLs@obs",      conf->CLs());
    config->add(s+"CLs_b@obs",    conf->GetExpectedCLs_b());
    config->add(s+"CLs_b_p1@obs", conf->GetExpectedCLs_b(1));
    config->add(s+"CLs_b_n1@obs", conf->GetExpectedCLs_b(-1));
    config->add(s+"CLs_b_p2@obs", conf->GetExpectedCLs_b(2));
    config->add(s+"CLs_b_n2@obs", conf->GetExpectedCLs_b(-2));
    config->add(s+"CLb_b@obs",    conf->GetExpectedCLb_b());
    config->add(s+"CLsb_b@obs",   conf->GetExpectedCLsb_b());
    config->add(s+"-2lnQ_b@obs",  conf->GetExpectedStatistic_b());
    config->add(s+"-2lnQ_sb@obs", conf->GetExpectedStatistic_sb());
    delete sig;
    delete conf;
    delete source;
  }
  if (exp_xsec_limit>=min && exp_xsec_limit<=max) {
    TH1D * sig = (TH1D*)signal_->Clone();
    sig->Scale( exp_xsec_limit );
    TLimitDataSource * source = new TLimitDataSource();
    if (!syst_) source->AddChannel(sig,backgd,data);
    else        source->AddChannel(sig,backgd,data,(TH1D*)esup_,(TH1D*)esdn_,(TH1D*)ebup_,(TH1D*)ebdn_,names_);
    TConfidenceLevel * conf = TLimit::ComputeLimit(source,fNMC_,stat_);
    config->add(s+"CLs@exp",      conf->CLs());
    config->add(s+"CLs_b@exp",    conf->GetExpectedCLs_b());
    config->add(s+"CLs_b_p1@exp", conf->GetExpectedCLs_b(1));
    config->add(s+"CLs_b_n1@exp", conf->GetExpectedCLs_b(-1));
    config->add(s+"CLs_b_p2@exp", conf->GetExpectedCLs_b(2));
    config->add(s+"CLs_b_n2@exp", conf->GetExpectedCLs_b(-2));
    config->add(s+"CLb_b@exp",    conf->GetExpectedCLb_b());
    config->add(s+"CLsb_b@exp",   conf->GetExpectedCLsb_b());
    config->add(s+"-2lnQ_b@exp",  conf->GetExpectedStatistic_b());
    config->add(s+"-2lnQ_sb@exp", conf->GetExpectedStatistic_sb());
    delete sig;
    delete conf;
    delete source;
  }
  delete backgd;
  delete signal;
  delete data;
}


void cls::WriteConfidence(ConfigFile * config, const std::string s)
{
  TH1D * backgd = (TH1D*)backgd_->Clone();
  TH1D * signal = (TH1D*)signal_->Clone();
  TH1D * data   = (TH1D*)data_->Clone();
  TLimitDataSource * source = new TLimitDataSource();
  if (!syst_) source->AddChannel(signal,backgd,data);
  else        source->AddChannel(signal,backgd,data,(TH1D*)esup_,(TH1D*)esdn_,(TH1D*)ebup_,(TH1D*)ebdn_,names_);
  TConfidenceLevel * conf = TLimit::ComputeLimit(source,fNMC_,stat_);

  config->add(s+"CLsUseStat",    stat_);
  config->add(s+"CLs@xsec",      conf->CLs());
  config->add(s+"CLs_b@xsec",    conf->GetExpectedCLs_b());
  config->add(s+"CLs_b_p1@xsec", conf->GetExpectedCLs_b(1));
  config->add(s+"CLs_b_n1@xsec", conf->GetExpectedCLs_b(-1));
  config->add(s+"CLs_b_p2@xsec", conf->GetExpectedCLs_b(2));
  config->add(s+"CLs_b_n2@xsec", conf->GetExpectedCLs_b(-2));
  config->add(s+"CLb_b@xsec",    conf->GetExpectedCLb_b());
  config->add(s+"CLsb_b@xsec",   conf->GetExpectedCLsb_b());
  config->add(s+"-2lnQ_b@xsec",  conf->GetExpectedStatistic_b());
  config->add(s+"-2lnQ_sb@xsec", conf->GetExpectedStatistic_sb());
  delete signal;
  delete conf;
  delete source;
  delete backgd;
  delete data;
}



void cls::WriteResult(const std::string out, double xsec, const std::string s)
{
  if (out=="") {
    std::cerr<<"ERROR [cls::WriteResult(const std::string out="<<out<<")]: No filename and no signal parameters specified!" <<std::endl;
    return;
  }
  if (!backgd_ || !signal_) {
    std::cerr<<"ERROR [cls::WriteResult(const std::string out="<<out<<")]: No signal ("
             <<signal_<<") or no background ("<<backgd_<<")  hypothesis specified!" <<std::endl;
    return;
  }
  std::stringstream ss;
  ss << out;
  ConfigFile * res = new ConfigFile();
  WriteResult( res, xsec, s );
  
  //write stuff:  
  time_t rawtime;
  struct tm * timeinfo;
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  ofstream ofile;
  ofile.open (ss.str().c_str());
  ofile << res->getComment() << asctime (timeinfo) 
        << res->getComment()<< "\n"
        << *res;
  ofile.close();
  delete res;
}
