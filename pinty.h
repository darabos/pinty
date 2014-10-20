#ifndef _PINTY_H_
#define _PINTY_H_ 1

class Closure {
 public:
  Closure() {}

  template <typename _Func>
  Closure(_Func f) :
    size_(sizeof(f)),
    data_(reinterpret_cast<const char*>(new _Func(std::move(f)))) {}

  static Closure Load(const std::string& dump) {
    return Closure(
        (new std::string(dump))->c_str(),
        dump.size());
  }

  const char* data() const { return data_; }
  int size() const { return size_; }
  std::string Save() const {
    return std::string(data_, size_);
  }

 private:
  Closure(const char* data, size_t size)
    : data_(data),
      size_(size) {}

  const char* data_;
  size_t size_;
};

template <typename _Res, typename _Func, typename... _Args>
class Caller {
 public:
  static _Res Call(const Closure& c, _Args... args) {
    return (*reinterpret_cast<const _Func*>(c.data()))(
        std::forward<_Args>(args)...);
  }
};

template <typename _Signature> class Function {
public:
  typedef typename Function<decltype(&_Signature::operator())>::ret ret;
};

template <typename _Res, typename _Class, typename... _Args>
class Function<_Res (_Class::*)(_Args...) const>
{
public:
  typedef _Res ret;
};

template <typename _Res, typename... _Args>
class Function<_Res(_Args...)> {
 public:
  typedef _Res ret;
  template <typename _Func>
  Function(_Func f) {
    typedef Caller<_Res, _Func, _Args...> MyCaller;
    call_ = &MyCaller::Call;
    closure_ = Closure(f);
  }

  _Res operator()(_Args... args) {
    return call_(closure_, std::forward<_Args>(args)...);
  }

  std::string Save() const {
    size_t call_size = sizeof(_Call);
    const char* call_data = reinterpret_cast<const char*>(&call_);
    std::string call = std::string(call_data, call_size);
    std::string closure = closure_.Save();
    return call + closure;
  }
 private:
  typedef _Res (*_Call)(const Closure&, _Args...);

  Function(_Call call, Closure cl)
    : call_(call),
      closure_(cl) {}

  _Call call_;
  Closure closure_;

 public:
  static Function Load(std::string dump) {
    _Call call = *const_cast<_Call*>(reinterpret_cast<const _Call*>(dump.c_str()));
    Closure cl = Closure::Load(dump.substr(sizeof(_Call)));
    return Function(call, cl);
  }
};
#endif // _PINTY_H_
