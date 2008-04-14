//System
#include <memory>
#include <string>
#include <fstream>
#include <iostream>
//Root
#include <TF1.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>
#include <TTree.h>
#include <TPostScript.h>
#include <TMath.h>
#include <TMatrix.h>
#include <TFormula.h>
#include <TAxis.h>
#include <TAttLine.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>
//User
#include "ConfigFile.h"
#include "NiceStyle.cc"

using namespace std;

class ValPlotMaker{
public:
  ValPlotMaker(){};
  ~ValPlotMaker(){};
  
  void readConfig( std::string );
  void loadHistograms();
  void loadHistograms(std::vector<std::string> filename, std::vector<string> *histnames, std::vector<TH1F*> *hist);
  void drawComparison();
  
protected:   
  bool contains(const string the_string, const string the_sub_string) const;
  //Style Stuff
  void setHistStyle(TH1F*, const char*, const char*);
  void setCanvasStyle( TCanvas& canv );
  void setLegendStyle( TLegend& leg );
  
private:
  //define input/output
  std::string treename_, output_, name_data_, name_reference_;
  std::vector<std::string> input_, reference_;
  
  //define histogram design
  std::vector<short int> cmpColor_, cmpStyle_, cmpWidth_; 
  
  //define canvas design
  int gridX_, gridY_;
  
  //define legend design
  double legXLeft_,  legXRight_;
  double legYLower_, legYUpper_;
  
  //bins
  std::vector<int> Bins_;
  
  //hists
  std::vector<std::string> HistNames_, Hist2DNames_;
  
  //validation hist
  std::vector<TH1F*> hists_, refs_;
  
  //background color of hists, if KS test fails
  double ks_treshold;
  int    ks_color;
  bool   UseTitle_;

};

bool ValPlotMaker::contains(const string the_string, const string the_sub_string) const
{
  return the_string.find(the_sub_string,0) != string::npos;
}

void ValPlotMaker::setHistStyle( TH1F* hist, const char* titleX, const char* titleY )
{
  //hist->SetTitle( "" );
  if ( hist->GetXaxis()->GetTitle()=="" ) 
    hist->GetXaxis()->SetTitle( titleX );
  hist->GetXaxis()->SetTitleSize  ( 0.04 );
  hist->GetXaxis()->SetTitleColor (    1 );
  hist->GetXaxis()->SetTitleOffset(  1.8 );
  hist->GetXaxis()->SetTitleFont  (   62 );
  hist->GetXaxis()->SetLabelSize  ( 0.05 );
  hist->GetXaxis()->SetLabelFont  (   62 );
  hist->GetXaxis()->CenterTitle   (      );
  hist->GetXaxis()->SetNdivisions (  505 );
  //hist->GetYaxis()->SetTitle( titleY );
  hist->GetYaxis()->SetTitleSize  ( 0.07 );
  hist->GetYaxis()->SetTitleColor (    1 );
  hist->GetYaxis()->SetTitleOffset(  1.3 );
  hist->GetYaxis()->SetTitleFont  (   62 );
  hist->GetYaxis()->SetLabelSize  ( 0.05 );
  hist->GetYaxis()->SetLabelFont  (   62 );
}
void ValPlotMaker::setCanvasStyle( TCanvas& canv )
{
  canv.SetFillStyle   ( 4000 );
  canv.SetLeftMargin  ( 0.20 );
  canv.SetRightMargin ( 0.05 );
  canv.SetBottomMargin( 0.15 );
  //canv.SetTopMargin   ( 0.05 );
}
void ValPlotMaker::setLegendStyle( TLegend& leg )
{
  leg.SetFillStyle ( 0 );
  leg.SetBorderSize( 0 );
  leg.SetFillColor ( 0 ); 
}

void ValPlotMaker::readConfig( std::string name )
{
  ConfigFile cfg( name );  
  input_       = bag_of_string(cfg.read<std::string>( "input_root" ));
  reference_   = bag_of_string(cfg.read<std::string>( "input_reference" ));
  name_data_      = cfg.read<std::string>( "name data", "data" );
  name_reference_ = cfg.read<std::string>( "name reference", "reference" );
  treename_    = cfg.read<std::string>( "tree_name" );
  output_      = cfg.read<std::string>( "output"   );
  HistNames_   = bag_of_string(cfg.read<std::string>( "hists"));
  ks_treshold  = cfg.read<double> ( "KS treshold", 0.05  );
  ks_color     = cfg.read<int>    ( "KS color", 5     );
  gridX_       = cfg.read<int>    ( "grid_x"	 );
  gridY_       = cfg.read<int>    ( "grid_y"	 );
  legXLeft_    = cfg.read<double> ( "leg_x_left"  );
  legXRight_   = cfg.read<double> ( "leg_x_right" );
  legYLower_   = cfg.read<double> ( "leg_y_lower" );
  legYUpper_   = cfg.read<double> ( "leg_y_upper" );
  cmpColor_    = bag_of<short int>( cfg.read<std::string>( "color" ));
  cmpStyle_    = bag_of<short int>( cfg.read<std::string>( "style" ));
  cmpWidth_    = bag_of<short int>( cfg.read<std::string>( "width" ));
  UseTitle_    = cfg.read<int>    ( "show title", false	 );
}


void ValPlotMaker::loadHistograms(vector<string> filenames, vector<string> *histnames, vector<TH1F*> *hists)
{
  std::vector<std::string>::iterator filename = filenames.begin();
  TFile * file_ = new TFile( (*filename).c_str() );
  TH1F * dummy;
  hists->clear();
  for (std::vector<std::string>::iterator it=histnames->begin(); 
       it!=histnames->end(); ) {
    string dummy_name = treename_+"/"+(*it);
    dummy = (TH1F*)file_->Get( dummy_name.c_str() );
    if( !dummy ){
      cerr << "WARNING:" 
	   << " Didn't find indicated hist" << " [" << (*it) << "]" 
	   << " in tree" << " [" << treename_ << "]" 
	   << " of file" << " [" << (*filename) << "]" 
	   << endl;
      it = histnames->erase( it ); 
    } else {
      hists->push_back( dummy );
      ++it;
    }
  }
  ++filename;
  for (; filename!=filenames.end(); ++filename){
    TFile * file_ = new TFile( (*filename).c_str() );
    int i=0;
    std::vector<TH1F*>::iterator hist=hists->begin();
    for (std::vector<std::string>::iterator it=histnames->begin(); 
	 it!=histnames->end()&&hist!=hists->end(); ++hist,++it,++i) {
      string dummy_name = treename_+"/"+(*it);
      dummy = (TH1F*)file_->Get( dummy_name.c_str() );
      if( dummy ){
	dummy->Add( (*hist) );
	hist = hists->erase(hist);
	hists->insert( hist, 1, dummy );
      }
    }
  }
  
  if (hists->size() > 0) {
    filename = filenames.begin();
    cout << "...loaded " << hists->size() 
	 <<" histograms from file(s) '" << (*filename) << "'." << endl;
  }     
}

void ValPlotMaker::loadHistograms()
{
  loadHistograms(input_,     &HistNames_, &hists_);
  loadHistograms(reference_, &HistNames_, &refs_);
  if (hists_.size()!=refs_.size()) {
    cerr << "ERROR: At least one histogram is only present in one input file!"
         << "The number of data and reference plots does not match." << endl;
    exit(2);	 
  }
  
}

void ValPlotMaker::drawComparison()
{
  TPostScript ps(output_.c_str(), 112 );
  TCanvas *c1 = new TCanvas("c1", "fit histograms", 600, 600);
  setCanvasStyle( *c1 );
  c1->SetGridx( 1 );
  c1->SetGridy( 1 );
  
  char * dummy_text = new char[100];
  unsigned i=0;
  double ks;
  std::vector<std::string>::iterator it=HistNames_.begin();
  std::vector<TH1F*>::iterator hst=hists_.begin();
  std::vector<TH1F*>::iterator ref=refs_.begin();
  
  for (;ref!=refs_.end()&&hst!=hists_.end()&&it!=HistNames_.end(); 
       ++ref,++hst,++it,++i){
    //cout << i << "th plot: " << (*it) << endl;
    
    TH1F& hist   = *((TH1F*)(*hst));
    TH1F& refer  = *((TH1F*)(*ref));
    //check if hist is filled
    if (refer.Integral()==0.0) {
      cerr << "Histogram '"<< (*it) <<"' is empty!" <<endl;
      continue;
    }	
    
    setHistStyle( &refer, it->c_str(), "events"  );
    if (UseTitle_){
      //refer.SetTitle( "" );
    }	
    else
      refer.SetTitle( "" );
    refer.SetLineWidth( 8 );
    refer.SetLineColor( 2 );
    
    cout << i << "th plot: " << (*it) 
	 << " has " << refer.Integral() << " 'reference' events and "
	 <<  hist.Integral() << " 'data' events" << endl;
    
    refer.Sumw2();
    if ( !contains(refer.GetYaxis()->GetTitle(), "eff") ) {
      refer.Scale( hist.Integral()/refer.Integral() );
    } else {
      if ( contains(refer.GetXaxis()->GetTitle(), "Pt") ) {
	hists_.at(6)->Sumw2();
	hists_.at(3)->Sumw2();
	hist.Divide(hists_.at(6),hists_.at(3),1.,1.,"B");
	refs_.at(6)->Sumw2();
	refs_.at(3)->Sumw2();
	refer.Divide(refs_.at(6),refs_.at(3),1.,1.,"B");
      }
      if ( contains(refer.GetXaxis()->GetTitle(), "eta") ) {
	hists_.at(7)->Sumw2();
	hists_.at(4)->Sumw2();
	hist.Divide(hists_.at(7),hists_.at(4),1.,1.,"B");
	refs_.at(7)->Sumw2();
	refs_.at(4)->Sumw2();
	refer.Divide(refs_.at(7),refs_.at(4),1.,1.,"B");
      }
      if ( contains(refer.GetXaxis()->GetTitle(), "phi") ) {
	hists_.at(8)->Sumw2();
	hists_.at(5)->Sumw2();
	hist.Divide(hists_.at(8),hists_.at(5),1.,1.,"B");
	refs_.at(8)->Sumw2();
	refs_.at(5)->Sumw2();
	refer.Divide(refs_.at(8),refs_.at(5),1.,1.,"B");
      }
    }
//  Statistical test of compatibility in shape between
//  THIS histogram and h2, using Kolmogorov test.
    ks = refer.KolmogorovTest( &hist, "");
//     Default: Ignore under- and overflow bins in comparison
//     option is a character string to specify options
//         "U" include Underflows in test  (also for 2-dim)
//         "O" include Overflows     (also valid for 2-dim)
//         "N" include comparison of normalizations
//         "D" Put out a line of "Debug" printout
//         "M" Return the Maximum Kolmogorov distance instead of prob
//         "X" Run the pseudo experiments post-processor with the following procedure:
//             make pseudoexperiments based on random values from the parent
//             distribution and compare the KS distance of the pseudoexperiment
//             to the parent distribution. Bin the KS distances in a histogram,
//             and then take the integral of all the KS values above the value
//             obtained from the original data to Monte Carlo distribution.
//             The number of pseudo-experiments nEXPT is currently fixed at 1000.
//             The function returns the integral.
//             (thanks to Ben Kilminster to submit this procedure). Note that
//             this option "X" is much slower.
    if (ks<ks_treshold){
      c1->SetFillColor(ks_color);
      cout << "Deviation in plot "<< i <<" '" << (*it) << "': " << ks 
	   << " (Kolmogorov-Smirnov Test)" << endl;
    }	
    else {
      c1->SetFillColor(0);
    }	
    
    double maximum = (refer.GetMaximum()>hist.GetMaximum() ?
		      1.5*refer.GetMaximum() : 1.5*hist.GetMaximum());
    refer.SetMaximum(maximum);
    hist.SetMarkerStyle(8);
    hist.SetLineWidth(3);
    refer.Draw("pe");
    hist.Draw("pe,same");
    TLegend* leg = new TLegend(0.5,0.67,0.95,0.87);
    setLegendStyle( *leg );
    sprintf(dummy_text,"KS = %2.2f",ks);
    leg->SetHeader(dummy_text);
    leg->AddEntry( &hist, name_data_.c_str(), "PE" );
    leg->AddEntry( &refer,name_reference_.c_str(), "L" );
    leg->Draw("same");
    
    c1->Draw();
    ps.NewPage();
  }  
  
  ps.Close();
  c1->Close();
  delete c1;
}


int ValidationPlotMaker_Eff(int argc, char* argv[])
{
  setNiceStyle();
  gStyle->SetOptStat( 0 );
  gStyle->SetOptFit ( 0 );
  gStyle->SetStatColor(0);
  gStyle->SetStatBorderSize(0);
  gStyle->SetStatX(0.93);
  gStyle->SetStatY(0.93);
  gStyle->SetStatW(0.18);
  gStyle->SetStatH(0.18);
  
  //If no argument is given, use default config file name
  string config_file = "bin/ValidationPlot_Eff.cfg";
  if( argc>=2 )
    config_file = argv[1];
  
  //Run the Validation Plot Maker
  ValPlotMaker * plots = new ValPlotMaker();
  plots->readConfig( config_file.c_str() );
  plots->loadHistograms();
  plots->drawComparison();
  delete plots;
  
  return 0;
}

int main(int argc, char* argv[])
{
  return ValidationPlotMaker_Eff(argc, argv);
}
