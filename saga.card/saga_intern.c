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

#include "saga_intern.h"

void dump_ModeInfo(struct ModeInfo *mi)
{
    bug("ModeInfo: %p\n", mi);

    if (!mi)
        return;

    bug("  Node: ln_Name=\"%s\"\n", mi->Node.ln_Name);
    bug("  OpenCount: %ld\n", mi->OpenCount);
    bug("  Active: %s\n", mi->Active ? "TRUE" : "FALSE");
    bug("  Width: %ld\n", mi->Width);
    bug("  Height: %ld\n", mi->Height);
    bug("  Depth: %ld\n", mi->Depth);
    bug("  Flags: 0x%02lx\n", mi->Flags);
    bug("  HorTotal: %ld\n", mi->HorTotal);
    bug("  HorBlankSize: %ld\n", mi->HorBlankSize);
    bug("  HorSyncStart: %ld\n", mi->HorSyncStart);
    bug("  HorSyncSize: %ld\n", mi->HorSyncSize);
    bug("  HorSyncSkew: %ld\n", mi->HorSyncSkew);
    bug("  HorEnableSkew: %ld\n", mi->HorEnableSkew);
    bug("  VerTotal: %ld\n", mi->VerTotal);
    bug("  VerBlankSize: %ld\n", mi->VerBlankSize);
    bug("  VerSyncStart: %ld\n", mi->VerSyncStart);
    bug("  VerSyncSize: %ld\n", mi->VerSyncSize);
    bug("  Numerator: %ld\n", mi->Numerator);
    bug("  Denomerator: %ld\n", mi->Denomerator);
    bug("  PixelClock: %ld\n", mi->PixelClock);
}

void dump_BoardInfo(struct BoardInfo *bi)
{
    bug("  RegisterBase: %p\n", bi->RegisterBase);
    bug("  MemoryBase: %p\n", bi->MemoryBase);
    bug("  MemoryIOBase: %p\n", bi->MemoryIOBase);
    bug("  MemorySize: %lu\n", (unsigned long)bi->MemorySize);
    bug("  BoardName: \"%s\"\n", bi->BoardName);
    bug("  VBIName: \"%s\"\n", bi->VBIName);
    bug("  CardBase: %p \"%s\"\n", bi->CardBase, bi->CardBase->lib_Node.ln_Name);
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
    bug("  BoardFlags: 0x%02lx\n", (unsigned long)bi->BoardFlags);
    bug("  SoftSpriteFlags: 0x%02lx\n", (unsigned long)bi->SoftSpriteFlags);
    bug("  ChipFlags: 0x%02lx\n", (unsigned long)bi->ChipFlags);
    bug("  CardFlags: 0x%02lx\n", (unsigned long)bi->CardFlags);
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
    dump_ModeInfo(bi->ModeInfo);
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

int format2bpp(RGBFTYPE format)
{
    if ((1UL << format) & RGBMASK_8BIT)
        return 1;
    if ((1UL << format) & (RGBMASK_15BIT | RGBMASK_16BIT))
        return 2;
    if ((1UL << format) & RGBFF_Y4U2V2)
        return 2;
    if ((1UL << format) & RGBMASK_24BIT)
        return 3;
    if ((1UL << format) & RGBMASK_32BIT)
        return 4;

    return 0;
}

/* vim: set shiftwidth=4 expandtab:  */
