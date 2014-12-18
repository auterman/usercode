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
                const int nr, const int wino, const int bino, const int min, const int max, bool caption)
{
  using namespace std;
  using namespace Table;

  if (max<min || min<0) return;
  int N = max-min+1;
  std::string titel = "";
  std::string pm = " +- ";
  if (style==Table::TeX) {
    titel = "%% DemoPoint, m(Wino) = "+ToString(wino)+" GeV, m(Bino) = "+ToString(bino)+" GeV (from "+dir+")";
    pm = "$\\pm$";
  }
  else if (caption) titel = "m(Wino) = "+ToString(wino)+", m(bino) = "+ToString(bino);
  TTable table(titel);
  table.SetDelimiter(" | ");
  table.SetStyle(style);
  ConfigFile * config[N];
  table.AddColumn<string>(""); //desciption
  double lumi[N],sig[N],xsec[N],bgd1[N],bgd2[N],bgd3[N],bgd4[N],bgd5[N],bgd6[N],
         u_bgd1[N],u_bgd2[N],u_bgd3[N],u_bgd4[N],u_bgd5[N],u_bgd6[N],u_sig[N],totbgd[N],u_totbgd[N];
  //string acceptance[N], signal[N], background[N], qcd[N], ewk[N], fsr[N], data[N];
  std::vector<string> acceptance(N), signal(N), background(N), b1(N), b2(N), b3(N),b4(N), b5(N), b6(N), 
                      data(N);
  
  for (int ch=min; ch<=max; ++ch) {
    int i = ch - min;
    stringstream head;
    head << "bin"<<ch<<"_";
    table.AddColumn<string>(head.str());
    
    try{
      stringstream ss;
      ss << dir << "/LimitInput_"<<nr<<"_Wino"<<wino<<"_Bino"<<bino<<"_bin"<<ch<<".txt.result.txt";
      config[ch-min] = new ConfigFile(ss.str());
    }
    catch(ConfigFile::file_not_found& e){
      std::cerr<<e.filename<<" not found!"<<std::endl;
      config[ch-min] = 0;
    }
    sig[i]=0;
    acceptance[i] = signal[i] = background[i] = b1[i] = b2[i] = b3[i] = b4[i] = b5[i] = b6[i] = "-";
    
    if (config[i]){
       lumi[i]      = config[i]->read<double>("Lumi");
       xsec[i]      = config[i]->read<double>("Xsection.NLO", -1);
       data[i]      = config[i]->read<std::string>(head.str()+"data");
       sig[i]	    = config[i]->read<double>(head.str()+"signal"), 
       bgd1[i]      = config[i]->read<double>(head.str()+"Vg");
       bgd2[i]      = config[i]->read<double>(head.str()+"gjets");
       bgd3[i]      = config[i]->read<double>(head.str()+"ttg");
       bgd4[i]      = config[i]->read<double>(head.str()+"diboson");
       bgd5[i]      = config[i]->read<double>(head.str()+"efake");
       bgd6[i]      = config[i]->read<double>(head.str()+"qcd");

       u_bgd1[i]    = sqrt(pow(config[i]->read<double>(head.str()+"Vg Scaling_syst_abs"),2)+
                           pow(config[i]->read<double>(head.str()+"Vg Vg_stat_abs"),2));
       u_bgd2[i]    = sqrt(pow(config[i]->read<double>(head.str()+"gjets Scaling_syst_abs"),2)+
                           pow(config[i]->read<double>(head.str()+"gjets gjets_stat_abs"),2));
       u_bgd3[i]    = sqrt(pow(config[i]->read<double>(head.str()+"ttg ttg_syst_abs"),2)+
                           pow(config[i]->read<double>(head.str()+"ttg lumi_unc_abs"),2)+
                           pow(config[i]->read<double>(head.str()+"ttg ttg_stat_abs"),2));
       u_bgd4[i]    = sqrt(pow(config[i]->read<double>(head.str()+"diboson diboson_syst_abs"),2)+
                           pow(config[i]->read<double>(head.str()+"diboson lumi_unc_abs"),2)+
                           pow(config[i]->read<double>(head.str()+"diboson diboson_stat_abs"),2));
       u_bgd5[i]    = sqrt(pow(config[i]->read<double>(head.str()+"efake efake_syst_abs"),2)+
                           pow(config[i]->read<double>(head.str()+"efake lumi_unc_abs"),2)+
                           pow(config[i]->read<double>(head.str()+"efake efake_stat_abs"),2));
       u_bgd6[i]    = sqrt(pow(config[i]->read<double>(head.str()+"qcd qcd_syst_abs"),2)+
                           pow(config[i]->read<double>(head.str()+"qcd lumi_unc_abs"),2)+
                           pow(config[i]->read<double>(head.str()+"qcd qcd_stat_abs"),2));
       u_sig[i]     = sqrt(pow(config[i]->read<double>(head.str()+"signal lumi_unc_abs"),2)+
                           pow(config[i]->read<double>(head.str()+"signal signal_stat_abs"),2));
		      
       totbgd[i]    = config[i]->read<double>(head.str()+"background");
       u_totbgd[i]  = sqrt( pow(config[i]->read<double>(head.str()+"background abs syst"),2) + pow(config[i]->read<double>(head.str()+"background abs stat"),2));


       b1[i]	    = ToStringYield(bgd1[i])+pm+ ToStringUnc( u_bgd1[i]  );	     
       b2[i]	    = ToStringYield(bgd2[i])+pm+ ToStringUnc( u_bgd2[i]  );	     
       b3[i]	    = ToStringYield(bgd3[i])+pm+ ToStringUnc( u_bgd3[i]  );	     
       b4[i]	    = ToStringYield(bgd4[i])+pm+ ToStringUnc( u_bgd4[i]  );	     
       b5[i]	    = ToStringYield(bgd5[i])+pm+ ToStringUnc( u_bgd5[i]  );	     
       b6[i]	    = ToStringYield(bgd6[i])+pm+ ToStringUnc( u_bgd6[i]  );	
       
       double acc =    (lumi[i]*xsec[i]!=0 ? 100*sig[i] / (lumi[i]*xsec[i]) : 0);
       double accunc = (lumi[i]*xsec[i]!=0 ? 100*u_sig[i] / (lumi[i]*xsec[i]) : 0);
       
       acceptance[i] = ToStringAcc(acc)  	+pm+ ToStringAcc( accunc  );
       signal[i]    = ToStringYield(sig[i])  	+pm+ ToStringUnc( u_sig[i]  );
       background[i]= ToStringYield(totbgd[i])	+pm+ ToStringUnc( u_totbgd[i]);
    }
  }    


  table << "Vg";
  for (int i=0; i<=max-min; ++i)  table << b1[i];
  table << "g+jets";
  for (int i=0; i<=max-min; ++i)  table << b2[i];
  table << "ttg";
  for (int i=0; i<=max-min; ++i)  table << b3[i];
  table << "diboson";
  for (int i=0; i<=max-min; ++i)  table << b4[i];
  table << "e-fake";
  for (int i=0; i<=max-min; ++i)  table << b5[i];
  table << "QCD";
  for (int i=0; i<=max-min; ++i)  table << b6[i];
  table << "Background";
  for (int i=0; i<=max-min; ++i)  table << background[i];
  table << "Data";
  for (int i=0; i<=max-min; ++i)  table << data[i];
  table << "Signal";
  for (int i=0; i<=max-min; ++i)  table << signal[i];
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
      ss << dir << "/LimitInput_"<<nr<<"_Wino"<<wino<<"_Bino"<<bino<<".txt.result.txt";
      config = new ConfigFile(ss.str());
    }
    catch(ConfigFile::file_not_found& e){
      std::cerr<<e.filename<<" not found!"<<std::endl;
      config= 0;
    }
    if (config) {
     stringstream ss;
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
     ss<<"\nResulting event yields for the data corresponding to $"
       << std::fixed << std::setprecision(1)<< config->read<double>("Lumi", 0) 
       << "$~fb$^{-1}$, the estimated background, and a signal point with a total signal cross section of $\\sigma_{\\mbox{\\footnotesize NLO}}="
       << std::fixed << std::setprecision(4)<< xsec<<"$~pb. \n";
     ss<<"The combined observed (expected) CLs cross-section limit for this point is  $"
       <<std::fixed << std::setprecision(4)<< xsec * config->read<double>("CLs observed", 0)
       <<"$~pb ($"
       <<std::fixed << std::setprecision(4)<< xsec * config->read<double>("CLs expected", 0)
       <<"$~pb) at $95\\%$~CL.\n"; 
     }
     if (caption) table.SetCaption(ss.str());
     //os << ss.str()<<std::endl;
    }
  }  
    
  os << table;
}

int main( int argc, char* argv[] )
{
   MyConfig* config = new MyConfig(argc, argv);

   int bino=630, wino=640, min=0, max=3, nr=15;
   std::string dir="cMSSM", stylestr;
   Table::TableStyle style = Table::Plain;
   bool caption = false;

   config->addUsage(  "Usage: " );
   config->addUsage(  "" );
   config->addFlag(   "help",   'h',              "Prints this help" );
   config->addFlag(   "caption",' ',              "Prints a caption" );
   config->addOption( "config", 'c',"[file]",     "configuration file (can be surpassed by commandline args) " );
   config->addOption( "style",  's',"[style]",    "Style can be 'tex', 'empty' or 'plain' (default) " );
   //config->addOption( "msquark",' ',"[GeV]",      "m(sq)" );
   //config->addOption( "mgluino",' ',"[GeV]",      "m(gl)" );
   config->addOption( "nr",    ' ',"",           "number of limit point" );
   config->addOption( "bino",  ' ',"[GeV]",      "m(chi^0_1)" );
   config->addOption( "wino",  ' ',"[GeV]",      "m(chi^+-_1)" );
   config->addOption( "min",    ' ',"",           "lower bin (channel); default: 0" );
   config->addOption( "max",    ' ',"",           "upper bin (channel); default: 5" );
   config->addOption( "dir",    'd',"[directory]","directory containing the demo limit result files" );

   config->Process();

   if (config->IsAvailable("dir",'d')) dir = config->Get("dir",'d');
   if (config->IsAvailable("bino"))   bino = StringTo<int>(config->Get("bino"));
   if (config->IsAvailable("wino"))   wino = StringTo<int>(config->Get("wino"));
   if (config->IsAvailable("nr"))      nr = StringTo<int>(config->Get("nr"));
   //if (config->IsAvailable("msquark"))  sq = StringTo<int>(config->Get("msquark"));
   //if (config->IsAvailable("mgluino"))  gl = StringTo<int>(config->Get("mgluino"));
   if (config->IsAvailable("min"))     min = StringTo<int>(config->Get("min"));
   if (config->IsAvailable("max"))     max = StringTo<int>(config->Get("max"));
   if (config->IsAvailable("style")) stylestr = config->Get("style");
   caption = config->IsAvailable("style");
   std::transform(stylestr.begin(), stylestr.end(), stylestr.begin(), std::ptr_fun<int, int>(std::toupper));
   if      (stylestr=="TEX")    style = Table::TeX;
   else if (stylestr=="EMPTY")  style = Table::Empty;

   WriteTable(std::cout,style, dir, nr, wino, bino, min, max, caption);


}
