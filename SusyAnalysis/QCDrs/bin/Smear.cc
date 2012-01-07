#include "Smear.h"
#include "JetResolution.h"

#include <iostream>
#include <algorithm>

struct Jet{
  Jet(float Pt,float Px,float Py,float Pz,float E,float Phi,float Eta):
      Pt(Pt),Px(Px),Py(Py),Pz(Pz),E(E),Phi(Phi),Eta(Eta){}
  bool operator<(const Jet& o) const {return Pt<o.Pt;}    
  float Pt,Px,Py,Pz,E,Phi,Eta;
};


void Smear( const Event*evt, Event* rs, JetResolution * JetRes )
{
  // In case an event has a weight > 1, e.g. because of a trigger prescale, we need to make sure that
  // it will be sampled (int)weight times. Alternatively we could only sample it once and give the result
  // a high weight which would result in larger statistical uncertainties. 
  int Ntries = 1;
  double w = rs->EvtWgt;
  if ( w > 1) {
     Ntries = (int)w;
     w = rs->EvtWgt / Ntries;
  }
  
  //sample the event evt 'Ntries' times:
  std::vector<Jet> smeared;
  for (int j = 1; j <= Ntries; ++j) {
     int ith_jet = 0;
     for (int i=0; i<evt->NrecoJet; ++i) {
        smeared.push_back(Jet( 
	       rs->recoJetPt[i] * JetRes->GetRandom( rs->recoJetPt[i], rs->recoJetEta[i], ith_jet++ ),
	       rs->recoJetPx[i],rs->recoJetPy[i],rs->recoJetPz[i],
	       rs->recoJetE[i],rs->recoJetPhi[i],rs->recoJetEta[i]));
	
     }
     //sort jet in pT:
     std::sort(smeared.begin(), smeared.end());

     for (std::vector<Jet>::const_iterator jet=smeared.begin(); jet!=smeared.end(); ++jet){
        rs->CopyEvent( *evt );
	rs->recoJetPt[jet-smeared.begin()] = jet->Pt;
	rs->recoJetPx[jet-smeared.begin()] = jet->Px;
	rs->recoJetPy[jet-smeared.begin()] = jet->Py;
	rs->recoJetPz[jet-smeared.begin()] = jet->Pz;
	rs->recoJetE[ jet-smeared.begin()] = jet->E;
	rs->recoJetPhi[jet-smeared.begin()] = jet->Phi;
	rs->recoJetEta[jet-smeared.begin()] = jet->Eta;
     }	
  }

}

void Smear( const std::vector<Event*>& evts, std::vector<Event*>& rs_events )
{
  int Sample_N_times = 100;
  JetResolution * JetRes = new JetResolution();
  std::cerr<<"...smearing: ";
  for (std::vector<Event*>::const_iterator it=evts.begin(); it!=evts.end(); ++it){
    for (int i_th=0; i_th<Sample_N_times; ++i_th) {
      Event * rs_event = new Event;
      Smear( *it, rs_event, JetRes );
      rs_event->EvtWgt /= 100.;
      rs_events.push_back( rs_event );
      if ((it-evts.begin())%(evts.size()/10)==0&&i_th==0) std::cerr<<"->"<<(it-evts.begin())/10<<"%";
    }
  }
  delete JetRes;
  
  std::cout << "\nCreated "<<rs_events.size()<<" smeared events from "<<evts.size()
            << " seeds sampling each event "<<Sample_N_times<<" times."<<std::endl;
}
