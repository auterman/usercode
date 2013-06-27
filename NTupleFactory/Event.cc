// $Id: Event.cc,v 1.8 2013/06/11 11:34:55 auterman Exp $

/*** ------------------------------------------------------------------------------------------------------- ***
     NTupleFactory, a tool to plot final results, using Root Reflex information

     Christian Autermann, RWTH Aachen, May 2013
 *** ------------------------------------------------------------------------------------------------------- ***/


//own includes
#include "Event.h"

Event::~Event()
{
  o.Destruct();
  //for (std::map<const std::string,const void*>::const_iterator it=uservars_.begin();it!=uservars_.end();++it)
  //  delete it->second;
}
      
