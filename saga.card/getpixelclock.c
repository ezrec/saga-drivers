/*
    Copyright 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
    Licensed under the MIT (Expat) license. ©
*/

#include <aros/libcall.h>

#include "saga_intern.h"

/*****************************************************************************

    NAME */
        AROS_UFH4(ULONG, GetPixelClock,

/*  SYNOPSIS */
        AROS_UFHA(struct BoardInfo *, bi, A0),
        AROS_UFHA(struct ModeInfo *, mi, A1),
        AROS_UFHA(ULONG, index, D0),
        AROS_UFHA(RGBFTYPE, RBGFormat, D7))

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
    ULONG freq = 0;

    saga_pll_clock_freq(index, is_NTSC, &freq);

    debug("index=%ld, pixelclock = %ld", index, freq);

    if (IS_DOUBLEX(mi->Width))
        freq /= 2;

    if (IS_DOUBLEY(mi->Height))
        freq /= 2;

    return freq;

    AROS_LIBFUNC_EXIT
}

