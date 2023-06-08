SUBSRC = $(wildcard src/*.cc)
SUBOBJ = $(SUBSRC:.cc=.o)
SUBHDR = $(wildcard include/*.hh) 

AUTOMAKE_OPTIONS = foreign

INCLUDES = -Iinclude -I$(ROOTSYS)/include -I$(TARTSYS)/include  -I$(TARTSYS)/../mysrc/include $(shell xml2-config --cflags)
CXXFLAGS = -fPIC -Iinclude $(shell root-config --cflags) -I$(TARTSYS)/include  -I$(TARTSYS)/../mysrc/include  -g -Wall $(shell xml2-config --cflags)
LDFLAGS = $(shell root-config --glibs) 

include_HEADERS = $(SUBHDR)

lib_LTLIBRARIES = libsmana.la
libsmana_la_SOURCES = $(SUBSRC) smana_dict.cc
libsmana_la_DEPENDENCIES = $(SUBOBJ)
libsmana_la_LIBADD = $(SUBOBJ)
libsmana_la_LDFLAGS = $(LDFLAGS)

smana_dict.cc: $(SUBHDR) smana_linkdef.hh
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) -p $^

.cc.o:

.f.o:

.c.o:

clean-local:
	rm -f *dict*

clean:
	rm -f $(SUBOBJ)
