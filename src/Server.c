
// #define NDEBUG

#include "Server.h"
#include "Config.h"
#include "Debug.h"

//- Global Vars ------------------------------------------------------------------------------------

ServerValues Values;
char OutputString[SERVER_OUTPUT_SIZE];


//- Public Methods ---------------------------------------------------------------------------------

void ServerInit( void )
{
    // Initial Values
    Values.RGB.Red   = 0;
    Values.RGB.Green = 0;
    Values.RGB.Blue  = 0;
}

char *ServerOutput( void )
{
    sprintf( 
        OutputString,
        "\"RGB\":{\"Red\":%d,\"Green\":%d,\"Blue\":%d},",
        Values.RGB.Red, Values.RGB.Green, Values.RGB.Blue
    );

    debug( "%s", OutputString );
    return OutputString;
}

