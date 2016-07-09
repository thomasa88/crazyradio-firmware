#!/bin/bash

make -C wiringPi/wiringPi DESTDIR=$PWD/prefix PREFIX= LDCONFIG= CC=arm-unknown-linux-gnueabi-gcc CFLAGS='-I. -I../wiringPi' install
