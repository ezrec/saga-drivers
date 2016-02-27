/*
    Copyright 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
    Licensed under the MIT (Expat) license. ©
*/

#include <string.h>

#include <aros/libcall.h>

#include "saga_intern.h"

/*****************************************************************************

    NAME */
        AROS_LH1(BOOL, FindCard,

/*  SYNOPSIS */
        AROS_LHA(struct BoardInfo *, bi, A0),

/*  LOCATION */
        struct SAGABase *, SAGABase, 5, Saga)

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

    SAGABase->sc_PlanePtr = (APTR)(IPTR)SAGA_VIDEO_MEMBASE;
    memset(&SAGABase->sc_CLUT[0], 0, sizeof(SAGABase->sc_CLUT));

    bi->MemoryBase = SAGABase->sc_PlanePtr;
    // FIXME: This should be the largest size of video memory
    bi->MemorySize = 2*1024*1024;

    return TRUE;

    AROS_LIBFUNC_EXIT
}

