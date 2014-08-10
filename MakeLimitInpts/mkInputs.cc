#include "mkInputs.h"
#include "table.h"
#include "ConfigFile.h"
#include <sys/stat.h>
#include <algorithm>
#include <fstream>

/*
 * COMMENTS on used datasets, scans, etc.
 *
 *
 *
 */

bool MkDir(const std::string& dir)
{
  struct stat sb;
  if (stat(dir.c_str(), &sb))
  {
     std::system( ((std::string)"mkdir "+dir).c_str() );
  }
  return true;
}

point* points::Get(const std::string& s1, double v1, const std::string& s2, double v2, const std::string& s3, double v3) {
   for (std::vector<point>::iterator it = p_.begin(); it != p_.end(); ++it){
    	    if (
		 it->info[s1] == v1 &&
		 (s2=="" || it->info[s2] == v2) &&
		 (s3=="" || it->info[s3] == v3)
    	       )
    		    return &(*it);
    }
    return 0;
}


void points::PrintGlobal(std::ofstream& os, point&p)
{
  os << "# Point = " << p.nr << "\n";
  for (std::map<std::string,double>::const_iterator it=p.info.begin();it!=p.info.end();++it){
    os << "# " << it->first << " = " << it->second << "\n";
  }
}

void PrintSample(std::ofstream& os, const std::string& str,const point::sample& samp, double*bkg=0, double*syst=0, double*stat=0)
{
    os << "# " << str << "  = " << samp.yield << "\n";
    if (bkg) (*bkg) += samp.yield;
    for (std::map<std::string,double>::const_iterator it=samp.abs_syst_unc.begin(); it!=samp.abs_syst_unc.end(); ++it){
      if (syst) (*syst) += it->second * it->second;
      os << "# "<< str << " " << it->first << " = " << it->second << "\n";
    }  
    for (std::map<std::string,double>::const_iterator it=samp.abs_stat_unc.begin(); it!=samp.abs_stat_unc.end(); ++it){
      if (stat) (*stat) += it->second * it->second;
      os << "# "<< str << " " << it->first << " = " << it->second << "\n";
    }  
}

void points::PrintGlobalSums(std::ofstream& os, point&p)
{
  for (std::map<std::string,point::sample>::const_iterator it=p.integrated.begin();it!=p.integrated.end();++it){
    PrintSample(os,it->first,it->second);
  }
}

void points::PrintBin(std::ofstream& os, point&p, unsigned bin, const std::string& str)
{	  
   double bkg=0, bkg_syst2=0, bkg_stat2=0;
   PrintSample(os, "bin"+ToString(bin)+"_signal", p.bins[bin].samples["signal"]);
   for (std::map<std::string,point::sample>::iterator sample=p.bins[bin].samples.begin();sample!=p.bins[bin].samples.end();++sample){
     if (sample->first!="data" && sample->first!="signal")
       PrintSample(os, "bin"+ToString(bin)+"_"+sample->first, sample->second, &bkg, &bkg_syst2, &bkg_stat2);
   }
   os << "# bin"+ToString(bin)+"_background = "<< bkg <<"\n";
   os << "# bin"+ToString(bin)+"_background abs syst = "<< sqrt(bkg_syst2) <<"\n";
   os << "# bin"+ToString(bin)+"_background abs stat = "<< sqrt(bkg_stat2) <<"\n";
   PrintSample(os, "bin"+ToString(bin)+"_data", p.bins[bin].samples["data"]);
}


std::string PrintSystError(point::sample& sample, const std::string& name)
{
  std::map<std::string,double>::iterator it=sample.abs_syst_unc.find(name);
  double unc = (sample.yield?1.+it->second/sample.yield:1.0);
  return (it==sample.abs_syst_unc.end()?"-":ToString(unc));
}

std::string PrintStatError(point::sample& sample, const std::string& name)
{
  std::map<std::string,double>::iterator it=sample.abs_stat_unc.find(name);
  double unc = (sample.yield?1.+it->second/sample.yield:1.0);
  return (it==sample.abs_stat_unc.end()?"-":ToString(unc));
}

void points::Write(const std::string dir) {

 using namespace Table;
 using namespace std;

 MkDir(dir);

 for (vector<point>::iterator point = p_.begin(); point != p_.end(); ++point) {

    ofstream ofile;
    ofile.open( (std::string)(dir+"/"+point->filename+".txt").c_str() );
    
    PrintGlobal( ofile, *point );
    PrintGlobalSums( ofile, *point );
    
    for (int bin=0; bin<(int)point->bins.size(); ++bin)
      PrintBin( ofile, *point, bin, "");
 
    ///some rough by-hand calculation of 'R' to pre-define (and check) the search range:
    ///---
    int n_channels    = (int)point->bins.size();
    int n_backgrounds = (int)point->bins.begin()->samples.size()-2;
    int n_nuisance    = (int)point->statistics.size() * (int)point->bins.size() +
                        (int)point->systematics.size();
 
    //calc tot bkgd & bkgd_unc
    double bkg=0, u2_syst_bkg=0, u2_stat_bkg=0;
    for (std::map<std::string,point::sample>::iterator s=point->integrated.begin(); s!=point->integrated.end();++s ){
        if (s->first!="signal" && s->first!="data") {
	  bkg         += s->second.yield;
	  u2_syst_bkg += s->second.GetTotalAbsSyst2();
	  u2_stat_bkg += s->second.GetTotalAbsStat2();
	}
    }
    ofile << "# background = " << bkg << "\n";
    ofile << "# background abs syst = " << sqrt(u2_syst_bkg) << "\n";
    ofile << "# background abs stat = " << sqrt(u2_stat_bkg) << "\n";

    double R,Rmin=9999999999999;
    for (std::vector<point::bin>::iterator bin=point->bins.begin(); bin!=point->bins.end();++bin){
      double unc2=0;
      for (std::map<std::string,point::sample>::iterator s=bin->samples.begin(); s!=bin->samples.end();++s ){
        if (s->first!="signal" && s->first!="data") {
	  //std::cout << s->first << " stat2 = " << s->second.GetTotalAbsStat2() << ", syst2 = "<<s->second.GetTotalAbsSyst2() << std::endl;
	  unc2 += fabs(s->second.GetTotalAbsSyst2());
	  unc2 += fabs(s->second.GetTotalAbsStat2());
	}
      }

      double s = bin->samples["signal"].yield;
      for (std::map<std::string,double>::iterator u=bin->samples["signal"].abs_syst_unc.begin();u!=bin->samples["signal"].abs_syst_unc.end();++u){
         //std::cout << "signal " << u->first << "  " << u->second <<std::endl;
         unc2 += u->second * u->second;
      }	 
      for (std::map<std::string,double>::iterator u=bin->samples["signal"].abs_stat_unc.begin();u!=bin->samples["signal"].abs_stat_unc.end();++u){
         //std::cout << "signal " << u->first << "  " << u->second <<std::endl;
         unc2 += u->second * u->second;
      }
      if (s) {
        R=2.*sqrt(unc2)/s;
        ofile << "# bin"<< (int)(bin-point->bins.begin()) <<" R_firstguess = " << R << "\n";
	if (R<Rmin) Rmin=R;  
      }
      //std::cout<<"bin"<<bin-point->bins.begin()<<", s="<<s<<", unc="<<sqrt(unc2)<<", R = " <<R<<", R_min = "<<Rmin<<std::endl<<std::endl;
    }
    ofile << "# R_firstguess = " << Rmin << "\n###============================================\n\n";

    ofile << "imax " << setw(2) << n_channels	 << "  number of channels" << endl;
    ofile << "jmax " << setw(2) << n_backgrounds << "  number of backgrounds" << endl;
    ofile << "kmax " << setw(2) << n_nuisance	 << "  number of nuisance parameters (sources of systematic uncertainties)" << endl;
    ofile << "------------" << endl;  

    //observed events in all channels
    TTable observed("## observed events");\
    observed.SetStyle(Empty);
    observed.SetDelimiter("  ");
    observed.AddColumn<string>(""); for (int bin=1; bin<=n_channels; ++bin) observed.AddColumn<std::string>("");
    observed << "bin";              for (int bin=1; bin<=n_channels; ++bin) observed << "bin_"+ToString(bin);
    observed << "observation"; 
    for (int bin=1; bin<=n_channels; ++bin) {
      //stringstream ss;
      //ss << "n" << bin;
      observed << ToString((int)point->bins[bin-1].samples["data"].yield);
    }  
    ofile << observed << "------------\n" << endl;  

    //expected events in all channels for signal and all backgrounds
    TTable exp("## expected events");
    exp.SetStyle(Empty);
    exp.SetDelimiter("  ");
    exp.AddColumn<string>(""); 
    exp.SetMinumumWidth(20,0);//make first column at least 20 chars
      for (int bin=1; bin<=n_channels; ++bin) 
    	for (int sample=0; sample<(int)point->bins[bin-1].samples.size()-1; ++sample) 
    	  exp.AddColumn<string>("");
    exp << "bin"; 
      for (int bin=1; bin<=n_channels; ++bin)
    	for (int sample=0; sample<(int)point->bins[bin-1].samples.size()-1; ++sample) {
 	   stringstream ss;
 	   ss << "bin_"<<bin;
 	   exp << ss.str();
    	}
    exp << "process";
    for (int bin=1; bin<=n_channels; ++bin) {
      exp << "signal";
      for (std::map<std::string,point::sample>::iterator s=point->bins[bin-1].samples.begin(); s!=point->bins[bin-1].samples.end();++s )
        if (s->first!="signal" && s->first!="data") exp << s->first;
    }  
    exp << "process"; 
    for (int bin=1; bin<=n_channels; ++bin) 
    	for (int sample=0; sample<(int)point->bins[bin-1].samples.size()-1; ++sample) {
 	   stringstream ss;
 	   ss << sample;
 	   exp << ss.str();
    	}   
    exp << "rate"; 
    for (int bin=1; bin<=n_channels; ++bin) {
      exp << ToString(point->bins[bin-1].samples["signal"].yield - point->bins[bin-1].samples["signal"].contamination);
      for (std::map<std::string,point::sample>::iterator s=point->bins[bin-1].samples.begin(); s!=point->bins[bin-1].samples.end();++s )
        if (s->first!="signal" && s->first!="data") exp << ToString(s->second.yield);
    }  
    ofile << exp << "------------" << std::endl;  

    TTable sys("");
    sys.SetStyle(Empty);
    sys.SetDelimiter("  ");
    sys.AddColumn<string>(""); 
    sys.SetMinumumWidth(20,0);//make first column at least 20 chars
    for (int bin=1; bin<=n_channels; ++bin) 
      for (int sample=0; sample<(int)point->bins[bin-1].samples.size()-1; ++sample) 
    	sys.AddColumn<string>("");

    for (std::vector<std::string>::iterator syst=point->systematics.begin(); syst!=point->systematics.end(); ++syst){
      sys << *syst+" lnN";
      for (int bin=1; bin<=n_channels; ++bin) {
        sys << PrintSystError(point->bins[bin-1].samples["signal"] , *syst);	
        for (std::map<std::string,point::sample>::iterator s=point->bins[bin-1].samples.begin(); s!=point->bins[bin-1].samples.end();++s )
          if (s->first!="signal" && s->first!="data") 
	    sys << PrintSystError(s->second, *syst);
      }
    }

    for (std::vector<std::string>::iterator syst=point->statistics.begin(); syst!=point->statistics.end(); ++syst){
      for (int sysbin=1; sysbin<=n_channels; ++sysbin) {
        sys << *syst+"_bin"+ToString(sysbin)+" lnN";
        for (int bin=1; bin<=n_channels; ++bin) {
	  if (bin==sysbin) sys << PrintStatError(point->bins[bin-1].samples["signal"], *syst);	
	  else sys << "-";
          for (std::map<std::string,point::sample>::iterator s=point->bins[bin-1].samples.begin(); s!=point->bins[bin-1].samples.end();++s )
            if (s->first!="signal" && s->first!="data") {
	      if (bin==sysbin) sys << PrintStatError(s->second, *syst);
              else sys << "-";
	    }  
         }
      }
    }
    ofile << sys << "------------" << std::endl;  
    ofile.close();
 }
}	

//=====================================================================================================
//=====================================================================================================
//=====================================================================================================
void points::WriteSingleBin(const std::string dir) {
 using namespace Table;
 using namespace std;

 MkDir(dir);

 for (vector<point>::iterator point = p_.begin(); point != p_.end(); ++point) {
    
    int i_bin=0;
    for (std::vector<point::bin>::iterator bin=point->bins.begin();bin!=point->bins.end();++bin,++i_bin)
    {

       ofstream ofile;
       ofile.open(dir+"/"+point->filename+"_bin"+ToString(i_bin)+".txt");

       ofile << "# i_bin = "<<i_bin<<"\n";
       PrintGlobal( ofile, *point );
       PrintGlobalSums( ofile, *point );
       PrintBin( ofile, *point, i_bin, "");

       ///some rough by-hand calculation of 'R' to pre-define (and check) the search range:
       ///---
       int n_channels    = 1;
       int n_backgrounds = (int)bin->samples.size()-2;
       int n_nuisance    = (int)point->statistics.size() +
                           (int)point->systematics.size();

       //calc tot bkgd & bkgd_unc
       double bkg=0, u2_syst_bkg=0, u2_stat_bkg=0;
       for (std::map<std::string,point::sample>::iterator s=bin->samples.begin(); s!=bin->samples.end();++s ){
           if (s->first!="signal" && s->first!="data") {
	     bkg         += s->second.yield;
	     u2_syst_bkg += fabs(s->second.GetTotalAbsSyst2());
	     u2_stat_bkg += fabs(s->second.GetTotalAbsStat2());
	   }
       }
       ofile << "# background = " << bkg << "\n";
       ofile << "# background abs syst = " << sqrt(u2_syst_bkg) << "\n";
       ofile << "# background abs stat = " << sqrt(u2_stat_bkg) << "\n";

       double R=0;
       double unc2=0;
       for (std::map<std::string,point::sample>::iterator s=bin->samples.begin(); s!=bin->samples.end();++s ){
         if (s->first!="signal" && s->first!="data") {
	   unc2 += fabs(s->second.GetTotalAbsSyst2());
	   unc2 += fabs(s->second.GetTotalAbsStat2());
	 }
       }

       double s = bin->samples["signal"].yield;
       for (std::map<std::string,double>::iterator u=bin->samples["signal"].abs_syst_unc.begin();u!=bin->samples["signal"].abs_syst_unc.end();++u)
          unc2 += u->second * u->second;
       for (std::map<std::string,double>::iterator u=bin->samples["signal"].abs_stat_unc.begin();u!=bin->samples["signal"].abs_stat_unc.end();++u)
          unc2 += u->second * u->second;
       if (s)  R=2.*sqrt(unc2)/s;
       ofile << "# R_firstguess = " << R << "\n###============================================\n\n";

       ofile << "imax " << setw(2) << n_channels    << "  number of channels" << endl;
       ofile << "jmax " << setw(2) << n_backgrounds << "  number of backgrounds" << endl;
       ofile << "kmax " << setw(2) << n_nuisance    << "  number of nuisance parameters (sources of systematic uncertainties)" << endl;
       ofile << "------------" << endl;  

       //observed events in all channels
       TTable observed("## observed events");\
       observed.SetStyle(Empty);
       observed.SetDelimiter("  ");
       observed.AddColumn<string>(""); observed.AddColumn<std::string>("");
       observed << "bin";              observed << "bin_"+ToString(i_bin);
       observed << "observation"; 
       observed << ToString(bin->samples["data"].yield);
       ofile << observed << "------------\n" << endl;  

       //expected events in all channels for signal and all backgrounds
       TTable exp("## expected events");
       exp.SetStyle(Empty);
       exp.SetDelimiter("  ");
       exp.AddColumn<string>(""); 
       exp.SetMinumumWidth(20,0);//make first column at least 20 chars
       for (int sample=0; sample<(int)bin->samples.size()-1; ++sample) 
    	     exp.AddColumn<string>("");
       exp << "bin"; 
       for (int sample=0; sample<(int)bin->samples.size()-1; ++sample) {
 	      stringstream ss;
 	      ss << "bin_"<<i_bin;
 	      exp << ss.str();
    	   }
       exp << "process";
       exp << "signal";
       for (std::map<std::string,point::sample>::iterator s=bin->samples.begin(); s!=bin->samples.end();++s )
           if (s->first!="signal" && s->first!="data") exp << s->first;

       exp << "process"; 
       for (int sample=0; sample<(int)bin->samples.size()-1; ++sample) {
 	      stringstream ss;
 	      ss << sample;
 	      exp << ss.str();
       }   
       exp << "rate"; 
       exp << ToString(bin->samples["signal"].yield - bin->samples["signal"].contamination);
       for (std::map<std::string,point::sample>::iterator s=bin->samples.begin(); s!=bin->samples.end();++s )
         if (s->first!="signal" && s->first!="data") exp << ToString(s->second.yield);
       ofile << exp << "------------" << std::endl;  

       TTable sys("");
       sys.SetStyle(Empty);
       sys.SetDelimiter("  ");
       sys.AddColumn<string>(""); 
       sys.SetMinumumWidth(20,0);//make first column at least 20 chars
       for (int sample=0; sample<(int)bin->samples.size()-1; ++sample) 
    	 sys.AddColumn<string>("");

       for (std::vector<std::string>::iterator syst=point->systematics.begin(); syst!=point->systematics.end(); ++syst){
	 sys << *syst+" lnN";
         sys << PrintSystError(bin->samples["signal"] , *syst);	
         for (std::map<std::string,point::sample>::iterator s=bin->samples.begin(); s!=bin->samples.end();++s )
           if (s->first!="signal" && s->first!="data") 
	     sys << PrintSystError(s->second, *syst);
       }

       for (std::vector<std::string>::iterator syst=point->statistics.begin(); syst!=point->statistics.end(); ++syst){
         sys << *syst+"_bin"+ToString(i_bin)+" lnN";
         for (int sysbin=0; sysbin<n_channels; ++sysbin) {
	   if (i_bin==sysbin) sys << PrintStatError(bin->samples["signal"], *syst);	
	   else sys << "-";
           for (std::map<std::string,point::sample>::iterator s=bin->samples.begin(); s!=bin->samples.end();++s )
             if (s->first!="signal" && s->first!="data") {
	       if (i_bin==sysbin) sys << PrintStatError(s->second, *syst);
               else sys << "-";
	     }  
          }
       }
       ofile << sys << "------------" << std::endl;  
       ofile.close();
    }   
 }
}


//// READ /////// -----------------------------------------------------------------------------

double Add(std::map<std::string,double>& m, ConfigFile*f, const std::string& s, const std::string& n="")
{
  return m[(n==""?s:n)]=f->read<double>(s);
}

double Add(std::map<std::string,double>& m, ConfigFile*f, const std::string& s,  const std::string& n, double v)
{
  return m[(n==""?s:n)]=f->read<double>(s,v);
}

std::vector<double>  Get(ConfigFile*f, const std::string& s)
{
  return bag_of<double>(f->read<std::string>(s));
}


void AddYields(point& p, ConfigFile* cfg, const std::string& val, const std::string& sample, int nBins)
{
  std::vector<double> vec = Get(cfg,val);
  if ((int)vec.size()!=nBins) {
    std::cerr << "ERROR: size of array '"<<val<<"' = "<<vec.size()<<" and not "<<nBins<<" as expected!"<< std::endl;
    exit(1);
  }
  for (std::vector<double>::iterator it=vec.begin();it!=vec.end();++it){
    p.integrated[sample].yield += *it;
    int i = it-vec.begin();
    while (i>=(int)p.bins.size()) { p.bins.push_back( point::bin() ); }
    p.bins[ i ].samples[ sample ].yield = *it; 
  }
}

void AddContamination(point& p, ConfigFile* cfg, const std::string& val, const std::string& sample, int nBins)
{
  std::vector<double> vec = Get(cfg,val);
  if ((int)vec.size()!=nBins) {
    std::cerr << "ERROR: size of array '"<<val<<"' = "<<vec.size()<<" and not "<<nBins<<" as expected!"<< std::endl;
    exit(1);
  }
  for (std::vector<double>::iterator it=vec.begin();it!=vec.end();++it){
    p.integrated[sample].contamination += *it;
    int i = it-vec.begin();
    while (i>=(int)p.bins.size()) { p.bins.push_back( point::bin() ); }
    p.bins[ i ].samples[ sample ].contamination = *it; 
  }
}

void AddSystematics(point& p, ConfigFile* cfg, const std::string& val, const std::string& name, const std::string& sample, int nBins, double corr=1)
{
  std::vector<double> vec = Get(cfg,val);
  if ((int)vec.size()!=nBins) {
    std::cerr << "ERROR: size of array '"<<val<<"' = "<<vec.size()<<" and not "<<nBins<<" as expected!"<< std::endl;
    exit(1);
  }
  for (std::vector<double>::iterator it=vec.begin();it!=vec.end();++it){
    p.integrated[sample].abs_syst_unc[name+" (squared)"] += ((*it) * (*it)); //squared
    int i = it-vec.begin();
    p.bins[ i ].samples[ sample ].abs_syst_unc[name] = corr * (*it); 
  }
  if (std::find(p.systematics.begin(),p.systematics.end(),name)==p.systematics.end()) 
    p.systematics.push_back(name);
}

void AddSystematicsRelative(point& p, double rel_val, const std::string& name, const std::string& sample, int nBins)
{
  p.integrated[sample].abs_syst_unc[name+" abs (squared)"] += rel_val*rel_val*p.integrated[sample].yield*p.integrated[sample].yield; //squared
  for (std::vector<point::bin>::iterator it=p.bins.begin();it!=p.bins.end();++it){
    it->samples[ sample ].abs_syst_unc[name+"_abs"] = rel_val * it->samples[ sample ].yield; 
  }
  
  if (std::find(p.systematics.begin(),p.systematics.end(),name+"_abs")==p.systematics.end()) 
    p.systematics.push_back(name+"_abs");
}

void AddStatistics(point& p, ConfigFile* cfg, const std::string& val, const std::string& name, const std::string& sample, int nBins)
{
  std::vector<double> vec = Get(cfg,val);
  if ((int)vec.size()!=nBins) {
    std::cerr << "ERROR: size of array '"<<val<<"' = "<<vec.size()<<" and not "<<nBins<<" as expected!"<< std::endl;
    exit(1);
  }
  for (std::vector<double>::iterator it=vec.begin();it!=vec.end();++it){
    p.integrated[sample].abs_stat_unc[sample+" "+name+" (squared)"] += ((*it) * (*it)); //squared
    int i = it-vec.begin();
    p.bins[ i ].samples[ sample ].abs_stat_unc[sample+"_"+name] = *it; 
  }
  if (std::find(p.statistics.begin(),p.statistics.end(),sample+"_"+name)==p.statistics.end()) 
    p.statistics.push_back(sample+"_"+name);
}

void Check(point &p) 
///makes sure that number of bins and number of uncertainties are consistant
{
  int nBins = (int)p.info["nBins"];
  assert( nBins==(int)p.bins.size() );
  int nsamp = -1;
  for (std::vector<point::bin>::iterator it=p.bins.begin(); it!=p.bins.end(); ++it){
    assert( nsamp==-1 || nsamp==(int)it->samples.size() );
    nsamp = (int)it->samples.size();   
  }
  int nsyst[nsamp], nstat[nsamp]; for (int i=0;i<nsamp;++i) {nsyst[i]=-1; nstat[i]=-1;}
  for (std::vector<point::bin>::iterator it=p.bins.begin(); it!=p.bins.end(); ++it){
    int s_idx = 0;
    for (std::map<std::string,point::sample>::iterator s=it->samples.begin(); s!=it->samples.end(); ++s,++s_idx){
      assert( nsyst[s_idx]==-1 || nsyst[s_idx]==(int)s->second.abs_syst_unc.size() );
      nsyst[s_idx] = s->second.abs_syst_unc.size();   
      assert( nstat[s_idx]==-1 || nstat[s_idx]==(int)s->second.abs_stat_unc.size() );
      nstat[s_idx] = (int)s->second.abs_stat_unc.size();   
    }  
  }
}

//Read Johannes Datacard
void ReadSignal(std::string sig_file, std::string dat_file="", std::string bgd_file="") {
  ConfigFile * cfg     = new ConfigFile(sig_file);
  ConfigFile * dat_cfg = (dat_file==""?cfg:    new ConfigFile(dat_file));
  ConfigFile * bgd_cfg = (bgd_file==""?dat_cfg:new ConfigFile(bgd_file));
  if(cfg==0||dat_cfg==0||bgd_cfg==0)
    std::cerr << "FILE NOT FOUND! scan file= "<<cfg<<", data file="<<dat_cfg<<", bgd file="<<bgd_cfg<<std::endl;
  
  int nPoints = cfg->read<int>("nPoints",0), n=0;   
  
  //loop over all signal points
  do {
    if (n>=nPoints) break;
    
    point p;
    std::stringstream ss,fn;
    p.nr = n;
    ss << "Point " << n++;
    
    Add(p.info, cfg, ss.str()+" wino mass","wino mass");
    Add(p.info, cfg, ss.str()+" bino mass","bino mass");
    Add(p.info, cfg, ss.str()+" gluino mass","gluino mass",-1);
    Add(p.info, cfg, ss.str()+" squark mass","squark mass",-1);
    Add(p.info, cfg, "nGen");
    Add(p.info, cfg, ss.str()+" Signal xs","Xsection.NLO");
    Add(p.info, cfg, "Lumi");
    int nBins = (int)Add(p.info, cfg, "nBins");
    fn << "LimitInput_"<<p.nr<<"_Wino"<<p.info["wino mass"]<<"_Bino"<<p.info["bino mass"];
    p.filename = fn.str();

   //data
    AddYields(p, dat_cfg, "Data yield", "data", nBins);


    //backgds
    AddYields(p, dat_cfg, "BG Vg yield", "Vg", nBins);
    AddSystematics(p, dat_cfg, "BG Vg syst uncertainty abs", "Scaling_syst_abs",  "Vg", nBins, -1);
    AddStatistics( p, dat_cfg, "BG Vg stat uncertainty abs", "stat_abs",     "Vg", nBins);

    AddYields(p, dat_cfg, "BG gjets yield", "gjets", nBins);
    AddSystematics(p, dat_cfg, "BG gjets syst uncertainty abs", "Scaling_syst_abs", "gjets", nBins);
    AddStatistics( p, dat_cfg, "BG gjets stat uncertainty abs", "stat_abs",         "gjets", nBins);

    AddYields(p, dat_cfg, "BG ttg yield", "ttg", nBins);
    AddSystematics(p, dat_cfg, "BG ttg syst uncertainty abs", "ttg_syst_abs", "ttg", nBins);
    AddSystematicsRelative(p, 0.0255, "lumi_unc", "ttg", nBins);
    AddStatistics( p, dat_cfg, "BG ttg stat uncertainty abs", "stat_abs",     "ttg", nBins);

    AddYields(p, dat_cfg, "BG QCD yield", "qcd", nBins);
    AddSystematics(p, dat_cfg, "BG QCD syst uncertainty abs", "qcd_syst_abs", "qcd", nBins);
    AddSystematicsRelative(p, 0.0255, "lumi_unc", "qcd", nBins);
    AddStatistics( p, dat_cfg, "BG QCD stat uncertainty abs", "stat_abs",     "qcd", nBins);

    AddYields(p, dat_cfg, "BG diboson yield", "diboson", nBins);
    AddSystematics(p, dat_cfg, "BG diboson syst uncertainty abs", "diboson_syst_abs", "diboson", nBins);
    AddSystematicsRelative(p, 0.0255, "lumi_unc", "diboson", nBins);
    AddStatistics( p, dat_cfg, "BG diboson stat uncertainty abs", "stat_abs",     "diboson", nBins);

    AddYields(p, dat_cfg, "BG efake yield", "efake", nBins);
    AddSystematics(p, dat_cfg, "BG efake syst uncertainty abs", "efake_syst_abs", "efake", nBins);
    AddSystematicsRelative(p, 0.0255, "lumi_unc", "efake", nBins);
    AddStatistics( p, dat_cfg, "BG efake stat uncertainty abs", "stat_abs",         "efake", nBins);

   
    //signal
    AddYields(     p, cfg, ss.str()+" Signal yield", "signal", nBins);
    //AddSystematics(p, cfg, ss.str()+" Signal syst. uncertainty", "signal", nBins);
    AddSystematicsRelative(p, 0.0255, "lumi_unc", "signal", nBins);
    AddStatistics( p, cfg, ss.str()+" Signal stat uncertainty abs", "stat_abs", "signal", nBins);

    Check(p);

    Points.Add(p);
  } while(1);

  if (bgd_cfg!=dat_cfg) delete bgd_cfg;
  if (dat_cfg!=cfg) delete dat_cfg;
  delete cfg;
  std::cout << "READ signal file: '"<<sig_file<<"'" << std::endl;; 
}

/*
void AddSMSXsec(std::string filelist) {
	std::cout << "READ XS---------file:" << filelist << std::endl;
	std::ifstream masses_file;
	masses_file.open(filelist.c_str());
	std::string file;
	point p;
	//p.lumi = luminosity;
	p.squark = 0; //eigentlich 1E09, aber 'GetPointsWithSameMass' checked 0..
	p.u_pdfxsec = 0;
	p.u_pdfacc = 0;
	double xsecNLO_Up, xsecNLO_Dn;
	while (1) {
	   masses_file >> p.gluino >> p.xsecNLO >> xsecNLO_Up >> xsecNLO_Dn;
	   
	   p.u_NLO_Up = (p.xsecNLO==0?0:xsecNLO_Up/p.xsecNLO);
	   p.u_NLO_Dn = (p.xsecNLO==0?0:xsecNLO_Dn/p.xsecNLO);
	   p.u_NLO    =  p.u_NLO_Up;
	      
	   if (!masses_file.good()) break;

	   std::vector<point *> aPoints;
	   aPoints = Points.GetPointsWithSameMass(p.gluino, p.squark);

	   for (std::vector<point *>::iterator it = aPoints.begin(); it != aPoints.end(); ++it){
	  	 point * a = *it;
	  	 if (a && !a->u_NLO){
	  		  //std::cout << "READ XSEC:" << p.xsecNLO << std::endl;
	  		//	   std::cout << "sq" << a->squark << std::endl;
	  		//	   std::cout << "gl" <<a->gluino << std::endl;
	  	 	    // a->xsec        = p.xsec;
	  	 	     a->xsecNLO     = p.xsecNLO;
	  	 	     a->signal     *= p.lumi*p.xsecNLO/a->totalGenerated;
	  	 	     //a->u_sig_stat *= p.lumi*p.xsecNLO/a->totalGenerated;
	  	 	     a->qcd_contamination  *= p.lumi*p.xsecNLO/a->totalGenerated;
	  	 	     a->ewk_contamination  *= p.lumi*p.xsecNLO/a->totalGenerated;
	  	 	     a->u_NLO       = p.u_NLO;
	  	 	     a->u_NLO_Up    = p.u_NLO_Up;
	  	 	     a->u_NLO_Dn    = p.u_NLO_Dn;
	  		     a->u_pdfxsec   = p.u_pdfxsec;
			     a->u_pdfacc    = p.u_pdfacc;
           	             for (std::vector<point::bin>::iterator bin=a->bins.begin(); bin!=a->bins.end(); ++bin) {
	  	 	       bin->signal     *= p.lumi*p.xsecNLO/a->totalGenerated;
	  	 	       //bin->u_sig_stat *= p.lumi*p.xsecNLO/a->totalGenerated;
	  	 	       bin->qcd_contamination *= p.lumi*p.xsecNLO/a->totalGenerated;
	  	 	       bin->ewk_contamination *= p.lumi*p.xsecNLO/a->totalGenerated;
	  	 	       bin->u_NLO = 1.0; //not considered for limit calculation
		               bin->u_pdfxsec = 1.0;
			       bin->u_pdfacc  = 1.0;
			       
			       //std::cout<<bin-a->bins.begin()<<": s="<<bin->signal<< ", s (w/o cont)="<<bin->signal-bin->qcd_contamination-bin->ewk_contamination<<std::endl;

	  	 	     }
	  	 	   }
	  	 	   //else Points.Add(p); //We don't actually want x-sections for points for which we don't have event yields
	   }



	}

}

void AddXsec(std::string filelist) {
	std::cout << "READ XS---------file:" << filelist << std::endl;
	std::ifstream masses_file;
	masses_file.open(filelist.c_str());
	std::string file;
	point p;
	//p.lumi = luminosity;
	double ngen, LO_dn, LO_up, NLO_up, NLO_dn;
	while (1) {
	   masses_file >> ngen >> p.squark >> p.gluino >> p.chi >> p.cha >> p.xsec >> LO_up >> LO_dn >> p.xsecNLO
	   	       >> NLO_up >> NLO_dn;
	   if (!masses_file.good()) break;
	   
	   //std::cout << "READ XSEC:" << p.xsecNLO << std::endl;
	   //std::cout << "sq" << p.squark << std::endl;
	   //std::cout << "gl" << p.gluino << std::endl;
	   //std::cout << "neutr" << p.chi << std::endl;
	   
	   point * a = 0;
	   a = Points.Get(p.gluino, p.squark, p.chi, p.cha);
	   //std::cout<<"gl:"<<p.gluino<<", sq:"<<p.squark<<", chi:"<<p.chi<<" cha:"<<p.cha<<"; a="<<a<<std::endl;
	   if (a && !a->u_NLO){
	     a->xsec        = p.xsec;
	     a->xsecNLO     = p.xsecNLO;
	     a->signal     *= a->lumi*p.xsecNLO/a->totalGenerated;
	     //a->u_sig_stat *= a->lumi*p.xsecNLO/a->totalGenerated;
	     a->qcd_contamination  *= a->lumi*p.xsecNLO/a->totalGenerated;
	     a->ewk_contamination  *= a->lumi*p.xsecNLO/a->totalGenerated;
	     a->u_NLO       = NLO_up / p.xsecNLO;
	     a->u_NLO_Up    = NLO_up / p.xsecNLO;
	     a->u_NLO_Dn    = NLO_dn / p.xsecNLO;
	     for (std::vector<point::bin>::iterator bin=a->bins.begin(); bin!=a->bins.end(); ++bin) {
	       bin->signal     *= a->lumi*p.xsecNLO/a->totalGenerated;
	       //bin->u_sig_stat *= a->lumi*p.xsecNLO/a->totalGenerated;
	       bin->qcd_contamination *= a->lumi*p.xsecNLO/a->totalGenerated;
	       bin->ewk_contamination *= a->lumi*p.xsecNLO/a->totalGenerated;
	       bin->u_NLO = 1.0 + NLO_up / p.xsecNLO; //assume that 'u_NLO_up' is the absolute uncertainty in the same units as 'xsecNLO'
	     }  
	   }
	   //else Points.Add(p); //We don't actually want x-sections for points for which we don't have event yields
	}

}
*/

void points::Add_WB_PDF(const std::string& filelist) {
	std::cout << "READ Wino/Bino new XS---------file:" << filelist << std::endl;
	std::ifstream masses_file;
	masses_file.open(filelist.c_str());
	std::string file;
	double totalGenerated, squark, gluino, bino, wino, xsec, LO_up, LO_dn, 
	       xsecNLO, NLO_up, NLO_dn;
	while (1) {
	   //$nevents, $msquark, $mgluino, $mbino, $mwwino, $loxsec,$lohierr,$loloerr,$nloxsec,$nlohierr,$nloloerr); 
	   masses_file >> totalGenerated >> squark >> gluino >> bino >> wino>> xsec >> LO_up >> LO_dn 
	               >> xsecNLO >> NLO_up >> NLO_dn;
	   if (!masses_file.good()) break;
	   
	   //std::cout << "READ XSEC:" << p.xsecNLO << std::endl;
	   //std::cout << "sq" << p.squark << std::endl;
	   //std::cout << "gl" << p.gluino << std::endl;
	   //std::cout << "bino= " << p.chi << std::endl;
	   //std::cout << "wino= " << p.cha << std::endl;
	   
	   point * a = 0;
	   a = Points.Get("bino mass", bino, "wino mass", wino);
	   if (a){
  	     //std::cout << "bino=" << bino << ", wino="<<wino<<", j-xs="<<a->info["Xsection.NLO"] << " <-> xs:"<<xsecNLO*1000. << std::endl;
	     a->info["signal xsec7TeV.LO"] = xsec;
	     a->info["signal xsec7TeV.NLO"] = xsecNLO;
	     
	     a->info["signal u_NLO"]       = fabs( NLO_up / xsecNLO );
	     a->info["signal u_NLO_Up"]    = fabs( NLO_up / xsecNLO );
	     a->info["signal u_NLO_Dn"]    = fabs( NLO_dn / xsecNLO );

	   }
	}

}

/*
void AddPDFs(const std::string filelist) {
	std::ifstream masses_file;
	masses_file.open(filelist.c_str());
	std::string file;
	point p;
	double u_pdfxsec, u_pdfacc;
	while (1) {
	        //nevents,mgluino,msquark,mbino,mwino,xsecpdferrs,acceppdferrs
                masses_file >> p.totalGenerated >> p.gluino >> p.squark >> p.chi >> p.cha >> u_pdfxsec >> u_pdfacc;
		if (!masses_file.good())	break;
		point * a = Points.Get(p.gluino, p.squark, p.chi, p.cha);
                //std::cout<<"PDFxsec gl="<<p.gluino<<", sq="<<p.squark<<", chi="<<p.chi<<", cha="<<p.cha<<"; point = "<<a
                //         <<", pdf_xs="<<u_pdfxsec<<std::endl;
		if (a) {
		  a->u_pdfxsec = 0.01 * u_pdfxsec; //relative per point(!) 
		  a->u_pdfacc  = 0.01 * u_pdfacc;
	          for (std::vector<point::bin>::iterator bin=a->bins.begin(); bin!=a->bins.end(); ++bin) {
		    bin->u_pdfxsec = 1.0 + 0.01 * u_pdfxsec; //factorial per bin(!)
		    bin->u_pdfacc  = 1.0 + 0.01 * u_pdfacc; 
		    
		  }  
                }
	}
	masses_file.close();
}

void AddSmsXsec(const std::string filelist) {
	std::ifstream masses_file;
	masses_file.open(filelist.c_str());
	std::string file;
	point p;
	double xsec, u_xsec;
	while (1) {
	        //nevents,mgluino,msquark,mbino,mwino,xsecpdferrs,acceppdferrs
                masses_file >>  p.gluino >> xsec >> u_xsec;
		if (!masses_file.good())	break;
		std::vector<point *> a = Points.GetGl(p.gluino);
                //std::cout<<"PDFxsec gl="<<p.gluino<<", sq="<<p.squark<<", chi="<<p.chi<<", cha="<<p.cha<<"; point = "<<a
                //         <<", pdf_xs="<<u_pdfxsec<<std::endl;
		for (std::vector<point*>::iterator it=a.begin();it!=a.end();++it){
		  (*it)->u_pdfxsec = 0.01 * u_xsec; //relative per point(!) 
		  (*it)->xsecNLO = xsec;
	  	  (*it)->signal	*= (*it)->lumi*xsec/(*it)->totalGenerated;
	  	  (*it)->qcd_contamination  *= (*it)->lumi*xsec/(*it)->totalGenerated;
	  	  (*it)->ewk_contamination  *= (*it)->lumi*xsec/(*it)->totalGenerated;
	  	  //(*it)->u_NLO	 = p.u_NLO;
	  	  //(*it)->u_NLO_Up	 = p.u_NLO_Up;
	  	  //(*it)->u_NLO_Dn	 = p.u_NLO_Dn;
	  	  //(*it)->u_pdfxsec       = p.u_pdfxsec;
		  //(*it)->u_pdfacc	 = p.u_pdfacc;
		  //std::cout << "lumi="<<p.lumi<<";xsec="<<p.xsecNLO<<",gen="<<(*it)->totalGenerated<<std::endl;
	          for (std::vector<point::bin>::iterator bin=(*it)->bins.begin(); bin!=(*it)->bins.end(); ++bin) {
		    bin->u_pdfxsec = 1.0 + 0.01 * u_xsec; //factorial per bin(!)		    
	  	    bin->signal            *= (*it)->lumi*xsec/(*it)->totalGenerated;
	  	    bin->qcd_contamination *= (*it)->lumi*xsec/(*it)->totalGenerated;
	  	    bin->ewk_contamination *= (*it)->lumi*xsec/(*it)->totalGenerated;
	  	    bin->u_NLO = 1.0; //not considered for limit calculation
		    bin->u_pdfacc  = 1.0;
		    
		    //std::cout<<bin-(*it)->bins.begin()<<": s="<<bin->signal<<""<<std::endl;
		  }  
                }
	}
	masses_file.close();
}

void AddPDFxsec(const std::string filelist, double neutralinomass=0) {
	std::ifstream masses_file;
	masses_file.open(filelist.c_str());
	std::string file;
	point p;
	double u_pdf;
	while (1) {
		if (neutralinomass == 0) {
			masses_file >> p.gluino >> p.squark >> p.chi >> u_pdf;
		} else {
			masses_file >> p.gluino >> p.squark >> u_pdf;
			p.chi = neutralinomass;
		}
		if (!masses_file.good())
			break;
		point * a = 0;
		a = Points.Get(p.gluino, p.squark, p.chi);
		if (a) {
		  a->u_pdfxsec = 0.01 * u_pdf;
	          for (std::vector<point::bin>::iterator bin=a->bins.begin(); bin!=a->bins.end(); ++bin)
		    bin->u_pdfxsec = 1.0 + 0.01 * u_pdf;
                }
	}
	masses_file.close();
}


void AddPDFAcceptance(const std::string filelist, double neutralinomass=0) {
	std::ifstream masses_file;
	masses_file.open(filelist.c_str());
	std::string file;
	point p;
	double u_pdfacc;
	while (1) {
		if (neutralinomass == 0) {
			masses_file >> p.gluino >> p.squark >> p.chi >> u_pdfacc;
		} else {
			masses_file >> p.gluino >> p.squark >> u_pdfacc;
			p.chi = neutralinomass;
		}
		if (!masses_file.good())
			break;
		point * a = 0;
		a = Points.Get(p.gluino, p.squark, p.chi);
		if (a) {
		  a->u_pdfacc = 0.01 * u_pdfacc;
	          for (std::vector<point::bin>::iterator bin=a->bins.begin(); bin!=a->bins.end(); ++bin)
		     bin->u_pdfacc = 1.0 + 0.01 * u_pdfacc;
		}     

	}
	masses_file.close();
}
*/

/*
void DeleteBins(point& p, unsigned bmin=0, unsigned bmax=-1){
  if (bmax<bmin||bmax>p.bins.size()) bmax=p.bins.size();
  if (bmax==bmin) p.bins.erase(p.bins.begin()+bmin);
  p.bins.erase(p.bins.begin()+bmin, p.bins.begin()+bmax);
}

point * MergeBins(const point& p, int bmin=0, int bmax=-1)
{
//std::cout<<"bmin="<<bmin<<", bmax="<<bmax<<", p.bins="<<p.bins.size()<<std::endl;
  if (bmax<bmin||bmax>(int)p.bins.size()) bmax=p.bins.size();
  point * res = new point(p);
  if (p.bins.size()<=0||bmax-bmin<=0) return res;

  bmin=bmin-1;bmax=bmax-1;
  
  res->bins[bmin].u_sig_stat = pow((res->bins[bmin].u_sig_stat-1.0) * res->bins[bmin].signal ,2);
  res->bins[bmin].u_qcd_stat = pow((res->bins[bmin].u_qcd_stat-1.0) * res->bins[bmin].bgd_qcd ,2);
  res->bins[bmin].u_ewk_stat = pow((res->bins[bmin].u_ewk_stat-1.0) * res->bins[bmin].bgd_ewk ,2);
  res->bins[bmin].u_fsr_stat = pow((res->bins[bmin].u_fsr_stat-1.0) * res->bins[bmin].bgd_fsr ,2);
  res->bins[bmin].u_NLO      = (res->bins[bmin].u_NLO-1.0)	      * res->bins[bmin].signal;
  res->bins[bmin].u_pdfxsec  = (res->bins[bmin].u_pdfxsec-1.0)      * res->bins[bmin].signal;
  res->bins[bmin].u_pdfacc   = (res->bins[bmin].u_pdfacc-1.0)       * res->bins[bmin].signal;
  res->bins[bmin].u_qcd      = (res->bins[bmin].u_qcd-1.0)	      * res->bins[bmin].bgd_qcd;      
  res->bins[bmin].u_ewk      = (res->bins[bmin].u_ewk-1.0)	      * res->bins[bmin].bgd_ewk;      
  res->bins[bmin].u_fsr      = (res->bins[bmin].u_fsr-1.0)          * res->bins[bmin].bgd_fsr;      
  for (int i=bmin+1; i<=bmax; ++i){
    res->bins[bmin].signal +=         res->bins[i].signal;
    res->bins[bmin].bgd_qcd +=        res->bins[i].bgd_qcd;
    res->bins[bmin].bgd_ewk +=	      res->bins[i].bgd_ewk;
    res->bins[bmin].bgd_fsr +=	      res->bins[i].bgd_fsr;
    res->bins[bmin].qcd_contamination += res->bins[i].qcd_contamination;
    res->bins[bmin].ewk_contamination += res->bins[i].ewk_contamination;
    res->bins[bmin].data +=	      res->bins[i].data;

    res->bins[bmin].u_NLO +=          (res->bins[i].u_NLO-1.0)      * res->bins[i].signal;
    res->bins[bmin].u_pdfxsec +=      (res->bins[i].u_pdfxsec-1.0)  * res->bins[i].signal; 
    res->bins[bmin].u_pdfacc +=       (res->bins[i].u_pdfacc-1.0)   * res->bins[i].signal;
    res->bins[bmin].u_qcd +=	      (res->bins[i].u_qcd-1.0)      * res->bins[i].bgd_qcd;  
    res->bins[bmin].u_ewk +=	      (res->bins[i].u_ewk-1.0)      * res->bins[i].bgd_ewk;  
    res->bins[bmin].u_fsr +=	      (res->bins[i].u_fsr-1.0)      * res->bins[i].bgd_fsr;  
    res->bins[bmin].u_sig_stat +=     pow((res->bins[i].u_sig_stat-1.0) * res->bins[i].signal ,2);
    res->bins[bmin].u_qcd_stat +=     pow((res->bins[i].u_qcd_stat-1.0) * res->bins[i].bgd_qcd ,2);
    res->bins[bmin].u_ewk_stat +=     pow((res->bins[i].u_ewk_stat-1.0) * res->bins[i].bgd_ewk ,2);
    res->bins[bmin].u_fsr_stat +=     pow((res->bins[i].u_fsr_stat-1.0) * res->bins[i].bgd_fsr ,2);
  }
  res->bins[bmin].u_sig_stat = 1.0 + sqrt(res->bins[bmin].u_sig_stat) / res->bins[bmin].signal;
  res->bins[bmin].u_qcd_stat = 1.0 + sqrt(res->bins[bmin].u_qcd_stat) / res->bins[bmin].bgd_qcd;
  res->bins[bmin].u_ewk_stat = 1.0 + sqrt(res->bins[bmin].u_ewk_stat) / res->bins[bmin].bgd_ewk;
  res->bins[bmin].u_fsr_stat = 1.0 + sqrt(res->bins[bmin].u_fsr_stat) / res->bins[bmin].bgd_fsr;
  res->bins[bmin].u_NLO	     = 1.0 + res->bins[bmin].u_NLO	    / res->bins[bmin].signal;
  res->bins[bmin].u_pdfxsec  = 1.0 + res->bins[bmin].u_pdfxsec    / res->bins[bmin].signal;
  res->bins[bmin].u_pdfacc   = 1.0 + res->bins[bmin].u_pdfacc	    / res->bins[bmin].signal;
  res->bins[bmin].u_qcd	     = 1.0 + res->bins[bmin].u_qcd	    / res->bins[bmin].bgd_qcd;   
  res->bins[bmin].u_ewk	     = 1.0 + res->bins[bmin].u_ewk	    / res->bins[bmin].bgd_ewk;   
  res->bins[bmin].u_fsr	     = 1.0 + res->bins[bmin].u_fsr	    / res->bins[bmin].bgd_fsr;   
  res->bins.erase(res->bins.begin()+bmin+1, res->bins.begin()+bmax+1);
  return res;
}
*/

void points::Do(const std::string& name, const std::string&dat, const std::string&sig, const std::string&xsec, const std::string&pdf)
{
   Points.Reset();
   ReadSignal(sig, dat);
//   if (xsec!="") AddXsec(xsec);
   if (pdf!="") Points.Add_WB_PDF( pdf );
   
   MkDir( "DataCards" );
   Points.Write(((std::string)"DataCards/"+name).c_str());
   Points.WriteSingleBin(((std::string)"DataCards/"+name+"_SingleChannels").c_str());
   
   
   //{ points MergedPoints;
   //  for (std::vector<point>::iterator it=Points.Get()->begin(); it!=Points.Get()->end(); ++it)
   //     MergedPoints.Add( *MergeBins(*it, 6));
   //  std::system( ((std::string)"mkdir DataCards/"+name).c_str());
   //  std::system( ((std::string)"mkdir DataCards/"+name+"_SingleChannels").c_str());
   //  MergedPoints.Write(((std::string)"DataCards/"+name+"/GMSB").c_str());
   //  MergedPoints.WriteSingleBin(((std::string)"DataCards/"+name+"_SingleChannels/GMSB").c_str());
   //}
}


int main(int argc, char* argv[]) {

   std::string signal="inputs/johannes_20140728.txt";
   std::string data  ="inputs/johannes_20140728.txt";   


  // Points.Do("WinoBino", signal, data, "", "");   
   
   Points.Do("WinoBino_MT", "inputs/johannes_20140804_MT.txt", "inputs/johannes_20140804_MT.txt", "", "inputs/Spectra_WB.xsec");   
   Points.Do("WinoBino_NewSigma", "inputs/johannes_20140804_NewSigma.txt", "inputs/johannes_20140804_NewSigma.txt", "", "inputs/Spectra_WB.xsec");   

}
