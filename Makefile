UNAME := $(shell uname)

# TO MAKE IT WORK ON APPLE (MAYBE)
ifeq ($(UNAME), Darwin)
	LIB_EXT="dylib"
else
	LIB_EXT="so"
endif

# THE DEFAULT MAKE
all:
ifeq "$(wildcard bin)" ""
	make links
endif

ifeq "$(wildcard lib)" ""
	make links
endif

	# Make the code!
	make -j -C core
	make -j -C analysis
	make -j -C fitqun
	make -j -C src

# THE MAKE FOR THE LINKS
links:
	@ [ -d lib ]	|| mkdir lib
	@ [ -d bin ]	|| mkdir bin

clean:
	make clean -C core
	make clean -C analysis
	make clean -C fitqun
	make clean -C src

	rm -rf lib/lib*
	rm -rf bin/*
