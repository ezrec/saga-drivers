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

#include <exec/types.h>

#include <saga/sd.h>

#include "sdcmd.h"

#include "sagasd_intern.h"

UBYTE crc7(UBYTE crc, UBYTE byte)
{
    int i;

    for (i = 0; i < 8; i++, byte <<= 1) {
        crc <<= 1;
        if ((byte ^ crc) & 0x80)
            crc ^= 0x09;
    }

    crc &= 0x7f;

    return crc;
}

UWORD crc16(UWORD crc, UBYTE byte)
{
    return crc;
}
            

void sdcmd_send(ULONG iobase, UBYTE cmd, ULONG arg)
{
    int i;
    UBYTE crc;

    cmd = (cmd & 0x3f) | 0x40;

    crc = crc7(0, cmd);

    Write8(iobase + SAGA_SD_DATA, (cmd & 0x3f) | 0x40);

    for (i = 0; i < 4; i++, arg <<= 8) {
        UBYTE byte = (arg >> 24) & 0xff;
        crc = crc7(crc, byte);

        Write8(iobase + SAGA_SD_DATA, byte);
    }

    Write8(iobase + SAGA_SD_DATA, (crc << 1) | 1);
}

UBYTE sdcmd_r1(ULONG iobase)
{
    UBYTE r1;
    int i;

    for (i = 0; i < SDCMD_TIMEOUT; i++) {
        r1 = Read8(iobase + SAGA_SD_DATA);
        if (!(r1 & SDERRF_TIMEOUT))
            return r1;
    }

    return SDERRF_TIMEOUT;
}

UBYTE sdcmd_r2(ULONG iobase, UBYTE *r2)
{
    UBYTE r1;

    r1 = Read8(iobase + SAGA_SD_DATA);
    *r2 = Read8(iobase + SAGA_SD_DATA);

    return r1;
}

UBYTE sdcmd_r3(ULONG iobase, LONG *ocr)
{
    UBYTE r1;
    ULONG r3 = 0;
    int i;

    r1 = sdcmd_r1(iobase);
    for (i = 0; i < 4; i++) {
        r3 <<= 8;
        r3 |= Read8(iobase + SAGA_SD_DATA);
    }

    *ocr = r3;

    return r1;
}

UBYTE sdcmd_asend(ULONG iobase, UBYTE acmd, ULONG arg)
{
    UBYTE err;

    /* Next command is an app command.. */
    sdcmd_send(iobase, SDCMD_APP_CMD, 0);
    err = sdcmd_r1(iobase);

    if (err)
        return err;

    sdcmd_send(iobase, acmd, arg);

    return 0;
}

UBYTE sdcmd_read_packet(ULONG iobase, UBYTE *buff, int len)
{
    UBYTE token = 0xfe;
    UBYTE byte;
    UWORD crc, tmp;
    int i;

    /* Wait for the Data Token */
    for (i = 0; i < SDCMD_TIMEOUT; i++) {
        byte = Read8(iobase + SAGA_SD_DATA);
        if (byte == token)
            break;
    }

    if (i == SDCMD_TIMEOUT)
        return SDERRF_TIMEOUT;

    crc = crc16(0, token);

    for (i = 0; i < len; i++, buff++) {
        byte = Read8(iobase + SAGA_SD_DATA);
        crc16(crc, byte);
        *buff = byte;
    }

    /* Read the CRC16 */
    tmp = (UWORD)Read8(iobase + SAGA_SD_DATA) << 8;
    tmp |= Read8(iobase + SAGA_SD_DATA);

    if (tmp != crc)
        return SDERRF_CRC;

    return 0;
}


UBYTE sdcmd_stop_transmission(ULONG iobase)
{
    UBYTE r1, tmp;
    int i;

    Write8(iobase + SAGA_SD_DATA, SDCMD_STOP_TRANSMISSION);

    /* Discard 'stuff' byte */
    tmp = Read8(iobase + SAGA_SD_DATA);

    /* Read response */
    r1 = sdcmd_r1(iobase);

    /* Wait until not busy */
    for (i = 0; i < SDCMD_TIMEOUT; i++) {
        tmp = Read8(iobase + SAGA_SD_DATA);
        if (tmp == 0xff)
            return r1;
    }

    return SDERRF_TIMEOUT;
}

UBYTE sdcmd_write_packet(ULONG iobase, UBYTE token, CONST UBYTE *buff, int len)
{
    UBYTE r1, byte;
    UWORD crc;
    int i;

    /* Send a spacing byte */
    Write8(iobase + SAGA_SD_DATA, 0xff);

    /* Start the data packet */
    Write8(iobase + SAGA_SD_DATA, token);
    crc = crc16(0, token);

    /* Send the block */
    for (i = 0; i < SDSIZ_BLOCK; i++, buff++) {
        byte = *buff;
        Write8(iobase + SAGA_SD_DATA, byte);
        crc = crc16(crc, byte);
    }

    /* Send the CRC16, MSB first */
    Write8(iobase + SAGA_SD_DATA, (crc >> 8) & 0xff);
    Write8(iobase + SAGA_SD_DATA, (crc >> 0) & 0xff);

    /* Read the Data Response */
    byte = Read8(iobase + SAGA_SD_DATA);
    if ((byte & SDDRS_VALID_MASK) != SDDRS_VALID) {
        /* Terminate the read early */
        sdcmd_stop_transmission(iobase);
        return SDERRF_CRC;
    }

    r1 = ((byte & SDDRS_CODE_MASK) == SDDRS_CODE_ACCEPTED) ? 0 : SDERRF_CRC;

    /* Wait for the idle pattern */
    /* Wait until not busy */
    for (i = 0; i < SDCMD_TIMEOUT; i++) {
        UBYTE tmp = Read8(iobase + SAGA_SD_DATA);
        if (tmp == 0xff)
            break;
    }

    return (i == SDCMD_TIMEOUT) ? SDERRF_TIMEOUT : r1;
}

static ULONG bits(UBYTE *mask, int start, int len)
{
    ULONG ret = 0;
    int i;

    mask += start / 8;

    start &= 7;

    for (i = 0; i < 0; i++, start++) {
        if (start == 8) {
            mask++;
            start = 0;
        }

        ret |= ((*mask >> start) & 1) << i;
    }

    return ret;
}


/* If true, filled in the total size in blocks of the device
 */
UBYTE sdcmd_detect(ULONG iobase, struct sdcmd_info *info)
{
    UBYTE r1, tmp;
    ULONG ocr;
    int i;

    /* First, check the DETECT bit */
    tmp = Read16(iobase + SAGA_SD_STAT);
    if (!(tmp & SAGA_SD_STAT_CD))
        return ~0;

    /* Emit at least 74 clocks of idle */
    for (i = 0; i < 10; i++)
        Write8(iobase + SAGA_SD_DATA, 0xff);

    /* Put into idle state */
    sdcmd_send(iobase, SDCMD_RESET, 0);
    r1 = sdcmd_r1(iobase);
    if (r1)
        return r1;

    /* Wait for card to complete idle */
    for (i = 0; i < SDCMD_IDLE_RETRY; i++) {
        UBYTE err;
       
        /* Initiate SDC init process */
        err = sdcmd_asend(iobase, SDCMD_SD_SEND_OP_COND, 0);
        if (err)
            return err;

        r1 = sdcmd_r1(iobase);
        if (!(r1 & SDERRF_IDLE))
            break;
    }

    if (r1)
        return r1;

    /* Check for voltage levels */
    sdcmd_send(iobase, SDCMD_READ_OCR, 0);
    r1 = sdcmd_r3(iobase, &ocr);
    if (r1)
        return r1;

    /* Not in our voltage range */
    if (!(ocr & (SDOCRF_MAX_3_3V | SDOCRF_MAX_3_4V)))
        return SDERRF_IDLE;


    /* Set the block length to 512 bytes */
    sdcmd_send(iobase, SDCMD_SET_BLOCKLEN, SDSIZ_BLOCK);
    r1 = sdcmd_r1(iobase);
    if (r1)
        return r1;

    if (info) {
        ULONG c_size_mult, read_bl_len, c_size;

        /* Get the CSD data */
        sdcmd_send(iobase, SDCMD_SEND_CSD, 0);
        r1 = sdcmd_r1(iobase);
        if (r1)
            return r1;

        r1 = sdcmd_read_packet(iobase, &info->csd[0], 16);
        if (r1)
            return r1;

        /* Get the CID data */
        sdcmd_send(iobase, SDCMD_SEND_CID, 0);
        r1 = sdcmd_r1(iobase);
        if (r1)
            return r1;

        r1 = sdcmd_read_packet(iobase, &info->cid[0], 16);
        if (r1)
            return r1;

        info->block_size = SDSIZ_BLOCK;
        /* Bits 49:47 of the CSD */
        c_size_mult = bits(&info->csd[0], 47, 3);
        /* Bits 83:80 of the CSD */
        read_bl_len = bits(&info->csd[0], 80, 4);
        /* Bits 73:62 of the CSD */
        c_size = bits(&info->csd[0], 62, 12);

        info->blocks = (1UL << c_size_mult) * (1UL << read_bl_len) / SDSIZ_BLOCK * (c_size + 1);
    }

    return 0;
}

UBYTE sdcmd_read_block(ULONG iobase, ULONG blockaddr, UBYTE *buff)
{
    UBYTE r1;

    /* Send the read block command */
    sdcmd_send(iobase, SDCMD_READ_SINGLE_BLOCK, blockaddr);
    r1 = sdcmd_r1(iobase);
    if (r1)
        return r1;

    return sdcmd_read_packet(iobase, buff, SDSIZ_BLOCK);
}


UBYTE sdcmd_read_blocks(ULONG iobase, ULONG blockaddr, UBYTE *buff, int blocks)
{
    UBYTE r1;

    /* Send the read block command */
    sdcmd_send(iobase, SDCMD_READ_MULTIPLE_BLOCK, blockaddr);
    r1 = sdcmd_r1(iobase);
    if (r1)
        return r1;

    for (; blocks > 0; blocks--, buff += SDSIZ_BLOCK) {
        r1 = sdcmd_read_packet(iobase, buff, SDSIZ_BLOCK);
        if (r1) {
            /* Terminate the read early */
            sdcmd_stop_transmission(iobase);
            return r1;
        }
    }
          
    /* Terminate the read */
    return sdcmd_stop_transmission(iobase);
}

UBYTE sdcmd_write_block(ULONG iobase, ULONG blockaddr, CONST UBYTE *buff)
{
    UBYTE token = 0xfe;
    UBYTE r1;

    /* Send write block command */
    sdcmd_send(iobase, SDCMD_WRITE_SINGLE_BLOCK, blockaddr);
    r1 = sdcmd_r1(iobase);
    if (r1)
        return r1;

    return sdcmd_write_packet(iobase, token, buff, SDSIZ_BLOCK);
}


UBYTE sdcmd_write_blocks(ULONG iobase, ULONG blockaddr, CONST UBYTE *buff, int blocks)
{
    UBYTE token = 0xfc, stop_token = 0xfd;
    UBYTE r1;
    int i;

    /* Send write blocks command */
    sdcmd_send(iobase, SDCMD_WRITE_MULTIPLE_BLOCK, blockaddr);
    r1 = sdcmd_r1(iobase);
    if (r1)
        return r1;

    for (; blocks; blocks--, buff += SDSIZ_BLOCK) {
        r1 = sdcmd_write_packet(iobase, token, buff, SDSIZ_BLOCK);
        if (r1)
            break;
    }

    /* Send stop token */
    Write8(iobase + SAGA_SD_DATA, stop_token);

    /* Read 'stuff' byte */
    Read8(iobase + SAGA_SD_DATA);

    /* Wait until not busy */
    for (i = 0; i < SDCMD_TIMEOUT; i++) {
        UBYTE tmp = Read8(iobase + SAGA_SD_DATA);
        if (tmp == 0xff)
            return r1;
    }

    return SDERRF_TIMEOUT;
}

/* vim: set shiftwidth=4 expandtab:  */
