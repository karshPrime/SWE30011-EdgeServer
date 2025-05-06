
#pragma once

#include <sys/types.h>

//- System Configs ---------------------------------------------------------------------------------

// Serial Communication
#define SERIAL_PORT "/dev/cu.usbmodem579A0192001"
#define BAUDRATE B9600

// I/O Buffer Size
#define MS_INPUT_SIZE       1024
#define MS_OUTPUT_SIZE      128
#define ES_INPUT_SIZE       8192
#define ES_OUTPUT_SIZE      32
#define SERVER_OUTPUT_SIZE  50

// System Defines
#define ECG_RATE 256

// Database Authenticaion
#define DB_HOST "localhost"
#define DB_USER "edge"
#define DB_PASS ""
#define DB_NAME "swe30011"


//- Custom Datatypes -------------------------------------------------------------------------------

// Should stay same as the enum defined in IoT firmware
typedef enum { STOP, START, UNCHANGED, BEEP, BEEP_FAST, BEEP_SLOW } AStatus;
typedef enum { RANGE1, RANGE2, RANGE3, RANGE4, DISCONNECTED } ValuesECG;

typedef enum { FALSE, TRUE } bool;
typedef const char* str;
typedef unsigned int uint;

typedef struct {
    struct {
        int16_t X, Y, Z;
    } Accelerometer;

    struct {
        int16_t X, Y, Z;
    } Gyro;

    double Temperature;
} MotionValues;

typedef struct {
    struct {
        uint  Red;
        uint  Green;
        uint  Blue;
    } RGB;

    // possible to expand
} ServerValues;

typedef struct {
    str         Key;
    AStatus     Status;
} OutputMap;

typedef struct {
    bool    Read;
    char    BracketStart;
    char    BracketEnd;
    char   *Data;
    char   *Output;
    void( *Process )( char *, char ** );
} Property;

typedef struct {
    pthread_t   Thread;
    bool        Status;
} Thread;

