DW=/home/johnmc/pkgs-src/elfutils-0.170/install
DW_INC = -I. -I $(DW)/include/elfutils -I $(DW)/include
DW_L = $(DW)/lib
DW_LIB = -L $(DW_L) -Wl,--rpath=$(DW_L) -ldw -lelf
SRC=ReadLineMap.cpp RelocateCubin.cpp ElfHelper.cpp ReadCubinLineMap.cpp

all: readlm

readlm: $(SRC) Line.hpp ReadCubinLineMap.hpp
	g++ -std=c++11 $(DW_INC) -o $@ -g $(SRC) $(DW_LIB)

run: readlm
	make -C binaries/x86 -f Makefile run
	make -C binaries/cubins -f Makefile run
clean:
	/bin/rm -rf readlm
