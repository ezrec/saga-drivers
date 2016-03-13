/*
    Copyright 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
    Licensed under the MIT (Expat) license. ©
*/

#include <aros/libcall.h>

#include "saga_intern.h"

/*****************************************************************************

    NAME */
        AROS_UFH2(ULONG, GetCompatibleFormats,

/*  SYNOPSIS */
        AROS_UFHA(struct BoardInfo *, bi, A0),
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

    debug("RGBFormat=%ld", RGBFormat);

    return RGBFF_CLUT |
           RGBFF_R8G8B8 |
           RGBFF_A8R8G8B8 |
           RGBFF_R5G6B5 |
           RGBFF_R5G5B5 |
           RGBFF_Y4U2V2;

    AROS_LIBFUNC_EXIT
}

