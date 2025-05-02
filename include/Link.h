
#pragma once

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include "Config.h"

void link_init( Property **, Property ** );
void link_terminate( Property *, Property * );
void link_dispatch( char **, char **, char* );

int GetSerialConnection( void );

#define link_fetch( aSingleChar ) read( GetSerialConnection(), aSingleChar, 1 )

