#include "MyConfig.h"
#include "table.h"
#include "demo.h"

#include <cstdlib>
#include <fstream>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <functional>


double GetR(ConfigFile* conf, const std::string& l)
{
  if (conf->read<double>("CLs "+l, 9999) >= 20.)
    return conf->read<double>("CLs "+l+" asymptotic", -1);
  else
    return conf->read<double>("CLs "+l, -1);
}


void WriteTable(std::ostream& os, const Table::TableStyle style, const std::string& dir, 
                const int sq, const int gl, const int chi, const int cha, const int min, const int max, bool caption)
{
  using namespace std;
  using namespace Table;

  if (max<min || min<0) return;
  int N = max-min+1;
  std::string titel = "";
  std::string pm = " +- ";
  if (style==Table::TeX) {
    titel = "%% DemoPoint, m($\\tilde{q}$) = "+ToString(sq)+" GeV, m($\\tilde{g}$) = "+ToString(gl)+" GeV, m($\\tilde{\\chi^0_1}$) = "+ToString(chi)+" GeV, m($\\tilde{\\chi^\\pm_1}$) = "+ToString(cha)+" GeV (from "+dir+")";
    pm = "$\\pm$";
  }
  else if (caption) titel = "m(squark) = "+ToString(sq)+", m(gluino) = "+ToString(gl)+", m(chi^0_1) = "+ToString(chi)+", m(chi^+-_1) = "+ToString(cha);
  TTable table(titel);
  table.SetDelimiter(" | ");
  table.SetStyle(style);
  ConfigFile * config[N];
  if( style == Table::TeX )
      table.AddColumn<string>("$\\met$ Range [GeV]");
  else
      table.AddColumn<string>("MET Range [GeV]");
  double lumi[N],sig[N],xsec[N],qcdyield[N],ewkyield[N],fsryield[N],u_qcd[N],u_ewk[N],u_fsr[N],u_sig[N],totbgd[N],u_totbgd[N];
  //string acceptance[N], signal[N], background[N], qcd[N], ewk[N], fsr[N], data[N];
  std::vector<string> acceptance(N), signal(N), background(N), qcd(N), ewk(N), fsr(N), data(N);
  
  for (int ch=min; ch<=max; ++ch) {
    int i = ch - min;

    // read config file
    try{
      stringstream ss;
      ss << dir << "/GMSB_"<<sq<<"_"<<gl<<"_"<<chi<<"_"<<cha<<"_bin"<<ch<<".txt.result.txt";
      config[i] = new ConfigFile(ss.str());
    }
    catch(ConfigFile::file_not_found& e){
      std::cerr<<e.filename<<" not found!"<<std::endl;
      config[i] = 0;
    }

    // Label the bins with their upper and lower edge, such as [100,120), ...
    // Warning: 'bin upper limit' and 'bin lower limit' are transposed
    stringstream head;
    if( style == Table::TeX ) head << "$";
    head << "[" << config[i]->read<double>("bin upper limit") << ",";
    if( ch == max )
        head << "\\infty";
    else
        head << config[i]->read<double>("bin lower limit");
    head << ")";
    if( style == Table::TeX ) head << "$";
    table.AddColumn<string>(head.str());
    

    sig[i]=0;
    acceptance[i] = signal[i] = background[i] = qcd[i] = ewk[i] = fsr[i] = "-";
       
    if (config[i]){
       lumi[i]      = config[i]->read<double>("Luminosity");
       xsec[i]      = config[i]->read<double>("Xsection.NLO", -1);
       data[i]      = config[i]->read<std::string>("data");
       sig[i]	    = config[i]->read<double>("signal"), 
       qcdyield[i]  = config[i]->read<double>("qcd");
       ewkyield[i]  = config[i]->read<double>("ewk"); 
       fsryield[i]  = config[i]->read<double>("fsr");
       u_qcd[i]     = sqrt(pow(config[i]->read<double>("u_rel_qcd"),2)+
                           pow(config[i]->read<double>("u_rel_qcd_stat"),2)) *
		      qcdyield[i];
       u_ewk[i]     = sqrt(pow(config[i]->read<double>("u_rel_ewk"),2)+
                           pow(config[i]->read<double>("u_rel_ewk_stat"),2)) *
		      ewkyield[i];
       u_fsr[i]     = sqrt(pow(config[i]->read<double>("u_rel_fsr"),2)+
     		           pow(config[i]->read<double>("u_rel_fsr_stat"),2)) *
		      fsryield[i];
       u_sig[i]     = config[i]->read<double>("u_rel_signal_exp") * 
                      sig[i];
       totbgd[i]    = config[i]->read<double>("background");
       u_totbgd[i]  = sqrt( pow(u_qcd[i],2)+pow(u_ewk[i],2)+pow(u_fsr[i],2));
       acceptance[i]= ToString(100.*sig[i]/(xsec[i]*lumi[i]) );


       qcd[i]	    = ToStringYield(qcdyield[i])+pm+ ToStringUnc( u_qcd[i]  );	     
       ewk[i]	    = ToStringYield(ewkyield[i])+pm+ ToStringUnc( u_ewk[i]  );
       fsr[i]	    = ToStringYield(fsryield[i])+pm+ ToStringUnc( u_fsr[i]  );
       signal[i]    = ToStringYield(sig[i])  	+pm+ ToStringUnc( u_sig[i]  );
       background[i]= ToStringYield(totbgd[i])	+pm+ ToStringUnc( u_totbgd[i]);
    }
  }    


  table << "QCD";
  for (int i=0; i<=max-min; ++i)  table << qcd[i];
  table << "EWK";
  for (int i=0; i<=max-min; ++i)  table << ewk[i];
  table << "ISR/FSR";
  for (int i=0; i<=max-min; ++i)  table << fsr[i];
  table << "Background";
  for (int i=0; i<=max-min; ++i)  table << background[i];
  if (style == Table::TeX )
      table << "\\hline\nData";
  else
      table << "Data";
  for (int i=0; i<=max-min; ++i)  table << data[i];
  table << "Signal";
  for (int i=0; i<=max-min; ++i)  table << signal[i];
  table << "Bkg. from Signal";
  for (int i=0; i<=max-min; ++i)  table << (!config[i]?"-":ToStringYield(config[i]->read<double>("signal.contamination", -1)));
  table << "Acceptance [\\%]";
  for (int i=0; i<=max-min; ++i)  table << acceptance[i];
  //--------------------------------------------------------------------------------------------------------------
  table << "Exp. limit [pb]";
  for (int i=0; i<=max-min; ++i)  
    table << (!config[i]?"-":ToString(xsec[i]*GetR(config[i],"expected"),3));
  table << "Obs. limit [pb]";
  for (int i=0; i<=max-min; ++i)  
    table << (!config[i]?"-":ToString(xsec[i]*GetR(config[i],"observed"),3));
  //table << "Exp. limit [events]";
  //for (int i=0; i<=max-min; ++i)  table << (!config[i]?"-":ToString(sig[i]*config[i]->read<double>("CLs expected", -1)));
  //table << "Obs. limit [events]";
  //for (int i=0; i<=max-min; ++i)  table << (!config[i]?"-":ToString(sig[i]*config[i]->read<double>("CLs observed", -1)));
  
  {
    ConfigFile * config;
    try{
      stringstream ss;
      ss << dir << "/GMSB_"<<sq<<"_"<<gl<<"_"<<chi<<"_"<<cha<<".txt.result.txt";
      config = new ConfigFile(ss.str());
    }
    catch(ConfigFile::file_not_found& e){
      std::cerr<<e.filename<<" not found!"<<std::endl;
      config= 0;
    }
    if (config) {
     stringstream ss;
     stringstream texDefinitions;
     double xsec = config->read<double>("Xsection.NLO");
     if (style!=Table::TeX) {
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
     ss << "The total luminosity corresponds to "<< config->read<double>("Luminosity", 0) << " pb-1, "
        << "the NLO cross-section is $\\sigma$ = "<<xsec<<" pb."
	<<std::endl;   
     } else {
     ss<<"\n\\caption{Resulting event yields for the data corresponding to $"
       << std::fixed << std::setprecision(0)<< config->read<double>("Luminosity", 0) 
       << "$~pb$^{-1}$, the estimated background, and a signal point with $M_{\\tilde{q}}="
       << ToString(sq) <<"$~GeV, $M_{\\tilde{g}}="
       << ToString(gl) <<"$~GeV, and a total signal cross section of $\\sigma_{\\mbox{\\footnotesize NLO}}="
       << std::fixed << std::setprecision(4)<< xsec<<"$~pb. \n";
     ss<<"The combined observed (expected) CLs cross-section limit for this point is  $"
       <<std::fixed << std::setprecision(4)<< xsec * config->read<double>("CLs observed", 0)
       <<"$~pb ($"
       <<std::fixed << std::setprecision(4)<< xsec * config->read<double>("CLs expected", 0)
       <<"$~pb) at $95\\%$~CL.}\n";

       texDefinitions << "\n\\newcommand{\\tableMSquark}{" << ToString(sq) << "} \% GeV\n";
       texDefinitions << "\\newcommand{\\tableMGluino}{" << ToString(gl) << "} \% GeV\n";
       texDefinitions << "\\newcommand{\\tableSignalXSec}{"
           << std::fixed << std::setprecision(4)<< xsec << "} \% pb\n";
       texDefinitions << "\\newcommand{\\tableObsLimit}{" << std::fixed
           << std::setprecision(4)<< xsec * config->read<double>("CLs observed", 0) << "} \% pb\n";
       texDefinitions << "\\newcommand{\\tableExpLimit}{" << std::fixed
           << std::setprecision(4)<< xsec * config->read<double>("CLs expected", 0) << "} \% pb\n";

     }
     if (caption) table.SetCaption(ss.str());
     if ( texDefinitions ) table.SetCaption( texDefinitions.str() );
     //os << ss.str()<<std::endl;
    }
  }  
    
  os << table;
}

int main( int argc, char* argv[] )
{
   MyConfig* config = new MyConfig(argc, argv);

   int sq = 700, gl=820, chi=375, cha=0, min=0, max=5;
   std::string dir="cMSSM", stylestr;
   Table::TableStyle style = Table::Plain;
   bool caption = false;

   config->addUsage(  "Usage: " );
   config->addUsage(  "" );
   config->addFlag(   "help",   'h',              "Prints this help" );
   config->addFlag(   "caption",' ',              "Prints a caption" );
   config->addOption( "config", 'c',"[file]",     "configuration file (can be supassed by commandline args) " );
   config->addOption( "style",  's',"[style]",    "Style can be 'tex', 'empty' or 'plain' (default) " );
   config->addOption( "msquark",' ',"[GeV]",      "m(sq)" );
   config->addOption( "mgluino",' ',"[GeV]",      "m(gl)" );
   config->addOption( "mchi1",  ' ',"[GeV]",      "m(chi^0_1)" );
   config->addOption( "mcha1",  ' ',"[GeV]",      "m(chi^+-_1)" );
   config->addOption( "min",    ' ',"",           "lower bin (channel); default: 0" );
   config->addOption( "max",    ' ',"",           "upper bin (channel); default: 5" );
   config->addOption( "dir",    'd',"[directory]","directory containing the demo limit result files" );

   config->Process();

   if (config->IsAvailable("dir",'d')) dir = config->Get("dir",'d');
   if (config->IsAvailable("mcha1"))   cha = StringTo<int>(config->Get("mcha1"));
   if (config->IsAvailable("mchi1"))   chi = StringTo<int>(config->Get("mchi1"));
   if (config->IsAvailable("msquark"))  sq = StringTo<int>(config->Get("msquark"));
   if (config->IsAvailable("mgluino"))  gl = StringTo<int>(config->Get("mgluino"));
   if (config->IsAvailable("min"))     min = StringTo<int>(config->Get("min"));
   if (config->IsAvailable("max"))     max = StringTo<int>(config->Get("max"));
   if (config->IsAvailable("style")) stylestr = config->Get("style");
   caption = config->IsAvailable("style");
   std::transform(stylestr.begin(), stylestr.end(), stylestr.begin(), std::ptr_fun<int, int>(std::toupper));
   if      (stylestr=="TEX")    style = Table::TeX;
   else if (stylestr=="EMPTY")  style = Table::Empty;

   WriteTable(std::cout,style, dir, sq, gl, chi, cha, min, max, caption);


}
