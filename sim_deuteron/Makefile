# Specify the source files, the name files, 
# and the install directory
name		:= sim_deuteron
SOURCES	 	 = $(wildcard src/*.cc)
INSTALLDIR	 = lib

# Specify the compiling options
CC			 = $(shell root-config --cc)
CXX			 = $(shell root-config --cxx)
CPPFLAGS	+= -Iinclude -I$(G4SMLIBDIR)/include -I$(TARTSYS)/include -I$(G4INCLUDE) \
			   -I$(ROOTSYS)/include
CXXFLAGS	+= -O3 -fPIC
LDFLAGS		+= $(shell root-config --ldflags) $(filter-out std=%,$(shell geant4-config --cflags))
ROOTLIBS	 = $(shell root-config --glibs) -ltbb
G4LIBS		 = $(shell geant4-config --libs)
TARTLIBS	 = -L$(TARTSYS)/lib -lanasamurai -lanabrips -lanacore -lXMLParser
LDLIBS		+= $(G4SMLDLIBS) $(TARTLIBS) $(ROOTLIBS) $(G4LIBS)

.PHONY: all
all: bin lib

bin: $(name)
	cp $< ../bin

lib: lib$(name).so
	cp $< ../lib

%: %.cc lib%.so
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $< \
	$(LOADLIBES) $(LDLIBS) -L. -l$@ -o $@

lib%.so: $(SOURCES)
	$(CXX) -shared $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $^ \
	$(LOADLIBES) $(LDLIBS) -o $@

.PHONY: clean 
clean:
	rm -vf $(name)
	rm -vf lib$(name).so
	rm -vf src/*.d
	rm -vf ../bin/$(name)
	rm -vf ../lib/lib$(name).so

# Generate dependencies of .cc files on .hh files
include $(subst .cc,.d,$(SOURCES))

%.d: %.cc
	$(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
