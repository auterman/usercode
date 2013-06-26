// $Id: Event.h,v 1.00 2013/05/26 20:00:00 auterman Exp $

/*** ------------------------------------------------------------------------------------------------------- ***
     NTupleFactory, a tool to plot final results, using Root Reflex information

     Christian Autermann, RWTH Aachen, May 2013
 *** ------------------------------------------------------------------------------------------------------- ***/

#ifndef EVENT_H
#define EVENT_H

#include "Reflex/Type.h"
#include "Reflex/Member.h"
#include "Reflex/Object.h"
#include "TROOT.h"


#include <string>
#include <vector>
#include <iostream>
#include <sstream>

const unsigned ARRAYSIZE = 10;


//Stores information on the used samples
class Sample {
 public:
  Sample(const std::vector<std::string> files, const std::string& Tree, const std::string& Selector, const std::string& Name, const std::string& Label, float weight):
     files_(files),Tree_(Tree),Selector_(Selector),Name_(Name),Label_(Label),weight_(weight),nevents_(0),
     fillcolor_(-1),linecolor_(-1),linestyle_(-1),markerstyle_(-1),markercolor_(-1){}
  
  void AddFile(const std::string& file){files_.push_back(file);}
  void SetTree(const std::string& Tree){Tree_=Tree;};
  void SetSelector(const std::string& Selector){Selector_=Selector;}
  void SetName(const std::string& Name){Name_=Name;}
  void SetLabel(const std::string& label){Label_=label;}
  void SetWeight(const float weight){weight_=weight;}
  void SetNevents(const long int nevents){nevents_=nevents;}
  void SetFillColor(int fillcolor){fillcolor_=fillcolor;}
  void SetLineColor(int linecolor){linecolor_=linecolor;}
  void SetLineStyle(int linestyle){linestyle_=linestyle;}
  void SetMarkerStyle(int markerstyle){markerstyle_=markerstyle;}
  void SetMarkerColor(int markercolor){markercolor_=markercolor;}
  const std::vector<std::string>* GetFiles() const {return &files_;}
  std::string GetTree( )        const {return Tree_;};
  std::string GetSelector( )    const {return Selector_;}
  std::string GetName( )        const {return Name_;}
  std::string GetLabel( )       const {return Label_;}
  float       GetWeight( )      const {return weight_;}
  long int    GetNevents( )     const {return nevents_;}
  int         GetFillColor( )   const {return fillcolor_;}
  int         GetLineColor( )   const {return linecolor_;}
  int         GetLineStyle()    const {return linestyle_;}
  int         GetMarkerStyle( ) const {return markerstyle_;}
  int         GetMarkerColor( ) const {return markercolor_;}

 private:
  std::vector<std::string> files_;
  std::string Tree_, Selector_, Name_, Label_;
  float weight_;
  long int nevents_;
  int fillcolor_, linecolor_, linestyle_, markerstyle_, markercolor_;
};

class Event {
 public:
   ~Event();
      
   /// Access for all MySelector member variables, e.g. Float_t met by Get<Float_T>("met")
   template<typename T>
   T Get(const std::string& var) const { 
         std::map<const std::string,const void*>::const_iterator u=uservars_.find(var);
         return ( u!=uservars_.end()
	          ? *((const T*)(u->second))
                  : Reflex::Object_Cast<T>(o.Get(var))); }
   
   /// Allow to add user calculated variables to the event.
   /// This additional information is not pushed into the root reflex object (though possible),
   /// but simply stored in a map, because of simplicity and higher speed
   template<typename T>
   void AddVariable(const std::string& name, const T* val){uservars_[name]=val;}
   
   /// Information on the sample the given event originates from
   const Sample * GetSample() const {return sample_;}
   void  SetSample(const Sample* sample){sample_=sample;}

   ///Direct access to root reflex object; this is used only when the events are read 
   const ROOT::Reflex::Object & GetRObject() const {return o;};
   void SetRObject(const ROOT::Reflex::Object& obj){ o=obj; }
   
 private:  
   ROOT::Reflex::Object o;
   const Sample * sample_;
   std::map<const std::string,const void*> uservars_;
};



/// Variable Types - allow to consistently access all variables types by the same methods

class Variable {
public:
 Variable(const std::string& name):name_(name){};
 virtual ~Variable(){}
 Float_t operator()(const Event* evt) const {return Get(evt);};
 virtual std::string Name() const {return name_;}
 virtual std::string Label() const {return name_;}

protected:
 virtual Float_t Get(const Event * evt) const {return 0;};
 std::string name_;
};

class FloatVariable : public Variable {
 public:
  FloatVariable(const std::string& name):Variable(name){};
 protected:
  virtual Float_t Get(const Event * evt) const {return evt->Get<Float_t>(name_);}
};

class IntVariable : public Variable {
 public:
  IntVariable(const std::string& name):Variable(name){};
 protected:
  virtual Float_t Get(const Event * evt) const {return (Float_t)evt->Get<Int_t>(name_);}
};

struct FloatArray {
  Float_t val[ARRAYSIZE];
};

struct IntArray {
  Int_t val[ARRAYSIZE];
};


class FloatArrayVariable : public Variable {
 public:
  FloatArrayVariable(const std::string& name, unsigned index):Variable(name),index_(index){};  
  virtual std::string Name() const {
    std::stringstream ss;
    ss<<name_<<"["<<index_<<"]";
    return ss.str();
  }
 virtual std::string Label() const {
    std::stringstream ss;
    ss<<name_<<index_;
    return ss.str();
  }
 protected:
  unsigned index_;
  virtual float Get(const Event * evt) const {return (evt->Get<FloatArray>(name_)).val[index_];}
};


#endif
