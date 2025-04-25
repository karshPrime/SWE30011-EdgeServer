
#pragma once

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "Structures.h"

#define SERIAL_PORT "/dev/cu.usbmodem579A0192001"
#define BAUDRATE B9600

#define MS_INPUT_SIZE   1024
#define MS_OUTPUT_SIZE  128
#define ES_INPUT_SIZE   8192
#define ES_OUTPUT_SIZE  32

#define LinkFetch( SingleChar ) read( GetSerialConnection(), SingleChar, 1 )


//- Public Methods ---------------------------------------------------------------------------------

void LinkInit( Property **, Property ** );
void LinkTerminate( Property *, Property * );

void LinkDispatch( char **, char ** );

int GetSerialConnection( void );

