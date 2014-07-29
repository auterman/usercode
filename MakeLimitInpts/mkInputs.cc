#include "mkInputs.h"
#include "table.h"
#include "ConfigFile.h"
#include <sys/stat.h>
#include <algorithm>

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

void PrintSample(std::ofstream& os, const std::string& str,const point::sample& samp)
{
    os << "# " << str << "  = " << samp.yield << "\n";
    for (std::map<std::string,double>::const_iterator it=samp.abs_syst_unc.begin(); it!=samp.abs_syst_unc.end(); ++it)
      os << "# "<< str << " " << it->first << " = " << it->second << "\n";
    for (std::map<std::string,double>::const_iterator it=samp.abs_stat_unc.begin(); it!=samp.abs_stat_unc.end(); ++it)
      os << "# "<< str << " " << it->first << " = " << it->second << "\n";
}

void points::PrintGlobalSums(std::ofstream& os, point&p)
{
  for (std::map<std::string,point::sample>::const_iterator it=p.integrated.begin();it!=p.integrated.end();++it){
    PrintSample(os,it->first,it->second);
  }
}

void points::PrintBin(std::ofstream& os, point&p, unsigned bin, const std::string& str)
{	  
   for (std::map<std::string,point::sample>::iterator sample=p.bins[bin].samples.begin();sample!=p.bins[bin].samples.end();++sample){
     PrintSample(os, "bin"+ToString(bin)+"_"+sample->first, sample->second);
   }
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
    ofile.open(dir+"/"+point->filename);
    
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
	  unc2 += s->second.GetTotalAbsSyst2();
	  unc2 += s->second.GetTotalAbsStat2();
	}
      }

      double s = bin->samples["signal"].yield;
      for (std::map<std::string,double>::iterator u=bin->samples["signal"].abs_syst_unc.begin();u!=bin->samples["signal"].abs_syst_unc.end();++u)
         unc2 += u->second * u->second;
      for (std::map<std::string,double>::iterator u=bin->samples["signal"].abs_stat_unc.begin();u!=bin->samples["signal"].abs_stat_unc.end();++u)
         unc2 += u->second * u->second;
      if (s) {
        R=2.*sqrt(unc2)/s;
        ofile << "# bin"<< (int)(bin-point->bins.begin()) <<" R_firstguess = " << R << "\n";
	if (R<Rmin) Rmin=R;  
      }	
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
 	   ss << (sample-1);
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
/*
  using namespace Table;
  using namespace std;
  // std::cout<<"write single bins:"<<p_.size()<<endl;
  for (vector<point>::iterator it = p_.begin(); it != p_.end(); ++it) {
     //std::cout << "point " <<it-p_.begin()<<", bins = "<<it->bins.size() << std::endl;
     for (int bin=0; bin<(int)it->bins.size(); ++bin){
  	  ofstream ofile;
  	  stringstream ss;
  	  ss << dir << "_" << it->squark << "_" << it->gluino << "_"
  			  << it->chi << "_"<<it->cha<<"_bin"<<bin<<".txt";
  	  ofile.open(ss.str().c_str());
	  
          PrintGlobal( ofile, *it );
	  PrintBin(ofile, *it, bin, "");

  	  int n_backgrounds = 3;
  	  int n_nuisance    = 8; //systs & stats
  	  double u_sig = 1.0 + sqrt( //pow(it->bins[b].u_NLO-1.,2) +
  				       pow(it->bins[bin].u_sig_stat-1,2) +
  				       //pow(it->bins[bin].u_pdfxsec-1.,2) +
  				       pow(it->bins[bin].u_scaleDataMC-1.,2) +
  				       pow(it->bins[bin].u_sig-1.,2) +
  				       pow(it->bins[bin].u_pdfacc-1.,2) );


      ///some rough by-hand calculation of 'R' to pre-define (and check) the search range:
      ///---
      double d=0,b=0,s=0,cont=0,R;
      d+=it->bins[bin].data;
      b+=it->bins[bin].bgd_qcd + it->bins[bin].bgd_ewk + it->bins[bin].bgd_fsr;
      s+=it->bins[bin].signal;
      cont+=it->bins[bin].qcd_contamination + it->bins[bin].ewk_contamination;
      double unc2= it->bins[bin].data;
      if (it->bins[bin].u_scaleDataMC) unc2 += pow(it->bins[bin].u_scaleDataMC-1.,2);
      if (it->bins[bin].u_sig)      unc2 += pow(it->bins[bin].u_sig-1.,2);
      if (it->bins[bin].u_pdfacc)   unc2 += pow(it->bins[bin].u_pdfacc-1.,2);
      if (it->bins[bin].u_lumi)     unc2 += pow(it->bins[bin].u_lumi-1,2);
      if (it->bins[bin].u_qcd)      unc2 += pow(it->bins[bin].u_qcd-1,2);
      if (it->bins[bin].u_ewk)      unc2 += pow(it->bins[bin].u_ewk-1,2);
      if (it->bins[bin].u_fsr)      unc2 += pow(it->bins[bin].u_fsr-1,2);
      if (it->bins[bin].u_sig_stat) unc2 += pow(it->bins[bin].u_sig_stat-1,2);
      if (it->bins[bin].u_qcd_stat) unc2 += pow(it->bins[bin].u_qcd_stat-1,2);
      if (it->bins[bin].u_ewk_stat) unc2 += pow(it->bins[bin].u_ewk_stat-1,2);
      if (it->bins[bin].u_fsr_stat) unc2 += pow(it->bins[bin].u_fsr_stat-1,2);
      R=2.*sqrt(unc2)/it->bins[bin].signal;
      ofile << "# R_firstguess = " << R << "\n";

  	  ofile << "imax  1  number of channels" << endl;
  	  ofile << "jmax " << setw(2) << n_backgrounds << "  number of backgrounds" << endl;
  	  ofile << "kmax " << setw(2) << n_nuisance    << "  number of nuisance parameters (sources of systematic uncertainties)" << endl;
  	  ofile << "------------" << endl;  

  	  //observed events in all channels
  	  TTable observed("## observed events");\
  	  observed.SetStyle(Empty);
  	  observed.SetDelimiter("  ");
  	  observed.AddColumn<string>(""); 
  	  observed.AddColumn<int>("");
  	  observed << "bin" << 0;
  	  observed << "observation" << (int)it->bins[bin].data;
  	  ofile << observed << "------------\n" << endl;  

  	  //expected events in all channels for signal and all backgrounds
  	  TTable exp("## expected events");
  	  exp.SetStyle(Empty);
  	  exp.SetDelimiter("  ");
  	  exp.AddColumn<string>(""); 
  	  exp.SetMinumumWidth(20,0);//make first column at least 20 chars
  	  for (int sample=1; sample<=n_backgrounds+1; ++sample) 
  		exp.AddColumn<string>("");
  	  exp << "bin"; 
  	      for (int sample=1; sample<=n_backgrounds+1; ++sample) {
  		 exp << "0";
  	      }
  	  exp << "process" << "signal" << "qcd" << "ewk" << "fsr";
  	  exp << "process"; 
  	      for (int sample=1; sample<=n_backgrounds+1; ++sample) {
  		 stringstream ss;
  		 ss << (sample-1);
  		 exp << ss.str();
  	      }   
  	  exp << "rate"; 
  	    exp << ToString(it->bins[bin].signal  
  			    - it->bins[bin].qcd_contamination 
  			    - it->bins[bin].ewk_contamination )
  		<< ToString(it->bins[bin].bgd_qcd)
  		<< ToString(it->bins[bin].bgd_ewk)
  		<< ToString(it->bins[bin].bgd_fsr);
  	  ofile << exp << "------------" << std::endl;  

  	  TTable sys("");
  	  sys.SetStyle(Empty);
  	  sys.SetDelimiter("  ");
  	  sys.AddColumn<string>(""); 
  	  sys.SetMinumumWidth(20,0);//make first column at least 20 chars
  	      for (int sample=1; sample<=n_backgrounds+1; ++sample) 
  		sys.AddColumn<string>("");
  	  sys << "U_Sig lnN";
  	    sys << ToString(u_sig,"-") // signal
  		<< "-" << "-" << "-"; //qcd, ewk, fsr
  	  sys << "U_Lumi lnN";
  	    sys << ToString(it->bins[bin].u_lumi,"-") // signal
  		<< "-" // qcd data-driven
  		<< "-" // ewk data-driven
  		<< ToString(it->bins[bin].u_lumi,"-");//fsr
  	  sys << "U_qcd lnN";
  	    sys << "-" // signal
  		<< ToString(it->bins[bin].u_qcd,"-") << "-" << "-"; //qcd, ewk, fsr
  	  sys << "U_ewk lnN";
  	    sys << "-" // signal
  		<< "-" << ToString(it->bins[bin].u_ewk,"-") << "-"; //qcd, ewk, fsr
  	  sys << "U_fsr lnN";
  	    sys << "-" // signal
  		<< "-" << "-" << ToString(it->bins[bin].u_fsr,"-"); //qcd, ewk, fsr

  	  //Now the statistical uncertainties:...................................
  	  sys << "U_qcd_statistic lnN" << "-" << ToString(it->bins[bin].u_qcd_stat,"-") << "-" << "-"; 
  	  sys << "U_ewk_statistic lnN" <<"-"<< "-"<< ToString(it->bins[bin].u_ewk_stat,"-")<< "-"; 
  	  sys << "U_fsr_statistic lnN" <<"-"<< "-"<<"-"<< ToString(it->bins[bin].u_fsr_stat,"-");
  	  ofile << sys << "------------" << std::endl;  
  	  //cout << "WRITE GL:" << it->gluino << ", SQ:" << it->squark << endl;

  	  ofile.close();
      }   

  }
*/
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
    fn << "LimitInput_"<<p.nr<<"_Wino"<<p.info["wino mass"]<<"_Bino"<<p.info["bino mass"]<<".txt";
    p.filename = fn.str();

   //data
    AddYields(p, dat_cfg, "Data yield", "data", nBins);


    //backgds
    AddYields(p, dat_cfg, "BG Vg yield", "Vg", nBins);
    AddSystematics(p, dat_cfg, "BG Vg syst uncertainty abs", "Scaling_syst_abs",  "Vg", nBins);
    AddStatistics( p, dat_cfg, "BG Vg stat uncertainty abs", "stat_abs",     "Vg", nBins);

    AddYields(p, dat_cfg, "BG gjets yield", "gjets", nBins);
    AddSystematics(p, dat_cfg, "BG gjets syst uncertainty abs", "Scaling_syst_abs", "gjets", nBins, -1);
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

void Add_WB_NewXsec(std::string filelist) {
	std::cout << "READ Wino/Bino new XS---------file:" << filelist << std::endl;
	std::ifstream masses_file;
	masses_file.open(filelist.c_str());
	std::string file;
	point p;
	p.lumi = luminosity;
	double LO_dn, LO_up, NLO_up, NLO_dn;
	while (1) {
	   //$nevents, $msquark, $mgluino, $mbino, $mwwino, $loxsec,$lohierr,$loloerr,$nloxsec,$nlohierr,$nloloerr); 
	   masses_file >> p. totalGenerated >> p.squark >> p.gluino >> p.chi >> p.cha >> p.xsec >> LO_up >> LO_dn 
	               >> p.xsecNLO >> NLO_up >> NLO_dn;
	   if (!masses_file.good()) break;
	   
	   //std::cout << "READ XSEC:" << p.xsecNLO << std::endl;
	   //std::cout << "sq" << p.squark << std::endl;
	   //std::cout << "gl" << p.gluino << std::endl;
	   //std::cout << "bino= " << p.chi << std::endl;
	   //std::cout << "wino= " << p.cha << std::endl;
	   
	   point * a = 0;
	   a = Points.Get(p.gluino, p.squark, p.chi, p.cha);
	   if (a && !a->u_NLO){
  	     //std::cout << "found a= " << a->squark << std::endl;
	     a->squark      = p.squark;
	     a->gluino      = p.gluino;
	     a->xsec        = p.xsec;
	     a->xsecNLO     = fabs( p.xsecNLO );
	     a->signal     *= p.lumi*p.xsecNLO/p.totalGenerated;
	     //a->u_sig_stat *= p.lumi*p.xsecNLO/p.totalGenerated;
	     a->qcd_contamination  *= p.lumi*p.xsecNLO/p.totalGenerated;
	     a->ewk_contamination  *= p.lumi*p.xsecNLO/p.totalGenerated;
	     a->u_NLO       = fabs( NLO_up / p.xsecNLO );
	     a->u_NLO_Up    = fabs( NLO_up / p.xsecNLO );
	     a->u_NLO_Dn    = fabs( NLO_dn / p.xsecNLO );
	     a->totalGenerated = p.totalGenerated;
	     for (std::vector<point::bin>::iterator bin=a->bins.begin(); bin!=a->bins.end(); ++bin) {
	       bin->signal     *= p.lumi*p.xsecNLO/p.totalGenerated;
	       //bin->u_sig_stat *= p.lumi*p.xsecNLO/p.totalGenerated;
	       bin->qcd_contamination *= p.lumi*p.xsecNLO/p.totalGenerated;
	       bin->ewk_contamination *= p.lumi*p.xsecNLO/p.totalGenerated;
	       bin->u_NLO = 1.0 + fabs( NLO_up / p.xsecNLO ); //assume that 'u_NLO_up' is the absolute uncertainty in the same units as 'xsecNLO'
	     }  
	   }
	   //else Points.Add(p); //We don't actually want x-sections for points for which we don't have event yields

	}

}

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
//   if (pdf!="")  AddPDFs(pdf);
   
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


   Points.Do("WinoBino", signal, data, "", "");   

}
