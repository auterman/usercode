// $Id: Analyzer.cc,v 1.11 2013/06/17 16:15:34 auterman Exp $

/*** ------------------------------------------------------------------------------------------------------- ***
     NTupleFactory, a tool to plot final results, using Root Reflex information

     Christian Autermann, RWTH Aachen, May 2013
 *** ------------------------------------------------------------------------------------------------------- ***/


#include <iostream>
#include <cmath>

#include "TH1.h"
#include "TH2.h"
#include "TLegend.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TStyle.h"

#include "Analyzer.h"
#include "Event.h"
#include "Selection.h"
#include "table.h"

//plotting 2D
void Plot2D::InitSample(const Selection*sel, const Sample* sample)
{
  filename_="results/TH2_"+sel->Name()+"_"+sample->GetName()+"_"+v1_->Label()+"_"+v2_->Label();
}
void Plot2D::Process(const Event * evt)
{
  if (v3_)   h_->Fill((*v1_)(evt), (*v2_)(evt), (*v3_)(evt)*evt->GetSample()->GetWeight());  
  else       h_->Fill((*v1_)(evt), (*v2_)(evt), evt->GetSample()->GetWeight() );
}
void Plot2D::EndSample()
{
  c_->cd();
  h_->Draw("");
  c_->SaveAs((filename_+".pdf").c_str());
  c_->SaveAs((filename_+".png").c_str());
}




//plotting 1D
void Plot1D::InitSample(const Selection*sel, const Sample* sample)
{
  filename_="results/TH1_"+sel->Name()+"_"+sample->GetLabel()+"_"+v1_->Label();
}
void Plot1D::Process(const Event * evt)
{
  if (v2_) h_->Fill((*v1_)(evt), (*v2_)(evt)*evt->GetSample()->GetWeight() );
  else     h_->Fill((*v1_)(evt), evt->GetSample()->GetWeight());
}
void Plot1D::EndSample()
{
  c_->cd();
  h_->Draw("h");
  c_->SaveAs((filename_+".pdf").c_str());
  c_->SaveAs((filename_+".png").c_str());
} 





// Print some event variables
void Print::Process(const Event * evt)
{
  std::cout << "Evt " << processed_ << " of sample "
            <<evt->GetSample()->GetName() <<":  "<< v1_->Name() <<" = " << (*v1_)(evt);
  if (v2_)   std::cout << ",  "<< v2_->Name() <<" = " << (*v2_)(evt);
  if (v3_)   std::cout << ",  "<< v3_->Name() <<" = " << (*v3_)(evt);
  std::cout << std::endl;
  ++processed_;
}




// Print Status 
void PrintStatus::InitSample(const Selection*sel, const Sample* sample){
  processed_=0;
  nentries_=sample->GetNevents();
  std::cout<<"...reading sample '"<<sample->GetName()<<"' ("<<sample->GetNevents()<<" events): "<<std::flush;
}
void PrintStatus::Process(const Event * evt)
{
  if (processed_%(nentries_/n_)==0) 
    std::cout<<"->"<<processed_/(nentries_/n_)*(100/n_)<<"%"<<std::flush;
  ++processed_;
  if (processed_==nentries_)
    std::cout<<std::endl;
}




//cutflow table --------------------------------------
void CutFlow::InitSample(const Selection*sel,const Sample* sample)
{
  s_.push_back(sample);
  std::pair<const Sample*,const Selection*> p = std::make_pair(sample,sel);
  total[p]=first[p]=final[p]=nminus1[p]=0;
  tables_[sample] = new Table::TTable( "Cutflow table for sample '"+sample->GetName()+"'", " | " );
}

void CutFlow::Process(const Event*evt)
{
  const Sample * s = evt->GetSample();
  if (std::find(s_.begin(),s_.end(),s)==s_.end()) return;
  float w = (weight_?(*weight_)(evt)*s->GetWeight():s->GetWeight());

  //init temp array with true
  std::vector<bool> cuts(cuts_.size()), n1(cuts_.size());
  for(std::vector<bool>::iterator it=n1.begin(); it!=n1.end(); ++it) (*it)=true;
  
  //check which selections current event passes 
  bool accum = true;
  for (std::vector<const Selection*>::const_iterator cut=cuts_.begin();cut!=cuts_.end();++cut) { 
    bool pass = (*cut)->Pass(evt);
    accum *= pass;
    cuts[cut-cuts_.begin()] = pass;
    std::pair<const Sample*,const Selection*> p = std::make_pair(s,*cut);    
    total[p] += w;
    if (pass)  first[p] += w;
    if (accum) final[p] += w;
  }
  
  //calculate (n-1) stuff:
  for(std::vector<bool>::iterator it=n1.begin(); it!=n1.end(); ++it) 
    for(std::vector<bool>::iterator tc=cuts.begin(); tc!=cuts.end(); ++tc){
      if (tc-cuts.begin()!=it-n1.begin())
        (*it) = (*it)*(*tc);
    }  
  for (std::vector<const Selection*>::const_iterator cut=cuts_.begin();cut!=cuts_.end();++cut) {
    if (n1[cut-cuts_.begin()]) nminus1[std::make_pair(s,*cut)] += w;
    
  }
}

void CutFlow::EndJob()
{
  for ( std::vector<const Sample*>::const_iterator sample=s_.begin();sample!=s_.end();++sample) {
    Table::TTable *t=tables_[*sample];
    t->AddColumn<std::string>("Selection");    
    t->AddColumn<double>     ("total evts");
    t->AddColumn<double>     ("evts pass");
    t->AddColumn<double>     ("this eff.");
    t->AddColumn<double>     ("(n-1) evts");
    t->AddColumn<double>     ("(n-1) eff.");
    t->AddColumn<double>     ("accum. evts");
    t->AddColumn<double>     ("accum. eff.");
    std::stringstream ss;
    ss << "Sample '"<<(*sample)->GetName()<<"' has "<<(*sample)->GetNevents()<< " and weight="<<(*sample)->GetWeight();
    t->SetCaption( ss.str() );
    for (std::vector<const Selection*>::const_iterator sel=cuts_.begin();sel!=cuts_.end();++sel) {
      std::pair<const Sample*,const Selection*> p = std::make_pair(*sample,*sel);
      if (total[p])
        *t << (*sel)->Name() //Name of cut/selection
	   << total[p]
	   << first[p]   << 100*first[p]/total[p]   //eff if cut is applied as first
	   << nminus1[p] << 100*nminus1[p]/total[p] //(n-1) cut
	   << final[p]   << 100*final[p]/total[p];  //accum. eff
    }
    if (file_=="")
      std::cout << *t <<std::endl;
    else{
      //print to file... 
    
    }    
  }  
}





//plotting Stack -------------------------------------
void Stack::InitSample(const Selection*sel, const Sample* sample)
{
  filename_="results/THStack_"+sel->Name()+"_"+v1_->Label();
  
  //Init histogram for new sample
  if (hists_.find(sample->GetLabel())==hists_.end())
    hists_[sample->GetLabel()]=(TH1*)h_->Clone();
  h_active_=hists_[sample->GetLabel()];
  if (sample->GetLineColor()>=0  ) h_active_->SetLineColor(   sample->GetLineColor() );  
  if (sample->GetFillColor()>=0  ) h_active_->SetFillColor(   sample->GetFillColor() );  
  if (sample->GetLineStyle()>=0  ) h_active_->SetLineStyle(   sample->GetLineStyle() );  
  if (sample->GetMarkerColor()>=0) h_active_->SetMarkerColor( sample->GetMarkerColor() );  
  if (sample->GetMarkerStyle()>=0) h_active_->SetMarkerStyle( sample->GetMarkerStyle() );  
}
void Stack::Process(const Event * evt)
{
  if (v2_) h_active_->Fill((*v1_)(evt), (*v2_)(evt)*evt->GetSample()->GetWeight() );
  else     h_active_->Fill((*v1_)(evt), evt->GetSample()->GetWeight());
}

struct HistSortHelper {
  HistSortHelper(double integral,std::string label,std::string DrawOpt,TH1 * h):
                 integral(integral),label(label),DrawOpt(DrawOpt),h(h){}
  double integral;
  std::string label, DrawOpt;
  TH1 * h;
  bool operator<(const HistSortHelper &rhs) const { return integral<rhs.integral; }
};

void Stack::EndJob()
{
  std::vector<HistSortHelper> hists;
  for (std::map<std::string,TH1*>::iterator it=hists_.begin();it!=hists_.end();++it)
    hists.push_back(HistSortHelper(it->second->Integral(),it->first,"f",it->second));
  std::sort(hists.begin(), hists.end());

  stack_ = new THStack();
  TLegend leg(0.5, 0.5, .9, .9);
  leg.SetFillColor(0);

  for (std::vector<HistSortHelper>::iterator it=hists.begin(); it!=hists.end(); ++it){
    stack_->Add(it->h);
    leg.AddEntry(it->h,it->label.c_str(),it->DrawOpt.c_str());
  }
  h_->SetMaximum(stack_->GetMaximum()+sqrt(stack_->GetMaximum()));

  c_->cd();
  int isLogy = gStyle->GetOptLogy();

  gPad->SetLogy(0);
  h_->Draw();
  stack_->Draw("same");
  leg.Draw();
  c_->SaveAs((filename_+"_linear.pdf").c_str());
  c_->SaveAs((filename_+"_linear.png").c_str());

  gPad->SetLogy(1);
  h_->SetMinimum(0.1);
  h_->Draw();
  stack_->Draw("same");
  leg.Draw();
  c_->SaveAs((filename_+"_log.pdf").c_str());
  c_->SaveAs((filename_+"_log.png").c_str());
  gPad->SetLogy(isLogy);
} 









//plotting Closure -------------------------------------
void Closure::Add(const Sample*samp, const Selection*sel, TH1*h, Variable*v1,Variable*v2,Variable*v3)
{
  Key key = std::make_pair(samp,sel);
  hists_[key]=(TH1*)h->Clone();
  v1_[key]=v1;
  v2_[key]=v2;
  v3_[key]=v3;
  selections_.push_back(sel);
  filename_="results/Closure_"+samp->GetName()+"_"+v1->Label();
}
void Closure::Process(const Event * evt)
{
  for (std::vector<const Selection*>::const_iterator sel=selections_.begin(); sel!=selections_.end(); ++sel) {
    if ( !(*sel)->Pass(evt) ) continue;
    Key key = std::make_pair(evt->GetSample(),*sel);
    TH1* h = hists_[key];
    if (v3_[key])      h->Fill((*v1_[key])(evt), (*v3_[key])(evt)*(*v2_[key])(evt)*evt->GetSample()->GetWeight() );
    else if (v2_[key]) h->Fill((*v1_[key])(evt), (*v2_[key])(evt)*evt->GetSample()->GetWeight() );
    else               h->Fill((*v1_[key])(evt), evt->GetSample()->GetWeight());
  }  
}
void Closure::EndJob()
{
  if (!hists_.size()) {
    std::cout << "WARNING! Closure::EndJob(); var = "<<v1_.begin()->second->Label()<<" found no filled histograms! Forget to Add(..)?"<<std::endl;
    return;
  }  
  
  TLegend leg(0.5, 0.5, .9, .9);
  leg.SetFillColor(0);
  int isLogy = gStyle->GetOptLogy();
  c_->cd();
  float maximum = 0;
  for (std::map<Key,TH1*>::iterator it=hists_.begin();it!=hists_.end();++it){
    leg.AddEntry(it->second,it->second->GetTitle());
    if (it->second->GetMaximum()>maximum) maximum=it->second->GetMaximum();
  }

  gPad->SetLogy(0);
  std::map<Key,TH1*>::iterator it=hists_.begin();
  it->second->SetMaximum(maximum+sqrt(maximum));
  if (it->second->GetFillColor()>=0)   it->second->Draw("h");
  if (it->second->GetLineColor()>=0)   it->second->Draw("l");
  if (it->second->GetMarkerStyle()>=0) it->second->Draw("pe");
  ++it;
  for (;it!=hists_.end();++it){
    if (it->second->GetFillColor()>=0)   it->second->Draw("h,same");
    if (it->second->GetLineColor()>=0)   it->second->Draw("l,same");
    if (it->second->GetMarkerStyle()>=0) it->second->Draw("pe,same");
  }  
  leg.Draw("same");
  c_->SaveAs((filename_+"_linear.pdf").c_str());
  c_->SaveAs((filename_+"_linear.png").c_str());

  gPad->SetLogy(1); 
  it=hists_.begin();
  it->second->SetMinimum(0.1);
  it->second->SetMaximum(1.5*maximum);
  if (it->second->GetFillColor()>=0)   it->second->Draw("h");
  if (it->second->GetLineColor()>=0)   it->second->Draw("l");
  if (it->second->GetMarkerStyle()>=0) it->second->Draw("pe");
  ++it;
  for (;it!=hists_.end();++it){
    if (it->second->GetFillColor()>=0)   it->second->Draw("h,same");
    if (it->second->GetLineColor()>=0)   it->second->Draw("l,same");
    if (it->second->GetMarkerStyle()>=0) it->second->Draw("pe,same");
  }  
  leg.Draw("same");
  c_->SaveAs((filename_+"_log.pdf").c_str());
  c_->SaveAs((filename_+"_log.png").c_str());
  gPad->SetLogy(isLogy);
} 





