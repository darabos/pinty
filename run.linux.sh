#!/bin/sh -ue
g++ pinty.cc -o pinty --std=c++11
setarch $(uname -m) -RL ./pinty $@
