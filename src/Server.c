
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
    );

}

