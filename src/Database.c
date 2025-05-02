
/* Database.c
 *
 *
 */

// #define NDEBUG

#include <pthread.h>
#include "Database.h"
#include "Debug.h"


//- Global Vars ------------------------------------------------------------------------------------

Thread ThreadMS;
Thread ThreadES;


//- Private Methods --------------------------------------------------------------------------------

void *_save_MS( void *aValues )
{
    debug( "_save_MS called" );

error:
    return NULL;
}


void *_save_ES( void *aValues )
{
    debug( "_save_ES called" );

error:
    return NULL;
}


//- Public Methods ---------------------------------------------------------------------------------

void db_init( void )
{
    db_threads_join();
}


void db_close( void )
{
    db_threads_join();
}


void db_write_MS( MotionValues *aValues )
{
    ThreadMS.Status = pthread_create( &ThreadMS.Thread, NULL, saveMS, (void*)aValues );
    check( ThreadMS.Status == FALSE, "Error creating MS thread\n");

error:
    // think of how to process data sequentially if thread fails
    return;
}


void db_write_ES( uint aValues[] )
{
    ThreadES.Status = pthread_create( &ThreadES.Thread, NULL, saveES, (void*)aValues );
    check( ThreadES.Status == 0, "Error creating ES thread\n");

error:
    // think of how to process data sequentially if thread fails
    return;
}


void db_threads_join( void )
{
    if ( !ThreadMS.Status ) pthread_join( ThreadMS.Thread, NULL );
    if ( !ThreadES.Status ) pthread_join( ThreadES.Thread, NULL );
}

