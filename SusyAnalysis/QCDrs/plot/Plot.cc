#include "Plot.h"
#include "Event.h"

#include "TH1.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"

#include <iostream>

void FillHist(TH1*h, const std::vector<Event*>& evts, float(*var)(const Event*) )
{
  for (std::vector<Event*>::const_iterator it=evts.begin(); it!=evts.end(); ++it) {
    //if ( (*it)->HT()<350. || (*it)->MHT()<100 ) continue;
    if ( (*it)->recoJetPt[0]<250. || (*it)->recoJetPt[1]<150.||
         (*it)->recoJetPt[2]<150. || (*it)->HT()<300. ) continue;
    h->Fill( var(*it), (*it)->EvtWgt );
  }  
}

float HT(const Event * evt){ return evt->HT(); }
float MHT(const Event * evt){ return evt->MHT(); }
float Jet1Pt(const Event * evt){ return evt->recoJetPt[0]; }
float Jet2Pt(const Event * evt){ return evt->recoJetPt[1]; }
float Jet3Pt(const Event * evt){ return evt->recoJetPt[2]; }
float JetMultiplicity(const Event * evt){ int N=0; for (int i=0;i<evt->NrecoJet;++i) if (evt->recoJetPt[i]>50.) ++N;return N; }


int main(int argc, char* argv[])
{
  gStyle->SetOptStat(0);
  gStyle->SetErrorX(0);
  gStyle->SetPalette(1);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(kWhite);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(1);
  gStyle->SetFrameLineStyle(0);
  gStyle->SetFrameLineWidth(1);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadColor(kWhite);
  gStyle->SetPadGridX(false);
  gStyle->SetPadGridY(false);
  gStyle->SetGridColor(0);
  gStyle->SetGridStyle(3);
  gStyle->SetGridWidth(1);

  TCanvas c1;
  c1.SetLogy(1);

  //STL container for all events
  std::vector<Event*> eventsA, eventsB, eventsC;

  ReadEvents( "data/QCDcontrol_data.root", eventsA);
  ReadEvents( "data/QCDcontrol_rebalanced.root", eventsB );
  ReadEvents( "data/QCDcontrol_rs.root", eventsC );

  //Fill histograms
  TLegend * leg = new  TLegend(0.50, 0.50, 0.9, 0.9);
  { TH1F* ht_A = new TH1F("ht_A",";HT (GeV);events",100,300,2000);
    TH1F* ht_B = (TH1F*)ht_A->Clone();  
    TH1F* ht_C = (TH1F*)ht_A->Clone();  
    FillHist(ht_A, eventsA, HT);
    FillHist(ht_B, eventsB, HT);
    FillHist(ht_C, eventsC, HT);
    ht_B->SetLineColor(2);ht_C->SetMarkerStyle(8);
    ht_B->Scale( ht_A->Integral()/ht_B->Integral() );
    ht_C->Scale( ht_A->Integral()/ht_C->Integral() );
    if (ht_B->GetMaximum()>ht_A->GetMaximum()) ht_A->SetMaximum( ht_B->GetMaximum() );
    if (ht_C->GetMaximum()>ht_A->GetMaximum()) ht_A->SetMaximum( ht_C->GetMaximum() );
    ht_A->Draw("h");
    ht_B->Draw("h, same");
    ht_C->Draw("pe, same");
    leg->SetBorderSize(1);leg->SetFillColor(0);
    leg->SetHeader("Comparison Plots");
    leg->AddEntry(ht_A, "Data control", "l");
    leg->AddEntry(ht_B, "Rebalanced", "l");
    leg->AddEntry(ht_C, "QCD R+S", "pe");
    leg->Draw();
    c1.SaveAs("HT.pdf");
  }  
  { TH1F* ht_A = new TH1F("mht_A",";MHT (GeV);events",100,0,800);
    TH1F* ht_B = (TH1F*)ht_A->Clone();  
    TH1F* ht_C = (TH1F*)ht_A->Clone();  
    FillHist(ht_A, eventsA, MHT);
    FillHist(ht_B, eventsB, MHT);
    FillHist(ht_C, eventsC, MHT);
    ht_B->SetLineColor(2);ht_C->SetMarkerStyle(8);
    ht_B->Scale( ht_A->Integral()/ht_B->Integral() );
    ht_C->Scale( ht_A->Integral()/ht_C->Integral() );
    if (ht_B->GetMaximum()>ht_A->GetMaximum()) ht_A->SetMaximum( ht_B->GetMaximum() );
    if (ht_C->GetMaximum()>ht_A->GetMaximum()) ht_A->SetMaximum( ht_C->GetMaximum() );
    ht_A->Draw("h");
    ht_B->Draw("h, same");
    ht_C->Draw("pe, same");
    leg->Draw();
    c1.SaveAs("MHT.pdf");
  }  
  { TH1F* ht_A = new TH1F("jet1pt_A",";jet 1 pT (GeV);events",90,100,1000);
    TH1F* ht_B = (TH1F*)ht_A->Clone();  
    TH1F* ht_C = (TH1F*)ht_A->Clone();  
    FillHist(ht_A, eventsA, Jet1Pt);
    FillHist(ht_B, eventsB, Jet1Pt);
    FillHist(ht_C, eventsC, Jet1Pt);
    ht_B->SetLineColor(2);ht_C->SetMarkerStyle(8);
    ht_B->Scale( ht_A->Integral()/ht_B->Integral() );
    ht_C->Scale( ht_A->Integral()/ht_C->Integral() );
    if (ht_B->GetMaximum()>ht_A->GetMaximum()) ht_A->SetMaximum( ht_B->GetMaximum() );
    if (ht_C->GetMaximum()>ht_A->GetMaximum()) ht_A->SetMaximum( ht_C->GetMaximum() );
    ht_A->Draw("h");
    ht_B->Draw("h, same");
    ht_C->Draw("pe, same");
    leg->Draw();
    c1.SaveAs("jet1pt.pdf");
  }  
  { TH1F* ht_A = new TH1F("jet2pt_A",";jet 2 pT (GeV);events",90,100,1000);
    TH1F* ht_B = (TH1F*)ht_A->Clone();  
    TH1F* ht_C = (TH1F*)ht_A->Clone();  
    FillHist(ht_A, eventsA, Jet2Pt);
    FillHist(ht_B, eventsB, Jet2Pt);
    FillHist(ht_C, eventsC, Jet2Pt);
    ht_B->SetLineColor(2);ht_C->SetMarkerStyle(8);
    ht_B->Scale( ht_A->Integral()/ht_B->Integral() );
    ht_C->Scale( ht_A->Integral()/ht_C->Integral() );
    if (ht_B->GetMaximum()>ht_A->GetMaximum()) ht_A->SetMaximum( ht_B->GetMaximum() );
    if (ht_C->GetMaximum()>ht_A->GetMaximum()) ht_A->SetMaximum( ht_C->GetMaximum() );
    ht_A->Draw("h");
    ht_B->Draw("h, same");
    ht_C->Draw("pe, same");
    leg->Draw();
    c1.SaveAs("jet2pt.pdf");
  }  
  { TH1F* ht_A = new TH1F("jet3pt_A",";jet 3 pT (GeV);events",90,100,1000);
    TH1F* ht_B = (TH1F*)ht_A->Clone();  
    TH1F* ht_C = (TH1F*)ht_A->Clone();  
    FillHist(ht_A, eventsA, Jet3Pt);
    FillHist(ht_B, eventsB, Jet3Pt);
    FillHist(ht_C, eventsC, Jet3Pt);
    ht_B->SetLineColor(2);ht_C->SetMarkerStyle(8);
    ht_B->Scale( ht_A->Integral()/ht_B->Integral() );
    ht_C->Scale( ht_A->Integral()/ht_C->Integral() );
    if (ht_B->GetMaximum()>ht_A->GetMaximum()) ht_A->SetMaximum( ht_B->GetMaximum() );
    if (ht_C->GetMaximum()>ht_A->GetMaximum()) ht_A->SetMaximum( ht_C->GetMaximum() );
    ht_A->Draw("h");
    ht_B->Draw("h, same");
    ht_C->Draw("pe, same");
    leg->Draw();
    c1.SaveAs("jet3pt.pdf");
  }  
  { 
    c1.SetLogy(1);
    TH1F* ht_A = new TH1F("Njets_A",";jet multiplicity;events",20,0,20);
    TH1F* ht_B = (TH1F*)ht_A->Clone();  
    TH1F* ht_C = (TH1F*)ht_A->Clone();  
    FillHist(ht_A, eventsA, JetMultiplicity);
    FillHist(ht_B, eventsB, JetMultiplicity);
    FillHist(ht_C, eventsC, JetMultiplicity);
    ht_B->SetLineColor(2);ht_C->SetMarkerStyle(8);
    ht_B->Scale( ht_A->Integral()/ht_B->Integral() );
    ht_C->Scale( ht_A->Integral()/ht_C->Integral() );
    if (ht_B->GetMaximum()>ht_A->GetMaximum()) ht_A->SetMaximum( ht_B->GetMaximum() );
    if (ht_C->GetMaximum()>ht_A->GetMaximum()) ht_A->SetMaximum( ht_C->GetMaximum() );
    ht_A->SetMinimum(0.1);
    ht_A->Draw("h");
    ht_B->Draw("h, same");
    ht_C->Draw("pe, same");
    leg->Draw();
    c1.SaveAs("jetMultiplicity.pdf");
  }  

  return 0;
}
