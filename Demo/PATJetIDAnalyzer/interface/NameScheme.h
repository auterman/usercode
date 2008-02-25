#ifndef NameScheme_h
#define NameScheme_h

#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include "TString.h"

using namespace std;

class NameScheme {
 public:
  explicit NameScheme();
  explicit NameScheme(char*);
  explicit NameScheme(char*, char*);
  ~NameScheme();
  
  TString name(const char*);
  TString name(const char*, const int);
  TString name(const char*, const int, const int);
  TString name(ofstream&, const char*);
  TString name(ofstream&, const char*, const int);
  TString name(ofstream&, const char*, const int, const int);
  TString setLink(const char*);
  
 private:
  char* name_;
  char* link_;
};
#endif
