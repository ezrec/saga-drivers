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
    UWORD width, hsstrt, hsstop, htotal;
    UWORD height, vsstrt, vsstop, vtotal;
    UBYTE doublescan = 0;
    struct SAGABase *sc = (struct SAGABase *)bi->CardBase;

    debug("");

    // Picasso96 version 2.3 doesn't set bi->ModeInfo for us.
    bi->ModeInfo = mi;
    bi->Border = border;

    dump_ModeInfo(mi);

    /* Borders (mi->HorBlankSize and mi->VerBlankSize)
     * are not truely supported, since the border color
     * can not be set.
     */

    width = mi->Width;
    hsstrt = width + mi->HorBlankSize + mi->HorSyncStart;
    hsstop = hsstrt + mi->HorSyncSize;
    htotal = mi->HorTotal;

    height = mi->Height;
    vsstrt = height + mi->VerBlankSize + mi->VerSyncStart;
    vsstop = vsstrt + mi->VerSyncSize;
    vtotal = mi->VerTotal;

    if (IS_DOUBLEX(width)) {
        doublescan |= SAGA_VIDEO_DBLSCAN_X;
        hsstrt <<= 1;
        hsstop <<= 1;
        htotal <<= 1;
        width  <<= 1;
    }

    if (IS_DOUBLEY(height)) {
        doublescan |= SAGA_VIDEO_DBLSCAN_Y;
        vsstrt <<= 1;
        vsstop <<= 1;
        vtotal <<= 1;
        height <<= 1;

    }

    /* Monitor mode info */
    debug("ModeLine \"%ldx%ld\" %ld, %ld %ld %ld %ld, %ld %ld %ld %ld, %sHSync %sVSync%s%s",
            width, height, mi->PixelClock / 1000000,
            width, hsstrt, hsstop, htotal,
            height, vsstrt, vsstop, vtotal,
            (mi->Flags & GMF_HPOLARITY) ? "+" : "-",
            (mi->Flags & GMF_VPOLARITY) ? "+" : "-",
            (doublescan & SAGA_VIDEO_DBLSCAN_X) ? " DoubleScanX" : "",
            (doublescan & SAGA_VIDEO_DBLSCAN_Y) ? " DoubleScanY" : "");

    Write16(SAGA_VIDEO_HPIXEL, width);
    Write16(SAGA_VIDEO_HSSTRT, hsstrt);
    Write16(SAGA_VIDEO_HSSTOP, hsstop);
    Write16(SAGA_VIDEO_HTOTAL, htotal);

    Write16(SAGA_VIDEO_VPIXEL, height);
    Write16(SAGA_VIDEO_VSSTRT, vsstrt);
    Write16(SAGA_VIDEO_VSSTOP, vsstop);
    Write16(SAGA_VIDEO_VTOTAL, vtotal);

    Write16(SAGA_VIDEO_HVSYNC, ((mi->Flags & GMF_HPOLARITY) ? (1 << 0) : 0) |
                               ((mi->Flags & GMF_VPOLARITY) ? (1 << 1) : 0));

    sc->sc_DoubleScan = doublescan;

    Write16(SAGA_VIDEO_MODE, SAGA_VIDEO_MODE_FORMAT(sc->sc_Format) |
                             SAGA_VIDEO_MODE_DBLSCN(sc->sc_DoubleScan));

    return TRUE;

    AROS_LIBFUNC_EXIT
}

