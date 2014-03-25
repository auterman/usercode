#ifndef DEMO_H
#define DEMO_H

#include <string>
#include <sstream>

std::string ToString(double d, int precision=1, std::string s=""){
  if (d<=0&&s!="") return s;
  std::stringstream ss;
  ss <<std::fixed << std::setprecision(precision)<< d;
  return ss.str();
}

template<class T>
T StringTo(const std::string& str)
{
  std::stringstream ss(str);
  T res;
  ss >> res;
  return res;
}

template<class T>
std::string ToString(T t)
{
  std::stringstream ss;
  ss << t;
  return ss.str();
}

#endif
