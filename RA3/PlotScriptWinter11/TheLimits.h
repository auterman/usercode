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
  //Fills SusyScan with the 'better' point from each file
  //where 'better' means better expected limit
  void FillBestExpectedLimit(const std::string filelist,const std::string filelist2);
  void add(SusyScan*s){_scan.push_back(s);}
  void FillGeneratorMasses(std::string file);

  void match();

  void FillEmptyPointsByInterpolationInMsqMgl();
  template<class T>
  void FillEmptyPointsByInterpolation(double(*x)(const T*), double(*y)(const T*));
  template<class T>
  void FillEmptyPointsNeutralinoScan(double(*x)(const T*), double(*y)(const T*));
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

template<class T>
void TheLimits::FillEmptyPointsByInterpolation(double(*x)(const T*),
double(*y)(const T*))
{
  std::vector<SusyScan*> newpoints;
  //first find out where to expect points
  std::cout<< "start: TheLimits::FillEmptyPointsByInterpolation()"
<<std::endl;
  double gridy=9999, miny=9999, maxy=0, gridx=9999, minx=9999, maxx=0;
  for (std::vector<SusyScan*>::const_iterator it=_scan.begin();
it!=_scan.end(); ++it){
    if (x(*it)<minx) minx=x(*it);
    if (x(*it)>maxx) maxx=x(*it);
    if (y(*it)<miny) miny=y(*it);
    if (y(*it)>maxy) maxy=y(*it);
    for (std::vector<SusyScan*>::const_iterator zt=it; zt!=_scan.end();
++zt){
      if (x(*it)==x(*zt) ) continue;
      if ( fabs(x(*it) - x(*zt)) < gridx &&
           y(*it)==y(*zt)) gridx = fabs(x(*it) - x(*zt));
      if ( fabs(y(*it) - y(*zt) &&
           x(*it)==x(*zt)) < gridy ) gridy = fabs(y(*it) - y(*zt));
    }
  }
  //Now, interpolate
  if(gridx==0)gridx=80;
  if(gridy==0)gridy=80;
  std::cout<<minx<<"  "<<maxx<<"  "<<gridx<<"  "<<miny<<"  "<<maxy<<" "<<gridy<<"  "<<std::endl;

  for (std::vector<SusyScan*>::const_iterator it=_scan.begin();
it!=_scan.end(); ++it){
     //find next right neighbor in x for it:
    //if (!(*it)) continue;
    std::vector<SusyScan*>::const_iterator nextx=_scan.end(),
nexty=_scan.end();
    double dx=9999, dy=9999;
    for (std::vector<SusyScan*>::const_iterator zt=_scan.begin();
zt!=_scan.end(); ++zt){
      //if (!(*zt)) continue;
      if (fabs(x(*it)-x(*zt))<0.9 || fabs(y(*it)-y(*zt))>0.9 ) continue;
      if ( fabs(x(*it) - x(*zt)) < dx && x(*it) < x(*zt)) {
        dx = fabs(x(*it) - x(*zt));
	      nextx = zt;
      }
      if (dx==gridx) break;
    }
    //interpolate in x:
    if (dx!=gridx && nextx!=_scan.end()){
        //std::cout << "m0 = " <<x(*it)  << ", m12="<< y(*it) << std::endl;
       double dist = x(*nextx) - x(*it);
       for (double r=gridx; r<dist; r+=gridx ){
         //std::cout <<x(*it)<<"/"<<y(*it) <<"->"
	 //          <<  x(*it)*r/dist + (x(*nextx) * (1.-r/dist)) << "<-
//"<<x(*nextx)
	 //          <<"/"<< y(*nextx)<<std::endl;
	 newpoints.push_back( new SusyScan( ( (**it * (r/dist)) + (**nextx *
(1.-r/dist)) )));
       }
    }
//    //interpolate in y
//    else{
//
//    }

  }
  _scan.insert(_scan.end(), newpoints.begin(), newpoints.end());
  std::cout<< "done: TheLimits::FillEmptyPointsByInterpolation() added "
<<newpoints.size() <<" new points."<<std::endl;
}

template<class T>
void TheLimits::FillEmptyPointsNeutralinoScan(double(*x)(const T*),
	double(*y)(const T*))
{
  std::vector<SusyScan*> newpoints;
  //first find out where to expect points
  std::cout<< "start: TheLimits::FillEmptyPointsNeutralinoScan()"
<<std::endl;
  double gridy=9999, miny=9999, maxy=0, gridx=9999, minx=9999, maxx=0;
  for (std::vector<SusyScan*>::const_iterator it=_scan.begin();
it!=_scan.end(); ++it){
    if (x(*it)<minx) minx=x(*it);
    if (x(*it)>maxx) maxx=x(*it);
    if (y(*it)<miny) miny=y(*it);
    if (y(*it)>maxy) maxy=y(*it);
    for (std::vector<SusyScan*>::const_iterator zt=it; zt!=_scan.end();
++zt){
      if (x(*it)==x(*zt) ) continue;
      if ( fabs(x(*it) - x(*zt)) < gridx &&
           y(*it)==y(*zt)) gridx = fabs(x(*it) - x(*zt));
      if ( fabs(y(*it) - y(*zt) &&
           x(*it)==x(*zt)) < gridy ) gridy = fabs(y(*it) - y(*zt));
    }
  }
gridy=20;
  std::cout<<minx<<"  "<<maxx<<"  "<<gridx<<"  "<<miny<<"  "<<maxy<<" "<<gridy<<"  "<<std::endl;

  for (int x=minx;x<=maxx;x=x+gridx){
  	for (int y=miny;y<=maxy;y=y+gridy){
  		if(y<x+80){
  			SusyScan * newscan=new SusyScan();
  			(*newscan).Mgluino=y;
  			(*newscan).Mchi1=x;
  			(*newscan).ObsR=9999;
  			(*newscan).ExpR=9999;
  			(*newscan).ExpRM1=9999;
  			(*newscan).ExpRM2=9999;
  			(*newscan).ExpRP1=9999;
  			(*newscan).ExpRP2=9999;
  			(*newscan).NLOXsection=1;
  			newpoints.push_back(newscan);
  		}

  	}
  }



  _scan.insert(_scan.end(), newpoints.begin(), newpoints.end());
  std::cout<< "done: TheLimits::FillEmptyPointsNeutralino() added "
<<newpoints.size() <<" new points."<<std::endl;
}


#endif
