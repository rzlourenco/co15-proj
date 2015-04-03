CC=../pwn
XMLDIR=xml/
DEPS=$(wildcard *.pwn)

all: $(patsubst %.pwn,%.xml,$(DEPS))

%.xml : %.pwn
	$(CC) $< -o $(XMLDIR)$(patsubst pwn,xml,$@)


print-%:
	@echo '$*=$($*)'

clean:
	rm -f $(XMLDIR)/*

