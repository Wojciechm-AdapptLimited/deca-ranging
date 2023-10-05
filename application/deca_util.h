/*! ----------------------------------------------------------------------------
 * @file	deca_util.h
 * @brief	DecaWave header for utility functions
 *
 * @attention
 *
 * Copyright 2013 (c) DecaWave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 */

#ifndef DECA_UTIL_H
#define DECA_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "deca_types.h"

#define DECA_STAMP_STRING_LENGTH     (16)    // Length of string returned for time stamp function (including terminating nul)

// This file defines data and functions for access to Parameters in the Device

// -------------------------------------------------------------------------------------------------------------------
// Function to return string of from YYYYMMDD-TTTTTT giving year month daty and time in seconds
//
// Returns handle for use in talking to device or -1 in case of an error.
//

char * fillDateTimeString       // returns pointer to nul byte at end of string (useful for tagging on more data)
(
    char *buffer,               // input parameter, pointer to buffer which should be at least DECA_STAMP_STRING_LENGTH long
    unsigned int bufSize
) ;

// -------------------------------------------------------------------------------------------------------------------

uint64 convertMicrosecondsToDeviceTime (double microSeconds);

double convertDeviceTimeToSeconds8(uint8* dt);
double convertDeviceTimeToSecondsU64(uint64 dt);
double convertDeviceTimeToSeconds64(int64 dt);

int isBigEndian(void) ;             // 1 = if big-endian , 0 = little-endian

#ifdef __cplusplus
}
#endif

#endif


