/*
    Copyright 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
    Licensed under the MIT (Expat) license. ©
*/

#include <aros/libcall.h>

#include <proto/exec.h>
#include <proto/alib.h>

#include "saga_intern.h"

#include "saga_private.h"

static void add_resolution(struct BoardInfo *bi, int id, UWORD width, UWORD height)
{
    struct Resolution *res;
    struct ModeInfo *mi;
    struct Library *SysBase = bi->ExecBase;
    const int refresh = 60; // FIXME: Refresh rate
    int i;

    // FIXME: Use bi->AllocMem
    res = AllocMem(sizeof(*res) + sizeof(struct ModeInfo) * MAXMODES, MEMF_ANY | MEMF_CLEAR);
    mi = (struct ModeInfo *)&res[1];

    res->DisplayID = 0x52000000 + id;
    res->BoardInfo = bi;
    res->Width = width;
    res->Height = height;
    res->Flags = P96F_PUBLIC;

    CopyMem("P96-0:", res->P96ID, 6);
    __sprintf(res->Name, "SAGA:%dx%d", width, height);

    res->Modes[0] = NULL;
    for (i = 1; i < MAXMODES; i++) {
        int depth[MAXMODES] = { 1, 8, 15, 24, 32 };

        res->Modes[i] = &mi[i];
        mi[i].Width = res->Width;
        mi[i].Height = res->Height;
        mi[i].Depth = depth[i];
        mi[i].Flags = 0;
        mi[i].HorTotal = res->Width;
        mi[i].HorBlankSize = 1;
        mi[i].HorSyncStart = 0;
        mi[i].HorSyncSize = 1;
        mi[i].HorSyncSkew = 0;
        mi[i].HorEnableSkew = 0;
        mi[i].VerTotal = res->Height;
        mi[i].VerBlankSize = 1;
        mi[i].VerSyncStart = 0;
        mi[i].VerSyncSize = 1;
        // FIXME: PixelClock
        mi[i].Numerator = 98;
        mi[i].Denomerator = 14;
        mi[i].PixelClock = res->Width * res->Height * refresh;
    }

    AddTail((struct List *)&bi->ResolutionsList, (struct Node *)res);
}


/*****************************************************************************

    NAME */
        AROS_LH1(BOOL, InitCard,

/*  SYNOPSIS */
        AROS_LHA(struct BoardInfo *, bi, A0),

/*  LOCATION */
        struct Library *, SAGABase, 6, Saga)

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

    int i;

    bi->BitsPerCannon = 8;
    bi->RGBFormats = RGBFF_CLUT |
                     RGBFF_R5G5B5PC |
                     RGBFF_R5G6B5PC |
                     RGBFF_R8G8B8 |
                     RGBFF_A8R8G8B8;
    bi->BoardType = 22; // One above 'Prototype5' in the Picasso96 type list
    bi->GraphicsControllerType = 0;     // 'Unknown'
    bi->PaletteChipType = 0;            // 'Unknown'
    bi->BoardName = "SAGA";

    for (i = 0; i < MAXMODES; i++) {
        bi->PixelClockCount[i] = 1;
        bi->MaxHorValue[i] = 0x4000;
        bi->MaxVerValue[i] = 0x4000;
    }

    bi->BoardFlags = (bi->BoardFlags & 0xffff0000) |
                        BIF_NOBLITTER |
                        BIF_NOMEMORYMODEMIX |
                        BIF_INDISPLAYCHAIN;

    /* For now, we only support 640x480 in all modes */
    for (i = 0; i < MAXMODES; i++) {
        bi->MaxHorResolution[i] = 640;
        bi->MaxVerResolution[i] = 480;
    }

    /* Create our resolutions */
    add_resolution(bi, 0, 640, 480);


#define BIC(name, entry) bi->name = AROS_SLIB_ENTRY(name, Saga, entry)
#define BID(name)        bi->name = bi->name##Default

    // BIC(AllocCardMem, 23);
    // BIC(FreeCardMem, 24);
    BIC(SetSwitch, 25);
    BIC(SetColorArray, 26);
    BIC(SetDAC, 27);
    BIC(SetGC, 28);
    BIC(SetPanning, 29);
    BIC(CalculateBytesPerRow, 30);
    BIC(CalculateMemory, 31);
    BIC(GetCompatibleFormats, 32);
    BIC(SetDisplay, 33);
    BIC(ResolvePixelClock, 34);
    BIC(GetPixelClock, 35);
    BIC(SetClock, 36);
    BIC(SetMemoryMode, 37);
    BIC(SetWriteMask, 38);
    BIC(SetClearMask, 39);
    BIC(SetReadPlane, 40);
    BIC(WaitVerticalSync, 41);
    BIC(SetInterrupt, 42);
    BIC(WaitBlitter, 43);

    // BIC(ScrollPlanar, 44);
    // BIC(UpdatePlanar, 45);

    if (0) {    /* No blitter support needed - our CPU is fast */
        BIC(BlitPlanar2Chunky, 46);
        BIC(FillRect, 47);
        BIC(InvertRect, 48);
        BIC(BlitRect, 49);
        BIC(BlitTemplate, 50);
        BIC(BlitPattern, 51);
        BIC(DrawLine, 52);
        BIC(BlitRectNoMaskComplete, 53);
        BIC(BlitPlanar2Direct, 54);
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
        BIC(SetDPMSLevel, 55);
        // BIC(ResetChip, 56);
        // BIC(GetfeatureAttrs, 57);
        // BIC(AllocBitMap, 58);
        // BIC(FeeBitMap, 59);
        BIC(SetSprite, 60);
        BIC(SetSpritePosition, 61);
        BIC(SetSpriteImage, 62);
        BIC(SetSpriteColor, 63);
        // BIC(CreateFeature, 64);
        // BIC(SetFeatureAttrs, 65);
        // BIC(DeleteFeature, 66);
    }

    return TRUE;

    AROS_LIBFUNC_EXIT
}

