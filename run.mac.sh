#!/bin/sh -ue
g++ pinty.cc -o pinty --std=c++11 -Wl,-no_pie
./pinty $@
