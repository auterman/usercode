#include "Tools.h"

#include <sstream>

void Histograms::Book()
{
  h_["met"]       = new TH1F(((std::string)"h_met"+label_).c_str(),";MET [GeV];events",n_metbins,metbins);
  h_["met_const"] = new TH1F(((std::string)"h_met_const"+label_).c_str(),";MET [GeV];events",20,0,500);
  h_["mht"]       = new TH1F(((std::string)"h_mht"+label_).c_str(),";MHT [GeV];events",20,0,1000);
  h_["ht" ]       = new TH1F(((std::string)"h_ht"+label_).c_str(),";HT [GeV];events",20,400,1500);
  h_["em1_pt" ]   = new TH1F(((std::string)"h_em1_pt"+label_).c_str(),";p_{T}^{em} [GeV];events",20,0,500);
  h_["weight" ]   = new TH1F(((std::string)"weight"+label_).c_str(),";weight;events",20,0,4);
 
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
   TCanvas * c1 = new TCanvas("","",600,600);
   for (std::map<std::string, TH1*>::iterator it=h_.begin(); it!=h_.end(); ++it)
   {
     gPad->SetLogy(0);
     it->second->Draw("he");
     c1->SaveAs(((std::string)"plots/"+label_+"_"+it->first+".pdf").c_str());
     gPad->SetLogy(1);
     it->second->Draw("he");
     c1->SaveAs(((std::string)"plots/"+label_+"_"+it->first+"_log.pdf").c_str());
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
  std::cout << label_ << "_" << s << "_" << plotnr<<std::endl;
  ss  << s << "_" << plotnr++;
  int nbins=GetNBins(s);
  double bins[nbins];
  //std::cout << "binning for >>"<<s<<"<<"<<std::endl;  
  for (int i=0; i<nbins; ++i) { 
    bins[i]=GetBinBorder(s,i);
  }
  TH1 * r = new TH1F(ss.str().c_str(),(";"+s+";events").c_str(),nbins-1,bins);
  
  for (int i=0; i<nbins; ++i) {
    r->SetBinContent(i, Weighted(s,i) );
    r->SetBinError(  i, Error(s,i) );
    //std::cout << " bin "<<i<<": > "<<bins[i]
    //          << " contents: "<< r->GetBinContent(i)<<" +- "<<r->GetBinError(i)
    //	      <<std::endl;
  }
  return r;
}

