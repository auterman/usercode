#ifndef QCDSmearProd_H
#define QCDSmearProd_H
// -*- C++ -*-
//
// Package:    QCDSmearProd
// Class:      QCDSmearProd
// 
/**\class QCDSmearProd QCDSmearProd.h RA2/QCDSmearProd/interface/QCDSmearProd.h

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Christian AUTERMANN
//         Created:  Tue Oct 27 11:06:54 CET 2009
// $Id$
//
//


// system include files
#include <memory>
#include <string>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include "TH1F.h"

class QCDSmearProd : public edm::EDProducer {
   public:
      explicit QCDSmearProd(const edm::ParameterSet&);
      ~QCDSmearProd();
   
   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      typedef math::XYZTLorentzVector LorentzVector;
      typedef std::vector<std::string>::const_iterator StrIter;

      edm::InputTag Jet_;
      edm::InputTag Met_;
      std::string   fileName_;
      std::string   plotName_;
      std::string   uncertaintyName_;
      std::vector<std::string> ptbinNames_;
      std::vector<double>      ptbins_;
      std::vector<TH1*>        smearf_;
      int plotindex_;

      std::string GetName(const std::string plot, const std::string uncert="", const std::string ptbin="") const;
      TH1 * GetHist(const std::string file, const std::string hist, const std::string treename="");
      LorentzVector SmearJetP4(const LorentzVector& jet) const;
};

#endif
