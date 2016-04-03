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

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x)   (sizeof(x)/sizeof((x)[0]))
#endif

struct Library *SysBase, *DOSBase;

static void setBases(struct ExecBase *pSysBase, struct DosLibrary *pDOSBase)
{
    SysBase = (struct Library *)pSysBase;
    DOSBase = (struct Library *)pDOSBase;
}

int kprintf(const char *format, ...)
{
    char buffer[1024];
    const ULONG m68k_string_sprintf = 0x16c04e75;

    RawDoFmt(format, &format+1, (VOID_FUNC)&m68k_string_sprintf, buffer);
    Printf("%s", buffer);

    return strlen(buffer);
}

ULONG write_buffer[512 * 16 / sizeof(ULONG)];
ULONG read_buffer[512 * 16 / sizeof(ULONG)];

AROS_SH4H(SDDiag , 0.2,                 "SAGA SD Diagnostic\n",
AROS_SHAH(ULONG *  , ,IOBASE,/K/N,  0 ,  "SD IO Base\n"),
AROS_SHAH(ULONG *  , ,PATTERN,/K/N,  0 ,  "Pattern for WRITE testing\n"),
AROS_SHAH(BOOL     , ,READ,/S,    FALSE, "Perform READ test to last 16 blocks\n"),
AROS_SHAH(BOOL     , ,WRITE,/S,    FALSE, "Perform WRITE test to last 16 blocks\n")
)
{
    AROS_SHCOMMAND_INIT

    ULONG iobase = SHArg(IOBASE) ? *SHArg(IOBASE) : SAGA_SD_BASE;
    ULONG pattern = SHArg(PATTERN) ? *SHArg(PATTERN) : 0x5af00000;
    BOOL readTest = SHArg(READ);
    BOOL writeTest = SHArg(WRITE);
    struct sdcmd_info info;
    int i, test_shift;
    ULONG test_block;
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
        return RETURN_FAIL; 
    }

    test_block = info.blocks - 16;
    test_shift = (info.ocr & SDOCRF_HCS) ? 0 : 9;

    if (writeTest) {
        for (i = 0; i < ARRAY_SIZE(write_buffer); i++)
            write_buffer[i] = pattern + i;

        Printf("WRITE Test: %ld bytes, at block %ld: ",
                512, test_block);

        err = sdcmd_write_block(iobase, test_block << test_shift, (UBYTE *)&write_buffer[0]);
        if (err) {
            Printf("FAILED WRITE, err=0x%02lx\n", err);
            return RETURN_FAIL;
        }

        err = sdcmd_read_block(iobase, test_block << test_shift, (UBYTE *)&read_buffer[0]);
        if (err) {
            Printf("FAILED READBACK, err=0x%02lx\n", err);
            return RETURN_FAIL;
        }
       
        err = 0;
        for (i = 0; i < 512/sizeof(ULONG); i++) {
            if (read_buffer[i] != write_buffer[i]) {
                if (!err) {
                    Printf("FAILED DATA:\n");
                }
                err++;
                Printf("$%04lx: $%08lx [expected $%08lx]\n", i*sizeof(ULONG),
                        write_buffer[i], read_buffer[i]);
            }
        }
        if (err)
            return RETURN_FAIL;
        Printf("PASSED\n");

        Printf("WRITE Test: %ld bytes, at block %ld: ",
                512*15, test_block);

        err = sdcmd_write_blocks(iobase, (test_block+1) << test_shift, (UBYTE *)&write_buffer[512/sizeof(ULONG)], 15);
        if (err) {
            Printf("FAILED WRITE, err=0x%02lx\n", err);
            return RETURN_FAIL;
        }

        err = sdcmd_read_blocks(iobase, (test_block+1) << test_shift, (UBYTE *)&read_buffer[512/sizeof(ULONG)], 15);
        if (err) {
            Printf("FAILED READBACK, err=0x%02lx\n", err);
            return RETURN_FAIL;
        }
       
        err = 0;
        for (i = 0; i < 15 * 512/sizeof(ULONG); i++) {
            if (read_buffer[512/sizeof(ULONG) + i] !=
                write_buffer[512/sizeof(ULONG) + i]) {
                if (!err) {
                    Printf("FAILED DATA:\n");
                }
                err++;
                Printf("$%04lx: $%08lx [expected $%08lx]\n", i*sizeof(ULONG),
                        write_buffer[i], read_buffer[i]);
            }
        }
        if (err)
            return RETURN_FAIL;

        Printf("PASSED\n");
    }

    if (readTest) {
    }

    return RETURN_OK;

    AROS_SHCOMMAND_EXIT
}

/* vim: set shiftwidth=4 expandtab:  */
