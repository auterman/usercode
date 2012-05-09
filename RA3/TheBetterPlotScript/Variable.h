#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>

class ConfigFile;
class Variable;
class Info;

///class Variables contains one value like cross-section and a link to static general information of this variable
class Variable{
 public:
  Variable():value_(0),info_(0){}
  Variable(double value, Info*info):value_(value),info_(info){}
  double  GetValue() const {return value_;}
  void    SetValue(double  v){value_=v;}
  Info *  GetInfo() const {return info_;}
  void    SetInfo(Info* i){info_=i;}

 private:
  double value_;
  Info * info_;
};
 
///helper class containing general infromation like the name of the variables. This is supposed to be static.
class Info{
 public:
   Info(const std::string& name, const std::string& name_in_datacard):
        name(name),name_in_datacard(name_in_datacard),default_value(0),use_default(false){}
   Info(const std::string& name, const std::string& name_in_datacard, double default_value):
        name(name),name_in_datacard(name_in_datacard),default_value(default_value),use_default(true){}
   const std::string name;
   const std::string name_in_datacard;
   const double default_value;
   const bool use_default;
   
   void Fill(Variable& v, ConfigFile& c);
};


Variable ReadVariable(ConfigFile& c, const std::string& name, const std::string& name_in_datacard);

Variable ReadVariable(ConfigFile& c, const std::string& name, const std::string& name_in_datacard, const double default_value);

#endif
