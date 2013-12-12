//create input files for different limit tools
//Christian Autermann, Universität Hamburg

//system
#include <string>
#include <vector>
#include <map>

#include "table.h"
#include "ConfigFile.h"

#define DEBUG 2

std::string ToString(double d, std::string s="");
std::string ToString(int d, std::string s="");
template<typename T> std::string PrintVector(const std::vector<T>& v)
{
   std::stringstream o;
   for (typename std::vector<T>::const_iterator it=v.begin();it!=v.end();++it)
     o << it-v.begin() <<". bin: " << *it << "  ";
   return o.str();     
}


static std::map<const std::string,ConfigFile*> ConfigFiles; 

struct point { //Data structre to store all event yield, uncertainty info etc.
        struct cMSSM{
	  std::map<std::string,std::string> params;
	  double xsec;
	  double xsecNLO;
	} cmssm;
	double lumi;
	
	struct Uncertainties{
	  std::string label;
	  std::string distribution;
	  double up;
	  double dn;
	};
	
	struct Sample{
	  std::string label;
	  double eventyield;
	  double contamination;
	  std::vector<Uncertainties> uncert;
	  double GetFactorialUncertainty(unsigned u){//Used by WriteHiggsInput
	    return (eventyield==0?1. : 1.0 + (fabs(uncert[u].up)+fabs(uncert[u].dn))/(2.*eventyield));
	  }
	  double GetFactorialUncertaintyUp(unsigned u){//Used by WriteHiggsInput
	    return (eventyield==0?1. : 1.0 + (fabs(uncert[u].up))/(eventyield));
	  }
	  double GetFactorialUncertaintyDn(unsigned u){//Used by WriteHiggsInput
	    return (eventyield==0?1. : 1.0 + (fabs(uncert[u].dn))/(eventyield));
	  }
	  double GetTotalUncertainty(){//Used by WriteTable
            double syst=0.;
	    for (std::vector<Uncertainties>::const_iterator it=uncert.begin();it!=uncert.end();++it)
	      syst+= pow( (fabs(it->up)+fabs(it->dn))/2.,2);
	    return sqrt(syst);  
	  }
	};
	
        struct Bin {
	  std::string label;
	  Sample data;
	  Sample signal;
	  std::vector<Sample> backgds;
	};
	
	int nr;
	std::vector<Bin> bins;
	int n_bins;
	std::map<std::string,std::vector<std::string> > uncertainties; //list of uncertainties and what samples are affected
        std::map<std::string, double> syst, systUp, systDn, stat2;
	
	void ScaleLumi(double l, double u);
	void Scale(Sample*, double, double);
};

class points { //class providing methods to reading in the data-cards and to write different kinds of output
public:
        points():print_(true){}
	void Read(ConfigFile& );
	point* Get(int i) {
		for (std::vector<point>::iterator it = p_.begin(); it != p_.end(); ++it)
			if (it->nr == i )
				return &(*it);
		return 0;
	}
	void Add(point p) {
		p_.push_back(p);
	}
	void Reset() {
		p_.clear();
	}
	//void WriteHiggsInput(const std::string dir);
	void WriteHiggsInputStatUncorr(const std::string dir, const std::string prefix);
	void WriteHiggsInputSingleChannel(const std::string dir, const	std::string prefix);
	void WriteTable(std::ostream& os, Table::TableStyle style = Table::Plain, unsigned point=0);
	void TheoryUncertainties(const bool t) {theoryUncertainties_=t;}
        void ScaleLumi(double l, double u);


private:
	std::vector<point> p_;
	bool theoryUncertainties_;
	bool print_;

        void AddData(point& p, ConfigFile * data);
	void AddBackground(point& p, ConfigFile * cfg, const std::string& name);
        void AddBkgTriggerUncertainty(point& p, ConfigFile * cfg, const std::string& name);
	void AddSignal(point& p, ConfigFile * cfg, int i);
	
	double lumiscale_, lumiuncscale_;
} Points;

