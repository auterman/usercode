#include "handleHists.h"
#include "ConfigFile.h"
#include "TPostScript.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TFile.h"
#include "TH1.h"
#include "TGraph.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <cmath>

using namespace std;


  
TH1 * handleHists::GetHist(const string file, const string hist, const string treename, const string uncert, const string dir)
{
   TH1F * result;
   
   stringstream ss;
   ss << hist<< "_"<<_plotindex++;
   string name = ss.str(); //root can't handle two hists with same names

   //open root file
   if (file=="") return 0;
   TFile f( file.c_str() );
   if (f.IsZombie()) {
       //cout << "Error opening file '" << file << "'"<< endl;
       return 0;
   }
   
   //construct tree-name for uncertainty histograms:
   string unc = (uncert=="" ? "" : "_"+uncert+"_"+dir);
   
   cout << "opening file '" << file << "' histogram '"<< treename+unc << "/" << hist << "'."<<endl;
   
   //open requested histogram 'hist', in directory 'treename'
   string dummy_name = hist;
   if (treename!="") dummy_name = treename+unc+"/"+hist;
   if (uncert.find("Stat")!=string::npos) {
     //There is no separate histogram for statistical uncertainties; calculate these from the errors of the normal histogram:     
     dummy_name = treename+"/"+hist;
   }
   result = (TH1F*)f.Get( dummy_name.c_str() );
   if (!result) {
     cerr << "ERROR:Unable to read histogram '" <<dummy_name<< "' from file '"<<file<< "'!"<<endl;
     exit(1);
   }
   result->SetDirectory(0);
   if (uncert=="") result->SetTitle( hist.c_str() );
   else            result->SetTitle( uncert.c_str() );
   if (uncert=="Stat") {
     bool up = (dir=="UP"||dir=="up"||dir=="PLUS"||dir=="plus");
     //There is no separate histogram for statistical uncertainties; calculate these from the errors of the normal histogram:     
     for (int i=0; i<=result->GetNbinsX(); ++i)
       if (up) result->SetBinContent(i,result->GetBinContent(i)+result->GetBinError(i));
       else    result->SetBinContent(i,result->GetBinContent(i)-result->GetBinError(i));
   }
   
   return result;
}

handleHists::handleHists( ConfigFile* config)
{
  _plotindex=0;
  _SumOfDiffs=0.;
  string qcdfile    = config->read<string>("qcd-rootfile","");
  string qcdtree    = config->read<string>("qcd-treename","");
         nqcd       = config->read<string>("qcd-leg-name","QCD");
  string ttbarfile  = config->read<string>("ttbar-rootfile","");
  string ttbartree  = config->read<string>("ttbar-treename","");
         nttbar     = config->read<string>("ttbar-leg-name","t#bar{t}");
  string znunufile  = config->read<string>("znunu-rootfile","");
  string znunutree  = config->read<string>("znunu-treename","");
         nznunu     = config->read<string>("znunu-leg-name","Z#rightarrow#nu#bar{#nu}");
  string signalfile = config->read<string>("signal-rootfile","");
  string signaltree = config->read<string>("signal-treename","");
         nsignal    = config->read<string>("signal-leg-name","Signal");
  string datafile   = config->read<string>("data-rootfile","");
  string datatree   = config->read<string>("data-treename","");
         ndata      = config->read<string>("data-leg-name","Data");
  
  vars = bag_of_string(config->read<string>("final variables","HT"));
  corr_unc = bag_of_string(config->read<string>("correlated uncertainties","JEC"));
  uncorr_unc = bag_of_string(config->read<string>("uncorrelated uncertainties","method"));

  _plotSystematics = config->read<bool>("plot systematics",false);
  _showSignalBand = config->read<bool>("show signal uncertainty",false);
  _showDiff = config->read<bool>("show difference",false);

  //read in all histograms and store them in the private vectors of this class
  for (vector<string>::const_iterator var=vars.begin(); var!=vars.end(); ++var) {

    //read 'normal' histograms (without syst. uncertainties)
    _data_hists.push_back( GetHist(datafile, (*var), datatree ) );
    TH1 * sig = GetHist(signalfile, (*var), signaltree ); 
    TH1 * qcd = GetHist(qcdfile, (*var), qcdtree );
    TH1 * top = GetHist(ttbarfile, (*var), ttbartree);
    TH1 * z   = GetHist(znunufile, (*var), znunutree);
    _signal_hists.push_back( sig );
    _qcd_hists.   push_back( qcd );
    _ttbar_hists. push_back( top );
    _znunu_hists. push_back( z   );
    
    //read background histogram correlated uncertainties:
    for (vector<string>::const_iterator cor=corr_unc.begin(); cor!=corr_unc.end(); ++cor) {
      _syst_up_corr[qcd].push_back( GetHist(qcdfile,  (*var),qcdtree, (*cor),"UP" ) );
      _syst_dn_corr[qcd].push_back( GetHist(qcdfile,  (*var),qcdtree, (*cor),"DN" ) );
      _syst_up_corr[top].push_back( GetHist(ttbarfile,(*var),ttbartree, (*cor),"UP" ) );
      _syst_dn_corr[top].push_back( GetHist(ttbarfile,(*var),ttbartree, (*cor),"DN" ) );
      _syst_up_corr[ z ].push_back( GetHist(znunufile,(*var),znunutree, (*cor),"UP" ) );
      _syst_dn_corr[ z ].push_back( GetHist(znunufile,(*var),znunutree, (*cor),"DN" ) );
      _syst_up_corr[sig].push_back( GetHist(signalfile,(*var),signaltree, (*cor),"UP" ) );
      _syst_dn_corr[sig].push_back( GetHist(signalfile,(*var),signaltree, (*cor),"DN" ) );
    }
    //read background histogram un-correlated uncertainties:
    for (vector<string>::const_iterator uco=uncorr_unc.begin(); uco!=uncorr_unc.end(); ++uco) {
      _syst_up_uncorr[qcd].push_back( GetHist(qcdfile,  (*var),qcdtree, (*uco),"UP" ) );
      _syst_dn_uncorr[qcd].push_back( GetHist(qcdfile,  (*var),qcdtree, (*uco),"DN" ) );
      _syst_up_uncorr[top].push_back( GetHist(ttbarfile,(*var),ttbartree, (*uco),"UP" ) );
      _syst_dn_uncorr[top].push_back( GetHist(ttbarfile,(*var),ttbartree, (*uco),"DN" ) );
      _syst_up_uncorr[ z ].push_back( GetHist(znunufile,(*var),znunutree, (*uco),"UP" ) );
      _syst_dn_uncorr[ z ].push_back( GetHist(znunufile,(*var),znunutree, (*uco),"DN" ) );
      _syst_up_uncorr[sig].push_back( GetHist(signalfile,(*var),signaltree, (*uco),"UP" ) );
      _syst_dn_uncorr[sig].push_back( GetHist(signalfile,(*var),signaltree, (*uco),"DN" ) );
    }
  }//loop over all 'final variables'

  if ((_data_hists.size()==_signal_hists.size())*(_data_hists.size()==_qcd_hists.size())*
      (_data_hists.size()==_ttbar_hists.size())*(_znunu_hists.size()==_qcd_hists.size())!=1){
     cerr<<"ERROR: Read in different number of data, signal, and background histograms!"<<endl;
     exit(1);
  }
}
    
TH1 * handleHists::GetSyst(const std::string dir, const TH1*h1, const TH1*h2, const TH1*h3)
{
  if (!h1&&!h2&&!h3) return 0;
  if (!h1) return GetSyst(dir, h2, h3);
  
  int bins = h1->GetNbinsX();
  stringstream ss; ss << _plotindex++; 
  string unique; unique = ss.str();
  string name = "syssum_"+unique+(string)h1->GetTitle()+dir;
  TH1 * sum     = new TH1F(name.c_str(),h1->GetTitle(),bins,h1->GetXaxis()->GetXmin(),h1->GetXaxis()->GetXmax());
  name = "relquad_"+unique+(string)h1->GetTitle()+dir;
  TH1 * relquadsum = new TH1F(name.c_str(),"",bins,h1->GetXaxis()->GetXmin(),h1->GetXaxis()->GetXmax());
  
  //Store all samples for which the syst. uncertainties shall be combined in one 
  //vector 'samples'. Consider only histograms with at least one entry.
  vector<const TH1*> samples;
  if (h1) samples.push_back(h1);
  if (h2) samples.push_back(h2);
  if (h3) samples.push_back(h3);

  //Calculate the total systematic uncertainty for 'samples':
  for (vector<const TH1*>::const_iterator s=samples.begin(); s!=samples.end(); ++s) {

    //simply add all correlated uncertainties for 's' (assume 100% correlation),
    //the uncertainties are *relative* to the unchanged hists 's'
    vector<TH1*>::const_iterator corr_begin =
       (dir=="UP" ? _syst_up_corr[(*s)].begin() : _syst_dn_corr[(*s)].begin());
    vector<TH1*>::const_iterator corr_end  =
       (dir=="UP" ? _syst_up_corr[(*s)].end() : _syst_dn_corr[(*s)].end());

    for (vector<TH1*>::const_iterator c=corr_begin; c!=corr_end; ++c) {
      sum->Add( *c );
      sum->Add( *s, -1.0 ); //sum up (c-s)
    }
    
    //now add quadratically all uncorrelated uncertainties for 's';
    //this is done bin-by-bin, and for *relative* uncertainties
    vector<TH1*>::const_iterator uncorr_begin =
       (dir=="UP" ? _syst_up_uncorr[*s].begin() : _syst_dn_uncorr[*s].begin()); 
    vector<TH1*>::const_iterator uncorr_end   =
       (dir=="UP" ? _syst_up_uncorr[*s].end() : _syst_dn_uncorr[*s].end());
    for (vector<TH1*>::const_iterator u=uncorr_begin; u!=uncorr_end; ++u) {
      for (int bin=0; bin<=bins; ++bin) {
        double cont = (*u)->GetBinContent(bin) - (*s)->GetBinContent(bin);
        relquadsum->SetBinContent(bin, relquadsum->GetBinContent(bin)+cont*cont);
      }	
    }
  }
  
  //add total correlated and total uncorrelated uncertainties quadratically:
  for (int bin=0; bin<=bins; ++bin) {
    double cont = sum->GetBinContent(bin);
    relquadsum->SetBinContent(bin, relquadsum->GetBinContent(bin)+cont*cont);
    //sqrt(relquadsum):
    cont=0;
    for (vector<const TH1*>::const_iterator s=samples.begin(); s!=samples.end(); ++s) 
      cont+=(*s)->GetBinContent(bin);
    cont += (dir=="UP" ? sqrt(relquadsum->GetBinContent(bin)) : -sqrt(relquadsum->GetBinContent(bin)));  
    sum->SetBinContent(bin, cont );
  }	
  sum->SetLineStyle( 9 ); //dashed
  
  delete relquadsum;
  return sum; //abolute syst. uncertainty, i.e. *not* relative
}

TGraph * handleHists::MakeBand(const TH1*h1, const TH1*h2)
{
  if (!h1 || !h2) return 0;
  TGraph * result = new TGraph(h1->GetNbinsX()+h1->GetNbinsX()+2);
  unsigned p = 0;
  for (int i=0; i<=h1->GetNbinsX(); ++i )
    result->SetPoint(p++, h1->GetBinCenter(i), h1->GetBinContent(i) );
  for (int i=h2->GetNbinsX(); i>=0; --i )
    result->SetPoint(p++, h2->GetBinCenter(i), h2->GetBinContent(i) );

  return result;
}

void handleHists::PlotStackedBackground(const string out)
{
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameFillColor(0);
  gStyle->SetTitleFillColor(0); 
  gStyle->SetOptStat(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetHistFillColor(0);
  gStyle->SetStatBorderSize(0);
  TPostScript ps(out.c_str(), 111);
  TCanvas * c1;
  if (_showDiff) { 
    c1 = new TCanvas("c1","",1200,600); 
    c1->Divide(2,1);
  }
  else 
    c1 = new TCanvas("c1","",600,600); 

  std::vector<TH1*>::const_iterator d=_data_hists.begin(),
                                    s=_signal_hists.begin(),
                                    q=_qcd_hists.begin(),
                                    t=_ttbar_hists.begin(),
                                    z=_znunu_hists.begin();

  for (;d!=_data_hists.end();++d,++s,++q,++t,++z) {//loop over all variables (HT, MHT, MET, ...)
    c1->cd(1);
    TH1 * axis = 0;
    if (*q) {(*q)->SetFillColor( kYellow ); axis = *q; }
    if (*t) {(*t)->SetFillColor( kBlue );   axis = *t; }
    if (*z) {(*z)->SetFillColor( kGreen );  axis = *z; }
    if (*s) {(*s)->SetLineColor( kRed );    axis = *s; }
    if (*s)  (*s)->SetLineWidth( 4 );
    if (*d) {(*d)->SetMarkerStyle( 8 );     axis = *d; }
    if (!axis) return; //nothing to plot.
    TH1 * ax = (TH1*)axis->Clone();

    stringstream ss;
    ss << "hs_"<<_plotindex++;
    string name = ss.str();
    THStack st(name.c_str(),"");
    if (*z) st.Add( *z );
    if (*t) st.Add( *t );
    if (*q) st.Add( *q );
    
    //background uncertainties
    TH1 * syst_up = GetSyst( "UP", *q, *t, *z);
    TH1 * syst_dn = GetSyst( "DN", *q, *t, *z);
 
    //signal uncertainties
    TH1 * sig_syst_up = GetSyst( "UP", *s);
    TH1 * sig_syst_dn = GetSyst( "DN", *s);
    TGraph * signalUncert = MakeBand( sig_syst_up, sig_syst_dn );
    if (signalUncert) signalUncert->SetFillColor( 6 );
    //if (signalUncert) signalUncert->SetFillStyle( 3004 );

    string var  = ax->GetTitle();
    string titel = ";"+var+" [GeV]; events";
    int    bins = ax->GetNbinsX();
    double min  = ax->GetXaxis()->GetXmin();
    double max  = ax->GetXaxis()->GetXmax();
    cout << "...plotting "<< var << " with bins="<<bins<<", min="<<min<<", max="<<max<<endl;  

    double maximum = 0;
    if (*d) if ((*d)->GetMaximum()+sqrt((*d)->GetMaximum()) > maximum) maximum = (*d)->GetMaximum()+sqrt((*d)->GetMaximum());
    if (*s) if ((*s)->GetMaximum()>maximum) maximum = (*s)->GetMaximum();
    if (*z||*t||*q) if (st.GetMaximum()>maximum) maximum = st.GetMaximum();
    if (syst_up) if (syst_up->GetMaximum()>maximum) maximum = syst_up->GetMaximum();
    ax->SetMinimum(0);
    ax->SetMaximum(maximum);
    ax->SetTitle(titel.c_str());
    ax->GetYaxis()->SetTitleOffset(1.4);
  
    TLegend leg(0.5,0.7,0.9,0.9);
    leg.SetFillColor(0);leg.SetBorderSize(1);
    if ( (*d) ) leg.AddEntry( (*d), ndata.c_str(),"pe");
    if ( (*s) ) leg.AddEntry( (*s), nsignal.c_str(),"l");
    if ( signalUncert && _showSignalBand ) leg.AddEntry( signalUncert, "signal uncert.","f");
    if ( (*q) ) leg.AddEntry( (*q), nqcd.c_str(),"f");
    if ( (*t) ) leg.AddEntry( (*t), nttbar.c_str(),"f");
    if ( (*z) ) leg.AddEntry( (*z), nznunu.c_str(),"f");
    if ( syst_up) leg.AddEntry( syst_up, "total sys. uncert.","l");
  
    ax->Draw("h");
    if (syst_up) syst_up->Draw("h,same");      //syst.uncertainty band upper border
    st.Draw("h,same");         //stacked backgrounds
    
    if (syst_dn) syst_dn->Draw("h,same"); //syst.uncertainty band lower border
    if (_showSignalBand && signalUncert) signalUncert->Draw("f");
    if (*s) (*s)->Draw("h,same");    //signal
    if (*d) (*d)->Draw("pe,same");   //data with stat. errors
    leg.Draw();              //legend    
    //c1->Update();
    
    if (_showDiff) {
      c1->cd(2);
      TH1 * diff = (TH1*)axis->Clone();
      for (int i=0; i<=(*d)->GetNbinsX(); ++i ) {
        diff->SetBinContent(i, (*d)->GetBinContent(i) - (*s)->GetBinContent(i));
        //diff->SetBinError(i, sqrt((*d)->GetBinError(i)*(*d)->GetBinError(i) + (*s)->GetBinError(i)*(*s)->GetBinError(i)) );
	_SumOfDiffs += (*d)->GetBinContent(i) - (*s)->GetBinContent(i);
      }
      diff->SetTitle("difference: PAT - RECO");
      diff->SetLineColor( kRed );
      //diff->SetLineWidth( 4 );
      //diff->SetMarkerStyle( 8 );
      diff->Draw("h");	
    }
    c1->Update();
    ps.NewPage();

  }//loop over all histograms (HT, MHT, MET, ...)
  cout << "Wrote result plots to file '"<<out<<"'"<<endl;
     
}

void handleHists::PlotAllSystematics(const string out)
{
  TPostScript ps(out.c_str(), 111);
  TCanvas c1("c1","",600,600);
  std::vector<TH1*>::const_iterator q=_qcd_hists.begin(),
                                    t=_ttbar_hists.begin(),
                                    z=_znunu_hists.begin();
  //construct for each background plot a full list of uncertainties (merge un- and correlated ones)
  std::map<const TH1*, std::vector<TH1*> > syst_up(_syst_up_corr);
  std::map<const TH1*, std::vector<TH1*> > syst_dn(_syst_dn_corr);
  for (std::map<const TH1*, std::vector<TH1*> >::const_iterator up=_syst_up_uncorr.begin();up!=_syst_up_uncorr.end();++up)
    for (std::vector<TH1*>::const_iterator val=up->second.begin();val!=up->second.end();++val) 
      syst_up[up->first].push_back( *val );
  for (std::map<const TH1*, std::vector<TH1*> >::const_iterator dn=_syst_dn_uncorr.begin();dn!=_syst_dn_uncorr.end();++dn)
    for (std::vector<TH1*>::const_iterator val=dn->second.begin();val!=dn->second.end();++val) 
      syst_dn[dn->first].push_back( *val );

  //loop over all variables (HT, MHT, MET, ...)
  for (;q!=_qcd_hists.end();++q,++t,++z) {
    
    //loop over all syst. uncertainties  
    for (unsigned uncert=0; uncert<corr_unc.size()+uncorr_unc.size(); ++uncert){ 
      TH1 * n; //A not-empty background histogram
      if (*q) {(*q)->SetFillColor( kYellow ); n = (*q);}
      if (*t) {(*t)->SetFillColor( kBlue );   n = (*t);}
      if (*z) {(*z)->SetFillColor( kGreen );  n = (*z);}

      stringstream ss;
      ss << "hs_"<<_plotindex++;
      string name = ss.str();
      THStack st(name.c_str(),"");
      if (*z) st.Add( *z );
      if (*t) st.Add( *t );
      if (*q) st.Add( *q );

      //Match to-be plotted backgrounds with their uncertainty
      ss << "hsysUP_"<<_plotindex++;
      TH1 * h_syst_up = new TH1F(ss.str().c_str(),"",n->GetNbinsX(),n->GetXaxis()->GetXmin(),n->GetXaxis()->GetXmax());
      ss << "hsysDN_"<<_plotindex++;
      TH1 * h_syst_dn = new TH1F(ss.str().c_str(),"",n->GetNbinsX(),n->GetXaxis()->GetXmin(),n->GetXaxis()->GetXmax());
      if (*z) h_syst_up->Add( syst_up[*z][uncert] ); 
      if (*t) h_syst_up->Add( syst_up[*t][uncert] ); 
      if (*q) h_syst_up->Add( syst_up[*q][uncert] ); 
      if (*z) h_syst_dn->Add( syst_dn[*z][uncert] );
      if (*t) h_syst_dn->Add( syst_dn[*t][uncert] );
      if (*q) h_syst_dn->Add( syst_dn[*q][uncert] );

      //Get names etc from "n" a non-empty background histogram
      string uncertName = syst_up[n][uncert]->GetTitle();
      string var  = n->GetTitle();
      string titel = ";"+var+" [GeV]; events";
      int    bins = n->GetNbinsX();
      double min  = n->GetXaxis()->GetXmin();
      double max  = n->GetXaxis()->GetXmax();
      cout << "...plotting "<< var << " with bins="<<bins<<", min="<<min<<", max="<<max<<endl;  

      h_syst_up->SetMinimum(0);
      h_syst_up->SetTitle(titel.c_str());
      h_syst_up->GetYaxis()->SetTitleOffset(1.4);

      TLegend leg(0.5,0.7,0.9,0.9);
      leg.SetFillColor(0);leg.SetBorderSize(1);
      if ( (*q) ) leg.AddEntry( (*q), nqcd.c_str(),"f");
      if ( (*t) ) leg.AddEntry( (*t), nttbar.c_str(),"f");
      if ( (*z) ) leg.AddEntry( (*z), nznunu.c_str(),"f");
      if ( h_syst_up->Integral()>0. ) leg.AddEntry( h_syst_up, uncertName.c_str(),"l");

      h_syst_up->Draw("h");      //syst.uncertainty band upper border
      st.Draw("h,same");         //stacked backgrounds
      h_syst_dn->Draw("h,same"); //syst.uncertainty band lower border
      leg.Draw();              //legend    
      c1.Update();
      ps.NewPage();

    }//loop over syst. uncertainties
  }//loop over all histograms (HT, MHT, MET, ...)
  cout << "Wrote result plots to file '"<<out<<"'"<<endl;
     
}
  
void handleHists::PlotHistograms(const string out)
{
  PlotStackedBackground( out );
  if (_plotSystematics) PlotAllSystematics( "systematics.ps" );
}

void handleHists::Print()
{
  std::vector<TH1*>::const_iterator d=_data_hists.begin(),
                                    s=_signal_hists.begin(),
                                    q=_qcd_hists.begin(),
                                    t=_ttbar_hists.begin(),
                                    z=_znunu_hists.begin();

  for (;d!=_data_hists.end();++d,++s,++q,++t,++z) {
    stringstream ss;
    ss << "hps_"<<_plotindex++;
    string name = ss.str();
    THStack st(name.c_str(),"");
    st.Add( *z );
    st.Add( *t );
    st.Add( *q );
    
    TH1 * syst_up = GetSyst( "UP", *q, *t, *z);
    TH1 * syst_dn = GetSyst( "DN", *q, *t, *z);
    double b = 0.0;
    if (*q) b+=(*q)->Integral();
    if (*t) b+=(*t)->Integral();
    if (*z) b+=(*z)->Integral();
    double data=0.0;
    if (*d) data=(*d)->Integral();
    double signal=0.0;
    if (*s) signal=(*s)->Integral();
 
    if (syst_up) cout << setw(10) << syst_up->GetTitle();
    else if (*s) cout << setw(10) << (*s)->GetTitle();
    else if (*d) cout << setw(10) << (*d)->GetTitle();
    cout << "  d: "     << data
	 << ", s: "     << signal
	 << ", b: "     << b;
    if (syst_up && syst_dn) 
      cout << " "         << showpos << syst_up->Integral()-b
	 << " "         << showpos << syst_dn->Integral()-b << noshowpos
         << "(syst.) ";
    cout << "+-"<< sqrt(data) << "(stat)"
	 << endl;
  }
  cout << "Sum of all differences: " << _SumOfDiffs << endl;
 
}
  
handleHists::~handleHists()
{
  //delete all histograms
  /*
  for (map<const TH1*, vector<TH1*> >::iterator it=_syst_up_corr.begin();it!=_syst_up_corr.end(); ++it)
    for (vector<TH1*>::iterator i=it->second.begin(); i!=it->second.end();++i)      delete *i;
  for (map<const TH1*, vector<TH1*> >::iterator it=_syst_dn_corr.begin();it!=_syst_up_corr.end(); ++it)
    for (vector<TH1*>::iterator i=it->second.begin(); i!=it->second.end();++i)     delete *i;
  for (map<const TH1*, vector<TH1*> >::iterator it=_syst_up_uncorr.begin();it!=_syst_up_corr.end(); ++it)
    for (vector<TH1*>::iterator i=it->second.begin(); i!=it->second.end();++i)      delete *i;
  for (map<const TH1*, vector<TH1*> >::iterator it=_syst_dn_uncorr.begin();it!=_syst_up_corr.end(); ++it)
    for (vector<TH1*>::iterator i=it->second.begin(); i!=it->second.end();++i)      delete *i;
  for (vector<TH1*>::iterator it=_qcd_hists.begin(); it!=_qcd_hists.end();++it)    delete *it;
  for (vector<TH1*>::iterator it=_ttbar_hists.begin(); it!=_ttbar_hists.end();++it)    delete *it;
  for (vector<TH1*>::iterator it=_znunu_hists.begin(); it!=_znunu_hists.end();++it)    delete *it;
  for (vector<TH1*>::iterator it=_signal_hists.begin(); it!=_signal_hists.end();++it)    delete *it;
  for (vector<TH1*>::iterator it=_data_hists.begin(); it!=_data_hists.end();++it)    delete *it;
  */
  _syst_up_corr.clear(); 
  _syst_dn_corr.clear(); 
  _syst_up_uncorr.clear();
  _syst_dn_uncorr.clear();
  _qcd_hists.clear(); 
  _ttbar_hists.clear(); 
  _znunu_hists.clear(); 
  _signal_hists.clear();
  _data_hists.clear(); 
}
