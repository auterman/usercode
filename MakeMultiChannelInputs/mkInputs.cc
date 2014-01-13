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

#include "table.h"
#include "ConfigFile.h"

/*
 * COMMENTS on used datasets, scans, etc.
 *
 *
 *
 */

//LUMI
const double luminosity = 19789.301;
const double luminosityUncertainty = 1.022; //lumi for 2013, 29.11.2013

const std::string WinoScanString = "Wino";  //sub-string in signalAcceptance input file, specifying if the number of generated events has to be set manually (because of generator cuts).
const std::string T1lgScanString = "T1lg";
const std::string T1ggScanString = "T1gg";


std::string ToString(double d, std::string s=""){
  if (d==0&&s!="") return s;
  std::stringstream ss;
  ss << d;
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
	double u_jes;      //signal jec
	double u_scaleDataMC; //signal photon data MC scale
	double u_lumi;
	double u_qcd;
	double u_qcd_stat;
	double u_ewk;
	double u_ewk_stat;
	double u_fsr;
	double u_fsr_stat;
	};
	std::vector<bin> bins;


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
	double u_jes;
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
	point* Get(double gl, double sq, double chi, double cha=0) {
		for (std::vector<point>::iterator it = p_.begin(); it != p_.end(); ++it)
			if ((gl==5000||gl==-1||it->gluino == gl) && (sq==5000||sq==-1||it->squark == sq) && it->chi == chi && (cha==0 || it->cha == cha))
				return &(*it);
		return 0;
	}
	std::vector<point*> GetPointsWithSameMass(double gl, double sq) {
		std::vector<point*> selectedPoints;
			for (std::vector<point>::iterator it = p_.begin(); it != p_.end(); ++it){
				//std::cout<<" Search:"<<gl<<" - "<<sq<<std::endl;
				//std::cout<<" Point:"<<it->gluino<<" - "<<it->squark<<std::endl;
				if ((it->gluino == gl && sq == 0 )||(gl == 0 && it->squark == sq)){
					//std::cout<<"!!!!!!!!!!!!!!!selected Point:"<<it->gluino<<" - "<<it->squark<<" - "<<it->chi<<std::endl;
					selectedPoints.push_back( &(*it));
				}
			}

			return selectedPoints;
		}
	void Add(point p) {
		p_.push_back(p);
	}
	void Reset() {
		p_.clear();
	}
	void PrintBins() {
		for (std::vector<point>::iterator it = p_.begin(); it != p_.end(); ++it) {

			for (unsigned bin = 1; bin <= it->bins.size(); ++bin) {

				std::cout << "bin no. " << bin - 1 << "content: " << it->bins[bin - 1].signal << std::endl;
			}
		}
	}
	void Write(const std::string dir) {
	        using namespace Table;
	        using namespace std;


		for (vector<point>::iterator it = p_.begin(); it != p_.end(); ++it) {

			ofstream ofile;
			stringstream ss;
			ss << dir << "_" << it->squark << "_" << it->gluino << "_"
					<< it->chi << "_"<<it->cha<<".txt";
			ofile.open(ss.str().c_str());
			
			double u_signal_exp2 =pow(it->u_pdfacc ,2) +
				             pow(it->u_sig_stat ,2) +
				             pow(luminosityUncertainty-1,2) + //lumi-err not included since correlated with backgd
				             pow(it->u_scaleDataMC-1 ,2) + //factorial
					     pow(it->u_jes-1 ,2); //factorial

			double u_signal_theo2 =   pow(it->u_NLO,2) +
				             pow(it->u_pdfxsec ,2);

			double u_acc = it->signal/(it->xsecNLO * it->lumi ) *
			               sqrt( u_signal_exp2 +u_signal_theo2 );
			ofile << "# gluino = " << it->gluino << "\n";
			ofile << "# squark = " << it-> squark << "\n";
			ofile << "# chi1 = " << it-> chi << "\n";
			ofile << "# cha1 = " << it-> cha << "\n";
			ofile << "# Xsection.LO = " << it-> xsec << "\n";
			ofile << "# Xsection.NLO = " << it-> xsecNLO << "\n";
			ofile << "# Luminosity = " << it-> lumi << "\n";
			ofile << "# signal.scale.uncertainty = " << it->u_NLO << "   #(relative)\n";
			ofile << "# signal.scale.uncertainty.UP = " << it->u_NLO_Up << "   #(relative)\n";
			ofile << "# signal.scale.uncertainty.DN = " << it->u_NLO_Dn << "   #(relative)\n";
			ofile << "# signal.PDF.uncertainty = " << it->u_pdfxsec << "   #(relative)\n";
			ofile << "# signal.PDFacc.uncertainty = " << it-> u_pdfacc << "   #(relative)\n";
			ofile << "# signal.ngen = " << it->totalGenerated << "\n";
			ofile << "# signal.acceptance = " << it->signal/(it->xsecNLO * it->lumi ) << "\n";
			ofile << "# signal.acceptance.uncertainty = " << u_acc << "\n";
			ofile << "# signal.Stat.uncertainty = " << it->u_sig_stat << "\n";
			ofile << "# qcd = " << it->bgd_qcd << "\n";
			ofile << "# ewk = " << it->bgd_ewk << "\n";
			ofile << "# fsr = " << it->bgd_fsr << "\n";
			ofile << "# background = " << it->bgd_qcd + it->bgd_ewk + it->bgd_fsr << "\n";
			ofile << "# u_qcd = "      << it->u_qcd  << "\n";
			ofile << "# u_qcd_stat = " << it->u_qcd_stat << "\n";
			ofile << "# u_ewk = "      << it->u_ewk  << "\n";
			ofile << "# u_ewk_stat = " << it->u_ewk_stat << "\n";
			ofile << "# u_fsr = "      << it->u_fsr  << "\n";
			ofile << "# u_fsr_stat = " << it->u_fsr_stat << "\n";
			ofile << "# background.uncertainty = " << (it->bgd_qcd + it->bgd_ewk + it->bgd_fsr)*sqrt(pow(it->u_qcd,2)+pow(it->u_qcd_stat,2)+pow(it->u_ewk,2)+pow(it->u_ewk_stat,2)+pow(it->u_fsr,2)+pow(it->u_fsr_stat,2)) << "\n";
			ofile << "# data = " << it-> data << "\n";
			ofile << "# signal = " << it->signal << "\n";
			ofile << "# signal.qcd.contamination = " << it->qcd_contamination << "\n";
			ofile << "# signal.ewk.contamination = " << it->ewk_contamination << "\n";
			ofile << "# signal.contamination = " << it->qcd_contamination + it->ewk_contamination << "\n";
                        ///some rough by-hand calculation of 'R' to pre-define (and check) the search range:
			//double u_back2 = pow(it->u_qcd,2)+pow(it->u_qcd_stat,2)+pow(it->u_ewk,2)+pow(it->u_ewk_stat,2)+pow(it->u_fsr,2)+pow(it->u_fsr_stat,2);			
			//double R = sqrt(it->data + u_signal_exp2*it->signal*it->signal + u_back2*(it->bgd_qcd + it->bgd_ewk + it->bgd_fsr)*(it->bgd_qcd + it->bgd_ewk + it->bgd_fsr))/(it->signal-it->qcd_contamination-it->ewk_contamination);
			//ofile << "# s = " << it->signal << "+-"<< it->signal*sqrt(u_signal_exp2) <<"\n";
			//ofile << "# b = " << (it->bgd_qcd + it->bgd_ewk + it->bgd_fsr)<< "+-"<< sqrt(u_back2)*(it->bgd_qcd + it->bgd_ewk + it->bgd_fsr)<<"\n";
			//ofile << "# R = "         << R/2.    << "\n";
			//ofile << "# R_m2sigma = " << R/8. << "\n";
			//ofile << "# R_p2sigma = " << 2.*R << "\n";
			///---
			int n_channels    = it->bins.size();
			int n_backgrounds = 3;
			int n_nuisance    = 5 + n_channels*4; //systs & stats
			double d=0,b=0,s=0,cont=0,R,Rmin=9999999999999;
			for (int bin=1; bin<=n_channels; ++bin){
			  d+=it->bins[bin-1].data;
			  b+=it->bins[bin-1].bgd_qcd + it->bins[bin-1].bgd_ewk + it->bins[bin-1].bgd_fsr;
			  s+=it->bins[bin-1].signal;
			  cont+=it->bins[bin-1].qcd_contamination + it->bins[bin-1].ewk_contamination;
			  double unc2= it->bins[bin-1].data;
      			  if (it->bins[bin-1].u_scaleDataMC) unc2 += pow(it->bins[bin-1].u_scaleDataMC-1.,2);
      			  if (it->bins[bin-1].u_jes)      unc2 += pow(it->bins[bin-1].u_jes-1.,2);
      			  if (it->bins[bin-1].u_pdfacc)   unc2 += pow(it->bins[bin-1].u_pdfacc-1.,2);
			  if (it->bins[bin-1].u_lumi)     unc2 += pow(it->bins[bin-1].u_lumi-1,2);
			  if (it->bins[bin-1].u_qcd)      unc2 += pow(it->bins[bin-1].u_qcd-1,2);
			  if (it->bins[bin-1].u_ewk)      unc2 += pow(it->bins[bin-1].u_ewk-1,2);
			  if (it->bins[bin-1].u_fsr)      unc2 += pow(it->bins[bin-1].u_fsr-1,2);
			  if (it->bins[bin-1].u_sig_stat) unc2 += pow(it->bins[bin-1].u_sig_stat-1,2);
			  if (it->bins[bin-1].u_qcd_stat) unc2 += pow(it->bins[bin-1].u_qcd_stat-1,2);
			  if (it->bins[bin-1].u_ewk_stat) unc2 += pow(it->bins[bin-1].u_ewk_stat-1,2);
			  if (it->bins[bin-1].u_fsr_stat) unc2 += pow(it->bins[bin-1].u_fsr_stat-1,2);
			  R=2.*sqrt(unc2)/it->bins[bin-1].signal;
			  if (R<Rmin) Rmin=R;	  
			}
			ofile << "# R_firstguess = " << Rmin << "\n";
			ofile << "## data = " << d << "\n";
			ofile << "## background = " << b << "\n";
			ofile << "## signal = " << s << "\n";
			ofile << "## signal.contamination = " << cont << "\n";
			ofile << "## acceptance = " << s/(it->lumi * it->xsecNLO ) << "\n";

			ofile << "imax " << setw(2) << n_channels    << "  number of channels" << endl;
			ofile << "jmax " << setw(2) << n_backgrounds << "  number of backgrounds" << endl;
			ofile << "kmax " << setw(2) << n_nuisance    << "  number of nuisance parameters (sources of systematic uncertainties)" << endl;
			ofile << "------------" << endl;  

                        //observed events in all channels
			TTable observed("## observed events");\
			observed.SetStyle(Empty);
			observed.SetDelimiter("  ");
			observed.AddColumn<string>(""); for (int bin=1; bin<=n_channels; ++bin) observed.AddColumn<int>("");
			observed << "bin";         for (int bin=1; bin<=n_channels; ++bin) observed << bin;
			observed << "observation"; 
			for (int bin=1; bin<=n_channels; ++bin) {
			  stringstream ss;
			  ss << "n" << bin;
			  observed << (int)it->bins[bin-1].data;
			}  
			ofile << observed << "------------\n" << endl;  

			//expected events in all channels for signal and all backgrounds
			TTable exp("## expected events");
			exp.SetStyle(Empty);
			exp.SetDelimiter("  ");
			exp.AddColumn<string>(""); 
			exp.SetMinumumWidth(20,0);//make first column at least 20 chars
			  for (int bin=1; bin<=n_channels; ++bin) 
			    for (int sample=1; sample<=n_backgrounds+1; ++sample) 
        		      exp.AddColumn<string>("");
			exp << "bin"; 
			  for (int bin=1; bin<=n_channels; ++bin)
			    for (int sample=1; sample<=n_backgrounds+1; ++sample) {
        		       stringstream ss;
        		       ss << bin;
        		       exp << ss.str();
			    }
			exp << "process";
			for (int bin=1; bin<=n_channels; ++bin) {
			  exp << "signal" << "qcd" << "ewk" << "fsr";
			}  
			exp << "process"; 
			for (int bin=1; bin<=n_channels; ++bin) 
			    for (int sample=1; sample<=n_backgrounds+1; ++sample) {
        		       stringstream ss;
        		       ss << (sample-1);
        		       exp << ss.str();
			    }	
			exp << "rate"; 
			for (int bin=1; bin<=n_channels; ++bin) {
			  exp << ToString(it->bins[bin-1].signal  
			                  - it->bins[bin-1].qcd_contamination 
					  - it->bins[bin-1].ewk_contamination )
			      << ToString(it->bins[bin-1].bgd_qcd)
			      << ToString(it->bins[bin-1].bgd_ewk)
			      << ToString(it->bins[bin-1].bgd_fsr);
			      
			}  
			ofile << exp << "------------" << std::endl;  
			
			TTable sys("");
			sys.SetStyle(Empty);
			sys.SetDelimiter("  ");
			sys.AddColumn<string>(""); 
			sys.SetMinumumWidth(20,0);//make first column at least 20 chars
			  for (int bin=1; bin<=n_channels; ++bin) 
			    for (int sample=1; sample<=n_backgrounds+1; ++sample) 
        		      sys.AddColumn<string>("");
			sys << "U_Sig lnN";
 		        for (int b=0; b<n_channels; ++b) {
			  //Signal total systmatic uncertainty not including statistical and lumi:
			  double u_sig = 1.0 + sqrt( //pow(it->bins[b].u_NLO-1.,2) +
      			                             //pow(it->bins[b].u_pdfxsec-1.,2) +
      			                             pow(it->bins[b].u_scaleDataMC-1.,2) +
      			                             pow(it->bins[b].u_jes-1.,2) +
      			                             pow(it->bins[b].u_pdfacc-1.,2) );
        		  sys << ToString(u_sig,"-") // signal
			      << "-" << "-" << "-"; //qcd, ewk, fsr
			}      
			sys << "U_Lumi lnN";
 		        for (int b=0; b<n_channels; ++b) 
        		  sys << ToString(it->bins[b].u_lumi,"-") // signal
			      << "-" // qcd data-driven
			      << "-" // ewk data-driven
			      << ToString(it->bins[b].u_lumi,"-");//fsr
			sys << "U_qcd lnN";
 		        for (int b=0; b<n_channels; ++b) 
        		  sys << "-" // signal
			      << ToString(it->bins[b].u_qcd,"-") << "-" << "-"; //qcd, ewk, fsr
			sys << "U_ewk lnN";
 		        for (int b=0; b<n_channels; ++b) 
        		  sys << "-" // signal
			      << "-" << ToString(it->bins[b].u_ewk,"-") << "-"; //qcd, ewk, fsr
			sys << "U_fsr lnN";
 		        for (int b=0; b<n_channels; ++b) 
        		  sys << "-" // signal
			      << "-" << "-" << ToString(it->bins[b].u_fsr,"-"); //qcd, ewk, fsr
			      
			//Now the statistical uncertainties:...................................
			//signal-statistical
			for (int bin=0; bin<n_channels; ++bin) {   
			  stringstream ss;
			  ss << "U_signal_statistic_bin"<<bin<<" lnN";
			  sys << ss.str(); 
 		          for (int b=0; b<n_channels; ++b) {
			    if (bin==b) sys << ToString(it->bins[b].u_sig_stat,"-"); 
			    else        sys << "-";
			    sys << "-" << "-" << "-"; // qcd, ewk, fsr
			  }
			}      
			//qcd-statistical
			for (int bin=0; bin<n_channels; ++bin) {    
			  stringstream ss;
			  ss << "U_qcd_statistic_bin"<<bin<<" lnN";
			  sys << ss.str(); 
 		          for (int b=0; b<n_channels; ++b) {
   		            sys << "-";//signal
			    if (bin==b) sys << ToString(it->bins[b].u_qcd_stat,"-");
			    else        sys << "-";
			    sys << "-" << "-"; //ewk, fsr
			  }
			}      
			//ewk-statistical
			for (int bin=0; bin<n_channels; ++bin) {    
			  stringstream ss;
			  ss << "U_ewk_statistic_bin"<<bin<<" lnN";
			  sys << ss.str(); 
 		          for (int b=0; b<n_channels; ++b){
			    sys <<"-"<< "-";//signal, qcd 
   		            if (bin==b) sys << ToString(it->bins[b].u_ewk_stat,"-");
			    else        sys << "-";
			    sys << "-"; //fsr
			  }  		
			}      
			//fsr-statistical
			for (int bin=0; bin<n_channels; ++bin) {    
			  stringstream ss;
			  ss << "U_fsr_statistic_bin"<<bin<<" lnN";
			  sys << ss.str(); 
 		          for (int b=0; b<n_channels; ++b){
			    sys <<"-"<< "-"<<"-";//signal, qcd, ewk 
   		            if (bin==b) sys << ToString(it->bins[b].u_fsr_stat,"-");
			    else        sys << "-";
			  }
			}      
			ofile << sys << "------------" << std::endl;  
			//cout << "WRITE GL:" << it->gluino << ", SQ:" << it->squark << endl;
			ofile.close();
		}
	}	

//=====================================================================================================
//=====================================================================================================
//=====================================================================================================

	void WriteSingleBin(const std::string dir) {
	        using namespace Table;
	        using namespace std;
	       // std::cout<<"write single bins:"<<p_.size()<<endl;
		for (vector<point>::iterator it = p_.begin(); it != p_.end(); ++it) {
		   //std::cout << "point " <<it-p_.begin()<<", bins = "<<it->bins.size() << std::endl;
 		   for (int bin=0; bin<(int)it->bins.size(); ++bin){
			ofstream ofile;
			stringstream ss;
			ss << dir << "_" << it->squark << "_" << it->gluino << "_"
					<< it->chi << "_bin"<<bin<<".txt";
			ofile.open(ss.str().c_str());
			ofile << "# gluino = " << it->gluino << "\n";
			ofile << "# squark = " << it-> squark << "\n";
			ofile << "# chi1 = " << it-> chi << "\n";
			ofile << "# cha1 = " << it-> cha << "\n";
			ofile << "# Xsection.LO = " << it-> xsec << "\n";
			ofile << "# Xsection.NLO = " << it-> xsecNLO << "\n";
			ofile << "# Luminosity = " << it-> lumi << "\n";
			ofile << "# channel = "<<bin<<"\n";
			int n_backgrounds = 3;
			int n_nuisance    = 8; //systs & stats
			double d=it->bins[bin].data;
			double b=it->bins[bin].bgd_qcd + it->bins[bin].bgd_ewk + it->bins[bin].bgd_fsr;
			double s=it->bins[bin].signal;
			double cont=it->bins[bin].qcd_contamination + it->bins[bin].ewk_contamination;
			  double u_sig = 1.0 + sqrt( //pow(it->bins[b].u_NLO-1.,2) +
			                             pow(it->bins[bin].u_sig_stat-1,2) +
      			                             //pow(it->bins[bin].u_pdfxsec-1.,2) +
        			                     pow(it->bins[bin].u_scaleDataMC-1.,2) +
      			                             pow(it->bins[bin].u_jes-1.,2) +
      			                             pow(it->bins[bin].u_pdfacc-1.,2) );
			ofile << "# data = " << d << "\n";
			ofile << "# background = " << b << "\n";
			ofile << "# signal = " << s << "\n";
			ofile << "# signal.contamination = " << cont << "\n";
			ofile << "# signal.acceptance = " << s/(it->lumi * it->xsecNLO ) << "\n";
			ofile << "# signal.acceptance.uncertainty = " << (u_sig-1.0) * s/(it->lumi * it->xsecNLO )  << "\n";

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
			sys << "U_ewk_statistic lnN" <<"-"<< "-"<< ToString(it->bins[b].u_ewk_stat,"-")<< "-"; 
			sys << "U_fsr_statistic lnN" <<"-"<< "-"<<"-"<< ToString(it->bins[b].u_fsr_stat,"-");
			ofile << sys << "------------" << std::endl;  
			//cout << "WRITE GL:" << it->gluino << ", SQ:" << it->squark << endl;

			ofile.close();
		    }	

		}


	}
	std::vector<point> * Get(){return &p_;}
private:
	std::vector<point> p_;
} Points;




//// READ /////// -----------------------------------------------------------------------------


//New
void ReadSignal(std::string sig_file, std::string dat_file="", std::string fsr_file="") {
  ConfigFile * cfg = new ConfigFile(sig_file);
  ConfigFile * dat_cfg = (dat_file==""?cfg:new ConfigFile(dat_file));
  ConfigFile * fsr_cfg = (fsr_file==""?dat_cfg:new ConfigFile(fsr_file));
  if(cfg==0||dat_cfg==0||fsr_cfg==0)
   std::cerr << "FILE NOT FOUND! scan file= "<<cfg<<", data file="<<dat_cfg<<", isr file="<<fsr_cfg<<std::endl;
  int n=0;
  do {

    point p;
    std::stringstream ss;
    p.nr = n;
    ss << "Point " << n++;
    p.gluino = cfg->read<double>(ss.str()+" gluino mass",-1.);
    p.squark = cfg->read<double>(ss.str()+" squark mass",-1.);
    p.chi    = cfg->read<double>(ss.str()+" NLSP mass",-1.);    
    p.chi    = cfg->read<double>(ss.str()+" bino mass",p.chi); 
    p.cha    = cfg->read<double>(ss.str()+" wino mass",-1);   
    
    if (p.gluino==-1 && p.squark==-1 && p.chi==-1 && p.cha==-1) break;  

    p.lumi = luminosity;
    p.signal = p.bgd_qcd = p.bgd_ewk= p.bgd_fsr = p.qcd_contamination = p.ewk_contamination = p.data = p.u_NLO = 0;
    p.u_pdfxsec = p.u_pdfacc = p.u_sig_stat = p.u_lumi = p.u_qcd = p.u_qcd_stat= p.u_ewk= p.u_ewk_stat= p.u_fsr= p.u_fsr_stat=0;
    std::vector<double> data       = bag_of<double>(dat_cfg->read<std::string>("selected"));
    std::vector<double> qcd        = bag_of<double>(dat_cfg->read<std::string>("QCD background"));
    std::vector<double> u_qcd      = bag_of<double>(dat_cfg->read<std::string>("QCD syst error"));
    std::vector<double> u_qcd_stat = bag_of<double>(dat_cfg->read<std::string>("QCD stat error"));
    std::vector<double> ewk        = bag_of<double>(dat_cfg->read<std::string>("EWK background"));
    std::vector<double> u_ewk      = bag_of<double>(dat_cfg->read<std::string>("EWK syst error"));
    std::vector<double> u_ewk_stat = bag_of<double>(dat_cfg->read<std::string>("EWK stat error"));

    std::vector<double> fsr        = bag_of<double>(fsr_cfg->read<std::string>("ISR background"));
    std::vector<double> u_fsr      = bag_of<double>(fsr_cfg->read<std::string>("ISR syst error"));
    std::vector<double> u_fsr_stat = bag_of<double>(fsr_cfg->read<std::string>("ISR stat error"));

    std::vector<double> sig        = bag_of<double>(cfg->read<std::string>(ss.str()+" number of signal events in bins"));
    std::vector<double> u_sig      = bag_of<double>(cfg->read<std::string>(ss.str()+" statistical error of signal events in bins"));
    std::vector<double> sig_qcd    = bag_of<double>(cfg->read<std::string>(ss.str()+" QCD prediction"));
    std::vector<double> sig_ewk    = bag_of<double>(cfg->read<std::string>(ss.str()+" EWK prediction"));
    int n_channels = data.size();
    if ((int)qcd.size()!=n_channels ||(int)ewk.size()!=n_channels ||(int)fsr.size()!=n_channels ||
        (int)sig.size()!=n_channels || (int)sig_ewk.size()!=n_channels) {
      std::cerr << "ERROR: inconstitent number of channels at signalpoint "<<n-1
                << "; data=" << data.size()
                << "; qcd=" << qcd.size()
                << "; ewk=" << ewk.size()
                << "; fsr=" << fsr.size()
                << "; sig=" << sig.size()
		<< "; sig_ewk="<<sig_ewk.size()
		<<std::endl;
      continue;
    }


    double scaleDataMC = 1.0;   //scale
    p.u_jes = 1.02;             //factorial uncertainty
    p.u_scaleDataMC = 1.04;     //factorial uncertainty
    
    for (int c=0;c<n_channels;++c){

      point::bin channel;

      channel.signal             = sig[c] * scaleDataMC;//This is in <number of generated events>, needs to be
                                                         //weighted according to Lumi*xsec/n_generated, do that 

      channel.bgd_qcd            = qcd[c];
      channel.bgd_ewk            = ewk[c];
      channel.bgd_fsr            = fsr[c];

      channel.qcd_contamination  = sig_qcd[c] * scaleDataMC;
      channel.ewk_contamination  = sig_ewk[c] * scaleDataMC;
      channel.data               = data[c];

      //store uncertainties factorial, i.e. 1 for no unc, 1.1 for 10% unc, 2.0 for 100% etc:
      channel.u_sig_stat         = (sig[c] ?           1.+ u_sig[c]/sig[c]:0);
      channel.u_lumi             = luminosityUncertainty;

      channel.u_qcd              = (channel.bgd_qcd?	   u_qcd[c]/channel.bgd_qcd	  :0);
      channel.u_qcd_stat         = (channel.bgd_qcd?   1.+ u_qcd_stat[c]/channel.bgd_qcd  :0);
      channel.u_ewk              = (channel.bgd_ewk?       u_ewk[c]/channel.bgd_ewk       :0);
      channel.u_ewk_stat         = (channel.bgd_ewk?   1.+ u_ewk_stat[c]/channel.bgd_ewk  :0);
      channel.u_fsr              = (channel.bgd_fsr?   1.+ u_fsr[c]/channel.bgd_fsr:0);
      channel.u_fsr_stat         = (channel.bgd_fsr?   1.+ u_fsr_stat[c]/channel.bgd_fsr  :0);

      channel.u_jes = p.u_jes;
      channel.u_scaleDataMC = p.u_scaleDataMC;
      
      //std::cout<<c<<": s="<<channel.signal<< ", s (w/o cont)="<<channel.signal-channel.qcd_contamination-channel.ewk_contamination<<std::endl;

      if (channel.signal-channel.qcd_contamination-channel.ewk_contamination>0. ) {
        p.bins.push_back( channel );
      
	p.signal  += channel.signal;
	p.bgd_qcd += channel.bgd_qcd;	
	p.bgd_ewk += channel.bgd_ewk;
	p.bgd_fsr += channel.bgd_fsr;
	p.qcd_contamination += channel.qcd_contamination;
	p.ewk_contamination += channel.ewk_contamination;
	p.data += channel.data;
	p.u_qcd      += u_qcd[c];
	p.u_qcd_stat += (qcd[c]>0.?pow(u_qcd_stat[c], 2):0);
	p.u_ewk      += u_ewk[c];
	p.u_ewk_stat += (ewk[c]>0.?pow(u_ewk_stat[c], 2):0);
	p.u_fsr      += u_fsr[c];
	p.u_fsr_stat += (fsr[c]>0.?pow(u_fsr_stat[c], 2):0); 
      }
  }

  //store total uncertainties per point *relative*, e.g. u_qcd=0.10 means 10% on qcd:
  p.u_lumi      = (luminosityUncertainty-1.)*luminosity;
  p.u_sig_stat  = sqrt(p.signal)/p.totalGenerated; 
  p.u_qcd       = p.u_qcd / p.bgd_qcd - 1;
  p.u_qcd_stat  = sqrt(p.u_qcd_stat) / p.bgd_qcd;
  p.u_ewk       = p.u_ewk / p.bgd_ewk - 1;
  p.u_ewk_stat  = sqrt(p.u_ewk_stat) / p.bgd_ewk;
  p.u_fsr       = p.u_fsr / p.bgd_fsr;
  p.u_fsr_stat  = sqrt(p.u_fsr_stat) / p.bgd_fsr;
  //std::cout << "acc "<<n-1<<"  gluino = "<<p.gluino<<"  squark = "<<p.squark<<"  chi = "<<p.chi <<std::endl;
    
    Points.Add(p);
  } while(1);

  if (fsr_cfg!=dat_cfg) delete fsr_cfg;
  if (dat_cfg!=cfg) delete dat_cfg;
  delete cfg;
}

void AddSMSXsec(std::string filelist) {
	std::cout << "READ XS---------file:" << filelist << std::endl;
	std::ifstream masses_file;
	masses_file.open(filelist.c_str());
	std::string file;
	point p;
	p.lumi = luminosity;
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
	  	 	     a->signal     *= luminosity*p.xsecNLO/a->totalGenerated;
	  	 	     //a->u_sig_stat *= luminosity*p.xsecNLO/a->totalGenerated;
	  	 	     a->qcd_contamination  *= luminosity*p.xsecNLO/a->totalGenerated;
	  	 	     a->ewk_contamination  *= luminosity*p.xsecNLO/a->totalGenerated;
	  	 	     a->u_NLO       = p.u_NLO;
	  	 	     a->u_NLO_Up    = p.u_NLO_Up;
	  	 	     a->u_NLO_Dn    = p.u_NLO_Dn;
	  		     a->u_pdfxsec   = p.u_pdfxsec;
			     a->u_pdfacc    = p.u_pdfacc;
           	             for (std::vector<point::bin>::iterator bin=a->bins.begin(); bin!=a->bins.end(); ++bin) {
	  	 	       bin->signal     *= luminosity*p.xsecNLO/a->totalGenerated;
	  	 	       //bin->u_sig_stat *= luminosity*p.xsecNLO/a->totalGenerated;
	  	 	       bin->qcd_contamination *= luminosity*p.xsecNLO/a->totalGenerated;
	  	 	       bin->ewk_contamination *= luminosity*p.xsecNLO/a->totalGenerated;
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
	p.lumi = luminosity;
	double LO_dn, LO_up, NLO_up, NLO_dn;
	while (1) {
	   masses_file >> p.squark >> p.gluino >> p.chi >> p.xsec >> LO_up >> LO_dn >> p.xsecNLO
	   	       >> NLO_up >> NLO_dn;
	   if (!masses_file.good()) break;
	   
	   //std::cout << "READ XSEC:" << p.xsecNLO << std::endl;
	   //std::cout << "sq" << p.squark << std::endl;
	   //std::cout << "gl" << p.gluino << std::endl;
	   //std::cout << "neutr" << p.chi << std::endl;
	   
	   point * a = 0;
	   a = Points.Get(p.gluino, p.squark, p.chi);
	   if (a && !a->u_NLO){
	     a->xsec        = p.xsec;
	     a->xsecNLO     = p.xsecNLO;
	     a->signal     *= luminosity*p.xsecNLO/a->totalGenerated;
	     //a->u_sig_stat *= luminosity*p.xsecNLO/a->totalGenerated;
	     a->qcd_contamination  *= luminosity*p.xsecNLO/a->totalGenerated;
	     a->ewk_contamination  *= luminosity*p.xsecNLO/a->totalGenerated;
	     a->u_NLO       = NLO_up / p.xsecNLO;
	     a->u_NLO_Up    = NLO_up / p.xsecNLO;
	     a->u_NLO_Dn    = NLO_dn / p.xsecNLO;
	     for (std::vector<point::bin>::iterator bin=a->bins.begin(); bin!=a->bins.end(); ++bin) {
	       bin->signal     *= luminosity*p.xsecNLO/a->totalGenerated;
	       //bin->u_sig_stat *= luminosity*p.xsecNLO/a->totalGenerated;
	       bin->qcd_contamination *= luminosity*p.xsecNLO/a->totalGenerated;
	       bin->ewk_contamination *= luminosity*p.xsecNLO/a->totalGenerated;
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
	     a->signal     *= luminosity*p.xsecNLO/p.totalGenerated;
	     //a->u_sig_stat *= luminosity*p.xsecNLO/p.totalGenerated;
	     a->qcd_contamination  *= luminosity*p.xsecNLO/p.totalGenerated;
	     a->ewk_contamination  *= luminosity*p.xsecNLO/p.totalGenerated;
	     a->u_NLO       = fabs( NLO_up / p.xsecNLO );
	     a->u_NLO_Up    = fabs( NLO_up / p.xsecNLO );
	     a->u_NLO_Dn    = fabs( NLO_dn / p.xsecNLO );
	     a->totalGenerated = p.totalGenerated;
	     for (std::vector<point::bin>::iterator bin=a->bins.begin(); bin!=a->bins.end(); ++bin) {
	       bin->signal     *= luminosity*p.xsecNLO/p.totalGenerated;
	       //bin->u_sig_stat *= luminosity*p.xsecNLO/p.totalGenerated;
	       bin->qcd_contamination *= luminosity*p.xsecNLO/p.totalGenerated;
	       bin->ewk_contamination *= luminosity*p.xsecNLO/p.totalGenerated;
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

int main(int argc, char* argv[]) {
/*
   //New Paper 2012 Scan: ///////// Bin --- Wino /////////////////////////////////////////////////////////////////////// 
   //2-jets
   Points.Reset();
   ReadSignalAcceptance("2j","inputWinter11/signalAcceptanceBinoWinoWinter11.dat", "inputWinter11/data_Full2011.txt");
   Add_WB_NewXsec("inputWinter11/Spectra_WB.xsec");
   AddPDFs("inputWinter11/Spectra_WB_phad_pdfuncert.dat");
   {points MergedPoints;
   for (std::vector<point>::iterator it=Points.Get()->begin(); it!=Points.Get()->end(); ++it)
      MergedPoints.Add( *MergeBins(*it, 6));
   std::system("mkdir GMSB_WinoBino_2j");
   std::system("mkdir GMSB_WinoBino_SingleChannels2j");
   MergedPoints.Write("GMSB_WinoBino_2j/GMSB");
   MergedPoints.WriteSingleBin("GMSB_WinoBino_SingleChannels2j/GMSB");
   }


  Points.Reset();
   ReadSignalAcceptance("2j","inputWinter11/signalAcceptanceWino_preARC20120208.dat", "inputWinter11/data_Full2011.txt");
   AddXsec("inputWinter11/wino375NLOxsec2_Dec1.dat");
   AddPDFxsec("inputWinter11/PDFcross.txt", 375);
   AddPDFAcceptance("inputWinter11/PDFacceptance.txt", 375);
   {points MergedPoints;
   for (std::vector<point>::iterator it=Points.Get()->begin(); it!=Points.Get()->end(); ++it)
      MergedPoints.Add( *MergeBins(*it, 6));
   std::system("mkdir GMSBWino375Neutr2j");
   std::system("mkdir GMSBWino375NeutrSingleChannels2j");
   MergedPoints.Write("GMSBWino375Neutr2j/GMSB");
   MergedPoints.WriteSingleBin("GMSBWino375NeutrSingleChannels2j/GMSB");
   }
*/   

   Points.Reset();
   ReadSignal("inputWinter13/SqGl-Wino.txt", "inputWinter13/mc_estimation.txt");
   AddXsec("Xsecs/NLOProspinoXsecs_Wino_Neutr375.txt");
   AddPDFxsec("PDF/PDFcross.txt", 375);
   AddPDFAcceptance("PDF/PDFacceptance.txt", 375);
   {points MergedPoints;
   for (std::vector<point>::iterator it=Points.Get()->begin(); it!=Points.Get()->end(); ++it)
      MergedPoints.Add( *MergeBins(*it, 6));
   std::system("mkdir GMSB_SqGl-Wino");
   std::system("mkdir GMSB_SqGl-Wino_SingleChannels");
   MergedPoints.Write("GMSB_SqGl-Wino/GMSB");
   MergedPoints.WriteSingleBin("GMSB_SqGl-Wino_SingleChannels/GMSB");
   }
   
}
