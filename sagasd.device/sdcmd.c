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

#include <string.h>     // For memset

#include <exec/types.h>

#include <saga/sd.h>

#include "sdcmd.h"

#include "common.h"

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
    crc = ((UBYTE)(crc >> 8)) | ((UBYTE)crc << 8);
    crc ^= byte;
    crc ^= (UBYTE)(crc & 0xff) >> 4;
    crc ^= crc << 12;
    crc ^= (crc & 0xff) << 5;

    return crc;
}

static UBYTE sdcmd_out(ULONG iobase, UBYTE data)
{
    Write8(iobase + SAGA_SD_DATA, data);

    return (UBYTE)Read8(iobase + SAGA_SD_DATA);
}

static UBYTE sdcmd_in(ULONG iobase)
{
    return sdcmd_out(iobase, 0xff);
}

            
BOOL sdcmd_present(ULONG iobase)
{
    UBYTE tmp;
    
    tmp = Read16(iobase + SAGA_SD_STAT);
    return (tmp & SAGA_SD_STAT_NCD) ? FALSE : TRUE;
}

VOID sdcmd_select(ULONG iobase, BOOL cs)
{
    UWORD val;

    val = SAGA_SD_CTL_NCS(cs ? 0 : 1);
    Write16(iobase + SAGA_SD_CTL, val);
}

void sdcmd_send(ULONG iobase, UBYTE cmd, ULONG arg)
{
    int i;
    UBYTE crc;

    sdcmd_select(iobase, TRUE);

    cmd = (cmd & 0x3f) | SDCMD_VALID;

    crc = crc7(0, cmd);

    sdcmd_out(iobase, (cmd & 0x3f) | 0x40);

    for (i = 0; i < 4; i++, arg <<= 8) {
        UBYTE byte = (arg >> 24) & 0xff;
        crc = crc7(crc, byte);

        sdcmd_out(iobase, byte);
    }

    sdcmd_out(iobase, (crc << 1) | 1);
}

static UBYTE sdcmd_r1a(ULONG iobase)
{
    UBYTE r1;
    int i;

    for (i = 0; i < SDCMD_TIMEOUT; i++) {
        r1 = sdcmd_in(iobase);
        if (!(r1 & SDERRF_TIMEOUT))
            return r1;
    }

    return SDERRF_TIMEOUT;
}

#define R1_OK(x)        (((x) & ~SDERRF_IDLE) == 0)

UBYTE sdcmd_r1(ULONG iobase)
{
    UBYTE r1;

    r1 = sdcmd_r1a(iobase);
    sdcmd_select(iobase, FALSE);

    return r1;
}

UBYTE sdcmd_r2(ULONG iobase, UBYTE *r2)
{
    UBYTE r1;

    r1 = sdcmd_r1a(iobase);
    if (R1_OK(r1))
        *r2 = sdcmd_in(iobase);

    sdcmd_select(iobase, FALSE);

    return r1;
}

UBYTE sdcmd_r3(ULONG iobase, ULONG *ocr)
{
    UBYTE r1;
    ULONG r3 = 0;
    int i;

    r1 = sdcmd_r1a(iobase);
    if (R1_OK(r1)) {
        for (i = 0; i < 4; i++) {
            r3 <<= 8;
            r3 |= sdcmd_in(iobase);
        }
    }

    sdcmd_select(iobase, FALSE);

    debug("r3=0x%08lx\n", r3);
    *ocr = r3;

    return r1;
}

UBYTE sdcmd_r7(ULONG iobase, ULONG *ifcond)
{
    UBYTE r1;
    ULONG r7 = 0;
    int i;

    r1 = sdcmd_r1a(iobase);
    if (R1_OK(r1)) {
        for (i = 0; i < 4; i++) {
            r7 <<= 8;
            r7 |= sdcmd_in(iobase);
        }
    }

    sdcmd_select(iobase, FALSE);

    debug("r7=0x%08lx\n", r7);
    *ifcond = r7;

    return r1;
}

UBYTE sdcmd_asend(ULONG iobase, UBYTE acmd, ULONG arg)
{
    UBYTE r1;

    /* Next command is an app command.. */
    sdcmd_send(iobase, SDCMD_APP_CMD, 0);

    r1 = sdcmd_r1a(iobase);
    if (!R1_OK(r1)) {
        sdcmd_select(iobase, FALSE);
        return r1;
    }

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
        byte = sdcmd_in(iobase);
        if (byte == token)
            break;
    }

    if (i == SDCMD_TIMEOUT) {
        sdcmd_select(iobase, FALSE);
        return SDERRF_TIMEOUT;
    }

    crc = 0;

    for (i = 0; i < len; i++, buff++) {
        byte = sdcmd_in(iobase);
        crc = crc16(crc, byte);
        *buff = byte;
    }

    /* Read the CRC16 */
    tmp = (UWORD)sdcmd_in(iobase) << 8;
    tmp |= sdcmd_in(iobase);

    if (tmp != crc)
        return SDERRF_CRC;

    return 0;
}


UBYTE sdcmd_stop_transmission(ULONG iobase)
{
    UBYTE r1, tmp;
    int i;

    sdcmd_send(iobase, SDCMD_STOP_TRANSMISSION, 0);

    /* Read the stuff byte */
    sdcmd_in(iobase);

    /* Read response */
    r1 = sdcmd_r1a(iobase);
    debug("r1=$%02lx", r1);

    /* But we ignore it... There is no circumstance
     * where it will have meaningful information,
     * and some cards put junk data in the R1
     * response.
     */
    r1 = 0;

    /* Wait until not busy */
    for (i = 0; i < SDCMD_TIMEOUT; i++) {
        tmp = sdcmd_in(iobase);
        debug("tmp=$%02lx", tmp);
        if (tmp == 0xff)
           break;
    }

    sdcmd_select(iobase, FALSE);

    return (i == SDCMD_TIMEOUT) ? SDERRF_TIMEOUT : r1;
}

UBYTE sdcmd_write_packet(ULONG iobase, UBYTE token, CONST UBYTE *buff, int len)
{
    UBYTE r1, byte;
    UWORD crc;
    int i;

    /* Send a spacing byte */
    sdcmd_out(iobase, 0xff);

    /* Start the data packet */
    sdcmd_out(iobase, token);
    crc = 0;

    /* Send the block */
    for (i = 0; i < SDSIZ_BLOCK; i++, buff++) {
        byte = *buff;
        sdcmd_out(iobase, byte);
        crc = crc16(crc, byte);
    }

    /* Send the CRC16, MSB first */
    sdcmd_out(iobase, (crc >> 8) & 0xff);
    sdcmd_out(iobase, (crc >> 0) & 0xff);

    /* Read the Data Response */
    byte = sdcmd_in(iobase);
    if ((byte & SDDRS_VALID_MASK) != SDDRS_VALID) {
        /* Terminate the read early */
        sdcmd_stop_transmission(iobase);
        return SDERRF_CRC;
    }

    r1 = ((byte & SDDRS_CODE_MASK) == SDDRS_CODE_ACCEPTED) ? 0 : SDERRF_CRC;
    debug("byte=$%02lx, r1=$%02lx", byte, r1);

    /* Wait for the idle pattern */
    /* Wait until not busy */
    for (i = 0; i < SDCMD_TIMEOUT; i++) {
        UBYTE tmp = sdcmd_in(iobase);
        debug("ptmp = $%02lx", tmp);
        if (tmp == 0xff)
            break;
    }

    return (i == SDCMD_TIMEOUT) ? SDERRF_TIMEOUT : r1;
}

static ULONG bits(UBYTE *mask, int start, int len)
{
    ULONG ret = 0;
    int i;

    mask -= start / 8;

    start &= 7;

    for (i = 0; i < len; i++, start++) {
        if (start == 8) {
            mask--;
            start = 0;
        }

        ret |= ((*mask >> start) & 1) << i;
    }

    return ret;
}

/* If non-zero, filled in the total size in blocks of the device
 */
UBYTE sdcmd_detect(ULONG iobase, struct sdcmd_info *info)
{
    UBYTE r1;
    ULONG r7;
    int i;

    memset(info, 0, sizeof(*info));

    /* First, check the DETECT bit */
    if (!sdcmd_present(iobase))
        return ~0;

    /* Emit at least 74 clocks of idle */
    sdcmd_select(iobase, TRUE);
    for (i = 0; i < 10; i++)
        sdcmd_out(iobase, 0xff);
    sdcmd_select(iobase, FALSE);

    /* Stuff two idle bytes while deasserted */
    sdcmd_out(iobase, 0xff);
    sdcmd_out(iobase, 0xff);

    /* Put into idle state */
    sdcmd_send(iobase, SDCMD_GO_IDLE_STATE, 0);
    r1 = sdcmd_r1(iobase);
    /* It's ok (and expected) that we are in IDLE state */
    r1 &= ~SDERRF_IDLE;
    if (r1)
        return r1;

    /* Do SHDC detection during idle */
    sdcmd_send(iobase, SDCMD_SEND_IF_COND, 0x000001aa);
    r1 = sdcmd_r7(iobase, &r7);
    /* It's ok (and expected) that we are in IDLE state */
    r1 &= ~SDERRF_IDLE;
    if (!r1) {
        /* Looks like a SDHC card? */
        if ((r7 & 0x000001ff) == 0x000001aa) {
            /* Set HCS (SDHC) mode */
            sdcmd_asend(iobase, SDCMD_SD_SEND_OP_COND, SDOCRF_HCS);
            r1 = sdcmd_r1(iobase);
            /* It's ok (and expected) that we are in IDLE state */
            r1 &= ~SDERRF_IDLE;
            if (r1)
                return r1;
        }
    }

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

    debug("r1=0x%lx", r1);
    if (r1)
        return r1;

    /* Check for voltage levels */
    sdcmd_send(iobase, SDCMD_READ_OCR, 0);
    r1 = sdcmd_r3(iobase, &info->ocr);
    debug("r1=0x%lx", r1);
    if (r1)
        return r1;

    /* Not in our voltage range */
    debug("ocr=0x%08lx (vs 0x%08lx)", info->ocr, SDOCRF_MAX_3_3V | SDOCRF_MAX_3_4V);
    if (!(info->ocr & (SDOCRF_MAX_3_3V | SDOCRF_MAX_3_4V)))
        return SDERRF_IDLE;

    if (info) {
        ULONG c_size_mult, read_bl_len, c_size;

        /* Get the CSD data */
        sdcmd_send(iobase, SDCMD_SEND_CSD, 0);
        r1 = sdcmd_r1a(iobase);
        debug("r1=0x%lx", r1);
        if (r1)
            goto exit;

        r1 = sdcmd_read_packet(iobase, &info->csd[0], 16);
        debug("r1=0x%lx", r1);
        if (r1)
            goto exit;

        debug("csd=%02lx%02lx%02lx%02lx-%02lx%02lx%02lx%02lx-%02lx%02lx%02lx%02lx-%02lx%02lx%02lx%02lx",
                info->csd[0], info->csd[1], info->csd[2], info->csd[3],
                info->csd[4], info->csd[5], info->csd[6], info->csd[7],
                info->csd[8], info->csd[9], info->csd[10], info->csd[11],
                info->csd[12], info->csd[13], info->csd[14], info->csd[15]);

        /* Get the CID data */
        sdcmd_send(iobase, SDCMD_SEND_CID, 0);
        r1 = sdcmd_r1a(iobase);
        debug("r1=%d", r1);
        if (r1)
            goto exit;

        r1 = sdcmd_read_packet(iobase, &info->cid[0], 16);
        sdcmd_select(iobase, FALSE);

        debug("r1=0x%lx", r1);
        if (r1)
            return r1;
        debug("cid=%02lx%02lx%02lx%02lx-%02lx%02lx%02lx%02lx-%02lx%02lx%02lx%02lx-%02lx%02lx%02lx%02lx",
                info->cid[0], info->cid[1], info->cid[2], info->cid[3],
                info->cid[4], info->cid[5], info->cid[6], info->cid[7],
                info->cid[8], info->cid[9], info->cid[10], info->cid[11],
                info->cid[12], info->cid[13], info->cid[14], info->cid[15]);

        info->block_size = SDSIZ_BLOCK;

        if (info->ocr & SDOCRF_HCS) {
            /* SDHC calculation */

            /* Bits 68:48 of the CSD */
            c_size = bits(&info->csd[15], 48, 20);

            debug("c_size=%ld", c_size);

            info->blocks = (c_size + 1) * 1024;
        } else {
            /* SD calculation */
            /* Bits 49:47 of the CSD */
            c_size_mult = bits(&info->csd[15], 47, 3);
            /* Bits 83:80 of the CSD */
            read_bl_len = bits(&info->csd[15], 80, 4);
            /* Bits 73:62 of the CSD */
            c_size = bits(&info->csd[15], 62, 12);

            debug("c_size_mult=%ld, read_bl_len=%ld, c_size=%ld", c_size_mult, read_bl_len, c_size);

            info->blocks = (1UL << (c_size_mult + read_bl_len + 2 - 9)) * (c_size + 1);

            /* Set block size */
            sdcmd_send(iobase, SDCMD_SET_BLOCKLEN, info->block_size);
            r1 = sdcmd_r1(iobase);
            if (r1)
                return r1;
        }
        debug("blocks=%ld", info->blocks);
    }

exit:
    sdcmd_select(iobase, FALSE);
    return r1;
}

UBYTE sdcmd_read_block(ULONG iobase, ULONG addr, UBYTE *buff)
{
    UBYTE r1;

    /* Send the read block command */
    sdcmd_send(iobase, SDCMD_READ_SINGLE_BLOCK, addr);
    r1 = sdcmd_r1a(iobase);
    if (!r1)
        r1 = sdcmd_read_packet(iobase, buff, SDSIZ_BLOCK);

    sdcmd_select(iobase, FALSE);
    return r1;
}


UBYTE sdcmd_read_blocks(ULONG iobase, ULONG addr, UBYTE *buff, int blocks)
{
    UBYTE r1;

    if (blocks == 1)
        return sdcmd_read_block(iobase, addr, buff);

    /* Send the read block command */
    sdcmd_send(iobase, SDCMD_READ_MULTIPLE_BLOCK, addr);
    r1 = sdcmd_r1a(iobase);
    if (r1) {
        debug("r1=$%02lx", r1);
        sdcmd_select(iobase, FALSE);
        return r1;
    }

    for (; blocks > 0; blocks--, buff += SDSIZ_BLOCK) {
        r1 = sdcmd_read_packet(iobase, buff, SDSIZ_BLOCK);
        if (r1) {
            debug("r1=$%02lx", r1);
            /* Terminate the read early */
            sdcmd_stop_transmission(iobase);
            return r1;
        }
    }
          
    /* Terminate the read */
    return sdcmd_stop_transmission(iobase);
}

UBYTE sdcmd_write_block(ULONG iobase, ULONG addr, CONST UBYTE *buff)
{
    UBYTE token = 0xfe;
    UBYTE r1;

    debug("addr=%ld", addr);

    /* Send write block command */
    sdcmd_send(iobase, SDCMD_WRITE_SINGLE_BLOCK, addr);
    r1 = sdcmd_r1a(iobase);
    if (!r1)
        r1 = sdcmd_write_packet(iobase, token, buff, SDSIZ_BLOCK);

    sdcmd_select(iobase, FALSE);

    return r1;
}


UBYTE sdcmd_write_blocks(ULONG iobase, ULONG addr, CONST UBYTE *buff, int blocks)
{
    UBYTE token = 0xfc, stop_token = 0xfd;
    UBYTE tmp, r1;
    int i;

    if (blocks == 1)
        return sdcmd_write_block(iobase, addr, buff);

    debug("addr=%ld, blocks=%ld", addr, blocks);

    /* Send write blocks command */
    sdcmd_send(iobase, SDCMD_WRITE_MULTIPLE_BLOCK, addr);
    r1 = sdcmd_r1a(iobase);
    if (r1) {
        sdcmd_select(iobase, FALSE);
        return r1;
    }

    for (; blocks; blocks--, buff += SDSIZ_BLOCK) {
        r1 = sdcmd_write_packet(iobase, token, buff, SDSIZ_BLOCK);
        debug("pr1=$%02lx", r1);
        if (r1)
            break;
    }

    sdcmd_select(iobase, TRUE);

    /* Send stop token */
    sdcmd_out(iobase, stop_token);

    /* Read 'stuff' byte */
    sdcmd_in(iobase);

    /* Wait until not busy */
    for (i = 0; i < SDCMD_TIMEOUT; i++) {
        tmp = sdcmd_in(iobase);
        debug("tmp=$%02lx", tmp);
        if (tmp == 0xff)
            break;
    }

    sdcmd_select(iobase, FALSE);

    debug("i=%ld, r1=$%02lx", i, r1);
    return (i == SDCMD_TIMEOUT) ? SDERRF_TIMEOUT : r1;
}

/* vim: set shiftwidth=4 expandtab:  */
