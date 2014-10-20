#include <fstream>
#include <vector>

#include "util.h"
#include "pinty.h"

using std::string;
using std::vector;

template <typename T> class RDD {
public:
  static RDD<T> From(vector<T> v, int partitions);
  template <typename F> RDD<typename Function<F>::ret> Map(F f);
  RDD<T> Filter(Function<bool(T)> f);
  vector<T> Collect();

private:
  // Fake RDD, just to think about the API.
  RDD<T>(vector<T> v);
  vector<T> data_;
};

template <typename T>
RDD<T> RDD<T>::From(vector<T> v, int partitions) {
  return RDD<T>(v);
}

template <typename T> template <typename F>
RDD<typename Function<F>::ret>
RDD<T>::Map(F f) {
  vector<typename Function<F>::ret> r;
  for (auto x : data_) {
    r.push_back(f(x));
  }
  return RDD<typename Function<F>::ret>::From(r, 1);
}

template <typename T> RDD<T> RDD<T>::Filter(Function<bool(T)> f) {
  vector<T> r;
  for (auto x : data_) {
    if (f(x)) {
      r.push_back(x);
    }
  }
  return RDD<T>::From(r, 1);
}

template <typename T> vector<T> RDD<T>::Collect() { return data_; }

template <typename T> RDD<T>::RDD(vector<T> v) : data_(v) {}

vector<int> Range(int n) {
  vector<int> r;
  for (int i = 0; i < n; ++i) {
    r.push_back(i);
  }
  return r;
}

template <typename F, typename T>
std::vector<typename Function<F>::ret>
Map(std::vector<T> v, F f) {
  std::vector<typename Function<F>::ret> r;
  for (auto x : v) {
    r.push_back(f(x));
  }
  return r;
}

int main(int argc, char **argv) {
  RDD<int> numbers = RDD<int>::From(Range(100), 2);
  auto x = Map(Range(100), [](int x) {
    return StrCat(x);
  });
  RDD<string> decimals = numbers.Map([](int x) {
    return StrCat(x);
  });
  RDD<string> have5 = decimals.Filter([](string s) {
    return s.find('5') != string::npos;
  });
  RDD<int> parsed = have5.Map([](string s) {
    return std::stoi(s);
  });
  vector<int> results = parsed.Collect();
  for (int i : results) {
    std::cout << i << "\n";
  }
  return 0;
}
