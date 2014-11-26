Pinty
=====

Pinty is a little prototype where I investigated the feasibility of an
[Apache Spark](http://spark.apache.org/)-like system in C++.

`pinty.cc` serves as a little demo. It defines `f()` like this:
~~~
Function<int(int)> f(int i) {
  auto x = [=](int y){ return y + i; };
  return x;
}
~~~

So `f(N)` is an `int` to `int` function that adds N to a number.

~~~
$ ./run.mac.sh
To save f(N):
  ./pinty save <N>
To load and call a function:
  ./pinty call <fn> <M>
$ ./run.mac.sh save 100
f(100): b01200000100000064000000
$ ./run.mac.sh call b01200000100000064000000 5
b01200000100000064000000(5): 105
~~~

The closure is serialized as `b01200000100000064000000`. You can see `64` (decimal 100) in
the serialized form. When deserialized it still adds 100 to a number. Mission accomplished.
