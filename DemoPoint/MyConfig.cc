#include "MyConfig.h"

#include <iostream>


void MyConfig::Process(){
  if (DEBUG) {
    std::cout<<"MyConfig::Process() ";
    for (int i=0; i<argc_;++i)
      std::cout <<"   "<<i<<".: "<<argv_[i];
    std::cout <<std::endl;  
  }

  opt_->processCommandArgs( argc_, argv_ );
  std::cout<< opt_->getFlag( 'h' ) <<std::endl;
  if (IsAvailableInCL(confStr_,confChar_)) config_=new ConfigFile(GetFromCL(confStr_,confChar_));
  if (IsAvailableInCL("help",'h')) printUsage();
}  


///return 0 if not found, <0 if from config, >0 if from command line
short MyConfig::Get(std::string& val, const std::string& label, const char&c)
{
  if (IsAvailableInCL(  label,c)) { 
    val=GetFromCL(label,c);
    return 1;  
  }  
  if (config_ && IsAvailableInConf(label)) {
    val=GetFromConf(label);
    return -1;
  }
  return false;
}

///throws if not found
std::string MyConfig::Get(const std::string& label, const char&c)
{
  if (IsAvailableInCL(  label,c)) return GetFromCL(label,c);  
  if (config_ && IsAvailableInConf(label)) return GetFromConf(label);
  throw "Label "+label+" ("+c+") not found in commandline [or --config ...]";
}

///throws if not found
std::string MyConfig::GetFromCL(const std::string& label, const char&c)
{
  return opt_->getValue( label.c_str() );
}

///throws if not found
std::string MyConfig::GetFromConf(const std::string& label)
{
  return config_->read<std::string>(label);
}

bool  MyConfig::IsAvailable(const std::string& label, const char&c)
{
  return (IsAvailableInCL(label,c) || (config_ && IsAvailableInConf(label)));
}

bool  MyConfig::IsAvailableInCL(const std::string& label, const char&c)
{
  bool result = ( opt_->getValue( label.c_str() ) != NULL  || 
                (c!=' '&&opt_->getValue( c ) != NULL) );
	   
  if (DEBUG) //std::cout << label <<" ("<<c<<") in CL? ->"<< result << endl;
  std::cout << label << " ? "
            << (opt_->getValue( label.c_str() ) != NULL)
	    << opt_->getFlag( label.c_str() )
	    << ";  "<< c <<" ? "
            << (opt_->getValue( c ) != NULL)
	    << opt_->getFlag( c )
	    <<std::endl;
	    
  
  return result;
}

bool  MyConfig::IsAvailableInConf(const std::string& label)
{
  if (DEBUG) std::cout << label <<" in ConfigFile ? ->"<< config_->keyExists( label ) << endl;
  return config_->keyExists( label );
}



void MyConfig::addUsage( const std::string& str, char c, const std::string& value, const std::string& usage)
{
  use_[i_usage_++] = Usage(str,c,value,usage);
}

void MyConfig::addFlag(  const std::string& str, char c, const std::string& usage)
{
  use_[i_usage_++] = Usage(str,c,"",usage);
  if (c!=' ') opt_->setFlag(str.c_str(),c);
  else opt_->setFlag(str.c_str());
}

void MyConfig::addOption(const std::string& str, char c, const std::string& value, const std::string& usage)
{
  use_[i_usage_++] = Usage(str,c,value,usage);
  if (c!=' ') opt_->setOption(str.c_str(),c);
  else opt_->setOption(str.c_str());
}
 
void MyConfig::printUsage(){
  unsigned max=0;
  for (std::map<unsigned,Usage>::iterator it=use_.begin();it!=use_.end();++it)
    if (it->second.length()>max) max=it->second.length();
  
  for (std::map<unsigned,Usage>::iterator it=use_.begin();it!=use_.end();++it){
    std::string str="";
    if (it->second.c!=' '){
      str = " -";
     // str += it->second.c;
      str +=" ";
    }  
    str="--"+it->second.label
    	+"  "+it->second.value
    	+"	    "+it->second.comment;
    opt_->addUsage(str.c_str());
    std::cout<<str<<std::endl;
  }
  //opt_->printUsage();
}

