/*! ----------------------------------------------------------------------------
 * @file	comport.h
 * @brief	Decawave Com port function definitions
 *
 * @attention
 *
 * Copyright 2013 (c) DecaWave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 */

#ifndef COMPORT_H
#define COMPORT_H
#ifdef __cplusplus
extern "C" {
#endif

#include "stdafx.h"

// Open COM port
unsigned long openSerial(const char* device);

// Is the COM port already open
unsigned long isSerialOpen();

// Close COM port
unsigned long closeSerial();

// Read from COM port
//
// This function returns any data available at the port when
// the call is made; it does not wait.
unsigned long readFromSerial(
        char* buffer,       // Buffer to read data into
        unsigned long length, // Size of buffer in bytes
        long* bytesRead         // Bytes read from COM port
);

// Write to COM port
//
// This function waits until all data is written to the port.
unsigned long writeToSerial(
        char* buffer,       // Buffer containing data to write
        unsigned long length, // Size of data in bytes
        long* bytesWritten      // Bytes written to COM port
);

#ifdef __cplusplus
}
#endif

#endif
