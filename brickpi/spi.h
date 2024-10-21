/*
 * spi.h
 *
 *  Created on: Mar 8, 2024
 *      Author: valy
 */

#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>
#include "brickpi_types.h"

int spi_setup();
int spi_transfer_array(uint8_t length, uint8_t *outArray, uint8_t *inArray);
void fatal_error(char *error);
//void fatal_error(const char *error);
int BrickPi3_set_address(int addr, const char *id);

extern int spi_file_handle;                    // SPI file handle
extern struct spi_ioc_transfer spi_xfer_struct;     // SPI transfer struct
extern uint8_t spi_array_out[]; // SPI out array
extern uint8_t spi_array_in[];  // SPI in array


#endif /* SPI_H_ */
