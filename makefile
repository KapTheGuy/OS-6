all:
	@echo COMPILING OS.C FOR E6502
	@./os6cc

clean:
	@echo REMOVING ALL OUTPUT FILES
	@rm OUT.ASM
