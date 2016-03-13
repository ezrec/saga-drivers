/*
 * Copyright (C) 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
 * All rights reserved.
 *
 * Licensed under the MIT License:
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef PICASSO_CARD_H
#define PICASSO_CARD_H

#include <exec/types.h>
#include <exec/memory.h>
#include <exec/interrupts.h>

#include <graphics/view.h>
#include <graphics/gfx.h>

#include <aros/types/timeval_s.h>

#ifndef __packed
#ifdef __GNUC__
#define __packed __attribute__((__packed__))
#else
#define __packed
#endif
#endif

/* Derived from WinUAE od-win32/picasso96_win.cpp and od-win32/picasso96_win.h
 *
 * Original UAE Picasso96 support:
 *
 * Copyright 1997 Brian King <Brian_King@Mitel.com, Brian_King@Cloanto.com>
 */
enum {
    PLANAR = 0,
    CHUNKY,
    HICLOLOR,
    TRUECOLOR,
    TRUEALPHA,
    MAXMODES
};

typedef enum {
    RGBFB_NONE = 0,
    RGBFB_CLUT = 1,
    RGBFB_R8G8B8 = 2,
    RGBFB_B8G8R8 = 3,
    RGBFB_R5G6B5PC = 4,
    RGBFB_R5G5B5PC = 5,
    RGBFB_A8R8G8B8 = 6,
    RGBFB_A8B8G8R8 = 7,
    RGBFB_R8G8B8A8 = 8,
    RGBFB_B8G8R8A8 = 9,
    RGBFB_R5G6B5 = 10,
    RGBFB_R5G5B5 = 11,
    RGBFB_B5G6R5PC = 12,
    RGBFB_B5G5R5PC = 13,
    RGBFB_Y4U2V2 = 14,
    RGBFB_Y4U1V1 = 15,
    RGBFB_MaxFormats = 16
} RGBFTYPE;


#define RGBFF_NONE      (1<<RGBFB_NONE)
#define RGBFF_CLUT      (1<<RGBFB_CLUT)
#define RGBFF_R8G8B8    (1<<RGBFB_R8G8B8)
#define RGBFF_B8G8R8    (1<<RGBFB_B8G8R8)
#define RGBFF_R5G6B5PC  (1<<RGBFB_R5G6B5PC)
#define RGBFF_R5G5B5PC  (1<<RGBFB_R5G5B5PC)
#define RGBFF_A8R8G8B8  (1<<RGBFB_A8R8G8B8)
#define RGBFF_A8B8G8R8  (1<<RGBFB_A8B8G8R8)
#define RGBFF_R8G8B8A8  (1<<RGBFB_R8G8B8A8)
#define RGBFF_B8G8R8A8  (1<<RGBFB_B8G8R8A8)
#define RGBFF_R5G6B5    (1<<RGBFB_R5G6B5)
#define RGBFF_R5G5B5    (1<<RGBFB_R5G5B5)
#define RGBFF_B5G6R5PC  (1<<RGBFB_B5G6R5PC)
#define RGBFF_B5G5R5PC  (1<<RGBFB_B5G5R5PC)
#define RGBFF_Y4U2V2    (1<<RGBFB_Y4U2V2)
#define RGBFF_Y4U1V1    (1<<RGBFB_Y4U1V1)

#define RGBMASK_8BIT RGBFF_CLUT
#define RGBMASK_16BIT (RGBFF_R5G6B5PC | RGBFF_B5G6R5PC | RGBFF_R5G6B5)
#define RGBMASK_15BIT (RGBFF_R5G5B5PC | RGBFF_B5G5R5PC | RGBFF_R5G5B5)
#define RGBMASK_24BIT (RGBFF_R8G8B8 | RGBFF_B8G8R8)
#define RGBMASK_32BIT (RGBFF_A8R8G8B8 | RGBFF_A8B8G8R8 | RGBFF_R8G8B8A8 | RGBFF_B8G8R8A8)

#define RGBFF_PLANAR    RGBFF_NONE
#define RGBFF_CHUNKY    RGBFF_CLUT

#define RGBFB_PLANAR    RGBFB_NONE
#define RGBFB_CHUNKY    RGBFB_CLUT

struct CLUTEntry {
    UBYTE Red;
    UBYTE Green;
    UBYTE Blue;
} __packed;

struct RenderInfo {
    APTR        Memory;
    UWORD       BytesPerRow;
    UWORD       _pad;
    RGBFTYPE    RGBFormat;
};

struct Pattern {
    APTR        Memory;
    UWORD       XOffset;
    UWORD       YOffset;
    ULONG       FgPen;
    ULONG       BgPen;
    UBYTE       Size;           /* Width = 16, Height = (1 << Size) */
    UBYTE       DrawMode;
};

struct Template {
    APTR        Memory;
    UWORD       BytesPerRow;
    UBYTE       XOffset;
    UBYTE       YOffset;
    ULONG       FgPen;
    ULONG       BgPen;
};

struct BoardInfo;

struct BitMapExtra {
    struct MinNode      BoardNode;
    APTR                HashChain;
    ULONG               Match;
    APTR                BitMap;
    struct BoardInfo *  BoardInfo;
    struct MemChunk *   MemChunk;
    struct RenderInfo   RenderInfo;
    UWORD               Width;
    UWORD               Height;
    UWORD               Flags;
    UWORD               BaseLevel;
    UWORD               CurrentLevel;
    ULONG               CompanionMaster;
} __packed;

#define P96B_FAMILY     0
#define P96B_PUBLIC     1
#define P96B_MONITOOL   2

#define P96F_FAMILY     (1 << P96B_FAMILY)
#define P96F_PUBLIC     (1 << P96B_PUBLIC)      /* Default */
#define P96F_MONITOOL   (1 << P96B_MONITOOL)


struct ModeInfo {
    struct Node Node;
    UWORD       OpenCount;
    BOOL        Active;
    UWORD       Width;
    UWORD       Height;
    UBYTE       Depth;
    UBYTE       Flags;  /* See GM* flags below */
    UWORD       HorTotal;
    UWORD       HorBlankSize;
    UWORD       HorSyncStart;
    UWORD       HorSyncSize;
    UBYTE       HorSyncSkew;
    UBYTE       HorEnableSkew;
    UWORD       VerTotal;
    UWORD       VerBlankSize;
    UWORD       VerSyncStart;
    UWORD       VerSyncSize;
    UBYTE       Numerator;
    UBYTE       Denomerator;
    ULONG       PixelClock;
} __packed;

#define GMB_DOUBLECLOCK         0       /* Clock is doubled after selection */
#define GMB_INTERLACE           1       /* Mode is interlaced */
#define GMB_DOUBLESCAN          2       /* Scanlines are doubled */
#define GMB_HPOLARITY           3       /* Horizontal Polarity */
#define GMB_VPOLARITY           4       /* Vertical Polarity */
#define GMB_COMPATVIDEO         5       /* Compatible Video */
#define GMB_DOUBLEVERTICAL      6       /* Doubled Vertical */

#define GMF_DOUBLECLOCK         (1UL << GMB_DOUBLECLOCK)
#define GMF_INTERLACE           (1UL << GMB_INTERLACE)
#define GMF_DOUBLESCAN          (1UL << GMB_DOUBLESCAN)
#define GMF_HPOLARITY           (1UL << GMB_HPOLARITY)
#define GMF_VPOLARITY           (1UL << GMB_VPOLARITY)
#define GMF_COMPATVIDEO         (1UL << GMB_COMPATVIDEO)
#define GMF_DOUBLEVERTICAL      (1UL << GMB_DOUBLEVERTICAL)


#define MAXRESOLUTIONNAMELENGTH 22

struct Resolution {
    struct Node Node;
    TEXT        P96ID[6];
    TEXT        Name[MAXRESOLUTIONNAMELENGTH];
    ULONG       DisplayID;
    UWORD       Width;
    UWORD       Height;
    UWORD       Flags;
    struct ModeInfo *Modes[MAXMODES];
    struct BoardInfo *BoardInfo;
};

#define BIB_HARDWARESPRITE      0
#define BIB_NOMEMORYMODEMIX     1
#define BIB_NEEDSALIGNMENT      2
#define BIB_DBLSCANDBLSPRITEY   8
#define BIB_ILACEHALFSPRITEY    9
#define BIB_ILACEDBLROWOFFSET   10
#define BIB_FLICKERFIXER        12
#define BIB_VIDEOCAPTURE        13
#define BIB_VIDEOWINDOW         14
#define BIB_BLITTER             15
#define BIB_HIRESSPRITE         16
#define BIB_BIGSPRITE           17
#define BIB_BORDEROVERRIDE      18
#define BIB_BORDERBLANK         19
#define BIB_INDISPLAYCHAIN      20
#define BIB_QUIET               21
#define BIB_NOMASKBLITS         22
#define BIB_NOC2PBLITS          23
#define BIB_NOBLITTER           24

#define BIF_HARDWARESPRITE      (1 << BIB_HARDWARESPRITE)
#define BIF_NOMEMORYMODEMIX     (1 << BIB_NOMEMORYMODEMIX)
#define BIF_NEEDSALIGNMENT      (1 << BIB_NEEDSALIGNMENT)
#define BIF_DBLSCANDBLSPRITEY   (1 << BIB_DBLSCANDBLSPRITEY)
#define BIF_ILACEHALFSPRITEY    (1 << BIB_ILACEHALFSPRITEY)
#define BIF_ILACEDBLROWOFFSET   (1 << BIB_ILACEDBLROWOFFSET)
#define BIF_FLICKERFIXER        (1 << BIB_FLICKERFIXER)
#define BIF_VIDEOCAPTURE        (1 << BIB_VIDEOCAPTURE)
#define BIF_VIDEOWINDOW         (1 << BIB_VIDEOWINDOW)
#define BIF_BLITTER             (1 << BIB_BLITTER)
#define BIF_HIRESSPRITE         (1 << BIB_HIRESSPRITE)
#define BIF_BIGSPRITE           (1 << BIB_BIGSPRITE)
#define BIF_BORDEROVERRIDE      (1 << BIB_BORDEROVERRIDE)
#define BIF_BORDERBLANK         (1 << BIB_BORDERBLANK)
#define BIF_INDISPLAYCHAIN      (1 << BIB_INDISPLAYCHAIN)
#define BIF_QUIET               (1 << BIB_QUIET)
#define BIF_NOMASKBLITS         (1 << BIB_NOMASKBLITS)
#define BIF_NOC2PBLITS          (1 << BIB_NOC2PBLITS)
#define BIF_NOBLITTER           (1 << BIB_NOBLITTER)

struct BoardInfo {
    APTR                RegisterBase;
    APTR                MemoryBase;
    APTR                MemoryIOBase;
    ULONG               MemorySize;
    CONST_STRPTR        BoardName;
    TEXT                VBIName[32];
    struct Library *    CardBase;
    struct Library *    ChipBase;
    struct Library *    ExecBase;
    struct Library *    UtilBase;
    struct Interrupt    HardInterrupt;
    struct Interrupt    SoftInterrupt;
    BYTE                BoardLock[46];
    struct MinList      ResolutionsList;
    ULONG               BoardType;
    ULONG               PaletteChipType;
    ULONG               GraphicsControllerType;
    UWORD               MoniSwitch;
    UWORD               BitsPerCannon;
    ULONG               BoardFlags;
    UWORD               SoftSpriteFlags;
    UWORD               ChipFlags;
    ULONG               CardFlags;
    UWORD               BoardNum;
    UWORD               RGBFormats;
    UWORD               MaxHorValue[MAXMODES];
    UWORD               MaxVerValue[MAXMODES];
    UWORD               MaxHorResolution[MAXMODES];
    UWORD               MaxVerResolution[MAXMODES];
    ULONG               MaxMemorySize;
    ULONG               MaxChunkSize;
    ULONG               MemoryClock;
    ULONG               PixelClockCount[MAXMODES];

    APTR                AllocCardMem;
    APTR                FreeCardMem;
    /* R: BOOL SetSwitch(struct BoardInfo *bi, BOOL new_state) (A0, D0) */
    APTR                SetSwitch;
    /* R: BOOL SetColorArray(struct BoardInfo *bi, UWORD start, UWORD count) (A0, D0, D1) */
    APTR                SetColorArray;
    /* R: BOOL SetDAC(struct BoardInfo *bi, RGBFTYPE RGBFormat) (A0, D7) */
    APTR                SetDAC;
    /* R: BOOL SetGC(struct BoardInfo *bi, struct ModeInfo *mi, BOOL border) (A0, A1, D0) */
    APTR                SetGC;
    /* R: BOOL SetPanning(struct BoardInfo *bi, UBYTE *mem, UWORD width, WORD xoffset, WORD yoffset, RGBFTYPE RGBFormat) (A0, A1, D0, D1, D2, D7) */
    APTR                SetPanning;
    /* R: ULONG CalculateBytesPerRow(struct BoardInfo *bi, UWORD width, RGBFTYPE RGBFormat) (A0, D0, D7) */
    APTR                CalculateBytesPerRow;
    /* R: APTR CalculateMemory(struct BoardInfo *bi, APTR LogicalMemory, RGBFTYPE RGBFormat) (A0, A1, D7) */
    APTR                CalculateMemory;
    /* R: ULONG GetCompatibleFormats(struct BoardInfo *bi, RGBFTYPE RGBFormat) (A0, D7) */
    APTR                GetCompatibleFormats;
    /* R: BOOL SetDisplay(struct BoardInfo *bi, BOOL enabled) (A0, D0) */
    APTR                SetDisplay;
    /* R: ULONG ResolvePixelClock(struct BoardInfo *bi, struct ModeInfo *mi, ULONG pixelclock, RGBFTYPE RGBFormat) (A0, A1, D0, D7) */
    APTR                ResolvePixelClock;
    /* R: ULONG GetPixelClock(struct BoardInfo *bi, struct ModeInfo *mi, ULONG index, RGBFTYPE RBGFormat) (A0, A1, D0, D7) */
    APTR                GetPixelClock;
    /* R: BOOL SetClock(struct BoardInfo *bi) (A0) */
    APTR                SetClock;
    /* R: BOOL SetMemoryMode(struct BoardInfo *bi, RGBFTYPE RGBMode) (A0, D7) */
    APTR                SetMemoryMode;
    /* R: BOOL SetWriteMask(struct BoardInfo *bi, UBYTE mask) (A0, D0) */
    APTR                SetWriteMask;
    /* R: BOOL SetClearMask(struct BoardInfo *bi, UBYTE mask) (A0, D0) */
    APTR                SetClearMask;
    /* R: BOOL SetReadPlane(struct BoardInfo *bi, UBYTE plane) (A0, D0) */
    APTR                SetReadPlane;
    /* R: VOID WaitVerticalSync(struct BoardInfo *bi, BOOL end) (A0, D0) */
    APTR                WaitVerticalSync;
    /* R: VOID SetInterrupt(struct BoardInfo *bi, BOOL enabled) (A0, D0) */
    APTR                SetInterrupt;
    /* R: VOID WaitBlitter(struct BoardInfo *bi) (A0) */
    APTR                WaitBlitter;
    APTR                ScrollPlanar;
    APTR                ScrollPlanarDefault;
    APTR                UpdatePlanar;
    APTR                UpdatePlanarDefault;
    /* O: BOOL BlitPlanar2Chunky(struct BoardInfo *bi, struct BitMap *bm, struct RenderInfo *ri, UWORD srcx, UWORD srcy, UWORD dstx, UWORD dsty, UWORD sizex, UWORD sizey, UBYTE minterm, UBYTE mask) (A0, A1, A2, D0, D1, D2, D3, D4, D5, D6, D7) */
    APTR                BlitPlanar2Chunky;
    APTR                BlitPlanar2ChunkyDefault;
    /* O: BOOL FillRect(struct BoardInfo *bi, struct RenderInfo *ri, UWORD x, UWORD y, UWORD width, UWORD height, ULONG pen, UBYTE mask, RGBFTYPE RGBFormat) (A0, A1, D0, D1, D2, D3, D4, D5, D7) */
    APTR                FillRect;
    APTR                FillRectDefault;
    /* O: BOOL InvertRect(struct BoardInfo *bi, struct RenderInfo *ri, UWORD x, UWORD y, UWORD width, UWORD height, UBYTE mask, RGBFTYPE RGBFormat) (A0, A1, D0, D1, D2, D3, D4, D7) */
    APTR                InvertRect;
    APTR                InvertRectDefault;
    /* O: BOOL BlitRect(struct BoardInfo *bi, struct RenderInfo *ri, UWORD srcx, UWORD srcy, UWORD dstx, UWORD dsty, UWORD height, UBYTE mask, RGBFTYPE RGBFormat) (A0, A1, D0, D1, D2, D3, D4, D5, D6, D7) */
    APTR                BlitRect;
    APTR                BlitRectDefault;
    /* O: BOOL BlitTemplate(struct BoardInfo *bi, struct RenderInfo *ri, struct Template *templ, UWORD x, UWORD y, UWORD width, UWORD height, UBYTE mask, RGBFTYPE RGBFormat) (A0, A1, A2, D0, D1, D2, D3, D4, D7) */
    APTR                BlitTemplate;
    APTR                BlitTemplateDefault;
    /* O: BOOL BlitPattern(struct BoardInfo *bi, struct RenderInfo *ri, struct Pattern *pat, UWORD x, UWORD y, UWORD width, UWORD height, UBYTE mask, RGBFTYPE RGBFormat) (A0, A1, A2, D0, D1, D2, D3, D4, D7) */
    APTR                BlitPattern;
    APTR                BlitPatternDefault;
    /* O: BOOL DrawLine(struct BoardInfo *bi, struct RenderInfo *ri, struct Line *line, UBYTE mask, RGBFTYPE RGBFormat) (A0, A1, A2, D0, D7) */
    APTR                DrawLine;
    APTR                DrawLineDefault;
    /* O: BOOL BlitRectNoMaskComplete(struct BoardInfo *bi, struct RenderInfo *src, struct RenderInfo *dst, UWORD srcx, UWORD srcy, UWORD dstx, UWORD dsty, UWORD height, UBYTE opcode, RGBFTYPE RGBFormat) (A0, A1, A2, D0, D1, D2, D3, D4, D5, D6, D7) */
    APTR                BlitRectNoMaskComplete;
    APTR                BlitRectNoMaskCompleteDefault;
    /* O: BOOL BlitPlanar2Direct(struct BoardInfo *bi, struct BitMap *bm, struct RenderInfo *ri, struct ColorIndexMapping *cmi, UWORD srcx, UWORD srcy, UWORD dstx, UWORD dsty, UWORD sizex, UWORD sizey, UBYTE minterm, UBYTE mask) (A0, A1, A2, A3, D0, D1, D2, D3, D4, D5, D6, D7) */
    APTR                BlitPlanar2Direct;
    APTR                BlitPlanar2DirectDefault;
    APTR                Reserved[12];
    /* O: BOOL SetDPMSLevel(struct BoardInfo *bi, UBYTE level) (A0, D0) */
    APTR                SetDPMSLevel;
    APTR                ResetChip;
    APTR                GetFeatureAttrs;
    APTR                AllocBitMap;
    APTR                FreeBitMap;
    APTR                GetBitMapAttr;
    /* O: BOOL SetSprite(struct BoardInfo *bi, BOOL active, RGBFTYPE RGBFormat) (A0, D0, D7) */
    APTR                SetSprite;
    /* O: BOOL SetSpritePositiion(struct BoardInfo *bi, RGBFTYPE RGBFormat) (A0, D7) */
    APTR                SetSpritePosition;
    /* O: BOOL SetSpriteImage(struct BoardInfo *bi, RGBFTYPE RGBFormat) (A0, D7) */
    APTR                SetSpriteImage;
    /* O: BOOL SetSpriteColor(struct BoardInfo *bi, UBYTE index, UBYTE r, UBYTE g, UBYTE b, RGBFTYPE RGBFormat) (A0, D0, D1, D2, D3, D7) */
    APTR                SetSpriteColor;
    APTR                CreateFeature;
    APTR                SetFeatureAttrs;
    APTR                DeleteFeature;
    struct MinList      SpecialFeatures;
    struct ModeInfo *   ModeInfo;
    RGBFTYPE            RGBFormat;
    UWORD               XOffset;
    UWORD               YOffset;
    UBYTE               Depth;
    UBYTE               ClearMask;
    BOOL                Border;
    ULONG               Mask;
    struct CLUTEntry    CLUT[256];
    struct ViewPort *   ViewPort;
    struct BitMap *     VisibleBitMap;
    struct BitMapExtra *BitMapExtra;
    struct MinList      BitMapList;
    struct MinList      MemList;
    UWORD               MouseX;
    UWORD               MouseY;
    UBYTE               MouseWidth;
    UBYTE               MouseHeight;
    UBYTE               MouseXOffset;
    UBYTE               MouseYOffset;
    APTR                MouseImage;
    APTR                MousePens;
    struct Rectangle    MouseRect;
    APTR                MouseChunky;
    APTR                MouseRendered;
    APTR                MouseSaveBuffer;
    APTR                ChipData[16];
    APTR                CardData[16];
    APTR                MemorySpaceBase;
    ULONG               MemorySpaceSize;
    APTR                DoubleBufferList;
    ULONG               SyncTime;
    struct timeval      SyncPeriod;
    struct MsgPort      SoftVBlankPort;
};

#endif /* PICASSO_CARD_H */
/* vim: set shiftwidth=4 expandtab:  */
