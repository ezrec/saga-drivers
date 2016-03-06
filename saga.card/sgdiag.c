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

#include "saga_intern.h"

AROS_SH6H(SDDiag , 0.1,                 "SAGA Graphic Diagnostic\n",
AROS_SHAH(ULONG *  , W= ,WIDTH,/K/N,    0 , "Width\n"),
AROS_SHAH(ULONG *  , H= ,HEIGHT,/K/N,   0 , "Height\n"),
AROS_SHAH(ULONG *  , F= ,FORMAT,/K/N,   0 , "Format (0-5)\n"),
AROS_SHAH(ULONG *  , P= ,PADDING,/K/N,  0 , "Padding\n"),
AROS_SHAH(BOOL     , X= ,XDOUBLE,/S, FALSE, "Double X\n"),
AROS_SHAH(BOOL     , Y= ,YDOUBLE,/S, FALSE, "Double Y\n"))
{
    AROS_SHCOMMAND_INIT

    ULONG width = SHArg(WIDTH) ? *SHArg(WIDTH) : 640;
    ULONG height= SHArg(HEIGHT) ? *SHArg(HEIGHT) : 480;
    ULONG mode  = SHArg(FORMAT) ? *SHArg(FORMAT) : SAGA_VIDEO_FORMAT_RGB16;
    BOOL  dx    = SHArg(XDOUBLE);
    BOOL  dy    = SHArg(YDOUBLE);
    ULONG padding = SHArg(PADDING) ? *SHArg(PADDING) : 0;
    int bpp = (mode == SAGA_VIDEO_FORMAT_CLUT8) ? 1 :
              (mode == SAGA_VIDEO_FORMAT_RGB16) ? 2 :
              (mode == SAGA_VIDEO_FORMAT_RGB15) ? 2 :
              (mode == SAGA_VIDEO_FORMAT_RGB24) ? 3 :
              (mode == SAGA_VIDEO_FORMAT_RGB32) ? 4 : 1;
    int i, gx, gy, x, y;
    ULONG here = SAGA_VIDEO_MEMBASE;

    /* Set up video hardware */
    Printf("%ldx%ld, Format %ld", width, height, mode);
    if (dx) {
        mode |= SAGA_VIDEO_MODE_DBLSCN(SAGA_VIDEO_DBLSCAN_X);
        Printf(", DoubleX");
    }
    if (dy) {
        mode |= SAGA_VIDEO_MODE_DBLSCN(SAGA_VIDEO_DBLSCAN_Y);
        Printf(", DoubleY");
    }
    Printf("\n");

    Write32(SAGA_VIDEO_PLANEPTR, SAGA_VIDEO_MEMBASE);
    Write16(SAGA_VIDEO_WIDTH, width);
    Write16(SAGA_VIDEO_HEIGHT, height);
    Write16(SAGA_VIDEO_MODE, mode);

    /* Fill CLUT with a grayscale */
    for (i  = 0; i < 256; i++)
        Write32(SAGA_VIDEO_CLUT(i), 0x010101 * i);

    /* Clear 4M of ram */
    for (i = 0; i < SAGA_VIDEO_MEMSIZE; i += sizeof(ULONG))
        Write32(SAGA_VIDEO_MEMBASE + i, 0);

    /* Fill visible video memory with a 8x8 grid */
    for (gy = 0; gy < height/8; gy++) {
        for (y = 0; y < 8; y++) {
            UBYTE color = gy;
            for (gx = 0; gx < width/8; gx++) {
                color++;
                for (x = 0; x < 8*bpp; x++, here++)
                    Write8(here, color);
            }
            here += padding;
        }
    }

    return RETURN_OK;

    AROS_SHCOMMAND_EXIT
}

/* vim: set shiftwidth=4 expandtab:  */
