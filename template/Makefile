CC=msp430-gcc
CFLAGS=-Os -Wall -g -mmcu=msp430g2231
INC_DIR=-I ../lib/ -I ./
OBJS=main.o uart.o print.o

VPATH=../lib 

all: main.elf

main.elf: $(OBJS)
	$(CC) $(CFLAGS) -o main.elf $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(INC_DIR) -c $<

clean:
	rm -fr main.elf $(OBJS)

download: main.elf
	mspdebug rf2500 "prog main.elf"

gdb: all
	mspdebug rf2500 "gdb"

uart:
	screen /dev/tty.SLAB_USBtoUART 9600
