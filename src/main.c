
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

    LinkInit( &ES, &MS );
    ServerInit();
    DBInit();

    int lDataIndex = 0;

    while ( TRUE )
    {
        Property *lCurrentSelected = NULL;
        char lSingleChar;

        LinkFetch( &lSingleChar );
        if ( '"' == lSingleChar )
        {
            LinkFetch( &lSingleChar );

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

            while ( lSingleChar != lCurrentSelected->BracketStart ) LinkFetch( &lSingleChar );
        }

        if ( lCurrentSelected )
        {
            while ( lSingleChar != lCurrentSelected->BracketEnd )
            {
                LinkFetch( &lSingleChar );
                lCurrentSelected->Data[lDataIndex++] = lSingleChar;
            }

            lCurrentSelected->Data[lDataIndex-1] = '\0';
            lCurrentSelected->Process( lCurrentSelected->Data, &lCurrentSelected->Output );
        }

        if ( ES->Output && MS->Output )
        {
            DBThreadsJoin();
            debug( "%s%s", ES->Output, MS->Output );
            LinkDispatch( &ES->Output, &MS->Output, ServerOutput() );
        }
    }

error:
    debug( "Terminating the program" );
    DBClose();
    LinkTerminate( ES, MS );

    return 0;
}

