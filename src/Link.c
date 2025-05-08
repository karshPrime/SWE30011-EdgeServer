
/* Link.c
 *
 *
 */

#define NDEBUG

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
    log_error( "Critical Error. Terminating." );
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
    log_error( "Critical Error. Terminating." );
    link_terminate( *ES, *MS );
    exit( EXIT_FAILURE );
}


void link_terminate( Property *ES, Property *MS )
{
    close( SerialConnection );

    _property_free( ES );
    _property_free( MS );
}


void link_dispatch( char **aResultMS, char **aResultES )
{
    debug( "%s%s", *aResultES, *aResultMS );

    write( SerialConnection, *aResultES, strlen(*aResultES) );
    aResultES = NULL;

    write( SerialConnection, *aResultMS, strlen(*aResultMS) );
    aResultMS = NULL;
}


void link_fetch( char **aMS, char **aES )
{
    static char lBuffer[MS_INPUT_SIZE + ES_INPUT_SIZE];
    static size_t lBufferIndex = 0;
    static bool lCapturing = FALSE;
    static int lBraceCount = 0;

    char lTempRead[128];
    ssize_t lBytesRead;

    while ( TRUE )
    {
        lBytesRead = read( SerialConnection, lTempRead, sizeof( lTempRead ) - 1 );
        if ( lBytesRead <= 0 ) return;

        for ( ssize_t i = 0; i < lBytesRead && lBufferIndex < sizeof( lBuffer ) - 1; i++ )
        {
            char lCurrentChar = lTempRead[i];

            // Start of message
            if ( !lCapturing && lCurrentChar == '{' && i + 5 < lBytesRead &&
                    strncmp( &lTempRead[i], "{\"MS\":", 6 ) == 0 )
            {
                lCapturing = TRUE;
                lBufferIndex = 0;
                lBraceCount = 0;

                size_t remaining = lBytesRead - i;
                if ( remaining > sizeof( lBuffer ) - 1 ) remaining = sizeof( lBuffer ) - 1;

                memcpy( lBuffer, &lTempRead[i], remaining );
                lBufferIndex = remaining;

                // Count lBraces
                for ( size_t j = 0; j < remaining; j++ )
                {
                    if ( lBuffer[j] == '{' ) lBraceCount++;
                    else if ( lBuffer[j] == '}' ) lBraceCount--;
                }

                break;
            }
            else if ( lCapturing )
            {
                lBuffer[lBufferIndex++] = lCurrentChar;

                if ( lCurrentChar == '{' )
                    lBraceCount++;
                else if ( lCurrentChar == '}' )
                    lBraceCount--;

                if ( lBraceCount == 0 )
                {
                    lBuffer[lBufferIndex] = '\0';

                    // Extract MS and ES
                    char *lStartMS = strstr( lBuffer, "{\"MS\":" );
                    char *lStartES = strstr( lBuffer, "\"ES\":[" );

                    if ( lStartES && lStartMS )
                    {
                        lStartMS += 6; // skip "{\"MS\":"
                        char *lEndMS = lStartES - 1;

                        while ( *lEndMS != '}' ) lEndMS--; // go back to close MS object

                        size_t msLen = lEndMS - lStartMS + 1;
                        static char msBuf[1024];
                        strncpy( msBuf, lStartMS, msLen );
                        msBuf[msLen] = '\0';

                        *aMS = msBuf;

                        char *esEnd = strrchr( lBuffer, ']' );
                        if ( esEnd && esEnd > lStartES )
                        {
                            lStartES += 5; // skip "\"ES\":"
                            size_t lLenES = esEnd - lStartES + 1;

                            static char esBuf[4096];
                            strncpy( esBuf, lStartES, lLenES );
                            esBuf[lLenES] = '\0';

                            *aES = esBuf;
                        }
                    }

                    lCapturing = FALSE;
                    lBufferIndex = 0;
                    return;
                }
            }
        }
    }
}

