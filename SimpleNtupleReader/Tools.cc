#include "Tools.h"

#include "TPad.h"
#include "TLine.h"
#include "TLegend.h"
#include "THStack.h"
#include "TStyle.h"
#include "TArrow.h"
#include "TLatex.h"

#include <iostream>
#include <fstream>

void Histograms::Book()
{
    h_["idxphoton"]  = new TH1F(((std::string)"h_idxphoton"+label_).c_str(),";Index of first tight #gamma;Events",11,-0.5,10.5);
    h_["met"]       = new TH1F(((std::string)"h_met"+label_).c_str(),";MET [GeV];Events",n_metbins,metbins);
    h_["met_const"] = new TH1F(((std::string)"h_met_const"+label_).c_str(),";MET [GeV];Events",20,0,500);
    h_["mht"]       = new TH1F(((std::string)"h_mht"+label_).c_str(),";MHT [GeV];Events",20,0,1000);
    h_["ht" ]       = new TH1F(((std::string)"h_ht"+label_).c_str(),";HT [GeV];Events",20,400,1500);
    h_["em1_pt" ]   = new TH1F(((std::string)"h_em1_pt"+label_).c_str(),";p_{T}^{em}_1 [GeV];Events",20,0,500);
    h_["em1_thePt" ]   = new TH1F(((std::string)"h_em1_thePt"+label_).c_str(),";the p_{T}^{em,*}_1 [GeV];Events",20,0,500);
    h_["weight" ]   = new TH1F(((std::string)"weight"+label_).c_str(),";weight;Events",20,0,4);

    h_["em1_phi" ]   = new TH1F(((std::string)"h_em1_phi"+label_).c_str(),";#phi^{em}_1;Events",20,-3.2,3.2);
    h_["em1_eta" ]   = new TH1F(((std::string)"h_em1_eta"+label_).c_str(),";#eta^{em}_1;Events",20,-2.5,2.5);
    h_["em1_ptstar" ]= new TH1F(((std::string)"h_em1_ptstar"+label_).c_str(),";p_{T}^{*,em}_1 [GeV];Events",20,0,500);
    h_["em1_sigmaIphiIphi" ]= new TH1F(((std::string)"h_em1_sigmaIphiIphi"+label_).c_str(),";#sigma_{i#phi#i#phi}^{em}_1 [GeV];Events",50,0.005,0.03);
    h_["em1_sigmaIetaIeta" ]= new TH1F(((std::string)"h_em1_sigmaIetaIeta"+label_).c_str(),";#sigma_{i#eta#i#eta}^{em}_1 [GeV];Events",50,0.005,0.03);
    h_["em1_r9" ]= new TH1F(((std::string)"h_em1_r9"+label_).c_str(),";R9^{em}_1 [GeV];Events",50,0.0,1.0);
    h_["em1_hadTowOverEm" ]= new TH1F(((std::string)"h_em1_hadTowOverEm"+label_).c_str(),";hadTowOverEm^{em}_1 [GeV];Events",50,0.,1.6);
    h_["em1_chargedIso" ]= new TH1F(((std::string)"h_em1_chargedIso"+label_).c_str(),";chargedIso^{em}_1 [GeV];Events",50,0.,150);
    h_["em1_neutralIso" ]= new TH1F(((std::string)"h_em1_neutralIso"+label_).c_str(),";neutralIso^{em}_1 [GeV];Events",50,0.,40);
    h_["em1_photonIso" ]= new TH1F(((std::string)"h_em1_photonIso"+label_).c_str(),";photonIso^{em}_1 [GeV];Events",50,0.,100);
    h_["em1_conversionSafeVeto" ]= new TH1F(((std::string)"h_em1_conversionSafeVeto"+label_).c_str(),";conversionSafeVeto^{em}_1 [GeV];Events",2,-0.5,1.5);
    h_["em1_pixelseed" ]= new TH1F(((std::string)"h_em1_pixelseed"+label_).c_str(),";pixelseed^{em}_1 [GeV];Events",50,0.005,0.03);
    h_["em1_bitFlag" ]= new TH1F(((std::string)"h_em1_bitFlag"+label_).c_str(),";bitFlag^{em}_1 [GeV];Events",2,-0.5,1.5);
    h_["em1_matchedJetIndex" ]= new TH1F(((std::string)"h_em1_matchedJetIndex"+label_).c_str(),";matchedJetIndex^{em}_1 [GeV];Events",7,-1.5,5.5);
}

void Histograms::Add(Histograms* r)
{
    for (std::map<std::string, TH1*>::iterator it=r->GetAll()->begin(); it!=r->GetAll()->end(); ++it) {
        if (h_[it->first]) h_[it->first]->Add( it->second );
        else h_[it->first] = it->second;
    }
}


void Histograms::Fill(const std::string&s,double c, double w)
{
    if (h_.find(s)!=h_.end()) h_[s]->Fill(c,w);
}

void Histograms::Write()
{
    struct stat st= {0};
    if(stat(dir_.c_str(),&st)==-1)
        mkdir(dir_.c_str(), 0700);
    if(stat(((std::string)dir_+"/log/").c_str(),&st)==-1)
        mkdir(((std::string)dir_+"/log/").c_str(), 0700);
    if(stat(((std::string)dir_+"/linear/").c_str(),&st)==-1)
        mkdir(((std::string)dir_+"/linear/").c_str(), 0700);
    TCanvas * c1 = new TCanvas("","",600,600);
    for (std::map<std::string, TH1*>::iterator it=h_.begin(); it!=h_.end(); ++it)
    {
        gPad->SetLogy(1);
        it->second->Draw("he");
        c1->SaveAs(((std::string)dir_+"/log/"+label_+"_"+it->first+"_log.pdf").c_str());
        gPad->SetLogy(0);
        it->second->SetMinimum(0);
        it->second->Draw("he");
        c1->SaveAs(((std::string)dir_+"/linear/"+label_+"_"+it->first+".pdf").c_str());
    }
    delete c1;
}




void Yields::Add(Yields*r)
{
    std::map<int,Yield> * ryield = r->GetYields();
    for (std::map<int,Yield>::iterator it=ryield->begin(); it!=ryield->end(); ++it) {
        yield[it->first].AddWeight( it->second.GetWeights() );
        yield[it->first].AddWeightError( it->second.GetWeightErrors() );
        yield[it->first].AddSamples( it->second.GetSamples() );
    }
}



void SetBinContentError(TH1*r, int i, double w, double e)
{
  
  
}

TH1 * MyYields::GetPlot(const std::string& s)
{
    std::stringstream ss;
    //std::cout << label_ << "_" << s << "_" << plotnr<<std::endl;
    ss  << s << "_" << plotnr++;
    int nbins=GetNBins(s);
    double bins[nbins];
    //std::cout << "binning for >>"<<s<<"<< nbins="<<nbins<<std::endl;
    for (int i=0; i<nbins; ++i) {
        bins[i]=GetBinBorder(s,i);
        //std::cout << "bin "<<i<<", lower border: "<<bins[i]<<std::endl;
    }
    TH1 * r = new TH1F(ss.str().c_str(),(";"+GetYieldsRef(s)->GetXaxisTitle()+";"+GetYieldsRef(s)->GetYaxisTitle()).c_str(),nbins-1,bins);

    //std::map<int,Yield> * mp = GetYieldsRef(s)->GetYields();
    bool corr = GetYieldsRef(s)->GetCorrelation();
    for (int i=0; i<=nbins; ++i) {
        if (!corr) {
            //std plot
            r->SetBinContent(i, Weighted(s,i) );
            r->SetBinError(  i, Error(s,i) );
	    //SetBinContentError(r, i, Weighted(s,i), Error(s,i)  ),
        } else {
            //correlation plot
            float n=Weighted(s,i);
            float d=Unweighted(s,i);
            float ne=Error(s,i);
            r->SetBinContent( i, (d==0?1.0:n / d) );
            r->SetBinError(   i, (d==0?1.0:sqrt( ne*ne/(d*d) + n*n/(d*d*d) ) ) );
        }
        //std::cout << " my bin "<<i<<": > "<<bins[i]
        //          << " contents: "<< r->GetBinContent(i)<<" +- "<<r->GetBinError(i)
        //	      << " map[]: "<<(*mp)[i].weighted()<<" +- "<<(*mp)[i].error()
        //	      <<std::endl;
    }
    return r;
}


TH1 * MyYields::GetWeightErrorPlot(const std::string& s, TH1*h)
{
    std::stringstream ss;
    ss  <<"we_"<< s << "_" << plotnr++;
    int nbins=GetNBins(s);
    double bins[nbins];
    for (int i=0; i<nbins; ++i) {
        bins[i]=GetBinBorder(s,i);
    }
    TH1 * r = new TH1F(ss.str().c_str(),(";"+GetYieldsRef(s)->GetXaxisTitle()+";"+GetYieldsRef(s)->GetYaxisTitle()).c_str(),nbins-1,bins);
    for (int i=0; i<nbins; ++i) {
        double sys = WeightError(s,i);
        r->SetBinContent(i, sys );
        r->SetBinError(i, 0 );
        if (h) h->SetBinError(i, sqrt(pow(h->GetBinError(i),2)+sys*sys ));
    }
    return r;
}

void Print(TH1*h1, TH1*h2, TH1*we) {
    if (!h1 || !h2 || !we) return;
    for (int i=0; i<=h1->GetXaxis()->GetNbins()+1; ++i) {

        std::cout << "Bin "<<i<<"\n"
                  << "Direct simulation: "<<h1->GetBinContent(i)<<" +- "<<h1->GetBinError(i)<<"\n"
                  << "Prediction:        "<<h2->GetBinContent(i)<<" +- "<<h2->GetBinError(i)<<"(stat) +- "
                  <<we->GetBinContent(i)<<"\n"
                  <<std::endl;
    }
}

void ShowOverflow(TH1*h)
{
    int b =  h->GetXaxis()->GetNbins();
    //h->Sumw2();
    // to error propagation
    h->SetBinError( b, sqrt( pow(h->GetBinError(b),2)+pow(h->GetBinError(b+1),2)) );
    // add the overflow bin to the last bin
    h->SetBinContent( b, h->GetBinContent(b)+h->GetBinContent(b+1) );
    // clear overflow bin
    h->SetBinContent( b+1, 0 );
    h->SetBinError( b+1, 0 );
}

void Add2(TH1*h1, TH1*h2)
{
    assert(h1&&h1&&h1->GetXaxis()->GetNbins()==h2->GetXaxis()->GetNbins()&&"Assert in Tools.cc: void Add2(TH1*h1, TH1*h2) failed!");
    for (int i=0; i<=h1->GetXaxis()->GetNbins(); ++i) {
        h1->SetBinContent(i, sqrt(pow(h1->GetBinContent(i),2)+pow(h2->GetBinContent(i),2)));
    }
}

void DivByBinWidth(TH1*h)
{
    for (int i=0; i<=h->GetXaxis()->GetNbins(); ++i) {
        if (!h->GetXaxis()->GetBinWidth(i)) continue;
//    std::cout << "old: "<< h->GetBinContent(i);
        h->SetBinContent(i, h->GetBinContent(i) / h->GetXaxis()->GetBinWidth(i) );
        h->SetBinError(i,   h->GetBinError(i) / h->GetXaxis()->GetBinWidth(i) );
//    std::cout << ";  new: "<< h->GetBinContent(i)<<std::endl;
    }
}

TH1* TotalErrorUp(THStack*hs, TH1*w)
{
    TH1* h=(TH1F*)hs->GetStack()->Last();
    for (int i=0; i<=h->GetXaxis()->GetNbins(); ++i) {
        h->SetBinContent(i,  h->GetBinContent(i) + sqrt( pow(h->GetBinError(i),2) + pow(w->GetBinContent(i),2) ) );
        h->SetBinError(i, 0 );
    }
    h->SetLineColor( 18);
    h->SetFillColor( 18 );
    h->SetFillStyle( 1001 );
    return h;
}
TH1* TotalErrorDn(THStack*hs, TH1*w)
{
    TH1* h=(TH1F*)hs->GetStack()->Last();
    for (int i=0; i<=h->GetXaxis()->GetNbins(); ++i) {
        h->SetBinContent(i,  h->GetBinContent(i) - sqrt( pow(h->GetBinError(i),2) + pow(w->GetBinContent(i),2) ) );
        h->SetBinError(i, 0 );
    }
    h->SetLineColor( 18);
    h->SetFillColor( 10 );
    h->SetFillStyle( 1001 );
    return h;
}

void Divide(TH1*h1,TH1*h2)
{
  assert(h1->GetXaxis()->GetNbins() == h2->GetXaxis()->GetNbins());
  for (int i=0; i<=h1->GetXaxis()->GetNbins(); ++i) {
        double z = h1->GetBinContent(i);
        double ez= h1->GetBinError(i);
        double n = h2->GetBinContent(i);
        double en= h2->GetBinError(i);
	if (!n) continue;
	h1->SetBinContent(i, z/n);
        //h1->SetBinError(i,   sqrt( ez*ez/(n*n) + en*en*z*z/(n*n*n*n) ) );
        h1->SetBinError(i,   sqrt( ez*ez/(n*n)  ) );
  }
}

/// h1: pe Data, h2: l Prediction, we: syst. unc. pred.
void ratio(TH1*h1, TH1*h2, TH1*we,std::vector<TH1*> *sig,std::vector<TH1*> *other,const std::string& dir, const std::string& file,const std::string& legtitle, const std::string& log) {

    // Plot labels--------------------------------------------------------------
    std::string mylegtitle = "";
    if ( file.find( "Combined" ) != std::string::npos )
        mylegtitle = "Multijet, #gamma+jet";
    if ( file.find( "CombinedSignal" ) != std::string::npos )
        mylegtitle = "Multijet, #gamma+jet, Signal";
    if ( file.find( "QCD" ) != std::string::npos )
        mylegtitle = "Multijet";
    TLatex* plotCaption = new TLatex();
    plotCaption->SetNDC();
    plotCaption->SetTextFont( h1->GetLabelFont() );
    plotCaption->SetTextSizePixels( h1->GetLabelSize() );
    // whitespaces optimized for this pad size
    if ( file.find("Final") == 0 )
        plotCaption->SetText( .01, .94, "CMS Simulation        19.7fb^{-1} (8 TeV), #geq1#gamma,#geq2jets,#slash{E}_{T}#geq100GeV" );
    else
        plotCaption->SetText( .01, .94, "CMS Simulation                            19.7fb^{-1} (8 TeV), #geq1#gamma,#geq2jets" );
    if ( file.find("Data") != std::string::npos )
        plotCaption->SetText( .01, .94, "CMS Preliminary                           19.7fb^{-1} (8 TeV) #geq1#gamma,#geq2jets" );

    //assertions----------------------------------------------------------------
    assert(h1->GetXaxis()->GetNbins() == h2->GetXaxis()->GetNbins());
    if (!we) we=h2;

    //pads----------------------------------------------------------------------
    std::stringstream ss;
    ss<<plotnr++;
    TCanvas *c1 = new TCanvas(((std::string)"c_"+ss.str()).c_str(),"example",600,600);
    TPad *pad1 = new TPad(((std::string)"pada_"+ss.str()).c_str(),"pada",0,0.3,1,1);
    pad1->SetBottomMargin(0);
    pad1->SetLogy(log.find("log")!=std::string::npos);
    pad1->Draw();
    pad1->cd();
    TH1* h_axis = (TH1F*)h2->Clone();
    //overflowbin --------------------------------------------------------------
    if ( file.find("nooverflow")==std::string::npos ) {
	    ShowOverflow( h1 );
	    ShowOverflow( h2 );
	    ShowOverflow( we );
	    for (std::vector<TH1*>::iterator o=other->begin(); o!=other->end(); ++o)
        	if (*o) {
        	    //(*o)->Sumw2();
        	    ShowOverflow( *o );
        	}
	    for (std::vector<TH1*>::iterator s=sig->begin(); s!=sig->end(); ++s)
        	if (*s) ShowOverflow( *s );
    }	
    //divide by bin width -----------------------------------------------------
    if (log=="log_div") {
        DivByBinWidth(h1);
        DivByBinWidth(h2);
        DivByBinWidth(we);
        for (std::vector<TH1*>::iterator o=other->begin(); o!=other->end(); ++o)
            if (*o) DivByBinWidth( *o );
        for (std::vector<TH1*>::iterator s=sig->begin(); s!=sig->end(); ++s)
            if (*s) DivByBinWidth( *s );
    }
    //plotting style stuff ----------------------------------------------------
    h2->SetLineColor(2);
    h2->SetLineWidth(3);
    //h2->Sumw2();
    h1->SetLineColor( kBlack );
    //h1->Sumw2();
    we->SetLineColor(2);
    we->SetFillColor(2);
    we->SetFillStyle( 3354 );
    TH1F* cover = (TH1F*)we->Clone();
    cover->SetLineColor(2);
    cover->SetFillColor( 18 );
    cover->SetFillStyle( 1001 );
    we->SetTitle("");
    h_axis->SetTitle("");
    h_axis->GetYaxis()->SetTitleSize(0.05);
    h_axis->GetYaxis()->SetLabelSize(0.05);
    TH1F* hleg = (TH1F*)we->Clone();
    hleg->SetLineWidth(2);
    THStack hs("hs","CMS preliminary          #leq1#gamma, #leq2jets");
    gStyle->SetTitleSize(0.2,"t");
    // Do Legend -----------------------------------------------------------------------
    TLegend * leg = new TLegend(0.5,0.65-(0.02*(sig->size()+other->size())),0.89,0.89);
    leg->SetFillColor(0);
    leg->SetBorderSize(0);
    leg->AddEntry(h1,h1->GetTitle(),"pe");
    leg->AddEntry(hleg,h2->GetTitle(),"lef");
    for (std::vector<TH1*>::iterator s=sig->begin(); s!=sig->end(); ++s)
        if (*s) {
            leg->AddEntry(*s,  (*s)->GetTitle(),  "l");
        }
    // stack h2 and 'other' backgrounds -----------------------------------------------
    for (std::vector<TH1*>::iterator o=other->begin(); o!=other->end(); ++o)
        if (*o) {
            hs.Add(*o);
            if (((std::string)(*o)->GetTitle())!="")
                leg->AddEntry(*o,(*o)->GetTitle(),"f");
        }
    hs.Add( (TH1F*)h2->Clone());
    // syst. error band --------------------------------------------------------------- 	
    TH1* h_totalUp = TotalErrorUp((THStack*)hs.Clone(), (TH1F*)we->Clone());
    TH1* h_totalDn = TotalErrorDn((THStack*)hs.Clone(), (TH1F*)we->Clone());
    leg->SetHeader(mylegtitle.c_str());
    for (int i=1; i<=we->GetXaxis()->GetNbins(); ++i) {
        cover->SetBinContent(i, ((TH1F*)hs.GetStack()->Last())->GetBinContent(i) - we->GetBinContent(i));
        we->SetBinContent(   i, ((TH1F*)hs.GetStack()->Last())->GetBinContent(i) + we->GetBinContent(i));
        we->SetBinError(i,0);
        cover->SetBinError(i,0);
    }
    // plot range and axis style ------------------------------------------------------
    h_axis->GetYaxis()->SetTitleOffset(1.1);
    if (log=="linear") h_axis->SetMinimum(0);
    if (log=="linear" && h1->GetMaximum()>h_axis->GetMaximum()) h_axis->SetMaximum(h1->GetMaximum()+sqrt(h1->GetMaximum()));
    if (log.find("log")!=std::string::npos && h1->GetMaximum()>h_axis->GetMaximum()) h_axis->SetMaximum(5.*h1->GetMaximum());
    if (log.find("log")!=std::string::npos){// && (file.find( "phi")!=std::string::npos || file.find( "Phi")!=std::string::npos)){
        h_axis->SetMaximum(10.*h_axis->GetMaximum());
        h_axis->SetMinimum( 0.05 );
	if (other->size()>1) h_axis->SetMinimum( 2 );//plotting data
	if (log.find("div")!=std::string::npos)
          h_axis->SetMinimum( std::min(h1->GetMinimum(0),h2->GetMinimum(0))/3. );
    }		
    if (log.find("div")!=std::string::npos)
          h_axis->GetYaxis()->SetTitle("Events / GeV");
    h_totalUp->SetStats(0);
    h_totalUp->SetTitle("");
    h_totalDn->SetStats(0);
    h_totalDn->SetTitle("");
    //h1->SetStats(0);
    //h2->SetStats(0);
    //we->SetStats(0);
    cover->SetStats(0);
    h_axis->SetStats(0);
    // finally sraw stuff ----------------------------------------------------------------
    h_axis->DrawCopy("AXIS");
    h_totalUp->DrawCopy("hist,h,same");
    we->DrawCopy("h,same");
    cover->DrawCopy("hf,same");
    h_totalDn->DrawCopy("hist,h,same");
    hs.Draw("same hist fe");
    for (std::vector<TH1*>::iterator s=sig->begin(); s!=sig->end(); ++s)
        if (*s) (*s)->DrawCopy("hist,h,same");
    h1->DrawCopy("E X0,same");
    leg->Draw("same");
    pad1->RedrawAxis();
    plotCaption->Draw();
    ///
    /// ==========================  Ratio plot  =========================================
    ///
    c1->cd();
    TPad *pad2 = new TPad(((std::string)"padb_"+ss.str()).c_str(),"padb",0,0,1,0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.5);
    pad2->SetFillStyle(0);
    pad2->SetFrameFillColor(10);
    pad2->SetFrameBorderMode(0);
    pad2->Draw();
    pad2->cd();
    // Get total bgd hist, divide all hists by bgd --------------------------------------
    TH1F * bgd = (TH1F*)h2->Clone();
    //bgd->Sumw2();
    for (std::vector<TH1*>::iterator o=other->begin(); o!=other->end(); ++o)
        if (*o) bgd->Add( *o );
    TH1F* StatUnc = (TH1F*)bgd->Clone();
    //StatUnc->Sumw2();
    //we->Sumw2();
    //h_totalUp->Divide(bgd);
    //h_totalDn->Divide(bgd);
    //we->Divide(bgd);
    //cover->Divide(bgd);
    //h1->Divide(bgd);
    //StatUnc->Divide(bgd);
    Divide(h_totalUp,bgd);
    Divide(h_totalDn,bgd);
    Divide(we,       bgd);
    Divide(cover,    bgd);
    Divide(h1,       bgd);
    Divide(StatUnc,  bgd);
    // blotting style --------------------------------------------------------------------
    h_axis->SetMinimum(0.);
    h_axis->SetMaximum(3.5);
    h_axis->GetXaxis()->SetNdivisions(505);
    h_axis->GetYaxis()->SetTitle("Direct/Pred.");
    h_axis->GetYaxis()->SetTitleOffset(0.4);
    h_axis->GetYaxis()->SetTitleSize(0.125);
    h_axis->GetYaxis()->SetLabelSize(0.125);
    h_axis->GetXaxis()->SetTitleSize(0.125);
    h_axis->GetXaxis()->SetLabelSize(0.125);
    h_axis->GetYaxis()->SetNdivisions(503);
    h1->SetMarkerStyle(20);
    h1->SetMarkerSize(1);
    h1->SetMarkerColor(1);
    h1->SetLineColor(1);
    // draw stuff ------------------------------------------------------------------------
    h_axis->Draw("AXIS");
    h_totalUp->Draw("hist,h,same");
    we->Draw("hist,h,same");
    cover->Draw("hist,h,same");
    h_totalDn->Draw("hist,h,same");
    StatUnc->Draw("E0,X0,same");
    h1->Draw("E0,X0,same");
    TLine *line = new TLine(h1->GetXaxis()->GetXmin(), 1, h1->GetXaxis()->GetXmax(), 1);
    line->SetLineColor(1);
    line->SetLineStyle(2);
    line->Draw("same");
    pad2->RedrawAxis();
    if (file=="Closure_Data_met_arrow") {
        TLine * l = new TLine(100,0,100,3);
        l->SetLineWidth(4);
        l->Draw();
        pad1->cd();
        l->DrawLine(100,0.01,100,20000);
        TArrow * a = new TArrow(100,5000,150,5000);
        a->SetLineWidth(4);
        a->Draw();
    }
    c1->cd();
    c1->SaveAs((dir+"/"+log+"/"+file+"_log.pdf").c_str());
    delete pad2;
    delete pad1;
    delete c1;
    delete h1;
    delete h2;
    delete bgd;
    delete hleg;
    delete cover;
    delete we;
    delete plotCaption;
}

void RatioPlot(TH1*a, TH1*b,TH1*we, std::vector<TH1*> *sig, std::vector<TH1*> *other,const std::string& dir_,  const std::string& file, const std::string& t)
{
    //a: signal (direct simulation), can be 0
    //b: prediction
    std::string dir = "plots/"+dir_;
    struct stat st= {0};
    if(stat(dir.c_str(),&st)==-1)
        mkdir(dir.c_str(), 0700);
    if(stat(((std::string)dir+"/log/").c_str(),&st)==-1)
        mkdir(((std::string)dir+"/log/").c_str(), 0700);
    if(stat(((std::string)dir+"/linear/").c_str(),&st)==-1)
        mkdir(((std::string)dir+"/linear/").c_str(), 0700);
    if(stat(((std::string)dir+"/log_div/").c_str(),&st)==-1)
        mkdir(((std::string)dir+"/log_div/").c_str(), 0700);

    if (!a) {
        TCanvas * c1 = new TCanvas("","",600,600);
        gPad->SetLogy(0);
        b->SetLineColor(2);
        b->SetLineWidth(3);
        b->Draw("he");
        c1->SaveAs((dir+"/linear/"+file+".pdf").c_str());
        gPad->SetLogy(1);
        b->Draw("he");
        c1->SaveAs((dir+"/log/"+file+"_log.pdf").c_str());
        return;
    }
    a->SetMarkerStyle(8);
    ratio((TH1F*)a->Clone(),(TH1F*)b->Clone(),(TH1F*)we->Clone(),sig,other,dir,file,t,"log");
    ratio((TH1F*)a->Clone(),(TH1F*)b->Clone(),(TH1F*)we->Clone(),sig,other,dir,file,t,"linear");
    ratio((TH1F*)a->Clone(),(TH1F*)b->Clone(),(TH1F*)we->Clone(),sig,other,dir,file,t,"log_div");
//  ratio((TH1F*)a->Clone(),(TH1F*)b->Clone(),0,dir,file,t,true);
//  ratio((TH1F*)a->Clone(),(TH1F*)b->Clone(),0,dir,file,t,false);
//  if (file=="Closure_Combined_met") Print(a,b,we);
//  if (file=="Closure_QCD_met") Print(a,b,we);
//  if (file=="Closure_Data_met_fibo") Print(a,b,we);
}


void PrintBinning(std::ostream& os, const std::string& s, MyYields*y)
{
    os << "# " << s <<"\n";
    int min=1;
    for (int i=1; i<y->GetNBins(s); ++i) {
        if (s=="met"&&y->GetBinBorder(s,i) <= 100.) min =i+1;;
    }
    if (y->GetNBins(s)>1)
        for (int i=min; i<y->GetNBins(s); ++i)
            os << "bin "<<i-min<<" = "<<y->GetBinBorder(s,i-1)<<" to "<<y->GetBinBorder(s,i)<<"\n";
    os << "bin "<<y->GetNBins(s)-min<<" = "<<y->GetBinBorder(s,y->GetNBins(s)-1)<<" to infinity \n";
    os << "nMetBins = "<<y->GetNBins(s) - min+1<<"\n";
    os << "variable = "<<s<<std::endl;
}
void PrintResult(std::ostream& os, const std::string& s, const std::string& n, MyYields*y)
{
    int nbins=y->GetNBins(s);
    int min=1;
    for (int i=1; i<nbins; ++i) {
        if (s=="met"&&y->GetBinBorder(s,i) <= 100.) min =i+1;;
    }
    os << n << " = ";
    for (int i=min; i<=nbins; ++i) {
        os << y->Weighted(s,i) << "  ";
    }
    os << "\n" << n << " stat uncert = ";
    for (int i=min; i<=nbins; ++i) {
        os << y->Error(s,i) << "  ";
    }
    os << "\n" << n << " syst uncert = ";
    for (int i=min; i<=nbins; ++i) {
        os << y->WeightError(s,i) << "  ";
    }
    os << std::endl;
}

void PrintResults(const std::string& dir, std::string file, std::string name, MyYields* direct, MyYields* yields, std::vector<MyYields*>* other )
{
    std::string d = "Results/"+dir;
    struct stat st= {0};
    if(stat(d.c_str(),&st)==-1)
        mkdir(d.c_str(), 0700);
    std::ofstream out;
    out.open ( (d+"/"+file+".txt").c_str() );
    PrintBinning( out, name, direct );
    PrintResult(  out, name, "data selected", direct);
    PrintResult(  out, name, "data QCD", yields);
    for (std::vector<MyYields*>::iterator oth=other->begin(); oth!=other->end(); ++oth)
        PrintResult( out, name, (*oth)->ResultName(), *oth);
    out.close();
}

void setStyle() {
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
    gStyle->SetPalette(56);
    gStyle->SetNumberContours(999);
}
