
name := crosssection
G4TARGET := $(name)
G4EXLIB := true
#G4WORKDIR := $(shell pwd)
G4WORKDIR := ..

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/architecture.gmk
include $(G4INSTALL)/config/binmake.gmk

CPPFLAGS += $(shell $(ROOTSYS)/bin/root-config --cflags) -g $(G4SMCPPFLAGS)

ROOTLIBS = $(shell $(ROOTSYS)/bin/root-config --glibs)
TARTLIBS = -L$(TARTSYS)/lib -lanaroot -lXMLParser
LDLIBS += $(ROOTLIBS) $(G4SMLDLIBS) $(TARTLIBS)

cleanall: clean
	rm -vf txt/*
	rm -vrf	bin
	rm -vrf	tmp
