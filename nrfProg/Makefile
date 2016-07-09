
LDFLAGS=$(shell pkg-config --libs libftdi1)
CFLAGS=$(shell pkg-config --cflags libftdi1)

all: nrfProg

nrfProg: nrfProg.o nrfSpi.o commands.o
	cc -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o nrfProg *~
