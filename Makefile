parse: parse.c buff.h buff.c
	gcc -I. --std=c99 -o parse parse.c buff.c

.PHONY: clean
.PHONY: force

clean:
	rm parse

force:
	make clean parse
