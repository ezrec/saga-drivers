/*
    Copyright 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
    Licensed under the MIT (Expat) license. ©
*/

#include  <string.h>

#include <aros/libcall.h>

#include <exec/execbase.h>

#include <proto/exec.h>

#include "saga_intern.h"

#include "saga_private.h"

static void add_resolution(struct BoardInfo *bi, ULONG displayid, const struct ModeInfo *tmpl)
{
    struct ExecBase *SysBase = (struct ExecBase *)bi->ExecBase;
    BOOL is_NTSC = SysBase->PowerSupplyFrequency == 60;
    struct Resolution *res;
    struct ModeInfo *mi;
    int i;
    TEXT *cp32;

    // FIXME: Use bi->AllocMem
    res = AllocMem(sizeof(*res) + (sizeof(struct ModeInfo) + 32) * (MAXMODES-1), MEMF_ANY | MEMF_CLEAR);
    mi = (struct ModeInfo *)&res[1];
    cp32 = (TEXT *)&mi[MAXMODES-1];

    res->DisplayID = displayid;
    res->BoardInfo = bi;
    res->Width = tmpl->Width;
    res->Height = tmpl->Height;
    res->Flags = P96F_PUBLIC;

    CopyMem("P96-0:", res->P96ID, 6);
    res->Node.ln_Name = res->P96ID;
    CopyMem(tmpl->Node.ln_Name, res->Name, strlen(tmpl->Node.ln_Name) + 1);

    res->Modes[0] = NULL;
    for (i = 1; i < MAXMODES; i++, cp32 += 32) {
        int depth[MAXMODES] = { 1, 8, 16, 24, 32 };
        int refresh;
        int clock_id;

        /* Initialize the template */
        CopyMem(tmpl, &mi[i], sizeof(*tmpl));

        CopyMem(tmpl->Node.ln_Name, cp32, strlen(tmpl->Node.ln_Name) + 1);
        mi[i].Node.ln_Name = cp32;
        mi[i].OpenCount = 0;
        mi[i].Active = TRUE;
        mi[i].Depth = depth[i];
        mi[i].Flags = tmpl->Flags;

        refresh = mi[i].Numerator;
        if (!refresh)
            refresh = 60;

        mi[i].PixelClock = tmpl->HorTotal * tmpl->VerTotal * refresh;
        if (mi[i].Flags & GMF_DOUBLESCAN)
            mi[i].PixelClock *= 2;
        if (mi[i].Flags & GMF_DOUBLECLOCK)
            mi[i].PixelClock *= 2;

        /* Fix up PixelClock to a 'sane' value */
        clock_id = saga_pll_clock_lookup(is_NTSC, &mi[i].PixelClock);
        mi[i].Numerator = (clock_id >> 8) & 0xff;
        mi[i].Denomerator = (clock_id >> 0) & 0xff;

        res->Modes[i] = &mi[i];
    }

    AddTail((struct List *)&bi->ResolutionsList, (struct Node *)res);
}

/*****************************************************************************

    NAME */
        AROS_LH1(BOOL, InitCard,

/*  SYNOPSIS */
        AROS_LHA(struct BoardInfo *, bi, A0),

/*  LOCATION */
        struct SAGABase *, SAGABase, 6, Saga)

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

    int i, clocks;
    const ULONG displayid_base = 0x52001000;

    debug("bi: %p", bi);
    dump_BoardInfo(bi);

    bi->BitsPerCannon = 8;
    bi->RGBFormats = RGBFF_CLUT |
                     RGBFF_Y4U2V2 |
                     RGBFF_R5G5B5 |
                     RGBFF_R5G6B5 |
                     RGBFF_R8G8B8 |
                     RGBFF_A8R8G8B8;
    bi->BoardType = 1;                  // 'Unknown'
    bi->GraphicsControllerType = 0;     // 'Unknown'
    bi->PaletteChipType = 0;            // 'Unknown'
    bi->BoardName = "SAGA";

    clocks = saga_pll_clock_count();

    for (i = 0; i < MAXMODES; i++) {
        bi->PixelClockCount[i] = clocks;
        bi->MaxHorValue[i] = 0x4000;
        bi->MaxVerValue[i] = 0x4000;
    }

    bi->BoardFlags = (bi->BoardFlags & 0xffff0000) |
                        BIF_NOBLITTER |
                        BIF_NOMEMORYMODEMIX;

    for (i = 0; i < MAXMODES; i++) {
        bi->MaxHorResolution[i] = 4000;
        bi->MaxVerResolution[i] = 4000;
    }

    /* Add VESA modelines */
    for (i = 0; i < modeline_vesa_entries; i++) {
        add_resolution(bi, displayid_base | (0x10000 * i), &modeline_vesa_entry[i]);
    }

#define BIC(name) bi->name = name;
#define BID(name) bi->name = bi->name##Default

    // BIC(AllocCardMem);
    // BIC(FreeCardMem);
    BIC(SetSwitch);
    BIC(SetColorArray);
    BIC(SetDAC);
    BIC(SetGC);
    BIC(SetPanning);
    BIC(CalculateBytesPerRow);
    BIC(CalculateMemory);
    BIC(GetCompatibleFormats);
    BIC(SetDisplay);
    BIC(ResolvePixelClock);
    BIC(GetPixelClock);
    BIC(SetClock);
    BIC(SetMemoryMode);
    BIC(SetWriteMask);
    BIC(SetClearMask);
    BIC(SetReadPlane);
    BIC(WaitVerticalSync);
    BIC(SetInterrupt);
    BIC(WaitBlitter);

    BIC(ScrollPlanar);
    // BIC(UpdatePlanar);

    if (0) {    /* No blitter support needed - our CPU is fast */
        BIC(BlitPlanar2Chunky);
        BIC(FillRect);
        BIC(InvertRect);
        BIC(BlitRect);
        BIC(BlitTemplate);
        BIC(BlitPattern);
        BIC(DrawLine);
        BIC(BlitRectNoMaskComplete);
        BIC(BlitPlanar2Direct);
    } else {
        /* Map to the default Picasso96 defaults */
        BID(BlitPlanar2Chunky);
        BID(FillRect);
        BID(InvertRect);
        BID(BlitRect);
        BID(BlitTemplate);
        BID(BlitPattern);
        BID(DrawLine);
        BID(BlitRectNoMaskComplete);
        BID(BlitPlanar2Direct);
    }

    /* Other optional features */
    if (0) {
        BIC(SetDPMSLevel);
    }

    BIC(ResetChip);

    /* Other optional features */
    if (0) {
        // BIC(GetfeatureAttrs);
        // BIC(AllocBitMap);
        // BIC(FeeBitMap);
        BIC(SetSprite);
        BIC(SetSpritePosition);
        BIC(SetSpriteImage);
        BIC(SetSpriteColor);
        // BIC(CreateFeature);
        // BIC(SetFeatureAttrs);
        // BIC(DeleteFeature);
    }

    debug("Card Initialized");
    debug("bi: %p", bi);
    dump_BoardInfo(bi);

    return TRUE;

    AROS_LIBFUNC_EXIT
}

