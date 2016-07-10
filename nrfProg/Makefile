CC=arm-unknown-linux-gnueabi-gcc
LDFLAGS=-Lprefix/lib -lwiringPi
CFLAGS=-Iprefix/include

FLASH_SIZE=0x4000
MAX_FLASH_WRITE_SIZE=0x3800


CFLAGS+=-DFLASH_SIZE=0x4000
# Protect nrf24LU1+ bootloader (16kB version)
CFLAGS+=-DMAX_FLASH_WRITE_SIZE=0x3800

all: nrfProg

nrfProg: nrfProg.o nrfSpi.o commands.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o nrfProg *~
