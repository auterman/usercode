#include "mkInputs.h"
#include "table.h"
#include "ConfigFile.h"

/*
 * COMMENTS on used datasets, scans, etc.
 *
 *
 *
 */

point* points::Get(double gl, double sq, double chi, double cha) {
   for (std::vector<point>::iterator it = p_.begin(); it != p_.end(); ++it){
//std::cout<<"   Get gl="<<it->gluino<<", sq="<<it->squark<<", chi="<<it->chi<<", cha="<<it->cha
//         <<std::endl;

    	    if ((gl==5000||gl==-1||it->gluino == gl) && 
    		(sq==5000||sq==-1||it->squark == sq) && 
    		(chi==5000||it->chi==chi) && 
    		(cha==5000||cha==2000||cha==0||it->cha == cha)
    	       )
    		    return &(*it);
    }
    return 0;
}

std::vector<point*> points::GetGl(double gl) {
   std::vector<point*> res;
   for (std::vector<point>::iterator it = p_.begin(); it != p_.end(); ++it){

    	    if (it->gluino == gl) 
    		    res.push_back(&(*it));
    }
    return res;
}

std::vector<point*> points::GetPointsWithSameMass(double gl, double sq) {
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

void points::PrintBins() {
   for (std::vector<point>::iterator it = p_.begin(); it != p_.end(); ++it) {

   	   for (unsigned bin = 1; bin <= it->bins.size(); ++bin) {

   		   std::cout << "bin no. " << bin - 1 << "content: " << it->bins[bin - 1].signal << std::endl;
   	   }
   }
}


void points::PrintGlobal(std::ofstream& os, point&p)
{
    os << "# gluino = " << p.gluino << "\n";
    os << "# squark = " << p. squark << "\n";
    os << "# chi1 = " << p. chi << "\n";
    os << "# cha1 = " << p. cha << "\n";
    os << "# Xsection.LO = " << p. xsec << "\n";
    os << "# Xsection.NLO = " << p. xsecNLO << "\n";
    os << "# Luminosity = " << p. lumi << "\n";
}

void points::PrintGlobalSums(std::ofstream& os, point&p)
{
    double u_signal_exp2 =pow(p.u_pdfacc ,2) +
 	   		 pow(p.u_sig_stat ,2) +
 	   		 pow(luminosityUncertainty-1,2) + //lumi-err not included since correlated with backgd
 	   		 pow(p.u_scaleDataMC-1 ,2) + //factorial
 	   		 pow(p.u_sig-1 ,2); //factorial

    double u_signal_theo2 =   pow(p.u_NLO,2) +
 	   		 pow(p.u_pdfxsec ,2);

    double u_acc = p.signal/(p.xsecNLO * p.lumi ) *
 	   	   sqrt( u_signal_exp2 +u_signal_theo2 );
    os << "# signal.scale.uncertainty = " << p.u_NLO << "   #(relative)\n";
    os << "# signal.scale.uncertainty.UP = " << p.u_NLO_Up << "   #(relative)\n";
    os << "# signal.scale.uncertainty.DN = " << p.u_NLO_Dn << "   #(relative)\n";
    os << "# signal.PDF.uncertainty = " << p.u_pdfxsec << "   #(relative)\n";
    os << "# signal.PDFacc.uncertainty = " << p. u_pdfacc << "   #(relative)\n";
    os << "# signal.ngen = " << p.totalGenerated << "\n";
    os << "# signal.acceptance = " << p.signal/(p.xsecNLO * p.lumi ) << "\n";
    os << "# signal.acceptance.uncertainty = " << u_acc << "\n";
    os << "# signal.Stat.uncertainty = " << p.u_sig_stat << "\n";
    os << "# qcd = " << p.bgd_qcd << "\n";
    os << "# ewk = " << p.bgd_ewk << "\n";
    os << "# fsr = " << p.bgd_fsr << "\n";
    os << "# background = " << p.bgd_qcd + p.bgd_ewk + p.bgd_fsr << "\n";
    os << "# u_rel_qcd = "      << p.u_qcd  << "\n";
    os << "# u_rel_qcd_stat = " << p.u_qcd_stat << "\n";
    os << "# u_rel_ewk = "      << p.u_ewk  << "\n";
    os << "# u_rel_ewk_stat = " << p.u_ewk_stat << "\n";
    os << "# u_rel_fsr = "      << p.u_fsr  << "\n";
    os << "# u_rel_fsr_stat = " << p.u_fsr_stat << "\n";
    os << "# data = " << p. data << "\n";
    os << "# signal = " << p.signal << "\n";
    os << "# u_rel_signal_exp = " << sqrt(u_signal_exp2) << "\n";
    os << "# u_rel_signal_theo = " << sqrt(u_signal_theo2) << "\n";
    
    os << "# signal.qcd.contamination = " << p.qcd_contamination << "\n";
    os << "# signal.ewk.contamination = " << p.ewk_contamination << "\n";
    os << "# signal.contamination = " << p.qcd_contamination + p.ewk_contamination << "\n";
    
    os << "# variable = " << p.variable << "\n";
    os << "# nbins = " << p.bins.size()<<"\n";
}

void points::PrintBin(std::ofstream& os, point&p, unsigned bin, const std::string& str)
{	  
  	  os << "# channel = "<<bin<<"\n";
  	  os << "# bin = "<<bin<<"\n";
  	  os << "# bin lower limit = "<<p.bins[bin].bin_low<<"\n";
  	  os << "# bin upper limit = "<<p.bins[bin].bin_up<<"\n";
  	  double d=p.bins[bin].data;
  	  double b=p.bins[bin].bgd_qcd + p.bins[bin].bgd_ewk + p.bins[bin].bgd_fsr;
  	  double s=p.bins[bin].signal;
  	  double cont=p.bins[bin].qcd_contamination + p.bins[bin].ewk_contamination;
  	  double u_sig = 1.0 + sqrt( //pow(p.bins[b].u_NLO-1.,2) +
  				       pow(p.bins[bin].u_sig_stat-1,2) +
  				       //pow(p.bins[bin].u_pdfxsec-1.,2) +
  				       pow(p.bins[bin].u_scaleDataMC-1.,2) +
  				       pow(p.bins[bin].u_sig-1.,2) +
  				       pow(p.bins[bin].u_pdfacc-1.,2) );
  	  os << "# data = " << d << "\n";
  	  os << "# qcd = " << p.bins[bin].bgd_qcd << "\n";
  	  os << "# ewk = " << p.bins[bin].bgd_ewk << "\n";
  	  os << "# fsr = " <<  p.bins[bin].bgd_fsr << "\n";
  	  os << "# background = " << b << "\n";
  	  os << "# background names = qcd; ewk; fsr\n";
  	  os << "# signal = " << s << "\n";
  	  os << "# u_rel_signal_exp = " << (u_sig-1.0) << "\n";
  	  //os << "# u_rel_signal_theo = " << sqrt( pow(p.bins[b].u_NLO-1.,2) +
  	  //			                  pow(p.bins[bin].u_pdfxsec-1.,2)) << "\n";
  	  os << "# signal.contamination = " << cont << "\n";
  	  os << "# signal.acceptance = " << s/(p.lumi * p.xsecNLO ) << "\n";
  	  os << "# signal.acceptance.uncertainty = " << (u_sig-1.0) * s/(p.lumi * p.xsecNLO )  << "\n";

  	  os << "# u_rel_Lumi_sys = " << p.bins[bin].u_lumi-1<<"\n";
  	  os << "# u_rel_qcd_sys = "  << p.bins[bin].u_qcd-1<<"\n"; 
	  os << "# u_rel_ewk_sys = "  << p.bins[bin].u_ewk-1<<"\n";
  	  os << "# u_rel_fsr_sys = "  << p.bins[bin].u_fsr-1<<"\n";
  	  os << "# u_rel_qcd_stat = " << p.bins[bin].u_qcd_stat-1<<"\n";
  	  os << "# u_rel_ewk_stat = " << p.bins[bin].u_ewk_stat-1<<"\n";
  	  os << "# u_rel_fsr_stat = " << p.bins[bin].u_fsr_stat-1<<"\n";
  	  os << "# u_rel_qcd = "  << sqrt((p.bins[bin].u_qcd-1)*(p.bins[bin].u_qcd-1) + (p.bins[bin].u_qcd_stat-1)*(p.bins[bin].u_qcd_stat-1)) <<"\n"; 
	  os << "# u_rel_ewk = "  << sqrt((p.bins[bin].u_ewk-1)*(p.bins[bin].u_ewk-1) + (p.bins[bin].u_ewk_stat-1)*(p.bins[bin].u_ewk_stat-1)) <<"\n";
  	  os << "# u_rel_fsr = "  << sqrt((p.bins[bin].u_fsr-1)*(p.bins[bin].u_fsr-1) + (p.bins[bin].u_fsr_stat-1)*(p.bins[bin].u_fsr_stat-1)) <<"\n";

}

void points::Write(const std::string dir) {
 using namespace Table;
 using namespace std;


 for (vector<point>::iterator it = p_.begin(); it != p_.end(); ++it) {

    ofstream ofile;
    stringstream ss;
    if (dir.find("GMSB")!=std::string::npos)
      ss << dir << "_" << it->squark << "_" << it->gluino << "_" << it->chi << "_"<<it->cha<<".txt";
    else
      ss << dir << "_" << it->gluino << "_" << it->chi << ".txt";
        
    ofile.open(ss.str().c_str());
    
    PrintGlobal( ofile, *it );
    PrintGlobalSums( ofile, *it );
 
    ///some rough by-hand calculation of 'R' to pre-define (and check) the search range:
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
      if (it->bins[bin-1].u_sig)      unc2 += pow(it->bins[bin-1].u_sig-1.,2);
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

    ofile << "imax " << setw(2) << n_channels	 << "  number of channels" << endl;
    ofile << "jmax " << setw(2) << n_backgrounds << "  number of backgrounds" << endl;
    ofile << "kmax " << setw(2) << n_nuisance	 << "  number of nuisance parameters (sources of systematic uncertainties)" << endl;
    ofile << "------------" << endl;  

    //observed events in all channels
    TTable observed("## observed events");\
    observed.SetStyle(Empty);
    observed.SetDelimiter("  ");
    observed.AddColumn<string>(""); for (int bin=1; bin<=n_channels; ++bin) observed.AddColumn<std::string>("");
    observed << "bin";         for (int bin=1; bin<=n_channels; ++bin) observed << "bin_"+ToString(bin);
    observed << "observation"; 
    for (int bin=1; bin<=n_channels; ++bin) {
      //stringstream ss;
      //ss << "n" << bin;
      observed << ToString((int)it->bins[bin-1].data);
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
 	   ss << "bin_"<<bin;
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
 	   			 pow(it->bins[b].u_sig-1.,2) +
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
    	else	    sys << "-";
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
    	else	    sys << "-";
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
    	else	    sys << "-";
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
    	else	    sys << "-";
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

void points::WriteSingleBin(const std::string dir) {
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
}



//// READ /////// -----------------------------------------------------------------------------


//New
void ReadSignal(std::string sig_file, std::string dat_file="", std::string fsr_file="") {
  ConfigFile * cfg = new ConfigFile(sig_file);
  ConfigFile * dat_cfg = (dat_file==""?cfg:new ConfigFile(dat_file));
  ConfigFile * fsr_cfg = (fsr_file==""?dat_cfg:new ConfigFile(fsr_file));
  if(cfg==0||dat_cfg==0||fsr_cfg==0)
   std::cerr << "FILE NOT FOUND! scan file= "<<cfg<<", data file="<<dat_cfg<<", isr file="<<fsr_cfg<<std::endl;
  int n=0;
  
  double default_gluino = cfg->read<double>("gluino mass",-1.);
  double default_squark = cfg->read<double>("squark mass",-1.);
  double default_chi    = cfg->read<double>("NLSP mass",-1.);	 
         default_chi    = cfg->read<double>("bino mass",default_chi); 
  double default_cha    = cfg->read<double>("wino mass",-1);   
  int    ngen           = cfg->read<int>("nGen",-1);   
  
  std::vector<std::pair<std::string,std::string> > bin_limits;
  int n_channels = dat_cfg->read<int>("nMetBins");
  for (int i=0; i<n_channels; ++i) {
    std::string bin_str( (std::string)"bin "+ToString<int>(i));
    std::vector<std::string> l = bag_of<std::string>(dat_cfg->read<std::string>(bin_str));
    bin_limits.push_back(std::make_pair( l[0], l[2] ));
  }
  
  
  //loop over all signal points
  do {

    point p;
    std::stringstream ss;
    p.nr = n;
    ss << "Point " << n++;
    p.gluino = cfg->read<double>(ss.str()+" gluino mass",default_gluino);
    p.squark = cfg->read<double>(ss.str()+" squark mass",default_squark);
    p.chi    = cfg->read<double>(ss.str()+" NLSP mass",default_chi);    
    p.chi    = cfg->read<double>(ss.str()+" nlsp mass",default_chi);    
    p.chi    = cfg->read<double>(ss.str()+" bino mass",p.chi); 
    p.cha    = cfg->read<double>(ss.str()+" wino mass",default_cha);   
    ngen     = cfg->read<double>(ss.str()+" generated events",ngen);   
    
    if (p.gluino==default_gluino && p.squark==default_squark && p.chi==default_chi && p.cha==default_cha) break;  

    p.lumi = dat_cfg->read<double>("lumi", luminosity);
    p.signal = p.bgd_qcd = p.bgd_ewk= p.bgd_fsr = p.qcd_contamination = p.ewk_contamination = p.data = p.u_NLO = 0;
    p.u_pdfxsec = p.u_pdfacc = p.u_sig_stat = p.u_lumi = p.u_qcd = p.u_qcd_stat= p.u_ewk= p.u_ewk_stat= p.u_fsr= p.u_fsr_stat=0;
    p.variable = dat_cfg->read<std::string>("variable", "met");

    p.xsec       = cfg->read<double>(ss.str()+" cross section",p.xsec); 
    p.xsecNLO    = cfg->read<double>(ss.str()+" cross section",p.xsecNLO); 
    p.u_pdfxsec    = 0.01 * cfg->read<double>(ss.str()+" relative total theoretical cross section uncert",p.u_pdfxsec); 

    //read data and data-driven backgrounds
    std::vector<double> data       = bag_of<double>(dat_cfg->read<std::string>("data selected"));
    std::vector<double> qcd        = bag_of<double>(dat_cfg->read<std::string>("data QCD"));
    std::vector<double> u_qcd      = bag_of<double>(dat_cfg->read<std::string>("data QCD syst uncert"));
    std::vector<double> u_qcd_stat = bag_of<double>(dat_cfg->read<std::string>("data QCD stat uncert"));
    std::vector<double> ewk        = bag_of<double>(dat_cfg->read<std::string>("data EWK"));
    std::vector<double> u_ewk      = bag_of<double>(dat_cfg->read<std::string>("data EWK syst uncert"));
    std::vector<double> u_ewk_stat = bag_of<double>(dat_cfg->read<std::string>("data EWK stat uncert"));
    //MC backgrounds
    std::vector<double> fsr        = bag_of<double>(fsr_cfg->read<std::string>("ISR"));
    std::vector<double> u_fsr      = bag_of<double>(fsr_cfg->read<std::string>("ISR syst uncert"));
    std::vector<double> u_fsr_stat = bag_of<double>(fsr_cfg->read<std::string>("ISR stat uncert"));
    //Signal
    std::vector<double> sig        = bag_of<double>(cfg->read<std::string>(ss.str()+" number of signal events in bins"));
    std::vector<double> u_sig_stat = bag_of<double>(cfg->read<std::string>(ss.str()+" statistical error of signal events in bins"));
    std::vector<double> u_sig      = bag_of<double>(cfg->read<std::string>(ss.str()+" systematical error of signal events in bins",""));
    std::vector<double> sig_qcd    = bag_of<double>(cfg->read<std::string>(ss.str()+" QCD prediction"));
    std::vector<double> sig_ewk    = bag_of<double>(cfg->read<std::string>(ss.str()+" EWK prediction"));
    //Consistency check: Same number of channels in all samples?
    if ( n_channels != (int)data.size() ||
        (int)qcd.size()!=n_channels ||(int)ewk.size()!=n_channels ||(int)fsr.size()!=n_channels ||
        (int)sig.size()!=n_channels || (int)sig_ewk.size()!=n_channels) {
      std::cerr << "ERROR: inconstitent number of channels (should be "<<n_channels<<") at signalpoint "<<n-1
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
    p.u_sig = 1.02;             //factorial uncertainty
    p.u_scaleDataMC = 1.04;     //factorial uncertainty
    
    //fill all channels/bins
    for (int c=0;c<n_channels;++c){

      point::bin channel;
      channel.bin_up             = bin_limits[c].first;
      channel.bin_low            = bin_limits[c].second;

      channel.signal             = sig[c] * scaleDataMC;//This is in <number of generated events>, needs to be
                                                         //weighted according to Lumi*xsec/n_generated, do that later

      channel.bgd_qcd            = qcd[c];
      channel.bgd_ewk            = ewk[c];
      channel.bgd_fsr            = fsr[c];

      channel.qcd_contamination  = sig_qcd[c] * scaleDataMC;
      channel.ewk_contamination  = sig_ewk[c] * scaleDataMC;
      channel.data               = data[c];

      //store uncertainties factorial, i.e. 1 for no unc, 1.1 for 10% unc, 2.0 for 100% etc:
      channel.u_sig_stat         = (sig[c] ?           1.+ u_sig_stat[c]/sig[c]:0);
      channel.u_sig              = ((int)u_sig.size()>c && sig[c] ? 1.+ u_sig[c]/sig[c] : p.u_sig);
      channel.u_lumi             = luminosityUncertainty;

      channel.u_qcd              = (channel.bgd_qcd?   1.+ u_qcd[c]/channel.bgd_qcd	  :0);
      channel.u_qcd_stat         = (channel.bgd_qcd?   1.+ u_qcd_stat[c]/channel.bgd_qcd  :0);
      channel.u_ewk              = (channel.bgd_ewk?   1.+ u_ewk[c]/channel.bgd_ewk       :0);
      channel.u_ewk_stat         = (channel.bgd_ewk?   1.+ u_ewk_stat[c]/channel.bgd_ewk  :0);
      channel.u_fsr              = (channel.bgd_fsr?   1.+ u_fsr[c]/channel.bgd_fsr       :0);
      channel.u_fsr_stat         = (channel.bgd_fsr?   1.+ u_fsr_stat[c]/channel.bgd_fsr  :0);

      channel.u_scaleDataMC = p.u_scaleDataMC;
      
      //std::cout<<c<<": s="<<channel.signal<< ", s (w/o cont)="<<channel.signal-channel.qcd_contamination-channel.ewk_contamination
//	       <<std::endl;

      if (channel.signal-channel.qcd_contamination-channel.ewk_contamination>=0. ) {
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
      } else {
        std::cout << "gl="<<p.gluino<<", sq="<<p.squark<<", chi="<<p.chi<<", cha="<<p.cha<<"; bin="<<c
	          << "   signal = "<<channel.signal<<", cont = "<<channel.qcd_contamination-channel.ewk_contamination
		  << std::endl;	 
      }
  }

  //store total uncertainties per point *relative*, e.g. u_qcd=0.10 means 10% on qcd:
  p.totalGenerated = ngen;
  p.u_lumi      = (luminosityUncertainty-1.)*p.lumi;
  p.u_sig_stat  = sqrt(p.signal)/p.totalGenerated; 
  p.u_qcd       = p.u_qcd / p.bgd_qcd;
  p.u_qcd_stat  = sqrt(p.u_qcd_stat) / p.bgd_qcd;
  p.u_ewk       = p.u_ewk / p.bgd_ewk;
  p.u_ewk_stat  = sqrt(p.u_ewk_stat) / p.bgd_ewk;
  p.u_fsr       = p.u_fsr / p.bgd_fsr;
  p.u_fsr_stat  = sqrt(p.u_fsr_stat) / p.bgd_fsr;
  //std::cout<<"lumi="<<p.lumi<<",xsec="<<p.xsecNLO<<",ngen="<<p.totalGenerated<<std::endl;
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


void points::ScaleSignalYieldsForXsec()
{
   for (std::vector<point>::iterator it=p_.begin(); it!=p_.end(); ++it){
      double scale = it->lumi*it->xsecNLO/it->totalGenerated;
      for (std::vector<point::bin>::iterator bin=it->bins.begin(); bin!=it->bins.end(); ++bin) {
        //bin->u_pdfxsec = 1.0 + 0.01 * u_xsec; //factorial per bin(!)			
        bin->signal	       *= scale;
        bin->qcd_contamination *= scale;
        bin->ewk_contamination *= scale;
        bin->u_NLO = 1.0; //not considered for limit calculation
        bin->u_pdfacc  = 1.0;        
        //std::cout<<bin-(*it)->bins.begin()<<": s="<<bin->signal<<""<<std::endl;
      }  
      it->signal  *= scale;
      it->qcd_contamination *= scale;
      it->ewk_contamination *= scale;
   }
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

//std::cout << "nbins: "<<p.bins.size()<<", min="<<bmin<<", max="<<bmax<<"; d[3]="<<res->bins[3].data<<std::endl;
  
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
//std::cout <<res->bins[3].data<<"  -> +"<< res->bins[i].data <<std::endl;

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
//std::cout <<res->bins[3].data<<std::endl;
  res->bins.erase(res->bins.begin()+bmin+1, res->bins.begin()+bmax+1);
//std::cout <<res->bins[3].data<<std::endl;
  return res;
}


void points::Do(const std::string& name, const std::string&dat, const std::string&sig, const std::string&xsec, const std::string&pdf)
{
   Points.Reset();
   ReadSignal(sig, dat);
   AddXsec(xsec);
   if (pdf!="") AddPDFs(pdf);
   {points MergedPoints;
   for (std::vector<point>::iterator it=Points.Get()->begin(); it!=Points.Get()->end(); ++it)
      MergedPoints.Add( *MergeBins(*it, 6));
   std::system( ((std::string)"mkdir DataCards/"+name).c_str());
   std::system( ((std::string)"mkdir DataCards/"+name+"_SingleChannels").c_str());
   MergedPoints.Write(((std::string)"DataCards/"+name+"/GMSB").c_str());
   MergedPoints.WriteSingleBin(((std::string)"DataCards/"+name+"_SingleChannels/GMSB").c_str());
   }

}


void points::DoSMS(const std::string& name, const std::string&dat, const std::string&sig, const std::string&xsec, const std::string&pdf)
{
   Points.Reset();
   ReadSignal(sig, dat);
   if (xsec!="") AddSmsXsec(xsec);
   if (pdf!="")  AddPDFs(pdf);
   ScaleSignalYieldsForXsec();
   
   {points MergedPoints;
   for (std::vector<point>::iterator it=Points.Get()->begin(); it!=Points.Get()->end(); ++it)
      MergedPoints.Add( *MergeBins(*it, 6));
   std::system( ((std::string)"mkdir DataCards/"+name).c_str());
   std::system( ((std::string)"mkdir DataCards/"+name+"_SingleChannels").c_str());
   MergedPoints.Write(((std::string)"DataCards/"+name+"/SMS").c_str());
   MergedPoints.WriteSingleBin(((std::string)"DataCards/"+name+"_SingleChannels/GMSB").c_str());
   }

}

int main(int argc, char* argv[]) {

   std::string gsq_w_xsec="Xsecs/Spectra_gsq_W_8TeV.xsec";//"Xsecs/NLOProspinoXsecs_Wino_Neutr375.txt";
   std::string gsq_b_xsec="Xsecs/Spectra_gsq_B_8TeV.xsec";//"Xsecs/NLOProspinoXsecs_Bino_Neutr375.txt";
   std::string gsq_w_pdf="PDF/Spectra_gsq_W_phad_pdfuncert.dat";
   std::string gsq_b_pdf="PDF/Spectra_gsq_B_phad_envpdfuncert.dat";
   //std::string YieldsData = "inputWinter13/eventYieldData-2014-03-19.txt";
   std::string YieldsData = "inputWinter13/eventYieldData-classic-2014-05-02.txt";
   

   Points.DoSMS("SMS_T5gg", "inputWinter13/Closure_Data_met.txt",
             "inputWinter14/eventYieldT5gg-2014-12-15.txt","","");   

   Points.DoSMS("SMS_T5wg", "inputWinter13/Closure_Data_met.txt",
             "inputWinter14/eventYieldT5wg-2014-12-15.txt","","");   

  // Points.Do("GMSB_SqGl_met-Wino", "inputWinter13/Closure_Data_met.txt",
  //           "inputWinter13/eventYieldSpectra_gsq_W-2014-07-13.txt",gsq_w_xsec,gsq_w_pdf);   

  // Points.Do("GMSB_SqGl_met-Bino", "inputWinter13/Closure_Data_met.txt",
  //           "inputWinter13/eventYieldSpectra_gsq_B-2014-07-13.txt",gsq_b_xsec,gsq_b_pdf); 

   /*
   //21 MET bins	       
   Points.Do("GMSB_SqGl-Wino-21metBins", "inputWinter13/eventYieldData_21metBins-2014-01-30.txt",
             "inputWinter13/eventYieldSpectra_gsq_W_21metBins-2014-01-29.txt",gsq_w_xsec,gsq_w_pdf);   

   Points.Do("GMSB_SqGl-Bino-21metBins", "inputWinter13/eventYieldData_21metBins-2014-01-30.txt",
             "inputWinter13/eventYieldSpectra_gsq_B_21metBins-2014-01-29.txt",gsq_b_xsec,gsq_b_pdf); 

	       
   //4fb-1, real data yields
   Points.Do("GMSB_SqGl-Wino-4fb", "inputWinter13/eventYieldData4fb-2014-01-30.txt",
             "inputWinter13/eventYieldSpectra_gsq_W-2014-01-29.txt",gsq_w_xsec,gsq_w_pdf);   

   Points.Do("GMSB_SqGl-Bino-4fb", "inputWinter13/eventYieldData4fb-2014-01-30.txt",
             "inputWinter13/eventYieldSpectra_gsq_B-2014-01-29.txt",gsq_b_xsec,gsq_b_pdf); 
   */


/*   //new-met
   Points.Do("GMSB_SqGl_metnew-Wino", "inputWinter13/Closure_Data_met_new.txt",
             "inputWinter13/eventYieldSpectra_gsq_Wnew-2014-05-02.txt",gsq_w_xsec,gsq_w_pdf);   

   Points.Do("GMSB_SqGl_metnew-Bino", "inputWinter13/Closure_Data_met_new.txt",
             "inputWinter13/eventYieldSpectra_gsq_Bnew-2014-05-02.txt",gsq_b_xsec,gsq_b_pdf); 

   //new-fibonacci
   Points.Do("GMSB_SqGl_metfibo-Wino", "inputWinter13/Closure_Data_met_fibo.txt",
             "inputWinter13/eventYieldSpectra_gsq_Wfibo-2014-05-02.txt",gsq_w_xsec,gsq_w_pdf);   

   Points.Do("GMSB_SqGl_metfibo-Bino", "inputWinter13/Closure_Data_met_fibo.txt",
             "inputWinter13/eventYieldSpectra_gsq_Bfibo-2014-05-02.txt",gsq_b_xsec,gsq_b_pdf); 

   //new-optimized
   Points.Do("GMSB_SqGl_metoptim-Wino", "inputWinter13/Closure_Data_met_optim.txt",
             "inputWinter13/eventYieldSpectra_gsq_Woptimized-2014-05-02.txt",gsq_w_xsec,gsq_w_pdf);   

   Points.Do("GMSB_SqGl_metoptim-Bino", "inputWinter13/Closure_Data_met_optim.txt",
             "inputWinter13/eventYieldSpectra_gsq_Boptimized-2014-05-02.txt",gsq_b_xsec,gsq_b_pdf); 

*/
/*
   //old-met
   Points.Do("GMSB_SqGl_met-Wino", "inputWinter13/Closure_Data_met.txt",
             "inputWinter13/eventYieldSpectra_gsq_Wold-2014-05-02.txt",gsq_w_xsec,gsq_w_pdf);   

   Points.Do("GMSB_SqGl_met-Bino", "inputWinter13/Closure_Data_met.txt",
             "inputWinter13/eventYieldSpectra_gsq_Bold-2014-05-02.txt",gsq_b_xsec,gsq_b_pdf); 

   //old-met
   Points.Do("GMSB_SqGl_met-Wino_sc1.5_u100", "inputWinter13/Closure_Data_met_sc1.5_u100.txt",
             "inputWinter13/eventYieldSpectra_gsq_Wold-2014-05-02.txt",gsq_w_xsec,gsq_w_pdf);   

   Points.Do("GMSB_SqGl_met-Bino_sc1.5_u100", "inputWinter13/Closure_Data_met_sc1.5_u100.txt",
             "inputWinter13/eventYieldSpectra_gsq_Bold-2014-05-02.txt",gsq_b_xsec,gsq_b_pdf); 


   //old-met
   Points.Do("GMSB_SqGl_met-Wino_sc2_u100", "inputWinter13/Closure_Data_met_sc2_u100.txt",
             "inputWinter13/eventYieldSpectra_gsq_Wold-2014-05-02.txt",gsq_w_xsec,gsq_w_pdf);   

   Points.Do("GMSB_SqGl_met-Bino_sc2_u100", "inputWinter13/Closure_Data_met_sc2_u100.txt",
             "inputWinter13/eventYieldSpectra_gsq_Bold-2014-05-02.txt",gsq_b_xsec,gsq_b_pdf); 


   //old-met
   Points.Do("GMSB_SqGl_met-Wino_sc2_u50", "inputWinter13/Closure_Data_met_sc2_u50.txt",
             "inputWinter13/eventYieldSpectra_gsq_Wold-2014-05-02.txt",gsq_w_xsec,gsq_w_pdf);   

   Points.Do("GMSB_SqGl_met-Bino_sc2_u50", "inputWinter13/Closure_Data_met_sc2_u50.txt",
             "inputWinter13/eventYieldSpectra_gsq_Bold-2014-05-02.txt",gsq_b_xsec,gsq_b_pdf); 

   //old-met
   Points.Do("GMSB_SqGl_met-Wino_sc3_u50", "inputWinter13/Closure_Data_met_sc3_u50.txt",
             "inputWinter13/eventYieldSpectra_gsq_Wold-2014-05-02.txt",gsq_w_xsec,gsq_w_pdf);   

   Points.Do("GMSB_SqGl_met-Bino_sc3_u50", "inputWinter13/Closure_Data_met_sc3_u50.txt",
             "inputWinter13/eventYieldSpectra_gsq_Bold-2014-05-02.txt",gsq_b_xsec,gsq_b_pdf); 

   //old-met
   Points.Do("GMSB_SqGl_met-Wino_sc3_u100", "inputWinter13/Closure_Data_met_sc3_u100.txt",
             "inputWinter13/eventYieldSpectra_gsq_Wold-2014-05-02.txt",gsq_w_xsec,gsq_w_pdf);   

   Points.Do("GMSB_SqGl_met-Bino_sc3_u100", "inputWinter13/Closure_Data_met_sc3_u100.txt",
             "inputWinter13/eventYieldSpectra_gsq_Bold-2014-05-02.txt",gsq_b_xsec,gsq_b_pdf); 

*/
}
