CC=msp430-gcc
CFLAGS=-Wall -Os -nostdlib -nostartfiles
LDFLAGS=-Wl,-Map,main.map

main.elf: start.o main.o link.ld
	msp430-gcc -Wall -Os -nostdlib -nostartfiles -o $@ -Wl,-Map,main.map -T link.ld start.o main.o

.PHONY: clean dbg download

dbg:
	mspdebug rf2500 

download:
	mspdebug rf2500 "prog main.elf"

clean:
	rm *.o *.map *.elf

dis:
	msp430-objdump -D main.elf
