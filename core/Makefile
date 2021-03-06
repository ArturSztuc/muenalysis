CXX=g++
AR=ar
LD_SHARED=g++

ROOTLIBS := $(shell root-config --glibs --libs)  -lMinuit
ROOTINCLUDES := $(shell root-config --prefix)/include 
ROOTCFLAGS := $(shell root-config --cflags) -I../

INCDIR = -I. 
LIBDIR = ../lib

UNAME := $(shell uname)

ifneq ($(UNAME), Darwin)
EXPORT_DYNAMIC="-Wl,-export-dynamic"
endif

CXXFLAGS   = -Wall -O3 -g -fPIC $(ROOTCFLAGS) $(INCDIR) -Werror
SYSLIB     = -lm -lg2c
LINK_ARGS_BIN = $(SYSLIB) $(LIBDIR)
SOFLAGS= -shared $(EXPORT_DYNAMIC) $(ROOTCFLAGS)
LDFLAGS=
ARFLAGS= rcsv

ifdef MULTITHREAD
CXXFLAGS += -fopenmp
OMP_DEFINES = -DMULTITHREAD
endif

OBJECTS := muenalysis.o 

%.o:%.cpp
	g++ -c $(CXXFLAGS) $< $(OMP_DEFINES)

libMuenalysis: libMuenalysis.a $(LIBDIR)/libMuenalysis.so

libMuenalysis.a: $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

$(LIBDIR)/libMuenalysis.so: libMuenalysis.a $(OBJECTS)
	$(LD_SHARED) $(SOFLAGS) $^ -o $@ $(LIB_OBJECTS) $(ROOTLIBS) -L../lib 

clean:
	rm -f ../lib/libMuenalysis.so	
	rm -f *.o lib*

