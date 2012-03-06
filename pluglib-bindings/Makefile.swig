# author: Martin Lazar <mlazar@suse.cz> -*- Makefile -*-

WRAPXML_TO_TYPEINFO_XSL ?= $(BASEDIR)/stylesheets/swigxml2typeinfo.xsl
WRAPXML_TO_TYPEINFO_FILTER ?= $(BASEDIR)/stylesheets/filter.pl

SWIGFLAGS	?= -I$(BASEDIR)/swig -c++

CXXFLAGS	?= -g -O0 -fPIC
CPPFLAGS	+= -I.
LDFLAGS		?= -shared
LDLIBS		?= -lstdc++

SO ?= $(addprefix modules/,$(addsuffix .so,$(MODULE))) $(addprefix modules/,$(addsuffix .so,$(MODULES)))
PM ?= $(addprefix modules/,$(addsuffix .pm,$(MODULE))) $(addprefix modules/,$(addsuffix .pm,$(MODULES)))
SRCS ?= $(wildcard *.cc)
OBJS ?= $(SRCS:.cc=.o)

all: $(SO) $(PM)

# do not delete %_wrap.cxx
.SECONDARY:

%_wrap.cxx %_wrap.xml modules/%.pm: %.i
	@mkdir -p modules
	swig -perl $(SWIGFLAGS) -xmlout $(<:.i=_wrap.xml) -outdir modules $<
	sabcmd $(WRAPXML_TO_TYPEINFO_XSL) $(<:.i=_wrap.xml) | perl $(WRAPXML_TO_TYPEINFO_FILTER) --outdir=modules

%.o: %.cc
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

%_wrap.o: %_wrap.cxx
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) `perl -MExtUtils::Embed -e 'ccopts'` -o $@ $<

modules/%.so: $(OBJS) %_wrap.o
	@mkdir -p modules
	$(CC) $(LDFLAGS) $(LDLIBS) -o $@ $^


# generate swig interface
%.I: %.h
	@echo -e "%module $(<:.h=)\n%include \"LiMaL.i\"\n%{\n#include \"$<\"\n%}\n%include \"$<\"\n" > $@


clean:
	rm -rf *_wrap.* *.o modules .dep

check:
	./test.pl
