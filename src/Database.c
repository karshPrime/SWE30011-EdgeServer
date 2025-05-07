
/* Database.c
 *
 *
 */

#define NDEBUG

#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <time.h>
#include <pthread.h>
#include "Database.h"
#include "Debug.h"


//- Global Vars ------------------------------------------------------------------------------------

Thread ThreadMS;
Thread ThreadES;
static MYSQL *DBConnection = NULL;


//- Private Methods --------------------------------------------------------------------------------

void *_save_MS( void *aValues )
{
    debug( "_save_MS called" );
    MotionValues *lValues = ( MotionValues* )aValues;

    char lQuery[512];
    time_t lCurrentTime = time( NULL );
    struct tm *lTMInfo = localtime( &lCurrentTime );
    char lTimeString[32];
    strftime( lTimeString, sizeof(lTimeString), "%Y-%m-%d %H:%M:%S", lTMInfo );

    snprintf( lQuery, sizeof(lQuery),
        "INSERT INTO SensorData (time, AcceleratorX, AcceleratorY, AcceleratorZ, "
        "GyroscopeX, GyroscopeY, GyroscopeZ, Temperature) "
        "VALUES ('%s', %d, %d, %d, %d, %d, %d, %.2f)",
        lTimeString,
        lValues->Accelerometer.X, lValues->Accelerometer.Y, lValues->Accelerometer.Z,
        lValues->Gyro.X, lValues->Gyro.Y, lValues->Gyro.Z,
        lValues->Temperature
    );

    check ( !mysql_query( DBConnection, lQuery ), "INSERT failed: %s\n", mysql_error(DBConnection));

    return NULL;

error:
    log_error( "Unable to save MS to the Database" );
    return NULL;
}


void *_save_ES( void *aValues )
{
    debug( "_save_ES called" );

    return NULL;

error:
    log_error( "Unable to write MS to the Database" );
    return NULL;
}


//- Public Methods ---------------------------------------------------------------------------------

void db_init( void )
{
    DBConnection = mysql_init( NULL );

    check ( DBConnection != NULL,  "mysql_init() failed" );

    check(
        mysql_real_connect( DBConnection, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, NULL, 0 ) != NULL,
        "mysql_real_connect() failed\nError: %s\n", mysql_error( DBConnection )
    );

    db_threads_join();
    return;

error:
    log_error( "Closing database connection" );
    db_close();
}


void db_close( void )
{
    if ( DBConnection )
    {
        mysql_close( DBConnection );
        DBConnection = NULL;
    }

    db_threads_join();
}


void db_write_MS( MotionValues *aValues )
{
    check( DBConnection, "Database connection is not initialized.\n" );

    ThreadMS.Status = pthread_create( &ThreadMS.Thread, NULL, _save_MS, (void*)aValues );
    check( ThreadMS.Status == FALSE, "Error creating MS thread\n");


    return;

error:
    // think of how to process data sequentially if thread fails
    log_error( "Unable to reserve thread to write MS to database" );
    return;
}


void db_write_ES( char *aValues )
{
    check( DBConnection, "Database connection is not initialized.\n" );

    ThreadES.Status = pthread_create( &ThreadES.Thread, NULL, _save_ES, (void*)aValues );
    check( ThreadES.Status == 0, "Error creating ES thread\n");


    return;

error:
    // think of how to process data sequentially if thread fails
    log_error( "Unable to reserve thread to write ES to database" );
    return;
}


void db_threads_join( void )
{
    if ( !ThreadMS.Status ) pthread_join( ThreadMS.Thread, NULL );
    if ( !ThreadES.Status ) pthread_join( ThreadES.Thread, NULL );
}

