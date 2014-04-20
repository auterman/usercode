#include "Tools.h"

#include "TPad.h"
#include "TLine.h"
#include "TLegend.h"



void Histograms::Book()
{
  h_["met"]       = new TH1F(((std::string)"h_met"+label_).c_str(),";MET [GeV];events",n_metbins,metbins);
  h_["met_const"] = new TH1F(((std::string)"h_met_const"+label_).c_str(),";MET [GeV];events",20,0,500);
  h_["mht"]       = new TH1F(((std::string)"h_mht"+label_).c_str(),";MHT [GeV];events",20,0,1000);
  h_["ht" ]       = new TH1F(((std::string)"h_ht"+label_).c_str(),";HT [GeV];events",20,400,1500);
  h_["em1_pt" ]   = new TH1F(((std::string)"h_em1_pt"+label_).c_str(),";p_{T}^{em}_1 [GeV];events",20,0,500);
  h_["em1_thePt" ]   = new TH1F(((std::string)"h_em1_thePt"+label_).c_str(),";the p_{T}^{em,*}_1 [GeV];events",20,0,500);
  h_["weight" ]   = new TH1F(((std::string)"weight"+label_).c_str(),";weight;events",20,0,4);

  h_["em1_phi" ]   = new TH1F(((std::string)"h_em1_phi"+label_).c_str(),";#phi^{em}_1;events",20,-3.2,3.2);
  h_["em1_eta" ]   = new TH1F(((std::string)"h_em1_eta"+label_).c_str(),";#eta^{em}_1;events",20,-2.5,2.5);
  h_["em1_ptstar" ]= new TH1F(((std::string)"h_em1_ptstar"+label_).c_str(),";p_{T}^{*,em}_1 [GeV];events",20,0,500);
  h_["em1_sigmaIphiIphi" ]= new TH1F(((std::string)"h_em1_sigmaIphiIphi"+label_).c_str(),";#sigma_{i#phi#i#phi}^{em}_1 [GeV];events",50,0.005,0.03);
  h_["em1_sigmaIetaIeta" ]= new TH1F(((std::string)"h_em1_sigmaIetaIeta"+label_).c_str(),";#sigma_{i#eta#i#eta}^{em}_1 [GeV];events",50,0.005,0.03);
  h_["em1_r9" ]= new TH1F(((std::string)"h_em1_r9"+label_).c_str(),";R9^{em}_1 [GeV];events",50,0.0,1.0);
  h_["em1_hadTowOverEm" ]= new TH1F(((std::string)"h_em1_hadTowOverEm"+label_).c_str(),";hadTowOverEm^{em}_1 [GeV];events",50,0.,1.6);
  h_["em1_chargedIso" ]= new TH1F(((std::string)"h_em1_chargedIso"+label_).c_str(),";chargedIso^{em}_1 [GeV];events",50,0.,150);
  h_["em1_neutralIso" ]= new TH1F(((std::string)"h_em1_neutralIso"+label_).c_str(),";neutralIso^{em}_1 [GeV];events",50,0.,40);
  h_["em1_photonIso" ]= new TH1F(((std::string)"h_em1_photonIso"+label_).c_str(),";photonIso^{em}_1 [GeV];events",50,0.,100);
  h_["em1_conversionSafeVeto" ]= new TH1F(((std::string)"h_em1_conversionSafeVeto"+label_).c_str(),";conversionSafeVeto^{em}_1 [GeV];events",2,-0.5,1.5);
  h_["em1_pixelseed" ]= new TH1F(((std::string)"h_em1_pixelseed"+label_).c_str(),";pixelseed^{em}_1 [GeV];events",50,0.005,0.03);
  h_["em1_genInformation" ]= new TH1F(((std::string)"h_em1_genInformation"+label_).c_str(),";genInformation^{em}_1 [GeV];events",2,-0.5,1.5);
  h_["em1_matchedJetIndex" ]= new TH1F(((std::string)"h_em1_matchedJetIndex"+label_).c_str(),";matchedJetIndex^{em}_1 [GeV];events",7,-1.5,5.5);
}

void Histograms::Add(Histograms* r)
{
  for (std::map<std::string, TH1*>::iterator it=r->GetAll()->begin();it!=r->GetAll()->end(); ++it){
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
   struct stat st={0};
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
    for (std::map<int,Yield>::iterator it=ryield->begin(); it!=ryield->end(); ++it)
      yield[it->first].Add( it->second.Get() );
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
  TH1 * r = new TH1F(ss.str().c_str(),(";"+s+";events").c_str(),nbins-1,bins);
 
  //std::map<int,Yield> * mp = GetYieldsRef(s)->GetYields();
 
  for (int i=0; i<nbins; ++i) {
    r->SetBinContent(i, Weighted(s,i) );
    r->SetBinError(  i, Error(s,i) );
    //std::cout << " my bin "<<i<<": > "<<bins[i]
    //          << " contents: "<< r->GetBinContent(i)<<" +- "<<r->GetBinError(i)
    //	      << " map[]: "<<(*mp)[i].weighted()<<" +- "<<(*mp)[i].error()
    //	      <<std::endl;
  }
  return r;
}



void ratio(TH1*h1, TH1*h2, const std::string& dir, const std::string& file,const std::string& legtitle, bool log) {
   std::stringstream ss;
   ss<<plotnr++;
   TCanvas *c1 = new TCanvas(((std::string)"c_"+ss.str()).c_str(),"example",600,600);
   TPad *pad1 = new TPad(((std::string)"pada_"+ss.str()).c_str(),"pada",0,0.3,1,1);
   pad1->SetBottomMargin(0);
   pad1->SetLogy(log);
   pad1->Draw();
   pad1->cd();
   TLegend * leg = new TLegend(0.5,0.7,0.89,0.92);
   leg->SetFillColor(0);
   leg->SetBorderSize(0);
   leg->AddEntry(h1,h1->GetTitle(),"pe");
   leg->AddEntry(h2,h2->GetTitle(),"l");
   leg->SetHeader(legtitle.c_str());
   h1->SetTitle("");
   h1->GetYaxis()->SetTitleSize(0.05);
   h1->GetYaxis()->SetLabelSize(0.05);
   h1->GetYaxis()->SetTitleOffset(1.1);
   if (!log) h1->SetMinimum(0);
   if (!log && h2->GetMaximum()>h1->GetMaximum()) h1->SetMaximum(h2->GetMaximum()+sqrt(h2->GetMaximum()));
   if (log  && h2->GetMaximum()>h1->GetMaximum()) h1->SetMaximum(5.*h2->GetMaximum());   
   h1->SetStats(0);
   h2->SetStats(0);
   h1->DrawCopy("pe");
   h2->Draw("l,same");
   c1->cd();
   leg->Draw("same");
   TPad *pad2 = new TPad(((std::string)"padb_"+ss.str()).c_str(),"padb",0,0,1,0.3);
   pad2->SetTopMargin(0);
   pad2->SetBottomMargin(0.5);
   pad2->SetFillStyle(0);
   pad2->SetFrameFillColor(10);
   pad2->SetFrameBorderMode(0);
   pad2->Draw();
   pad2->cd();
   h1->Sumw2();
   h1->Divide(h2);
   h1->SetMinimum(0.);
   h1->SetMaximum(2.4);
   h1->GetXaxis()->SetNdivisions(505);
   h1->GetYaxis()->SetTitle("Direct/Pred.");
   h1->GetYaxis()->SetTitleSize(0.125);
   h1->GetYaxis()->SetLabelSize(0.125);
   h1->GetXaxis()->SetTitleSize(0.125);
   h1->GetXaxis()->SetLabelSize(0.125);
   h1->GetYaxis()->SetNdivisions(503);
   h1->SetMarkerStyle(20);
   h1->SetMarkerSize(1);
   h1->SetMarkerColor(1);
   h1->SetLineColor(1);
   h1->Draw("E X0");
   TLine *line = new TLine(h1->GetXaxis()->GetXmin(), 1, h1->GetXaxis()->GetXmax(), 1);
   line->SetLineColor(1);
   line->SetLineStyle(2);
   line->Draw("same");
   c1->cd();
   if (log) c1->SaveAs((dir+"/log/"+file+"_log.pdf").c_str());
   else     c1->SaveAs((dir+"/linear/"+file+".pdf").c_str());
   delete pad2;
   delete pad1;
   delete c1;
   delete h1;
   delete h2;
}

void RatioPlot(TH1*a, TH1*b, const std::string& dir,  const std::string& file, const std::string& t)
{
  //a: signal (direct simulation), can be 0
  //b: prediction
  b->SetLineColor(2);
  b->SetLineWidth(3);
  struct stat st={0};
  if(stat(dir.c_str(),&st)==-1)
     mkdir(dir.c_str(), 0700);
  if(stat(((std::string)dir+"/log/").c_str(),&st)==-1)
     mkdir(((std::string)dir+"/log/").c_str(), 0700);
  if(stat(((std::string)dir+"/linear/").c_str(),&st)==-1)
     mkdir(((std::string)dir+"/linear/").c_str(), 0700);
  if (!a) {
    TCanvas * c1 = new TCanvas("","",600,600);  
    gPad->SetLogy(0);
    b->Draw("he");
    c1->SaveAs((dir+"/linear/"+file+".pdf").c_str());
    gPad->SetLogy(1);
    b->Draw("he");
    c1->SaveAs((file+"/log/"+"_log.pdf").c_str());
    return;
  }
  a->SetMarkerStyle(8);
  ratio((TH1F*)a->Clone(),(TH1F*)b->Clone(),dir,file,t,true);
  ratio((TH1F*)a->Clone(),(TH1F*)b->Clone(),dir,file,t,false);
}

