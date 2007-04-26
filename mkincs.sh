#!/bin/bash

cd src/bu
rm *
for i in ../*.h; do ln -s $i; done
