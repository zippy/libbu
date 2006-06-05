#!/bin/sh

g++ -fPIC -shared -Wl,-soname,guy.so -o guy.so -I../src -I../src/test/plugin guy.cpp ../src/test/plugin/plugin.cpp
