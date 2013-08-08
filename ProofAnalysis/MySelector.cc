
#include "MySelector.h"

#include <TH2.h>
#include <TCanvas.h>
#include <TStyle.h>
#include "TMath.h"
#include "TFile.h"

#include <complex>
#include <iostream>
#include <string>



void MySelector::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   

}

void MySelector::SlaveBegin(TTree * tree)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   //initialize the Tree branch addresses
   Init(tree);

   //print the option specified in the Process function.
   TString option = GetOption();
   Info("SlaveBegin",
        "starting h1analysis with process option: %s (tree: %p)", option.Data(), tree);

   float binning[] = {0, 70, 80, 90, 100, 120, 140, 160, 180, 200, 250, 300, 350, 400, 500, 600, 700, 800};
   int nbins = 17;

   h_weight_photonpt = new TH1F("h_weight_photonpt",";photon pT [GeV];entries",nbins,binning);
   h_weight_fakept = new TH1F("h_weight_fakept",";fake-photon pT [GeV];entries",nbins,binning);

   fOutput->Add(h_weight_photonpt);
   fOutput->Add(h_weight_fakept);
}

float DeltaPhi(float a, float b) {
  float temp = std::abs(a-b);
  if (temp <= TMath::Pi())
    return temp;
  else
    return  2.*TMath::Pi() - temp;
} 

bool MySelector::QcdWeightSelection() const
{
  return (met<100. && ht>450.);
}

bool MySelector::isTightIsolated(unsigned i) const
{
    float pt = photon_pt[i];
    return ( photon_hadTowOverEm[i]  < 0.05  &&
             photon_sigmaIetaIeta[i] < 0.012 &&
             photon_chargedIso[i]    < 2.6    &&
             photon_neutralIso[i]    < 3.5 + 0.04*pt  &&
             photon_photonIso[i]     < 1.3 + 0.005*pt );
}


bool MySelector::GetIdx_TightPhoton(unsigned * idx) const
{
  for (unsigned i= *idx; i<(unsigned)kMaxphoton; ++i) {
    if (  photon_pt[i] < 80.              ) continue;
    if ( std::abs(photon_eta[i]) > 1.4442 ) continue;
    if ( photon_pixelseed[i]              ) continue;
    if ( !isTightIsolated(i)              ) continue;
    *idx=i;	
    return true; 
  }
  return false;
}

bool MySelector::GetIdx_FakePhoton(unsigned * idx) const
{
  for (unsigned i= *idx; i<(unsigned)kMaxphoton; ++i) {
    //same as tight
    float pt = photon_pt[i];
    if ( pt < 80.                         ) continue;
    if ( std::abs(photon_eta[i]) > 1.4442 ) continue;
    if ( photon_pixelseed[i]              ) continue;
    //loose isolation
    if ( photon_sigmaIetaIeta[i] > 0.014       ) continue;
    if ( photon_hadTowOverEm[i]  > 0.15        ) continue;
    if ( photon_chargedIso[i]    > 4.6          ) continue;
    if ( photon_neutralIso[i]    > 4.5 + 0.04*pt ) continue;
//    if ( photon_photonIso[i]     > 4 + 0.01*pt ) continue;
    //not tight isolated!
    if ( isTightIsolated(i)          ) continue;

    *idx=i;	
    return true; 
  }
  return false;
}


Bool_t MySelector::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either MySelector::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.


   //read full event:
   //fChain->GetTree()->GetEntry(entry);
 
   //read only specific branches:
   b_met->GetEntry(entry);
   b_ht->GetEntry(entry);
   b_photon_pt->GetEntry(entry);   
   b_photon_eta->GetEntry(entry);
   b_photon_pixelseed->GetEntry(entry);
   b_photon_sigmaIetaIeta->GetEntry(entry);
   b_photon_hadTowOverEm->GetEntry(entry);
   b_photon_chargedIso->GetEntry(entry);
   b_photon_neutralIso->GetEntry(entry);
   b_photon_photonIso->GetEntry(entry);
 
   if (!QcdWeightSelection()) return kFALSE;

   unsigned idx_tight_photon = 0;
   unsigned idx_fake_photon  = 0;

   //check if we have a tight (signal) photon
   if (GetIdx_TightPhoton( &idx_tight_photon )) {
     h_weight_photonpt->Fill( photon_pt[idx_tight_photon] );  
     return kTRUE;
   }   
   //no signal photon: search for a fake (loose) photon-object
   for (;idx_fake_photon<(unsigned)kMaxphoton;++idx_fake_photon){
       if (!GetIdx_FakePhoton( &idx_fake_photon ) ) continue;
       float pt_fake = photon_pt[idx_fake_photon];
       //if (photon_neutralIso[idx_fake_photon]    < 3.5 + 0.04*pt_fake ) continue;  
       if (photon_photonIso[idx_fake_photon]     < 1.3 + 0.005*pt_fake) continue;
       h_weight_fakept->Fill( photon_pt[idx_fake_photon] );
       return kTRUE;
   }

   //neither tight nor loose photon - useless event   
   return kFALSE;  
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
   

   h_weight_photonpt = dynamic_cast<TH1F *>(fOutput->FindObject("h_weight_photonpt"));
   h_weight_fakept   = dynamic_cast<TH1F *>(fOutput->FindObject("h_weight_fakept"));
   std::string fileName = "QcdWeight.root";
   
// Create a canvas, with 100 pads
   TCanvas *c1 = new TCanvas("c1", "Proof canvas 1",3,3,600,600);
   c1->cd();
   if (h_weight_photonpt) h_weight_photonpt->Draw("h");
   c1->Update();

   TCanvas *c2 = new TCanvas("c2", "Proof canvas 2",3,3,600,600);
   c2->cd();
   if (h_weight_fakept) h_weight_fakept->Draw("h");
   c2->Update();

   if (h_weight_fakept && h_weight_photonpt) {
     TCanvas *c3 = new TCanvas("c3", "Proof canvas 3",3,3,600,600);
     c3->cd();
     
     h_weight_fakept->Sumw2();
     h_weight_photonpt->Sumw2();
     TH1F * h_weight = (TH1F*)h_weight_photonpt->Clone();
     h_weight->Divide( h_weight_fakept );
/*
     TH1F * h_w1m = (TH1F*)h_weight_photonpt->Clone();
     TH1F * h_w1p = (TH1F*)h_weight_photonpt->Clone();
     for (int i=0; i<=h_w1m->GetXaxis()->GetNbins()+1;++i) {
       h_w1m->SetBinContent(i, h->GetBinContent(i)-h->GetBinError(i));
       h_w1p->SetBinContent(i, h->GetBinContent(i)+h->GetBinError(i));
     }
     h_w1m->SetLineColor(2);
     h_w1p->SetLineColor(8);
*/
     h_weight->GetXaxis()->SetTitle("photon-object pT [GeV]");
     h_weight->GetYaxis()->SetTitle("QCD weight");
     h_weight->GetYaxis()->SetTitleOffset(1.4);
     h_weight->Draw("pe");
     c3->Update();
 
     TFile* file = new TFile(fileName.c_str(),"recreate");
     file->cd();
     h_weight->Write("QcdWeight");
     h_weight_photonpt->Write("PhotonPt");
     h_weight_fakept->Write("FakePt");
     file->Write();
     file->Close();
     delete file;
   }
}
