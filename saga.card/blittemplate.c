/*
    Copyright 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
    Licensed under the MIT (Expat) license. ©
*/

#include <aros/libcall.h>

#include "saga_intern.h"

/*****************************************************************************

    NAME */
        AROS_UFH9(BOOL, BlitTemplate,

/*  SYNOPSIS */
        AROS_UFHA(struct BoardInfo *, bi, A0),
        AROS_UFHA(struct RenderInfo *, ri, A1),
        AROS_UFHA(struct Template *, templ, A2),
        AROS_UFHA(UWORD, x, D0),
        AROS_UFHA(UWORD, y, D1),
        AROS_UFHA(UWORD, width, D2),
        AROS_UFHA(UWORD, height, D3),
        AROS_UFHA(UBYTE, mask, D4),
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

    return FALSE;

    AROS_LIBFUNC_EXIT
}

