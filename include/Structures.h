
#pragma once

#include <sys/types.h>

//- Typedefs ---------------------------------------------------------------------------------------

typedef const char* str;
typedef unsigned int uint;

// Should stay same as the enum defined in IoT firmware
typedef enum { STOP, START, UNCHANGED, BEEP, BEEP_FAST, BEEP_SLOW } AStatus;

typedef enum { FALSE, TRUE } bool;

typedef struct {
    bool    Read;
    char    BracketStart;
    char    BracketEnd;
    char   *Data;
    char   *Output;
    void( *Process )( char * );
} Property;

