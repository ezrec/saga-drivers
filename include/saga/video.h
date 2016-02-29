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


#ifndef SAGA_VIDEO_H
#define SAGA_VIDEO_H

#define SAGA_VIDEO_FORMAT_OFF        0
#define SAGA_VIDEO_FORMAT_CLUT8      1
#define SAGA_VIDEO_FORMAT_RGB16      2       /* R5|G6|B5 */
#define SAGA_VIDEO_FORMAT_RGB15      3       /* -|R5|G5|B5 */
#define SAGA_VIDEO_FORMAT_RGB24      4       /* R8|G8|B8 */
#define SAGA_VIDEO_FORMAT_RGB32      5       /* -|R8|G8|B8 */

#define SAGA_VIDEO_DBLSCAN_OFF       0
#define SAGA_VIDEO_DBLSCAN_X         1
#define SAGA_VIDEO_DBLSCAN_Y         2
#define SAGA_VIDEO_DBLSCAN_XY        (SAGA_VIDEO_DBLSCAN_X | SAGA_VIDEO_DBLSCAN_Y)

#define SAGA_VIDEO_MEMBASE   0x0FB00000    /* Location of video ram */
#define SAGA_VIDEO_MEMSIZE   0x00400000    /* Default 4M video ram */

#define SAGA_VIDEO_PLANEPTR  0x00DFF1EC    /* 32-bit pointer to video ram */
#define SAGA_VIDEO_WIDTH     0x00DFF1F0    /* 16-bit width */
#define SAGA_VIDEO_HEIGHT    0x00DFF1F2    /* 16-bit height */
#define SAGA_VIDEO_MODE      0x00DFF1F4    /* 16-bit mode */
#define   SAGA_VIDEO_MODE_FORMAT(x)  (((x) & 0xff) << 0)
#define   SAGA_VIDEO_MODE_DBLSCN(x)  (((x) & 0xff) << 8)

#define SAGA_VIDEO_CLUT(x)   (0x00DFF400 + (((x) & 0xFF) << 2))

#endif /* SAGA_VIDEO_H */
/* vim: set shiftwidth=4 expandtab:  */
