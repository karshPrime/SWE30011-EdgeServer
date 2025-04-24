
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

#include "Link.h"
#include "Process.h"
#include "Debug.h"

//- Global Vars ------------------------------------------------------------------------------------

int SerialConnection;

int GetSerialConnection( void ) { return SerialConnection; }

//- Private Methods --------------------------------------------------------------------------------

void setupSerial( void )
{
    debug( "Setting Serial Connection" );

    SerialConnection = open( SERIAL_PORT, O_RDWR | O_NOCTTY );
    check( SerialConnection != -1, "Unable to open serial port" );

    struct termios lTermConfig;
    tcgetattr( SerialConnection, &lTermConfig );
    cfsetispeed( &lTermConfig, BAUDRATE );
    cfsetospeed( &lTermConfig, BAUDRATE );

    lTermConfig.c_cflag |= ( CLOCAL | CREAD );
    lTermConfig.c_cflag &= ~CSIZE;
    lTermConfig.c_cflag |= CS8;      // 8 data bits
    lTermConfig.c_cflag &= ~PARENB;  // No parity
    lTermConfig.c_cflag &= ~CSTOPB;  // 1 stop bit
    lTermConfig.c_cflag &= ~CRTSCTS; // No flow control

    tcsetattr( SerialConnection, TCSANOW, &lTermConfig );

    return;

error:
    exit( EXIT_FAILURE );
}

void propertyFree( Property *aProperty )
{
    if ( aProperty )
    {
        if ( aProperty->Data ) free( aProperty->Data );
        free( aProperty );
    }
}


//- Public Methods ---------------------------------------------------------------------------------

void LinkInit( Property **ES, Property **MS )
{
    setupSerial();

    *ES = malloc( sizeof(Property) );
    check_mem( *ES );
    (*ES)->BracketStart = '[';
    (*ES)->BracketEnd = ']';
    (*ES)->Process = ProcessES;
    (*ES)->Output = NULL;
    (*ES)->Data = malloc( sizeof(char) * MAX_ES_SIZE );
    check_mem( (*ES)->Data );

    *MS = malloc( sizeof(Property) );
    check_mem( *MS );
    (*MS)->BracketStart = '{';
    (*MS)->BracketEnd = '}';
    (*MS)->Process = ProcessMS;
    (*MS)->Output = NULL;
    (*MS)->Data = malloc( sizeof(char) * MAX_ES_SIZE );
    check_mem( (*MS)->Data );

    return;
error:
    close( SerialConnection );
    exit( EXIT_FAILURE );
}

void LinkTerminate( Property *ES, Property *MS )
{
    close( SerialConnection );

    propertyFree( ES );
    propertyFree( MS );
}

void LinkDispatch( char **aResultMS, char **aResultES )
{
    char lOutputBuffer[MAX_OUT_SIZE];
    snprintf(
        lOutputBuffer, sizeof(lOutputBuffer),
        "%s%s\n",
        *aResultES, *aResultMS
    );

    write( SerialConnection, lOutputBuffer, strlen( lOutputBuffer ) );
    debug( "%s", lOutputBuffer );

    aResultES = NULL;
    aResultMS = NULL;
}

