
#pragma once

#include "Config.h"

void DBInit( void );
void DBClose( void );

void DBWriteMS( MotionValues * );
void DBWriteES( uint[] );
void DBThreadsJoin( void );

