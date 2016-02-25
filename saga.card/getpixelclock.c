/*
    Copyright 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
    Licensed under the MIT (Expat) license. ©
*/

#include <aros/libcall.h>

#include "saga_intern.h"

/*****************************************************************************

    NAME */
        AROS_LH4(ULONG, GetPixelClock,

/*  SYNOPSIS */
        AROS_LHA(struct BoardInfo *, bi, A0),
        AROS_LHA(struct ModeInfo *, mi, A1),
        AROS_LHA(ULONG, index, D0),
        AROS_LHA(RGBFTYPE, RBGFormat, D7),

/*  LOCATION */
        struct Library *, SAGABase, 35, Saga)

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

