/*
    Copyright 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
    Licensed under the MIT (Expat) license. ©
*/

#include <aros/libcall.h>

#include "saga_intern.h"

/*****************************************************************************

    NAME */
        AROS_UFH1(BOOL, SetClock,

/*  SYNOPSIS */
        AROS_UFHA(struct BoardInfo *, bi, A0))

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

    struct ModeInfo *mi = bi->ModeInfo;
    int clock_id = ((ULONG)mi->Numerator << 8) | mi->Denomerator;

    debug("Mode: %p", mi);
    debug("clock_id = %ld (%ld, %ld)", clock_id, mi->Numerator, mi->Denomerator);

    if (!SIMULATE)
        saga_pll_clock_program(clock_id);

    return 0;

    AROS_LIBFUNC_EXIT
}

