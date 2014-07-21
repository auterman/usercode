#ifndef MKINPUTS_H
#define MKINPUTS_H


//system
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <exception>


//LUMI
const double luminosity = 19789.301;
const double luminosityUncertainty = 1.0255; //lumi for 2013, 30.1.2014

const std::string T1lgScanString = "T1lg";
const std::string T1ggScanString = "T1gg";


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
	double gluino;
	double squark;
	double chi; //bino
	double cha; //wino
	double xsec;
	double xsecNLO;
	double lumi;
	int    totalGenerated;

        struct bin {
        //event yields
	double signal;
	double bgd_qcd;
	double bgd_ewk;
	double bgd_fsr;
	double qcd_contamination;
	double ewk_contamination;
	double data;
        //uncertainties
	double u_NLO;      //signal scale
	double u_pdfxsec;  //signal PDF xsec
	double u_pdfacc;   //signal PDF acceptance
	double u_sig_stat; //signal stat
	double u_sig;      //signal jec
	double u_scaleDataMC; //signal photon data MC scale
	double u_lumi;
	double u_qcd;
	double u_qcd_stat;
	double u_ewk;
	double u_ewk_stat;
	double u_fsr;
	double u_fsr_stat;
	//bin limits
	std::string bin_low;
	std::string bin_up;
	};
	std::vector<bin> bins;
	std::string variable;


        //this contains the *total* yields integrated over all channels,
	//used only by the plotting tool and the old single-channel limit code
	double signal;
	double bgd_qcd;
	double bgd_ewk;
	double bgd_fsr;
	double qcd_contamination;
	double ewk_contamination;
	double data;
	double u_NLO;      //signal scale
	double u_NLO_Up;      //signal scale
	double u_NLO_Dn;      //signal scale
	double u_pdfxsec;  //signal PDF xsec
	double u_pdfacc;   //signal PDF acceptance
	double u_sig_stat; //signal stat
	double u_scaleDataMC;
	double u_sig;
	double u_lumi;
	double u_qcd;
	double u_qcd_stat;
	double u_ewk;
	double u_ewk_stat;
	double u_fsr;
	double u_fsr_stat;
};

class points {
public:
	void Do(const std::string& name, const std::string&dat, 
	        const std::string&sig, const std::string&xsec, 
		const std::string&pdf);
	void DoSMS(const std::string& name, const std::string&dat, 
	        const std::string&sig, const std::string&xsec, 
		const std::string&pdf);

	point* Get(double gl, double sq, double chi, double cha=0);
	std::vector<point*> GetGl(double gl);
	std::vector<point*> GetPointsWithSameMass(double gl, double sq);
	
	void Add(point p) { p_.push_back(p); }
	void Reset() { p_.clear();}
	void PrintBins();
	
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
