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
  template <class T> void loadHistograms(std::vector<std::string> filename, vector<string> *histnames, std::vector<T*> *hists);
  void drawComparison();
  
protected:   
  bool contains(const string the_string, const string the_sub_string) const;
  //Style Stuff
  void setHistStyle(TH1*, const char*, const char*);
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
  bool scale_;
  
  //define legend design
  double legXLeft_,  legXRight_;
  double legYLower_, legYUpper_;
  
  //bins
  std::vector<int> Bins_;
  
  //hists
  std::vector<std::string> HistNames_, Hist2DNames_;
  
  //validation hist
  std::vector<TH1F*> hists_, refs_;
  std::vector<TH2F*> hists2D_, refs2D_;
  
  //background color of hists, if KS test fails
  double ks_treshold;
  int    ks_color;
  bool   UseTitle_;

};

bool ValPlotMaker::contains(const string the_string, const string the_sub_string) const
{
  return the_string.find(the_sub_string,0) != string::npos;
}

void ValPlotMaker::setHistStyle( TH1* hist, const char* titleX, const char* titleY )
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
  Hist2DNames_ = bag_of_string(cfg.read<std::string>( "2D hists"));
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
  scale_       = cfg.read<bool>   ( "scale", false	 );
}


template <class T> void ValPlotMaker::loadHistograms(vector<string> filenames, vector<string> *histnames, vector<T*> *hists)
{
  std::vector<std::string>::iterator filename = filenames.begin();
  TFile * file_ = new TFile( (*filename).c_str() );
  T * dummy;
  hists->clear();
  for (std::vector<std::string>::iterator it=histnames->begin(); 
       it!=histnames->end(); ) {
    string dummy_name = treename_+"/"+(*it);
    dummy = (T*)file_->Get( dummy_name.c_str() );
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
    typename std::vector<T*>::iterator hist=hists->begin();
    for (std::vector<std::string>::iterator it=histnames->begin(); 
	 it!=histnames->end()&&hist!=hists->end(); ++hist,++it,++i) {
      string dummy_name = treename_+"/"+(*it);
      dummy = (T*)file_->Get( dummy_name.c_str() );
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
  loadHistograms<TH1F>(input_,     &HistNames_, &hists_);
  loadHistograms<TH1F>(reference_, &HistNames_, &refs_);
  if (hists_.size()!=refs_.size()) {
    cerr << "ERROR: At least one histogram is only present in one input file!"
         << "The number of data and reference plots does not match." << endl;
    exit(2);	 
  }
  
  loadHistograms<TH2F>(input_,     &Hist2DNames_, &hists2D_);
  loadHistograms<TH2F>(reference_, &Hist2DNames_, &refs2D_);
  if (hists2D_.size()!=refs2D_.size()) {
    cerr << "ERROR: At least one 2D-histogram is only present in one input file!"
         << "The number of data and reference plots does not match." << endl;
    exit(3);	 
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
    if ( scale_ && !contains(refer.GetYaxis()->GetTitle(), "eff") ) {
      refer.Scale( hist.Integral()/refer.Integral() );
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
    
    //Show under- and over-flow bins
    refer.SetBinContent(1,refer.GetBinContent(0)+refer.GetBinContent(1));
    hist.SetBinContent(1,hist.GetBinContent(0)+hist.GetBinContent(1));
    unsigned int mb = refer.GetNbinsX();
    refer.SetBinContent(mb-1,refer.GetBinContent(mb)+refer.GetBinContent(mb-1));
    mb = hist.GetNbinsX();
    hist.SetBinContent(mb-1,hist.GetBinContent(mb)+hist.GetBinContent(mb-1));
    
    double maximum = (refer.GetMaximum()>hist.GetMaximum() ?
		      1.5*refer.GetMaximum() : 1.5*hist.GetMaximum());
    double minimum = (refer.GetMinimum()<hist.GetMinimum() ?
		      refer.GetMinimum() : hist.GetMinimum());
    refer.SetMaximum(maximum);
    refer.SetMinimum(minimum);
    for (unsigned int b=0; b<mb; ++b)
      if (refer.GetBinContent(b)<=0.)
        refer.SetBinError(b, sqrt(-1.*refer.GetBinContent(b)));
    
    
    hist.SetMarkerStyle(8);
    hist.SetLineWidth(3);
    refer.Draw("h");
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
  
  
  
  //2D-stuff 
  
  i=0;
  it=Hist2DNames_.begin();
  std::vector<TH2F*>::iterator hst2D=hists2D_.begin();
  std::vector<TH2F*>::iterator ref2D=refs2D_.begin();
  
  for (;ref2D!=refs2D_.end()&&hst2D!=hists2D_.end()&&it!=Hist2DNames_.end(); 
       ++ref2D,++hst2D,++it,++i){
    cout << i << "th 2D-plot: " << (*it) << endl;
    
    TH1F& hist2D   = *((TH1F*)(*hst2D));
    TH1F& refer2D  = *((TH1F*)(*ref2D));
    TH2F * diff2D  = new TH2F( *((TH2F*)(*hst2D)) );
    setHistStyle( &refer2D, it->c_str(), "events"  );

    for (int binx=0; binx<=refer2D.GetNbinsX();++binx) {
    for (int biny=0; biny<=refer2D.GetNbinsY();++biny) {
    //  if (refer2D.GetBinContent(b)<=0.)
    //    refer2D.SetBinContent(b, fabs(refer2D.GetBinContent(b)));
    //  if (hist2D.GetBinContent(b)<=0.)
    //    hist2D.SetBinContent(b, fabs(hist2D.GetBinContent(b)));
        diff2D->SetBinContent(binx,biny,
	        refer2D.GetBinContent(binx,biny)-hist2D.GetBinContent(binx,biny) );
    }}
    if (UseTitle_){
      //refer2D.SetTitle( "" );
    }	
    else
      refer2D.SetTitle( "" );
    //refer2D.SetLineWidth( 8 );
    refer2D.SetLineColor( 2 );
    refer2D.Sumw2();
    if ( !contains(refer2D.GetYaxis()->GetTitle(), "eff") ) {
      refer2D.Scale( hist2D.Integral()/refer2D.Integral() );
    }
    ks = refer2D.KolmogorovTest( &hist2D, "");
    if (ks<ks_treshold){
      c1->SetFillColor(ks_color);
      cout << "Deviation in plot "<< i <<" '" << (*it) << "': " << ks 
	   << " (Kolmogorov-Smirnov Test)" << endl;
    }	
    else {
      c1->SetFillColor(0);
    }	

    double maximum = (refer2D.GetMaximum()>hist2D.GetMaximum() ?
		      1.5*refer2D.GetMaximum() : 1.5*hist2D.GetMaximum());
    double minimum = (refer2D.GetMinimum()<hist2D.GetMinimum() ?
		      refer2D.GetMinimum() : hist2D.GetMinimum());
    //refer2D.SetMaximum(maximum);
    //refer2D.SetMinimum(minimum);
    
    refer2D.Draw("BOX");
    hist2D.Draw("BOX,same");
    
    TLegend* leg = new TLegend(0.7,0.75,0.95,0.95);
    setLegendStyle( *leg );
    sprintf(dummy_text,"KS = %2.2f",ks);
    leg->SetHeader(dummy_text);
    leg->AddEntry( &hist2D, "data", "F" );
    leg->AddEntry( &refer2D,"reference", "F" );
    leg->Draw("same");
    c1->Draw();
    ps.NewPage();


    diff2D->Draw("BOX");
    
    TLegend* dleg = new TLegend(0.7,0.8,0.95,0.95);
    setLegendStyle( *dleg );
    sprintf(dummy_text,"Difference:",ks);
    dleg->SetHeader(dummy_text);
    dleg->AddEntry( diff2D,"reference - data", "F" );
    dleg->Draw("same");
    c1->Draw();
    ps.NewPage();
    delete diff2D;
  }
  
  ps.Close();
  c1->Close();
  delete c1;
}


int ValidationPlotMaker(int argc, char* argv[])
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
  string config_file = "bin/ValidationPlot.cfg";
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
  return ValidationPlotMaker(argc, argv);
}
