/*
    Copyright 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
    Licensed under the MIT (Expat) license. ©
*/

#include <aros/libcall.h>

#include "saga_intern.h"

/*****************************************************************************

    NAME */
        AROS_UFH3(BOOL, SetColorArray,

/*  SYNOPSIS */
        AROS_UFHA(struct BoardInfo *, bi, A0),
        AROS_UFHA(UWORD, start, D0),
        AROS_UFHA(UWORD, count, D1))

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

    struct SAGABase *SAGABase = (struct SAGABase *)bi->CardBase;
    int i, changed = 0;
    ULONG *clut = &SAGABase->sc_CLUT[0];
    struct CLUTEntry *ce = &bi->CLUT[start];

    debug("");

    for (i = 0; i < count; i++, ce++) {
        ULONG was, val;

        val = ((ULONG)ce->Red   << 16) |
              ((ULONG)ce->Green <<  8) |
              ((ULONG)ce->Blue  <<  0);
        was = clut[start + i];
        if (was != val) {
            changed++;
            clut[start + i] = val;
            Write32(SAGA_VIDEO_CLUT(start + i), val);
        }
    }

    return changed;

    AROS_LIBFUNC_EXIT
}

