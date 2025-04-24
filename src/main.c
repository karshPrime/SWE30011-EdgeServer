
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
        char lSingleChar;

        if ( '"' == lSingleChar )
        {
        }

        if ( ES->Output && MS->Output )
        {
            ProcessDone();
            LinkDispatch( ES->Output, MS->Output );
        }
    }

error:
    ProcessDone();
    LinkTerminate( ES, MS );
    return 0;
}

