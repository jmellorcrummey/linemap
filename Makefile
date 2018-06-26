DW=/home/johnmc/spack/opt/spack/linux-ubuntu18.04-x86_64/gcc-7.3.0/elfutils-0.170-feejdzwdx5tcvn6sgwnivcqo6dxecjai
DW_INC = -I. -I $(DW)/include/elfutils -I $(DW)/include
DW_L = $(DW)/lib
DW_LIB = -L $(DW_L) -Wl,--rpath=$(DW_L) -ldw -lelf
SRC=ReadLineMap.cpp RelocateCubin.cpp ElfHelper.cpp ReadCubinLineMap.cpp

all: readlm readlmd

readlm: $(SRC) Line.hpp ReadCubinLineMap.hpp
	g++ -std=c++11 $(DW_INC) -DDUMP_RAW_LINE -o $@ -g $(SRC) $(DW_LIB)

readlmd: $(SRC) Line.hpp ReadCubinLineMap.hpp
	g++ -std=c++11 $(DW_INC) -DDUMP_DECODED_LINE -o $@ -g $(SRC) $(DW_LIB)

run: readlm
	make -C binaries/x86 -f Makefile run
	make -C binaries/cubins -f Makefile run
clean:
	/bin/rm -rf readlm readlmd
