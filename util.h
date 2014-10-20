#ifndef _PINTY_UTILS_H_
#define _PINTY_UTILS_H_

#include <cstdlib>
#include <iostream>
#include <string>

void check(bool b, const std::string& message) {
  if (!b) {
    std::cerr << message << "\n";
    std::exit(1);
  }
}

std::string StrCat(int i) { return std::to_string(i); }

std::string StrCat(const std::string& s) { return s; }

std::string StrCat(const char* s) { return s; }

std::string StrCat() { return ""; }

template <typename T, typename... Args>
std::string StrCat(T t, Args... args) {
  return StrCat(t) + StrCat(args...);
}

#endif
