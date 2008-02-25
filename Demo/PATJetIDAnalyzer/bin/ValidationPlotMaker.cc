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
  void loadHistograms(std::string const filename, std::vector<TH1F*> *hists);
  void drawComparison();

protected:   
  //Style Stuff
  void setHistStyle(TH1*, const char*, const char*);
  void setCanvasStyle( TCanvas& canv );
  void setLegendStyle( TLegend& leg );

 private:
  //define input/output
  std::string treename_, input_, reference_, output_;

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
  std::vector<std::string> HistNames_;

  //validation hist
  std::vector<TH1F*> hists_, refs_;
};

void ValPlotMaker::setHistStyle( TH1* hist, const char* titleX, const char* titleY )
{
  hist->SetTitle( "" );
  hist->GetXaxis()->SetTitle( titleX );
  hist->GetXaxis()->SetTitleSize  ( 0.06 );
  hist->GetXaxis()->SetTitleColor (    1 );
  hist->GetXaxis()->SetTitleOffset(  1.0 );
  hist->GetXaxis()->SetTitleFont  (   62 );
  hist->GetXaxis()->SetLabelSize  ( 0.05 );
  hist->GetXaxis()->SetLabelFont  (   62 );
  hist->GetXaxis()->CenterTitle   (      );
  hist->GetXaxis()->SetNdivisions (  505 );
  hist->GetYaxis()->SetTitle( titleY );
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
  canv.SetTopMargin   ( 0.05 );
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
  input_     = cfg.read<std::string>( "input_root" );
  reference_ = cfg.read<std::string>( "input_reference" );
  treename_  = cfg.read<std::string>( "tree_name" );
  output_    = cfg.read<std::string>( "output"   );
  Bins_      = bag_of<int>(  cfg.read<std::string>( "bins","0 1 2"));
  HistNames_ = bag_of_string(cfg.read<std::string>( "hists"));

  gridX_    = cfg.read<int>    ( "grid_x"     );
  gridY_    = cfg.read<int>    ( "grid_y"     );
  legXLeft_ = cfg.read<double> ( "leg_x_left"  );
  legXRight_= cfg.read<double> ( "leg_x_right" );
  legYLower_= cfg.read<double> ( "leg_y_lower" );
  legYUpper_= cfg.read<double> ( "leg_y_upper" );
  cmpColor_ = bag_of<short int>( cfg.read<std::string>( "color" ));
  cmpStyle_ = bag_of<short int>( cfg.read<std::string>( "style" ));
  cmpWidth_ = bag_of<short int>( cfg.read<std::string>( "width" ));
}


void ValPlotMaker::loadHistograms(string const filename, vector<TH1F*> *hists)
{
  TFile * file_ = new TFile( filename.c_str() );
  TH1F *dummy;
  hists->clear();
  for (std::vector<std::string>::iterator it=HistNames_.begin(); 
       it!=HistNames_.end(); ) {
    
      string dummy_name = treename_+"/"+(*it);
      dummy = (TH1F*)file_->Get( dummy_name.c_str() );
      if( !dummy ){
	cerr << "WARNING:" 
	     << " Didn't find indicated hist" << " [" << (*it) << "]" 
	     << " in tree" << " [" << treename_ << "]" 
	     << " of file" << " [" << filename << "]" 
	     << endl;
        it = HistNames_.erase( it ); 
      } else {
        hists->push_back( dummy );
	++it;
      }
  }
  cout << "...loaded " << hists->size() 
       <<" histograms from file '" << filename << "'." << endl;
}

void ValPlotMaker::loadHistograms()
{
  loadHistograms(input_, &hists_);
  loadHistograms(reference_, &refs_);
  if (hists_.size()!=refs_.size()) {
    cerr << "ERROR: At least one histogram is only present in one input file!"
         << "The number of data and reference plots does not match." << endl;
    exit(3);	 
  }
}

void ValPlotMaker::drawComparison()
{
  TPostScript ps(output_.c_str(), 112 );
  TCanvas *canv = new TCanvas("canv", "fit histograms", 600, 600);
  setCanvasStyle( *canv );
  canv->SetGridx( 1 );
  canv->SetGridy( 1 );

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
      setHistStyle( &refer, it->c_str(), "events"  );
      refer.SetTitle( "" );
      refer.SetLineWidth( 8 );
      refer.SetLineColor( 2 );
      refer.Sumw2();
      refer.Scale( hist.Integral()/refer.Integral() );
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
      if (ks<0.05){
        canv->SetFillColor(5);
	cout << "Deviation in plot '" << (*it) << "': " << ks 
	     << " (Kolmogorov-Smirnov Test)" << endl;
      }	
      else {
        canv->SetFillColor(0);
      }	
	
      double maximum = (refer.GetMaximum()>hist.GetMaximum() ?
                        1.5*refer.GetMaximum() : 1.5*hist.GetMaximum());
      refer.SetMaximum(maximum);
      hist.SetMarkerStyle(8);
      hist.SetLineWidth(3);
      refer.Draw("h");
      hist.Draw("pe,same");
      TLegend* leg = new TLegend(0.5,0.67,0.95,0.87);
      setLegendStyle( *leg );
      sprintf(dummy_text,"KS = %2.2f",ks);
      leg->SetHeader(dummy_text);
      leg->AddEntry( &hist, "data", "PE" );
      leg->AddEntry( &refer,"reference", "L" );
      leg->Draw("same");
      
      canv->Draw();
      ps.NewPage();
  }     

  ps.Close();
  canv->Close();
  delete canv;
}

int main(int argc, char* argv[])
{
  setNiceStyle();
  gStyle->SetOptStat( 0 );
  gStyle->SetOptFit ( 0 );

  //gStyle->SetStatColor(0);
  gStyle->SetStatBorderSize(0);
  gStyle->SetStatX(0.93);
  gStyle->SetStatY(0.93);
  gStyle->SetStatW(0.18);
  gStyle->SetStatH(0.18);

  if( argc<2 ){
    cerr << "ERROR:" 
	 << " Missing argument (the cfg file)!" << endl;
    return 1;
  }

  ValPlotMaker * plots = new ValPlotMaker();
  plots->readConfig( argv[1] );
  plots->loadHistograms();
  
  plots->drawComparison();
  delete plots;

  cout << "done." << endl; 
  return 0;
}
