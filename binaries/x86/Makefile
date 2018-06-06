OBJ = main main.noaranges main.stripped main.noline
NPD=--no-print-directory
RLM=../../readlm

all: run

main: main.c
	gcc -g -O2 -o main main.c

main.noaranges: main
	objcopy --remove-section .debug_aranges main main.noaranges

main.stripped: main
	cp main main.stripped
	strip main.stripped

main.noline:
	objcopy --remove-section .debug_line main main.noline

fail:
	@make $(NPD) -f Makefile test EXEC=$(EXEC) RESULT=FAILURE

success:
	@make $(NPD) -f Makefile test EXEC=$(EXEC) RESULT="line information"

test:
	@echo
	@echo '***' testing $(EXEC), result expected=$(RESULT) '***'
	@$(RLM) $(EXEC)

run: $(OBJ)
	@echo --------------------------------------------------
	@echo expect these tests to produce line map information
	@echo --------------------------------------------------
	@echo
	@make $(NPD) -f Makefile success EXEC=main
	@make $(NPD) -f Makefile success EXEC=main.noaranges
	@make $(NPD) -f Makefile success EXEC=$(RLM)
	@echo
	@echo --------------------------------------------------
	@echo expect these tests to fail to find line maps
	@echo --------------------------------------------------
	@echo
	@make $(NPD) -f Makefile fail EXEC=main.noline
	@make $(NPD) -f Makefile fail EXEC=main.stripped

clean:
	/bin/rm -rf $(OBJ)
