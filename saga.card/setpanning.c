/*
    Copyright 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
    Licensed under the MIT (Expat) license. ©
*/

#include <aros/libcall.h>

#include "saga_intern.h"

/*****************************************************************************

    NAME */
        AROS_UFH6(BOOL, SetPanning,

/*  SYNOPSIS */
        AROS_UFHA(struct BoardInfo *, bi, A0),
        AROS_UFHA(UBYTE *, mem, A1),
        AROS_UFHA(UWORD, width, D0),
        AROS_UFHA(WORD, xoffset, D1),
        AROS_UFHA(WORD, yoffset, D2),
        AROS_UFHA(RGBFTYPE, RGBFormat, D7))

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

    int bpp = format2bpp(RGBFormat);

    Write32(SAGA_VIDEO_PLANEPTR, (IPTR)mem + (width * bpp) * yoffset + (xoffset * bpp));
             
    return FALSE;

    AROS_LIBFUNC_EXIT
}

