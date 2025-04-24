
// main.c

#include "Link.h"
#include "Process.h"
#include "Structures.h"
#include "Debug.h"

int main( void )
{
    debug( "Starting..." );

    Property *ES, *MS;
    LinkInit( &ES, &MS );

    while ( TRUE )
    {
        Property *lCurrentSelected = NULL;
        int lDataIndex = 0;
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
            ProcessDone();
            LinkDispatch( &ES->Output, &MS->Output );
        }
    }

error:
    ProcessDone();
    LinkTerminate( ES, MS );
    return 0;
}

