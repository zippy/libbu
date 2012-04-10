#
# Copyright (C) 2007-2011 Xagasoft, All rights reserved.
#
# This file is part of the libbu++ library and is released under the
# terms of the license contained in the file LICENSE.
#

OBJECTS := $(patsubst %.cpp,%.o,$(wildcard src/stable/*.cpp src/unstable/*.cpp src/experimental/*.cpp))
HEADERS := bu/signals.h bu/autoconfig.h bu/version.h bu/config.h $(foreach fn,$(wildcard src/stable/*.h src/unstable/*.h src/experimental/*.h),bu/$(notdir ${fn})) $(patsubst src/%,bu/%,$(wildcard src/compat/*.h))
TOOLS := $(patsubst src/tools/%.cpp,%,$(wildcard src/tools/*.cpp))
UNITS := $(patsubst src/unit/%.unit,unit/%,$(wildcard src/unit/*.unit))
TESTS := $(patsubst src/tests/%.cpp,tests/%,$(wildcard src/tests/*.cpp))

.PHONY: default all headers clean tests

default: libbu++.a tools

all: default tests

tests: ${UNITS} ${TESTS}

clean:
	-rm ${HEADERS} ${OBJECTS} libbu++.a ${TOOLS} ${UNITS} ${TESTS}

bu/signals.h bu/config.h bu/autoconfig.h bu/version.h: bu/%: src/%
	ln -s ../$< $@

$(foreach fn,$(wildcard src/stable/*.h),bu/$(notdir ${fn})): bu/%: src/stable/%
	ln -s ../$< $@

$(foreach fn,$(wildcard src/unstable/*.h),bu/$(notdir ${fn})): bu/%: src/unstable/%
	ln -s ../$< $@

$(foreach fn,$(wildcard src/experimental/*.h),bu/$(notdir ${fn})): bu/%: src/experimental/%
	ln -s ../$< $@

$(patsubst src/%,bu/%,$(wildcard src/compat/*.h)): bu/%: src/%
	ln -s ../../$< $@

autoconfig: autoconfig.cpp
	${CXX} -o autoconfig autoconfig.cpp

src/autoconfig.h src/version.h: autoconfig
	./autoconfig $@

src/signals.h: pregen/signals.h
	cp $< $@

headers: ${HEADERS}

tools: ${TOOLS}

${TOOLS}: %: src/tools/%.cpp libbu++.a

$(filter-out viewcsv bin2cpp,${TOOLS}):
	g++ -ggdb -W -Wall -I. -L. $< -o $@ -lbu++

viewcsv:
	g++ -ggdb -W -Wall -I. -L. $< -o $@ -lbu++ -lncurses

bin2cpp:
	g++ -ggdb -W -Wall -I. -L. $< -o $@ -lbu++ -lbz2 -lz -llzma

${OBJECTS}: %.o: %.cpp
	g++ -ggdb -W -Wall -I. $< -c -o $@

$(patsubst %,src/%.cpp,${UNITS}): %.cpp: %.unit mkunit
	./mkunit $< $@

${UNITS}: %: src/%.cpp
	g++ -ggdb -W -Wall -I. -L. $< -o $@ -lbu++

${TESTS}: %: src/%.cpp

$(filter-out tests/bzip2 tests/deflate tests/lzma,${TESTS}):
	g++ -ggdb -W -Wall -I. -L. $< -o $@ -lbu++

tests/bzip2:
	g++ -ggdb -W -Wall -I. -L. $< -o $@ -lbu++ -lbz2

tests/deflate:
	g++ -ggdb -W -Wall -I. -L. $< -o $@ -lbu++ -lz

tests/lzma:
	g++ -ggdb -W -Wall -I. -L. $< -o $@ -lbu++ -llzma

libbu++.a: ${HEADERS} ${OBJECTS}
	ar -r libbu++.a ${OBJECTS}

