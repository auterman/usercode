
#include "MySelector.h"

#include <TH2.h>
#include <TCanvas.h>
#include <TStyle.h>
//#include "TMath.h"
#include "TFile.h"

#include <complex>
#include <iostream>
#include <string>

ClassImp(MySelector) //no ";" !

void MySelector::Begin(TTree * tree)
{
  Init(tree);
 
  TString option = GetOption();
}

void MySelector::SlaveBegin(TTree * tree)
{
 
   //initialize the Tree branch addresses
//   Init(tree);
/*
   nentries = tree->GetEntries();

   //print the option specified in the Process function.
   TString option = GetOption();
   Info("SlaveBegin",
        "starting MySelector analysis with process option: %s (tree: %p)", option.Data(), tree);

   //binning
   float pt_binning[] = {100, 200, 300, 400, 500, 600, 800, 1000};
   int   pt_nbins = 7;
   float met_binning[] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 120, 160, 200, 250, 300, 350, 500};
   int   met_nbins = 17;
   float ht_binning[] = {500, 600, 700, 800, 900, 1000, 1200, 1600};
   int   ht_nbins = 7;

   //QCD weights
   h_pt    = new TH1F("h_lowmet_pt",";pT [GeV];entries",          pt_nbins,pt_binning);
   w_pt    = new TH1F("w_lowmet_pt",";pT [GeV];entries",          pt_nbins,pt_binning);
   h_ht    = new TH1F("h_lowmet_ht",";HT [GeV];entries",          ht_nbins,ht_binning);
   w_ht    = new TH1F("w_lowmet_ht",";HT [GeV];entries",          ht_nbins,ht_binning);
   h_met   = new TH1F("h_lowmet_met",";HT [GeV];entries",         met_nbins,met_binning);
   w_met   = new TH1F("w_lowmet_met",";HT [GeV];entries",         met_nbins,met_binning);  
   h_pt_ht = new TH2F("h_lowmet_pt_ht",";pT [GeV];HT [GeV];entries", pt_nbins,pt_binning, ht_nbins,ht_binning);    
   w_pt_ht = new TH2F("w_lowmet_pt_ht",";pT [GeV];HT [GeV];entries", pt_nbins,pt_binning, ht_nbins,ht_binning);
   fOutput->Add(h_pt);
   fOutput->Add(w_pt);
   fOutput->Add(h_ht);
   fOutput->Add(w_ht);
   fOutput->Add(h_met);
   fOutput->Add(w_met);
   fOutput->Add(h_pt_ht);
   fOutput->Add(w_pt_ht);
*/
/*
   //QCD closure
   h_qcdclosure_signal_photonpt     = new TH1F("h_qcdclosure_signal_photonpt",";photon pT [GeV];entries",pt_nbins,pt_binning);
   fOutput->Add(h_qcdclosure_signal_photonpt);
   h_qcdclosure_prediction_photonpt = new TH1F("h_qcdclosure_prediction_photonpt",";photon pT [GeV];entries",pt_nbins,pt_binning);
   fOutput->Add(h_qcdclosure_prediction_photonpt);
   h_qcdclosure_error_photonpt = new TH1F("h_qcdclosure_error_photonpt",";photon pT [GeV];entries",pt_nbins,pt_binning);
   fOutput->Add(h_qcdclosure_error_photonpt);
   h_qcdclosure_signal_met     = new TH1F("h_qcdclosure_signal_met",";MET [GeV];entries",met_nbins,met_binning);
   fOutput->Add(h_qcdclosure_signal_met);
   h_qcdclosure_prediction_met = new TH1F("h_qcdclosure_prediction_met",";MET [GeV];entries",met_nbins,met_binning);
   fOutput->Add(h_qcdclosure_prediction_met);
   h_qcdclosure_error_met = new TH1F("h_qcdclosure_error_met",";MET [GeV];entries",met_nbins,met_binning);
   fOutput->Add(h_qcdclosure_error_met);
   h_qcdclosure_signal_ht     = new TH1F("h_qcdclosure_signal_ht",";HT [GeV];entries",ht_nbins,ht_binning);
   fOutput->Add(h_qcdclosure_signal_ht);
   h_qcdclosure_prediction_ht = new TH1F("h_qcdclosure_prediction_ht",";HT [GeV];entries",ht_nbins,ht_binning);
   fOutput->Add(h_qcdclosure_prediction_ht);
   h_qcdclosure_error_ht = new TH1F("h_qcdclosure_error_ht",";HT [GeV];entries",ht_nbins,ht_binning);
   fOutput->Add(h_qcdclosure_error_ht);
*/  

/*
   //Second iteration: Read weight-histograms, calculate weight hist
   TFile pfile( "results/WeightHists_photonTree.root" );
   TFile ffile( "results/WeightHists_photonJetTree.root", "READ" );
   if (pfile.IsZombie() || ffile.IsZombie()) {
       std::cerr << "Error opening file, quitting MySelector::SlaveBegin(TTree * tree="<<tree<<")" << std::endl;
       return;
   }
   TH2F* n_photon= (TH2F*)pfile.Get("h_lowmet_pt_ht"); 
   TH2F* w_photon= (TH2F*)pfile.Get("w_lowmet_pt_ht"); 
   TH2F* n_fake= (TH2F*)ffile.Get("h_lowmet_pt_ht"); 
   TH2F* w_fake= (TH2F*)ffile.Get("w_lowmet_pt_ht"); 

   if (!n_photon || !w_photon || !n_fake || !w_fake) 
     std::cerr << "ERROR: Reading weight histograms from file failed!" 
               << n_photon <<"; "<<w_photon<<"; "<<n_fake<<"; "<<w_fake<<std::endl;
   else {
     h_weight = (TH2F*)w_photon->Clone();
     h_weight->Divide( w_fake );
     h_err = (TH2F*)n_photon->Clone();

     for (int x=0; x<=n_photon->GetXaxis()->GetNbins()+1;++x) {
       for (int y=0; y<=n_photon->GetYaxis()->GetNbins()+1;++y) {
	  h_err->SetBinContent(x, y, (w_fake->GetBinContent(x,y)!=0?w_photon->GetBinContent(x,y)/w_fake->GetBinContent(x,y):0) );
     } }

     TFile* file = new TFile("results/QcdWeight.root","UPDATE");
     file->cd();
     h_weight->Write("h_weight");
     h_err->Write("h_err");
     file->Write();
     file->Close();
     delete file;
     
     gStyle->SetPaintTextFormat("1.1f");
     TCanvas *canv = new TCanvas("c_weight", "QCD Weight",1,1,600,600);
     h_weight->Draw("COLZ, TEXT");
     canv->SaveAs("results/h_weight.pdf");
     h_err->Draw("COLZ, TEXT");
     canv->SaveAs("results/h_err.pdf");
     delete canv;
  }
*/
}

/// End of Initialization ///
//===========================================================================================================================//


//=== -2- ===================================================================================================================//
/// Start Processing ///


bool MySelector::doQcdWeight() const 
{     
  h_pt->Fill( pt );  
  w_pt->Fill( pt, weight );
  h_ht->Fill( ht );
  w_ht->Fill( ht, weight );
  h_met->Fill( met );
  w_met->Fill( met, weight );
  h_pt_ht->Fill( pt, ht );
  w_pt_ht->Fill( pt, ht, weight );
  
  return true;
}



bool MySelector::doQcdClosure() const 
{
   return true;
}


Bool_t MySelector::Process(Long64_t entry)
{
   //read full event:
   fChain->GetTree()->GetEntry(entry);
 
   //read only specific branches:
   //b_met->GetEntry(entry);
   //b_ht->GetEntry(entry);
   //b_photon_pt->GetEntry(entry);   
 
   bool result = false;
 
   //QCD weighting stuff
   result |= doQcdWeight();
   
   //QCD closure
   //result |= doQcdClosure();

   //print Progress
   if (entry%(nentries/10)==0)std::cerr<<"->"<<entry/(nentries/10)*10<<"%";
   if (entry==nentries-1)std::cerr<<std::endl;

   //neither tight nor loose photon - useless event   
   return result;  
}

/// End of processing ///
//=== -2 - ==================================================================================================================//


//=== -3- ===================================================================================================================//
/// Start post-processing ///


template<typename T>
void MySelector::GetAndWrite(const std::string hist, const std::string fname, const std::string opt) const
{
  T* h = dynamic_cast<T*>(fOutput->FindObject(hist.c_str()));
  if (!h) {
    std::cerr << "ERROR: Cannot find histogram '"<<hist<<"'! GetAndWrite failed!"<<std::endl;
    return;
  }
  TFile* file = new TFile(fname.c_str(),opt.c_str());
  if (!file || file->IsZombie()) {
    std::cerr<<"ERROR: Cannot "<<opt<<" file "<<fname<<std::endl;
  } else {
    file->cd();
    h->Write(hist.c_str());
    file->Write();
    file->Close();
    delete file;  
  }

  TCanvas *canv = new TCanvas((hist+fname).c_str(), fname.c_str(),1,1,600,600);
  h->Draw("COLZ");
  canv->SaveAs(("results/"+hist+".pdf").c_str());
  delete canv;
}

void MySelector::QcdWeightHistogram() const
{
  std::string filename = "results/WeightHists_"+(std::string)fChain->GetName()+".root";
  GetAndWrite<TH1F>("h_lowmet_pt",	filename, "RECREATE");
  GetAndWrite<TH1F>("w_lowmet_pt",	filename, "UPDATE");
  GetAndWrite<TH1F>("h_lowmet_ht",	filename, "UPDATE");
  GetAndWrite<TH1F>("w_lowmet_ht",	filename, "UPDATE");
  GetAndWrite<TH1F>("h_lowmet_met",	filename, "UPDATE");
  GetAndWrite<TH1F>("w_lowmet_met",	filename, "UPDATE");
  GetAndWrite<TH2F>("h_lowmet_pt_ht",	filename, "UPDATE");
  GetAndWrite<TH2F>("w_lowmet_pt_ht",   filename, "UPDATE");
}


void PlotClosure(TCanvas*c, std::string filename, TH1*signal, TH1*prediction, TH1*error)
{
   if (!prediction || !signal) return;
   c->cd();
   gPad->SetLogy(0);
   if (signal->GetMaximum()>prediction->GetMaximum())prediction->SetMaximum(signal->GetMaximum());
   prediction->SetMaximum(prediction->GetMaximum()+sqrt(prediction->GetMaximum()));
   prediction->Draw("h");
   error->SetLineColor(2);
   error->Draw("h,same");
   signal->Draw("pe,same");
   c->SaveAs(("QcdClosure/"+filename+"_lin.pdf").c_str());

   gPad->SetLogy(1);
   prediction->SetMaximum(2*prediction->GetMaximum());
   prediction->Draw("h");
   error->SetLineColor(2);
   signal->Draw("pe,same");
   c->SaveAs(("QcdClosure/"+filename+"_log.pdf").c_str());
   gPad->SetLogy(0);
}

void MySelector::QcdClosure() const
{
   TH1F *h_signal_photonpt     = dynamic_cast<TH1F*>(fOutput->FindObject("h_qcdclosure_signal_photonpt"));
   TH1F *h_prediction_photonpt = dynamic_cast<TH1F*>(fOutput->FindObject("h_qcdclosure_prediction_photonpt"));
   TH1F *h_error_photonpt      = dynamic_cast<TH1F*>(fOutput->FindObject("h_qcdclosure_error_photonpt"));
   TH1F *h_signal_met          = dynamic_cast<TH1F*>(fOutput->FindObject("h_qcdclosure_signal_met"));
   TH1F *h_prediction_met      = dynamic_cast<TH1F*>(fOutput->FindObject("h_qcdclosure_prediction_met"));
   TH1F *h_error_met           = dynamic_cast<TH1F*>(fOutput->FindObject("h_qcdclosure_error_met"));
   TH1F *h_signal_ht           = dynamic_cast<TH1F*>(fOutput->FindObject("h_qcdclosure_signal_ht"));
   TH1F *h_prediction_ht       = dynamic_cast<TH1F*>(fOutput->FindObject("h_qcdclosure_prediction_ht"));
   TH1F *h_error_ht            = dynamic_cast<TH1F*>(fOutput->FindObject("h_qcdclosure_error_ht"));

   TFile* file = new TFile("results/weight.root","recreate");
   file->cd();
   h_signal_photonpt->Write("SignalSelectionPhotonPt");
   h_prediction_photonpt->Write("PredictionPhotonPt");
   file->Write();
   file->Close();
   delete file;
   
// Create a canvas
   TCanvas *canv = new TCanvas("c_closure", "QCD Closure",1,1,600,600);
   PlotClosure(canv, "photonpt", h_signal_photonpt, h_prediction_photonpt, h_error_photonpt);
   PlotClosure(canv, "met",      h_signal_met,      h_prediction_met, h_error_met);
   PlotClosure(canv, "ht",       h_signal_ht,       h_prediction_ht,  h_error_ht);
}






void MySelector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
  
   //TCanvas *c2 = new TCanvas("c2", "c2",3,3,600,600);
   //h->Draw("h");
   //c2->Update();
}


void MySelector::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
   
   QcdWeightHistogram();
   //QcdClosure();
}

/// End of post-processing ///
//=== -3- ===================================================================================================================//
