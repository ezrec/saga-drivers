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

#include <exec/memory.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/alib.h>

#include <aros/shcommands.h>

#include <saga/video.h>

#include <proto/picasso96.h>

#include "saga_intern.h"

static int p96mode(struct Library *SysBase, struct Library *DOSBase, ULONG width, ULONG height, RGBFTYPE mode)
{
    ULONG color[4] = { 0xff0000, 0x00ff00, 0x0000ff, 0xffffff };
    struct Screen *scr;
    struct RastPort *rp;
    struct Library *P96Base;
    struct TagItem screentag[] = {
        { P96SA_RGBFormat, mode },
        { P96SA_Width, width },
        { P96SA_Height, height },
        { TAG_END }
    };

    P96Base = OpenLibrary("Picasso96API.library", 0);
    if (!P96Base) {
        Printf("Can't open Picasso96API.library\n");
        return RETURN_FAIL;
    }

    scr = p96OpenScreenTagList(screentag);
    if (!scr) {
        Printf("Can't open %ldx%ld screen, for RGBFTYPE %ld\n",
                width, height, mode);
        CloseLibrary(P96Base);
        return RETURN_FAIL;
    }

    rp = &scr->RastPort;

    p96RectFill(rp, 0, 0, width/2, height/2, color[0]);
    p96RectFill(rp, width/2, 0, width, height/2, color[1]);
    p96RectFill(rp, 0, height/2, width/2, height, color[2]);
    p96RectFill(rp, width/2, height/2, width, height, color[3]);

    /* Delay for 4 seconds */
    Delay(4 * 50);

    p96CloseScreen(scr);

    CloseLibrary(P96Base);
    return RETURN_OK;
}

AROS_SH7H(SDDiag , 0.9a,                 "SAGA Graphic Diagnostic\n",
AROS_SHAH(ULONG *  , W= ,WIDTH,/K/N,    0 , "Width\n"),
AROS_SHAH(ULONG *  , H= ,HEIGHT,/K/N,   0 , "Height\n"),
AROS_SHAH(ULONG *  , F= ,FORMAT,/K/N,   0 , "Format (0-5)\n"),
AROS_SHAH(ULONG *  , P= ,PADDING,/K,  0 , "Padding\n"),
AROS_SHAH(BOOL     , X= ,XDOUBLE,/S, FALSE, "Double X\n"),
AROS_SHAH(BOOL     , Y= ,YDOUBLE,/S, FALSE, "Double Y\n"),
AROS_SHAH(BOOL     , P96= ,PICASSO96,/S, FALSE, "Use Picasso96, not hardware banging\n"))
{
    AROS_SHCOMMAND_INIT

    ULONG width = SHArg(WIDTH) ? *SHArg(WIDTH) : 640;
    ULONG height= SHArg(HEIGHT) ? *SHArg(HEIGHT) : 480;
    ULONG mode  = SHArg(FORMAT) ? *SHArg(FORMAT) : SAGA_VIDEO_FORMAT_RGB16;
    BOOL  dx    = SHArg(XDOUBLE);
    BOOL  dy    = SHArg(YDOUBLE);
    BOOL  p96   = SHArg(PICASSO96);
    ULONG padding = SHArg(PADDING) ? *SHArg(PADDING) : 0;
    int bpp = (mode == SAGA_VIDEO_FORMAT_CLUT8) ? 1 :
              (mode == SAGA_VIDEO_FORMAT_RGB16) ? 2 :
              (mode == SAGA_VIDEO_FORMAT_RGB15) ? 2 :
              (mode == SAGA_VIDEO_FORMAT_RGB24) ? 3 :
              (mode == SAGA_VIDEO_FORMAT_RGB32) ? 4 : 1;
    int i, gx, gy, x, y;
    UBYTE *here;

    if (p96)
        return p96mode((struct Library *)SysBase, (struct Library *)DOSBase, width, height, mode);

    here = AllocMem(SAGA_VIDEO_MEMSIZE, MEMF_REVERSE | MEMF_LOCAL | MEMF_FAST);
    if (!here) {
        Printf("Can't allocate %ldMB memory window\n", SAGA_VIDEO_MEMSIZE / 1024 / 1024);
        return RETURN_FAIL;
    }

    /* Set up video hardware */
    Printf("@$%08lx %ldx%ld, Format %ld", (ULONG)here, width, height, mode);
    if (dx) {
        mode |= SAGA_VIDEO_MODE_DBLSCN(SAGA_VIDEO_DBLSCAN_X);
        Printf(", DoubleX");
    }
    if (dy) {
        mode |= SAGA_VIDEO_MODE_DBLSCN(SAGA_VIDEO_DBLSCAN_Y);
        Printf(", DoubleY");
    }
    Printf("\n");

    Write32(SAGA_VIDEO_PLANEPTR, (ULONG)here);
    Write16(SAGA_VIDEO_WIDTH, width);
    Write16(SAGA_VIDEO_HEIGHT, height);
    Write16(SAGA_VIDEO_MODE, mode);

    /* Fill CLUT with a grayscale */
    for (i  = 0; i < 256; i++)
        Write32(SAGA_VIDEO_CLUT(i), 0x010101 * i);

    /* Clear 4M of ram */
    for (i = 0; i < SAGA_VIDEO_MEMSIZE; i += sizeof(ULONG))
        Write32((ULONG)here + i, 0);

    /* Fill visible video memory with a 8x8 grid */
    for (gy = 0; gy < height/8; gy++) {
        for (y = 0; y < 8; y++) {
            UBYTE color = gy;
            for (gx = 0; gx < width/8; gx++) {
                color++;
                for (x = 0; x < 8*bpp; x++, here++)
                    Write8((ULONG)here, color);
            }
            here += padding;
        }
    }

    FreeMem(here, SAGA_VIDEO_MEMSIZE);

    return RETURN_OK;

    AROS_SHCOMMAND_EXIT
}

/* vim: set shiftwidth=4 expandtab:  */
