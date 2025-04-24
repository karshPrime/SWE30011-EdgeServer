
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "Process.h"
#include "Debug.h"
#include "Structures.h"

pthread_t ThreadMS;
int ThreadStatus;

//- Private Methods --------------------------------------------------------------------------------

void *msAction( void *aData )
{
    // save data
    // process data

    printf( "MS-%s\n", ( char* )aData );
    free(aData);
    return NULL;
}

ColourRGB mapAverageECG( uint aSum )
{
    if ( aSum < (10 * ECG_RATE) ) return RED;
    else if ( aSum < (1000 * ECG_RATE) ) return BLUE;
    else return GREEN;
}


//- Public Methods ---------------------------------------------------------------------------------

void ProcessMS( char *aData, char **aOutput )
{
    char *lCopyData = strdup( aData );
    check_mem( lCopyData );

    ThreadStatus = pthread_create( &ThreadMS, NULL, msAction, (void*)lCopyData );
    check( ThreadStatus == 0, "Error creating thread\n");

    return;

error:
    // think of how to process data sequentially if thread fails

    free(aData);
    return;
}

void ProcessES( char *aData, char **aOutput )
{
    int lValues[ECG_RATE];
    uint lSum = 0;

    for ( int i = 0; i < ECG_RATE; i++ )
    {
        lValues[i] = atoi( aData );
        lSum += lValues[i];

        while ( *aData != ',' && *aData != '\0' )
        {
            aData++;
        }

        if ( *aData == ',' )
        {
            aData++;
        }
    }

    // save in database

    printf( "{\"AHR\":%d,\n", lSum/ECG_RATE );
    snprintf( *aOutput, 10, "{\"AHR\":%d,\n", (int)(mapAverageECG( lSum )));
}

void ProcessDone( void )
{
    if ( !ThreadStatus ) pthread_join( ThreadMS, NULL );
}

