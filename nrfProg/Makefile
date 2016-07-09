CC=arm-unknown-linux-gnueabi-gcc
LDFLAGS=-Lprefix/lib -lwiringPi
CFLAGS=-Iprefix/include

all: nrfProg

nrfProg: nrfProg.o nrfSpi.o commands.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o nrfProg *~
