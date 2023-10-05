/*! ------------------------------------------------------------------------------------------------------------------
 * @file	deca_vcspi.c
 * @brief	Virtual COM port (USB to SPI)
 *
 * @attention
 * 
 * Copyright 2008, 2013 (c) DecaWave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 */

#include "deca_vcspi.h"
#include "deca_device_api.h"
#include "comport.h"
#include <stdio.h>
#include <unistd.h>

#define DEBUGPRINT (0)
#define BUFFER_SIZE 1024
#define DEFAULT_PORT 24
#define MAX_PORT 31
#define HIGH_SPEED_THRESHOLD 4000
#define COUNT_MAX 1000
#define DW_COUNT_MAX 500000
#define USB_REV 8

usbtospiConfig_t usbToSPIConfig = {DEFAULT_PORT, //default COM port
                                   0, //hispeed (1 == high speed)
                                   0, //connected
                                   0, //DW1000 device
                                   0
};

//open specified COM port
int openPort(int port) {
    unsigned long error;
    char buffer[BUFFER_SIZE];

    sprintf(buffer, "/dev/ttyS%d", port);

    // Open port
    error = openSerial((const char*) buffer);

    if (error != 0) {
        printf("Error %08lx opening ttyS%d.\n", error, port);
        return EXIT_FAILURE;
    }

    printf("Port ttyS%d open.\n", port);
    usbToSPIConfig.com_port = port;

    return EXIT_SUCCESS;
}

#define EVB1000STRA ("EVB1000 USB2SPI 1.0")
#define EVB1000STRB ("EVB1000 USB2SPI 2.0")
#define OCANCHORSTR ("REK1101 USB2SPI 1.1")    //628 and 707 type 1 (fine grain seq off)
#define OCTAGSTR    ("REK1101 USB2SPI 1.0")

int isEVB1000device(void) {
    const int defaultWait = sizeof(EVB1000STRB) + 2;
    const int longWait = sizeof(EVB1000STRB) * 2 + 2;
    int error;
    char buffer[BUFFER_SIZE];
    int size = 0;
    int waitFor = defaultWait; //"deca?" response string length is 22

    memset(buffer, 0, BUFFER_SIZE);

    //send 5 bytes to the COM port and wait for response
    writeData("deca?", strlen("deca?"), &size);

    //read back the string sent by the EVB1000 USB to SPI application
    sleep(2);

    int i = 0;

    do {
        error = readData(buffer + i, 1, &size);

        //if ( (buffer[1] == 'O') && (buffer[20] == '0') ) break;
        if (buffer[i] == 'y')  //reset buffer
        {
            buffer[0] = 'y';
            i = 0;
            waitFor = defaultWait;
        }
        if (buffer[i] ==
            'i')  //reset buffer - we are connecting to a tag/anchor that is already outputting range information
        {
            buffer[0] = buffer[i];
            i = 0;
            waitFor = longWait;
        }
        if (buffer[i] == 'n')  //reset buffer
        {
            buffer[0] = 'n';
            i = 0;
            waitFor = defaultWait;
        }

        i = (i + 1) % BUFFER_SIZE;

        if (i == waitFor) {
            break;
        }

        if (error) {
            break;
        }

    } while (1);

    //check if the read response is "yEVB1000 USB to SPI 1.0"
    if (buffer[0] == 'y') {
        if (strcmp(&buffer[1], EVB1000STRA) == 0) {
            printf("Connected to EVB1000, rev:<%s>\n", &buffer[1]);
            usbToSPIConfig.connected = 1;
            usbToSPIConfig.dw1000 = 1;
            usbToSPIConfig.usbrev = 1;
            return EXIT_FAILURE;
        }

        if (strcmp(&buffer[1], EVB1000STRB) == 0) {
            printf("Connected to EVB1000, rev:<%s>\n", &buffer[1]);
            usbToSPIConfig.connected = 1;
            usbToSPIConfig.dw1000 = 1;
            usbToSPIConfig.usbrev = 2;
            return EXIT_FAILURE;
        }

        if (strcmp(&buffer[1], OCANCHORSTR) == 0) {
            printf("Connected to Anchor, rev:<%s>\n", &buffer[1]);
            usbToSPIConfig.connected = 1;
            usbToSPIConfig.dw1000 = 2;
            usbToSPIConfig.usbrev = 2;
            return EXIT_FAILURE;
        }

        if (strcmp(&buffer[1], OCTAGSTR) == 0) {
            printf("Connected to Tag, rev:<%s>\n", &buffer[1]);
            usbToSPIConfig.connected = 1;
            usbToSPIConfig.dw1000 = 3;
            usbToSPIConfig.usbrev = 2;
            return EXIT_FAILURE;
        }
    }
#if 0
    else if(buffer[0] == 'n')
    {
        if(strcmp(&buffer[1],EVB1000STRB) == 0)
        {
            uint8 buf[6];
            uint16 rxant = 0;
            uint16 txant = 0;
            int read = 0;
            int sw1 = 0x0
                    | 0x0;

            printf("Connected to EVB1000 on port %d, rev:<%s>\n", usbToSPIConfig.com_port, &buffer[1]);
            usbToSPIConfig.connected = 1;
            usbToSPIConfig.dw1000 = 2;
            usbToSPIConfig.usbrev = 2;
            //return 1;


#if 0
            //antenna cal...
            buf[0] = buf[5] = 0x5;
            buf[1] = txant & 0xff;
            buf[2] = (txant >> 8) & 0xff;
            buf[3] = rxant & 0xff;
            buf[4] = (rxant >> 8) & 0xff;
            //send 6 bytes to the COM port and wait for response
            error = writeCOMdata((char*) buf, 6, &size);
#else
            //S1 config
            buf[0] = buf[2] = 0x6;
            buf[1] = sw1;
            //send 3 bytes to the COM port and wait for response
            error = writeCOMdata((char*) buf, 3, &size);
#endif


            i = 0;

            size = 0;

            Sleep(2);
            do
            {
                error = readCOMdata(buffer + i, 1, &size);

                if (buffer[i] == 'i') //reset buffer on 1st char ... ranging output starts with "i" and size is 40 chars.
                {
                    buffer[0] = 'i';
                    i = 0;
                }
                i = (i + 1) % 1024;
                read += size;
                if(i == 42)
                {
                    int w = 0;
                    i = 0;
                    printf("read %d chars, <%s>\n", read, buffer);
                    //break;

                    if(w == 1)
                    {
                        error = writeCOMdata((char*) buf, 6, &size);
                    }

                    read = 0;
                }

                if(error) break;

            } while (1);
        }
    }
#endif

    //if not as expected fail by returning 0
    {
        printf("read %d chars, <%s>\n", size, buffer);
        usbToSPIConfig.connected = 0;
        usbToSPIConfig.dw1000 = 0;
    }

    return EXIT_SUCCESS;
}


int findOpenEVB1000Port(void) {
    int i;

    for (i = 3; i < MAX_PORT; i++) {
        if (openPort(i) != 0) { // i-th port exist and is open
            continue;
        }

        if (isEVB1000device()) { //check if EVB1000 is on this port
            return (i | (usbToSPIConfig.usbrev << USB_REV));
        } //close the COM port

        closePort(i);
    }

    return DWT_ERROR;
}


//close specified COM port
int closePort(int port) {
    unsigned long error = closeSerial();

    if (error != 0) {
        printf("Error %08lx closing ttyS%d.\n", error, port);
        return EXIT_FAILURE;
    }

    usbToSPIConfig.connected = 0; //we have disconnected from the controller
    usbToSPIConfig.dw1000 = 0;

    return EXIT_SUCCESS;
}


int writeData(char* buffer, unsigned long length, int* written) {
    unsigned long error;

#if (DEBUGPRINT == 1)
    int i;
    int len = strlen( buffer );
    printf("GPIBwr:>");
    for(i=0; i<len; i++)
    {
        if(buffer[i] == '\r')
            printf("\\r");
        else
            printf("%c",buffer[i]);
    }
    printf("<\n");
#endif
    error = writeToSerial(buffer, length, (long*) written);         // Write command to port

    //Sleep(2);

    if (error != 0) {
        printf("Error %08lx writing ttyS%d.\n", error, usbToSPIConfig.com_port);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int readData(char* buffer, int buffer_size, int* read) {
    unsigned long error;
    int index = 0;
    int reading = 0;
    int bytesLeft = buffer_size;
    int count = COUNT_MAX;
    int dw_count = DW_COUNT_MAX;
    int start = 0;

    do    //read (buffer_size) bytes from the COM port
    {

        reading = 0;
        error = readFromSerial(&buffer[index], bytesLeft, (long*) &reading);

        if (reading > 0) {
            bytesLeft -= reading; //subtract the read bytes
            index += reading; //next index in the buffer

            //printf("read %d %d %d %d\n", reading, count, bytesLeft, GetLastError());

            start = 1;
            count = COUNT_MAX;
            //Sleep(2);
        } else if (usbToSPIConfig.dw1000 != 2) //don't do timeout for OC ANCHORS
        {
            //Sleep(2);

            if (start && (count-- == 0)) {
                printf("ERROR: read %d %d left %d\n", reading, count, bytesLeft);
                error = 1;
                break;
            }

            if (usbToSPIConfig.dw1000 == 0) //Serial port is not DW1000/ARM
            {
                if ((start == 0) && (count-- == 0)) {
                    printf("ERROR: port read timeout %d %d left %d\n", reading, count, bytesLeft);
                    error = 2;
                    break;
                }
            } else //if(usbToSPIConfig.dw1000 == 1)
            {
                if (((reading + start) == 0) && (dw_count-- == 0)) {
                    printf("ERROR: port read error\n");
                    error = 1;
                    break;
                }
            }
        }


    } while (bytesLeft > 0);

    if (error != 0) {
        printf("Error %08lx reading ttyS%d.\n", error, usbToSPIConfig.com_port);
        return EXIT_FAILURE;
    }
    *read = index;
    return EXIT_SUCCESS;
}


int vcSPISetSPIBitrate(int desiredRatekHz) {
    if (desiredRatekHz < HIGH_SPEED_THRESHOLD) {
        usbToSPIConfig.hispeed = 0;
    } else {
        usbToSPIConfig.hispeed = 1;
    }

    return EXIT_SUCCESS;
}