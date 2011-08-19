#ifndef THELIMITS_H
#define THELIMITS_H

#include <vector>
#include <string>
#include <iostream>

#include "SusyScan.h"
#include "GeneratorMasses.h"

class TH2;


class TheLimits{
 public:
  TheLimits(){}

  void Fill(int argc, char** argv);
  void Fill(const std::string filelist);

  void add(SusyScan*s){_scan.push_back(s);}
  void FillGeneratorMasses(std::string file);

  void match();

  template<class T>
  void ExpandGrid(double(*x)(const T*), double(*y)(const T*));
  int GetScanSize(){return _scan.size();};
  std::vector<SusyScan*> * GetScan(){return &_scan;}
  std::vector<GeneratorMasses*> * GetGeneratorMasses(){return &_masses;}
  
  void OverwriteLimits(std::string flag);
  
 private:
  std::vector<SusyScan*> _scan; 
  std::vector<GeneratorMasses*> _masses;
};

template<class T>
void TheLimits::ExpandGrid(double(*x)(const T*), double(*y)(const T*) )
{
  std::vector<T*> new_grid;
  for (typename std::vector<T*>::iterator it=_scan.begin(); it!=_scan.end(); ++it){
    double dx=9999, dy=9999;
    typename std::vector<T*>::iterator next_x=_scan.end(), next_y=_scan.end();
    //x
    for (typename std::vector<T*>::iterator nx=_scan.begin(); nx!=_scan.end(); ++nx)
      if ( x(*it) < x(*nx) && fabs( x(*it)-x(*nx))<dx && y(*it) == y(*nx)) {
        dx=fabs(x(*it)-x(*nx));
	next_x=nx;
      }
    //y  
    for (typename std::vector<T*>::iterator ny=_scan.begin(); ny!=_scan.end(); ++ny)
      if ( y(*it) < y(*ny) && fabs( y(*it) - y(*ny))<dy && x(*it) == x(*ny)) {
        dy=fabs(y(*it)-y(*ny) );
	next_y=ny;
      }
    if (next_y!=_scan.end()) {  
	T*  ny = new T((**it + **next_y) * 0.5);
	new_grid.push_back( ny );

//	std::cout
//	<< "l<>x:"<<x(*it)<< ",y:"<<y(*it)<<", sigma"<<(*it)->NLOXsection
//	<< " <>x:"<<x(ny) << ",y:"<<y(ny)<<", sigma="<<ny->NLOXsection
//	<< " r<>x:"<<x(*next_y)<< ",y:"<<y(*next_y)<<", sigma="<<(*next_y)->NLOXsection
//	<<std::endl;
	
    }
    if (next_x!=_scan.end()) {  
	T*  nx = new T((**it + **next_x) * 0.5);
	new_grid.push_back( nx );
    }
    if (next_x!=_scan.end() && next_y!=_scan.end()) {  
	T*  nxy = new T((**next_x + **next_y) * 0.5);
	new_grid.push_back( nxy );
    }  
      
  } 
  _scan.insert(_scan.end(), new_grid.begin(), new_grid.end());

}

#endif
