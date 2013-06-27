// $Id: table.cc,v 1.5 2013/06/12 14:01:01 auterman Exp $

/*** ------------------------------------------------------------------------------------------------------- ***
     Table, a tool to print nicely formatted tables

     Christian Autermann, University Hamburg/LPC, 2012
 *** ------------------------------------------------------------------------------------------------------- ***/

#include "table.h"
#include <iomanip>
#include <sstream>

namespace Table {

std::string TTable::hline(char c,char r) const {

  if (style_==TeX) return "\\\\ \\hline";
  
  std::stringstream ss;
  for (int i=0; i<((int)delimiter_.size())/2; ++i) ss<<c;
  ss<<r;
  for (int i=0; i<((int)delimiter_.size()-1)/2; ++i) ss<<c;
  std::stringstream os;
  std::vector<TColumnBase*>::const_iterator it=table_.begin();
  os.fill('-');
  os<<std::endl<<std::setw((*it)->Width())<<"";
  ++it;
  for (;it!=table_.end();++it){
    os << ss.str() << std::setw((*it)->Width()) << "";
  }
  return os.str();
}


std::ostream& operator<<( std::ostream& os, const TTable& tab )
{
    //determine current style
    if (tab.GetTable()->size()==0) return os;
    std::string delimiter = tab.GetDelimiter();
    bool tex   = (tab.GetStyle()==TeX);
    bool empty = (tab.GetStyle()==Empty);
    //header
    if (tab.GetHeader()!="") os<<tab.GetHeader()<<std::endl;
    if (tex) {
      delimiter = " & ";
      os << "\\begin{tabular}{";
      for (unsigned i=0; i<tab.GetTable()->size(); ++i) os << "r";
      os << "}" <<std::endl;
    }  
    //column-headers
    std::vector<TColumnBase*>::const_iterator it=tab.GetTable()->begin();
    if (!empty && tab.GetLayout()==Default){
      os<<std::setw((*it)->Width())<<(*it)->GetHeader();
      ++it;
      for (;it!=tab.GetTable()->end();++it){
        os <<delimiter<<std::setw((*it)->Width())<<(*it)->GetHeader();
      }
      //draw a line below the header
      os<<tab.hline()<<std::endl;
    } 
    else  //empty style: define at least the column width's
      for (;it!=tab.GetTable()->end();++it) (*it)->Width();

    //the table elements
    for (unsigned r=0;r<tab.GetNRows();++r){
      os << std::left;
      if (tab.GetLayout()==Default) 
        tab.Get(os, 0, r);
      else if (tab.GetLayout()==Transpose) 
        tab.Get(os, r, 0);
      
      for (unsigned c=1; c<tab.GetNColumns(); ++c){
	os<<delimiter;
        if (tab.GetLayout()==Default) 
	  tab.Get(os, c, r);
        else if (tab.GetLayout()==Transpose) 
	  tab.Get(os, r, c);
      }
      if (tex) os<<"\\\\";
      os<<std::endl;
    }

    //caption
    if (!tex && tab.GetCaption()!="")
       os<<tab.GetCaption()<<std::endl;
    if (tex) {
       os<<"\\end{tabular}"<<std::endl;
       os<<"\\label{tab:xyz}"<<std::endl;
       if (tab.GetCaption()!="") os<<"\\caption{"<<tab.GetCaption()<<"}"<<std::endl;
      } 
      
     // os << "rows="<<tab.GetNRows() <<",  cols="<<  tab.GetNColumns() 
     //    << ",  delimiter="<<delimiter
     //	 << std::endl;
    return os;
  }
}

