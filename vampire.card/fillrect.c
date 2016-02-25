/*
    Copyright 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
    Licensed under the MIT (Expat) license. ©
*/

#include <aros/libcall.h>

#include "vampire_intern.h"

/*****************************************************************************

    NAME */
        AROS_LH9(BOOL, FillRect,

/*  SYNOPSIS */
        AROS_LHA(struct BoardInfo *, bi, A0),
        AROS_LHA(struct RenderInfo *, ri, A1),
        AROS_LHA(UWORD, x, D0),
        AROS_LHA(UWORD, y, D1),
        AROS_LHA(UWORD, width, D2),
        AROS_LHA(UWORD, height, D3),
        AROS_LHA(ULONG, pen, D4),
        AROS_LHA(UBYTE, mask, D5),
        AROS_LHA(RGBFTYPE, RGBFormat, D7),

/*  LOCATION */
        struct Library *, VampireBase, 46, Vampire)

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

