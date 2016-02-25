/*
    Copyright 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
    Licensed under the MIT (Expat) license. ©
*/

#include <aros/libcall.h>

#include "vampire_intern.h"

/*****************************************************************************

    NAME */
        AROS_LH6(BOOL, SetSpriteColor,

/*  SYNOPSIS */
        AROS_LHA(struct BoardInfo *, bi, A0),
        AROS_LHA(UBYTE, index, D0),
        AROS_LHA(UBYTE, r, D1),
        AROS_LHA(UBYTE, g, D2),
        AROS_LHA(UBYTE, b, D3),
        AROS_LHA(RGBFTYPE, RGBFormat, D7),

/*  LOCATION */
        struct Library *, VampireBase, 62, Vampire)

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

