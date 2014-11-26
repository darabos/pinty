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

void usage(const char* program) {
  std::cout << "To save f(N):\n  " << program << " save <N>\n";
  std::cout << "To load and call a function:\n  " << program << " call <fn> <M>\n";
  std::exit(64);
}

int main(int argc, char** argv) {
  if (argc <= 1) usage(argv[0]);
  std::string mode(argv[1]);

  if (mode == "save") {
    if (argc <= 2) usage(argv[0]);
    int N = atoi(argv[2]);
    auto g = f(N);
    std::cout << "f(" << N << "): " << Dumpstr(g.Save()) << "\n";
  } else if (mode == "call") {
    if (argc <= 3) usage(argv[0]);
    auto fn = argv[2];
    int M = atoi(argv[3]);
    auto h = Function<int(int)>::Load(Undumpstr(fn));
    std::cout << fn << "(" << M << "): " << h(M) << "\n";
  } else usage(argv[0]);
  return 0;
}
