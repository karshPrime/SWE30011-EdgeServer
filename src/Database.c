
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

void *saveMS( void *aValues )
{
    debug( "saveMS called" );
    return NULL;
}

void *saveES( void *aValues )
{
    debug( "saveES called" );
    return NULL;
}


//- Public Methods ---------------------------------------------------------------------------------

void DBInit( void )
{
    DBThreadsJoin();
    //
}

void DBClose( void )
{
    DBThreadsJoin();
    //
}

void DBWriteMS( MotionValues *aValues )
{
    ThreadMS.Status = pthread_create( &ThreadMS.Thread, NULL, saveMS, (void*)aValues );
    check( ThreadMS.Status == FALSE, "Error creating MS thread\n");

error:
    // think of how to process data sequentially if thread fails
    return;
}

void DBWriteES( uint aValues[] )
{
    ThreadES.Status = pthread_create( &ThreadES.Thread, NULL, saveES, (void*)aValues );
    check( ThreadES.Status == 0, "Error creating ES thread\n");

error:
    // think of how to process data sequentially if thread fails
    return;
}

void DBThreadsJoin( void )
{
    if ( !ThreadMS.Status ) pthread_join( ThreadMS.Thread, NULL );
    if ( !ThreadES.Status ) pthread_join( ThreadES.Thread, NULL );
}

