/*
 * Copyright (C) 2016, Netronome Systems, Inc.
 * All rights reserved.
 *
 */

#ifndef SAGASD_INTERN_H
#define SAGASD_INTERN_H

#include <exec/libraries.h>
#include <exec/devices.h>

#include <proto/arossupport.h>

#if DEBUG
#define bug(x,args...)   kprintf(x ,##args)
#define debug(x,args...) bug("%s:%d " x "\n", __func__, __LINE__ ,##args)
#else
#define bug(x,args...)   do { } while (0)
#define debug(x,args...) do { } while (0)
#endif

#define SAGASD_UNITS    1       /* Only one chip select for now */

struct SAGASDBase {
    struct Device       sd_Device;
    struct Library *    sd_ExecBase;
    struct SAGASDUnit {
        struct Unit sdu_Unit;
        BOOL sdu_Enabled;
        IPTR sdu_IOBase;
        struct MsgPort *sdu_MsgPort;
        ULONG sdu_ChangeNum;
        struct {
            BOOL sdi_ReadOnly;
        } sdu_Identify;
    } sd_Unit[SAGASD_UNITS];
};

static inline UBYTE Read8(IPTR addr)
{
    return *(volatile UBYTE *)addr;
}

static inline VOID Write8(IPTR addr, ULONG value)
{
    bug("0x%06lx <= 0x%08lx\n", addr, value);
    if (!SIMULATE) {
        *(volatile UBYTE *)addr = value;
    }
}

static inline UWORD Read16(IPTR addr)
{
    return *(volatile UWORD *)addr;
}

static inline VOID Write16(IPTR addr, UWORD value)
{
    bug("0x%06lx <= 0x%04lx\n", addr, value);
    if (!SIMULATE) {
        *(volatile UWORD *)addr = value;
    }
}


#endif /* SAGASD_INTERN_H */
/* vim: set shiftwidth=4 expandtab:  */
