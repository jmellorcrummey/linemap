This directory contains code to read the line map of binaries. It works for host binaries and
for cubins compiled -G as well as for cubins compiled with optimization with the 
--generate-line-info option. The latter binaries have line maps encoded in a non-standard form: 
they contain a .debug_line section but not a .debug_info section.

The code performs the following steps:
1. open a binary and copy it into memory
2. open the in-memory copy as an elf object
2. if the elf object is a cubin, 
3. read the dwarf linemap using the reader in ReadCubinLineMap.cpp. At present, the reader
   scans the line map section, incrementally updating a line representation 'lineInfo'. 
   Today, it simply prints out the line map in a form that matches the output of the readelf
   implementation in the copy of Linux I have running in Virtual Box. readelf --versin says
   'GNU readelf (GNU Binutils for Ubuntu) 2.26.1'

One can test the harness, comparing its output with that of readelf, by executing the following bash 
commands:

cd cubins
for f in *.cubin ; do  ../readlm $f > $f.lm; readelf --debug-dump $f > $f.re; diff $f.lm $f.re; done 

cd ../cubins-opt
for f in *.cubin ; do  ../readlm $f > $f.lm; readelf --debug-dump $f > $f.re; diff $f.lm $f.re; done 

Here is my Ubuntu configuration where the output matches: 
Linux JMC-VirtualBox 4.13.0-26-generic #29~16.04.2-Ubuntu SMP Tue Jan 9 22:00:44 UTC 2018 x86_64 x86_64 x86_64 GNU/Linux

Note: the ReadLineMap.cpp source code was originally written to illustrate reading line maps with libdw. It has been
hacked to just call the new ELF line map reader in ReadCubinLineMap.cpp without any effort to rip out the code that is
no longer used.

