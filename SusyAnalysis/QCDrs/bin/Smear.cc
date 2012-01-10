#include "Smear.h"
#include "JetResolution.h"

#include <iostream>
#include <algorithm>

#include "TRandom.h"

struct Jet{
  Jet(float Pt,float Px,float Py,float Pz,float E,float Phi,float Eta):
      Pt(Pt),Px(Px),Py(Py),Pz(Pz),E(E),Phi(Phi),Eta(Eta){}
  bool operator<(const Jet& o) const {return Pt>o.Pt;}    
  float Pt,Px,Py,Pz,E,Phi,Eta;
};


void Smear( const Event*evt, Event* rs, JetResolution * JetRes )
{
  // In case an event has a weight > 1, e.g. because of a trigger prescale, we need to make sure that
  // it will be sampled (int)weight times. Alternatively we could only sample it once and give the result
  // a high weight which would result in larger statistical uncertainties. 
  int Ntries = 1;
  float w = evt->EvtWgt;
  if ( w > 1) {
     Ntries = (int)w;
     w = evt->EvtWgt / Ntries;
  }
  
  //sample the event evt 'Ntries' times:
  for (int j = 1; j <= Ntries; ++j) {
     std::vector<Jet> smeared;
     int ith_jet = 0;
     for (int i=0; i<evt->NrecoJet; ++i) {
        if (evt->recoJetPt[i] > 30. ) { //Smear only jets above 30.
          float rand = JetRes->GetRandom( evt->recoJetPt[i], evt->recoJetEta[i], ith_jet++ );
          smeared.push_back(Jet( 
	         evt->recoJetPt[i]*rand,
	         evt->recoJetPx[i]*rand,evt->recoJetPy[i]*rand,evt->recoJetPz[i]*rand,
	         evt->recoJetE[i]*rand,evt->recoJetPhi[i],evt->recoJetEta[i]));
	} else {
          smeared.push_back(Jet( 
	         evt->recoJetPt[i],
	         evt->recoJetPx[i],evt->recoJetPy[i],evt->recoJetPz[i],
	         evt->recoJetE[i],evt->recoJetPhi[i],evt->recoJetEta[i]));
	}
     }
     //sort jet in pT:
     std::sort(smeared.begin(), smeared.end());

     rs->CopyEvent( *evt );
     rs->EvtWgt = w;
     for (std::vector<Jet>::const_iterator jet=smeared.begin(); jet!=smeared.end(); ++jet){
        if ( jet-smeared.begin() >= rs->NrecoJet ) break;
//std::cout <<"Jet "<<jet-smeared.begin()<<", old-Pt="<<rs->recoJetPt[jet-smeared.begin()]
//          <<", new-Pt="<<jet->Pt<<std::endl;
	rs->recoJetPt[jet-smeared.begin()] = jet->Pt;
	rs->recoJetPx[jet-smeared.begin()] = jet->Px;
	rs->recoJetPy[jet-smeared.begin()] = jet->Py;
	rs->recoJetPz[jet-smeared.begin()] = jet->Pz;
	rs->recoJetE[ jet-smeared.begin()] = jet->E;
	rs->recoJetPhi[jet-smeared.begin()] = jet->Phi;
	rs->recoJetEta[jet-smeared.begin()] = jet->Eta;
     }	
     smeared.clear();
  }
}

void Smear( const std::vector<Event*>& evts, std::vector<Event*>& rs_events )
{
  int Sample_N_times = 10;
  gRandom->SetSeed(0);
  std::cout<<"...reading jet resolutions"<<std::endl;
  JetResolution * JetRes = new JetResolution();
  std::cerr<<"...smearing: ";
  for (std::vector<Event*>::const_iterator it=evts.begin(); it!=evts.end(); ++it){
    for (int i_th=0; i_th<Sample_N_times; ++i_th) {
      Event * rs_event = new Event;
      Smear( *it, rs_event, JetRes );
      rs_event->EvtWgt /= (float)Sample_N_times;
      rs_events.push_back( rs_event );
    }
    if (int(it-evts.begin())%(evts.size()/10)==0) std::cerr<<"->"<<int(it-evts.begin())/10<<"%";
  }
  delete JetRes;
  
  std::cout << "\nCreated "<<rs_events.size()<<" smeared events from "<<evts.size()
            << " seeds sampling each event "<<Sample_N_times<<" times."<<std::endl;
}
