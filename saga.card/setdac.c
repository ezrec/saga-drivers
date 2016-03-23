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

    struct SAGABase *sc = (struct SAGABase *)bi->CardBase;
    UBYTE format;

    if (RGBFormat == RGBFB_CLUT) {
        format = SAGA_VIDEO_FORMAT_CLUT8;
    } else if (RGBFormat == RGBFB_R5G5B5) {
        format = SAGA_VIDEO_FORMAT_RGB15;
    } else if (RGBFormat == RGBFB_R5G6B5) {
        format = SAGA_VIDEO_FORMAT_RGB16;
    } else if (RGBFormat == RGBFB_R8G8B8) {
        format = SAGA_VIDEO_FORMAT_RGB24;
    } else if (RGBFormat == RGBFB_A8R8G8B8) {
        format = SAGA_VIDEO_FORMAT_RGB32;
    } else if (RGBFormat == RGBFB_Y4U2V2) {
        format = SAGA_VIDEO_FORMAT_YUV422;
    } else
        return FALSE;

    debug("SAGA: %ld => format: %ld", (long)RGBFormat, (long)format);

    sc->sc_Format = format;

    Write16(SAGA_VIDEO_MODE, SAGA_VIDEO_MODE_FORMAT(sc->sc_Format) |
                             SAGA_VIDEO_MODE_DBLSCN(sc->sc_DoubleScan));

    return TRUE;

    AROS_LIBFUNC_EXIT
}

