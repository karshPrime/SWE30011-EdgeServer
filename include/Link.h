
#pragma once

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include "Config.h"

void LinkInit( Property **, Property ** );
void LinkTerminate( Property *, Property * );
void LinkDispatch( char **, char ** );

int GetSerialConnection( void );

#define LinkFetch( SingleChar ) read( GetSerialConnection(), SingleChar, 1 )

