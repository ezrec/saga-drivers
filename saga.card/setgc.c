/*
    Copyright 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
    Licensed under the MIT (Expat) license. ©
*/

#include <aros/libcall.h>

#include "saga_intern.h"

/*****************************************************************************

    NAME */
        AROS_UFH3(BOOL, SetGC,

/*  SYNOPSIS */
        AROS_UFHA(struct BoardInfo *, bi, A0),
        AROS_UFHA(struct ModeInfo *, mi, A1),
        AROS_UFHA(BOOL, border, D0))

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
    UWORD width, height;
    UBYTE doublescan = 0;
    struct SAGABase *sc = (struct SAGABase *)bi->CardBase;

    debug("");

    // Picasso96 version 2.3 doesn't set bi->ModeInfo for us.
    bi->ModeInfo = mi;
    bi->Border = border;

    dump_ModeInfo(mi);

    width = mi->Width;
    height = mi->Height;

    if (IS_DOUBLEY(height))
        doublescan |= SAGA_VIDEO_DBLSCAN_Y;

    if (IS_DOUBLEX(width))
        doublescan |= SAGA_VIDEO_DBLSCAN_X;

    Write16(SAGA_VIDEO_WIDTH, width);
    Write16(SAGA_VIDEO_HEIGHT, height);

    sc->sc_DoubleScan = doublescan;

    Write16(SAGA_VIDEO_MODE, SAGA_VIDEO_MODE_FORMAT(sc->sc_Format) |
                             SAGA_VIDEO_MODE_DBLSCN(sc->sc_DoubleScan));

    return TRUE;

    AROS_LIBFUNC_EXIT
}

