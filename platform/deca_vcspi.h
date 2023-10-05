/*! ----------------------------------------------------------------------------
 * @file	deca_vcspi.h
 * @brief	Virtual COM port (USB to SPI)
 *
 * @attention
 *
 * Copyright 2013 (c) DecaWave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 */

#ifndef DECA_VCSPI_H
#define DECA_VCSPI_H

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
    int com_port;
    int hispeed;           //if 1 it is high speed, 0 it is slow
    int connected;
	int dw1000;
	int usbrev;

} usbtospiConfig_t ;

int openPort(int port);

int closePort(int port);

int writeData(char* buffer, unsigned long length, int *);

int readData(char* buffer, int buffer_size, int *);

int findOpenEVB1000Port(void);

int vcSPISetSPIBitrate(int desiredRatekHz);
#ifdef __cplusplus
}
#endif

#endif //DECA_VCSPI_H

