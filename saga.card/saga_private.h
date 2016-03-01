

#ifndef SAGA_PRIVATE_H

#include <exec/types.h>
#include <exec/libraries.h>

#include <aros/asmcall.h>

#include "saga_intern.h"

AROS_UFP0(VOID, _AllocCardMem);
AROS_UFP0(VOID, _FreeCardMem);
AROS_UFP2(BOOL, SetSwitch, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(BOOL,(__arg2),D0));
AROS_UFP3(BOOL, SetColorArray, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(UWORD,(__arg2),D0), 
          AROS_UFPA(UWORD,(__arg3),D1));
AROS_UFP2(BOOL, SetDAC, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(RGBFTYPE,(__arg2),D7));
AROS_UFP3(BOOL, SetGC, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(struct ModeInfo *,(__arg2),A1), 
          AROS_UFPA(BOOL,(__arg3),D0));
AROS_UFP6(BOOL, SetPanning, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(UBYTE *,(__arg2),A1), 
          AROS_UFPA(UWORD,(__arg3),D0), 
          AROS_UFPA(WORD,(__arg4),D1), 
          AROS_UFPA(WORD,(__arg5),D2), 
          AROS_UFPA(RGBFTYPE,(__arg6),D7));
AROS_UFP3(ULONG, CalculateBytesPerRow, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(UWORD,(__arg2),D0), 
          AROS_UFPA(RGBFTYPE,(__arg3),D7));
AROS_UFP3(APTR, CalculateMemory, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(APTR,(__arg2),A1), 
          AROS_UFPA(RGBFTYPE,(__arg3),D7));
AROS_UFP2(ULONG, GetCompatibleFormats, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(RGBFTYPE,(__arg2),D7));
AROS_UFP2(BOOL, SetDisplay, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(BOOL,(__arg2),D0));
AROS_UFP4(ULONG, ResolvePixelClock, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(struct ModeInfo *,(__arg2),A1), 
          AROS_UFPA(ULONG,(__arg3),D0), 
          AROS_UFPA(RGBFTYPE,(__arg4),D7));
AROS_UFP4(ULONG, GetPixelClock, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(struct ModeInfo *,(__arg2),A1), 
          AROS_UFPA(ULONG,(__arg3),D0), 
          AROS_UFPA(RGBFTYPE,(__arg4),D7));
AROS_UFP1(BOOL, SetClock, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0));
AROS_UFP2(VOID, SetMemoryMode, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(RGBFTYPE,(__arg2),D7));
AROS_UFP2(BOOL, SetWriteMask, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(UBYTE,(__arg2),D0));
AROS_UFP2(BOOL, SetClearMask, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(UBYTE,(__arg2),D0));
AROS_UFP2(BOOL, SetReadPlane, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(UBYTE,(__arg2),D0));
AROS_UFP2(VOID, WaitVerticalSync, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(BOOL,(__arg2),D0));
AROS_UFP2(BOOL, SetInterrupt,
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(BOOL,(__arg2),D0));
AROS_UFP1(VOID, WaitBlitter, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0));
AROS_UFP1(VOID, ScrollPlanar, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0));
AROS_UFP1(VOID, _UpdatePlanar, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0));
AROS_UFP11(BOOL, BlitPlanar2Chunky, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(struct BitMap *,(__arg2),A1), 
          AROS_UFPA(struct RenderInfo *,(__arg3),A2), 
          AROS_UFPA(UWORD,(__arg4),D0), 
          AROS_UFPA(UWORD,(__arg5),D1), 
          AROS_UFPA(UWORD,(__arg6),D2), 
          AROS_UFPA(UWORD,(__arg7),D3), 
          AROS_UFPA(UWORD,(__arg8),D4), 
          AROS_UFPA(UWORD,(__arg9),D5), 
          AROS_UFPA(UBYTE,(__arg10),D6), 
          AROS_UFPA(UBYTE,(__arg11),D7));
AROS_UFP9(BOOL, FillRect, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(struct RenderInfo *,(__arg2),A1), 
          AROS_UFPA(UWORD,(__arg3),D0), 
          AROS_UFPA(UWORD,(__arg4),D1), 
          AROS_UFPA(UWORD,(__arg5),D2), 
          AROS_UFPA(UWORD,(__arg6),D3), 
          AROS_UFPA(ULONG,(__arg7),D4), 
          AROS_UFPA(UBYTE,(__arg8),D5), 
          AROS_UFPA(RGBFTYPE,(__arg9),D7));
AROS_UFP8(BOOL, InvertRect, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(struct RenderInfo *,(__arg2),A1), 
          AROS_UFPA(UWORD,(__arg3),D0), 
          AROS_UFPA(UWORD,(__arg4),D1), 
          AROS_UFPA(UWORD,(__arg5),D2), 
          AROS_UFPA(UWORD,(__arg6),D3), 
          AROS_UFPA(UBYTE,(__arg7),D4), 
          AROS_UFPA(RGBFTYPE,(__arg8),D7));
AROS_UFP10(BOOL, BlitRect, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(struct RenderInfo *,(__arg2),A1), 
          AROS_UFPA(UWORD,(__arg3),D0), 
          AROS_UFPA(UWORD,(__arg4),D1), 
          AROS_UFPA(UWORD,(__arg5),D2), 
          AROS_UFPA(UWORD,(__arg6),D3), 
          AROS_UFPA(UWORD,(__arg7),D4), 
          AROS_UFPA(UWORD,(__arg8),D5), 
          AROS_UFPA(UBYTE,(__arg9),D6), 
          AROS_UFPA(RGBFTYPE,(__arg10),D7));
AROS_UFP9(BOOL, BlitTemplate, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(struct RenderInfo *,(__arg2),A1), 
          AROS_UFPA(struct Template *,(__arg3),A2), 
          AROS_UFPA(UWORD,(__arg4),D0), 
          AROS_UFPA(UWORD,(__arg5),D1), 
          AROS_UFPA(UWORD,(__arg6),D2), 
          AROS_UFPA(UWORD,(__arg7),D3), 
          AROS_UFPA(UBYTE,(__arg8),D4), 
          AROS_UFPA(RGBFTYPE,(__arg9),D7));
AROS_UFP9(BOOL, BlitPattern, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(struct RenderInfo *,(__arg2),A1), 
          AROS_UFPA(struct Pattern *,(__arg3),A2), 
          AROS_UFPA(UWORD,(__arg4),D0), 
          AROS_UFPA(UWORD,(__arg5),D1), 
          AROS_UFPA(UWORD,(__arg6),D2), 
          AROS_UFPA(UWORD,(__arg7),D3), 
          AROS_UFPA(UBYTE,(__arg8),D4), 
          AROS_UFPA(RGBFTYPE,(__arg9),D7));
AROS_UFP5(BOOL, DrawLine, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(struct RenderInfo *,(__arg2),A1), 
          AROS_UFPA(struct Line *,(__arg3),A2), 
          AROS_UFPA(UBYTE,(__arg4),D0), 
          AROS_UFPA(RGBFTYPE,(__arg5),D7));
AROS_UFP11(BOOL, BlitRectNoMaskComplete, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(struct RenderInfo *,(__arg2),A1), 
          AROS_UFPA(struct RenderInfo *,(__arg3),A2), 
          AROS_UFPA(UWORD,(__arg4),D0), 
          AROS_UFPA(UWORD,(__arg5),D1), 
          AROS_UFPA(UWORD,(__arg6),D2), 
          AROS_UFPA(UWORD,(__arg7),D3), 
          AROS_UFPA(UWORD,(__arg8),D4), 
          AROS_UFPA(UWORD,(__arg9),D5), 
          AROS_UFPA(UBYTE,(__arg10),D6), 
          AROS_UFPA(RGBFTYPE,(__arg11),D7));
AROS_UFP12(BOOL, BlitPlanar2Direct, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(struct BitMap *,(__arg2),A1), 
          AROS_UFPA(struct RenderInfo *,(__arg3),A2), 
          AROS_UFPA(struct ColorIndexMapping *,(__arg4),A3), 
          AROS_UFPA(UWORD,(__arg5),D0), 
          AROS_UFPA(UWORD,(__arg6),D1), 
          AROS_UFPA(UWORD,(__arg7),D2), 
          AROS_UFPA(UWORD,(__arg8),D3), 
          AROS_UFPA(UWORD,(__arg9),D4), 
          AROS_UFPA(UWORD,(__arg10),D5), 
          AROS_UFPA(UBYTE,(__arg11),D6), 
          AROS_UFPA(UBYTE,(__arg12),D7));
AROS_UFP2(BOOL, SetDPMSLevel, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(UBYTE,(__arg2),D0));
AROS_UFP1(VOID, ResetChip, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0));
AROS_UFP0(VOID, _GetFeatureAttrs);
AROS_UFP0(VOID, _AllocBitMap);
AROS_UFP0(VOID, _FreeBitMap);
AROS_UFP3(BOOL, SetSprite, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(BOOL,(__arg2),D0), 
          AROS_UFPA(RGBFTYPE,(__arg3),D7));
AROS_UFP2(BOOL, SetSpritePosition, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(RGBFTYPE,(__arg2),D7));
AROS_UFP2(BOOL, SetSpriteImage, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(RGBFTYPE,(__arg2),D7));
AROS_UFP6(BOOL, SetSpriteColor, 
          AROS_UFPA(struct BoardInfo *,(__arg1),A0), 
          AROS_UFPA(UBYTE,(__arg2),D0), 
          AROS_UFPA(UBYTE,(__arg3),D1), 
          AROS_UFPA(UBYTE,(__arg4),D2), 
          AROS_UFPA(UBYTE,(__arg5),D3), 
          AROS_UFPA(RGBFTYPE,(__arg6),D7));
AROS_UFP0(VOID, _CreateFeature);
AROS_UFP0(VOID, _SetFeatureAttrs);
AROS_UFP0(VOID, _DeleteFeature);

#endif /* DEFINES_SAGA_H*/
