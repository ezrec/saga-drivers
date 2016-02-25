

#ifndef SAGA_PRIVATE_H

#include <exec/types.h>
#include <exec/libraries.h>

#include <aros/libcall.h>

#include "saga_intern.h"

AROS_LD1(BOOL, FindCard, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          struct Library *, SAGABase, 5, Saga);
AROS_LD1(BOOL, InitCard, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          struct Library *, SAGABase, 6, Saga);
AROS_LD0(VOID, _AllocCardMem, 
          struct Library *, SAGABase, 23, Saga);
AROS_LD0(VOID, _FreeCardMem, 
          struct Library *, SAGABase, 24, Saga);
AROS_LD2(BOOL, SetSwitch, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(BOOL,(__arg2),D0), 
          struct Library *, SAGABase, 25, Saga);
AROS_LD3(BOOL, SetColorArray, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(UWORD,(__arg2),D0), 
          AROS_LDA(UWORD,(__arg3),D1), 
          struct Library *, SAGABase, 26, Saga);
AROS_LD2(BOOL, SetDAC, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(RGBFTYPE,(__arg2),D7), 
          struct Library *, SAGABase, 27, Saga);
AROS_LD3(BOOL, SetGC, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(struct ModeInfo *,(__arg2),A1), 
          AROS_LDA(BOOL,(__arg3),D0), 
          struct Library *, SAGABase, 28, Saga);
AROS_LD6(BOOL, SetPanning, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(UBYTE *,(__arg2),A1), 
          AROS_LDA(UWORD,(__arg3),D0), 
          AROS_LDA(WORD,(__arg4),D1), 
          AROS_LDA(WORD,(__arg5),D2), 
          AROS_LDA(RGBFTYPE,(__arg6),D7), 
          struct Library *, SAGABase, 29, Saga);
AROS_LD3(ULONG, CalculateBytesPerRow, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(UWORD,(__arg2),D0), 
          AROS_LDA(RGBFTYPE,(__arg3),D7), 
          struct Library *, SAGABase, 30, Saga);
AROS_LD3(APTR, CalculateMemory, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(APTR,(__arg2),A1), 
          AROS_LDA(RGBFTYPE,(__arg3),D7), 
          struct Library *, SAGABase, 31, Saga);
AROS_LD2(ULONG, GetCompatibleFormats, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(RGBFTYPE,(__arg2),D7), 
          struct Library *, SAGABase, 32, Saga);
AROS_LD2(BOOL, SetDisplay, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(BOOL,(__arg2),D0), 
          struct Library *, SAGABase, 33, Saga);
AROS_LD4(ULONG, ResolvePixelClock, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(struct ModeInfo *,(__arg2),A1), 
          AROS_LDA(ULONG,(__arg3),D0), 
          AROS_LDA(RGBFTYPE,(__arg4),D7), 
          struct Library *, SAGABase, 34, Saga);
AROS_LD4(ULONG, GetPixelClock, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(struct ModeInfo *,(__arg2),A1), 
          AROS_LDA(ULONG,(__arg3),D0), 
          AROS_LDA(RGBFTYPE,(__arg4),D7), 
          struct Library *, SAGABase, 35, Saga);
AROS_LD1(BOOL, SetClock, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          struct Library *, SAGABase, 36, Saga);
AROS_LD2(BOOL, SetMemoryMode, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(RGBFTYPE,(__arg2),D7), 
          struct Library *, SAGABase, 37, Saga);
AROS_LD2(BOOL, SetWriteMask, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(UBYTE,(__arg2),D0), 
          struct Library *, SAGABase, 38, Saga);
AROS_LD2(BOOL, SetClearMask, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(UBYTE,(__arg2),D0), 
          struct Library *, SAGABase, 39, Saga);
AROS_LD2(BOOL, SetReadPlane, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(UBYTE,(__arg2),D0), 
          struct Library *, SAGABase, 40, Saga);
AROS_LD2(VOID, WaitVerticalSync, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(BOOL,(__arg2),D0), 
          struct Library *, SAGABase, 41, Saga);
AROS_LD2(BOOL, SetInterrupt,
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(BOOL,(__arg2),D0), 
          struct Library *, SAGABase, 42, Saga);
AROS_LD1(VOID, WaitBlitter, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          struct Library *, SAGABase, 43, Saga);
AROS_LD0(VOID, _ScrollPlanar, 
          struct Library *, SAGABase, 44, Saga);
AROS_LD0(VOID, _UpdatePlanar, 
          struct Library *, SAGABase, 45, Saga);
AROS_LD11(BOOL, BlitPlanar2Chunky, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(struct BitMap *,(__arg2),A1), 
          AROS_LDA(struct RenderInfo *,(__arg3),A2), 
          AROS_LDA(UWORD,(__arg4),D0), 
          AROS_LDA(UWORD,(__arg5),D1), 
          AROS_LDA(UWORD,(__arg6),D2), 
          AROS_LDA(UWORD,(__arg7),D3), 
          AROS_LDA(UWORD,(__arg8),D4), 
          AROS_LDA(UWORD,(__arg9),D5), 
          AROS_LDA(UBYTE,(__arg10),D6), 
          AROS_LDA(UBYTE,(__arg11),D7), 
          struct Library *, SAGABase, 46, Saga);
AROS_LD9(BOOL, FillRect, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(struct RenderInfo *,(__arg2),A1), 
          AROS_LDA(UWORD,(__arg3),D0), 
          AROS_LDA(UWORD,(__arg4),D1), 
          AROS_LDA(UWORD,(__arg5),D2), 
          AROS_LDA(UWORD,(__arg6),D3), 
          AROS_LDA(ULONG,(__arg7),D4), 
          AROS_LDA(UBYTE,(__arg8),D5), 
          AROS_LDA(RGBFTYPE,(__arg9),D7), 
          struct Library *, SAGABase, 47, Saga);
AROS_LD8(BOOL, InvertRect, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(struct RenderInfo *,(__arg2),A1), 
          AROS_LDA(UWORD,(__arg3),D0), 
          AROS_LDA(UWORD,(__arg4),D1), 
          AROS_LDA(UWORD,(__arg5),D2), 
          AROS_LDA(UWORD,(__arg6),D3), 
          AROS_LDA(UBYTE,(__arg7),D4), 
          AROS_LDA(RGBFTYPE,(__arg8),D7), 
          struct Library *, SAGABase, 48, Saga);
AROS_LD10(BOOL, BlitRect, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(struct RenderInfo *,(__arg2),A1), 
          AROS_LDA(UWORD,(__arg3),D0), 
          AROS_LDA(UWORD,(__arg4),D1), 
          AROS_LDA(UWORD,(__arg5),D2), 
          AROS_LDA(UWORD,(__arg6),D3), 
          AROS_LDA(UWORD,(__arg7),D4), 
          AROS_LDA(UWORD,(__arg8),D5), 
          AROS_LDA(UBYTE,(__arg9),D6), 
          AROS_LDA(RGBFTYPE,(__arg10),D7), 
          struct Library *, SAGABase, 49, Saga);
AROS_LD9(BOOL, BlitTemplate, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(struct RenderInfo *,(__arg2),A1), 
          AROS_LDA(struct Template *,(__arg3),A2), 
          AROS_LDA(UWORD,(__arg4),D0), 
          AROS_LDA(UWORD,(__arg5),D1), 
          AROS_LDA(UWORD,(__arg6),D2), 
          AROS_LDA(UWORD,(__arg7),D3), 
          AROS_LDA(UBYTE,(__arg8),D4), 
          AROS_LDA(RGBFTYPE,(__arg9),D7), 
          struct Library *, SAGABase, 50, Saga);
AROS_LD9(BOOL, BlitPattern, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(struct RenderInfo *,(__arg2),A1), 
          AROS_LDA(struct Pattern *,(__arg3),A2), 
          AROS_LDA(UWORD,(__arg4),D0), 
          AROS_LDA(UWORD,(__arg5),D1), 
          AROS_LDA(UWORD,(__arg6),D2), 
          AROS_LDA(UWORD,(__arg7),D3), 
          AROS_LDA(UBYTE,(__arg8),D4), 
          AROS_LDA(RGBFTYPE,(__arg9),D7), 
          struct Library *, SAGABase, 51, Saga);
AROS_LD5(BOOL, DrawLine, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(struct RenderInfo *,(__arg2),A1), 
          AROS_LDA(struct Line *,(__arg3),A2), 
          AROS_LDA(UBYTE,(__arg4),D0), 
          AROS_LDA(RGBFTYPE,(__arg5),D7), 
          struct Library *, SAGABase, 52, Saga);
AROS_LD11(BOOL, BlitRectNoMaskComplete, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(struct RenderInfo *,(__arg2),A1), 
          AROS_LDA(struct RenderInfo *,(__arg3),A2), 
          AROS_LDA(UWORD,(__arg4),D0), 
          AROS_LDA(UWORD,(__arg5),D1), 
          AROS_LDA(UWORD,(__arg6),D2), 
          AROS_LDA(UWORD,(__arg7),D3), 
          AROS_LDA(UWORD,(__arg8),D4), 
          AROS_LDA(UWORD,(__arg9),D5), 
          AROS_LDA(UBYTE,(__arg10),D6), 
          AROS_LDA(RGBFTYPE,(__arg11),D7), 
          struct Library *, SAGABase, 53, Saga);
AROS_LD12(BOOL, BlitPlanar2Direct, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(struct BitMap *,(__arg2),A1), 
          AROS_LDA(struct RenderInfo *,(__arg3),A2), 
          AROS_LDA(struct ColorIndexMapping *,(__arg4),A3), 
          AROS_LDA(UWORD,(__arg5),D0), 
          AROS_LDA(UWORD,(__arg6),D1), 
          AROS_LDA(UWORD,(__arg7),D2), 
          AROS_LDA(UWORD,(__arg8),D3), 
          AROS_LDA(UWORD,(__arg9),D4), 
          AROS_LDA(UWORD,(__arg10),D5), 
          AROS_LDA(UBYTE,(__arg11),D6), 
          AROS_LDA(UBYTE,(__arg12),D7), 
          struct Library *, SAGABase, 54, Saga);
AROS_LD2(BOOL, SetDPMSLevel, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(UBYTE,(__arg2),D0), 
          struct Library *, SAGABase, 55, Saga);
AROS_LD0(VOID, _ResetChip, 
          struct Library *, SAGABase, 56, Saga);
AROS_LD0(VOID, _GetFeatureAttrs, 
          struct Library *, SAGABase, 57, Saga);
AROS_LD0(VOID, _AllocBitMap, 
          struct Library *, SAGABase, 58, Saga);
AROS_LD0(VOID, _FreeBitMap, 
          struct Library *, SAGABase, 59, Saga);
AROS_LD3(BOOL, SetSprite, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(BOOL,(__arg2),D0), 
          AROS_LDA(RGBFTYPE,(__arg3),D7), 
          struct Library *, SAGABase, 60, Saga);
AROS_LD2(BOOL, SetSpritePosition, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(RGBFTYPE,(__arg2),D7), 
          struct Library *, SAGABase, 61, Saga);
AROS_LD2(BOOL, SetSpriteImage, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(RGBFTYPE,(__arg2),D7), 
          struct Library *, SAGABase, 62, Saga);
AROS_LD6(BOOL, SetSpriteColor, 
          AROS_LDA(struct BoardInfo *,(__arg1),A0), 
          AROS_LDA(UBYTE,(__arg2),D0), 
          AROS_LDA(UBYTE,(__arg3),D1), 
          AROS_LDA(UBYTE,(__arg4),D2), 
          AROS_LDA(UBYTE,(__arg5),D3), 
          AROS_LDA(RGBFTYPE,(__arg6),D7), 
          struct Library *, SAGABase, 63, Saga);
AROS_LD0(VOID, _CreateFeature, 
          struct Library *, SAGABase, 64, Saga);
AROS_LD0(VOID, _SetFeatureAttrs, 
          struct Library *, SAGABase, 65, Saga);
AROS_LD0(VOID, _DeleteFeature, 
          struct Library *, SAGABase, 66, Saga);

#endif /* DEFINES_SAGA_H*/
