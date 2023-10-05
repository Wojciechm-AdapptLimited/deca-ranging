/*! ----------------------------------------------------------------------------
 * @file	comport.c
 * @brief	Decawave Com port functions
 *
 * @attention
 *
 * Copyright 2013 (c) DecaWave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 */

#include "comport.h"
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>


static int portFd;

// Open COM port
unsigned long openSerial(const char* device) {
    int parity = 0;
    struct termios tty;

    // Get handle to COM port
    portFd = open(device, O_RDWR | O_NOCTTY);

    if (portFd < 0) {
        return errno;
    }

    // Read current port parameters
    if (tcgetattr(portFd, &tty) != 0) {
        return errno;
    }

    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);

    // Change port parameters as needed
    //dcb.BaudRate        = CBR_9600;
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl


    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 0;

    // Set port parameters
    if (tcsetattr(portFd, TCSANOW, &tty) != 0) {
        return errno;
    }

    // A value of MAXDWORD, combined with zero values for both the
    // ReadTotalTimeoutConstant and ReadTotalTimeoutMultiplier members,
    // specifies that the read operation is to return immediately with the
    // bytes that have already been received, even if no bytes have been
    // received.

    return EXIT_SUCCESS;
}

//
unsigned long isSerialOpen() {
    return (portFd >= 0); //open
}

// Close COM port
unsigned long closeSerial() {
    unsigned long error = 0;

    if (portFd >= 0) {
        if (close(portFd) != 0) {
            error = errno;
        } else {
            portFd = -1;
        }
    }

    return error;
}


// Read from COM port
//
// This function returns any data available at the port when
// the call is made; it does not wait.
unsigned long readFromSerial(char* buffer, unsigned long length, long* bytesRead) {
    *bytesRead = read(portFd, buffer, length);

    if (*bytesRead == -1) {
        return errno;
    }

    return EXIT_SUCCESS;
}


// Write to COM port
//
// This function waits until all data is written to the port.
unsigned long writeToSerial(char* buffer, unsigned long length, long* bytesWritten) {
    *bytesWritten = write(portFd, buffer, length);

    if (*bytesWritten == -1) {
        return errno;
    }

    return EXIT_SUCCESS;
}

