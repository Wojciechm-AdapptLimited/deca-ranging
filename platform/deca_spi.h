/*! ----------------------------------------------------------------------------
 * @file	deca_spi.h.h
 * @brief	SPI Interface Access Functions
 *
 * @attention
 *
 * Copyright 2013 (c) DecaWave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 */

#ifndef DECA_SPI_H
#define DECA_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#define DW_DEFAULT_SPI		(3000) //3MHz
#define DW_FAST_SPI			(20000) //20MHz

#define SPI_CHEETAH_SUPPORT				(1)					// enable SPI over Cheetah USBtoSPI driver support
#define SPI_VCP_SUPPORT					(1)					// enable SPI over Virtual COM port support

#define DECA_MAX_SPI_HEADER_LENGTH      (3)                 // max number of bytes in header (for formatting & sizing)

// -------------------------------------------------------------------------------------------------------------------
// Low level abstract function to open and initialise access to the SPI device.
// Returns 0 if successful or -1 in case of an error.

int openSPI(void) ;

int openSPI_VCP(void);
int openSPI_CH(void);

// -------------------------------------------------------------------------------------------------------------------
// Low level abstract function to close the SPI device.
// returns 0 for success, or -1 for error

int closeSPI(void) ;

int closeSPI_VCP(void);
int closeSPI_CH(void);

// -------------------------------------------------------------------------------------------------------------------
//

int setSPIBitrate(int desiredRatekHz) ;
int setSPIBitrate_VCP(int desiredRatekHz) ;
int setSPIBitrate_CH(int desiredRatekHz) ;

int spiLogEnable(int enable);                       // run time enable/disable logging of SPI activity to a file

#ifdef __cplusplus
}
#endif

#endif /* DECA_SPI_H */



