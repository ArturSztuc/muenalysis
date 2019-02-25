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

OBJECTS := comparison.o cuts.o skutil.o

%.o:%.cpp
	g++ -c $(CXXFLAGS) $< $(OMP_DEFINES)

libAnalysis: libAnalysis.a $(LIBDIR)/libAnalysis.so

libAnalysis.a: $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

$(LIBDIR)/libAnalysis.so: libAnalysis.a $(OBJECTS)
	$(LD_SHARED) $(SOFLAGS) $^ -o $@ $(LIB_OBJECTS) $(ROOTLIBS) -L../lib 

clean:
	rm -f ../lib/libAnalysis.so	
	rm -f *.o lib*

