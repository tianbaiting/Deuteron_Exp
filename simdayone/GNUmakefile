# $Id: GNUmakefile,v 1.1 1999/01/07 16:05:40 gunter Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := simdayone
G4TARGET := $(name)
G4EXLIB := true
G4WORKDIR := ..

.PHONY: all
all: bin lib rootlib
rootlib:
	g++ -Wl,-soname=lib$(name).so -shared -o lib$(name).so $(G4WORKDIR)/tmp/Linux-g++/$(name)/*.o
	@cp lib$(name).so ../lib

CPPFLAGS += -fPIC $(shell $(ROOTSYS)/bin/root-config --cflags)  -g -UG4UI_USE_QT $(G4SMCPPFLAGS)
ROOTLIBS = $(shell $(ROOTSYS)/bin/root-config --glibs) -ltbb
TARTLIBS = -L$(TARTSYS)/lib -lanacatana -lanadali -lanasamurai -lanabrips -lanacore -lXMLParser
EXTRALIBS += $(G4SMLDLIBS) $(TARTLIBS) $(ROOTLIBS)

DICT = src/simdayone_classDict.cc
DICTPCM = $(patsubst %.cc,%_rdict.pcm,$(DICT))
LINKDEF = include/LinkDef.hpp

sources = $(patsubst $(DICT),,$(wildcard src/*.cc)) $(DICT)

include $(G4INSTALL)/config/binmake.gmk

root_class_headers = TDeutSimParameter.hh

DICT: $(DICT)
$(DICT): $(LINKDEF) $(patsubst %,include/%,$(root_class_headers))
	rootcling -I $(shell pwd)/include -f $@ $(root_class_headers) $<
	-install -Dvm644 $(DICTPCM) ../lib/

cleanall: clean
	rm -vf include/*~
	rm -vf src/*~
	rm -vf ./*~
	rm -vf ./*.so
	rm -vf ../lib/lib$(name).so
	rm -vf $(DICT) $(DICTPCM) $(patsubst src/%,../lib/%,$(DICTPCM))
