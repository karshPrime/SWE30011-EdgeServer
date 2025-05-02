
/* Link.c
 *
 *
 */

// #define NDEBUG

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

void _setup_serial( void )
{
    SerialConnection = open( SERIAL_PORT, O_RDWR | O_NOCTTY );
    check( SerialConnection != -1, "Unable to open serial port %s", SERIAL_PORT );

    struct termios lTermConfig;
    tcgetattr( SerialConnection, &lTermConfig );
    cfsetispeed( &lTermConfig, BAUDRATE );
    cfsetospeed( &lTermConfig, BAUDRATE );

    lTermConfig.c_cflag |= ( CLOCAL | CREAD );
    lTermConfig.c_cflag &=  ~CSIZE;
    lTermConfig.c_cflag |=   CS8;     // 8 data bits
    lTermConfig.c_cflag &=  ~PARENB;  // No parity
    lTermConfig.c_cflag &=  ~CSTOPB;  // 1 stop bit
    lTermConfig.c_cflag &=  ~CRTSCTS; // No flow control

    tcsetattr( SerialConnection, TCSANOW, &lTermConfig );

    return;

error:
    exit( EXIT_FAILURE );
}


void _property_free( Property *aProperty )
{
    if ( aProperty )
    {
        if ( aProperty->Data ) free( aProperty->Data );
        if ( aProperty->Output ) free( aProperty->Output );
        free( aProperty );
    }
}


//- Public Methods ---------------------------------------------------------------------------------

void link_init( Property **ES, Property **MS )
{
    _setup_serial();

    *ES = malloc( sizeof(Property) );
    check_mem( *ES );
    (*ES)->BracketStart = '[';
    (*ES)->BracketEnd = ']';
    (*ES)->Process = process_ES;
    (*ES)->Output = malloc( sizeof(char) * ES_OUTPUT_SIZE );
    (*ES)->Data = malloc( sizeof(char) * ES_INPUT_SIZE );
    check_mem( (*ES)->Data );

    *MS = malloc( sizeof(Property) );
    check_mem( *MS );
    (*MS)->BracketStart = '{';
    (*MS)->BracketEnd = '}';
    (*MS)->Process = process_MS;
    (*MS)->Output = malloc( sizeof(char) * MS_OUTPUT_SIZE );
    (*MS)->Data = malloc( sizeof(char) * MS_INPUT_SIZE );
    check_mem( (*MS)->Data );

    return;

error:
    link_terminate( *ES, *MS );
    exit( EXIT_FAILURE );
}


void link_terminate( Property *ES, Property *MS )
{
    close( SerialConnection );

    _property_free( ES );
    _property_free( MS );
}


void link_dispatch( char **aResultMS, char **aResultES, char *aServer )
{
    debug( "%s", *aResultES );
    write( SerialConnection, *aResultES, ES_OUTPUT_SIZE );
    aResultES = NULL;

    debug( "%s", aServer );
    write( SerialConnection, aServer, SERVER_OUTPUT_SIZE );
    aResultES = NULL;

    debug( "%s", *aResultMS );
    write( SerialConnection, *aResultMS, MS_OUTPUT_SIZE );
    aResultMS = NULL;
}

