#include "Combine.h"

#include <sstream>
#include <map>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <algorithm>

void Parser::Line(const std::string& key, const std::string& val, const std::string& label)
{
  std::stringstream ss(val);
  if (key=="observation") { 
    int i=0, d;
    while (ss>>d&&i<imax) {
      if (i>=(int)channels.size()) { channels.push_back(Channel()); 
                                //std::cout<<"observation = "<<d<<", channel = "<<i<<std::endl;
			      }
      channels[i++].observation = d;
    }
  }
  
  bool ok = true;    
  if (key=="rate")  
    for (int i=0;(ok&&i<imax);++i) {
      for (int j=0;(ok&&j<=jmax);++j) {
        //if (j>=channels[i].processes.size()) channels[i].processes.push_back(Process());
        double r=0;
	ok = ss >> r;
	if (!ok) break;
	//std::cout<<"i="<<i<<", j="<<j<<", ok? "<<ok<<std::endl;
        if (i>=(int)channels.size()) channels.push_back(Channel());
        channels[i].processes[j].rate = r;
	//std::cout<<"channels[i="<<i<<"].processes[j="<<j<<"].rate = r="<<r<<";   channels.size()="<<channels.size()<<std::endl;
      }
    }
    
  if (key=="process")  
    for (int i=0;(!ss.eof()&&i<imax);++i) {
      for (int j=0;(!ss.eof()&&j<=jmax);++j) {
        //if (j>=channels[i].processes.size()) channels[i].processes.push_back(Process());
	std::string label;
	ok = ss >> label;
	if (!ok) break;
        if (i>=(int)channels.size()) channels.push_back(Channel());
        channels[i].processes[j].label += (channels[i].processes[j].label==""?label:"_"+label);
      }
    }

  //else uncertainties:
  //first check if the 'key' uncertainty is a correlated uncertainty accoding to the rules
  std::map<std::string, std::string> alias_map;
  std::vector<std::string> u_corr;
  if (rules_)
    u_corr = bag_of_string(rules_->read<std::string>("u_corr",";"));
  for (std::vector<std::string>::iterator it=u_corr.begin(); it!=u_corr.end(); ++it) {
    std::vector<std::string> u_alias = bag_of_string(rules_->read<std::string>(*it));
    for (std::vector<std::string>::iterator al=u_alias.begin(); al!=u_alias.end(); ++al) 
       alias_map[*al]=*it;    
  }
  std::string alias(label+"_"+key);
  if (key!="bin" && alias_map.find(key)!=alias_map.end() ) 
    alias = alias_map[key];

  //second read and fill uncertainties
  std::string s, statistic;
  int i=0, j=0;
  while (ss>>s) {
    if (s=="lnN") {
      statistic=s;
      continue;
    } else if (s=="gmN") {
      statistic=s;
      ss >> s;
      statistic += " "+s;
      continue;    
    } else if (key=="bin") break;
    else {
      std::istringstream v(s);
      double value;
      if (s=="-") value=1;
      else {
        ok = v >> value;
	if (!ok) break;
      }	
      //std::cout<<"i="<<i<<", j="<<j<<", s="<<s<<", ok? "<<ok<<std::endl;
      if (i>=(int)channels.size()) channels.push_back(Channel());
      channels[i].uncertainties[std::make_pair(Uncertainty(alias,statistic),j)] = value;
      if (std::find(uncertainties_.begin(), uncertainties_.end(), Uncertainty(alias,statistic))==uncertainties_.end()) 
        uncertainties_.push_back(Uncertainty(alias,statistic));
      ++j;
      if (j>jmax) {
        j=0;
	++i;
      }
    }
  }
    
}

void Parser::ExpandUncertainties(std::vector<Uncertainty> otherUncs)
{
  for (std::vector<Channel>::iterator bin=channels.begin(); bin!=channels.end(); ++bin) {
    for (std::vector<Uncertainty>::iterator unc=otherUncs.begin(); unc!=otherUncs.end(); ++unc){
      if (bin->uncertainties.find(std::make_pair(*unc,0))==bin->uncertainties.end() ) {//found a non-existing unc
        //std::cout << unc->label << " is not present in this point (bin "<<bin-channels.begin() <<" )"<<std::endl;
        if (std::find(uncertainties_.begin(), uncertainties_.end(), *unc)==uncertainties_.end()) 
	  uncertainties_.push_back(*unc);
	for (int j=0;j<=jmax;++j)//s+backgds
	  bin->uncertainties[std::make_pair(*unc,j)] = 1.0;
      }
    }  
  }
}

void Parser::ExpandProcesses(unsigned n)
{
  for (std::vector<Channel>::iterator it=channels.begin(); it!=channels.end(); ++it){ 
    for(unsigned i=it->processes.size();i<n;++i) {
      std::stringstream ss;
      ss << "dummy_" << i;
      it->processes[i] = Process(ss.str(), 0.0);
    }
  }  
}


Parser::Parser(const std::string& filename, ConfigFile* r)
{
  //std::map<string,string> myContents;   
  std::ifstream is( filename.c_str() );
  if( !is ) {
    std::cerr<<"Cannot find file '"<<filename<<"'"<<std::endl;
    throw filename; 
  }  
  rules_ = r; 
  //bkg alias names
  std::vector<std::string> bkgs = bag_of_string(r->read<std::string>("bkgds",";"));
  for (std::vector<std::string>::const_iterator it=bkgs.begin();it!=bkgs.end();++it) {
    std::vector<std::string> bkg = bag_of_string(r->read<std::string>(*it,";"));
    for (std::vector<std::string>::const_iterator b=bkg.begin();b!=bkg.end();++b) 
      bkg_alias_[*b] = *it; 
  }
  // Load a ConfigFile from is
  // Read in keys and values, keeping internal whitespace
  typedef string::size_type pos;
  const string& delim  = " ";  // separator
  const string& comm   = "#";	 // comment
  const pos skip = delim.length();	  // length of separator

  string nextline = "";  // might need to read ahead to see where value ends
  while( is || nextline.length() > 0 )
  {
  	  // Read an entire line at a time
  	  string line;
  	  if( nextline.length() > 0 )
  	  {
  		  line = nextline;  // we read ahead; use it now
  		  nextline = "";
  	  }
  	  else
  	  {
  		  std::getline( is, line );
  	  }
	  
	  if (line.find(comm)!=string::npos)
	    comments_.push_back( line.substr(line.find(comm) ) );
  	  
  	  // Ignore comments
  	  line = line.substr( 0, line.find(comm) );
  			  
  	  // Parse the line if it contains a delimiter
  	  pos delimPos = line.find( delim );
  	  if( delimPos < string::npos )
  	  {

  		  // Extract the key
  		  string key = line.substr( 0, delimPos );
  		  line.replace( 0, delimPos+skip, "" );
  		  
  		  // See if value continues on the next line
  		  // Stop at blank line, next line with a key, end of stream,
  		  bool terminate = false;
  		  while( !terminate && is )
  		  {
  			  std::getline( is, nextline );
  			  terminate = true;
  			  
  			  string nlcopy = nextline;
  			  ConfigFile::trim(nlcopy);
  			  if( nlcopy == "" ) continue;
  			  
  			  nextline = nextline.substr( 0, nextline.find(comm) );
  			  if( nextline.find(delim) != string::npos )
  				  continue;
  			  nlcopy = nextline;
  			  ConfigFile::trim(nlcopy);
  			  if( nlcopy != "" ) line += "\n";
  			  line += nextline;
  			  terminate = false;
  		  }
  		  
  		  // Store key and value
  		  ConfigFile::trim(key);
  		  ConfigFile::trim(line);
  		  
		  //myContents[key] = line;  // overwrites if key is repeated
		  if      (key=="imax") imax = ConfigFile::string_as_T<int>( line ); 
		  else if (key=="jmax") jmax = ConfigFile::string_as_T<int>( line );
		  else if (key=="kmax") kmax = ConfigFile::string_as_T<int>( line );
		  else                  Line(key, line, filename.substr(0, filename.find("/")) ); 
  	  }
  }
//  assert(imax==channels.size());
//  for (int i=0;i<imax;++i) {
//    assert(imax*jmax==channels[i].processes.size());
//    assert(kmax==channels[i].uncertainties.size());
//  }
}
  
void Parser::Print()
{
  std::cout<<"imax = "<<imax<<", jmax = "<<jmax<<", kmax = "<<kmax<<std::endl;
  for (std::vector<Channel>::iterator it=channels.begin(); (it!=channels.end()&&(it-channels.begin())<imax); ++it) {
    std::cout << "\nChannel "<<it-channels.begin()<<std::endl
              << "Obs = " << it->observation << std::endl;
    
    for (std::map<int,Process>::iterator pi=it->processes.begin(); pi!=it->processes.end(); ++pi) {
      std::cout << pi->first<< ".: " << pi->second.label << " = " << pi->second.rate <<std::endl;
    }	
    
    for (Channel::UncMap::iterator un=it->uncertainties.begin(); 
         un!=it->uncertainties.end(); ++un){
      //if (it-channels.begin() == un->first.second)      
      std::cout << "Sample "<< un->first.second<< " "<< un->first.first.label<< " " << un->first.first.pdf << " = " << un->second <<std::endl;
    }
  }  
}


void MergeFiles::Add(const std::string&  s)
{
  Parser * a = new Parser(s, rules_);
  assert( a->imax == (int)a->channels.size());
  
  f_->ExpandUncertainties( a->GetUncertainties() );
  a->ExpandUncertainties( f_->GetUncertainties() );
  assert( f_->GetUncertainties().size() == a->GetUncertainties().size() );
  
  f_->ExpandProcesses( a->GetNProcesses() );
  a->ExpandProcesses( f_->GetNProcesses() );
  assert( f_->GetNProcesses() == a->GetNProcesses() );
  
  
  history_= history_ + " + " + s;
  f_->imax += a->imax;
  if (a->jmax>f_->jmax) f_->jmax=a->jmax;
  
  f_->kmax = f_->GetUncertainties().size();
  
  //std::cout<<"imax = "<<f_->imax<<", jmax = "<<f_->jmax<<", kmax = "<<f_->kmax<<std::endl;
  f_->channels.insert(f_->channels.end(),a->channels.begin(),a->channels.end());

  assert( f_->imax == (int)f_->channels.size());

}

std::ostream& operator<<( std::ostream& os, const MergeFiles& f )
{
  //Meta info  
  time_t rawtime;
  struct tm * timeinfo;
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  os << "## " << getenv("USER") 
     << ": " << asctime (timeinfo) 
     << "## Combined "<<f.history_<<"\n";

  os << "imax  " << f.f_->imax <<"\n";
  os << "jmax  " << f.f_->jmax <<"\n";
  os << "kmax  " << f.f_->kmax <<"\n";
  os << "------------\n## observed events\nbin        ";
  for (int i=0;i<f.f_->imax;++i)
    os<< "  "<<i;
  os << "\nobservation";  
  for (int i=0;i<f.f_->imax;++i)
    os<< "  "<<f.f_->channels[i].observation;
  os << "\n------------\n## expected events\nbin    ";
  for (int i=0;i<f.f_->imax;++i)
    for (int j=0;j<=f.f_->jmax;++j)
      os<< "  "<<i;
  os << "\nprocess";
  for (int i=0;i<f.f_->imax;++i)
    for (int j=0;j<=f.f_->jmax;++j)
      os<< "  "<< f.f_->bkg_alias_[f.f_->channels[i].processes[j].label];
  os << "\nprocess";
  for (int i=0;i<f.f_->imax;++i)
    for (int j=0;j<=f.f_->jmax;++j)
      os<< "  "<<j;
  os << "\nrate   ";
  for (int i=0;i<f.f_->imax;++i)
    for (int j=0;j<=f.f_->jmax;++j)
      os<< "  "<<f.f_->channels[i].processes[j].rate;
  os << "\n------------\n## uncertainties";
/*
  for (int i=0;i<f.f_->imax;++i) {
    Channel::UncMap::iterator un=f.f_->channels[i].uncertainties.begin();
    os << "\n"<<un->first.first.label<<" "<<un->first.first.pdf;
    for (;un!=f.f_->channels[i].uncertainties.end();++un) {
       os << "  "<<un->second;
    }
  }   
*/
  for (Channel::UncMap::iterator un=f.f_->channels.front().uncertainties.begin();
       un!=f.f_->channels.front().uncertainties.end();++un) {
    if (un->first.second!=0) continue;
    os << "\n"<<un->first.first.label<<" "<<un->first.first.pdf;
    for (int i=0;i<f.f_->imax;++i) {
      for (int j=0;j<=f.f_->jmax;++j)
        if (f.f_->channels[i].uncertainties.find(std::make_pair(un->first.first,j))!=f.f_->channels[i].uncertainties.end())
          os << "  " << f.f_->channels[i].uncertainties[std::make_pair(un->first.first,j)];
	else
	  os << "  -  ";
    }
  }
  os << "\n" << std::endl;
  
  std::vector<std::string> c = f.f_->GetComments();
  for (std::vector<std::string>::const_iterator it=c.begin();it!=c.end();++it)
    os << *it << "\n";
  
  os<<std::endl;
  return os;
}

