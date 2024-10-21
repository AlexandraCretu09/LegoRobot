/*
 * spi.c
 *
 *  Created on: Mar 8, 2024
 *      Author: valy
 */

#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <stdio.h>            // for printf
#include <string.h>           // for strstr
#include "spi.h"

int spi_file_handle = -1;                    // SPI file handle
struct spi_ioc_transfer spi_xfer_struct;     // SPI transfer struct
uint8_t spi_array_out[LONGEST_SPI_TRANSFER]; // SPI out array
uint8_t spi_array_in[LONGEST_SPI_TRANSFER];  // SPI in array

int spi_setup(){
  spi_file_handle = open(SPIDEV_FILE_NAME, O_RDWR);

  if (spi_file_handle < 0){
    return ERROR_SPI_FILE;
  }

  spi_xfer_struct.cs_change = 0;               // Keep CS activated
  spi_xfer_struct.delay_usecs = 0;             // delay in us
  spi_xfer_struct.speed_hz = SPI_TARGET_SPEED; // speed
  spi_xfer_struct.bits_per_word = 8;           // bites per word 8

  return ERROR_NONE;
}

// Transfer length number of bytes. Write from outArray, read to inArray.
int spi_transfer_array(uint8_t length, uint8_t *outArray, uint8_t *inArray){
  spi_xfer_struct.len = length;
  spi_xfer_struct.tx_buf = (unsigned long)outArray;
  spi_xfer_struct.rx_buf = (unsigned long)inArray;

  if (ioctl(spi_file_handle, SPI_IOC_MESSAGE(1), &spi_xfer_struct) < 0) {
    return ERROR_SPI_FILE;
  }

  return ERROR_NONE;
}

// Function to call if an error occured that can not be resolved, such as failure to set up SPI
void fatal_error(char *error){
  printf(error);
  printf("\n");
  exit(-1);
}
/*
// Function to call if an error occured that can not be resolved, such as failure to set up SPI
void fatal_error(const char *error){
  printf(error);
  printf("\n");
  exit(-1);
}
*/

// Set a BrickPi3's address to allow stacking
int BrickPi3_set_address(int addr, const char *id){
  if(addr < 1 || addr > 255){
    fatal_error("BrickPi3_set_address error: invalid address. Must be in the range of 1 to 255");
    return -1;
  }

  spi_array_out[0] = 0;                         // use address 0 so all BrickPi3s will listen, regardless of current address
  spi_array_out[1] = BPSPI_MESSAGE_SET_ADDRESS;
  spi_array_out[2] = addr;
  for(uint8_t i = 0; i < 16; i++){
    if(strlen(id) == 32){
      char id_str[2];
      id_str[0] = *(id + (i * 2));
      id_str[1] = *(id + (i * 2) + 1);
      spi_array_out[3 + i] = strtol(id_str, NULL, 16);
    }else if(strlen(id) == 0){
      spi_array_out[3 + i] = 0;
    }else{
      fatal_error("BrickPi3_set_address error: wrong serial number id length. Must be a 32-digit hex string.");
    }
  }
  if(spi_transfer_array(19, spi_array_out, spi_array_in)){
    return -1;
  }
}

