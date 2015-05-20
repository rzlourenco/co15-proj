MAKEOPTS += -k

PWN    := ../pwn
YASM   := yasm
TMPDIR := .obj
OUTDIR := bin

SRCS   := $(wildcard *.pwn)
EXES   := $(SRCS:%.pwn=$(OUTDIR)/%)

all: | $(TMPDIR)/.dummy $(OUTDIR)/.dummy $(EXES)

$(TMPDIR)/%.asm: %.pwn
	$(PWN) -g $< -o $(ASMDIR)$(patsubst pwn,asm,$@)

$(TMPDIR)/%.o: $(TMPDIR)/%.asm
	$(YASM) -felf32 -gdwarf2 $< -o $@

$(OUTDIR)/%: $(TMPDIR)/%.o
	$(LD) -melf_i386 -L$(HOME)/compiladores/root/usr/lib $< -lrts -o $@
	
$(TMPDIR)/.dummy:
	@mkdir -p $(TMPDIR)
	@touch $@

$(OUTDIR)/.dummy:
	@mkdir -p $(OUTDIR)
	@touch $@

.PHONY: clean
clean:

