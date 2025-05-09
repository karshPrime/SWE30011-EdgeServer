
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

uint8_t *_map_average_ECG( uint aSum )
{
    const int lSum = aSum % ECG_RATE;

    if ( aSum < ( 100 * ECG_RATE ) ) return server_ECG_disconnected();
    else if ( lSum % 2 == 0 ) return server_ECG_even();
    else if ( lSum % 3 == 0 ) return server_ECG_three();
    else return server_ECG_else();
}


//- Public Methods ---------------------------------------------------------------------------------

void process_MS( char *aData, char **aOutput )
{
    debug( "%s", aData );

    MotionValues lValues;
    sscanf(aData,
        "{\"AX\":%hd,\"AY\":%hd,\"AZ\":%hd,\"GX\":%hd,\"GY\":%hd,\"GZ\":%hd,\"Temp\":%hd}",
        &lValues.Accelerometer.X, &lValues.Accelerometer.Y, &lValues.Accelerometer.Z,
        &lValues.Gyro.X, &lValues.Gyro.Y, &lValues.Gyro.Z,
        &lValues.Temperature
    );

    const uint16_t lMotionSensitive = server_motion_sensitive();

    lValues.Temperature /= ( lValues.Temperature < 0 ) ? -237 : 237;

    sprintf( *aOutput,
        "\"LEDR\":%d,\"LEDL\":%d,\"LEDU\":%d,\"LEDD\":%d,\"BUZ\":%d}",
        ( lValues.Accelerometer.Y < ( lMotionSensitive * -1 ) ),
        ( lValues.Accelerometer.Y >  lMotionSensitive ),
        ( lValues.Accelerometer.X >  lMotionSensitive ),
        ( lValues.Accelerometer.X < ( lMotionSensitive * -1 )  ),
        ( lValues.Temperature >= server_temperature() )
    );

    debug( "%s", *aOutput );
    db_write_MS( &lValues );
}


void process_ES( char *aData, char **aOutput )
{
    debug( "%s", aData );
    db_write_ES( aData );

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

    uint8_t *lColours = _map_average_ECG( lSum );
    snprintf( *aOutput, 64, "{\"HRR\":%d,\"HRG\":%d,\"HRB\":%d,",
        lColours[0], lColours[1], lColours[2] );

    debug( "%s", *aOutput );
}

