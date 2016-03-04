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

    struct ExecBase *SysBase = (struct ExecBase *)bi->ExecBase;
    BOOL is_NTSC = SysBase->PowerSupplyFrequency == 60;
    int clock_id;

    debug("Mode: %p", mi);
    debug("pixelclock in = %ld", pixelclock);

    mi->PixelClock = pixelclock;

    if (IS_DOUBLEY(mi->Height))
        pixelclock *= 2;

    clock_id = saga_pll_clock_lookup(is_NTSC, &pixelclock);

    mi->Numerator = clock_id >> 8;
    mi->Denomerator = clock_id & 0xff;

    debug("pixelclock out = %ld (%ld, %ld)", clock_id, mi->Numerator, mi->Denomerator);

    return clock_id;

    AROS_LIBFUNC_EXIT
}

