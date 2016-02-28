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

    if ((1 << RGBFormat) & RGBMASK_8BIT) {
        format = SAGA_VIDEO_FORMAT_CLUT8;
    } else if ((1 << RGBFormat) & RGBMASK_15BIT) {
        format = SAGA_VIDEO_FORMAT_RGB15;
    } else if ((1 << RGBFormat) & RGBMASK_16BIT) {
        format = SAGA_VIDEO_FORMAT_RGB16;
    } else if ((1 << RGBFormat) & RGBMASK_24BIT) {
        format = SAGA_VIDEO_FORMAT_RGB24;
    } else if ((1 << RGBFormat) & RGBMASK_32BIT) {
        format = SAGA_VIDEO_FORMAT_RGB32;
    } else
        return FALSE;

    Write16(SAGA_VIDEO_MODE, SAGA_VIDEO_MODE_FORMAT(format) |
                             SAGA_VIDEO_MODE_DBLSCN(SAGA_VIDEO_DBLSCAN_OFF));

    return TRUE;

    AROS_LIBFUNC_EXIT
}

