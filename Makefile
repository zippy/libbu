OBJS:=$(patsubst %.cpp,%.o,$(wildcard src/*.cpp))
TOBJS:=$(patsubst %.cpp,%.o,$(wildcard src/test/*.cpp))
TDIRS:=$(filter-out %.o %.cpp %.h %.d,$(wildcard src/test/*))
TESTS:=$(patsubst src/test/%.o,tests/%,$(TOBJS)) $(patsubst src/test/%,tests/%,$(TDIRS))
ATOBJS:=$(TOBJS) $(patsubst %.cpp,%.o,$(foreach dr,$(TDIRS),$(wildcard $(dr)/*.cpp)))
LIB:=libbu++.a
DATE:=$(shell date +%F)

TXTCPP:="\ g++:\ \ \ "
TXTLNK:="\ \ ld:\ \ \ "
TXTDEP:="\ dep:\ \ \ "
TXTARC:="\ \ ar:\ \ \ "

.PHONY: all clean dist tests depclean cleanapi
.SILENT: clean $(OBJS) $(TOBJS) $(ATOBJS) $(TESTS) $(patsubst %.o,%.d,$(OBJS) $(TOBJS) $(ATOBJS) $(TESTS)) $(LIB)

all: libbu++.a tests

depclean:
	-rm $(patsubst %.o,%.d,$(OBJS) $(ATOBJS))

-include $(patsubst %.o,%.d,$(OBJS) $(ATOBJS))

clean:
	-rm $(OBJS) $(ATOBJS) $(TESTS) $(LIB)

# This bit I cribbed from the docs, seems to work great though!
%.d: %.cpp
	echo "$(TXTDEP)$@"
	g++ $(CXXFLAGS) -Isrc -M $(CPPFLAGS) $< | sed 's,\($(notdir $*)\)\.o[: ]*,$(dir $*)\1.o $@: ,g' > $@

%.o: %.cpp
	echo "$(TXTCPP)$@"
	g++ $(CXXFLAGS) -Isrc $(foreach dr,$(filter $(dir $@),$(foreach ddr,$(TDIRS),$(ddr)/)),-I$(dr)) -ggdb -c -o $@ $<

$(LIB): $(OBJS)
	echo "$(TXTARC)$@"
	ar cr $(LIB) $(OBJS)

$(TESTS): $(ATOBJS) $(LIB)
	echo "$(TXTLNK)$@"
	g++ $(LDFLAGS) -ggdb $(filter %$(patsubst tests/%,%,$@).o, $(TOBJS) ) $(patsubst %.cpp,%.o,$(wildcard $(filter %$(patsubst tests/%,%,$@), $(TDIRS))/*.cpp)) -L. -lbu++ -o $@
	
tests: $(TESTS)

dist: clean depclean
	mkdir libbu++-$(DATE)
	cp -a --target-directory=libbu++-$(DATE) Makefile src
	tar --exclude=\.svn -c libbu++-$(DATE) | bzip2 -9 > libbu++-$(DATE).tar.bz2
	rm -Rf libbu++-$(DATE)

install: libbu++.a
	cat src/*.h | grep -v "#include \"" - > libbu++.h
	cp -a libbu++.a $(PREFIX)/lib
	cp -a libbu++.h $(PREFIX)/include

cleanapi:
	-rm -Rf api

api: $(SRC)
	-rm -Rf api
	doxygen
	make -C api/latex

#tests: $(TOBJS) $(LIB)
#	for file in $(patsubst src/test/%.o,%,$(TOBJS)); \
#	do g++ -o $$file src/test/$$file.o -L. -lbu++; \
#	done

