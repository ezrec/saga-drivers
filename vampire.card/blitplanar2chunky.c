/*
    Copyright 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
    Licensed under the MIT (Expat) license. ©
*/

#include <aros/libcall.h>

#include "vampire_intern.h"

/*****************************************************************************

    NAME */
        AROS_LH11(BOOL, BlitPlanar2Chunky,

/*  SYNOPSIS */
        AROS_LHA(struct BoardInfo *, bi, A0),
        AROS_LHA(struct BitMap *, bm, A1),
        AROS_LHA(struct RenderInfo *, ri, A2),
        AROS_LHA(UWORD, srcx, D0),
        AROS_LHA(UWORD, srcy, D1),
        AROS_LHA(UWORD, dstx, D2),
        AROS_LHA(UWORD, dsty, D3),
        AROS_LHA(UWORD, sizex, D4),
        AROS_LHA(UWORD, sizey, D5),
        AROS_LHA(UBYTE, minterm, D6),
        AROS_LHA(UBYTE, mask, D7),

/*  LOCATION */
        struct Library *, VampireBase, 45, Vampire)

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

    return FALSE;

    AROS_LIBFUNC_EXIT
}

