OBJ = vectorAdd.sm_52.cubin
NPD=--no-print-directory
RLM=../../readlm

all: run

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
	@make $(NPD) -f Makefile success EXEC=vectorAdd.sm_52.cubin
