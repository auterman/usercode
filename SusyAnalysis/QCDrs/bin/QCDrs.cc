#include "QCDrs.h"
#include "Event.h"
#include "Selection.h"
#include "Rebalance.h"
#include "Smear.h"

float HT(const Event * evt){ return evt->HT(); }
float MHT(const Event * evt){ return evt->MHT(); }
float Jet1Pt(const Event * evt){ return evt->recoJetPt[0]; }
float Jet2Pt(const Event * evt){ return evt->recoJetPt[1]; }
float Jet3Pt(const Event * evt){ return evt->recoJetPt[2]; }
float Weight(const Event * evt){ return evt->EvtWgt; }

int main(int argc, char* argv[])
{
  //STL container for all events
  std::vector<Event*> events, rebalanced_events, rs_events;
  
  ReadEvents( "data/QCDcontrol_data.root", events);

  Rebalance( events,  rebalanced_events );
  WriteEvents("data/QCDcontrol_rebalanced.root", rebalanced_events);
  
  //Smear events by jet energy resolution  
//  ReadEvents( "data/QCDcontrol_rebalanced.root", rebalanced_events);
  Smear(rebalanced_events, rs_events);
  Cut(rs_events, Jet1Pt, '<', 70); 
  Cut(rs_events, Jet2Pt, '<', 50); 
  Cut(rs_events, Jet3Pt, '<', 50); 
  WriteEvents("data/QCDcontrol_rs.root", rs_events);

  return 0;
}
