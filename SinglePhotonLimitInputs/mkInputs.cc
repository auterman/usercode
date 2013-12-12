//create input files for different limit tools
//Christian Autermann, Universität Hamburg

//system
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <exception>
#include <algorithm>

#include "ConfigFile.h"
#include "mkInputs.h"

using namespace std;

const double trigger_uncertainty = 0.022; //2% trigger uncertainty
const std::string SignalSystematicVeto1="SIGNAL_THEORY_SYST";
const std::string SignalSystematicVeto2="SIGNAL_SYST_XSEC";


std::string ToString(double d, std::string s){
  if (d==0&&s!="") return s;
  std::stringstream ss;
  ss << d;
  return ss.str();
}

std::string ToString(int d, std::string s){
  if (d==0&&s!="") return s;
  std::stringstream ss;
  ss << d;
  return ss.str();
}

double ToDouble(const std::string& s)
{
  double r;
  std::stringstream ss(s);
  ss >> r;
  return r;
}

bool StrContains(std::string str,std::string str2){
  std::transform(str.begin(), str.end(),str.begin(), ::toupper);
  return (str.find(str2)!=string::npos);
}

void point::Scale(point::Sample*s, double scale, double scaleUnc)
{
  s->eventyield *= scale;
  for (std::vector<Uncertainties>::iterator u=s->uncert.begin();u!=s->uncert.end();++u){
    u->up *= scaleUnc;
    u->dn *= scaleUnc; 
  }  
}

void point::ScaleLumi(double scale, double scaleUnc)
{
  lumi *= scale;
  for (std::vector<Bin>::iterator it=bins.begin(); it!=bins.end();++it){
    Scale(&it->data, scale, scaleUnc);
    Scale(&it->signal, scale, scaleUnc);
    for (std::vector<Sample>::iterator b=it->backgds.begin(); b!=it->backgds.end(); ++b)
      Scale(&(*b), scale, scaleUnc );   
  }
}


void points::ScaleLumi(double scale, double ScaleUnc)
{
    lumiscale_=scale;
    lumiuncscale_=ScaleUnc;
    for (vector<point>::iterator it = p_.begin(); it != p_.end(); ++it)
      it->ScaleLumi(scale, ScaleUnc);
}

void points::WriteHiggsInputSingleChannel(const std::string dir, const std::string prefix) {
    using namespace Table;
    using namespace std;
    std::cout << "...writing datacards for single channels" <<std::endl;
    for (vector<point>::iterator it = p_.begin(); it != p_.end(); ++it) {
       int n_nonzero_channels=0;
       for (int bin=0; bin<(int)it->bins.size(); ++bin) {
	 if (it->bins[bin].signal.eventyield - it->bins[bin].signal.contamination >0) ++n_nonzero_channels;
	 else continue;
	 unsigned b=bin;
  
         ofstream ofile;
         stringstream ss;
         //std::system(((string)"mkdir "+dir).c_str());
         ss << dir << "/"<<prefix<<"_" << ToString(it->nr) << "_bin_"<< ToSTring(bin) <<".txt";

	 ofile.open(ss.str().c_str());
	 for (std::map<std::string,std::string>::iterator p=it->cmssm.params.begin(); p!=it->cmssm.params.end(); ++p)
	   ofile << "# " <<p->first<<" = " << p->second << "\n";
	 ofile << "# Luminosity = " << it-> lumi << "\n";
	 ofile << "# Channel = "<<b<<"\n";

	 int n_backgrounds = it->bins[0].backgds.size();
	 int n_nuisance	 = 0;
	 for (std::map<std::string,std::vector<std::string> >::const_iterator u=it->uncertainties.begin();
              u!=it->uncertainties.end(); ++u) 
	       ++n_nuisance;   

	 ofile << "# n_channels = 1 \n";
         ofile << "# "<<it->bins[b].data.label<<"_0 = "<<it->bins[b].data.eventyield<<"\n";

         ofile << "# "<<it->bins[b].signal.label<<"_0 = "<<it->bins[b].signal.eventyield<<"\n";
         ofile << "# "<<it->bins[b].signal.label<<"_contamination_0 = "<<it->bins[b].signal.contamination<<"\n";
	 for (int sample=0; sample<n_backgrounds;++sample) 
           ofile << "# "<<it->bins[b].backgds[sample].label<<"_0 = "
	         << it->bins[b].backgds[sample].eventyield<<"\n";


	 ofile << "imax  1  number of channels" << endl;
	 ofile << "jmax " << setw(2) << n_backgrounds << "  number of backgrounds" << endl;
	 ofile << "kmax " << setw(2) << n_nuisance    
               << "  number of nuisance parameters (sources of systematic uncertainties)" << endl;
	 ofile << "------------" << endl;  

	 //observed events in all channels
	 TTable observed("## observed events");
	 observed.SetStyle(Empty);
	 observed.SetDelimiter("  ");
	 observed.AddColumn<string>(""); 
	 observed.AddColumn<int>("");
	 observed << "bin" << 0;
	 observed << "observation" << (int)it->bins[b].data.eventyield;
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
        	stringstream ss;
        	ss << 0;
        	exp << ss.str();
	 }   
	 exp << "process";
         exp << it->bins[b].signal.label;
	 for (int sample=0; sample<n_backgrounds;++sample) 
             exp << it->bins[b].backgds[sample].label;
	 exp << "process"; 
         for (int sample=1; sample<=n_backgrounds+1; ++sample) {
        	stringstream ss;
        	ss << (sample-1);
        	exp << ss.str();
         }
	 exp << "rate"; 
         exp << ToString(it->bins[b].signal.eventyield - it->bins[b].signal.contamination);
	 for (int sample=0; sample<n_backgrounds;++sample) 
              exp << ToString(it->bins[b].backgds[sample].eventyield);

	 ofile << exp << "------------" << std::endl;  
	 TTable sys("");
	 sys.SetStyle(Empty);
	 sys.SetDelimiter("  ");
	 sys.AddColumn<string>(""); 
	 sys.SetMinumumWidth(20,0);//make first column at least 20 chars
         for (int sample=1; sample<=n_backgrounds+1; ++sample) 
               sys.AddColumn<string>("");
	 for (std::map<std::string,std::vector<std::string> >::const_iterator u=it->uncertainties.begin();
              u!=it->uncertainties.end(); ++u) {
	    sys << u->first;
	      bool foundSyst=false;
              for (int unc=0; unc<(int)it->bins[b].signal.uncert.size();++unc){
		string unc_name( it->bins[b].signal.uncert[unc].label );
        	std::replace(unc_name.begin(), unc_name.end(), ' ', '_'); 
        	if (unc_name+" "+it->bins[b].signal.uncert[unc].distribution ==u->first){
	          sys << ToString(it->bins[b].signal.GetFactorialUncertainty(unc),"-"); // signal
                  it->syst[unc_name] += (it->bins[b].signal.GetFactorialUncertainty(unc)-1)*
		                     it->bins[b].signal.eventyield;

		  foundSyst=true;
        	}
	      }
	      if (!foundSyst) sys << "-";
	      for (int sample=0; sample<n_backgrounds; ++sample) { 
		foundSyst=false; 
        	for (int unc=0; unc<(int)it->bins[b].backgds[sample].uncert.size();++unc){
	          string unc_name( it->bins[b].backgds[sample].uncert[unc].label );
                  std::replace(unc_name.begin(), unc_name.end(), ' ', '_'); 
                  if (unc_name+" "+it->bins[b].backgds[sample].uncert[unc].distribution ==u->first){
	            sys << ToString(it->bins[b].backgds[sample].GetFactorialUncertainty(unc),"-"); // background 'sample'
                    it->syst[unc_name] += (it->bins[b].backgds[sample].GetFactorialUncertainty(unc)-1)*
		                           it->bins[b].backgds[sample].eventyield;
 		    foundSyst=true;
	          }
		}
		if (!foundSyst) sys << "-";
	      }
	    
	 }
	 ofile << sys << "------------" << std::endl;  

	 for (std::map<std::string, double>::const_iterator sys=it->syst.begin();sys!=it->syst.end();++sys){
           ofile << "# "<<sys->first<<" = "<<sys->second<<"\n";
	 } 
	 it->syst.clear();
	 ofile.close();
      }
   }
}


//====================================================================================================
//====================================================================================================
void points::WriteHiggsInputStatUncorr(const std::string dir, const std::string prefix) {
    using namespace Table;
    using namespace std;
    std::cout << "...writing datacards" <<std::endl;
    for (vector<point>::iterator it = p_.begin(); it != p_.end(); ++it) {
       //create result file
       ofstream ofile;
       stringstream ss;
       //std::system(((string)"mkdir "+dir).c_str());
       ss << dir << "/"<<prefix<<"_" << ToString(it->nr)<< ".txt";
       ofile.open(ss.str().c_str());
       //write general SUSY parameter information
       for (std::map<std::string,std::string>::iterator p=it->cmssm.params.begin(); p!=it->cmssm.params.end(); ++p)
	   ofile << "# " <<p->first<<" = " << p->second << "\n";
       ofile << "# point = " << it->nr << "\n";
       ofile << "# Luminosity = " << it-> lumi << "\n";
       if (lumiscale_>1.) {
         ofile << "# Luminosity Scale = " << lumiscale_ << "\n";
         ofile << "# Luminosity Uncertainty Scale = " << lumiuncscale_ << "\n";       
       }
       
       int n_channels	 = it->n_bins;
       int n_nonzero_channels=0;
       int n_backgrounds = it->bins[0].backgds.size();
       int n_nuisance	 = 0;
       for (int bin=0; bin<n_channels; ++bin) 
	   if (it->bins[bin].signal.eventyield - it->bins[bin].signal.contamination>0) ++n_nonzero_channels;
       for (std::map<std::string,std::vector<std::string> >::const_iterator u=it->uncertainties.begin();
            u!=it->uncertainties.end(); ++u) { 
	 if (!theoryUncertainties_ && (StrContains(u->first, SignalSystematicVeto1) || StrContains(u->first, SignalSystematicVeto2)))
	     continue; 
          if (StrContains( u->first, "STAT") && !StrContains( u->first, "QCD"))
             n_nuisance += n_nonzero_channels;
	  else
	     ++n_nuisance;   
       }
	     
       ofile << "# n_channels = " << n_nonzero_channels << "\n";
       double Rmin=9999999999, R;
       for (int bin=0; bin<n_channels; ++bin) {
         if (it->bins[bin].signal.eventyield - it->bins[bin].signal.contamination<=0) continue;
	 double uncert2=it->bins[bin].data.eventyield;
         ofile << "# "<<it->bins[bin].data.label<<"_" << bin<<" = "<<it->bins[bin].data.eventyield<<"\n";
         ofile << "# "<<it->bins[bin].signal.label<<"_" << bin<<" = "<<it->bins[bin].signal.eventyield<<"\n";
         ofile << "# "<<it->bins[bin].signal.label<<"_contamination_" << bin
	              <<" = "<<it->bins[bin].signal.contamination<<"\n";
	 for (int unc=0; unc<(int)it->bins[bin].signal.uncert.size(); ++unc){
           ofile << "# "<<it->bins[bin].signal.label<<"_" << bin<<"_"<<it->bins[bin].signal.uncert[unc].label 
	         <<"_UP = "<<it->bins[bin].signal.uncert[unc].up<<"\n";
           ofile << "# "<<it->bins[bin].signal.label<<"_" << bin<<"_"<<it->bins[bin].signal.uncert[unc].label 
	         <<"_DN = "<<it->bins[bin].signal.uncert[unc].dn<<"\n";
	   uncert2 += pow( (it->bins[bin].signal.uncert[unc].up + it->bins[bin].signal.uncert[unc].dn)/(2.* it->bins[bin].signal.eventyield),2);	 
	 }	 
	 for (int sample=0; sample<n_backgrounds;++sample) {
           ofile << "# "<<it->bins[bin].backgds[sample].label<<"_" << bin<<" = "
	         << it->bins[bin].backgds[sample].eventyield<<"\n";
	   for (int unc=0; unc<(int)it->bins[bin].backgds[sample].uncert.size(); ++unc){
             ofile << "# "<<it->bins[bin].backgds[sample].label<<"_" << bin<<"_"<<it->bins[bin].backgds[sample].uncert[unc].label 
	           <<"_UP = "<<it->bins[bin].backgds[sample].uncert[unc].up<<"\n";
             ofile << "# "<<it->bins[bin].backgds[sample].label<<"_" << bin<<"_"<<it->bins[bin].backgds[sample].uncert[unc].label 
	           <<"_DN = "<<it->bins[bin].backgds[sample].uncert[unc].dn<<"\n";
	     uncert2 += pow((it->bins[bin].backgds[sample].uncert[unc].up+it->bins[bin].backgds[sample].uncert[unc].dn)/(2.*it->bins[bin].backgds[sample].eventyield)  ,2);
	   }
	 }
	 R=2.*sqrt(uncert2)/(it->bins[bin].signal.eventyield - it->bins[bin].signal.contamination);
	 if (R<Rmin) Rmin=R;  	 
       }  
       ofile << "# R_firstguess = " << Rmin << endl;
	     
	     
       ofile << "imax " << setw(2) << n_nonzero_channels    << "  number of channels" << endl;
       ofile << "jmax " << setw(2) << n_backgrounds << "  number of backgrounds" << endl;
       ofile << "kmax " << setw(2) << n_nuisance    
             << "  number of nuisance parameters (sources of systematic uncertainties)" << endl;
       ofile << "------------" << endl;  

       //observed events in all channels
       TTable observed("## observed events");\
       observed.SetStyle(Empty);
       observed.SetDelimiter("  ");
       observed.AddColumn<string>(""); 
       for (int bin=1; bin<=n_channels; ++bin) {
	   if (it->bins[bin-1].signal.eventyield - it->bins[bin-1].signal.contamination<=0) continue;
	   observed.AddColumn<int>("");}
       observed << "bin";	  
       for (int bin=1; bin<=n_channels; ++bin) {
	 if (it->bins[bin-1].signal.eventyield - it->bins[bin-1].signal.contamination<=0) continue;
         observed << bin;
       }
       observed << "observation"; 
       for (int bin=1; bin<=n_channels; ++bin) {
         if (it->bins[bin-1].signal.eventyield - it->bins[bin-1].signal.contamination<=0) continue;
	 stringstream ss;
         ss << "n" << bin;
         observed << (int)it->bins[bin-1].data.eventyield;
       }  
       ofile << observed << "------------\n" << endl;  

       //expected events in all channels for signal and all backgrounds
       TTable exp("## expected events");
       exp.SetStyle(Empty);
       exp.SetDelimiter("  ");
       exp.AddColumn<string>(""); 
       exp.SetMinumumWidth(20,0);//make first column at least 20 chars
       for (int bin=1; bin<=n_channels; ++bin) {
           if (it->bins[bin-1].signal.eventyield - it->bins[bin-1].signal.contamination<=0) continue;
           for (int sample=1; sample<=n_backgrounds+1; ++sample) 
             exp.AddColumn<string>("");
       }     
       exp << "bin"; 
       for (int bin=1; bin<=n_channels; ++bin) {
           if (it->bins[bin-1].signal.eventyield - it->bins[bin-1].signal.contamination<=0) continue;
           for (int sample=1; sample<=n_backgrounds+1; ++sample) {
              stringstream ss;
              ss << bin;
              exp << ss.str();
           }
       }   
       exp << "process";
       for (int bin=0; bin<n_channels; ++bin) {
         if (it->bins[bin].signal.eventyield - it->bins[bin].signal.contamination<=0) continue;
         exp << it->bins[bin].signal.label;
	 for (int sample=0; sample<n_backgrounds;++sample) 
           exp << it->bins[bin].backgds[sample].label;
       }  
       exp << "process"; 
       for (int bin=1; bin<=n_channels; ++bin) {
           if (it->bins[bin-1].signal.eventyield - it->bins[bin-1].signal.contamination<=0) continue;
           for (int sample=1; sample<=n_backgrounds+1; ++sample) {
              stringstream ss;
              ss << (sample-1);
              exp << ss.str();
           }
       }	      
       exp << "rate"; 
       for (int bin=0; bin<n_channels; ++bin) {
         if (it->bins[bin].signal.eventyield - it->bins[bin].signal.contamination<=0) continue;
         exp << ToString(it->bins[bin].signal.eventyield - it->bins[bin].signal.contamination);
	 for (int sample=0; sample<n_backgrounds;++sample) 
            exp << ToString(it->bins[bin].backgds[sample].eventyield);

             
       }  
       ofile << exp << "------------" << std::endl;  
       TTable sys("");
       sys.SetStyle(Empty);
       sys.SetDelimiter("  ");
       sys.AddColumn<string>(""); 
       sys.SetMinumumWidth(20,0);//make first column at least 20 chars
       for (int bin=1; bin<=n_channels; ++bin) {
           if (it->bins[bin-1].signal.eventyield - it->bins[bin-1].signal.contamination<=0) continue;       
           for (int sample=1; sample<=n_backgrounds+1; ++sample) 
             sys.AddColumn<string>("");
       }     
       for (std::map<std::string,std::vector<std::string> >::const_iterator u=it->uncertainties.begin();
            u!=it->uncertainties.end(); ++u) {
	  if (!theoryUncertainties_ &&(StrContains(u->first, SignalSystematicVeto1) || StrContains(u->first, SignalSystematicVeto2))) {
	    for (int b=0; b<n_channels; ++b) {
	      if (it->bins[b].signal.eventyield - it->bins[b].signal.contamination<=0) continue;
                for (int unc=0; unc<(int)it->bins[b].signal.uncert.size();++unc){
 	          string unc_name( it->bins[b].signal.uncert[unc].label );
                  std::replace(unc_name.begin(), unc_name.end(), ' ', '_'); 
                  if (unc_name+" "+it->bins[b].signal.uncert[unc].distribution ==u->first){
                    it->syst[it->bins[b].signal.label+" "+unc_name] +=
                  (it->bins[b].signal.GetFactorialUncertainty(unc)-1)* it->bins[b].signal.eventyield;
		  }
		}
	     }  
	     continue; 
	  }	
  
	  if (StrContains( u->first, "STAT") ){//uncorrelated stat. uncertainties
	    if (print_) cout << u->first << " is a statistical uncertainty!"<<endl;
	    for (int thisbin=0; thisbin<n_channels; ++thisbin){
              if (it->bins[thisbin].signal.eventyield - it->bins[thisbin].signal.contamination<=0) continue; 
              stringstream ss(u->first);
	      string Unc_name, Unc_distr;
	      ss >> Unc_name; 
	      ss >> Unc_distr;
	      stringstream ss1;
	      ss1 << Unc_name<<"_bin"<<thisbin+1<<" "<<Unc_distr;
	      sys << ss1.str();
	      for (int bin=0; bin<n_channels; ++bin){
                if (it->bins[bin].signal.eventyield - it->bins[bin].signal.contamination<=0) continue; 
		//signal:
		bool foundSyst=false;
        	for (int unc=0; unc<(int)it->bins[bin].signal.uncert.size();++unc){
		  string unc_name( it->bins[bin].signal.uncert[unc].label );
        	  std::replace(unc_name.begin(), unc_name.end(), ' ', '_'); 

		  //cout << "look for: "<<"signal_"+unc_name+" "+it->bins[bin].signal.uncert[unc].distribution<< ", found: "<<u->first<<endl;
        	  if (bin==thisbin && "signal_"+unc_name+" "+it->bins[bin].signal.uncert[unc].distribution ==u->first){
	            sys << ToString(it->bins[bin].signal.GetFactorialUncertainty(unc),"-"); // signal
		    it->stat2[it->bins[bin].signal.label+" "+unc_name] +=
		       pow((it->bins[bin].signal.GetFactorialUncertainty(unc)-1)*
		                              it->bins[bin].signal.eventyield,2);
		    foundSyst=true;
        	  }
	        }
                if (!foundSyst) sys << "-";
                //backgds:
		for (int sample=0; sample<n_backgrounds; ++sample) { 
		  foundSyst=false; 
        	  for (int unc=0; unc<(int)it->bins[bin].backgds[sample].uncert.size();++unc){
		    string unc_name( it->bins[bin].backgds[sample].uncert[unc].label );
        	    std::replace(unc_name.begin(), unc_name.end(), ' ', '_'); 
        	    if (bin==thisbin && unc_name+" "+it->bins[bin].backgds[sample].uncert[unc].distribution ==u->first){
	              sys << ToString(it->bins[bin].backgds[sample].GetFactorialUncertainty(unc),"-"); // background 'sample'
		      it->stat2[it->bins[bin].backgds[sample].label+" "+unc_name] +=
		         pow((it->bins[bin].backgds[sample].GetFactorialUncertainty(unc)-1)*
		                             it->bins[bin].backgds[sample].eventyield,2);
 		      foundSyst=true;
		    }
		  }
		  if (!foundSyst) sys << "-";
		}
	      }	
	    }
	  } else {  
	  sys << u->first;
	  for (int b=0; b<n_channels; ++b) {
	    if (it->bins[b].signal.eventyield - it->bins[b].signal.contamination<=0) continue;
	    bool foundSyst=false;
            for (int unc=0; unc<(int)it->bins[b].signal.uncert.size();++unc){
	      string unc_name( it->bins[b].signal.uncert[unc].label );
              std::replace(unc_name.begin(), unc_name.end(), ' ', '_'); 
              if (unc_name+" "+it->bins[b].signal.uncert[unc].distribution ==u->first){
	        sys << ToString(it->bins[b].signal.GetFactorialUncertainty(unc),"-"); // signal
                it->syst[it->bins[b].signal.label+" "+unc_name] += (it->bins[b].signal.GetFactorialUncertainty(unc)-1)* it->bins[b].signal.eventyield;
                it->systUp[it->bins[b].signal.label+" "+unc_name] += (it->bins[b].signal.GetFactorialUncertaintyUp(unc)-1)* it->bins[b].signal.eventyield;
                it->systDn[it->bins[b].signal.label+" "+unc_name] += (it->bins[b].signal.GetFactorialUncertaintyDn(unc)-1)* it->bins[b].signal.eventyield;

		foundSyst=true;
              }
	    }
	    if (!foundSyst) sys << "-";
	    for (int sample=0; sample<n_backgrounds; ++sample) { 
	      foundSyst=false; 
              for (int unc=0; unc<(int)it->bins[b].backgds[sample].uncert.size();++unc){
	        string unc_name( it->bins[b].backgds[sample].uncert[unc].label );
                std::replace(unc_name.begin(), unc_name.end(), ' ', '_'); 
                if (unc_name+" "+it->bins[b].backgds[sample].uncert[unc].distribution ==u->first){
	          sys << ToString(it->bins[b].backgds[sample].GetFactorialUncertainty(unc),"-"); // background 'sample'
                  it->syst[it->bins[b].backgds[sample].label+" "+unc_name] += (it->bins[b].backgds[sample].GetFactorialUncertainty(unc)-1)* it->bins[b].backgds[sample].eventyield;
                  it->systUp[it->bins[b].backgds[sample].label+" "+unc_name] += (it->bins[b].backgds[sample].GetFactorialUncertaintyUp(unc)-1)* it->bins[b].backgds[sample].eventyield;
                  it->systDn[it->bins[b].backgds[sample].label+" "+unc_name] += (it->bins[b].backgds[sample].GetFactorialUncertaintyDn(unc)-1)* it->bins[b].backgds[sample].eventyield;
 		  foundSyst=true;
	        }
	      }
	      if (!foundSyst) sys << "-";
	    }
	  }
	  }        
       }
       ofile << sys << "------------" << std::endl;  

       for (std::map<std::string, double>::const_iterator sys=it->syst.begin();sys!=it->syst.end();++sys){
         ofile << "# "<<sys->first<<" = "<<sys->second<<"\n";
       } 
       for (std::map<std::string, double>::const_iterator sys=it->systUp.begin();sys!=it->systUp.end();++sys){
         ofile << "# "<<sys->first<<"_Up = "<<sys->second<<"\n";
       } 
       for (std::map<std::string, double>::const_iterator sys=it->systDn.begin();sys!=it->systDn.end();++sys){
         ofile << "# "<<sys->first<<"_Dn = "<<sys->second<<"\n";
       } 
       for (std::map<std::string, double>::const_iterator sys=it->stat2.begin();sys!=it->stat2.end();++sys){
         ofile << "# "<<sys->first<<" = "<<sqrt(sys->second)<<"\n";
       } 
	
       ofile.close();
       print_=false;
    }
}
//=========================================================================================================
//=========================================================================================================
//=========================================================================================================
//=========================================================================================================

void points::WriteTable(std::ostream& os, Table::TableStyle style, unsigned p)
{
   if (p>=p_.size()) return;
   using Table::TTable;
   TTable out("Observed and expected event yields");
   std::string captionstr = "Given systematics are simple quadric-sums, and not what is used for limit calculation where the correlations are considered. Signal parameters are: ";
   for (std::map<std::string,std::string>::iterator it=p_[p].cmssm.params.begin(); it!=p_[p].cmssm.params.end(); ++it)
      captionstr += it->first+" = " +it->second +", ";
   captionstr += "Luminosity = " + ToString(p_[p].lumi);
   out.SetCaption( captionstr );
   out.SetStyle(style);
   out.SetDelimiter("|");
   out.AddColumn<string>( "Samples" );
   for (int bin=0; bin<(int)p_[p].bins.size(); ++bin)
      out.AddColumn<string>( p_[p].bins[bin].label );
   // add signal
   out << p_[p].bins[0].signal.label;
   for (int bin=0; bin<(int)p_[p].bins.size(); ++bin) {
     std::stringstream ss;
     ss<<std::fixed << std::setprecision(1) << p_[p].bins[bin].signal.eventyield;
     if (style!=Table::TeX) ss << " +- ";
     else ss <<" $\\pm$ ";
     ss<<std::fixed << std::setprecision(1)<<p_[p].bins[bin].signal.GetTotalUncertainty() << " ";
     out << ss.str();
   }
   //signal contamination  
   out << p_[p].bins[0].signal.label+" contamination";
   for (int bin=0; bin<(int)p_[p].bins.size(); ++bin) {
     std::stringstream ss;
     ss<<std::fixed << std::setprecision(1) << p_[p].bins[bin].signal.contamination;
     out << ss.str();
   }
   
   //backgrounds
   std::vector<double> total_back, total_backuncert2;
   for (int bin=0; bin<(int)p_[p].bins.size(); ++bin) {total_back.push_back(0); total_backuncert2.push_back(0);}
   for (int sample=0; sample<(int)p_[p].bins[0].backgds.size(); ++sample) {
     out << p_[p].bins[0].backgds[sample].label;
     for (int bin=0; bin<(int)p_[p].bins.size(); ++bin) {
       total_back[bin] += p_[p].bins[bin].backgds[sample].eventyield;
       total_backuncert2[bin] += pow(p_[p].bins[bin].backgds[sample].GetTotalUncertainty(),2);
       std::stringstream ss;
       ss<<std::fixed << std::setprecision(1) << p_[p].bins[bin].backgds[sample].eventyield;
       if (style!=Table::TeX) ss << " +- ";
       else ss <<" $\\pm$ ";
       ss<<std::fixed << std::setprecision(1)<<p_[p].bins[bin].backgds[sample].GetTotalUncertainty() << " ";
       out << ss.str();
     }  
   }
   out << "total background";
   for (int bin=0; bin<(int)p_[p].bins.size(); ++bin) {
     std::stringstream ss;
     ss<<std::fixed << std::setprecision(1) << total_back[bin];
     if (style!=Table::TeX) ss << " +- ";
     else ss <<" $\\pm$ ";
     ss<<std::fixed << std::setprecision(1)<< sqrt(total_backuncert2[bin]) << " ";
     out << ss.str();
   }  
   out << p_[p].bins[0].data.label;
   for (int bin=0; bin<(int)p_[p].bins.size(); ++bin) {
     std::stringstream ss;
     ss<<std::fixed << std::setprecision(1) << p_[p].bins[bin].data.eventyield << " ";
     out << ss.str();
   }  
   os << out << std::endl;
}


void points::AddData(point& p, ConfigFile * data){
    p.lumi = data  ->read<double>("lumi", p.lumi);
    std::vector<double> dat_yield = bag_of<double>(data  ->read<std::string>("selected"));
    for (int b=0; b<p.n_bins; ++b) {
      p.bins[b].data.eventyield = dat_yield[b];
      p.bins[b].data.label="data";
    }
}


void points::AddBackground(point& p, ConfigFile * cfg, const std::string& name){
    double lumiscale = p.lumi / cfg->read<double>("luminosity", p.lumi);
    std::vector<double> yield = bag_of<double>(cfg->read<std::string>(name+" background"));
    int n_nuis = cfg->read<int>("nuisances",1);
    std::vector<string> nuisance_label = bag_of_string(cfg->read<std::string>("nuisance",""));
    std::vector<string> nuisance_distr;
    std::vector< std::vector<double> > nuisUP, nuisDN;
    for (int nuis=0; nuis<n_nuis; ++nuis) {
      nuisance_distr.push_back(cfg->read<std::string>(name+" Distribution","lnN"));
      nuisDN.push_back(bag_of<double>(cfg->read<std::string>(name+" "+nuisance_label[nuis]+" DN",cfg->read<std::string>(name+" "+nuisance_label[nuis],""))) ); //use symmetric uncertainty as default
      nuisUP.push_back(bag_of<double>(cfg->read<std::string>(name+" "+nuisance_label[nuis]+" UP",cfg->read<std::string>(name+" "+nuisance_label[nuis],""))) );
      //if (nuisUP[nuis].size()!=p.bins.size() || nuisDN[nuis].size()!=p.bins.size() ){
      assert (nuisUP[nuis].size()==nuisDN[nuis].size());
      assert (nuisUP[nuis].size()==nuisUP[nuis].size());
      //keep track of all correlated and uncorrelated uncertainties
      string unc_name( name+"_"+nuisance_label[nuis] );
      std::replace(unc_name.begin(), unc_name.end(), ' ', '_');
      p.uncertainties[ unc_name+" "+nuisance_distr[nuis] ].push_back(name );
   }
   for (int i=0; i<(int)p.bins.size(); ++i) {
      point::Sample s;
      s.label = name;
      s.eventyield = yield[i] * lumiscale;
      for (int u=0; u<n_nuis; ++u){
        point::Uncertainties uncert;
	uncert.label = name+"_"+nuisance_label[u];
	uncert.distribution = nuisance_distr[u];
	uncert.up = nuisUP[u][i] * lumiscale;
	uncert.dn = nuisDN[u][i] * lumiscale;
		
	s.uncert.push_back(uncert);
      }
      p.bins[i].backgds.push_back(s);      
   }
}

void points::AddBkgTriggerUncertainty(point& p, ConfigFile * cfg, const std::string& name){
    double lumiscale = p.lumi / cfg->read<double>("luminosity", p.lumi);
    std::vector<double> yield = bag_of<double>(cfg->read<std::string>(name+" background"));
    p.uncertainties[ name+"_trigger lnN"].push_back(name);
    for (int i=0; i<(int)p.bins.size(); ++i) {
      point::Sample s;
      s.label = name;
      s.eventyield = yield[i] * lumiscale;
      point::Uncertainties uncert;
      uncert.label = name+"_trigger";
      uncert.distribution = "lnN";
      uncert.up = trigger_uncertainty * yield[i] * lumiscale;
      uncert.dn = trigger_uncertainty * yield[i] * lumiscale;
      s.uncert.push_back(uncert);
      p.bins[i].backgds.push_back(s);      
    }
}

void points::AddSignal(point& p, ConfigFile * cfg, int point_nr){
    string name = "Point "+ToString(point_nr)+" ";
    double lumiscale = p.lumi / cfg->read<double>("luminosity", p.lumi);
    p.cmssm.params["gluino"] = cfg->read<std::string>(name+"gluino mass",cfg->read<std::string>("gluino mass","-1") );
    p.cmssm.params["squark"] = cfg->read<std::string>(name+"squark mass",cfg->read<std::string>("squark mass","-1"));
    p.cmssm.params["chi1"]   = cfg->read<std::string>(name+"chi1 mass",cfg->read<std::string>("chi1 mass","-1"));
    p.cmssm.params["cha1"]   = cfg->read<std::string>(name+"cha1 mass",cfg->read<std::string>("cha1 mass","-1"));
    p.cmssm.params["bino"]   = cfg->read<std::string>(name+"bino mass",cfg->read<std::string>("bino mass",p.cmssm.params["chi1"]));
    p.cmssm.params["wino"]   = cfg->read<std::string>(name+"wino mass",cfg->read<std::string>("wino mass","-1"));
    p.cmssm.params["Scan"]   = cfg->read<std::string>("signal scan","");
    p.cmssm.params["ngenerated"]   = cfg->read<std::string>("nGen","");
    p.cmssm.params["name"]   = name;
    
    //throws exception when end of file is reached:
    std::vector<double> yield = bag_of<double>(cfg->read<std::string>(name+"number of signal events in bins"));
    
    //contamination
    std::vector<double> qcd_contamination = bag_of<double>(cfg->read<std::string>(name+"QCD prediction"));
    std::vector<double> ewk_contamination = bag_of<double>(cfg->read<std::string>(name+"EWK prediction"));
    std::vector<double> contamination;
    assert(yield.size()==p.bins.size());
    assert(qcd_contamination.size()==p.bins.size());
    assert(ewk_contamination.size()==p.bins.size());
    for (int b=0; b<p.bins.size(); ++b)
      contamination.push_back( qcd_contamination[b]+ewk_contamination[b] );

    //STAT uncertainties *must* contain "STAT" in their label string (used to separate stat and syst uncs)!

    int n_nuis = cfg->read<int>("nuisances",1);
    std::vector<string> nuisance_label = bag_of_string(cfg->read<std::string>("nuisance",";"));
    assert(n_nuis==nuisance_label.size());

    std::vector<string> nuisance_distr;
    std::vector< std::vector<double> > nuisUP, nuisDN;
    for (int nuis=0; nuis<n_nuis; ++nuis) {

      nuisance_distr.push_back(cfg->read<std::string>(nuisance_label[nuis]+" Distribution","lnN"));
      nuisDN.push_back(bag_of<double>(cfg->read<std::string>(name+nuisance_label[nuis]+" DN", cfg->read<std::string>(name+nuisance_label[nuis],""))) ); //use symmetric uncertainty as default
      nuisUP.push_back(bag_of<double>(cfg->read<std::string>(name+nuisance_label[nuis]+" UP", cfg->read<std::string>(name+nuisance_label[nuis],""))) );

      assert(p.bins.size()==nuisDN[nuis].size());
      assert(p.bins.size()==nuisUP[nuis].size());
      
      //keep track of all correlated and uncorrelated uncertainties
      string unc_name( "signal_"+nuisance_label[nuis] );
      std::replace(unc_name.begin(), unc_name.end(), ' ', '_');
      p.uncertainties[ unc_name+" "+nuisance_distr[nuis] ].push_back(name );
    }
    //p.uncertainties[ "signal_trigger lnN"].push_back( name );
    p.uncertainties[ "signal_trigger lnN"].push_back( "signal" );

    for (int i=0; i<(int)p.bins.size(); ++i) {
      p.bins[i].signal.label="signal";//name+" ("+p.cmssm.params["Scan"]+")";
      p.bins[i].signal.eventyield    = yield[i] * lumiscale;      
      p.bins[i].signal.contamination = contamination[i] * lumiscale;
      
      //uncertainties read from file
      for (int u=0; u<n_nuis; ++u){
        point::Uncertainties uncert;
	uncert.label = nuisance_label[u];
	uncert.distribution = nuisance_distr[u];
	uncert.up = nuisUP[u][i] * lumiscale;
	uncert.dn = nuisDN[u][i] * lumiscale;
	
	if ((int)p.bins[i].signal.uncert.size()<=u ) 
	  p.bins[i].signal.uncert.push_back(uncert);
	else {        
	  p.bins[i].signal.uncert[u].label = uncert.label;
	  p.bins[i].signal.uncert[u].distribution = uncert.distribution;
	  p.bins[i].signal.uncert[u].up = uncert.up;
	  p.bins[i].signal.uncert[u].dn = uncert.dn;
	}  
	std::cout << "bin:"<<i<<", unc:"<<u<< " '"<<uncert.label<<"', up="<<uncert.up<<", dn="<<uncert.dn
	          << "; Get()="<< p.bins[i].signal.GetFactorialUncertainty(u)
		  <<endl;
	

      }
      //other uncertainties - by hand
      //luminosity & trigger & data/MC scale uncertainty (affects all MC)
      point::Uncertainties uncert;
      uncert.label = "signal_trigger";
      uncert.distribution = "lnN";
      uncert.up = trigger_uncertainty * yield[i] * lumiscale;
      uncert.dn = trigger_uncertainty * yield[i] * lumiscale;
      if ((int)p.bins[i].signal.uncert.size()<=n_nuis ) p.bins[i].signal.uncert.push_back(uncert);
      else {	    
        p.bins[i].signal.uncert[n_nuis].label = uncert.label;
        p.bins[i].signal.uncert[n_nuis].distribution = uncert.distribution;
        p.bins[i].signal.uncert[n_nuis].up = uncert.up;
        p.bins[i].signal.uncert[n_nuis].dn = uncert.dn;
      }      
    }
}

void points::Read(ConfigFile& config) {
  using namespace std;
  point p;
  std::string infofile = config.read<std::string>("info file");
  std::string datafile = config.read<std::string>("data file", infofile);
  std::string bkgfile = config.read<std::string>("backgrounds file", datafile);
  std::string qcdfile = config.read<std::string>("qcd file", bkgfile);
  std::string ewkfile = config.read<std::string>("ewk file", bkgfile);
  std::string isrfile = config.read<std::string>("isr file", bkgfile);
  std::string signalfile = config.read<std::string>("signal file");

  ConfigFile info(infofile);
    p.lumi   = info.read<double>( config.read<std::string>("luminosity") );
  p.n_bins = info.read<int>( config.read<std::string>("bins"), 1 );
  for (int b=0; b<p.n_bins; ++b) {
     point::Bin bin;
     bin.label = info.read<std::string>("bin "+ToString(b), "bin "+ToString(b));
     p.bins.push_back( bin );
  }
  
  ConfigFile data( datafile );
  AddData(p, &data);

  ConfigFile qcd( qcdfile );
  AddBackground(p,&qcd,"QCD");

  ConfigFile ewk( ewkfile );
  AddBackground(p,&ewk,"EWK");

  ConfigFile isr( isrfile );
  AddBackground(p,&isr,"ISR");
  AddBkgTriggerUncertainty(p,&isr,"ISR");

  ConfigFile conf( signalfile );
  int i=0;
  do {
    p.nr = i; 
    try { AddSignal(p, &conf, i); } 
    catch (exception& e){ cerr << e.what() << endl; throw e;}    
    catch(std::string s){ cerr <<" -> catched exception in point "<<i<<": '"<<s<<"'. Stop processing of signals. "<<endl; return; }
    catch(...){           cerr <<" -> catched exception in point "<<i<<". Stop processing of signals. "<<endl; return; }
    std::cout << "...processing signal '"<< conf.read<string>("signal scan","") <<"' point "<<i++<<std::endl;
    Add(p);  
  } while (1);
}



int main( int argc, char* argv[] ) {

   ConfigFile config( (argc>1?argv[1]:"default.cfg") );

   Points.Reset();
   std::string out="limits";
   double lumiscale = 1., lumiuncscale = 1;
             
   Points.Read( config );

   if (lumiscale>1.)
     Points.ScaleLumi(lumiscale, lumiuncscale);

   Points.WriteHiggsInputStatUncorr(out,"dataCard");
   Points.WriteHiggsInputSingleChannel(out,"dataCard_sc");
   Points.WriteTable(std::cout,Table::Plain,0);

   ofstream ofile;
   ofile.open("Table.tex");
   Points.WriteTable(ofile,Table::TeX, 0);
   ofile.close();

   return 0;
}
