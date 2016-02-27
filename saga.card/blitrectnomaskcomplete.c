/*
    Copyright 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
    Licensed under the MIT (Expat) license. ©
*/

#include <aros/libcall.h>

#include "saga_intern.h"

/*****************************************************************************

    NAME */
        AROS_UFH11(BOOL, BlitRectNoMaskComplete,

/*  SYNOPSIS */
        AROS_UFHA(struct BoardInfo *, bi, A0),
        AROS_UFHA(struct RenderInfo *, src, A1),
        AROS_UFHA(struct RenderInfo *, dst, A2),
        AROS_UFHA(UWORD, srcx, D0),
        AROS_UFHA(UWORD, srcy, D1),
        AROS_UFHA(UWORD, dstx, D2),
        AROS_UFHA(UWORD, dsty, D3),
        AROS_UFHA(UWORD, width, D4),
        AROS_UFHA(UWORD, height, D5),
        AROS_UFHA(UBYTE, opcode, D6),
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

