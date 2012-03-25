#include "anyoption.h"
#include "ConfigFile.h"
#include "table.h"

#include <string>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <cmath>

std::string ToString(double d, int precision=1, std::string s=""){
  if (d<=0&&s!="") return s;
  std::stringstream ss;
  ss <<std::fixed << std::setprecision(precision)<< d;
  return ss.str();
}

void WriteTable(std::ostream& os, const Table::TableStyle style, const std::string& dir, const int mzero, const int mhalf, const int min, const int max)
{
  using namespace std;
  using namespace Table;

  if (max<min || min<0) return;
  int N = max-min+1;
  TTable table("DemoPoint, Mzero = "+ToString(mzero)+", Mhalf = "+ToString(mhalf)+"  (from "+dir+")");
  table.SetStyle(style);
  table.SetDelimiter(" | ");
  ConfigFile * config[N];
  table.AddColumn<string>(""); //desciption
  double sig[N],xsec[N],qcdyield[N],lostle[N],tau[N],zinvis[N],u_qcd2[N],u_wtop2[N],u_zinvis2[N],u_sig2[N];
  string acceptance[N], signal[N], background[N], qcd[N], wtop[N], z[N];
  
  for (int ch=min; ch<=max; ++ch) {
    int i = ch - min;
    stringstream head;
    head << "Bin "<<ch;
    table.AddColumn<string>(head.str());
    
    try{
      stringstream ss;
      ss << dir << "/cMSSM_"<<mzero<<"_"<<mhalf<<"_10_0_1_ch"<<ch<<".txt.result.txt";
      config[ch-min] = new ConfigFile(ss.str());
    }
    catch(ConfigFile::file_not_found& e){
      std::cerr<<e.filename<<" not found!"<<std::endl;
      config[ch-min] = 0;
    }
    sig[i]=0;
    acceptance[i] = signal[i] = background[i] = qcd[i] = wtop[i] = z[i] = "-";
    
    if (config[i]){
       qcdyield[i]  = config[i]->read<double>("qcd_0"),
       lostle[i]    = config[i]->read<double>("lostle_0"),
       tau[i]	    = config[i]->read<double>("tau_0"),
       zinvis[i]    = config[i]->read<double>("zinvis_0");
       u_qcd2[i]    = pow(config[i]->read<double>("qcd_trigger"),2)+
                      pow(config[i]->read<double>("QCD_stat_unc"),2)+
		      pow(config[i]->read<double>("other_QCD_uncerts"),2),
       u_wtop2[i]   = pow(config[i]->read<double>("lostle_trigger"),2)+
                      pow(config[i]->read<double>("lostlepton_uncertainties"),2)+
		      //pow(config[i]->read<double>("lole_non-closure"),2)+
     		      pow(config[i]->read<double>("stat_IsoMuCS"),2)+
     		      pow(config[i]->read<double>("tau_trigger"),2)+pow(config[i]->read<double>("other_tau_uncertainties"),2),
       u_zinvis2[i] = pow(config[i]->read<double>("zinvis_trigger"),2)+pow(config[i]->read<double>("systematic_uncertainties"),2)+pow(config[i]->read<double>("statistics"),2);
       sig[i]	    = config[i]->read<double>("signal_0"), 
       u_sig2[i]    = pow(config[i]->read<double>("signal_stat"),2)+
                       pow(config[i]->read<double>("other_syst"),2)+
                       pow(config[i]->read<double>("JES"),2)+
                       pow(config[i]->read<double>("JER"),2)+
                       pow(config[i]->read<double>("signal_trigger"),2)+
    	               pow(config[i]->read<double>("signal_syst_acceptance_PDF"),2);
       qcd[i]       = ToString(qcdyield[i])+" +- "+ToString(sqrt(u_qcd2[i]));	       
       wtop[i]      = ToString(lostle[i]+tau[i])+" +- "+ToString(sqrt(u_wtop2[i]));
       z[i]         = ToString(zinvis[i])+ " +- "+ToString(sqrt(u_zinvis2[i]));
       signal[i]    = ToString(sig[i])+ " +- "+ToString(sqrt(u_sig2[i]));
       background[i] = ToString(qcdyield[i]+lostle[i]+tau[i]+zinvis[i]) + " +- "+ToString(sqrt(u_qcd2[i]+u_wtop2[i]+u_zinvis2[i]));
       xsec[i]      = config[i]->read<double>("Xsection.NLO", -1);
       acceptance[i]= ToString(100*sig[i]/(xsec[i]*config[i]->read<double>("Luminosity")) );
    }
  }    


  table << "qcd";
  for (int i=0; i<=max-min; ++i)  table << qcd[i];
  table << "W+ttbar";
  for (int i=0; i<=max-min; ++i)  table << wtop[i];
  table << "Z->nunu";
  for (int i=0; i<=max-min; ++i)  table << z[i];
  table << "background";
  for (int i=0; i<=max-min; ++i)  table << background[i];
//  table << "data";
//  for (int i=0; i<=max-min; ++i)  table << (!config[i]?"-":ToString(config[i]->read<int>("data", -1),0));
  table << "signal";
  for (int i=0; i<=max-min; ++i)  table << signal[i];
  table << "sig. cont.";
  for (int i=0; i<=max-min; ++i)  table << (!config[i]?"-":ToString(config[i]->read<double>("signal_contamination_0", -1),2));
  table << "Acceptance [%]";
  for (int i=0; i<=max-min; ++i)  table << acceptance[i];
  //--------------------------------------------------------------------------------------------------------------
  table << "xsec NLO [pb]";
  for (int i=0; i<=max-min; ++i)  table << (!config[i]?"-":ToString(xsec[i],3));
  table << "Exp. CLs [pb]";
  for (int i=0; i<=max-min; ++i)  table << (!config[i]?"-":ToString(xsec[i]*config[i]->read<double>("CLs expected", -1),3));
  table << "Obs. CLs [pb]";
  for (int i=0; i<=max-min; ++i)  table << (!config[i]?"-":ToString(xsec[i]*config[i]->read<double>("CLs observed", -1),3));
  table << "Exp. [events]";
  for (int i=0; i<=max-min; ++i)  table << (!config[i]?"-":ToString(sig[i]*config[i]->read<double>("CLs expected", -1)));
  table << "Obs. [events]";
  for (int i=0; i<=max-min; ++i)  table << (!config[i]?"-":ToString(sig[i]*config[i]->read<double>("CLs observed", -1)));
  
  {
    ConfigFile * config;
    try{
      stringstream ss;
      ss << dir << "/cMSSM_"<<mzero<<"_"<<mhalf<<"_10_0_1.txt.result.txt";
      config = new ConfigFile(ss.str());
    }
    catch(ConfigFile::file_not_found& e){
      std::cerr<<e.filename<<" not found!"<<std::endl;
      config= 0;
    }
    if (config) {
     stringstream ss;
     double xsec = config->read<double>("Xsection.NLO");
     ss<<"\nThe combined frequentistic observed (expected) CLs cross-section limit for this point is  "
       <<std::fixed << std::setprecision(4)<< xsec * config->read<double>("CLs observed", 0)
       <<" pb-1 ("
       <<std::fixed << std::setprecision(4)<< xsec * config->read<double>("CLs expected", 0)
       <<" pb-1) at 95% CL.\n"; 
     ss  <<"The combined asymptotic observed (expected) CLs cross-section limit for this point is     "
       <<std::fixed << std::setprecision(4)<< xsec * config->read<double>("CLs observed asymptotic", 0)
       <<" pb-1 ("
       <<std::fixed << std::setprecision(4)<< xsec * config->read<double>("CLs expected asymptotic", 0)
       <<" pb-1) at 95% CL.\n"; 
     ss  <<"The combined Profile-Likelihood observed (expected) cross-section limit for this point is "
       <<std::fixed << std::setprecision(4)<< xsec * config->read<double>("CLs observed asymptotic", 0)
       <<" pb-1 ("
       <<std::fixed << std::setprecision(4)<< xsec * config->read<double>("CLs expected asymptotic", 0)
       <<" pb-1) at 95% CL.\n"; 
     ss << "The total luminosity corresponds to "<< config->read<double>("Luminosity", 0) << " pb-1."<<std::endl;   
      table.SetCaption(ss.str());
    }
  }  
    
  os << table;
}

int main( int argc, char* argv[] )
{
   AnyOption *opt = new AnyOption();
   opt->addUsage( "Usage: " );
   opt->addUsage( "" );
   opt->addUsage( " -h  --help                Prints this help " );
   opt->addUsage( " --mzero [GeV]             m0 " );
   opt->addUsage( " --mhalf [GeV]             m1/2 " );
   opt->addUsage( " -d  --dir [directory]     directory containing the demo limit result files" );
   opt->setFlag(  "help", 'h' );
   opt->setOption(  "dir", 'd' );
   opt->setOption(  "mzero" );
   opt->setOption(  "mhalf" );
   
   opt->processCommandArgs( argc, argv );

   
   int mzero = 500, mhalf=560;
   std::string dir="cMSSM";
   { stringstream ss;
     if ( opt->getValue( "mzero" ) != NULL  )  ss << opt->getValue( "mzero" );
     ss >> mzero; }
   { stringstream ss;
   if ( opt->getValue( "mhalf" ) != NULL  )  ss << opt->getValue( "mhalf" );
     ss >> mhalf; }
   if ( opt->getValue( "dir" ) != NULL  || opt->getValue( 'd' ) != NULL )  
     dir = (std::string)opt->getValue( 'd' );
   if( opt->getFlag( "help" ) || opt->getFlag( 'h' ) ) { 
     opt->printUsage();
     return 0;
   }  
   WriteTable(std::cout,Table::Plain, dir, mzero, mhalf, 0, 13);


}
