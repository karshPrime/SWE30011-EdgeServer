
// #define NDEBUG

#include "Server.h"
#include "Config.h"
#include "Debug.h"

//- Global Vars ------------------------------------------------------------------------------------

uint8_t ECGRateEvenValue[]  = {0,255,0};     // default green
uint8_t ECGRateThreeValue[] = {0,0,255};     // default blue
uint8_t ECGRateElseValue[]  = {255,255,255}; // default white

uint8_t ECGRateDisconnectedThreshhold[] = {255,0,0}; // default red
uint16_t MotionSensitiveThreshold = 1000;
uint16_t TemperatureThreshhold    = 10;


//- Get Functions ----------------------------------------------------------------------------------

uint8_t *server_ECG_even( void ) { return ECGRateEvenValue; }
uint8_t *server_ECG_three( void ) { return ECGRateThreeValue; }
uint8_t *server_ECG_else( void ) { return ECGRateElseValue; }
uint8_t *server_ECG_disconnected( void ) { return ECGRateDisconnectedThreshhold; }

uint16_t server_temperature( void ) { return TemperatureThreshhold; }
uint16_t server_motion_sensitive( void ) { return MotionSensitiveThreshold; }


//- Public Methods ---------------------------------------------------------------------------------

void server_init( void ) {}


void server_update( void )
{
    FILE *f = fopen( CONFIG_FILE, "rb" );
    check ( f, "Failed to open config.dat" );

    uint8_t buffer[16];
    check( fread(buffer, 1, 16, f) == 16,  "Failed to read config.dat" );
    fclose( f );

    // Unpack bytes
    for ( int i = 0; i < 3; i++ ) ECGRateEvenValue[i]  = buffer[i];
    for ( int i = 0; i < 3; i++ ) ECGRateThreeValue[i] = buffer[3 + i];
    for ( int i = 0; i < 3; i++ ) ECGRateElseValue[i]  = buffer[6 + i];
    for ( int i = 0; i < 3; i++ ) ECGRateDisconnectedThreshhold[i] = buffer[9 + i];

    MotionSensitiveThreshold = buffer[12] | ( buffer[13] << 8 );
    TemperatureThreshhold    = buffer[14] | ( buffer[15] << 8 );

    debug(
        "ECG-Even:%d,%d,%d\tECG-Three:%d,%d,%d\tECG-Else:%d,%d,%d\tDisconnect:%d,Temp:%d",
        ECGRateEvenValue[0], ECGRateEvenValue[1], ECGRateEvenValue[2],
        ECGRateThreeValue[0], ECGRateThreeValue[1], ECGRateThreeValue[2],
        ECGRateElseValue[0], ECGRateElseValue[1], ECGRateElseValue[2],
        MotionSensitiveThreshold, TemperatureThreshhold
    );

    return;

error:
    fclose( f );
    return;
}

