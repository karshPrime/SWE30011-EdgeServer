
#pragma once
#include <stdint.h>

void server_init( void );
void server_update( void );

uint8_t *server_ECG_even( void );
uint8_t *server_ECG_three( void );
uint8_t *server_ECG_else( void );
uint8_t *server_ECG_disconnected( void );

uint16_t server_temperature( void );
uint16_t server_motion_sensitive( void );

