/*
    Copyright 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
    Licensed under the MIT (Expat) license. ©
*/

#include <aros/libcall.h>

#include "saga_intern.h"

#include "saga_private.h"

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

    bi->BitsPerCannon = 8;
    bi->RGBFormats = RGBFF_CLUT |
                     RGBFF_R5G5B5PC |
                     RGBFF_R5G6B5PC |
                     RGBFF_R8G8B8 |
                     RGBFF_A8R8G8B8;

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

    return FALSE;

    AROS_LIBFUNC_EXIT
}

