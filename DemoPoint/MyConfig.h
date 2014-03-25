#ifndef MYCONFIG_H
#define MYCONFIG_H

#include <string>

#include "anyoption.h"
#include "ConfigFile.h"

const static bool DEBUG = false;

class MyConfig {
 public:
  MyConfig(int argc, char* argv[] ):argc_(argc),argv_(argv),confStr_("config"),confChar_('c'),i_usage_(0){opt_ = new AnyOption();};
  void Process();
  ~MyConfig(){
    delete opt_;
    if (config_) delete config_;
  }

  //return 0 if not found, <0 if from config, >0 if from command line
  short Get(std::string& val,const std::string& label, const char& =' ');
  //throws if not found
  std::string Get(const std::string& label, const char& =' ');
  std::string GetFromCL(const std::string& label, const char& =' ');
  std::string GetFromConf(const std::string& label);
  bool  IsAvailable(const std::string& label, const char& =' ');
  bool  IsAvailableInCL(const std::string& label, const char& =' ');
  bool  IsAvailableInConf(const std::string& label);

  //Configuration
  void SetConfigStr(const std::string& str){confStr_=str;}
  
  void addUsage( const std::string& str, char c=' ', const std::string& value="", const std::string& usage="");
  void addFlag(  const std::string& str, char c=' ', const std::string& usage="");
  void addOption(const std::string& str, char c=' ', const std::string& value="", const std::string& usage="");
   
  void printUsage();

 private:
  struct Usage{
    Usage():label(""),c(' '),value(""),comment(""){}
    Usage(const std::string& label, char c=' ', const std::string& value="", const std::string& comment=""):
          label(label),c(c),value(value),comment(comment){}
    std::string label, comment, value; char c; 
    unsigned length(){return label.size()+value.size()+(c==' '?0:4);}
  };
  
  int argc_;
  char ** argv_;
  std::string confStr_;
  char confChar_;
  unsigned i_usage_;
  std::map<unsigned,Usage> use_;
  AnyOption*opt_;
  ConfigFile*config_;
};


#endif
