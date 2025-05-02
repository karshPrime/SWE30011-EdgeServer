
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
        Property *lCurrentSelected = NULL;
        char lSingleChar;

        link_fetch( &lSingleChar );
        if ( '"' == lSingleChar )
        {
            link_fetch( &lSingleChar );

            if ( 'M' == lSingleChar )
            {
                lCurrentSelected = MS;
                lDataIndex = 0;
            }
            else if ( 'E' == lSingleChar && 0 != lDataIndex )
            {
                lCurrentSelected = ES;
                lDataIndex = 0;
            }
            else
            {
                continue;
            }

            while ( lSingleChar != lCurrentSelected->BracketStart ) link_fetch( &lSingleChar );
        }

        if ( lCurrentSelected )
        {
            while ( lSingleChar != lCurrentSelected->BracketEnd )
            {
                link_fetch( &lSingleChar );
                lCurrentSelected->Data[lDataIndex++] = lSingleChar;
            }

            lCurrentSelected->Data[lDataIndex-1] = '\0';
            lCurrentSelected->Process( lCurrentSelected->Data, &lCurrentSelected->Output );
        }

        if ( ES->Output && MS->Output )
        {
            db_threads_join();
            debug( "%s%s", ES->Output, MS->Output );
            link_dispatch( &ES->Output, &MS->Output, server_output() );
        }
    }

error:
    debug( "Terminating the program" );
    db_close();
    link_terminate( ES, MS );

    return 0;
}

