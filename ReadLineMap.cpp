#include <assert.h> 
#include <errno.h>
#include <fcntl.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#include <iostream> 

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <libdw.h> 

#include <RelocateCubin.hpp> 
#include <ReadCubinLineMap.hpp> 

#ifndef EM_CUDA
#define EM_CUDA 190
#endif

typedef struct {
  Dwarf_Addr addr;
  const char *file;
  int lineno;
  int col;
  bool endseq;
} stmt_t;

int
main(int argc, char **argv)
{
  if (argc != 2) {
    printf("usage: %s <load-module>\n", argv[0]);
    exit(-1);
  }

  int fd = open(argv[1], O_RDONLY); 

  if (fd == -1) {
    printf("FAILURE: unable to open file %s: %s\n", argv[1],
	   strerror(errno));
    exit(-1);
  }

  struct stat sb;
  fstat(fd, &sb);
  void *p = (char *) mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE,
		 fd, (off_t) 0);
  if (p == MAP_FAILED) {
    printf("FAILURE: unable to memory map file %s: %s\n", argv[1],
	   strerror(errno));
    exit(-1);
  }

  // create a memory-resident copy of the file that we can read and write 
  char *memPtr = (char *) malloc(sb.st_size); 
  if (memPtr == 0) {
    printf("FAILURE: unable to copy file %s into memory\n", argv[1]);
    exit(-1);
  }

  memcpy(memPtr, p, sb.st_size); 

  munmap(p, sb.st_size); 

  Elf *elf = elf_memory(memPtr, sb.st_size);

  GElf_Ehdr ehdr_v; 
  GElf_Ehdr *ehdr = gelf_getehdr(elf, &ehdr_v);
  if (!ehdr) {
    printf("FAILURE: file %s is not an ELF file\n", argv[1]);
  }

  readCubinLineMap(memPtr, elf);
  return 0;

  if (ehdr->e_machine == EM_CUDA) {
    relocateCubin(memPtr, elf);
  }


  Dwarf *dbg = dwarf_begin_elf(elf, DWARF_C_READ, 0);
  if (dbg) {
    // iterate over the compilation-unit headers
    size_t cu_header_size;
    for(Dwarf_Off cu_off = 0, next_cu_off;
        dwarf_nextcu(dbg, cu_off, &next_cu_off, &cu_header_size,
		     NULL, NULL, NULL) == 0;
        cu_off = next_cu_off)
    {
      // extract a compilation unit by offset
      Dwarf_Off cu_die_off = cu_off + cu_header_size;
      Dwarf_Die cu_die, *cu_die_p; 
      cu_die_p = dwarf_offdie(dbg, cu_die_off, &cu_die);

      Dwarf_Lines *lines;
      size_t nlines;
      int found_lines = dwarf_getsrclines(&cu_die, &lines, &nlines);

      if (nlines == 0) {
        printf("FAILURE: file %s contains no discernable line map\n", argv[1]);
      }

      for (size_t i = 0; i < nlines; i++) {
	stmt_t stmt;

	Dwarf_Line *line = dwarf_onesrcline(lines, i);
	int haslineno    = dwarf_lineno(line, &stmt.lineno);
	int hascolumn    = dwarf_linecol(line, &stmt.col);
	int hasaddr      = dwarf_lineaddr(line, &stmt.addr);
	stmt.file        = dwarf_linesrc(line, NULL, NULL);
	int hasendseq    = dwarf_lineendsequence(line, &stmt.endseq);

	printf("%8p ", (void *) stmt.addr);
	if (stmt.endseq) printf("END SEQUENCE\n");
	else {
          if (haslineno == 0) printf("%s:%d", stmt.file, stmt.lineno);
          if (hascolumn == 0) printf(" col=%d", stmt.col);
        }
	printf("\n");
      }
    }

    dwarf_end(dbg);

  } else {
#if 0
    std::cout << "FAILURE: unable to open dwarf information for file " <<
      argv[1] << std::endl;
#endif
  }

  elf_end(elf);

  return 0;
} 
