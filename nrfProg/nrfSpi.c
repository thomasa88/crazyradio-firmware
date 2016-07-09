/**
 *    ||          ____  _ __                           
 * +------+      / __ )(_) /_______________ _____  ___ 
 * | 0xBC |     / __  / / __/ ___/ ___/ __ `/_  / / _ \
 * +------+    / /_/ / / /_/ /__/ /  / /_/ / / /_/  __/
 *  ||  ||    /_____/_/\__/\___/_/   \__,_/ /___/\___/
 *
 * Crazyradio firmware
 *
 * Copyright (C) 2011-2013 Bitcraze AB
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, in version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * srfSpi.c - SPI driver program the nRF24LU1 with a JTAG Key
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <wiringPi.h>
#include <wiringPiSPI.h>
#include "nrfSpi.h"

#define RESET_N_PIN 6
#define PROG_PIN 5
#define MANUAL_CS_N_PIN 1

//Global variable used here
static int spi_fd;
static char buffer[256+3];  //128bytes buffer + 3bytes for the send command

//Send the buffer, try 10 times and return the number of bytes sent
int ftdiSendBuffer(char *buff, int len)
{
  int i=0, ret, try=0;

  while((i<len) && ((try++)<10)) {
    //printf("i=%d\n", i);
    //putchar('/');

    ret = write(spi_fd, buff+i, len-i);
//    ret = wiringPiSPIDataRW(0, buff, len);
    
    i+=ret;
    if (ret==-1) {
        fprintf(stderr, "Unable to send to the chip (%d): %s\n", errno,
                strerror(errno));
        return -1;
    }

    if (ret != len) {
      fprintf(stderr, "Failed to write all data\n"); // Chip select will be released by HW
      return -1;
    }
  }
  //puts("------\n");
  return i;
}

//Receive the buffer, try 100 times and return the number of bytes sent
int ftdiRecvBuffer(char* buff, int len)
{
  int i=0, ret, try=0;
  
  while((i<len) && ((try++)<100)) {
    //putchar('.');

    ret = read(spi_fd, buff+i, len-i);

//    memset(buff, 0, len);
//    ret = wiringPiSPIDataRW(0, buff, len);

    
    i+=ret;
    if (ret==-1) {
        fprintf(stderr, "Unable to receive from the chip (%d): %s\n", errno,
                strerror(errno));
        return -1;
    }

    if (ret != len) {
      fprintf(stderr, "Failed to read all data\n"); // Chip select will be released by HW
      return -1;
    }
  }
  
  return i;
}


int spiInit()
{
  wiringPiSetup();

  int channel = 0;
  int speed_hz = 1200000;
  int mode = 0;
  spi_fd = wiringPiSPISetupMode(channel, speed_hz, mode);
  if (spi_fd == -1)
  {
    fprintf(stderr, "Unable to setup spi: %d (%s)\n", errno, strerror(errno));
    return -1;
  }
  
  // need to set spi pins in/out?

  pinMode(PROG_PIN, OUTPUT);
  pinMode(RESET_N_PIN, OUTPUT);

  pinMode(MANUAL_CS_N_PIN, OUTPUT);
  spiSetCS(DIS_CS);
  
  return spiSetResetProg(EN_RESET, DIS_PROG);
}

int spiSetResetProg(int reset, int prog)
{
  usleep(5000);
//  printf("\nRSTn=%d, PROG=%d\n", reset, prog);
  digitalWrite(RESET_N_PIN, reset);
  digitalWrite(PROG_PIN, prog);
  usleep(5000);
  return 0;
}

int spiSetSpiOE(int oe) {
  // not used
  return 0;
}

int spiSetCS(int cs)
{
  // controlled by driver
  usleep(5000);
//  printf("\nCSn=%d\n", cs);
  digitalWrite(MANUAL_CS_N_PIN, cs);
  usleep(5000);
  return 0;
}

void spiDeinit()
{
  // no-op
}

int spiSend(char *data, int len)
{
  if(len<1) return 0;
  if(len>128) {
    fprintf(stderr, "Unable to send more than 128 bytes!\n");
    return -1;
  }

  /* printf("\n"); */
  /* for (int i = 0; i < len; i++) */
  /* { */
  /*   printf("%02x ", data[i]); */
  /* } */
  /* printf("\n"); */
  
  return ftdiSendBuffer(data, len);
}

int spiReceive(char *data, int len)
{
  if(len<1) return 0;
  if(len>256) {
    fprintf(stderr, "Unable to receive more than 128 bytes!\n");
    return -1;
  }
  
  int ret = ftdiRecvBuffer(data, len);
  /* printf("\n"); */
  /* for (int i = 0; i < len; i++) */
  /* { */
  /*   printf("%02x ", data[i]); */
  /* } */
  /* printf("\n"); */
  
  return ret;
}

