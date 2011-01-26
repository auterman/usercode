
#include "table.h"

std::ostream& operator<<( std::ostream& os, const TTable& tab )
{
  if (tab.GetTable()->size()==0) return os;
  std::string delimiter = tab.GetDelimiter();
  bool tex = (tab.GetStyle()==TTable::TeX);
  if (tab.GetHeader()!="") os<<tab.GetHeader()<<std::endl;
  if (tex) {
    delimiter = " & ";
    os << "\\begin{tabular}{";
    for (unsigned i=0; i<tab.GetTable()->size(); ++i) os << "c";
    os << "}" <<std::endl;
  }  
  
  std::vector<TColumnBase*>::const_iterator it=tab.GetTable()->begin();
  os<<std::setw((*it)->Width())<<(*it)->GetHeader();
  ++it;
  for (;it!=tab.GetTable()->end();++it){
    os <<delimiter<<std::setw((*it)->Width())<<(*it)->GetHeader();
  }
  if (!tex) {
    it=tab.GetTable()->begin(); 
    os.fill('-');
    os<<std::endl<<std::setw((*it)->Width())<<"";
    ++it;
    for (;it!=tab.GetTable()->end();++it){
      os << "+" << std::setw((*it)->Width()) << "";
    }
    os.fill(' ');
  } else
    os<<"\\\\ \\hline";
  os<<std::endl;
  for (unsigned l=0; l<tab.Length(); ++l){
    it=tab.GetTable()->begin();
    os<<(*it++)->Str(l);
    for (;it!=tab.GetTable()->end();++it){
      os <<delimiter<<(*it)->Str(l);
    }
    if (tex) os<<"\\\\";
    os<<std::endl;
  }
  if (!tex && tab.GetCaption()!="")
     os<<tab.GetCaption()<<std::endl;
  if (tex) {
     os<<"\\label{tab:xyz}"<<std::endl;
     if (tab.GetCaption()!="") os<<"\\caption{"<<tab.GetCaption()<<"}"<<std::endl;
     os<<"\\end{tabular}"<<std::endl;
    }  
  return os;
}

