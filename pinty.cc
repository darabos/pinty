#include <fstream>
#include <iomanip>
#include <iostream>
#include <cstdio>
#include <cstring>

#include "pinty.h"

template<typename X> std::string Dump(const X& x) {
  const unsigned char *s = reinterpret_cast<const unsigned char*>(&x);
  std::string d;
  for (int i = 0; i < sizeof(x); ++i) {
    char buf[3];
    sprintf(buf, "%02x", s[i]);
    d += buf;
  }
  return d;
}

std::string Dumpstr(const std::string& s) {
  std::string d;
  for (int i = 0; i < s.size(); ++i) {
    char buf[3];
    sprintf(buf, "%02x", static_cast<unsigned char>(s[i]));
    d += buf;
  }
  return d;
}

std::string Undumpstr(const std::string& s) {
  size_t l = s.size() / 2;
  char bytes[l];
  for (int i = 0; i < l; ++i) {
    sscanf(&s.c_str()[i * 2], "%2hhx", &bytes[i]);
  }
  return std::string(bytes, l);
}

Function<int(int)> f(int i) {
  auto x = [=](int y){ return y + i; };
  return x;
}

int main(int argc, char** argv) {
  int N = atoi(argv[2]);
  std::cout << "N: " << N << "\n";
  auto g = f(8);
  std::cout << "result: " << g(N) << "\n";
  std::cout << "saved: " << Dumpstr(g.Save()) << "\n";

  auto h = Function<int(int)>::Load(Undumpstr(argv[1]));
  std::cout << "loaded: " << Dumpstr(h.Save()) << "\n";
  std::cout << "result: " << h(N) << "\n";
  return 0;
}
