#include "variable.h"
#include "ConfigFile.h"


void Info::Fill(Variable& v, ConfigFile& c)
{
  if (use_default) v.SetValue( c.read<double>(name_in_datacard, default_value) ); 
  else             v.SetValue( c.read<double>(name_in_datacard ) ); 
  v.SetInfo(this);
}


Variable ReadVariable(ConfigFile& c, const std::string& name, const std::string& name_in_datacard)
{
  Variable var;
  Info(name, name_in_datacard).Fill(var, c);
  return var;
}

Variable ReadVariable(ConfigFile& c, const std::string& name, const std::string& name_in_datacard, const double default_value)
{
  Variable var;
  Info(name, name_in_datacard, default_value).Fill(var, c);
  return var;
}
