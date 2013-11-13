#ifndef MYSELECTOR_H
#define MYSELECTOR_H

#include <iostream>

#include "TH1F.h"
#include "TH2F.h"
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH1F.h>
#include <TList.h>

#include "PhotonSelector.h"

class MySelector : public PhotonSelector {

  public:
   MySelector()    {  }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual Bool_t  Process(Long64_t entry);
   virtual void    SlaveTerminate();
   virtual void    Terminate();
   

  //private member functions
  private:
   //processors
   bool doQcdWeight() const;
   bool doQcdClosure() const;    
   //finalizers
   template<typename T> void GetAndWrite(const std::string hist, const std::string fname, const std::string opt) const;
   void QcdWeightHistogram() const;
   void QcdClosure() const;

  //private member variables
  private: 
   long nentries;
   
   //QCD weights
   TH1F *h_pt, *h_ht, *h_met,  
        *w_pt, *w_ht, *w_met;
   TH2F *h_pt_ht, *w_pt_ht;	

   TH2F *h_weight, *h_err;	

   //QCD closure
   TH1F *h_qcdclosure_signal_photonpt, *h_qcdclosure_prediction_photonpt,*h_qcdclosure_error_photonpt,
        *h_qcdclosure_signal_met, *h_qcdclosure_prediction_met,*h_qcdclosure_error_met,
	*h_qcdclosure_signal_ht, *h_qcdclosure_prediction_ht, *h_qcdclosure_error_ht;
   
  private: 
  ClassDef(MySelector,0) //no ";"!
};

#endif
