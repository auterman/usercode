#ifndef MKINPUTS_H
#define MKINPUTS_H


//system
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <exception>


std::string ToString(double d, std::string s=""){
  if (d==0&&s!="") return s;
  std::stringstream ss;
  ss << d;
  return ss.str();
}

template<class T>
T StringTo(const std::string& str)
{
  std::stringstream ss(str);
  T res;
  ss >> res;
  return res;
}

template<class T>
std::string ToString(T t)
{
  std::stringstream ss;
  ss << t;
  return ss.str();
}

struct point {
	//general stuff, independent from selection
	unsigned int nr;
	std::string filename;
	std::map<std::string,double> info;

        struct sample {
	  double yield, contamination;
	  std::map<std::string,double> abs_syst_unc;
	  std::map<std::string,double> abs_stat_unc;

	  double GetTotalAbsSyst2(){double res=0; for (std::map<std::string,double>::iterator it=abs_syst_unc.begin();it!=abs_syst_unc.end();++it) res += it->second; return res;}
	  double GetTotalAbsStat2(){double res=0; for (std::map<std::string,double>::iterator it=abs_stat_unc.begin();it!=abs_stat_unc.end();++it) res += it->second; return res;}
	};
	
	struct bin {
          std::map<std::string,sample> samples;
	};
	std::vector<bin> bins;
        
	std::map<std::string,sample> integrated;
	std::vector<std::string> systematics, statistics;
};

class points {
public:
	void Do(const std::string& name, const std::string&dat, 
	        const std::string&sig, const std::string&xsec, 
		const std::string&pdf);

	point* Get(const std::string& s1, double v1, const std::string& s2="", double v2=0, const std::string& s3="", double v3=0);
	
	void Add(point p) { p_.push_back(p); }
	void Reset() { p_.clear();}
	void Add_WB_PDF(const std::string&);
	void AddPDFs(const std::string&);
	
	void Write(const std::string dir);	
	void WriteSingleBin(const std::string dir);

	std::vector<point> * Get(){return &p_;}
private:
	void PrintGlobal(std::ofstream& os, point&p);
	void PrintGlobalSums(std::ofstream& os, point&p);
	void PrintBin(std::ofstream& os, point&p, unsigned b, const std::string& s);

	std::vector<point> p_;
} Points;





#endif
