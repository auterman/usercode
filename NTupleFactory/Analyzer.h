// $Id: Analyzer.h,v 1.00 2013/05/26 20:00:00 auterman Exp $

/*** ------------------------------------------------------------------------------------------------------- ***
     NTupleFactory, a tool to plot final results, using Root Reflex information

     Christian Autermann, RWTH Aachen, May 2013
 *** ------------------------------------------------------------------------------------------------------- ***/

#ifndef ANALYZER_H
#define ANALYZER_H

#include <vector>
#include <map>
#include <string>

class Event;
class Variable;
class Selection;
class Sample;
class TH1;
class TH2;
class THStack;
class TCanvas;
namespace Table{
  class TTable;
};

// Virtual Base Class --------------------------------------------------------------
class Analyzer {
 public:
  Analyzer(){}
  virtual ~Analyzer(){}
  virtual void InitJob(){}
  virtual void InitSample(const Selection*sel,const Sample* sample){}
  virtual void Process(const Event*evt){}
  virtual void Produce(Event*evt){}
  virtual void EndSample(){}
  virtual void EndJob(){}
};
// ---------------------------------------------------------------------------------



// A few use case examples ---------------------------------------------------------

class Plot1D : public Analyzer {
 public:
   Plot1D(TCanvas*c,TH1*h, Variable*v1,Variable*v2=0):Analyzer(),c_(c),h_(h),v1_(v1),v2_(v2){};
   virtual void InitSample(const Selection*sel,const Sample* sample);
   virtual void Process(const Event*evt);
   virtual void EndSample();
   void SetFileName(const std::string& s){filename_=s;}
   TH1*Get(){return h_;}
 private:
   TCanvas*c_;
   TH1*h_;
   Variable * v1_, *v2_;
   std::string filename_;
};

class Plot2D : public Analyzer {
 public:
   Plot2D(TCanvas*c,TH2*h, Variable*v1,Variable*v2=0,Variable*v3=0):Analyzer(),c_(c),h_(h),v1_(v1),v2_(v2),v3_(v3){};
   virtual void InitSample(const Selection*sel,const Sample* sample);
   virtual void Process(const Event*evt);
   virtual void EndSample();
   void SetFileName(const std::string& s){filename_=s;}
   TH2*Get(){return h_;}
 private:
   TCanvas*c_;
   TH2*h_;
   Variable * v1_, *v2_, *v3_;
   std::string filename_;
};

class Print : public Analyzer {
 public:
   Print(Variable*v1,Variable*v2=0,Variable*v3=0):Analyzer(),processed_(0),v1_(v1),v2_(v2),v3_(v3){};
   virtual void Process(const Event*evt);
 protected:
  long int processed_;
  Variable * v1_, *v2_, *v3_;
};

class PrintStatus : public Analyzer {
 public:
   PrintStatus(unsigned n):Analyzer(),n_(n),processed_(0),nentries_(1){};
   virtual void InitSample(const Selection*sel,const Sample* sample);
   virtual void Process(const Event*evt);
 protected:
  int n_;
  long int processed_,nentries_;
};


class CutFlow : public Analyzer {
 public:
  CutFlow(const Selection*sel, Variable * weight=0):Analyzer(),weight_(weight){
    cuts_.push_back(sel);}
  virtual void InitSample(const Selection*sel,const Sample* sample);
  virtual void Process(const Event*evt);
  virtual void EndJob();
  void AddSelection(const Selection*sel){cuts_.push_back(sel);}
  Table::TTable * GetTable(Sample* s){return tables_[s];}
  void PrintToFile(const std::string& file){file_=file;}
 private:
  Variable * weight_;
  std::vector<const Sample*> s_;
  std::vector<const Selection*> cuts_;
  std::map<std::pair<const Sample*,const Selection*>,double> total, first, nminus1, final; 
  std::map<const Sample*,Table::TTable*> tables_;
  std::string file_;
};


class Stack : public Analyzer {
 public:
   Stack(TCanvas*c,TH1*h, Variable*v1,Variable*v2=0):Analyzer(),c_(c),h_(h),stack_(0),v1_(v1),v2_(v2){};
   virtual void InitSample(const Selection*sel,const Sample* sample);
   virtual void Process(const Event*evt);
   virtual void EndJob();
   void SetFileName(const std::string& s){filename_=s;}
   THStack*Get(){return stack_;}
 private:
   TCanvas*c_;
   TH1*h_;
   THStack*stack_;
   Variable * v1_, *v2_;
   std::string filename_;
   TH1*h_active_;
   std::map<std::string,TH1*> hists_;
};



#endif
