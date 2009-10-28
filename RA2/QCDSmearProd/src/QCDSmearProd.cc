// -*- C++ -*-
//
// Package:    QCDSmearProd
// Class:      QCDSmearProd
// 
/**\class QCDSmearProd QCDSmearProd.cc RA2/QCDSmearProd/src/QCDSmearProd.cc

 Description: <one line class summary>
**/

#include "RA2/QCDSmearProd/interface/QCDSmearProd.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/Candidate/interface/CompositeCandidateFwd.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "CommonTools/Utils/interface/PtComparator.h"
#include "TFile.h"

//
// constructors and destructor
//
QCDSmearProd::QCDSmearProd(const edm::ParameterSet& iConfig):
  Jet_(  iConfig.getParameter<edm::InputTag>("Jet") ),
  Met_(  iConfig.getParameter<edm::InputTag>("MET") ),
  fileName_( iConfig.getParameter<std::string>("fileName") ),
  plotName_( iConfig.getParameter<std::string>("plotName") ),
  uncertaintyName_( iConfig.getParameter<std::string>("uncertaintyName") ),
  ptbinNames_( iConfig.getParameter<std::vector<std::string> >("ptBinNames") ),
  ptbins_( iConfig.getParameter<std::vector<double> >("ptBins") ),
  plotindex_(0)
{
   if (ptbins_.size()!=ptbinNames_.size())
     std::cerr << "ERROR: Mismatch of 'ptBins' ("<<ptbins_.size()
               << ") and 'ptBinNames' (" << ptbinNames_.size()<< ") size!" << std::endl;
   
   //Read Smearing function histograms from external file
   for (StrIter it=ptbinNames_.begin(); it!=ptbinNames_.end(); ++it) 
     smearf_.push_back( GetHist(fileName_, GetName(plotName_,uncertaintyName_,*it)) );

   //register your products
   produces<reco::CompositeCandidateCollection >( "SmearedJet"+uncertaintyName_ );
   produces<std::vector<reco::MET> >( "SmearedMET"+uncertaintyName_ );
}


QCDSmearProd::~QCDSmearProd()
{
  //for(std::vector<TH1*>::iterator it=smearf_.begin();it!=smearf_.end();++it)
  // delete *it;
  smearf_.clear();
  ptbins_.clear();
}


//
// member functions
//
std::string QCDSmearProd::GetName(const std::string plot, const std::string uncert, const std::string ptbin) const
{  
  std::string result(plot);
  if (uncert!=""&&uncert!=" ") result+="_"+uncert;
  if (ptbin !=""&&ptbin !=" ") result+="_"+ptbin;
  return result;
}

///Read smearing function from file
TH1 * QCDSmearProd::GetHist(const std::string file, const std::string hist, const std::string treename)
{
   using namespace std;
   TH1F * result;
   
   stringstream ss;
   ss << hist<< "_"<<plotindex_++;
   string name = ss.str(); //root can't handle two hists with same names
//For debugging: if not all background are available, create an empty hist:
if (file=="") {
  result = new TH1F(name.c_str(), hist.c_str(), 100, 0.0, 500.0);
  result->FillRandom("gaus", 10000);
  return result;
}//can delete this later

   //open root file
   TFile f( file.c_str() );
   if (f.IsZombie()) {
       cerr << "Error opening file '" << file << "'"<< endl;
   }
   
   //open requested histogram 'hist', in directory 'treename'
   string dummy_name = hist;
   if (treename!="") dummy_name = treename+"/"+hist;
   result = (TH1F*)f.Get( dummy_name.c_str() );
   if (!result) {
     cerr << "Unable to read histogram '" <<dummy_name<< "' from file '"<<file<< "'!"<<endl;
   }
   result->SetTitle( hist.c_str() );
   result->SetDirectory(0);
   
   return result;
}

// ------------ do the actual jet pt smearing      ------------
QCDSmearProd::LorentzVector 
QCDSmearProd::SmearJetP4(const LorentzVector& jet) const
{
  //first find correct smearing fuction:
  unsigned i=0;
  for (; i<ptbins_.size(); ++i)
    if ( jet.pt()< ptbins_[i]) break;
  if (i==ptbins_.size()) return jet;    

  //Get randomnumber from hist smearf_[i]
  return smearf_[i]->GetRandom() * jet;
}

// ------------ method called to produce the data  ------------
void
QCDSmearProd::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   Handle<edm::View<reco::Jet> > jets;
   iEvent.getByLabel(Jet_,jets);

   Handle<edm::View<reco::MET> > met;
   iEvent.getByLabel(Met_,met);

   // loop over jets
   std::vector<reco::CompositeCandidate> * sJets = new std::vector<reco::CompositeCandidate>(); 
   std::vector<reco::MET> * sMet = new std::vector<reco::MET>(); 
   LorentzVector newMETP4( met->at(0).p4() );
   
   for (edm::View<reco::Jet>::const_iterator itJet=jets->begin(); itJet!=jets->end(); itJet++) 
   {
     LorentzVector newJetP4( SmearJetP4( itJet->p4() ) );
     reco::CompositeCandidate jet( (*itJet).charge(), newJetP4, (*itJet).vertex() );
     sJets->push_back( jet );
     newMETP4 += (*itJet).p4() - newJetP4;
   }
   sMet->push_back( reco::MET( newMETP4, met->at(0).vertex()) );

   // sort smeared jets in Et
   GreaterByPt<reco::CompositeCandidate> ptComparator_;
   std::sort(sJets->begin(), sJets->end(), ptComparator_ );

   // put smeared collections into the Event
   std::auto_ptr<std::vector<reco::CompositeCandidate> > smearedJets( sJets );
   iEvent.put(smearedJets, "SmearedJet"+uncertaintyName_);

   std::auto_ptr<std::vector<reco::MET> > smearedMET( sMet );
   iEvent.put(smearedMET, "SmearedMET"+uncertaintyName_);
}

// ------------ method called once each job just before starting event loop  ------------
void 
QCDSmearProd::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
QCDSmearProd::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(QCDSmearProd);
