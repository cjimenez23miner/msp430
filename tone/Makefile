CC=msp430-gcc
CFLAGS=-Os -Wall -g -mmcu=msp430g2231

OBJS=main.o


all: $(OBJS)
	$(CC) $(CFLAGS) -o main.elf $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -fr main.elf $(OBJS)

download: $(OBJS)
	mspdebug rf2500 "prog main.elf"
