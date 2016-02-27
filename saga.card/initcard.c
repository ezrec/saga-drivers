/*
    Copyright 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
    Licensed under the MIT (Expat) license. ©
*/

#include  <string.h>

#include <aros/libcall.h>

#include <proto/exec.h>

#include "saga_intern.h"

#include "saga_private.h"

static void add_resolution(struct BoardInfo *bi, CONST_STRPTR name, int id, UWORD width, UWORD height)
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
    CopyMem(res->Name, name, strlen(name));

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

void dump_bi(struct BoardInfo *bi)
{
    bug("  RegisterBase: %p\n", bi->RegisterBase);
    bug("  MemoryBase: %p\n", bi->MemoryBase);
    bug("  MemoryIOBase: %p\n", bi->MemoryIOBase);
    bug("  MemorySize: %lu\n", (unsigned long)bi->MemorySize);
    bug("  BoardName: \"%s\"\n", bi->BoardName);
    bug("  VBIName: \"%s\"\n", bi->VBIName);
    bug("  CardBase: %p\n", bi->CardBase);
    bug("  ChipBase: %p\n", bi->ChipBase);
    bug("  ExecBase: %p\n", bi->ExecBase);
    bug("  UtilBase: %p\n", bi->UtilBase);
    //struct Interrupt    HardInterrupt;
    //struct Interrupt    SoftInterrupt;
    //BYTE                BoardLock[46];
    //struct MinList      ResolutionsList;
    bug("  BoardType: %lu\n", (unsigned long)bi->BoardType);
    bug("  PaletteChipType: %lu\n", (unsigned long)bi->PaletteChipType);
    bug("  GraphicsControllerType: %lu\n", (unsigned long)bi->GraphicsControllerType);
    bug("  MoniSwitch: %lu\n", (unsigned long)bi->MoniSwitch);
    bug("  BitsPerCannon: %lu\n", (unsigned long)bi->BitsPerCannon);
    bug("  BoardFlags: %lu\n", (unsigned long)bi->BoardFlags);
    bug("  SoftSpriteFlags: %lu\n", (unsigned long)bi->SoftSpriteFlags);
    bug("  ChipFlags: %lu\n", (unsigned long)bi->ChipFlags);
    bug("  CardFlags: %lu\n", (unsigned long)bi->CardFlags);
    bug("  BoardNum: %lu\n", (unsigned long)bi->BoardNum);
    bug("  RGBFormats: %lu\n", (unsigned long)bi->RGBFormats);
    //UWORD               MaxHorValue[MAXMODES];
    //UWORD               MaxVerValue[MAXMODES];
    //UWORD               MaxHorResolution[MAXMODES];
    //UWORD               MaxVerResolution[MAXMODES];
    bug("  MaxMemorySize: %lu\n", (unsigned long)bi->MaxMemorySize);
    bug("  MaxChunkSize: %lu\n", (unsigned long)bi->MaxChunkSize);
    bug("  MemoryClock: %lu\n", (unsigned long)bi->MemoryClock);
    //ULONG               PixelClockCount[MAXMODES];

    bug("  AllocCardMem: %p\n", bi->AllocCardMem);
    bug("  FreeCardMem: %p\n", bi->FreeCardMem);
    /* R: BOOL SetSwitch(struct BoardInfo *bi, BOOL new_state) (A0, D0) */
    bug("  SetSwitch: %p\n", bi->SetSwitch);
    /* R: BOOL SetColorArray(struct BoardInfo *bi, UWORD start, UWORD count) (A0, D0, D1) */
    bug("  SetColorArray: %p\n", bi->SetColorArray);
    /* R: BOOL SetDAC(struct BoardInfo *bi, RGBFTYPE RGBFormat) (A0, D7) */
    bug("  SetDAC: %p\n", bi->SetDAC);
    /* R: BOOL SetGC(struct BoardInfo *bi, struct ModeInfo *mi, BOOL border) (A0, A1, D0) */
    bug("  SetGC: %p\n", bi->SetGC);
    /* R: BOOL SetPanning(struct BoardInfo *bi, UBYTE *mem, UWORD width, WORD xoffset, WORD yoffset, RGBFTYPE RGBFormat) (A0, A1, D0, D1, D2, D7) */
    bug("  SetPanning: %p\n", bi->SetPanning);
    /* R: ULONG CalculateBytesPerRow(struct BoardInfo *bi, UWORD width, RGBFTYPE RGBFormat) (A0, D0, D7) */
    bug("  CalculateBytesPerRow: %p\n", bi->CalculateBytesPerRow);
    /* R: APTR CalculateMemory(struct BoardInfo *bi, APTR LogicalMemory, RGBFTYPE RGBFormat) (A0, A1, D7) */
    bug("  CalculateMemory: %p\n", bi->CalculateMemory);
    /* R: ULONG GetCompatibleFormats(struct BoardInfo *bi, RGBFTYPE RGBFormat) (A0, D7) */
    bug("  GetCompatibleFormats: %p\n", bi->GetCompatibleFormats);
    /* R: BOOL SetDisplay(struct BoardInfo *bi, BOOL enabled) (A0, D0) */
    bug("  SetDisplay: %p\n", bi->SetDisplay);
    /* R: ULONG ResolvePixelClock(struct BoardInfo *bi, struct ModeInfo *mi, ULONG pixelclock, RGBFTYPE RGBFormat) (A0, A1, D0, D7) */
    bug("  ResolvePixelClock: %p\n", bi->ResolvePixelClock);
    /* R: ULONG GetPixelClock(struct BoardInfo *bi, struct ModeInfo *mi, ULONG index, RGBFTYPE RBGFormat) (A0, A1, D0, D7) */
    bug("  GetPixelClock: %p\n", bi->GetPixelClock);
    /* R: BOOL SetClock(struct BoardInfo *bi) (A0) */
    bug("  SetClock: %p\n", bi->SetClock);
    /* R: BOOL SetMemoryMode(struct BoardInfo *bi, RGBFTYPE RGBMode) (A0, D7) */
    bug("  SetMemoryMode: %p\n", bi->SetMemoryMode);
    /* R: BOOL SetWriteMask(struct BoardInfo *bi, UBYTE mask) (A0, D0) */
    bug("  SetWriteMask: %p\n", bi->SetWriteMask);
    /* R: BOOL SetClearMask(struct BoardInfo *bi, UBYTE mask) (A0, D0) */
    bug("  SetClearMask: %p\n", bi->SetClearMask);
    /* R: BOOL SetReadPlane(struct BoardInfo *bi, UBYTE plane) (A0, D0) */
    bug("  SetReadPlane: %p\n", bi->SetReadPlane);
    /* R: VOID WaitVerticalSync(struct BoardInfo *bi, BOOL end) (A0, D0) */
    bug("  WaitVerticalSync: %p\n", bi->WaitVerticalSync);
    /* R: VOID SetInterrupt(struct BoardInfo *bi, BOOL enabled) (A0, D0) */
    bug("  SetInterrupt: %p\n", bi->SetInterrupt);
    /* R: VOID WaitBlitter(struct BoardInfo *bi) (A0) */
    bug("  WaitBlitter: %p\n", bi->WaitBlitter);
    bug("  ScrollPlanar: %p\n", bi->ScrollPlanar);
    bug("  ScrollPlanarDefault: %p\n", bi->ScrollPlanarDefault);
    bug("  UpdatePlanar: %p\n", bi->UpdatePlanar);
    bug("  UpdatePlanarDefault: %p\n", bi->UpdatePlanarDefault);
    /* O: BOOL BlitPlanar2Chunky(struct BoardInfo *bi, struct BitMap *bm, struct RenderInfo *ri, UWORD srcx, UWORD srcy, UWORD dstx, UWORD dsty, UWORD sizex, UWORD sizey, UBYTE minterm, UBYTE mask) (A0, A1, A2, D0, D1, D2, D3, D4, D5, D6, D7) */
    bug("  BlitPlanar2Chunky: %p\n", bi->BlitPlanar2Chunky);
    bug("  BlitPlanar2ChunkyDefault: %p\n", bi->BlitPlanar2ChunkyDefault);
    /* O: BOOL FillRect(struct BoardInfo *bi, struct RenderInfo *ri, UWORD x, UWORD y, UWORD width, UWORD height, ULONG pen, UBYTE mask, RGBFTYPE RGBFormat) (A0, A1, D0, D1, D2, D3, D4, D5, D7) */
    bug("  FillRect: %p\n", bi->FillRect);
    bug("  FillRectDefault: %p\n", bi->FillRectDefault);
    /* O: BOOL InvertRect(struct BoardInfo *bi, struct RenderInfo *ri, UWORD x, UWORD y, UWORD width, UWORD height, UBYTE mask, RGBFTYPE RGBFormat) (A0, A1, D0, D1, D2, D3, D4, D7) */
    bug("  InvertRect: %p\n", bi->InvertRect);
    bug("  InvertRectDefault: %p\n", bi->InvertRectDefault);
    /* O: BOOL BlitRect(struct BoardInfo *bi, struct RenderInfo *ri, UWORD srcx, UWORD srcy, UWORD dstx, UWORD dsty, UWORD height, UBYTE mask, RGBFTYPE RGBFormat) (A0, A1, D0, D1, D2, D3, D4, D5, D6, D7) */
    bug("  BlitRect: %p\n", bi->BlitRect);
    bug("  BlitRectDefault: %p\n", bi->BlitRectDefault);
    /* O: BOOL BlitTemplate(struct BoardInfo *bi, struct RenderInfo *ri, struct Template *templ, UWORD x, UWORD y, UWORD width, UWORD height, UBYTE mask, RGBFTYPE RGBFormat) (A0, A1, A2, D0, D1, D2, D3, D4, D7) */
    bug("  BlitTemplate: %p\n", bi->BlitTemplate);
    bug("  BlitTemplateDefault: %p\n", bi->BlitTemplateDefault);
    /* O: BOOL BlitPattern(struct BoardInfo *bi, struct RenderInfo *ri, struct Pattern *pat, UWORD x, UWORD y, UWORD width, UWORD height, UBYTE mask, RGBFTYPE RGBFormat) (A0, A1, A2, D0, D1, D2, D3, D4, D7) */
    bug("  BlitPattern: %p\n", bi->BlitPattern);
    bug("  BlitPatternDefault: %p\n", bi->BlitPatternDefault);
    /* O: BOOL DrawLine(struct BoardInfo *bi, struct RenderInfo *ri, struct Line *line, UBYTE mask, RGBFTYPE RGBFormat) (A0, A1, A2, D0, D7) */
    bug("  DrawLine: %p\n", bi->DrawLine);
    bug("  DrawLineDefault: %p\n", bi->DrawLineDefault);
    /* O: BOOL BlitRectNoMaskComplete(struct BoardInfo *bi, struct RenderInfo *src, struct RenderInfo *dst, UWORD srcx, UWORD srcy, UWORD dstx, UWORD dsty, UWORD height, UBYTE opcode, RGBFTYPE RGBFormat) (A0, A1, A2, D0, D1, D2, D3, D4, D5, D6, D7) */
    bug("  BlitRectNoMaskComplete: %p\n", bi->BlitRectNoMaskComplete);
    bug("  BlitRectNoMaskCompleteDefault: %p\n", bi->BlitRectNoMaskCompleteDefault);
    /* O: BOOL BlitPlanar2Direct(struct BoardInfo *bi, struct BitMap *bm, struct RenderInfo *ri, struct ColorIndexMapping *cmi, UWORD srcx, UWORD srcy, UWORD dstx, UWORD dsty, UWORD sizex, UWORD sizey, UBYTE minterm, UBYTE mask) (A0, A1, A2, A3, D0, D1, D2, D3, D4, D5, D6, D7) */
    bug("  BlitPlanar2Direct: %p\n", bi->BlitPlanar2Direct);
    bug("  BlitPlanar2DirectDefault: %p\n", bi->BlitPlanar2DirectDefault);
    //APTR                Reserved[10];
    /* O: BOOL SetDPMSLevel(struct BoardInfo *bi, UBYTE level) (A0, D0) */
    bug("  SetDPMSLevel: %p\n", bi->SetDPMSLevel);
    bug("  ResetChip: %p\n", bi->ResetChip);
    bug("  GetFeatureAttrs: %p\n", bi->GetFeatureAttrs);
    bug("  AllocBitMap: %p\n", bi->AllocBitMap);
    bug("  FreeBitMap: %p\n", bi->FreeBitMap);
    bug("  GetBitMapAttr: %p\n", bi->GetBitMapAttr);
    /* O: BOOL SetSprite(struct BoardInfo *bi, BOOL active, RGBFTYPE RGBFormat) (A0, D0, D7) */
    bug("  SetSprite: %p\n", bi->SetSprite);
    /* O: BOOL SetSpritePositiion(struct BoardInfo *bi, RGBFTYPE RGBFormat) (A0, D7) */
    bug("  SetSpritePosition: %p\n", bi->SetSpritePosition);
    /* O: BOOL SetSpriteImage(struct BoardInfo *bi, RGBFTYPE RGBFormat) (A0, D7) */
    bug("  SetSpriteImage: %p\n", bi->SetSpriteImage);
    /* O: BOOL SetSpriteColor(struct BoardInfo *bi, UBYTE index, UBYTE r, UBYTE g, UBYTE b, RGBFTYPE RGBFormat) (A0, D0, D1, D2, D3, D7) */
    bug("  SetSpriteColor: %p\n", bi->SetSpriteColor);
    bug("  CreateFeature: %p\n", bi->CreateFeature);
    bug("  SetFeatureAttrs: %p\n", bi->SetFeatureAttrs);
    bug("  DeleteFeature: %p\n", bi->DeleteFeature);
    //struct MinList      SpecialFeatures;
    //struct ModeInfo *   ModeInfo;
    bug("  RGBFormat: %lu\n", (unsigned long)bi->RGBFormat);
    bug("  XOffset: %lu\n", (unsigned long)bi->XOffset);
    bug("  YOffset: %lu\n", (unsigned long)bi->YOffset);
    bug("  Depth: %lu\n", (unsigned long)bi->Depth);
    bug("  ClearMask: %lu\n", (unsigned long)bi->ClearMask);
    bug("  Border: %lu\n", (unsigned long)bi->Border);
    bug("  Mask: %lu\n", (unsigned long)bi->Mask);
    //struct CLUTEntry    CLUT[256];
    //struct ViewPort     ViewPort;
    //struct BitMap       VisibleBitMap;
    //struct BitMapExtra  BitMapExtra;
    //struct MinList      BitMapList;
    //struct MinList      MemList;
    bug("  MouseX: %lu\n", (unsigned long)bi->MouseX);
    bug("  MouseY: %lu\n", (unsigned long)bi->MouseY);
    bug("  MouseWidth: %lu\n", (unsigned long)bi->MouseWidth);
    bug("  MouseHeight: %lu\n", (unsigned long)bi->MouseHeight);
    bug("  MouseXOffset: %lu\n", (unsigned long)bi->MouseXOffset);
    bug("  MouseYOffset: %lu\n", (unsigned long)bi->MouseYOffset);
    bug("  MouseImage: %p\n", bi->MouseImage);
    bug("  MousePens: %p\n", bi->MousePens);
    //struct Rectangle    MouseRect;
    bug("  MouseChunky: %p\n", bi->MouseChunky);
    bug("  MouseRendered: %p\n", bi->MouseRendered);
    bug("  MouseSaveBuffer: %p\n", bi->MouseSaveBuffer);
    //APTR                ChipData[16];
    //APTR                CardData[16];
    bug("  MemorySpaceBase: %p\n", bi->MemorySpaceBase);
    bug("  MemorySpaceSize: %lu\n", (unsigned long)bi->MemorySpaceSize);
    bug("  DoubleBufferList: %p\n", bi->DoubleBufferList);
    bug("  SyncTime: %lu\n", (unsigned long)bi->SyncTime);
    //struct timeval      SyncPeriod;
    //struct MsgPort      SoftVBlankPort;
    bug("  sizeof() = %lu\n", sizeof(*bi));
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

    debug("bi: %p", bi);
    dump_bi(bi);

    bi->BitsPerCannon = 8;
    bi->RGBFormats = RGBFF_CLUT |
                     RGBFF_R5G5B5 |
                     RGBFF_R5G6B5 |
                     RGBFF_R8G8B8 |
                     RGBFF_A8R8G8B8;
    bi->BoardType = 1;                  // 'Unknown'
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
                        BIF_NOMEMORYMODEMIX;

    /* For now, we only support 640x480 in all modes */
    for (i = 0; i < MAXMODES; i++) {
        bi->MaxHorResolution[i] = 640;
        bi->MaxVerResolution[i] = 480;
    }

    /* Create our resolutions */
    add_resolution(bi, "SAGA:640x480", 0, 640, 480);

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
    dump_bi(bi);

    return TRUE;

    AROS_LIBFUNC_EXIT
}

