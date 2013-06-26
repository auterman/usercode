// $Id: QcdClosure.cc,v 1.00 2013/05/26 20:00:00 auterman Exp $

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
  qcdfiles.push_back("input/QCD_250_500_V01.00__7_tree.root");
  Sample qcd_sample(qcdfiles,"susyTree","QcdSelector","QCD","QCD",1);
  qcd_sample.SetFillColor(7);
  thefactory.AddSample( &qcd_sample );
  
  //Selections
  Selection * inclusive = new Selection("Inclusive");
  Selection * presel    = new PreSelection("PreSel");

  //Some Analyzer tasks ---
  //give 20 status updates, i.e. every 5%
  Analyzer* status = new PrintStatus(20); 
  thefactory.AddAnalyzer( inclusive, status ); 


  //calculate QCD weights used for data-driven QCD background estimation
  QcdWeights* QcdWeight_tight = new QcdWeights( );
  QcdWeight_tight->AddSample( &qcd_sample );
  QcdWeights* QcdWeight_fake  = new QcdWeights( );
  QcdWeight_fake->AddSample( &qcd_sample );
  thefactory.AddAnalyzer( presel,    QcdWeight_tight ); 
  thefactory.AddAnalyzer( inclusive, QcdWeight_fake ); 

  //QCD background estimation
  QcdPrediction* qcd_pred  = new QcdPrediction( );
  qcd_pred->AddSample( &qcd_sample, "results/QcdWeights_QCD_PreSel.root", "results/QcdWeights_QCD_Inclusive.root" );
  thefactory.AddAnalyzer( presel,    qcd_pred ); 

  //Closure test QCD
  TH1 * h_qcdclosure_photonpt = new TH1F("h_qcdclosure_photonpt",";Photon p_{T} [GeV]; events",100,0,1000);//template plot for style etc
  h_qcdclosure_photonpt->SetStats(0);
  Analyzer* qcdclosure_photonpt = new Stack(&c1,h_qcdclosure_photonpt,new FloatArrayVariable("photon_pt",0) );
  thefactory.AddAnalyzer( presel, qcdclosure_photonpt );

  //process events
  thefactory.Process();


  return 0;
}



///QCD Weights -------------------------------------------------------------------------
QcdWeights::QcdWeights(Variable*weight):v1_(new FloatArrayVariable("photon_pt",0)),weight_(weight){};

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
QcdPrediction::QcdPrediction(Variable*weight):v1_(new FloatArrayVariable("photon_pt",0)),weight_(weight){};

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
  TH1F * h  = (TH1F*)fact->Clone();
  TH1F *w1m = (TH1F*)fact->Clone();
  TH1F *w1p = (TH1F*)fact->Clone();
  
  TFile *fnorm = new TFile( norm_[sample].c_str() );
  TH1F* norm=(TH1F*)fnorm->Get("QcdWeight"); 
  TH1F* n = (TH1F*)norm->Clone();
  
  hist_[sample] = h;
  w1m_[sample]  = w1m;
  w1p_[sample]  = w1p;
  h->Sumw2();
  n->Sumw2();
  h->Divide( n );
  for (int i=0; i<w1m->GetXaxis()->GetNbins()+1;++i) {
    w1m->SetBinContent(i, h->GetBinContent(i)-h->GetBinError(i));
    w1p->SetBinContent(i, h->GetBinContent(i)+h->GetBinError(i));
  }
  w1m->SetLineColor(2);
  w1p->SetLineColor(8);

  TCanvas c2("c2","c2",600,600);
  c2.cd();
  h->Draw("pe");
  w1m->Draw("h,same");
  w1p->Draw("h,same");
  c2.SaveAs(("results/QcdWeight_"+sample->GetName()+".pdf").c_str());

  fact->Draw("h");
  c2.SaveAs(("results/QcdWeight_factor_"+sample->GetName()+".pdf").c_str());

  n->Draw("h");
  c2.SaveAs(("results/QcdWeight_normal_"+sample->GetName()+".pdf").c_str());

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
  
  evt->AddVariable<Float_t>( "QcdWeight",         new Float_t( h->GetBinContent(h->FindBin((*v1_)(evt))) ));
  evt->AddVariable<Float_t>( "syst_m1_qcdweight", new Float_t( w1m->GetBinContent(w1m->FindBin((*v1_)(evt))) ));
  evt->AddVariable<Float_t>( "syst_p1_qcdweight", new Float_t( w1p->GetBinContent(w1p->FindBin((*v1_)(evt))) ));
}
