
#define NDEBUG

#include "Server.h"
#include "Config.h"
#include "Debug.h"

//- Global Vars ------------------------------------------------------------------------------------

ServerValues Values;
char OutputString[SERVER_OUTPUT_SIZE];

int MotionSensitiveThreshold      = 1000;
int TemperatureThreshhold         = 10;
int ECGRateDisconnectedThreshhold = 10;
int ECGRateRange1Threshhold       = 1000;
int ECGRateRange2Threshhold       = 1300;
int ECGRateRange3Threshhold       = 1600;


//- Get Functions ----------------------------------------------------------------------------------

int server_ECG_range1( void ) { return ECGRateRange1Threshhold; }
int server_ECG_range2( void ) { return ECGRateRange2Threshhold; }
int server_ECG_range3( void ) { return ECGRateRange3Threshhold; }
int server_ECG_disconnected( void ) { return ECGRateDisconnectedThreshhold; }

int server_temperature( void ) { return TemperatureThreshhold; }
int server_motion_sensitive( void ) { return MotionSensitiveThreshold; }


//- Public Methods ---------------------------------------------------------------------------------

void server_init( void )
{
    // Initial Values
    Values.RGB.Red   = 0;
    Values.RGB.Green = 0;
    Values.RGB.Blue  = 0;
}


char *server_output( void )
{
    sprintf( 
        OutputString,
        "\"RGB\":{\"Red\":%d,\"Green\":%d,\"Blue\":%d},",
        Values.RGB.Red, Values.RGB.Green, Values.RGB.Blue
    );

    debug( "%s", OutputString );
    return OutputString;
}

