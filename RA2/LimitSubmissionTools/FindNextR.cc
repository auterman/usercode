//Christian Autermann, U Hamburg/LPC, 2012

//system
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdlib.h>

#include "ConfigFile.h"

struct point{
  point(double r,double cls):r(r),cls(cls){}
  double r;
  double cls;
};
bool operator<(const point& l, const point& r){return (l.cls!=r.cls?l.cls<r.cls:l.r>r.r);}

double BiSection(const std::vector<point>& p)
{
  std::vector<point>::const_iterator it=p.begin();
  const point * old = 0;
  for (;it!=p.end();++it) {
    //std::cout<<"r="<<it->r<<", CLs="<<it->cls<<std::endl;
    if       (old  && it->cls < 0.05 && old->cls >= 0.05) return old->r + fabs(it->r - old->r)/2.;
    else  if (old  && it->cls > 0.05 && old->cls <= 0.05) return it->r  + fabs(old->r - it->r)/2.;
    else  if (!old && it->cls > 0.05) return it->r*2.;
    old = &(*it);
  }
  return (old->r==0?0:old->r/2);
}

int main(int argc, char* argv[]) {
  using namespace std;
  if (argc!=2) exit(1);
  string file=argv[1];
  ConfigFile config(file);
  vector<double> R   = bag_of<double>(config.read<string>("R"));  
  vector<double> CLs = bag_of<double>(config.read<string>("CLs"));  
  if (R.size()!=CLs.size()) exit(2);
  vector<point> p;
  for (unsigned i=0;i<R.size();++i)
    p.push_back( point(R[i], CLs[i]) );
  std::sort(p.begin(),p.end());
  cout << BiSection( p ) << endl;
  return 0;
}
