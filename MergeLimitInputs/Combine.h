#ifndef COMBINE_H
#define COMBINE_H

#include "ConfigFile.h"

#include <string>

struct Process{
  Process(){};
  Process(const std::string& l, double r):label(l),rate(r){};
  std::string label;
  double rate;
};

struct Uncertainty{
  Uncertainty(const std::string& l, const std::string& p):label(l),pdf(p){};
  std::string label;
  std::string pdf;
  bool operator<(const Uncertainty& r) const {return label<r.label;}
  bool operator==(const Uncertainty& r) const {return label==r.label&&pdf==r.pdf;}
};

struct Channel{
  typedef std::map<std::pair<const Uncertainty,const int>,double> UncMap;
  UncMap uncertainties;
  int observation;
  std::map<int,Process> processes;
};  

class Parser{
 public:
  Parser(const std::string& file, ConfigFile* r=0);
  void Print(); //for debugging
  
  int imax;
  int jmax;
  int kmax;  
  std::vector<Channel> channels;
  std::map<std::string, std::string> bkg_alias_;

  std::vector<std::string> GetComments(){return comments_;}
  std::vector<Uncertainty> GetUncertainties(){return uncertainties_;};
  void ExpandUncertainties(std::vector<Uncertainty> );
  void ExpandProcesses(unsigned n);
  unsigned GetNProcesses(){return channels.front().processes.size();};

  protected:
  void Line(const std::string& key, const std::string& val, const std::string& label=""); 
  std::vector<std::string> comments_;
  std::vector<Uncertainty> uncertainties_;
  ConfigFile * rules_;
};



class MergeFiles{
 public:
  MergeFiles(const std::string& f, ConfigFile* r):rules_(r),history_(f){f_=new Parser(f,r);};
  friend std::ostream& operator<<( std::ostream& os, const MergeFiles& cf );
   
  void Add(const std::string& s);
  
  void Print(){f_->Print();}

 private:
  Parser* f_;
  ConfigFile* rules_;
  std::string history_;
};

#endif
