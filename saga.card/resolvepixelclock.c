/*
    Copyright 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
    Licensed under the MIT (Expat) license. ©
*/

#include <aros/libcall.h>

#include "saga_intern.h"

/*****************************************************************************

    NAME */
        AROS_UFH4(ULONG, ResolvePixelClock,

/*  SYNOPSIS */
        AROS_UFHA(struct BoardInfo *, bi, A0),
        AROS_UFHA(struct ModeInfo *, mi, A1),
        AROS_UFHA(ULONG, pixelclock, D0),
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

    mi->PixelClock = pixelclock;
    mi->Numerator = 98;
    mi->Denomerator = 14;

    return 0;   // PixelClock Index

    AROS_LIBFUNC_EXIT
}

