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

#include <picasso96/card.h>

/* Originally adapted from AROS uaegfx.hidd timing data
 */
const struct ModeInfo modeline_vesa_entry[] = {
    { 
        .Node = { .ln_Name = "VESA: 320 x 240 60Hz" },
        .Width = 320, .Height = 240,
        .Flags = GMF_HPOLARITY | GMF_VPOLARITY | GMF_DOUBLESCAN | GMF_DOUBLEVERTICAL,
        .HorTotal = 408,        .VerTotal = 262,
        .HorBlankSize = 0,      .VerBlankSize = 0,
        .HorSyncStart = 32,     .VerSyncStart = 5,
        .HorSyncSize  = 24,     .VerSyncSize  = 1,
        .Numerator = 60,        /* Refresh rate */
    },
    {
        .Node = { .ln_Name = "VESA: 640 x 480 60Hz" },
        .Width = 640, .Height = 480,
        .Flags = GMF_HPOLARITY | GMF_VPOLARITY,
        .HorTotal = 800,        .VerTotal = 525,
        .HorBlankSize = 0,      .VerBlankSize = 0,
        .HorSyncStart = 40,     .VerSyncStart = 1,
        .HorSyncSize  = 88,     .VerSyncSize  = 2,
        .Numerator = 60,        /* Refresh rate */
    },
    {
        .Node = { .ln_Name = "VESA: 800 x 600 60Hz" },
        .Width = 800, .Height = 600,
        .Flags = 0,
        .HorTotal = 1024,       .VerTotal = 625,
        .HorBlankSize = 0,      .VerBlankSize = 0,
        .HorSyncStart = 56,     .VerSyncStart = 1,
        .HorSyncSize  = 40,     .VerSyncSize  = 2,
        .Numerator = 60,        /* Refresh rate */
    },
    {
        .Node = { .ln_Name = "VESA:1024 x 768 60Hz" },
        .Width = 1024, .Height = 768,
        .Flags = 0,
        .HorTotal = 1336,       .VerTotal = 800,
        .HorBlankSize = 0,      .VerBlankSize = 0,
        .HorSyncStart = 48,     .VerSyncStart = 4,
        .HorSyncSize  = 112,    .VerSyncSize  = 5,
        .Numerator = 60,        /* Refresh rate */
    },
};

const int modeline_vesa_entries = sizeof(modeline_vesa_entry)/sizeof(modeline_vesa_entry[0]);

/* vim: set shiftwidth=4 expandtab:  */
