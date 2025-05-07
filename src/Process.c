
/* Process.c
 *
 *
 */

#define NDEBUG

#include <unistd.h>
#include <stdlib.h>

#include "Process.h"
#include "Database.h"
#include "Server.h"
#include "Debug.h"

//- Private Methods --------------------------------------------------------------------------------

ValuesECG _map_average_ECG( uint aSum )
{
    if ( aSum < ( server_ECG_disconnected() * ECG_RATE ) ) return DISCONNECTED;
    else if ( aSum < (  server_ECG_range1() * ECG_RATE ) ) return RANGE1;
    else if ( aSum < (  server_ECG_range2() * ECG_RATE ) ) return RANGE2;
    else if ( aSum < (  server_ECG_range3() * ECG_RATE ) ) return RANGE3;
    else return RANGE4;
}


//- Public Methods ---------------------------------------------------------------------------------

void process_MS( char *aData, char **aOutput )
{
    MotionValues lValues;

    debug( "%s", aData );

    sscanf( aData,
        " \"AX\":%hd,\"AY\":%hd,\"AZ\":%hd,\"GX\":%hd,\"GY\":%hd,\"GZ\":%hd,\"Temp\":%lf",
        &lValues.Accelerometer.X, &lValues.Accelerometer.Y, &lValues.Accelerometer.Z,
        &lValues.Gyro.X, &lValues.Gyro.Y, &lValues.Gyro.Z,
        &lValues.Temperature
    );

    sprintf( *aOutput,
        "\"LLED\":%d,\"RLED\":%d,\"ULED\":%d,\"DLED\":%d,\"FLED\":%d,\"BLED\":%d,\"TLED\":%d}",
        ( lValues.Accelerometer.X >  server_motion_sensitive() ),
        ( lValues.Accelerometer.X < -server_motion_sensitive() ),
        ( lValues.Accelerometer.Y >  server_motion_sensitive() ),
        ( lValues.Accelerometer.Y < -server_motion_sensitive() ),
        ( lValues.Accelerometer.Z >  server_motion_sensitive() ),
        ( lValues.Accelerometer.Z < -server_motion_sensitive() ),
        ( lValues.Temperature >= server_temperature() )
    );

    debug( "%s", *aOutput );
    db_write_MS( &lValues );
}


void process_ES( char *aData, char **aOutput )
{

    debug( "%s", aData );
    // use the array when there's more need for the data but to just take the average
    // uint lValues[ECG_RATE];
    uint lSum = 0;

    for ( int i = 0; i < ECG_RATE; i++ )
    {
        // lValues[i] = atoi( aData );
        // lSum += lValues[i];

        lSum += atoi( aData );

        while ( *aData != ',' && *aData != '\0' ) aData++;

        if ( *aData == ',' ) aData++;
    }

    snprintf( *aOutput, 10, "{\"AHR\":%d,\n", (int)(_map_average_ECG( lSum )));

    debug( "%s", *aOutput );
    db_write_ES( aData );
}

