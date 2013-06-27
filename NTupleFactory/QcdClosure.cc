// $Id: QcdClosure.cc,v 1.4 2013/06/17 16:15:34 auterman Exp $

/*** ------------------------------------------------------------------------------------------------------- ***
     NTupleFactory, a tool to plot final results, using Root Reflex information

     Christian Autermann, RWTH Aachen, May 2013
 *** ------------------------------------------------------------------------------------------------------- ***/

#include "QcdClosure.h"
#include "Event.h"
#include "Selection.h"
#include "Analyzer.h"
#include "Factory.h"

#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TFile.h"

#include <string>

//The main function 
int main(int argc, char* argv[])
{
  TCanvas c1("c1","c1",600,600);


  ///Factory approach ------------------------------------------------------------------------
  Factory thefactory;
  
  //QCD sample
  std::vector<std::string> qcdfiles;
  //qcdfiles.push_back("input/QCD_250_500_V01.00__7_tree.root");
  //qcdfiles.push_back("input/QCD_250_500_V01.07_tree.root");
  qcdfiles.push_back("input/QCD_500_1000_V01.07_tree.root");
  qcdfiles.push_back("input/QCD_1000_inf_V01.07_tree.root");
  
  
  Sample qcd_sample(qcdfiles,"susyTree","QcdV107Selector","QCD","QCD",1);
  //Sample qcd_sample(qcdfiles,"susyTree","QcdSelector","QCD","QCD",1);
  qcd_sample.SetFillColor(7);
  thefactory.AddSample( &qcd_sample );
  thefactory.ProcessFraction(0.01);
  
  //Selections
  Selection * inclusive = new Selection("Inclusive");
  
  //  isolation  ^
  // (tight)     |  QcdWeightPhoton   |    QcdControlPhoton   |
  //             |--------------------+-----------------------+
  // (loose)     |   QcdWeightFake    |    QcdControlFake     |
  //             |--------------------+----------------------->
  //                    <100 GeV            >100GeV        MET
  
  Selection * qcdWeightPhoton  = new QcdWeightPhoton("QcdWeightPhoton");
  Selection * qcdWeightFake    = new QcdWeightFake("QcdWeightFake");
  Selection * qcdControlPhoton = new QcdControlPhoton("QcdControlPhoton");
  Selection * qcdControlFake   = new QcdControlFake("QcdControlFake");


  //Make sure we have all user variables available
  ProduceVariables* produceVariables = new ProduceVariables();
  thefactory.AddAnalyzer( inclusive, produceVariables ); 


  //Some Analyzer tasks ---
  //give 20 status updates, i.e. every 5%
  Analyzer* status = new PrintStatus(20); 
  thefactory.AddAnalyzer( inclusive, status ); 


  //calculate QCD weights used for data-driven QCD background estimation
  QcdWeights* QcdWeight_tight = new QcdWeights( new FloatVariable("tightPhoton_pt_1" ) );
  QcdWeight_tight->AddSample( &qcd_sample );
  QcdWeights* QcdWeight_fake  = new QcdWeights( new FloatVariable("loosePhoton_pt_1" ) );
  QcdWeight_fake->AddSample( &qcd_sample );
  thefactory.AddAnalyzer( qcdWeightPhoton,  QcdWeight_tight ); 
  thefactory.AddAnalyzer( qcdWeightFake,    QcdWeight_fake ); 

  //QCD background estimation
  QcdPrediction* qcd_pred  = new QcdPrediction( new FloatVariable("loosePhoton_pt_1" ) );
  qcd_pred->AddSample( &qcd_sample, "results/QcdWeights_QCD_QcdWeightPhoton.root", "results/QcdWeights_QCD_QcdWeightFake.root" );
  thefactory.AddAnalyzer( qcdControlFake,    qcd_pred ); 




  //Closure test plots QCD  -------------------------------------------------------------------------------------------------------------------
  //-- photon pt
  TH1 * h_qcdclosure_tight_pt = new TH1F("h_qcdclosure_photonpt",";Photon p_{T} [GeV]; events",100,0,1000);//template plot for style etc
  h_qcdclosure_tight_pt->SetStats(0);
  TH1 * h_qcdclosure_fake_pt = (TH1*)h_qcdclosure_tight_pt->Clone();
  h_qcdclosure_fake_pt->SetFillColor(5);
  h_qcdclosure_tight_pt->SetMarkerStyle(7);
  Closure* qcdclosure_photonpt = new Closure(&c1);
  qcdclosure_photonpt->Add(&qcd_sample, qcdControlFake,   h_qcdclosure_fake_pt,  new FloatVariable("loosePhoton_pt_1" ), new FloatVariable("QcdWeight" ));
  qcdclosure_photonpt->Add(&qcd_sample, qcdControlPhoton, h_qcdclosure_tight_pt, new FloatVariable("tightPhoton_pt_1" ));
  thefactory.AddAnalyzer( inclusive, qcdclosure_photonpt );

  //-- photon pt
  TH1 * h_qcdclosure_tight_met = new TH1F("h_qcdclosure_met",";MET [GeV]; events",100,0,1000);//template plot for style etc
  h_qcdclosure_tight_met->SetStats(0);
  TH1 * h_qcdclosure_fake_met = (TH1*)h_qcdclosure_tight_met->Clone();
  h_qcdclosure_fake_met->SetFillColor(5);
  h_qcdclosure_tight_met->SetMarkerStyle(7);
  Closure* qcdclosure_photonmet = new Closure(&c1 );
  qcdclosure_photonmet->Add(&qcd_sample, qcdControlFake,   h_qcdclosure_fake_met,  new FloatVariable("met"));
  qcdclosure_photonmet->Add(&qcd_sample, qcdControlPhoton, h_qcdclosure_tight_met, new FloatVariable("met"));
  thefactory.AddAnalyzer( inclusive, qcdclosure_photonmet );


  //1D plot for every sample ------------------------------------------------------------------------------------------------------------------
  // --met
  Analyzer* plot_Met = new Plot1D(&c1,new TH1F("plot1D_met",";Met [GeV]; events",100,0,500),new FloatVariable("met"));
  thefactory.AddAnalyzer( qcdControlFake,   plot_Met ); 
  thefactory.AddAnalyzer( qcdControlPhoton, plot_Met ); 

  // --photon pt [0]
  Analyzer* plot_photonpt = new Plot1D(&c1,new TH1F("plot1D_photonpt",";Photon p_{T} [GeV]; events",100,0,500),new FloatArrayVariable("photon_pt",0) );
  thefactory.AddAnalyzer( qcdControlFake, plot_photonpt ); 
  thefactory.AddAnalyzer( qcdControlPhoton, plot_photonpt ); 

  // --first loose photon pt
  Analyzer* plot_1loosephotonpt = new Plot1D(&c1,new TH1F("plot1D_firstloosephotonpt",";1rst tight Photon p_{T} [GeV]; events",100,0,500),new FloatVariable("loosePhoton_pt_1") );
  thefactory.AddAnalyzer( qcdControlFake, plot_1loosephotonpt ); 

  // --first tight photon pt
  Analyzer* plot_1tightphotonpt = new Plot1D(&c1,new TH1F("plot1D_firsttightphotonpt",";1rst loose Photon p_{T} [GeV]; events",100,0,500),new FloatVariable("tightPhoton_pt_1") );
  thefactory.AddAnalyzer( qcdControlPhoton, plot_1tightphotonpt ); 




  //cutflow table
  CutFlow* cutflow = new CutFlow( inclusive );
  cutflow->AddSelection( qcdControlFake );
  cutflow->AddSelection( qcdControlPhoton );
  thefactory.AddAnalyzer( inclusive, cutflow ); 







  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  //process events
  thefactory.Process();



 
  return 0;
}



///QCD Weights -------------------------------------------------------------------------
QcdWeights::QcdWeights(Variable*v1,Variable*weight):v1_(v1),weight_(weight){};

void QcdWeights::AddSample(const Sample*sample,const std::string& name)
{
  file_[ sample ] = name;
}

void QcdWeights::InitSample(const Selection*sel,const Sample* sample)
{
  //check if we want to use this sample
  if ( file_.find(sample)==file_.end()) return;
  if ( file_[sample]=="") file_[ sample ] = "results/QcdWeights_"+sample->GetName()+"_"+sel->Name()+".root";
  
  hist_[sample]  = new TH1F(("QW_"+sample->GetName()+"_"+sel->Name()).c_str(),
                            ("Qcd Weights "+sample->GetName()+" "+sel->Name()+"; EM pT [GeV]; events").c_str(),
                            1000,0,500);
}

void QcdWeights::Process(const Event*evt)
{
  std::map<const Sample*, TH1*>::const_iterator s = hist_.find(evt->GetSample());
  if ( s ==hist_.end()) return;

  if (weight_) 
     s->second->Fill((*v1_)(evt), (*weight_)(evt)*s->first->GetWeight() );
  else
     s->second->Fill((*v1_)(evt), s->first->GetWeight() );
}

void QcdWeights::EndJob()
{
  for (std::map<const Sample*, TH1*>::iterator it=hist_.begin(); it!=hist_.end(); ++it) {
    TFile* file = new TFile(file_[it->first].c_str(),"recreate");
    file->cd();
    it->second->Write("QcdWeight");
    file->Write();
    file->Close();
    delete file;
  }
}



///QCD Prediction -------------------------------------------------------------------------
QcdPrediction::QcdPrediction(Variable*v1, Variable*weight):v1_(v1),weight_(weight){};

void QcdPrediction::AddSample(const Sample*sample, const std::string& fact, const std::string& norm)
{
  fact_[ sample ] = fact;
  norm_[ sample ] = norm;
}
void QcdPrediction::InitSample(const Selection*sel,const Sample* sample)
{
  //check if we want to use this sample
  if ( fact_.find(sample)==fact_.end()) return;
  
  TFile * ffact = new TFile( fact_[sample].c_str() );
  gROOT->cd(); // can't close files otherwise!?
  TH1F* fact=(TH1F*)ffact->Get("QcdWeight"); 

  TH1F * h  = (TH1F*)fact->Clone(); h->Rebin(25);
  TH1F *w1m = (TH1F*)fact->Clone(); w1m->Rebin(25);
  TH1F *w1p = (TH1F*)fact->Clone(); w1p->Rebin(25);
  
  TFile *fnorm = new TFile( norm_[sample].c_str() );
  TH1F* norm=(TH1F*)fnorm->Get("QcdWeight"); 
  
  TH1F * n  = (TH1F*)norm->Clone(); n->Rebin(25);

  hist_[sample] = h;
  w1m_[sample]  = w1m;
  w1p_[sample]  = w1p;
  h->Sumw2();
  n->Sumw2();
  h->Divide( n );
  for (int i=0; i<=w1m->GetXaxis()->GetNbins()+1;++i) {
    w1m->SetBinContent(i, h->GetBinContent(i)-h->GetBinError(i));
    w1p->SetBinContent(i, h->GetBinContent(i)+h->GetBinError(i));
    w1m->SetBinError(i, 0);
    w1p->SetBinError(i, 0);
  }
  w1m->SetLineColor(2);
  w1p->SetLineColor(8);

  TCanvas c2("c2","c2",600,600);
  c2.cd();
  
  h->Draw("pe");
  w1m->Draw("h,same");
  w1p->Draw("h,same");
  c2.SaveAs(("results/QcdWeight_"+sample->GetName()+".pdf").c_str());
  c2.SaveAs(("results/QcdWeight_"+sample->GetName()+".png").c_str());

  fact->Draw("h");
  c2.SaveAs(("results/QcdWeight_factor_"+sample->GetName()+".pdf").c_str());
  c2.SaveAs(("results/QcdWeight_factor_"+sample->GetName()+".png").c_str());

  n->Draw("h");
  c2.SaveAs(("results/QcdWeight_normal_"+sample->GetName()+".pdf").c_str());
  c2.SaveAs(("results/QcdWeight_normal_"+sample->GetName()+".png").c_str());

  delete fact;
  ffact->Close();
  delete ffact;
  delete norm;
  fnorm->Close();
  delete fnorm;
}

void QcdPrediction::Produce(Event*evt)
{
  if ( fact_.find(evt->GetSample())==fact_.end()) return;
  
  TH1*h  =hist_[evt->GetSample()];
  TH1*w1m=w1m_[ evt->GetSample()];
  TH1*w1p=w1p_[ evt->GetSample()];

  int bin = h->FindBin((*v1_)(evt));
  evt->AddVariable<Float_t>( "QcdWeight",         new Float_t( h->GetBinContent(bin) ));
  evt->AddVariable<Float_t>( "syst_m1_qcdweight", new Float_t( w1m->GetBinContent(bin) ));
  evt->AddVariable<Float_t>( "syst_p1_qcdweight", new Float_t( w1p->GetBinContent(bin) ));
}


void ProduceVariables::Produce(Event*evt)
{
  int idxTight = -1;
  FindTightPhoton( &idxTight, evt);
  float tightPhoton_pt_1 = (idxTight<0 ? -999. : (evt->Get<FloatArray>("photon_pt")).val[idxTight] ); 
  evt->AddVariable<Float_t>( "tightPhoton_pt_1",   new Float_t(tightPhoton_pt_1) );
  
  int idxLoose = -1;
  FindLoosePhoton( &idxLoose, evt);
  float loosePhoton_pt_1 = (idxLoose<0 ? -999. : (evt->Get<FloatArray>("photon_pt")).val[idxLoose] ); 
  evt->AddVariable<Float_t>( "loosePhoton_pt_1",   new Float_t(loosePhoton_pt_1) );

  evt->AddVariable<Float_t>( "QcdWeight",   new Float_t(1) );

}


