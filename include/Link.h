
#pragma once

#include "Structures.h"

#define SERIAL_PORT "/dev/cu.usbmodem579A0192001"
#define BAUDRATE B9600

#define MAX_MS_SIZE 1024
#define MAX_ES_SIZE 8192


//- Public Methods ---------------------------------------------------------------------------------

void LinkInit( Property **, Property ** );
void LinkTerminate( Property *, Property * );

void LinkDispatch( char *, char * );

int GetSerialConnection( void );

