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
#include "anyoption.h"
#include "Combine.h"


void Combine(const std::vector<std::string>& s, ConfigFile* rules)
{
  std::vector<std::string>::const_iterator it = s.begin();
  std::cout << "mergeInputs: '"<<*it<<"'"<<std::flush;
  MergeFiles merged( *it, rules );
  
  for (++it; it!=s.end(); ++it)
  {
    std::cout<<" + '"<<*it<<"'"<<std::flush;
    merged.Add(*it);    
  }
  
  //print for debug
  if (rules->read<bool>("debug", false)) merged.Print();
  
  //write combined datacard to file:
  std::string out = rules->read<std::string>("output","out.txt");
  std::cout<<" to '"<<out<<"'"<<std::endl;
  ofstream ofile;
  ofile.open(out.c_str(), ios::out );
  ofile << merged;
}

int main(int argc, char* argv[]) 
{
   //possible configurations
   AnyOption *opt = new AnyOption();
   opt->addUsage( "mergeInputs [-hro] [files...]\n\nUsage:" );
   opt->addUsage( "" );
   opt->addUsage( " -h  --help                     Prints this help " );
   opt->addUsage( " -r  --rules                    Configuration file with rules for the combination" );
   opt->addUsage( " -o  --output                   set name of output file" );
   opt->setFlag(    "help",   'h' );
   opt->setOption(  "rules",  'r' );
   opt->setOption(  "output", 'o' );
   opt->processCommandArgs( argc, argv );

   //print help
   if( opt->getFlag( "help" ) || opt->getFlag( 'h' ) ) opt->printUsage();
 
   //check for and read config file
   std::string c="";
   if( opt->getValue( 'r' ) != NULL  || opt->getValue( "rules"  ) != NULL  )  
      c = (string)opt->getValue( 'r' );
   ConfigFile * config;
   if (c!="") config=new ConfigFile(c);
   else config = new ConfigFile();

   //over ride defaults or configuration from file by commandline:
   if( opt->getValue( 'o' ) != NULL  || opt->getValue( "output"  ) != NULL  )  
      config->add("output", (string)opt->getValue( 'o' ));

   //finally, get list of data cards to combine:
   std::vector<std::string> signals;
   for( int i = 0 ; i < opt->getArgc() ; i++ ) 
     signals.push_back( (std::string)opt->getArgv( i ) );
   
   //Combine stuff
   if (signals.size()>0)
     Combine(signals, config);
   else
     std::cerr<<"Error: Nothing to merge! Try 'mergeInputs --help'..."<<std::endl;
}
