// @(#)root/hist:$Id: TConfidenceLevel.h,v 1.1.1.1 2011/01/26 14:37:51 auterman Exp $
// Author: Christophe.Delaere@cern.ch   21/08/2002

#ifndef ROOT_TConfidenceLevel
#define ROOT_TConfidenceLevel

#ifndef ROOT_TObject
#include "TObject.h"
#endif

//____________________________________________________________________
//
// TConfidenceLevel
//
// This class serves as output for the TLimit::ComputeLimit method.
// It is created just after the time-consuming part and can be stored 
// in a TFile for further processing. It contains
// light methods to return CLs, CLb and other interesting quantities.
//____________________________________________________________________


class TConfidenceLevel : public TObject {
 public:
   TConfidenceLevel();
   TConfidenceLevel(Int_t mc, bool onesided = kTRUE);
   virtual ~TConfidenceLevel();
   inline void SetTSD(Double_t in) { fTSD = in; } 
   void SetTSB(Double_t * in);
   void SetTSS(Double_t * in);
   inline void SetLRS(Double_t * in) { fLRS = in; }
   inline void SetLRB(Double_t * in) { fLRB = in; }
   inline void SetBtot(Double_t in) { fBtot = in; }
   inline void SetStot(Double_t in) { fStot = in; }
   inline void SetDtot(Int_t in) { fDtot = in; }
   inline Double_t GetStatistic() const { return -2 * (fTSD - fStot); } 
   void Draw(const Option_t *option="");
   Double_t GetExpectedStatistic_b(Int_t sigma = 0) const;
   Double_t GetExpectedStatistic_sb(Int_t sigma = 0) const;
   Double_t CLb(bool use_sMC = kFALSE) const;
   Double_t CLsb(bool use_sMC = kFALSE) const;
   Double_t CLs(bool use_sMC = kFALSE) const;
   Double_t GetExpectedCLb_sb(Int_t sigma = 0) const;
   Double_t GetExpectedCLb_b(Int_t sigma = 0) const;
   Double_t GetExpectedCLsb_b(Int_t sigma = 0) const;
   Double_t GetExpectedCLb_b_sigcont(Double_t IntLim, Int_t sigma = 0) const;
   Double_t GetExpectedCLsb_b_sigcont(Double_t IntLim, Int_t sigma = 0) const;
   inline Double_t GetExpectedCLs_b(Int_t sigma = 0) const { return (GetExpectedCLsb_b(sigma) / GetExpectedCLb_b(sigma)); } 
   inline Double_t GetExpectedCLs_b_sigcont(Double_t IntLim, Int_t sigma = 0) const { return (GetExpectedCLsb_b_sigcont(IntLim, sigma) / GetExpectedCLb_b_sigcont(IntLim, sigma)); } 
   Double_t GetAverageCLs() const;
   Double_t GetAverageCLsb() const;
   Double_t Get3sProbability() const;
   Double_t Get5sProbability() const;
   inline Int_t GetDtot() const { return fDtot; } 
   inline Double_t GetStot() const { return fStot; } 
   inline Double_t GetBtot() const { return fBtot; } 
   inline Double_t * GetTestStatistic_SB(){ return fTSS; }
   inline Double_t * GetTestStatistic_B(){  return fTSB; }
   inline Double_t * GetLikelihoodRatio_SB(){ return fLRS; }
   inline Double_t * GetLikelihoodRatio_B(){  return fLRB; }
   inline void SetSig(Double_t * sig=0){fSig=sig;};
   inline void SetBgd(Double_t * bgd=0){fBgd=bgd;};
   inline Double_t * GetSig(){return fSig;};
   inline Double_t * GetBgd(){return fBgd;};

 private:
   // data members used for the limits calculation
   Int_t      fNNMC;
   Int_t      fDtot;
   Double_t   fStot;
   Double_t   fBtot;
   Double_t   fTSD;
   Double_t   fNMC;
   Double_t   fMCL3S;
   Double_t   fMCL5S;
   Double_t  *fTSB;              //[fNNMC] TestStatistic_B 
   Double_t  *fTSS;              //[fNNMC] TestStatistic_SB
   Double_t  *fLRS;              //[fNNMC] LikelihoodRatio_SB
   Double_t  *fLRB;              //[fNNMC] LikelihoodRatio_B
   Int_t     *fISS;              //[fNNMC]
   Int_t     *fISB;              //[fNNMC]
   // cumulative probabilities for defining the bands on plots
   static const Double_t fgMCLM2S;
   static const Double_t fgMCLM1S;
   static const Double_t fgMCLMED;
   static const Double_t fgMCLP1S;
   static const Double_t fgMCLP2S;
   static const Double_t fgMCL3S1S;
   static const Double_t fgMCL5S1S;
   static const Double_t fgMCL3S2S;
   static const Double_t fgMCL5S2S;

   //The random sig and bgd events within uncertainties:
   Double_t  *fSig;
   Double_t  *fBgd;

   ClassDef(TConfidenceLevel, 1)	// output for TLimit functions
};

#endif

