
#pragma once

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include "Config.h"

void link_init( Property **, Property ** );
void link_terminate( Property *, Property * );
void link_dispatch( char **, char ** );
void link_fetch( char **, char ** );

