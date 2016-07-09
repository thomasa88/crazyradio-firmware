# nrfProg - nRF24LU1p SPI programmer.

Type 'nrfProg help' for help.

# Programming E05-MLE124A using Raspberry Pi

Pin-mapping

Raspberry Pi
WiringPi  Name   Header (P1)     E05-MLE124A   nRF24LE1
12        MOSI   19              4             FMOSI P0.3
13        MISO   21              6             FMISO P0.4
14        SCLK   23              2             FSCK  P0.2
10        CE0    24              8             FCSN  P0.5
5         GPIO5  18              11            PROG
6         GPIO6  22              3             RESETn
-         VCC    17              1             VCC
-         GND    20              12            GND

          manualCS GPI01  12     8 xxx

Cannot use built-in chip select (CE0) until all spi commands are done in one read/write operation. Otherwise CE0 will be released in-between.


raspi-config:
Advanced Options --> A5 SPI
reboot
