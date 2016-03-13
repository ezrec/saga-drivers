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

#include <aros/debug.h>

#include <exec/memory.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/alib.h>

#include <aros/shcommands.h>

#include "sdcmd.h"

#include <saga/sd.h>

struct Library *SysBase, *DOSBase;

static void setBases(struct ExecBase *pSysBase, struct DosLibrary *pDOSBase)
{
    SysBase = (struct Library *)pSysBase;
    DOSBase = (struct Library *)pDOSBase;
}

int kprintf(const char *format, ...)
{
    UBYTE buffer[1024];
    const ULONG m68k_string_sprintf = 0x16c04e75;

    RawDoFmt(format, &format+1, (VOID_FUNC)&m68k_string_sprintf, buffer);
    Printf("%s", buffer);

    return strlen(buffer);
}

AROS_SH1H(SDDiag , 0.1,                 "SAGA SD Diagnostic\n",
AROS_SHAH(ULONG *  , ,IOBASE,/K/N,  0 ,  "SD IO Base\n"))
{
    AROS_SHCOMMAND_INIT

    ULONG iobase = SHArg(IOBASE) ? *SHArg(IOBASE) : SAGA_SD_BASE;
    struct sdcmd_info info;
    int i;
    UBYTE err;

    setBases(SysBase, DOSBase);

    err = sdcmd_detect(iobase, &info);

    if (!err) {
        Printf("SD Card Detected on $0x%lx:\n", iobase);
        Printf("Block Size: %ld\n", info.block_size);
        Printf("Blocks: %ld\n", info.blocks);
        Printf("Capacity: ");
        if (info.blocks < 1024*2) {
            Printf("%ldK\n", (info.blocks + 1) / 2);
        } else if (info.blocks < 1024*1024*2) {
            Printf("%ldM\n", (info.blocks + 1024*2-1) / 1024 / 2);
        } else {
            Printf("%ldG\n", (info.blocks + 1024*1024*2-1) / 1024 / 1024 / 2);
        }

        Printf("OCR: %08lx\n", info.ocr);
        Printf("CID:");
        for (i = 0; i < 16; i++)
            Printf(" %02lx", info.cid[i]);
        Printf("\n");
        Printf("CSD:");
        for (i = 0; i < 16; i++)
            Printf(" %02lx", info.csd[i]);
        Printf("\n");
    } else {
        Printf("SD Card Detection Error: %lx\n", err);
    }

    return RETURN_OK;

    AROS_SHCOMMAND_EXIT
}

/* vim: set shiftwidth=4 expandtab:  */
