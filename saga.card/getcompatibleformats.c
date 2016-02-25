/*
    Copyright 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
    Licensed under the MIT (Expat) license. ©
*/

#include <aros/libcall.h>

#include "saga_intern.h"

/*****************************************************************************

    NAME */
        AROS_LH2(ULONG, GetCompatibleFormats,

/*  SYNOPSIS */
        AROS_LHA(struct BoardInfo *, bi, A0),
        AROS_LHA(RGBFTYPE, RGBFormat, D7),

/*  LOCATION */
        struct Library *, SAGABase, 32, Saga)

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

