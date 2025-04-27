
#include <unistd.h>
#include <stdlib.h>
#include "Process.h"
#include "Debug.h"
#include "Database.h"

const int MotionSensitiveThreshold = 1000;
const int TemperatureThreshhold = 10;

//- Private Methods --------------------------------------------------------------------------------

ColourRGB mapAverageECG( uint aSum )
{
    if ( aSum < (10 * ECG_RATE) ) return RED;
    else if ( aSum < (1000 * ECG_RATE) ) return BLUE;
    else return GREEN;
}


//- Public Methods ---------------------------------------------------------------------------------

void ProcessMS( char *aData, char **aOutput )
{
    MotionValues lValues;

    debug( "%s\n", ( char* )aData );

    DBWriteMS( &lValues );
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

    debug( "{\"AHR\":%d,\n", lSum/ECG_RATE );
    snprintf( *aOutput, 10, "{\"AHR\":%d,\n", (int)(mapAverageECG( lSum )));

    DBWriteES( lValues );
}

