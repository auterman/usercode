// $Id: CutStudies.cc,v 1.1 2013/06/12 10:23:17 mglagla Exp $

/*** ------------------------------------------------------------------------------------------------------- ***
     NTupleFactory, a tool to plot final results, using Root Reflex information

     Christian Autermann, RWTH Aachen, May 2013
 *** ------------------------------------------------------------------------------------------------------- ***/

#include "CutStudies.h"
#include "Event.h"
#include "Selection.h"
#include "Analyzer.h"
#include "Factory.h"

#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"

 

#include <string>

//The main function 
int main(int argc, char* argv[])
{
  TCanvas c1("c1","c1",600,600);
  
  /*const float wjetsCross = 12503;
  const float qcd250Cross = 276000;
  const float qcd500Cross = 8426;
  const float qcd1000Cross = 204;
  const float ttjetsCross = 225.197;
  const float gjetsCross = 107.5;
  
  const float integratedLumi = 19300; //pb
  */
  const std::string dir = "input/";
  const std::string slim = "slim";
   
  //SampleWeight* gewicht = new SampleWeight();
  

  ///Factory approach ------------------------------------------------------------------------
  Factory thefactory;
  Factory datafactory;
  
  //Data
  std::vector<std::string> winofiles;
  winofiles.push_back(dir+"gsq_wino_1000_1020_375_tree.root");
  Sample wino_sample(winofiles,"susyTree","WinoSelector","Wino","Wino",1);
  wino_sample.SetLineColor(kRed);
  datafactory.AddSample(&wino_sample);
  
  std::vector<std::string> binofiles;
  binofiles.push_back(dir+"gsq_bino_1200_1420_375_tree.root");
  Sample bino_sample(binofiles,"susyTree","BinoSelector","Bino","Bino",1);
  bino_sample.SetLineColor(kBlue);
  datafactory.AddSample(&bino_sample);
  
  
  //Wjets sample
  std::vector<std::string> wjetsfiles;
  wjetsfiles.push_back(dir+slim+"WJets_V01.07_tree.root");
  Sample wjet_sample(wjetsfiles,"susyTree","WjetsSelector","WJets","WJets",1);
  wjet_sample.SetFillColor(kOrange);
  thefactory.AddSample( &wjet_sample );
  
  //QCD sample
  
  std::vector<std::string> qcdfiles;
  qcdfiles.push_back(dir+slim+"QCD_250_500_V01.07_tree.root");
  qcdfiles.push_back(dir+slim+"QCD_500_1000_V01.07_tree.root");
  qcdfiles.push_back(dir+slim+"QCD_1000_inf_V01.07_tree.root");
  Sample qcd_sample(qcdfiles ,"susyTree","QcdSelector","QCD","QCD",1);
  qcd_sample.SetFillColor(kCyan);
  thefactory.AddSample( &qcd_sample );
  
  //TT sample
  std::vector<std::string> ttfiles;
  ttfiles.push_back(dir+slim+"TTJets_V01.07_tree.root");
  Sample tt_sample(ttfiles,"susyTree","TtjetsSelector","TTJets","TTJets",1);
  tt_sample.SetFillColor(kYellow);
  thefactory.AddSample( &tt_sample );
  
  //GJets Sample
  std::vector<std::string> gjetsfiles;
  gjetsfiles.push_back(dir+slim+"GJets_V01.07_tree.root");
  Sample gjet_sample(gjetsfiles,"susyTree","GjetsSelector","GJets","GJets",1);
  gjet_sample.SetFillColor(kGreen);
  thefactory.AddSample( &gjet_sample );
  
  
  //delete gewicht;
  
  Float_t Bin_array [] = {0,10,20,30,40,50,60,70,80,90,100,120,160,200,270,350,500};
  Int_t N_Bins = 16;
   
  //Selections
  Selection * inclusive = new Selection("Inclusive");
  Selection * presel    = new PreSelection("PreSel");
  Selection * finsel	= new FinalSelection("FinSel");
  //Selection * consel	= new ControlSelection("ConSel");
 
  //Some Analyzer tasks ---
  //give 20 status updates, i.e. every 5%
  Analyzer* status = new PrintStatus(20); 
  thefactory.AddAnalyzer( inclusive, status ); 
  datafactory.AddAnalyzer( inclusive, status);
  //cutflow table
  CutFlow* cutflow = new CutFlow( inclusive );
  cutflow->AddSelection( presel );
  thefactory.AddAnalyzer( inclusive, cutflow ); 
  //1D plot of met for every sample
  TH1 * h_met = new TH1F("met",";Met [GeV]; events",N_Bins,Bin_array);
  Analyzer* plot_Met = new Plot1D(&c1,h_met,new FloatVariable("met"),new FloatArrayVariable("jet_pt",0));
  thefactory.AddAnalyzer( presel, plot_Met ); 
  datafactory.AddAnalyzer( finsel, plot_Met);
  //2D plot
  TH2 * h_met_vs_jet1pt = new TH2F("met_jet1pt",";Met [GeV]; Jet 1 p_{T}; events",100,0,500,100,0,500);
  Analyzer* plot_Met_vs_Jet1Pt = new Plot2D(&c1,h_met_vs_jet1pt,new FloatVariable("met"),new FloatArrayVariable("jet_pt",0));
  thefactory.AddAnalyzer( presel, plot_Met_vs_Jet1Pt ); 
  //datafactory.AddAnalyzer( inclusive, plot_Met_vs_Jet1Pt);
  //1D stacked plot of met
  TH1 * h_met_templ = new TH1F("mettmpl",";Met [GeV]; events",100,0,1000);//template plot for style etc
  h_met_templ->SetStats(0);
  Analyzer* stacked_Met = new Stack(&c1,h_met_templ,new FloatVariable("met") );
  thefactory.AddAnalyzer( presel, stacked_Met ); 
  //1D stacked plot of met with FinalSelection
  
  TH1 * h_met_fin = new TH1F("met_fin",";Met [GeV]; events",N_Bins,Bin_array);
  h_met_fin->SetStats(0);
  Analyzer* stacked_Met_fin = new Stack(&c1,h_met_fin,new FloatVariable("met") );
  thefactory.AddAnalyzer( finsel, stacked_Met_fin );
  
  TH1 * h_photon_pt_fin = new TH1F("photon_pt_fin",";#gamma_{P_{T}} [GeV]; events",100,0,1000);
  h_photon_pt_fin->SetStats(0);
  Analyzer* stacked_Photon_Pt_fin = new Stack(&c1,h_photon_pt_fin,new FloatArrayVariable("photon_pt",0) );
  thefactory.AddAnalyzer( finsel, stacked_Photon_Pt_fin );
  
  TH1 * h_ht_fin = new TH1F("ht_fin",";H_{T} [GeV]; events",100,0,1000);
  h_ht_fin->SetStats(0);
  Analyzer* stacked_Ht_fin = new Stack(&c1,h_ht_fin,new FloatVariable("ht") );
  thefactory.AddAnalyzer( finsel, stacked_Ht_fin );
  
  TH1 * h_photon_eta_fin = new TH1F("photon_eta_fin",";#eta ; events",100,-4,4);
  h_photon_eta_fin->SetStats(0);
  Analyzer* stacked_Photon_Eta_fin = new Stack(&c1,h_photon_eta_fin,new FloatArrayVariable("photon_eta",0) );
  thefactory.AddAnalyzer( finsel, stacked_Photon_Eta_fin );
  
  TH1 * h_jet_eta_fin = new TH1F("jet_eta_fin",";#eta ; events",100,-4,4);
  h_jet_eta_fin->SetStats(0);
  Analyzer* stacked_Jet_Eta_fin = new Stack(&c1,h_jet_eta_fin,new FloatArrayVariable("jet_eta",0) );
  thefactory.AddAnalyzer( finsel, stacked_Jet_Eta_fin );
  
  TH1 * h_jet_pt_fin = new TH1F("jet_pt_fin",";Jet_{P_{T}} [GeV]; events",100,0,1000);
  h_jet_pt_fin->SetStats(0);
  Analyzer* stacked_Jet_Pt_fin = new Stack(&c1,h_jet_pt_fin,new FloatArrayVariable("jet_pt",0) );
  thefactory.AddAnalyzer( finsel, stacked_Jet_Pt_fin );
  
  TH1 * h_photon_pixelseed_fin = new TH1F("photon_pixelseed_fin","; ; events",100,0,10);
  h_photon_pixelseed_fin->SetStats(0);
  Analyzer* stacked_Photon_Pixelseed_fin = new Stack(&c1,h_photon_pixelseed_fin,new FloatArrayVariable("photon_pixelseed",0) );
  thefactory.AddAnalyzer( finsel, stacked_Photon_Pixelseed_fin );

 /* TH1 * h_photon_pt_con = new TH1F("photon_pt_con",";{#gamma}_{{P}_{T}} [GeV]; events",100,0,100);
  h_photon_pt_con->SetStats(0);
  Analyzer* stacked_Photon_Pt_con = new Stack(&c1,h_photon_pt_con,new FloatArrayVariable("photon_pt",0) );
  thefactory.AddAnalyzer( consel, stacked_Photon_Pt_con );
	*/
  //Analyzer* PrintVariables = new Print(new FloatVariable("met"),new FloatArrayVariable("jet_pt",0),new FloatArrayVariable("jet_pt",1));
  //thefactory.AddAnalyzer( 1, PrintVariables ); 

  //process events
  thefactory.Process();
  datafactory.Process();


  return 0;
}
