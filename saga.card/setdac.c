/*
    Copyright 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
    Licensed under the MIT (Expat) license. ©
*/

#include <aros/libcall.h>

#include "saga_intern.h"

/*****************************************************************************

    NAME */
        AROS_UFH2(BOOL, SetDAC,

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

    UWORD format;

    debug("");

    switch (RGBFormat) {
    case RGBFB_CLUT: format = SAGA_VIDEO_FORMAT_CLUT8; break;
    case RGBFB_R5G5B5: format = SAGA_VIDEO_FORMAT_RGB15; break;
    case RGBFB_R5G6B5: format = SAGA_VIDEO_FORMAT_RGB16; break;
    case RGBFB_R8G8B8: format = SAGA_VIDEO_FORMAT_RGB24; break;
    case RGBFB_A8R8G8B8: format = SAGA_VIDEO_FORMAT_RGB32; break;
    default:
        return FALSE;
    }

    Write16(SAGA_VIDEO_MODE, SAGA_VIDEO_MODE_FORMAT(format) |
                             SAGA_VIDEO_MODE_DBLSCN(SAGA_VIDEO_DBLSCAN_OFF));

    return TRUE;

    AROS_LIBFUNC_EXIT
}

