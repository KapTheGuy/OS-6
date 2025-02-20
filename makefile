CC = ./65-elf-gcc
ARCH = SB6

all:
	@echo COMPILING OS.C FOR $(ARCH) 
	@$(CC)

clean:
	@echo REMOVING ALL OUTPUT FILES
	@rm OUT.ASM
