/*
    Copyright 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
    Licensed under the MIT (Expat) license. ©
*/

#include <string.h>

#include <proto/exec.h>
#include <proto/input.h>

#include <devices/inputevent.h>

#include <aros/libcall.h>

#include "saga_intern.h"

/*****************************************************************************

    NAME */
        AROS_LH1(BOOL, FindCard,

/*  SYNOPSIS */
        AROS_LHA(struct BoardInfo *, bi, A0),

/*  LOCATION */
        struct SAGABase *, SAGABase, 5, Saga)

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

    debug("");

    // If sc_BoardInfo is set, it means that the driver was already loaded,
    // and has a BoardInfo attached to it. Don't load again!
    if (SAGABase->sc_BoardInfo) {
        debug("Cowardly refusing to load twice");
        return FALSE;
    }

    // If the user is holding down SHIFT, then don't load.
    if (1) {
        struct Library *SysBase = SAGABase->sc_ExecBase;
        struct IOStdReq io;

        if (0 == OpenDevice("input.device", NULL, &io, NULL)) {
            struct Library *InputBase = (struct Library *)io.io_Device;
            UWORD qual;

            qual = PeekQualifier();
            CloseDevice(&io);

            if (qual & (IEQUALIFIER_LSHIFT | IEQUALIFIER_RSHIFT)) {
                debug("Cancelled by SHIFT");
                return FALSE;
            }
        }
    }


    // Set up SAGABase
    SAGABase->sc_BoardInfo = bi;

    memset(&SAGABase->sc_CLUT[0], 0, sizeof(SAGABase->sc_CLUT));

    bi->MemoryBase = (APTR)(IPTR)SAGA_VIDEO_MEMBASE;
    bi->MemorySize = SAGA_VIDEO_MEMSIZE;
 
    return TRUE;

    AROS_LIBFUNC_EXIT
}

