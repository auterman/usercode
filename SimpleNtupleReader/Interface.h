#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include <map>
#include <vector>
#include <iostream>

#include "TMath.h"

#include "GJets_Photon.h"
#include "GJets_Jet.h"
#include "QCD_Photon.h"
#include "QCD_Jet.h"


const static Double_t kPI	 = TMath::Pi();
const static Double_t kTWOPI	 = 2.*kPI;

Double_t DeltaPhi(Double_t x, Double_t y);
static int plotnr = 0;


double transverse_met(GJets_Photon*t);
double transverse_met(GJets_Jet*t);
double transverse_met(QCD_Photon*t);
double transverse_met(QCD_Jet*t);

double parallel_met(GJets_Photon*t);
double parallel_met(GJets_Jet*t);
double parallel_met(QCD_Photon*t);
double parallel_met(QCD_Jet*t);

bool tight_isolated(double pt, double ptstar, double phi, double eta, int seed, double HoE, 
                    double sigmaIetaOeta, double chaIso, double neuIso, double phoIso);
bool loose_isolated(double pt, double ptstar, double phi, double eta, int seed, double HoE, 
                    double sigmaIetaOeta, double chaIso, double neuIso, double phoIso);



template <typename T> 
class Processor {
 public:
  Processor(std::string n):name_(n){}
  void Add(  bool(*f)(TSelector*,Long64_t,Long64_t,double) ){genfuncts_.push_back(f);}
  void Add(  bool(*f)(T*,Long64_t,Long64_t,double) ){functs_.push_back(f);}
  virtual void Init(){std::cout<<"...processing '"<<name_<<"'"<<std::endl;}
  virtual bool Process(T*t,Long64_t i,Long64_t n, double w){
    bool result = true;
    for (std::vector< bool(*)(TSelector*,Long64_t,Long64_t,double) >::iterator it=genfuncts_.begin();
         it!=genfuncts_.end();++it) {
      result *= (*it)(t,i,n,w);
      if (!result) break;
    }
    for (typename std::vector< bool(*)(T*,Long64_t,Long64_t,double) >::iterator it=functs_.begin();
         it!=functs_.end();++it) {
      result *= (*it)(t,i,n,w);
      if (!result) break;
    }
    return result;
  }
  virtual void Terminate(){}
 
 protected:
  std::string name_; 
  std::vector< bool(*)(TSelector*,Long64_t,Long64_t,double) > genfuncts_;
  std::vector< bool(*)(T*,Long64_t,Long64_t,double) > functs_;
};


template<class T>
class Interface {
  public:
    Interface(TChain*chain, double);
    void Process(std::vector<Processor<T>*>&);
    void Terminate();

  private:
   int nentries_;
   T * proc_; 
   double weight_;
};


template<class T>
Interface<T>::Interface(TChain*chain, double w) 
{
  nentries_ = chain->GetEntries();
  proc_ = new T();
  proc_->Init(chain);
  weight_=w;
}

template<class T>
void Interface<T>::Process( std::vector<Processor<T>*>& t )
{
  for (typename std::vector<Processor<T>*>::iterator it=t.begin();it!=t.end();++it)
     (*it)->Init();
  for (int i=0; i<nentries_; ++i) {
    proc_->GetEntry( i );
    proc_->Process( i );
    for (typename std::vector<Processor<T>*>::iterator it=t.begin();it!=t.end();++it)
      if (!(*it)->Process(proc_,i,nentries_,weight_)) break;
  }  
  for (typename std::vector<Processor<T>*>::iterator it=t.begin();it!=t.end();++it)
     (*it)->Terminate();
}

template<class T>
void Interface<T>::Terminate() 
{
  proc_->Terminate();  
}



#endif
