
#pragma once

#include "Structures.h"

void DBInit( void );
void DBClose( void );

void DBWriteMS( MotionValues * );
void DBWriteES( uint[] );
void DBThreadsJoin( void );

