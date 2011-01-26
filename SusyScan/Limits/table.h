#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <iomanip>

#include "column.h"

class TTable {
 public:
  enum TableStyle {Plain, TeX};

  TTable():_header(""),_delimiter(" "),_caption(""),_style(Plain){};
  TTable(std::string h):_header(h),_delimiter(" "),_caption(""),_style(Plain){};
  TTable(std::string h, std::string d):_header(h),_delimiter(d),_caption(""),_style(Plain){};
  TTable(std::string h,std::string d,std::string c):_header(h),_delimiter(d),_caption(c),_style(Plain){};
  virtual ~TTable(){
    for (std::vector<TColumnBase*>::iterator it=_table.begin(); it!=_table.end(); ++it) delete *it;
    _table.clear();};
  template <class T> void AddColumn(std::string header){_table.push_back(new TColumn<T>(header));};
  template <class T> void Set(unsigned idx,const T &value){
    _table.at(idx)->Add(new T(value));};
  template <class T> void Set( unsigned x, unsigned y, T value){static_cast<TColumn<T> >(_table.at(x))->Set(y,value);};

  template <class T1> void Fill(const T1 &v1){
    Fill<T1,short>(v1,0);}
  template <class T1, class T2> void Fill(const T1 &v1, const T2 &v2){
    Fill<T1,T2,short>(v1,v2,0);}
  template <class T1, class T2, class T3> void Fill(const T1 &v1, const T2 &v2, const T3 &v3){
    Fill<T1,T2,T3,short>(v1,v2,v3,0);}
  template <class T1, class T2, class T3, class T4> void Fill(const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4){
    Fill<T1,T2,T3,T4,short>(v1,v2,v3,v4,0);}
  template <class T1, class T2, class T3, class T4, class T5> void Fill(const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4, const T5 &v5){
    Fill<T1,T2,T3,T4,T5,short>(v1,v2,v3,v4,v5,0);}
  template <class T1, class T2, class T3, class T4, class T5, class T6> void Fill(const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4, const T5 &v5, const T6 &v6){
    Fill<T1,T2,T3,T4,T5,T6,short>(v1,v2,v3,v4,v5,v6,0);}
  template <class T1, class T2, class T3, class T4, class T5, class T6, class T7> void Fill(const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4, const T5 &v5, const T6 &v6, const T7 &v7){
    Fill<T1,T2,T3,T4,T5,T6,T7,short>(v1,v2,v3,v4,v5,v6,v7,0);}
  template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8> void Fill(const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4, const T5 &v5, const T6 &v6, const T7 &v7, const T8 &v8){
    Fill<T1,T2,T3,T4,T5,T6,T7,T8,short>(v1,v2,v3,v4,v5,v6,v7,v8,0);}
  template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9> void Fill(const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4, const T5 &v5, const T6 &v6, const T7 &v7, const T8 &v8, const T9 &v9){
    Fill<T1,T2,T3,T4,T5,T6,T7,T8,T9,short>(v1,v2,v3,v4,v5,v6,v7,v8,v9,0);}
  template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10> 
    void Fill(const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4, const T5 &v5, const T6 &v6, const T7 &v7, const T8 &v8, const T9 &v9, const T10 &v10){
    Fill<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,short>(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,0);}
  template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11> 
    void Fill(const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4, const T5 &v5, const T6 &v6, const T7 &v7, const T8 &v8, const T9 &v9, const T10 &v10, const T11 &v11){
    Fill<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,short>(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,0);}
  template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12> 
    void Fill(const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4, const T5 &v5, const T6 &v6, const T7 &v7, const T8 &v8, const T9 &v9, const T10 &v10, const T11 &v11, const T12 &v12){
    Fill<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,short>(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,0);}
  template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13> 
    void Fill(const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4, const T5 &v5, const T6 &v6, const T7 &v7, const T8 &v8, const T9 &v9, const T10 &v10, const T11 &v11, const T12 &v12, const T13 &v13){
    Fill<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,short>(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,0);}
  template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14> 
    void Fill(const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4, const T5 &v5, const T6 &v6, const T7 &v7, const T8 &v8, const T9 &v9, const T10 &v10, const T11 &v11, const T12 &v12, const T13 &v13, const T14 &v14){
    Fill<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,short>(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,0);}
  template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15> 
    void Fill(const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4, const T5 &v5, const T6 &v6, const T7 &v7, const T8 &v8, const T9 &v9, const T10 &v10, const T11 &v11, const T12 &v12, const T13 &v13, const T14 &v14, const T15 &v15){
    Fill<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,short>(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,0);}
  template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16> 
    void Fill(const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4, const T5 &v5, const T6 &v6, const T7 &v7, const T8 &v8, const T9 &v9, const T10 &v10, const T11 &v11, const T12 &v12, const T13 &v13, const T14 &v14, const T15 &v15, const T16 &v16){
    Fill<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,short>(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,v16,0);}
  template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17> 
    void Fill(const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4, const T5 &v5, const T6 &v6, const T7 &v7, const T8 &v8, const T9 &v9, const T10 &v10, const T11 &v11, const T12 &v12, const T13 &v13, const T14 &v14, const T15 &v15, const T16 &v16, const T17 &v17){
    Fill<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,short>(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,v16,v17,0);}
  template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18> 
    void Fill(const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4, const T5 &v5, const T6 &v6, const T7 &v7, const T8 &v8, const T9 &v9, const T10 &v10, const T11 &v11, const T12 &v12, const T13 &v13, const T14 &v14, const T15 &v15, const T16 &v16, const T17 &v17, const T18 &v18){
    Fill<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,short>(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,v16,v17,v18,0);}
  template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19> 
    void Fill(const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4, const T5 &v5, const T6 &v6, const T7 &v7, const T8 &v8, const T9 &v9, const T10 &v10, const T11 &v11, const T12 &v12, const T13 &v13, const T14 &v14, const T15 &v15, const T16 &v16, const T17 &v17, const T18 &v18, const T19 &v19){
    Fill<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,short>(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,v16,v17,v18,v19,0);}

  template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20> 
    void Fill(const T1 &v1, const T2 &v2, const T3 &v3, const T4 &v4, const T5 &v5, const T6 &v6, const T7 &v7, const T8 &v8, const T9 &v9, const T10 &v10, const T11 &v11, const T12 &v12, const T13 &v13, const T14 &v14, const T15 &v15, const T16 &v16, const T17 &v17, const T18 &v18, const T19 &v19, const T20 &v20){
    //Elements are copies; TColumn takes care of freeing the memory
    if (_table.size()>0) _table.at(0)->Add(new T1(v1));
    if (_table.size()>1) _table.at(1)->Add(new T2(v2));
    if (_table.size()>2) _table.at(2)->Add(new T3(v3));
    if (_table.size()>3) _table.at(3)->Add(new T4(v4));
    if (_table.size()>4) _table.at(4)->Add(new T5(v5));
    if (_table.size()>5) _table.at(5)->Add(new T6(v6));
    if (_table.size()>6) _table.at(6)->Add(new T7(v7));
    if (_table.size()>7) _table.at(7)->Add(new T8(v8));
    if (_table.size()>8) _table.at(8)->Add(new T9(v9));
    if (_table.size()>9) _table.at(9)->Add(new T10(v10));
    if (_table.size()>10) _table.at(10)->Add(new T11(v11));
    if (_table.size()>11) _table.at(11)->Add(new T12(v12));
    if (_table.size()>12) _table.at(12)->Add(new T13(v13));
    if (_table.size()>13) _table.at(13)->Add(new T14(v14));
    if (_table.size()>14) _table.at(14)->Add(new T15(v15));
    if (_table.size()>15) _table.at(15)->Add(new T16(v16));
    if (_table.size()>16) _table.at(16)->Add(new T17(v17));
    if (_table.size()>17) _table.at(17)->Add(new T18(v18));
    if (_table.size()>18) _table.at(18)->Add(new T19(v19));
    if (_table.size()>19) _table.at(19)->Add(new T20(v20));
  };
  unsigned Length() const {
    unsigned length=0;
    for (std::vector<TColumnBase*>::const_iterator it=_table.begin();it!=_table.end();++it)
      if ((*it)->Size()>length) length = (*it)->Size();
    return length;      
  };
  const std::vector<TColumnBase*> * GetTable() const {return &_table;};
  std::string GetHeader() const{return _header;};
  std::string GetDelimiter() const{return _delimiter;};
  std::string GetCaption() const{return _caption;};
  void SetHeader(const std::string s){_header=s;};
  void SetDelimiter(const std::string s){_delimiter=s;};
  void SetCaption(const std::string s){_caption=s;};
  void SetStyle(TableStyle s){_style=s;};
  TableStyle GetStyle() const {return _style;};
  void SetPrecision(unsigned i, unsigned p){_table[i]->SetPrecision(p);};
  void SetPrecision(unsigned p){
    for (std::vector<TColumnBase*>::const_iterator it=_table.begin();it!=_table.end();++it)
     (*it)->SetPrecision(p);};

  friend std::ostream& operator<<( std::ostream& os, const TTable& tab );
  
 protected:
  std::string _header, _delimiter, _caption;
  std::vector<TColumnBase*> _table;
  TableStyle _style;
};


#endif
