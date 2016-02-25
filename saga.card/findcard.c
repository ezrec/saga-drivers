/*
    Copyright 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
    Licensed under the MIT (Expat) license. �
*/

#include <aros/libcall.h>

#include "saga_intern.h"

/*****************************************************************************

    NAME */
        AROS_LH1(BOOL, FindCard,

/*  SYNOPSIS */
        AROS_LHA(struct BoardInfo *, bi, A0),

/*  LOCATION */
        struct Library *, SAGABase, 5, Saga)

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

    bi->MemoryBase = (APTR)(IPTR)Read32(SAGA_VIDEO_PLANEPTR); 
    // FIXME: This should be the largest size of memory
    bi->MemorySize = 2*1024*1024;

    return TRUE;

    AROS_LIBFUNC_EXIT
}

