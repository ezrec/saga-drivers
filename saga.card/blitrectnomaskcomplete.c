/*
    Copyright 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
    Licensed under the MIT (Expat) license. ©
*/

#include <aros/libcall.h>

#include "saga_intern.h"

/*****************************************************************************

    NAME */
        AROS_LH11(BOOL, BlitRectNoMaskComplete,

/*  SYNOPSIS */
        AROS_LHA(struct BoardInfo *, bi, A0),
        AROS_LHA(struct RenderInfo *, src, A1),
        AROS_LHA(struct RenderInfo *, dst, A2),
        AROS_LHA(UWORD, srcx, D0),
        AROS_LHA(UWORD, srcy, D1),
        AROS_LHA(UWORD, dstx, D2),
        AROS_LHA(UWORD, dsty, D3),
        AROS_LHA(UWORD, width, D4),
        AROS_LHA(UWORD, height, D5),
        AROS_LHA(UBYTE, opcode, D6),
        AROS_LHA(RGBFTYPE, RGBFormat, D7),

/*  LOCATION */
        struct Library *, SAGABase, 52, SAGA)

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

