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
    h->Fill( var(*it), (*it)->EvtWgt );
  }  
}

float HT(const Event * evt){ return evt->MHT(); }


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

  //STL container for all events
  std::vector<Event*> eventsA, eventsB;

  ReadEvents( "QCDcontrol_data.root", eventsA);
  ReadEvents( "QCDcontrol_rs.root", eventsB );

  //Fill histograms
  TH1F* ht_A = new TH1F("ht_A",";MHT (GeV);events",100,0,1000);
  TH1F* ht_B = (TH1F*)ht_A->Clone();  
  FillHist(ht_A, eventsA, HT);
  FillHist(ht_B, eventsB, HT);
  ht_B->SetMarkerStyle(8);
  
  
  //plotting
  TCanvas c1;
  ht_B->Scale( ht_A->Integral()/ht_B->Integral() );
  if (ht_B->GetMaximum()>ht_A->GetMaximum()) ht_A->SetMaximum( ht_B->GetMaximum() );
  ht_A->Draw("h");
  ht_B->Draw("pe, same");
  TLegend * leg = new  TLegend(0.50, 0.50, 0.9, 0.9);
  leg->SetBorderSize(1);leg->SetFillColor(0);
  leg->SetHeader("Comparison Plots");
  leg->AddEntry(ht_A, "Sample A", "l");
  leg->AddEntry(ht_B, "Sample B", "pe");
  leg->Draw();
  
  c1.SaveAs("Plot.pdf");

  return 0;
}
