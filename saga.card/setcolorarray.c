/*
    Copyright 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
    Licensed under the MIT (Expat) license. �
*/

#include <aros/libcall.h>

#include "saga_intern.h"

/*****************************************************************************

    NAME */
        AROS_LH3(BOOL, SetColorArray,

/*  SYNOPSIS */
        AROS_LHA(struct BoardInfo *, bi, A0),
        AROS_LHA(UWORD, start, D0),
        AROS_LHA(UWORD, count, D1),

/*  LOCATION */
        struct Library *, SAGABase, 26, Saga)

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

    int i, changed = 0;
    struct CLUTEntry *ce = &bi->CLUT[start];

    for (i = 0; i < count; i++, ce++) {
        ULONG was, val;

        val = ((ULONG)ce->Red   << 16) |
              ((ULONG)ce->Green <<  8) |
              ((ULONG)ce->Blue  <<  0);
        was = Read32(SAGA_VIDEO_CLUT(start + i));
        Write32(SAGA_VIDEO_CLUT(start + i), val);
        if (was != val)
            changed++;
    }

    return changed;

    AROS_LIBFUNC_EXIT
}

