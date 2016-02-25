/*
    Copyright 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
    Licensed under the MIT (Expat) license. ©
*/

#include <aros/libcall.h>

#include "saga_intern.h"

/*****************************************************************************

    NAME */
        AROS_LH6(BOOL, SetPanning,

/*  SYNOPSIS */
        AROS_LHA(struct BoardInfo *, bi, A0),
        AROS_LHA(UBYTE *, mem, A1),
        AROS_LHA(UWORD, width, D0),
        AROS_LHA(WORD, xoffset, D1),
        AROS_LHA(WORD, yoffset, D2),
        AROS_LHA(RGBFTYPE, RGBFormat, D7),

/*  LOCATION */
        struct Library *, SAGABase, 29, Saga)

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

    int bpp = format2bpp(RGBFormat);

    Write32(SAGA_VIDEO_PLANEPTR, (IPTR)mem + (width * bpp) * yoffset + (xoffset * bpp));
             
    return FALSE;

    AROS_LIBFUNC_EXIT
}

