#!/bin/bash

grep 'to\(Lower\|Upper\)' -o $(find -iname *.cpp) $(find -iname *.h) 2>&1 | grep -v '.svn' | sort | uniq
