/*
    Copyright 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
    Licensed under the MIT (Expat) license. ©
*/

#include <aros/libcall.h>

#include "saga_intern.h"

/*****************************************************************************

    NAME */
        AROS_LH1(VOID, ResetChip,
          AROS_LHA(struct BoardInfo *,(__arg1),A0), 

/*  SYNOPSIS */

/*  LOCATION */
        struct Library *, SAGABase, 56, Saga)

/*  FUNCTION

    INPUTS

    RESULT

    NOTES

    EXAMPLE

    BUGS

    SEE ALSO

    INTERNALS

    HISTORY

*****************************************************************************/
{
    AROS_LIBFUNC_INIT

    debug("");

    return;

    AROS_LIBFUNC_EXIT
}

