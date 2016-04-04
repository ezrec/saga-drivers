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

#ifndef PROTO_PICASSO96_H
#define PROTO_PICASSO96_H

#include <aros/libcall.h>

extern struct Library P96Base;

//------ Defines and enumerations

#define P96SA_Dummy                     (TAG_USER + 0x20000 + 96)
#define P96SA_Left                      (P96SA_Dummy + 0x0001)
#define P96SA_Top                       (P96SA_Dummy + 0x0002)
#define P96SA_Width                     (P96SA_Dummy + 0x0003)
#define P96SA_Height                    (P96SA_Dummy + 0x0004)
#define P96SA_Depth                     (P96SA_Dummy + 0x0005)
#define P96SA_DetailPen                 (P96SA_Dummy + 0x0006)
#define P96SA_BlockPen                  (P96SA_Dummy + 0x0007)
#define P96SA_Title                     (P96SA_Dummy + 0x0008)
#define P96SA_Colors                    (P96SA_Dummy + 0x0009)
#define P96SA_ErrorCode                 (P96SA_Dummy + 0x000a)
#define P96SA_Font                      (P96SA_Dummy + 0x000b)
#define P96SA_SysFont                   (P96SA_Dummy + 0x000c)
#define P96SA_Type                      (P96SA_Dummy + 0x000d)
#define P96SA_BitMap                    (P96SA_Dummy + 0x000e)
#define P96SA_PubName                   (P96SA_Dummy + 0x000f)
#define P96SA_PubSig                    (P96SA_Dummy + 0x0010)
#define P96SA_PubTask                   (P96SA_Dummy + 0x0011)
#define P96SA_DisplayID                 (P96SA_Dummy + 0x0012)
#define P96SA_DClip                     (P96SA_Dummy + 0x0013)
#define P96SA_ShowTitle                 (P96SA_Dummy + 0x0014)
#define P96SA_Behind                    (P96SA_Dummy + 0x0015)
#define P96SA_Quiet                     (P96SA_Dummy + 0x0016)
#define P96SA_AutoScroll                (P96SA_Dummy + 0x0017)
#define P96SA_Pens                      (P96SA_Dummy + 0x0018)
#define P96SA_SharePens                 (P96SA_Dummy + 0x0019)
#define P96SA_BackFill                  (P96SA_Dummy + 0x001a)
#define P96SA_Colors32                  (P96SA_Dummy + 0x001b)
#define P96SA_VideoControl              (P96SA_Dummy + 0x001c)
#define P96SA_RGBFormat                 (P96SA_Dummy + 0x001d)
#define P96SA_NoSprite                  (P96SA_Dummy + 0x001e)
#define P96SA_NoMemory                  (P96SA_Dummy + 0x001f)
#define P96SA_RenderFunc                (P96SA_Dummy + 0x0020)
#define P96SA_SaveFunc                  (P96SA_Dummy + 0x0021)
#define P96SA_UserData                  (P96SA_Dummy + 0x0022)
#define P96SA_Alignment                 (P96SA_Dummy + 0x0023)
#define P96SA_FixedScreen               (P96SA_Dummy + 0x0024)
#define P96SA_Exclusive                 (P96SA_Dummy + 0x0025)
#define P96SA_ConstantBytesPerRow       (P96SA_Dummy + 0x0026)

//------ Structs
struct TrueColorInfo {
    ULONG       PixelDistance;
    ULONG       BytesPerRow;
    UBYTE       *RedData;
    UBYTE       *GreenData;
    UBYTE       *BlueData;
};

//------ BitMaps
#define _p96AllocBitMap(_P96Base,SizeX,SizeY,Depth,Flags,Friend,RGBFormat) ({ \
    AROS_LIBREQ(P96Base,0)\
    AROS_LC6(struct BitMap *, p96AllocBitMap, \
        AROS_LCA(ULONG, SizeY, D0), \
        AROS_LCA(ULONG, SizeX, D1), \
        AROS_LCA(ULONG, Depth, D2), \
        AROS_LCA(ULONG, Flags, D3), \
        AROS_LCA(struct BitMap *, Friend, A0), \
        AROS_LCA(RGBFTYPE, RGBFormat, D7), \
    struct P96Base *, _P96Base, 5, Picasso96); \
})

#define p96AllocBitMap(SizeX,SizeY,Depth,Flags,Friend,RGBFormat) \
    _p96AllocBitMap(P96Base, SizeX,SizeY,Depth,Flags,Friend,RGBFormat)

#define _p96FreeBitMap(_P96Base,BitMap) ({ \
    AROS_LIBREQ(P96Base,0)\
    AROS_LC1NR(VOID, p96FreeBitMap, \
            AROS_LCA(struct BitMap *, BitMap, A0), \
    struct P96Base *, _P96Base, 6, Picasso96); \
})

#define p96FreeBitMap(BitMap) \
    _p96FreeBitMap(P96Base, BitMap)

#define _p96GetBitMapAttr(_P96Base,BitMap,Attribute) ({ \
    AROS_LIBREQ(P96Base,0)\
    AROS_LC2(ULONG, p96GetBitMapAttr, \
            AROS_LCA(struct BitMap *, BitMap, A0), \
            AROS_LCA(ULONG, Attribute, D0), \
    struct P96Base *, _P96Base, 7, Picasso96); \
})

#define p96GetBitMapAttr(BitMap,Attribute) \
    _p96GetBitMapAttr(P96Base, BitMap,Attribute)

#define _p96LockBitMap(_P96Base,BitMap,Buffer,Size) ({ \
    AROS_LIBREQ(P96Base,0)\
    AROS_LC3(LONG, p96LockBitMap, \
            AROS_LCA(struct BitMap *, BitMap, A0), \
            AROS_LCA(UBYTE *, Buffer, A1), \
            AROS_LCA(ULONG, Size, D0), \
    struct P96Base *, _P96Base, 8, Picasso96); \
})

#define p96LockBitMap(BitMap,Buffer,Size) \
    _p96LockBitMap(P96Base, BitMap,Buffer,Size)

#define _p96UnlockBitMap(_P96Base,BitMap,Lock) ({ \
    AROS_LIBREQ(P96Base,0)\
    AROS_LC2NR(VOID, p96UnlockBitMap, \
            AROS_LCA(struct BitMap *, BitMap, A0), \
            AROS_LCA(LONG, Lock, D0), \
    struct P96Base *, _P96Base, 9, Picasso96); \
})

#define p96UnlockBitMap(BitMap,Lock) \
    _p96UnlockBitMap(P96Base, BitMap,Lock)

//------ Modes
#define _p96BestModeIDTagList(_P96Base,Tags) ({ \
    AROS_LIBREQ(P96Base,0)\
    AROS_LC1(ULONG, p96BestModeIDTagList, \
            AROS_LCA(struct TagItem *, Tags, A0), \
    struct P96Base *, _P96Base, 10, Picasso96); \
})

#define p96BestModeIDTagList(Tags) \
    _p96BestModeIDTagList(P96Base, Tags)

#define _p96RequestModeIDTagList(_P96Base,Tags) ({ \
    AROS_LIBREQ(P96Base,0)\
    AROS_LC1(ULONG, p96RequestModeIDTagList, \
            AROS_LCA(struct TagItem *, Tags, A0), \
    struct P96Base *, _P96Base, 11, Picasso96); \
})

#define p96RequestModeIDTagList(Tags) \
    _p96RequestModeIDTagList(P96Base, Tags)

#define _p96AllocModeListTagList(_P96Base,Tags) ({ \
    AROS_LIBREQ(P96Base,0)\
    AROS_LC1(struct List *, p96AllocModeListTagList, \
            AROS_LCA(struct TagItem *, Tags, A0), \
    struct P96Base *, _P96Base, 12, Picasso96); \
})

#define p96AllocModeListTagList(Tags) \
    _p96AllocModeListTagList(P96Base, Tags)

#define _p96FreeModeList(_P96Base,ModeList) ({ \
    AROS_LIBREQ(P96Base,0)\
    AROS_LC1(ULONG, p96FreeModeList, \
            AROS_LCA(struct List *, ModeList, A0), \
    struct P96Base *, _P96Base, 13, Picasso96); \
})

#define p96FreeModeList(List) \
    _p96FreeModeList(P96Base, List)

#define _p96GetModeIDAttr(_P96Base,Mode,Attribute) ({ \
    AROS_LIBREQ(P96Base,0)\
    AROS_LC2(ULONG, p96GetModeIDAttr, \
            AROS_LCA(ULONG, Mode, D0), \
            AROS_LCA(ULONG, Attribute, D1), \
    struct P96Base *, _P96Base, 14, Picasso96); \
})

#define p96GetModeIDAttr(Mode,Attribute) \
    _p96GetModeIDAttr(P96Base, Mode,Attribute)

//------ Picasso96API screens
#define _p96OpenScreenTagList(_P96Base,Tags) ({ \
    AROS_LIBREQ(P96Base,0)\
    AROS_LC1(struct Screen *, p96OpenScreenTagList, \
            AROS_LCA(struct TagItem *, Tags, A0), \
    struct P96Base *, _P96Base, 15, Picasso96); \
})

#define p96OpenScreenTagList(Tags) \
    _p96OpenScreenTagList(P96Base, Tags)

#define _p96CloseScreen(_P96Base,Screen) ({ \
    AROS_LIBREQ(P96Base,0)\
    AROS_LC1(BOOL, p96CloseScreen, \
            AROS_LCA(struct Screen *, Screen, A0), \
    struct P96Base *, _P96Base, 16, Picasso96); \
})

#define p96CloseScreen(Screen) \
    _p96CloseScreen(P96Base, Screen)

//------ Painting functions
#define _p96WritePixelArray(_P96Base,ri,SrcX,SrcY,rp,DestX,DestY,SizeX,SizeY) ({ \
    AROS_LIBREQ(P96Base,0)\
    AROS_LC8NR(VOID, p96WritePixelArray, \
            AROS_LCA(struct RenderInfo *, ri, A0), \
            AROS_LCA(UWORD, SrcX, D0), \
            AROS_LCA(UWORD, SrcY, D1), \
            AROS_LCA(struct RastPort *, rp, A1), \
            AROS_LCA(UWORD, DestX, D2), \
            AROS_LCA(UWORD, DestY, D3), \
            AROS_LCA(UWORD, SizeX, D4), \
            AROS_LCA(UWORD, SizeY, D5), \
    struct P96Base *, _P96Base, 17, Picasso96); \
})

#define p96WritePixelArray(ri,SrcX,SrcY,rp,DestX,DestY,SizeX,SizeY) \
    _p96WritePixelArray(P96Base, ri,SrcX,SrcY,rp,DestX,DestY,SizeX,SizeY)

#define _p96ReadPixelArray(_P96Base,ri,DestX,DestY,rp,SrcX,SrcY,SizeX,SizeY) ({ \
    AROS_LIBREQ(P96Base,0)\
    AROS_LC8NR(VOID, p96ReadPixelArray, \
            AROS_LCA(struct RenderInfo *, ri, A0), \
            AROS_LCA(UWORD, DestX, D0), \
            AROS_LCA(UWORD, DestY, D1), \
            AROS_LCA(struct RastPort *, rp, A1), \
            AROS_LCA(UWORD, SrcX, D2), \
            AROS_LCA(UWORD, SrcY, D3), \
            AROS_LCA(UWORD, SizeX, D4), \
            AROS_LCA(UWORD, SizeY, D5), \
    struct P96Base *, _P96Base, 18, Picasso96); \
})

#define p96ReadPixelArray(ri,DestX,DestY,rp,SrcX,SrcY,SizeX,SizeY) \
    _p96ReadPixelArray(P96Base, ri,DestX,DestY,rp,SrcX,SrcY,SizeX,SizeY)

#define _p96WritePixel(_P96Base,rp,x,y,color) ({ \
    AROS_LIBREQ(P96Base,0)\
    AROS_LC4(ULONG, p96WritePixel, \
            AROS_LCA(struct RastPort *, rp, A0), \
            AROS_LCA(UWORD, x, D0), \
            AROS_LCA(UWORD, y, D1), \
            AROS_LCA(ULONG, color, D2), \
    struct P96Base *, _P96Base, 19, Picasso96); \
})

#define p96WritePixel(rp,x,y,color) \
    _p96WritePixel(P96Base, rp,x,y,color)

#define _p96ReadPixel(_P96Base,rp,x,y) ({ \
    AROS_LIBREQ(P96Base,0)\
    AROS_LC3(ULONG, p96ReadPixel, \
            AROS_LCA(struct RastPort *, rp, A0), \
            AROS_LCA(UWORD, x, D0), \
            AROS_LCA(UWORD, y, D1), \
    struct P96Base *, _P96Base, 20, Picasso96); \
})

#define p96ReadPixel(rp,x,y) \
    _p96ReadPixel(P96Base, rp,x,y)

#define _p96RectFill(_P96Base,rp,MinX,MinY,MaxX,MaxY,color) ({ \
    AROS_LIBREQ(P96Base,0)\
    AROS_LC6NR(VOID, p96RectFill, \
            AROS_LCA(struct RastPort *, rp, A1), \
            AROS_LCA(UWORD, MinX, D0), \
            AROS_LCA(UWORD, MinY, D1), \
            AROS_LCA(UWORD, MaxX, D2), \
            AROS_LCA(UWORD, MaxY, D3), \
            AROS_LCA(ULONG, color, D4), \
    struct P96Base *, _P96Base, 21, Picasso96); \
})

#define p96RectFill(rp,MinX,MinY,MaxX,MaxY,color) \
    _p96RectFill(P96Base, rp,MinX,MinY,MaxX,MaxY,color)

#define _p96WriteTrueColorData(_P96Base,tci,SrcX,SrcY,rp,DestX,DestY,SizeX,SizeY) ({ \
    AROS_LIBREQ(P96Base,0)\
    AROS_LC8NR(VOID, p96WriteTrueColorData, \
            AROS_LCA(struct TrueColorInfo *, tci, A0), \
            AROS_LCA(UWORD, SrcX, D0), \
            AROS_LCA(UWORD, SrcY, D1), \
            AROS_LCA(struct RastPort *, rp, A1), \
            AROS_LCA(UWORD, DestX , D2), \
            AROS_LCA(UWORD, DestY, D3), \
            AROS_LCA(UWORD, SizeX, D4), \
            AROS_LCA(UWORD, SizeY, D5), \
    struct P96Base *, _P96Base, 22, Picasso96); \
})

#define p96WriteTrueColorData(tci,SrcX,SrcY,rp,DestX,DestY,SizeX,SizeY) \
    _p96WriteTrueColorData(P96Base, tci,SrcX,SrcY,rp,DestX,DestY,SizeX,SizeY)

#define _p96ReadTrueColorData(_P96Base,tci,DestX,DestY,rp,SrcX,SrcY,SizeX,SizeY) ({ \
    AROS_LIBREQ(P96Base,0)\
    AROS_LC8NR(VOID, p96ReadTrueColorData, \
            AROS_LCA(struct TrueColorInfo *, tci, A0), \
            AROS_LCA(UWORD, DestX, D0), \
            AROS_LCA(UWORD, DestY, D1), \
            AROS_LCA(struct RastPort *, rp, A1), \
            AROS_LCA(UWORD, SrcX, D2), \
            AROS_LCA(UWORD, SrcY, D3), \
            AROS_LCA(UWORD, SizeX, D4), \
            AROS_LCA(UWORD, SizeY, D5), \
    struct P96Base *, _P96Base, 23, Picasso96); \
})

#define p96ReadTrueColorData(tci,DestX,DestY,rp,SrcX,SrcY,SizeX,SizeY) \
    _p96ReadTrueColorData(P96Base, tci,DestX,DestY,rp,SrcX,SrcY,SizeX,SizeY)

//------ PIP functions
#define _p96PIP_OpenTagList(_P96Base,Tags) ({ \
    AROS_LIBREQ(P96Base,0)\
    AROS_LC1(struct Window *, p96PIP_OpenTagList, \
            AROS_LCA(struct TagItem *, Tags, A0), \
    struct P96Base *, _P96Base, 24, Picasso96); \
})

#define p96PIP_OpenTagList(Tags) \
    _p96PIP_OpenTagList(P96Base, Tags)

#define _p96PIP_Close(_P96Base,Window) ({ \
    AROS_LIBREQ(P96Base,0)\
    AROS_LC1(BOOL, p96PIP_Close, \
            AROS_LCA(struct Window *, Window, A0), \
    struct P96Base *, _P96Base, 25, Picasso96); \
})

#define p96PIP_Close(Window) \
    _p96PIP_Close(P96Base, Window)

#define _p96PIP_SetTagList(_P96Base,Window,Tags) ({ \
    AROS_LIBREQ(P96Base,0)\
    AROS_LC2(ULONG, p96PIP_SetTagList, \
            AROS_LCA(struct Window *, Window, A0), \
            AROS_LCA(struct TagItem *, Tags, A1), \
    struct P96Base *, _P96Base, 26, Picasso96); \
})

#define p96PIP_SetTagList(Window,Tags) \
    _p96PIP_SetTagList(P96Base, Window,Tags)

#define _p96PIP_GetTagList(_P96Base,Window,Tags) ({ \
    AROS_LIBREQ(P96Base,0)\
    AROS_LC2(ULONG, p96PIP_GetTagList, \
            AROS_LCA(struct Window *, Window, A0), \
            AROS_LCA(struct TagItem *, Tags, A1), \
    struct P96Base *, _P96Base, 27, Picasso96); \
})

#define p96PIP_GetTagList(Window,Tags) \
    _p96PIP_GetTagList(P96Base, Window,Tags)

#define _p96PIP_GetIMsg(_P96Base,Port) ({ \
    AROS_LIBREQ(P96Base,0)\
    AROS_LC1(struct IntuiMessage *, p96PIP_GetIMsg, \
            AROS_LCA(struct MsgPort *, Port, A0), \
    struct P96Base *, _P96Base, 28, Picasso96); \
})

#define p96PIP_GetIMsg(Port) \
    _p96PIP_GetIMsg(P96Base, Port)

#define _p96PIP_ReplyIMsg(_P96Base,IntuiMessage) ({ \
    AROS_LIBREQ(P96Base,0)\
    AROS_LC1NR(VOID, p96PIP_ReplyIMsg, \
            AROS_LCA(struct IntuiMessage, IntuiMessage, A1), \
    struct P96Base *, _P96Base, 29, Picasso96); \
})

#define p96PIP_ReplyIMsg(IntuiMessage) \
    _p96PIP_ReplyIMsg(P96Base, IntuiMessage)

//------ functions in V1.42 or higher
#define _p96GetRTGDataTagList(_P96Base,Tags) ({ \
    AROS_LIBREQ(P96Base,1)\
    AROS_LCx(LONG, p96GetRTGDataTagList, \
            AROS_LCA(struct TagItem *, Tags, A0), \
    struct P96Base *, _P96Base, 30, Picasso96); \
})

#define p96GetRTGDataTagList(Tags) \
    _p96GetRTGDataTagList(P96Base, Tags)

#define _p96GetBoardDataTagList(_P96Base,Board,Tags) ({ \
    AROS_LIBREQ(P96Base,1)\
    AROS_LC2(LONG, p96GetBoardDataTagList, \
            AROS_LCA(ULONG, Board, D0), \
            AROS_LCA(struct TagItem *, Tags, A0), \
    struct P96Base *, _P96Base, 31, Picasso96); \
})

#define p96GetBoardDataTagList(Board,Tags) \
    _p96GetBoardDataTagList(P96Base, Board,Tags)

#define _p96EncodeColor(_P96Base,RGBFormat,Color) ({ \
    AROS_LIBREQ(P96Base,1)\
    AROS_LC2(ULONG, p96EncodeColor, \
            AROS_LCA(RGBFTYPE, RGBFormat, D0), \
            AROS_LCA(ULONG, Color, D1), \
    struct P96Base *, _P96Base, 32, Picasso96); \
})

#define p96EncodeColor(RGBFormat,Color) \
    _p96EncodeColor(P96Base, RGBFormat,Color)


#endif /* PROTO_PICASSO96_H */
/* vim: set shiftwidth=4 expandtab:  */
