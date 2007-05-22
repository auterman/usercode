// -*- C++ -*-
//
// Package:    HHSimpleAnalyzer
// Class:      HHSimpleAnalyzer
// 
/**\class HHSimpleAnalyzer HHSimpleAnalyzer.cc HHAnalysis/HHSimpleAnalyzer/src/HHSimpleAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Christian Autermann
//         Created:  Wed May 16 14:02:29 CEST 2007
// $Id$
//
//


// system include files
#include <memory>
#include <string>
#include <sstream>
#include <ostream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <ios>


// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TFile.h"
#include "TH1F.h"
#include "TH1.h"


#include "HHAnalysis/HHSimpleAnalyzer/interface/ExpressionHisto.h"
//
// class decleration
//

template <typename T1, typename T2>
class HHSimpleAnalyzer : public edm::EDAnalyzer {
   public:
      explicit HHSimpleAnalyzer(const edm::ParameterSet&);
      ~HHSimpleAnalyzer();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
      edm::InputTag src1_, src2_;
      std::string histo_file_;
      std::vector<ExpressionHisto<T2>* > hists;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
template <typename T1, typename T2>
HHSimpleAnalyzer<T1,T2>::HHSimpleAnalyzer(const edm::ParameterSet& iConfig) :
  src1_( iConfig.template getParameter<edm::InputTag>( "src1" ) ),
  src2_( iConfig.template getParameter<edm::InputTag>( "src2" ) ),
  histo_file_( iConfig.template getUntrackedParameter<std::string>( "histo_file" ) )
{
  //now do what ever initialization is needed
  using edm::ParameterSet;
  typedef std::vector<edm::ParameterSet> VPSet;

  //VPSet not yet implemented ???
  VPSet histograms = iConfig.template getParameter<VPSet>("histograms");
  VPSet::const_iterator i = histograms.begin();
  VPSet::const_iterator e = histograms.end();
  edm::Service<TFileService> fs;
  for ( ; i!=e; ++i )
  {
    ExpressionHisto<T2> hist( *i );
    hist.initialize( fs );
    hists.push_back( &hist );
  }
  ofstream hist(histo_file_.c_str(), std::ios::out);
}


template <typename T1, typename T2>
HHSimpleAnalyzer<T1,T2>::~HHSimpleAnalyzer()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
   typename std::vector<ExpressionHisto<T2>* >::iterator it = hists.begin(); 
   for (;it!=hists.end(); ++it)
      (*it)->~ExpressionHisto();
   hists.clear();   
}


//
// member functions
//

// ------------ method called to for each event  ------------
template <typename T1, typename T2> void
HHSimpleAnalyzer<T1,T2>::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;
  Handle<T1> src1;
  //Handle<T2> src2;
  iEvent.getByLabel( src1_, src1 );
  //iEvent.getByLabel( src2_, src2 );

  typename std::vector<ExpressionHisto<T2>* >::iterator it = hists.begin(); 
  for (;it!=hists.end(); ++it)
    for( typename T1::const_iterator elem1=src1->begin(); elem1!=src1->end(); ++elem1 )
      (*it)->fill( *elem1 );
   
//   ESHandle<SetupData> pSetup;
//   iSetup.get<SetupRecord>().get(pSetup);
}


// ------------ method called once each job just before starting event loop  ------------
template <typename T1, typename T2> void 
HHSimpleAnalyzer<T1,T2>::beginJob(const edm::EventSetup&)
{

}

// ------------ method called once each job just after ending the event loop  ------------
template <typename T1, typename T2> void 
HHSimpleAnalyzer<T1,T2>::endJob() {
}

/*

void Test(const edm::EventSetup&)
{
  edm::Service<TFileService> fs;
  TH1F * hist = fs->make<TH1F>("a","b",  10,  -2.,   2.);
}

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
class MagneticField;
void Test2(const edm::EventSetup& setup)
{
  edm::ESHandle<MagneticField> fieldHandle;
  setup.get<IdealMagneticFieldRecord>().get(fieldHandle);   
}
*/

