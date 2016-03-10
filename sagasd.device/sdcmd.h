/* Raw SD interface
 *
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

#ifndef SDCMD_H
#define SDCMD_H

#include <exec/types.h>

#define SDSIZ_BLOCK        512  /* Block size in bytes */

#define SDCMD_TIMEOUT      100
#define SDCMD_IDLE_RETRY   100

#define SDERRF_TIMEOUT  (1 << 7)
#define SDERRF_PARAM    (1 << 6)
#define SDERRF_ADDRESS  (1 << 5)
#define SDERRF_ERASESEQ (1 << 4)
#define SDERRF_CRC      (1 << 3)
#define SDERRF_ILLEGAL  (1 << 2)
#define SDERRF_ERASERES (1 << 1)
#define SDERRF_IDLE     (1 << 0)

#define SDOCRF_HCS         (1UL << 30)   /* Uses block, not byte, addressing */
#define SDOCRF_MAX_3_6V     (1UL << 23)
#define SDOCRF_MAX_3_5V     (1UL << 22)
#define SDOCRF_MAX_3_4V     (1UL << 21)
#define SDOCRF_MAX_3_3V     (1UL << 20)
#define SDOCRF_MAX_3_2V     (1UL << 19)
#define SDOCRF_MAX_3_1V     (1UL << 18)
#define SDOCRF_MAX_3_0V     (1UL << 17)
#define SDOCRF_MAX_2_9V     (1UL << 16)
#define SDOCRF_MAX_2_8V     (1UL << 15)

/* Base commands */
#define SDCMD_GO_IDLE_STATE             0
#define SDCMD_SEND_OP_COND              1
#define SDCMD_SEND_IF_COND              8
#define SDCMD_SEND_CSD                  9
#define SDCMD_SEND_CID                  10
#define SDCMD_STOP_TRANSMISSION         12
#define SDCMD_SET_BLOCKLEN              16
#define SDCMD_READ_SINGLE_BLOCK         17
#define SDCMD_READ_MULTIPLE_BLOCK       18
#define SDCMD_WRITE_SINGLE_BLOCK        24
#define SDCMD_WRITE_MULTIPLE_BLOCK      25
#define SDCMD_APP_CMD                   55
#define SDCMD_READ_OCR                  58
#define SDCMD_CRC_ON_OFF                59

#define SDCMD_VALID                     0x40


/* App commands */
#define SDCMD_SD_SEND_OP_COND   41

/* SD Data Response Status */
#define SDDRS_VALID_MASK        0x11
#define SDDRS_VALID             0x01
#define SDDRS_CODE_MASK         0x0e
#define SDDRS_CODE_ACCEPTED     0x04
#define SDDRS_CODE_REJECT_CRC   0x0a
#define SDDRS_CODE_REJECT_WRITE 0x0c

/* Raw SD card interface */

VOID sdcmd_send(ULONG iobase, UBYTE cmd, ULONG arg);
UBYTE sdcmd_asend(ULONG iobase, UBYTE acmd, ULONG arg);

UBYTE sdcmd_r1(ULONG iobase);
UBYTE sdcmd_r2(ULONG iobase, UBYTE *r2);
UBYTE sdcmd_r3(ULONG iobase, ULONG *ocr);
UBYTE sdcmd_r7(ULONG iobase, ULONG *ifcond);

UBYTE sdcmd_read_packet(ULONG iobase, UBYTE *buff, int len);
UBYTE sdcmd_write_packet(ULONG iobase, UBYTE token, CONST UBYTE *buff, int len);
UBYTE sdcmd_stop_transmission(ULONG iobase);

/* Disk-like interface */
struct sdcmd_info {
    ULONG block_size;
    ULONG blocks;
    BOOL  read_only;

    /* Raw OCR, CSD and CID data */
    ULONG ocr;
    UBYTE csd[16];
    UBYTE cid[16];
};

/* Is there something in the SD slot?
 */
BOOL  sdcmd_present(ULONG iobase);

/* Detect and initialize the SD device
 */
UBYTE sdcmd_detect(ULONG iobase, struct sdcmd_info *info);

/* NOTE: Depending on SDOCRF_HCS, you will need to use either
 *       SDOCRF_HCS == 0   => addr is in bytes
 *     or
 *       SDOCRF_HCS == 1   => addr is in blocks
 */
UBYTE sdcmd_read_block(ULONG iobase, ULONG addr, UBYTE *buff);
UBYTE sdcmd_read_blocks(ULONG iobase, ULONG addr, UBYTE *buff, int blocks);

UBYTE sdcmd_write_block(ULONG iobase, ULONG addr, CONST UBYTE *buff);
UBYTE sdcmd_write_blocks(ULONG iobase, ULONG addr, CONST UBYTE *buff, int blocks);

#endif /* SDCMD_H */

/* vim: set shiftwidth=4 expandtab:  */
