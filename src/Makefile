SRCS = $(wildcard *.cpp)
TRGS = $(patsubst %.cpp, ../bin/%, $(SRCS))

all : $(TRGS)

OPTS := -O3 -Wno-write-strings

#ROOTLIBS := $(shell root-config --glibs --libs --evelibs) -lRooFit -lRooFitCore -lHist -lTree -lCore -lMinuit -lgomp -lRooFitCore -lHistPainter
ROOTLIBS := $(shell root-config --glibs --libs --evelibs)  -lHist -lTree -lCore -lMinuit -lgomp -lHistPainter
ROOTINCLUDES := $(shell root-config --prefix)/include 
ROOTCFLAGS := $(shell root-config --cflags)
GSL_LIBS=$(shell gsl-config --libs)
GSL_INCLUDES=$(shell gsl-config --cflags)


# Where we have our libraries
LIBDIR = ../lib

# The libraries themselves for MaCh3
BEAMLIBS := -lMuenalysis -lAnalysis -lFitqun

INCLUDES := -I$(ROOTINCLUDES) -I../ $(GSL_INCLUDES)

# The general CFLAGS
CPPFLAGS := $(OPTS) $(ROOTCFLAGS)

# The general libs
LIBS := $(BEAMLIBS) $(ROOTLIBS) $(GSL_LIBS)

ifdef CUDAPATH
CUDA_DEFINES=-DCUDA
endif

ifdef MULTITHREAD
OMP_DEFINES=-DMULTITHREAD
CPPFLAGS += -fopenmp
endif

$(TRGS) : $(SRCS)

##############
# AAAAAH WE SHOULDN'T HAVE -FPERMISSIVE AAAAAH
##############

../bin/% : %.cpp
	g++ $(CPPFLAGS) $(INCLUDES) -o $@ $< -L$(LIBDIR) $(LIBS) -fpermissive -Wl,-rpath,$(LIBDIR) $(CUDA_DEFINES) $(OMP_DEFINES)

# Remove the executables
clean:
	rm -f ../bin/*
