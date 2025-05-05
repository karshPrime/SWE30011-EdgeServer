
// main.c

#define NDEBUG

#include "Link.h"
#include "Server.h"
#include "Database.h"
#include "Debug.h"

#include <stdio.h>

int main( void )
{
    debug( "Starting the system" );
    Property *ES, *MS;

    link_init( &ES, &MS );
    server_init();
    db_init();

    int lDataIndex = 0;

    while ( TRUE )
    {
        link_fetch( &MS->Data, &ES->Data );

        MS->Process( MS->Data, &MS->Output );
        ES->Process( ES->Data, &ES->Output );

        if ( ES->Output && MS->Output )
        {
            db_threads_join();
            debug( "%s%s", ES->Output, MS->Output );
            link_dispatch( &MS->Output, &ES->Output, server_output() );
        }
    }

error:
    debug( "Terminating the program" );
    db_close();
    link_terminate( ES, MS );

    return 0;
}

