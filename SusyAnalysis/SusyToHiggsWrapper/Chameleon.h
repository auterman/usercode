#ifndef CHAMELEON_H
#define CHAMELEON_H


//---chameleon class

#include <sstream>
#include <string>
#include <map>
#include <fstream>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

class Chameleon {

  public:
Chameleon() {}
Chameleon(std::string const& value) {
  value_=value;
}


Chameleon(const char* c) {
  value_=c;
}

Chameleon(double d) {
  std::stringstream s;
  s<<d;
  value_=s.str();
}

Chameleon(Chameleon const& other) {
  value_=other.value_;
}

Chameleon& operator=(Chameleon const& other) {
  value_=other.value_;
  return *this;
}

Chameleon& operator=(double i) {
  std::stringstream s;
  s << i;
  value_ = s.str();
  return *this;
}

Chameleon& operator=(std::string const& s) {
  value_=s;
  return *this;
}

operator std::string() const {
  return value_;
}

operator double() const {
  return atof(value_.c_str());
}



private:
  std::string value_;
};



//---trim function
std::string trim(std::string const& source, char const* delims = " \t\r\n");

//---trim function

//---class
class ConfigFile {
  std::map<std::string,Chameleon> content_;

public:
  ConfigFile(std::string const& configFile);

  Chameleon const& Value(std::string const& section, std::string const& entry) const;
  Chameleon const& Value(std::string const& section, std::string const& entry, double value);
  Chameleon const& Value(std::string const& section, std::string const& entry, std::string const& value);
};



#endif
