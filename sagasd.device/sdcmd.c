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

#define sdcmd_log(sd,level,fmt,args...) \
    do { \
        if (sd->func.log) \
            sd->func.log(sd, level, "%s:%ld " fmt, __func__, (ULONG)__LINE__ ,##args); \
    } while (0)

#define diag(fmt,args...)       sdcmd_log(sd, SDLOG_DIAG, fmt ,##args)
#define debug(fmt,args...)      sdcmd_log(sd, SDLOG_DEBUG, fmt ,##args)
#define info(fmt,args...)       sdcmd_log(sd, SDLOG_INFO,  fmt ,##args)
#define warn(fmt,args...)       sdcmd_log(sd, SDLOG_WARN, fmt ,##args)
#define error(fmt,args...)      sdcmd_log(sd, SDLOG_ERROR, fmt ,##args)

#define SDCMD_CLKDIV_SLOW       0x10
#define SDCMD_CLKDIV_FAST       0x01

static UBYTE crc7(UBYTE crc, UBYTE byte)
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

static UWORD crc16(UWORD crc, UBYTE byte)
{
    crc = ((UBYTE)(crc >> 8)) | ((UBYTE)crc << 8);
    crc ^= byte;
    crc ^= (UBYTE)(crc & 0xff) >> 4;
    crc ^= crc << 12;
    crc ^= (crc & 0xff) << 5;

    return crc;
}

static VOID sdcmd_out(struct sdcmd *sd, UBYTE data)
{
    diag("SD_DATA <= $%02lx", data);

    Write8(sd->iobase + SAGA_SD_DATA, data);

    (void)Read8(sd->iobase + SAGA_SD_DATA);
}

static UBYTE sdcmd_in(struct sdcmd *sd)
{
    UBYTE val;

    Write8(sd->iobase + SAGA_SD_DATA, 0xff);
    val = Read8(sd->iobase + SAGA_SD_DATA);

    diag("SD_DATA => $%02lx", val);

    return val;
}

static UWORD sdcmd_ins(struct sdcmd *sd, UWORD crc, UBYTE *buff, size_t len)
{
    UBYTE val;
    ULONG dataio = sd->iobase + SAGA_SD_DATA;

    if (len == 0)
        return crc;

    /* Since the read of the SAGA_SD_DATA will stall until
     * filled by the SPI, we amortize that cost by computing
     * the CRC16 while waiting for the next fill.
     */
    Write8(dataio, 0xff);
    for (len--; len > 0; len--, buff++) {
        val = Read8(dataio + SAGA_SD_DATA);
        Write8(dataio + SAGA_SD_DATA, 0xff);
        crc = crc16(crc, val);
        *buff = val;
        if (DEBUG)
            diag("SD_DATA => $%02lx", val);
    }
    val = Read8(dataio + SAGA_SD_DATA);
    crc = crc16(crc, val);
    *buff = val;

    return crc;
}
            
BOOL sdcmd_present(struct sdcmd *sd)
{
    UWORD val;
    
    val = Read16(sd->iobase + SAGA_SD_STAT);

    diag("SD_STAT => $%04lx", val);

    return (val & SAGA_SD_STAT_NCD) ? FALSE : TRUE;
}

VOID sdcmd_select(struct sdcmd *sd, BOOL cs)
{
    UWORD val;

    val = cs ? 0 : SAGA_SD_CTL_NCS;

    diag("SD_CTL  => $%04lx", val);

    Write16(sd->iobase + SAGA_SD_CTL, val);
    sdcmd_out(sd, 0xff);

    /*  Wait for card ready */
    if (cs) {
        int i;
        for (i = 0; i < SDCMD_TIMEOUT; i++) {
            UBYTE r1 = sdcmd_in(sd);
            if (r1 == 0xff)
                break;
        }
    }
}

static VOID sdcmd_clkdiv(struct sdcmd *sd, UBYTE val)
{
    diag("SD_CLK  => $%04lx", val);

    Write16(sd->iobase + SAGA_SD_CLK, val);
}


void sdcmd_send(struct sdcmd *sd, UBYTE cmd, ULONG arg)
{
    int i;
    UBYTE crc;

    sdcmd_select(sd, TRUE);

    cmd = (cmd & 0x3f) | SDCMD_VALID;

    crc = crc7(0, cmd);

    sdcmd_out(sd, (cmd & 0x3f) | 0x40);

    for (i = 0; i < 4; i++, arg <<= 8) {
        UBYTE byte = (arg >> 24) & 0xff;
        crc = crc7(crc, byte);

        sdcmd_out(sd, byte);
    }

    sdcmd_out(sd, (crc << 1) | 1);
}

static UBYTE sdcmd_r1a(struct sdcmd *sd)
{
    UBYTE r1;
    int i;

    for (i = 0; i < SDCMD_TIMEOUT; i++) {
        r1 = sdcmd_in(sd);
        if (!(r1 & SDERRF_TIMEOUT))
            return r1;
    }

    return SDERRF_TIMEOUT;
}

#define R1_OK(x)        (((x) & ~SDERRF_IDLE) == 0)

UBYTE sdcmd_r1(struct sdcmd *sd)
{
    UBYTE r1;

    r1 = sdcmd_r1a(sd);
    sdcmd_select(sd, FALSE);

    return r1;
}

UBYTE sdcmd_r2(struct sdcmd *sd, UBYTE *r2)
{
    UBYTE r1;

    r1 = sdcmd_r1a(sd);
    if (R1_OK(r1))
        *r2 = sdcmd_in(sd);

    sdcmd_select(sd, FALSE);

    return r1;
}

UBYTE sdcmd_r3(struct sdcmd *sd, ULONG *ocr)
{
    UBYTE r1;
    ULONG r3 = 0;
    int i;

    r1 = sdcmd_r1a(sd);
    if (R1_OK(r1)) {
        for (i = 0; i < 4; i++) {
            r3 <<= 8;
            r3 |= sdcmd_in(sd);
        }
    }

    sdcmd_select(sd, FALSE);

    debug("r3=0x%08lx", r3);
    *ocr = r3;

    return r1;
}

UBYTE sdcmd_r7(struct sdcmd *sd, ULONG *ifcond)
{
    UBYTE r1;
    ULONG r7 = 0;
    int i;

    r1 = sdcmd_r1a(sd);
    if (R1_OK(r1)) {
        for (i = 0; i < 4; i++) {
            r7 <<= 8;
            r7 |= sdcmd_in(sd);
        }
    }

    sdcmd_select(sd, FALSE);

    debug("r7=0x%08lx", r7);
    *ifcond = r7;

    return r1;
}

UBYTE sdcmd_asend(struct sdcmd *sd, UBYTE acmd, ULONG arg)
{
    UBYTE r1;

    /* Next command is an app command.. */
    sdcmd_send(sd, SDCMD_APP_CMD, 0);

    r1 = sdcmd_r1a(sd);
    if (!R1_OK(r1)) {
        sdcmd_select(sd, FALSE);
        return r1;
    }

    sdcmd_send(sd, acmd, arg);

    return 0;
}

UBYTE sdcmd_read_packet(struct sdcmd *sd, UBYTE *buff, int len)
{
    UBYTE token = 0xfe;
    UBYTE byte;
    UWORD crc, tmp;
    int i;

    /* Wait for the Data Token */
    for (i = 0; i < SDCMD_TIMEOUT; i++) {
        byte = sdcmd_in(sd);
        if (byte == token)
            break;
    }

    if (i == SDCMD_TIMEOUT) {
        sdcmd_select(sd, FALSE);
        return SDERRF_TIMEOUT;
    }

    crc = sdcmd_ins(sd, 0, buff, len);

    /* Read the CRC16 */
    tmp = (UWORD)sdcmd_in(sd) << 8;
    tmp |= sdcmd_in(sd);

    if (tmp != crc)
        return SDERRF_CRC;

    return 0;
}


UBYTE sdcmd_stop_transmission(struct sdcmd *sd)
{
    LONG crc_retry = sd->retry.read;
    UBYTE r1, tmp;
    int i = 0;

    do {
        sdcmd_send(sd, SDCMD_STOP_TRANSMISSION, 0);

        /* Read the stuff byte */
        sdcmd_in(sd);

        /* Read response */
        r1 = sdcmd_r1a(sd);
        debug("r1=$%02lx", r1);
    } while ((r1 & SDERRF_CRC) && (crc_retry-- > 0));

    /* If it's a CRC error, after our retires, just die. */
    if (r1 & SDERRF_CRC)
        goto exit;

    /* Otherwise we ignore it... There is no circumstance
     * where it will have meaningful information,
     * and some cards put junk data in the R1
     * response.
     */
    r1 = 0;

    /* Wait until not busy */
    for (i = 0; i < SDCMD_TIMEOUT; i++) {
        tmp = sdcmd_in(sd);
        debug("tmp=$%02lx", tmp);
        if (tmp == 0xff)
           break;
    }

exit:
    sdcmd_select(sd, FALSE);

    return (i == SDCMD_TIMEOUT) ? SDERRF_TIMEOUT : r1;
}

UBYTE sdcmd_write_packet(struct sdcmd *sd, UBYTE token, CONST UBYTE *buff, int len)
{
    UBYTE r1, byte;
    UWORD crc;
    int i;

    /* Send a spacing byte */
    sdcmd_out(sd, 0xff);

    /* Start the data packet */
    sdcmd_out(sd, token);
    crc = 0;

    /* Send the block */
    for (i = 0; i < SDSIZ_BLOCK; i++, buff++) {
        byte = *buff;
        sdcmd_out(sd, byte);
        crc = crc16(crc, byte);
    }

    /* Send the CRC16, MSB first */
    sdcmd_out(sd, (crc >> 8) & 0xff);
    sdcmd_out(sd, (crc >> 0) & 0xff);

    /* Read the Data Response */
    byte = sdcmd_in(sd);
    if ((byte & SDDRS_VALID_MASK) != SDDRS_VALID) {
        /* Terminate the read early */
        sdcmd_stop_transmission(sd);
        return SDERRF_CRC;
    }

    r1 = ((byte & SDDRS_CODE_MASK) == SDDRS_CODE_ACCEPTED) ? 0 : SDERRF_CRC;
    debug("byte=$%02lx, r1=$%02lx", byte, r1);

    /* Wait for the idle pattern */
    /* Wait until not busy */
    for (i = 0; i < SDCMD_TIMEOUT; i++) {
        UBYTE tmp = sdcmd_in(sd);
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
UBYTE sdcmd_detect(struct sdcmd *sd)
{
    struct sdcmd_info *info = &sd->info;
    UBYTE r1;
    ULONG r7;
    int i;

    memset(info, 0, sizeof(*info));

    /* First, check the DETECT bit */
    if (!sdcmd_present(sd))
        return ~0;

    /* Switch to slow speed mode */
    sdcmd_clkdiv(sd, SDCMD_CLKDIV_SLOW);

    /* Emit at least 74 clocks of idle */
    sdcmd_select(sd, TRUE);
    for (i = 0; i < 10; i++)
        sdcmd_out(sd, 0xff);
    sdcmd_select(sd, FALSE);

    /* Stuff two idle bytes while deasserted */
    sdcmd_out(sd, 0xff);
    sdcmd_out(sd, 0xff);

    /* Put into idle state */
    sdcmd_send(sd, SDCMD_GO_IDLE_STATE, 0);
    r1 = sdcmd_r1(sd);
    /* It's ok (and expected) that we are in IDLE state */
    r1 &= ~SDERRF_IDLE;
    if (r1)
        return r1;

    /* Do SHDC detection during idle */
    sdcmd_send(sd, SDCMD_SEND_IF_COND, 0x000001aa);
    r1 = sdcmd_r7(sd, &r7);
    /* It's ok (and expected) that we are in IDLE state */
    r1 &= ~SDERRF_IDLE;
    if (!r1) {
        /* Looks like a SDHC card? */
        if ((r7 & 0x000001ff) == 0x000001aa) {
            /* Set HCS (SDHC) mode */
            sdcmd_asend(sd, SDCMD_SD_SEND_OP_COND, SDOCRF_HCS);
            r1 = sdcmd_r1(sd);
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
        err = sdcmd_asend(sd, SDCMD_SD_SEND_OP_COND, 0);
        if (err)
            return err;

        r1 = sdcmd_r1(sd);
        if (!(r1 & SDERRF_IDLE))
            break;
    }

    debug("r1=0x%lx", r1);
    if (r1)
        return r1;

    /* Check for voltage levels */
    sdcmd_send(sd, SDCMD_READ_OCR, 0);
    r1 = sdcmd_r3(sd, &info->ocr);
    debug("r1=0x%lx", r1);
    if (r1)
        return r1;

    /* Not in our voltage range */
    info("ocr=0x%08lx (vs 0x%08lx)", info->ocr, SDOCRF_MAX_3_3V | SDOCRF_MAX_3_4V);
    if (!(info->ocr & (SDOCRF_MAX_3_3V | SDOCRF_MAX_3_4V)))
        return SDERRF_IDLE;

    if (info) {
        ULONG c_size_mult, read_bl_len, c_size;
        UBYTE *csd = &info->csd[0];
        UBYTE *cid = &info->cid[0];

        /* Get the CSD data */
        sdcmd_send(sd, SDCMD_SEND_CSD, 0);
        r1 = sdcmd_r1a(sd);
        debug("r1=0x%lx", r1);
        if (r1)
            goto exit;

        r1 = sdcmd_read_packet(sd, csd, 16);
        debug("r1=0x%lx", r1);
        if (r1)
            goto exit;

        info("csd=%02lx%02lx%02lx%02lx-%02lx%02lx%02lx%02lx-%02lx%02lx%02lx%02lx-%02lx%02lx%02lx%02lx",
                csd[0], csd[1], csd[2], csd[3],
                csd[4], csd[5], csd[6], csd[7],
                csd[8], csd[9], csd[10], csd[11],
                csd[12], csd[13], csd[14], csd[15]);

        /* Get the CID data */
        sdcmd_send(sd, SDCMD_SEND_CID, 0);
        r1 = sdcmd_r1a(sd);
        debug("r1=%d", r1);
        if (r1)
            goto exit;

        r1 = sdcmd_read_packet(sd, cid, 16);
        sdcmd_select(sd, FALSE);

        debug("r1=0x%lx", r1);
        if (r1)
            return r1;
        info("cid=%02lx%02lx%02lx%02lx-%02lx%02lx%02lx%02lx-%02lx%02lx%02lx%02lx-%02lx%02lx%02lx%02lx",
                cid[0], cid[1], cid[2], cid[3],
                cid[4], cid[5], cid[6], cid[7],
                cid[8], cid[9], cid[10], cid[11],
                cid[12], cid[13], cid[14], cid[15]);

        info->block_size = SDSIZ_BLOCK;

        if (info->ocr & SDOCRF_HCS) {
            /* SDHC calculation */

            /* Bits 68:48 of the CSD */
            c_size = bits(&csd[15], 48, 20);

            info("SDHC: c_size=%ld", c_size);

            info->blocks = (c_size + 1) * 1024;

            info->addr_shift = 0;
        } else {
            /* SD calculation */
            /* Bits 49:47 of the CSD */
            c_size_mult = bits(&csd[15], 47, 3);
            /* Bits 83:80 of the CSD */
            read_bl_len = bits(&csd[15], 80, 4);
            /* Bits 73:62 of the CSD */
            c_size = bits(&csd[15], 62, 12);

            info("SD: c_size_mult=%ld, read_bl_len=%ld, c_size=%ld", c_size_mult, read_bl_len, c_size);

            info->blocks = (1UL << (c_size_mult + read_bl_len + 2 - 9)) * (c_size + 1);

            /* Set block size */
            sdcmd_send(sd, SDCMD_SET_BLOCKLEN, info->block_size);
            r1 = sdcmd_r1(sd);
            if (r1)
                return r1;

            info->addr_shift = 9;
        }
        info("blocks=%ld", info->blocks);
    }

    /* Switch to high speed mode */
    sdcmd_clkdiv(sd, SDCMD_CLKDIV_FAST);

exit:
    sdcmd_select(sd, FALSE);
    return r1;
}

UBYTE sdcmd_read_block(struct sdcmd *sd, ULONG addr, UBYTE *buff)
{
    UBYTE r1;
    LONG crc_retry = sd->retry.read;

    do {
        info("read block=%ld", addr);

        /* Send the read block command */
        sdcmd_send(sd, SDCMD_READ_SINGLE_BLOCK, addr << sd->info.addr_shift);
        r1 = sdcmd_r1a(sd);
        if (!r1)
            r1 = sdcmd_read_packet(sd, buff, SDSIZ_BLOCK);

        sdcmd_select(sd, FALSE);

    } while ((crc_retry-- > 0) && (r1 & SDERRF_CRC));

    return r1;
}


UBYTE sdcmd_read_blocks(struct sdcmd *sd, ULONG addr, UBYTE *buff, int blocks)
{
    UBYTE r1;
    LONG crc_retry = sd->retry.read;

    if (blocks == 1)
        return sdcmd_read_block(sd, addr, buff);

    do {
        info("read block=%ld, blocks=%ld", addr, blocks);

        /* Send the read block command */
        sdcmd_send(sd, SDCMD_READ_MULTIPLE_BLOCK, addr << sd->info.addr_shift);
        r1 = sdcmd_r1a(sd);
        if (r1) {
            debug("r1=$%02lx", r1);
            sdcmd_select(sd, FALSE);
            continue;
        }

        for (; blocks > 0; addr++, blocks--, buff += SDSIZ_BLOCK) {
            r1 = sdcmd_read_packet(sd, buff, SDSIZ_BLOCK);
            if (r1) {
                debug("r1=$%02lx", r1);
                /* Terminate the read early */
                sdcmd_stop_transmission(sd);
                break;
            }

            /* Reset the retry counter if we read a block */
            crc_retry = sd->retry.read;
        }

        if (r1)
            continue;
              
        /* Terminate the read */
        r1 = sdcmd_stop_transmission(sd);
    } while ((r1 & SDERRF_CRC) && (crc_retry-- > 0));

    return r1;
}

UBYTE sdcmd_write_block(struct sdcmd *sd, ULONG addr, CONST UBYTE *buff)
{
    LONG crc_retry = sd->retry.write;
    UBYTE token = 0xfe;
    UBYTE r1;

    do {
        info("write block=%ld", addr);

        /* Send write block command */
        sdcmd_send(sd, SDCMD_WRITE_SINGLE_BLOCK, addr << sd->info.addr_shift);
        r1 = sdcmd_r1a(sd);
        if (!r1)
            r1 = sdcmd_write_packet(sd, token, buff, SDSIZ_BLOCK);

        sdcmd_select(sd, FALSE);
    } while ((r1 & SDERRF_CRC) && (crc_retry-- > 0));

    return r1;
}


UBYTE sdcmd_write_blocks(struct sdcmd *sd, ULONG addr, CONST UBYTE *buff, int blocks)
{
    LONG crc_retry = sd->retry.write;
    UBYTE token = 0xfc, stop_token = 0xfd;
    UBYTE tmp, r1;
    int i;

    if (blocks == 1)
        return sdcmd_write_block(sd, addr, buff);

    do {
        info("block=%ld, blocks=%ld", addr, blocks);

        /* Send write blocks command */
        sdcmd_send(sd, SDCMD_WRITE_MULTIPLE_BLOCK, addr << sd->info.addr_shift);
        r1 = sdcmd_r1a(sd);
        if (r1) {
            sdcmd_select(sd, FALSE);
            continue;
        }

        for (; blocks; addr++, blocks--, buff += SDSIZ_BLOCK) {
            r1 = sdcmd_write_packet(sd, token, buff, SDSIZ_BLOCK);
            debug("pr1=$%02lx", r1);
            if (r1)
                break;

            /* Reset the retry counter if we wrote a block */
            crc_retry = sd->retry.read;
        }

        sdcmd_select(sd, TRUE);

        /* Send stop token */
        sdcmd_out(sd, stop_token);

        /* Read 'stuff' byte */
        sdcmd_in(sd);

        /* Wait until not busy */
        for (i = 0; i < SDCMD_TIMEOUT; i++) {
            tmp = sdcmd_in(sd);
            debug("tmp=$%02lx", tmp);
            if (tmp == 0xff)
                break;
        }

        sdcmd_select(sd, FALSE);

        debug("i=%ld, r1=$%02lx", i, r1);
        if (i == SDCMD_TIMEOUT)
            r1 = SDERRF_TIMEOUT;

    } while ((r1 & SDERRF_CRC) && (crc_retry-- > 0));

    return r1;
}

/* vim: set shiftwidth=4 expandtab:  */
