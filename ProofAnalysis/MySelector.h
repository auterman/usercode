#ifndef MYSELECTOR_H
#define MYSELECTOR_H

#include "QcdSelector.h"

#include "TH1F.h"


class MySelector : public QcdSelector {

  public:
   MySelector(TTree * /*tree*/=0):QcdSelector(0){}
   virtual ~MySelector() { }

   virtual Int_t   Version() const { return 2; }

   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual Bool_t  Process(Long64_t entry);
   virtual void    SlaveTerminate();
   virtual void    Terminate();


  //private member functions
  private:
   bool isTightIsolated(unsigned i) const;
   bool QcdWeightSelection() const;
   bool GetIdx_TightPhoton(unsigned * idx) const;
   bool GetIdx_FakePhoton(unsigned * idx) const;

  //public member variables
  public: 
   TH1F* h_weight_photonpt;
   TH1F* h_weight_fakept;

};

#endif
