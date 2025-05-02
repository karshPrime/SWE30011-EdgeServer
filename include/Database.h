
#pragma once

#include "Config.h"

void db_init( void );
void db_close( void );

void db_write_MS( MotionValues * );
void db_write_ES( uint[] );
void db_threads_join( void );

