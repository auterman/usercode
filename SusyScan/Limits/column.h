#ifndef COLUMN_H
#define COLUMN_H

#include <string>
#include <vector>
#include <sstream>

#include <iostream>

//virtual base class
class TColumnBase
{
 public:
  TColumnBase():_header(""){};
  TColumnBase(std::string header):_header(header){};
  virtual ~TColumnBase(){};

  std::string         GetHeader(){return _header;};
  void                SetHeader(const std::string & header){_header=header;};
  virtual void        Add(const void * ){};
  virtual void        SetPrecision(unsigned p){};
  virtual unsigned    Size(){return 0;};
  virtual std::string Str(const unsigned i){return "";};
  virtual unsigned    Width(){return 10;};

 protected:
  std::string _header;
};


//A templated class for each column of class table
template<typename T> 
class TColumn : public TColumnBase {
 public:
  TColumn(std::string header){SetHeader(header);_precision=1;};
  virtual ~TColumn() {
    for (typename std::vector<const T*>::iterator it=_field.begin(); it!=_field.end(); ++it) delete *it;
    _field.clear();}
  
  virtual void     SetPrecision(unsigned p){_precision=p;};
  virtual void     Add(const void * f){ _field.push_back( static_cast<const T*>(f));};
  void             Set(unsigned i, T f){_field.at(i)=f;};
  T                At(const unsigned i){return _field.at(i);};
  virtual unsigned Size(){return _field.size();};
  std::string      Str(const unsigned i){
    if (i<0||i>=_field.size()) return "";
    std::stringstream ss;
    ss<<std::fixed << std::setprecision(_precision)<<std::setw(_width)<< *_field[i];
    return ss.str();
  };
  virtual unsigned Width(){
    std::stringstream ss;
    ss << _header;
    unsigned max=ss.str().size();
    for (typename std::vector<const T*>::const_iterator it=_field.begin();it!=_field.end();++it){
      std::stringstream ss;
      ss <<std::fixed << std::setprecision(_precision) << **it;
      if (ss.str().size()>max) max=ss.str().size();
    }
    _width=max;
    return max; 
  }

 protected:
  std::vector<const T*> _field;
  unsigned _width;
  unsigned _precision;
};

#endif
