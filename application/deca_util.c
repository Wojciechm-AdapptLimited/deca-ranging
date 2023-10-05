/*! ------------------------------------------------------------------------------------------------------------------
 * @file	deca_util.c
 * @brief	DecaWave utility Functions
 *
 * @attention
 * 
 * Copyright 2008, 2013 (c) DecaWave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 */

#include "deca_device_api.h"
#include "deca_util.h"

// -------------------------------------------------------------------------------------------------------------------
// Function to return string of from YYYYMMDD-TTTTTT giving year month daty and time in seconds
//
// Returns pointer to nul byte at end of string (useful for tagging on more data)
//
#ifdef _MSC_VER
char * filldatetimestring(char *buffer, size_t bufSize)  // input parameter, pointer to buffer at least DECA_STAMP_STRING_LENGTH long
{
   time_t timer;
   struct tm tblock;
   int cnt ;

   /* gets time of day */
   timer = time(NULL);

   /* converts date/time to a structure */

   localtime_s(&tblock,&timer);

   cnt = sprintf_s(buffer,bufSize,
                 "%4i%02i%02i-%02i%02i%02i",
                 tblock.tm_year+1900,tblock.tm_mon+1,tblock.tm_mday,
                 tblock.tm_hour,tblock.tm_min,tblock.tm_sec) ;

   return buffer + cnt ;

} // end fillDateTimeString()
#endif


// -------------------------------------------------------------------------------------------------------------------
// convert device time to microseconds
uint64 convertMicrosecondsToDeviceTime (double microSeconds)
{
   return (uint64) ((microSeconds / (double) DWT_TIME_UNITS) / 1e6);
}

double convertDeviceTimeToSecondsU64(uint64 dt)
{
	return (double) dt * DWT_TIME_UNITS ;  // seconds #define TIME_UNITS          (1.0/499.2e6/128.0) = 15.65e-12
}

double convertDeviceTimeToSeconds64(int64 dt)
{
	return (double) dt * DWT_TIME_UNITS ; // seconds #define TIME_UNITS          (1.0/499.2e6/128.0) = 15.65e-12
}

double convertDeviceTimeToSeconds8(uint8* dt)
{
    double f;

	uint32 lo = 0;
	uint8 hi;

	memcpy(&lo, dt, 4);
	hi = dt[4] ;

	f = ((hi * 65536.00 * 65536.00) + (double) lo) * DWT_TIME_UNITS ;  // seconds #define TIME_UNITS          (1.0/499.2e6/128.0) = 15.65e-12

	return f ;
}
// -------------------------------------------------------------------------------------------------------------------
// Figure if byte swap is needed because of device endianness.
//
int isBigEndian(void)
{
    static int bigEndian = -1 ;                             // -1 unless test is done and then we can return its value

    if (bigEndian != -1) return bigEndian ;

    {
        union
        {
            unsigned char string[2] ;
            uint16  s2byte ;
        } endianTest ;

        endianTest.string[0] = 0x11 ;
        endianTest.string[1] = 0x22 ;

        if (endianTest.s2byte == 0x1122) bigEndian = 1 ;    // big endian - most significant byte first
        else bigEndian = 0 ;                                // otherwise its little endian

        return bigEndian ;
    }
}


