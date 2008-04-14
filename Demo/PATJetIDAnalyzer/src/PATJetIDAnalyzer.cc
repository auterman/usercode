// -*- C++ -*-
//
// Package:    PATJetIDAnalyzer
// Class:      PATJetIDAnalyzer
// 
/**\class PATJetIDAnalyzer PATJetIDAnalyzer.cc Demo/PATJetIDAnalyzer/src/PATJetIDAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Christian Autermann
//         Created:  Mon Feb 25 11:33:02 CET 2008
// $Id: PATJetIDAnalyzer.cc,v 1.3 2008/04/14 08:46:50 auterman Exp $
//
//

//system includes
#include <fstream>
#include <iostream>
//framework includes
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
//root includes
#include "TH1F.h"
#include "TH2F.h"
#include "TString.h"
#include "TRandom.h"
//User include files
#include "DataFormats/Math/interface/deltaR.h"
//AOD
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetfwd.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleCandidate.h"
//PAT
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include "PhysicsTools/Utilities/interface/EtComparator.h"
#include "Demo/PATJetIDAnalyzer/interface/NameScheme.h"
#include "Demo/PATJetIDAnalyzer/interface/PATJetIDAnalyzer.h"
#include "TMatrixT.h"

//EM-cells
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "FWCore/Framework/interface/EventSetup.h"


const unsigned PATJetIDAnalyzer::N_Fourier_Bins_2D;
const unsigned PATJetIDAnalyzer::N_Fourier_Bins_1D;
const unsigned PATJetIDAnalyzer::Nf_Fourier_Bins_1D;
//
// constructors and destructor
//
PATJetIDAnalyzer::PATJetIDAnalyzer(const edm::ParameterSet& iConfig) :
  _patJet   ( iConfig.getParameter<edm::InputTag>( "patJet" ) ),
  _patMet   ( iConfig.getParameter<edm::InputTag>( "patMet" ) ),
  _ebrechits( iConfig.getParameter<edm::InputTag>("EBRecHits") ),
  _hist     ( iConfig.getParameter<std::string>( "hist" ) ),
  _jetminpt ( iConfig.getParameter<double>( "MinJetPt" ) ),
  _jetmaxeta(iConfig.getParameter<double>( "MaxJetEta" ) ),
  _simulate_noise(iConfig.getParameter<bool>( "SimulateNoise" ) ),
  _NoiseMean(iConfig.getParameter<double>( "NoiseMean" ) ),
  _NoiseSigma(iConfig.getParameter<double>( "NoiseSigma" ) ),
  _NoiseThreshold(iConfig.getParameter<double>( "NoiseThreshold" ) ),
  _DoNormalization(iConfig.getParameter<bool>( "Normalize" ) ),
  _uniqueplotid(0)
{
   //now do what ever initialization is needed
  random = new TRandom(123455);
}


PATJetIDAnalyzer::~PATJetIDAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

double PATJetIDAnalyzer::norm(int n)
{
  return ( n==0 ? 1 : sqrt(2.) );
}

TH2F * PATJetIDAnalyzer::FDCT(TH2F & k)
{
  TH2F* result= new TH2F(k);
  unsigned int Nx=k.GetNbinsX();
  unsigned int Ny=k.GetNbinsY();
  
  for (unsigned int u=0; u<=Nx; ++u){
    for (unsigned int v=0; v<=Ny; ++v){
      result->SetBinContent(u,v,0.0); 
      for (unsigned int i=0; i<Nx; ++i){
        for (unsigned int j=0; j<Ny; ++j){
          result->SetBinContent(u,v, result->GetBinContent(u,v)+
              norm(u)*norm(v)/(sqrt((double)Nx)*sqrt((double)Ny))*
	      (k.GetBinContent(i,j)-0.)*
              cos( ( (double)i+0.5)*3.14159*(double)u/( (double)Nx) )*
              cos( ( (double)j+0.5)*3.14159*(double)v/( (double)Ny) )
	     ); 
          //cout << "x="<<u<<", y="<<v<<", f="<<result->GetBinContent(u,v)<<endl;
        }  
      }
    }
  }  
  return result;  
}

void PATJetIDAnalyzer::FakeNoise(TH2F & hk, double * k)
{
  unsigned int Nx=hk.GetNbinsX();
  unsigned int Ny=hk.GetNbinsY();
  unsigned int r;
  double noise, absnoise=0., abs=0.;
  double norm = hk.Integral();
  for (unsigned i=0; i<N_Fourier_Bins_1D; ++i)
    abs+=k[i];
  for (unsigned int u=1; u<=Nx; ++u){
    for (unsigned int v=1; v<=Ny; ++v){
       noise = random->Gaus(_NoiseMean,_NoiseSigma);
       if (noise+hk.GetBinContent(u,v)>_NoiseThreshold) {
         hk.SetBinContent(u,v, noise+hk.GetBinContent(u,v) );
	 double phi = ((double)(u-1)-(double)Nx)/2/(double)Nx;
	 double eta = ((double)(v-1)-(double)Ny)/2/(double)Ny;
	 r = (int)(sqrt( (phi*phi+eta*eta)/2. )*(double)(N_Fourier_Bins_1D-1)/0.5);
//cout <<"u="<<u<<", v="<<v<<", phi="<<phi<<", eta="<<eta<<", r="<<r<<", E(r)="<<k[r]
//     <<", E(u,v)="<<hk.GetBinContent(u,v)<<", noise="<<noise<<endl;
         if (r>0 && r<N_Fourier_Bins_1D){ k[r] += noise; absnoise+=noise; }
       }	 
    }
  }
 
  //normalize such, that the energy of the jet ("brightness") is unchanged by noise
  if (_DoNormalization){
    hk.Scale( norm/hk.Integral() );  
    for (unsigned i=0; i<N_Fourier_Bins_1D; ++i)
      k[i]=k[i]*abs/(abs+absnoise);
  }  
}

void PATJetIDAnalyzer::FakeNoise2x2(TH2F & hk, double * k)
{
  unsigned int Nx=hk.GetNbinsX();
  unsigned int Ny=hk.GetNbinsY();
  unsigned int r;
  double noise, absnoise=0., abs=0.;
  double norm = hk.Integral();
  for (unsigned i=0; i<N_Fourier_Bins_1D; ++i)
    abs+=k[i];
  unsigned int u=(int)(random->Rndm()*(double)hk.GetNbinsX())+1;
  unsigned int v=(int)(random->Rndm()*(double)hk.GetNbinsY())+1;
  noise = 15.;
cout << u << ", " << v << ":  old="<<hk.GetBinContent(u,v);
  hk.SetBinContent(u,v, noise+hk.GetBinContent(u,v) );
cout << ";  new="<<hk.GetBinContent(u,v)<<endl;;
  double phi = ((double)(u-1)-(double)Nx)/2/(double)Nx;
  double eta = ((double)(v-1)-(double)Ny)/2/(double)Ny;
  r = (int)(sqrt( (phi*phi+eta*eta)/2. )*(double)(N_Fourier_Bins_1D-1)/0.5);
//cout <<"u="<<u<<", v="<<v<<", phi="<<phi<<", eta="<<eta<<", r="<<r<<", E(r)="<<k[r]
//     <<", E(u,v)="<<hk.GetBinContent(u,v)<<", noise="<<noise<<endl;
  if (r>0 && r<N_Fourier_Bins_1D){ k[r] += noise; absnoise+=noise; }
 
  //normalize such, that the energy of the jet ("brightness") is unchanged by noise
  if (_DoNormalization){
    hk.Scale( norm/hk.Integral() );  
    for (unsigned i=0; i<N_Fourier_Bins_1D; ++i)
      k[i]=k[i]*abs/(abs+absnoise);
  }  
}

double PATJetIDAnalyzer::dphi(double phi1, double phi2)
{
  double result = phi1 - phi2;
  if (result> 3.14159) result-=3.14159;
  if (result<-3.14159) result+=3.14159;
  return result;
}


void PATJetIDAnalyzer::FourierTransformation( const unsigned int i,
                                              const pat::Jet& jet, 
                                              const EBRecHitCollection& EBRecHit)
{
   ///Studies
  double k[N_Fourier_Bins_1D], f[Nf_Fourier_Bins_1D];
  for (unsigned b=0; b<N_Fourier_Bins_1D; ++b){
    k[b]=0.;
//k[b] = cos(2.*3.1415*(double)b/(double)N_Fourier_Bins) +
//       cos(0.2*2.*3.1415*(double)b/(double)N_Fourier_Bins) +
//       cos(10*2.*3.1415*(double)b/(double)N_Fourier_Bins);   
    
  }
  for (unsigned b=0; b<Nf_Fourier_Bins_1D; ++b){
    f[b]=0.;
  }
  char * name = new char[64];
  sprintf(name,"h_%d",++_uniqueplotid);
  TH2F * k2d = new TH2F(name,"",16,-8,7,16,-8,7);
  TH2F * f2d;

  int jtow=0; //, icell=0;
  std::vector <CaloTowerRef> jetTowers = jet.getConstituents();

  for(std::vector<CaloTowerRef>::const_iterator tow = jetTowers.begin();
      tow != jetTowers.end(); ++tow, ++jtow){

    //2D-energy picture of this jet
    k2d->Fill( dphi((*tow)->phi(),jet.phi())/0.0873, 
                        ((*tow)->eta()-jet.eta())/0.087, 
			(*tow)->energy() );

    //1D-energy picture    
    double dR = deltaR(jet,*(*tow));
    if (fabs(dR)>0.5) dR=0.5;
    //_ft_k[i]->Fill( dR, (*tow)->energy() );  
    k[ (int)(((double)N_Fourier_Bins_1D-1.)*dR/0.5) ] += (*tow)->energy();

//no EM cells in my current test data....
/*    
    double eem=0.;
    for (size_t it=0; it<(*tow)->constituentsSize(); ++it) {
      const DetId detid = (*tow)->constituent(it);
      EcalRecHitCollection::const_iterator myRecHit = EBRecHit.find(detid);
      if(myRecHit != EBRecHit.end()) {
	eem +=  myRecHit->energy(); 
	EBDetId det = myRecHit->id();
	
	const CaloCellGeometry* cell=EBgeom->getGeometry( myRecHit->id() );
/
	etowet [icell] = myRecHit->energy()*sin( cell->getPosition().theta());
	etoweta[icell] = cell->getPosition().eta();
	etowphi[icell] = cell->getPosition().phi();
	etowe  [icell] = myRecHit->energy();
	etowid_phi[icell] = det.iphi();
	etowid_eta[icell] = det.ieta();
	etowid [icell] = myRecHit->id().rawId();
	etownum[icell] = icell;
//
        _ft_energy[i]->Fill( deltaPhi( (double)cell->getPosition().phi(),jet.phi()), cell->getPosition().eta()-jet.eta(), myRecHit->energy() );
	++icell;
      }
    }
*/ 
  }


    //Add fake-noise to the jet:
    if(_simulate_noise)
       FakeNoise( *k2d, k);
       //FakeNoise2x2( *k2d, k);
    
    
    //1-D fourier transformation
    for (unsigned b=0; b<N_Fourier_Bins_1D; ++b) {
      _ft_k[i]->SetBinContent(b, k[b] + _ft_k[i]->GetBinContent(b) );
      //before fourier-trafo substract average jet spectrum:
      k[b] -= 167.53*exp( -11.941*0.5*(double)b/(double)N_Fourier_Bins_1D );
      _ft_ksubavg[i]->SetBinContent(b, k[b] + _ft_ksubavg[i]->GetBinContent(b) );
    }
    for (unsigned b=0; b<=Nf_Fourier_Bins_1D; ++b) {
      double fourier=0.;
      for (unsigned j=0; j<N_Fourier_Bins_1D; ++j) {
        fourier += 2./0.5*k[j]*
	           cos( ((double)j)*3.14159*(double)b/
		        (double)N_Fourier_Bins_1D );
      }
      //_ft_f[i]->SetBinContent(b, fourier);
      _ft_f[i]->SetBinContent(b+1, fourier + _ft_f[i]->GetBinContent(b+1) );
    }
 

    //2D-fourier transformation & calculation of observables
    TMatrixT<double> matrix16(16,16);
    TMatrixT<double> matrix8(8,8);
    for (int binx=0; binx<=k2d->GetNbinsX();++binx) {
    for (int biny=0; biny<=k2d->GetNbinsY();++biny) {
       _ft_energy[i]->SetBinContent(binx,biny,_ft_energy[i]->GetBinContent(binx,biny)+ 
                                       k2d->GetBinContent(binx,biny) );
    }}

    std::vector<double> db;
    double sum=0.;
    f2d = FDCT( *k2d);
    for (int binx=0; binx<=f2d->GetNbinsX();++binx) {
    for (int biny=0; biny<=f2d->GetNbinsY();++biny) {
     double bincont = f2d->GetBinContent(binx,biny);
     _ft_frequency[i]->SetBinContent(binx,biny, _ft_frequency[i]->GetBinContent(binx,biny)+ 
                                          bincont );
     if (bincont!=0.) db.push_back( fabs(bincont) );
     sum += fabs(bincont);
       if (binx>0 && binx<16 &&biny>0 && biny<16) 
       matrix16[binx][biny] = f2d->GetBinContent(binx,biny);
       if (binx>3 && binx<12 &&biny>3 && biny<12) {
         matrix8[binx-4][biny-4] = f2d->GetBinContent(binx,biny);
//cout <<"(" <<binx << "::"<<biny<< ") = " <<k2d->GetBinContent(bin)<<endl;
       }
    }}
    std::sort(db.begin(), db.end());
    double ig=0., F10=0., Fs10=0.;
    int n10=-1, n30=-1, n60=-1, n90=-1, n95=-1, n99=-1;
    for (unsigned j=0; j<db.size(); ++j){
      if ( ig>0.1*sum && n10==-1) n10 = j;
      if ( ig>0.3*sum && n30==-1) n30 = j;
      if ( ig>0.6*sum && n60==-1) n60 = j;
      if ( ig>0.9*sum && n90==-1) n90 = j;
      if ( ig>0.95*sum && n95==-1) n95 = j;
      if ( ig>0.99*sum && n99==-1) n99 = j;
      ig += fabs(db[j]);
      if (j<10) Fs10 += db[j];
      if (j>=db.size()-10 && db.size()>10) F10 += db[j];
    }
    double Ilow = f2d->Integral( 1, (int)(f2d->GetNbinsX()/3.),0,(int)(f2d->GetNbinsY()/3.) );
    double Ihi  = f2d->Integral( (int)(f2d->GetNbinsX()*2./3.),f2d->GetNbinsX(), (int)(f2d->GetNbinsY()*2./3.), f2d->GetNbinsY()-1 );
    
    //fill observables from the 2D-fourier-transformation:
    _fto_n99[i]->Fill( n99 );
    _fto_n95[i]->Fill( n95 );
    _fto_n90[i]->Fill( n90 );
    _fto_n60[i]->Fill( n60 );
    _fto_n30[i]->Fill( n30 );
    _fto_n10[i]->Fill( n10 );
    _fto_F10[i]->Fill( F10 );
    _fto_Fs10[i]->Fill( Fs10 );
    if (Ihi!=0.) {
      _fto_LowFovHiF[i]->Fill( Ilow/Ihi );
      _fto_LowFvsHiF[i]->Fill( (Ihi-Ilow)/(Ihi+Ilow) );
    }
    //_fto_det16[i]->Fill( matrix16.Determinant() );
    //_fto_det8[i]->Fill( matrix8.Determinant() );
    
    //clean-up
    delete name;
    delete f2d;
    delete k2d;
}

// ------------ method called to for each event  ------------
void
PATJetIDAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace reco;
  using namespace std;
  using namespace pat;
 
//  edm::ESHandle<CaloGeometry> pG;
//  iSetup.get<IdealGeometryRecord>().get(pG);
//  const CaloGeometry cG = *pG;
//  EBgeom=cG.getSubdetectorGeometry(DetId::Ecal,1);
 
  //EB RecHits
  const EBRecHitCollection *EBRecHit = 0;
  edm::Handle<EBRecHitCollection> EcalRecHitEB;
//  iEvent.getByLabel( _ebrechits, EcalRecHitEB);
//  if( EcalRecHitEB.isValid() ){ 
//    EBRecHit = EcalRecHitEB.product();
//  }
    
  //PAT Jets
  typedef vector<pat::Jet>::const_iterator PatJetCIter;
  edm::Handle<vector<pat::Jet> > PatJets;
  iEvent.getByLabel( _patJet, PatJets );
  GreaterByEt<pat::Jet>  eTComparator_;
  vector<pat::Jet> patjets = *PatJets;
  std::sort(patjets.begin(), patjets.end(), eTComparator_);
  PatJetCIter PatJetBegin = patjets.begin();
  PatJetCIter PatJetEnd   = patjets.end();

  //Jets
  unsigned multiplicity = 0;
  double met=0.0, ht=0.0;
  for (PatJetCIter it=PatJetBegin; it!=PatJetEnd; ++it) {
    if (it->pt() > _jetminpt && fabs( it->eta() )<_jetmaxeta){
      if (multiplicity<_njets ) {
	_pt_jet[     multiplicity]->Fill( it->pt()  );
	_eta_jet[    multiplicity]->Fill( it->eta() );
	_phi_jet[    multiplicity]->Fill( it->phi() );
	_emfrac_jet[ multiplicity]->Fill( it->emEnergyFraction() );
	_hadfrac_jet[multiplicity]->Fill( it->energyFractionHadronic() );
	
	_n60_jet[    multiplicity]->Fill( it->n60() );
	_n90_jet[    multiplicity]->Fill( it->n90() );
	_area_jet[   multiplicity]->Fill( it->towersArea() );
	
	FourierTransformation( multiplicity, *it, *EBRecHit );
      }
      ++multiplicity;
      ht += it->pt();
      met += it->pt();//@@ just to kill warnings....
    }  
  }  
  _jetmult->Fill( multiplicity );
  _ht->Fill( ht );
  if (multiplicity>1)
    _dijet->Fill( sqrt( pow(patjets[0].energy()+patjets[1].energy(),2) - 
                	pow(patjets[0].px()+patjets[1].px(),2) - 
                	pow(patjets[0].py()+patjets[1].py(),2) - 
                	pow(patjets[0].pz()+patjets[1].pz(),2) 
                      ) );



/*
  //PAT MET
  typedef vector<PatMet>::const_iterator PatJetCIter;
  edm::Handle<vector<PatJet> > PatJets;
  iEvent.getByLabel( _patJet, PatJets );
  std::sort( PatJets->begin(), PatJets->end(), PtGreater());
  PatJetCIter PatJetBegin = PatJets->begin();
  PatJetCIter PatJetEnd   = PatJets->end();
  //MET
  edm::Handle<reco::CaloMETCollection> recMet;
  iEvent.getByLabel(_recMet,recMet);

  for ( reco::CaloMETCollection::const_iterator it=recMet->begin(); 
        it!=recMet->end(); ++it) {
    met = it->pt();
    _met->Fill( met );
    _metx->Fill(met*sin(it->phi()));
    _mety->Fill(met*cos(it->phi()));
    break;
  }
  _metmult->Fill( recMet->size() );
  _htmet->Fill( ht+met );
  _metsig->Fill( met / (0.97*sqrt(ht)) );
  if (multiplicity>1 ) {
    _nv->Fill( (met*met-(CaloJets[0].pt()-CaloJets[1].pt())*(CaloJets[0].pt()-CaloJets[1].pt())) / (CaloJets[1].pt()*CaloJets[1].pt())  );
    _nv2->Fill( (met*met-(CaloJets[0].pt()-CaloJets[1].pt())*(CaloJets[0].pt()-CaloJets[1].pt())) / (CaloJets[0].pt()*CaloJets[0].pt()) );
    _dijet->Fill(sqrt(pow(CaloJets[0].energy()+CaloJets[1].energy(),2)
		      -(pow(CaloJets[0].px()+CaloJets[1].px(),2)
			+pow(CaloJets[0].py()+CaloJets[1].py(),2)
			+pow(CaloJets[0].pz()+CaloJets[1].pz(),2) ) ) );
  }  
*/
/*
  //GenJets
  multiplicity = 0;
  met=0.0; 
  ht=0.0;
  for (reco::GenJetCollection::const_iterator it=GenJets.begin();
       it!=GenJets.end(); ++it) {
    if (it->pt() > _jetminpt && fabs( it->eta() )<_jetmaxeta) {
      if (multiplicity<_ngenjets ) {
	_pt_genjet[       multiplicity]->Fill( it->pt()  );
	_eta_genjet[      multiplicity]->Fill( it->eta() );
	_phi_genjet[      multiplicity]->Fill( it->phi() );
	_emfrac_genjet[   multiplicity]->Fill( it->emEnergy() / it->energy() );
	_hadfrac_genjet[  multiplicity]->Fill( it->hadEnergy() / it->energy() );
	_invisible_genjet[multiplicity]->Fill( it->invisibleEnergy() / it->energy() );
	_aux_genjet[      multiplicity]->Fill( it->auxiliaryEnergy() / it->energy() );
      }
      ++multiplicity;
      ht += it->pt();
    }  
  }  
  _genjetmult->Fill( multiplicity );
  
  //GenMET
  edm::Handle<reco::GenMETCollection> genMet;
  iEvent.getByLabel(_genMet,genMet);

  for ( reco::GenMETCollection::const_iterator it=genMet->begin(); 
        it!=genMet->end(); ++it) {
    met = it->pt();
    _genmet->Fill( met );
    _genmetx->Fill(met*sin(it->phi()));
    _genmety->Fill(met*cos(it->phi()));
    break;
  }
  _genmetmult->Fill( recMet->size() );
  _genht->Fill( ht );
  _genhtmet->Fill( ht+met );
  if (multiplicity>1 ) {
    _gendijet->Fill(sqrt(pow(GenJets[0].energy()+GenJets[1].energy(),2)
			 -(pow(GenJets[0].px()+GenJets[1].px(),2)
			   +pow(GenJets[0].py()+GenJets[1].py(),2)
			   +pow(GenJets[0].pz()+GenJets[1].pz(),2) ) ) );
  }  
*/



}


// ------------ method called once each job just before starting event loop  ------------
void 
PATJetIDAnalyzer::beginJob(const edm::EventSetup&)
{
  if( _hist.empty() )
    return;

  edm::Service<TFileService> fs;
  if( !fs ){
    throw edm::Exception( edm::errors::Configuration,
                          "TFile Service is not registered in cfg file" );
  }

  NameScheme hsusy("pat");
  ofstream hist(_hist.c_str(), std::ios::out);
  //CaloJets
  for (unsigned i=0; i<_njets; ++i) {
    _pt_jet[i]      = fs->make<TH1F>(hsusy.name(hist, "ptj",  i ), hsusy.name("PtJet",  i), 100,  0. , 1000.);
    _eta_jet[i]     = fs->make<TH1F>(hsusy.name(hist, "etaj", i ), hsusy.name("EtaJet", i), 100, -5. ,    5.);
    _phi_jet[i]     = fs->make<TH1F>(hsusy.name(hist, "phij", i ), hsusy.name("PhiJet", i),  70, -3.5,    3.5);
    _emfrac_jet[i]  = fs->make<TH1F>(hsusy.name(hist, "emfj", i ), hsusy.name("EmfJet", i),  50,  0. ,    2.);
    _hadfrac_jet[i] = fs->make<TH1F>(hsusy.name(hist, "hadfj",i ), hsusy.name("HadfJet",i),  50,  0. ,    2.);
    _n60_jet[i]     = fs->make<TH1F>(hsusy.name(hist, "n60j", i ), hsusy.name("n60Jet", i),  50,  0. ,   50.);
    _n90_jet[i]     = fs->make<TH1F>(hsusy.name(hist, "n90j", i ), hsusy.name("n90Jet", i),  50,  0. ,   50.);
    _area_jet[i]    = fs->make<TH1F>(hsusy.name(hist, "areaj",i ), hsusy.name("AreaJet",i),  50,  0. ,    5.);

    _ft_energy[i]   = fs->make<TH2F>(hsusy.name(hist, "ft_E_j",i ), hsusy.name("FtEJet",i),  16,-8,8,16,-8,8);
    _ft_frequency[i]= fs->make<TH2F>(hsusy.name(hist, "ft_F_j",i ), hsusy.name("FtFJet",i),  16,-8,8,16,-8,8);
    //_ft_frequency[i]= fs->make<TH1F>(hsusy.name(hist, "ft_F_j",i ), hsusy.name("FtFJet",i),  100,-50,50);
    _ft_k[i]        = fs->make<TH1F>(hsusy.name(hist, "ft_k_j",i ), hsusy.name("FtKJet",i),  N_Fourier_Bins_1D,0.,0.5);
    _ft_f[i]        = fs->make<TH1F>(hsusy.name(hist, "ft_f_j",i ), hsusy.name("FtFJet",i),  Nf_Fourier_Bins_1D+2,0.,N_Fourier_Bins_1D+1);
    _ft_ksubavg[i]  = fs->make<TH1F>(hsusy.name(hist, "ft_ksubavg_j",i ), hsusy.name("FtKsubavgJet",i),  N_Fourier_Bins_1D,0.,0.5);
    
    _noisecontrib[i]= fs->make<TH1F>(hsusy.name(hist, "noisecontrib_j",i ), hsusy.name("noisecontrib",i),  100,0.,20.);
    _fto_n99[i]     = fs->make<TH1I>(hsusy.name(hist, "fto_n99_j",i ), hsusy.name("fto_n99",i),  100,-1,500);
    _fto_n95[i]     = fs->make<TH1I>(hsusy.name(hist, "fto_n95_j",i ), hsusy.name("fto_n95",i),  100,-1,500);
    _fto_n90[i]     = fs->make<TH1I>(hsusy.name(hist, "fto_n90_j",i ), hsusy.name("fto_n90",i),  100,-1,500);
    _fto_n60[i]     = fs->make<TH1I>(hsusy.name(hist, "fto_n60_j",i ), hsusy.name("fto_n60",i),  100,-1,500);
    _fto_n30[i]     = fs->make<TH1I>(hsusy.name(hist, "fto_n30_j",i ), hsusy.name("fto_n30",i),  50,-1,500);
    _fto_n10[i]     = fs->make<TH1I>(hsusy.name(hist, "fto_n10_j",i ), hsusy.name("fto_n10",i),  50,-1,500);
    _fto_F10[i]     = fs->make<TH1F>(hsusy.name(hist, "fto_F10_j",i ), hsusy.name("fto_F10",i),  100,0.,2000.);
    _fto_Fs10[i]    = fs->make<TH1F>(hsusy.name(hist, "fto_Fs10_j",i),hsusy.name("fto_Fs10",i),  100,0.,100.);
    _fto_LowFvsHiF[i]=fs->make<TH1F>(hsusy.name(hist, "fto_LowFvsHiF_j",i ), hsusy.name("LowFvsHiF",i),  100,-5.,5.);
    _fto_LowFovHiF[i]=fs->make<TH1F>(hsusy.name(hist, "fto_LowFovHiF_j",i ), hsusy.name("LowFovHiF",i),  100,-5.,5.);
    _fto_det16[i]      =fs->make<TH1F>(hsusy.name(hist, "fto_Det16_j",i ), hsusy.name("Det16",i),  100,-1.0E08,1.0E08);
    _fto_det8[i]       =fs->make<TH1F>(hsusy.name(hist, "fto_Det8_j",i ), hsusy.name("Det8",i),  100,-1.0E08,1.0E08);

  }
  _jetmult   = fs->make<TH1F>(hsusy.name(hist, "multj" ), hsusy.name("JetMultiplicity"), 21,  -0.5, 20.5);
  _ht        = fs->make<TH1F>(hsusy.name(hist, "ht"    ), hsusy.name("Ht"),     100,    0.0, 2000.);
  _htmet     = fs->make<TH1F>(hsusy.name(hist, "htmet" ), hsusy.name("HtMet"),  100,    0.0, 2000.);
  _dijet     = fs->make<TH1F>(hsusy.name(hist, "dijet" ), hsusy.name("DiJet"),  100,    0.0, 2000.);
  //GenJets
  for (unsigned i=0; i<_ngenjets; ++i) {
    _pt_genjet[i]      = fs->make<TH1F>(hsusy.name(hist, "ptgj",  i ), hsusy.name("PtGenJet",  i), 100,  0. , 1000.);
    _eta_genjet[i]     = fs->make<TH1F>(hsusy.name(hist, "etagj", i ), hsusy.name("EtaGenJet", i), 100, -5. ,    5.);
    _phi_genjet[i]     = fs->make<TH1F>(hsusy.name(hist, "phigj", i ), hsusy.name("PhiGenJet", i),  70, -3.5,    3.5);
    _emfrac_genjet[i]  = fs->make<TH1F>(hsusy.name(hist, "emfgj", i ), hsusy.name("EmfGenJet", i),  50,  0. ,    5.);
    _hadfrac_genjet[i] = fs->make<TH1F>(hsusy.name(hist, "hadfgj",i ), hsusy.name("HadfGenJet",i),  50,  0. ,    5.);
    _invisible_genjet[i]=fs->make<TH1F>(hsusy.name(hist, "invisgj",i), hsusy.name("InvisiblefGenJet", i), 50, 0.,5.);
    _aux_genjet[i]     = fs->make<TH1F>(hsusy.name(hist, "auxgj", i ), hsusy.name("AuxfGenJet", i), 50,  0. ,    5.);
  }
  _genjetmult   = fs->make<TH1F>(hsusy.name(hist, "multgj" ), hsusy.name("GenJetMultiplicity"), 21,  -0.5, 20.5);
  _genht        = fs->make<TH1F>(hsusy.name(hist, "htgj"   ), hsusy.name("GenHt"),     100,    0.0, 2000.);
  _genhtmet     = fs->make<TH1F>(hsusy.name(hist, "htmetgj"), hsusy.name("GenHtMet"),  100,    0.0, 2000.);
  _gendijet     = fs->make<TH1F>(hsusy.name(hist, "dijetgj"), hsusy.name("GenDiJet"),  100,    0.0, 2000.);
  //MET
  _met       = fs->make<TH1F>(hsusy.name(hist, "met"   ), hsusy.name("Met"),    100,     0.0, 1000.);
  _metmult   = fs->make<TH1F>(hsusy.name(hist, "metmult"),hsusy.name("MetMult"), 11,    -0.5, 10.5);
  _metx      = fs->make<TH1F>(hsusy.name(hist, "metx"  ), hsusy.name("MetX"),   100, -1000.0, 1000.);
  _mety      = fs->make<TH1F>(hsusy.name(hist, "mety"  ), hsusy.name("MetY"),   100, -1000.0, 1000.);
  _metsig    = fs->make<TH1F>(hsusy.name(hist, "metsig"), hsusy.name("MetSig"), 100,     0.0, 100.);
  //GenMET
  _genmet       = fs->make<TH1F>(hsusy.name(hist, "metgj"   ), hsusy.name("GenMet"),    100,     0.0, 1000.);
  _genmetmult   = fs->make<TH1F>(hsusy.name(hist, "metmultgj"),hsusy.name("GenMetMult"), 11,    -0.5, 10.5);
  _genmetx      = fs->make<TH1F>(hsusy.name(hist, "metxgj"  ), hsusy.name("GenMetX"),   100, -1000.0, 1000.);
  _genmety      = fs->make<TH1F>(hsusy.name(hist, "metygj"  ), hsusy.name("GenMetY"),   100, -1000.0, 1000.);
  //other
  _nv        = fs->make<TH1F>(hsusy.name(hist, "nv"    ), hsusy.name("NV"),     100,   -5.0,  5.);
  _nv2       = fs->make<TH1F>(hsusy.name(hist, "nv2"   ), hsusy.name("NV2"),    100,   -5.0,  5.);
  //matching
  _GenOnCalo_match = fs->make<TH2F>(hsusy.name(hist,"gencalo"), hsusy.name("GenOnCalo"), 60, 0., 3., 120, -1, 5.);
  _CaloOnGen_match = fs->make<TH2F>(hsusy.name(hist,"calogen"), hsusy.name("CaloOnGen"), 60, 0., 3., 120, -1, 5.);
  _GenVsMatched_pt  = fs->make<TH2F>(hsusy.name(hist,"genmatched_pt"), hsusy.name("GenVsMatched_Pt")  , 100,  0. , 2000. , 100,  0. , 2000.);
  _GenVsMatched_eta = fs->make<TH2F>(hsusy.name(hist,"genmatched_eta"), hsusy.name("GenVsMatched_Eta"), 100, -5. ,    5. , 100, -5. ,    5.);
  _GenVsMatched_phi = fs->make<TH2F>(hsusy.name(hist,"genmatched_phi"), hsusy.name("GenVsMatched_Phi"),  70, -3.5,    3.5,  70, -3.5,    3.5);
  _RecoEff_pt  = fs->make<TH1F>(hsusy.name(hist, "recoeffpt") , hsusy.name("RecoEff_Pt") , 100,  0. , 2000. );
  _RecoEff_eta = fs->make<TH1F>(hsusy.name(hist, "recoeffeta"), hsusy.name("RecoEff_Eta"), 100, -5. ,    5. );
  _RecoEff_phi = fs->make<TH1F>(hsusy.name(hist, "recoeffphi"), hsusy.name("RecoEff_Phi"),  70, -3.5,    3.5);
  //helper histograms
  _RecoTot_pt = new TH1F("recototpt" , "RecoTot_Pt" , 100,  0. , 2000. );
  _RecoTot_eta= new TH1F("recototeta", "RecoTot_Eta", 100, -5. ,    5. );
  _RecoTot_phi= new TH1F("recototphi", "RecoTot_Phi",  70, -3.5,    3.5);
}

// ------------ method called once each job just after ending the event loop  ------------
void 
PATJetIDAnalyzer::endJob() {
  _RecoTot_pt->Sumw2();
  _RecoEff_pt->Sumw2();
  _RecoEff_pt->Divide(_RecoTot_pt);
  _RecoTot_eta->Sumw2();
  _RecoEff_eta->Sumw2();
  _RecoEff_eta->Divide(_RecoTot_eta);
  _RecoTot_phi->Sumw2();
  _RecoEff_phi->Sumw2();
  _RecoEff_phi->Divide(_RecoTot_phi);
}

// ------------ redo matching 
void 
PATJetIDAnalyzer::makeMatchingMaps(edm::Handle<reco::GenJetCollection> GenJets, edm::Handle<reco::CaloJetCollection> CaloJets)
{
  MatchingMapGen.clear();
  MatchingMapJet.clear();

  // Loop over generator Jets
  for(reco::GenJetCollection::const_iterator genjet = GenJets->begin(); genjet!=GenJets->end(); ++genjet) {
    const reco::CaloJet* jetbest=0;
    const reco::CaloJet* jetmatched=0;
    double minDelR= 0.3;
    double minDelE=-1/4.;
    double maxDelE= 3.0;
    double mindE=999.;
    double mindR=999.;
    double delR;
    double delE;
    double E1=(*genjet).energy();
    bool matched = false;
    // Loop over Calo jets
    for(reco::CaloJetCollection::const_iterator calojet = CaloJets->begin(); calojet!=CaloJets->end(); ++calojet){  
      double E2=(*calojet).energy();
      delR=deltaR(*calojet,*genjet);
      delE=(E2-E1)/E1;
      // This is the matching condition
      if(delR<minDelR && delE>minDelE && delE<maxDelE){
	matched=true;
	minDelR=delR;
	jetmatched=&(*calojet);
      }
      if(delR<mindR){
	mindR=delR;
	mindE=delE;
	jetbest=&(*calojet);
      }
    }
    if (matched) {
      MatchingMapGen.insert(make_pair(&(*genjet),&(*jetmatched)));
      _GenVsMatched_pt->Fill((*genjet).pt(),(*jetmatched).pt());
      _GenVsMatched_eta->Fill((*genjet).eta(),(*jetmatched).eta());
      _GenVsMatched_phi->Fill((*genjet).phi(),(*jetmatched).phi());
      _RecoEff_pt->Fill((*genjet).pt());
      _RecoEff_eta->Fill((*genjet).eta());
      _RecoEff_phi->Fill((*genjet).phi());
    }
    _RecoTot_pt->Fill((*genjet).pt());
    _RecoTot_eta->Fill((*genjet).eta());
    _RecoTot_phi->Fill((*genjet).phi());
    _GenOnCalo_match->Fill(mindR,mindE);
  }

  // Loop over Calo Jets
  for(reco::CaloJetCollection::const_iterator calojet = CaloJets->begin(); calojet!=CaloJets->end(); ++calojet) {
    const reco::GenJet* jetbest=0;
    const reco::GenJet* jetmatched=0;
    double minDelR= 0.3;
    double minDelE=-1/4.;
    double maxDelE= 3.0;
    double mindE=999.;
    double mindR=999.;
    double delR;
    double delE;
    double E1=(*calojet).energy();
    bool matched = false;
    // Loop over jets
    for(reco::GenJetCollection::const_iterator genjet = GenJets->begin(); genjet!=GenJets->end(); ++genjet){  
      double E2=(*genjet).energy();
      delR=deltaR(*genjet,*calojet);
      delE=(E2-E1)/E1;
      // This is the matching condition
      if(delR<minDelR && delE>minDelE && delE<maxDelE){
	matched=true;
	minDelR=delR;
	jetmatched=&(*genjet);
      }
      if(delR<mindR){
	mindR=delR;
	mindE=delE;
	jetbest=&(*genjet);
      }
    }
    if (matched) {
      MatchingMapJet.insert(make_pair(&(*calojet),&(*jetmatched)));
    }
    _CaloOnGen_match->Fill(mindR,mindE);
  }

}

//define this as a plug-in
DEFINE_FWK_MODULE(PATJetIDAnalyzer);
