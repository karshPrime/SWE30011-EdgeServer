
/* Process.c
 *
 *
 */

// #define NDEBUG

#include <unistd.h>
#include <stdlib.h>

#include "Process.h"
#include "Database.h"
#include "Debug.h"

// temporary vars; would be replaced with webserver connecting module
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

    sscanf( aData,
        " \"AX\":%hd,\"AY\":%hd,\"AZ\":%hd,\"GX\":%hd,\"GY\":%hd,\"GZ\":%hd,\"Temp\":%hd",
        &lValues.Accelerometer.X, &lValues.Accelerometer.Y, &lValues.Accelerometer.Z,
        &lValues.Gyro.X, &lValues.Gyro.Y, &lValues.Gyro.Z,
        &lValues.Temperature
    );

    debug( "Temperature: %d\n", lValues.Temperature );
    debug( "Gyro: X=%d, Y=%d, Z=%d\n", lValues.Gyro.X, lValues.Gyro.Y, lValues.Gyro.Z );
    debug( "Accelerometer: X=%d, Y=%d, Z=%d\n",
        lValues.Accelerometer.X, lValues.Accelerometer.Y, lValues.Accelerometer.Z );

    sprintf( *aOutput,
        ",\"LLED\": %d,\"RLED\":%d,\"ULED\":%d,\"DLED\":%d,\"FLED\":%d,\"BLED\":%d,\"TLED\":%d}",
        ( lValues.Accelerometer.X >  MotionSensitiveThreshold ),
        ( lValues.Accelerometer.X < -MotionSensitiveThreshold ),
        ( lValues.Accelerometer.Y >  MotionSensitiveThreshold ),
        ( lValues.Accelerometer.Y < -MotionSensitiveThreshold ),
        ( lValues.Accelerometer.Z >  MotionSensitiveThreshold ),
        ( lValues.Accelerometer.Z < -MotionSensitiveThreshold ),
        ( lValues.Temperature >= TemperatureThreshhold )
    );

    DBWriteMS( &lValues );
}

void ProcessES( char *aData, char **aOutput )
{
    uint lValues[ECG_RATE];
    uint lSum = 0;

    for ( int i = 0; i < ECG_RATE; i++ )
    {
        lValues[i] = atoi( aData );
        lSum += lValues[i];

        while ( *aData != ',' && *aData != '\0' ) aData++;

        if ( *aData == ',' ) aData++;
    }

    debug( "{\"AHR\":%d,\n", lSum/ECG_RATE );
    snprintf( *aOutput, 10, "{\"AHR\":%d,\n", (int)(mapAverageECG( lSum )));

    DBWriteES( lValues );
}

