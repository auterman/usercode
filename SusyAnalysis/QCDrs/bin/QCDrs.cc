#include "QCDrs.h"
#include "Event.h"
//#include "Rebalance.h"
#include "Smear.h"


int main(int argc, char* argv[])
{
  //STL container for all events
  std::vector<Event*> events;

  //MakePseudoEvents( "Pseudo QCD events", events);  ///only for testing
  //WriteEvents("QCDpseudo.root", events);
  ReadEvents( "QCDpseudo.root", events);

  //Rebalance events 
  std::vector<Event*> rebalanced_events;
  //Rebalance( events,  rebalanced_events );
  
  //Smear events by jet energy resolution
  
  //Write Events back to file
  WriteEvents("QCDpseudo_rebalanced.root", rebalanced_events);

  return 0;
}
