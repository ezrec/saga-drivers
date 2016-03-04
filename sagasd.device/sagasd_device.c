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

#include <string.h>     // memset

#include <exec/resident.h>
#include <exec/errors.h>
#include <exec/memory.h>
#include <exec/lists.h>
#include <exec/alerts.h>
#include <exec/tasks.h>
#include <exec/io.h>

#include <devices/newstyle.h>
#include <devices/trackdisk.h>
#include <devices/timer.h>

#include <aros/symbolsets.h>

#include <dos/filehandler.h>

#include <proto/exec.h>
#include <proto/disk.h>
#include <proto/expansion.h>

#include "common.h"

#include <saga/sd.h>

#include LC_LIBDEFS_FILE

/* Execute the SD read or write command, return IOERR_* or TDERR_*
 */
static LONG SAGASD_ReadWrite(struct IORequest *io, UQUAD off64, BOOL is_write)
{
    struct IOStdReq *iostd = (struct IOStdReq *)io;
    struct IOExtTD *iotd = (struct IOExtTD *)io;
    struct SAGASDUnit *sdu = (struct SAGASDUnit *)io->io_Unit;
    APTR data = iotd->iotd_Req.io_Data;
    ULONG len = iotd->iotd_Req.io_Length;
    ULONG block_size, bmask;
    UBYTE sderr;

    block_size = sdu->sdu_Identify.block_size;
    bmask = block_size - 1;

    /* Read/Write is not permitted if the unit is not Valid */
    if (!sdu->sdu_Valid)
        return TDERR_BadDriveType;

    if (sdu->sdu_ReadOnly)
        return TDERR_WriteProt;

    if ((off64 & bmask) || bmask == 0 || data == NULL)
        return IOERR_BADADDRESS;

    if ((len & bmask) || len == 0) {
        debug("IO %p Fault, io_Flags = %d, io_Command = %d, IOERR_BADLENGTH (len=0x%x, bmask=0x%x)", io, io->io_Flags, io->io_Command, len, bmask);
        return IOERR_BADLENGTH;
    }

    /* Make in units of sector size */
    len /= block_size;
    off64 /= block_size;

    /* Nothing to do... */
    if (len == 0) {
        iostd->io_Actual = 0;
        return 0;
    }

    /* Do the IO */
    if (is_write) {
        sderr = sdcmd_write_blocks(sdu->sdu_IOBase, (ULONG)off64, data, len);
    } else {
        sderr = sdcmd_read_blocks(sdu->sdu_IOBase, (ULONG)off64, data, len);
    }

    if (sderr) {
        iostd->io_Actual = 0;

        /* Decode sderr into IORequest io_Errors */
        if (sderr & SDERRF_TIMEOUT)
            return TDERR_DiskChanged;
        if (sderr & SDERRF_PARAM)
            return TDERR_SeekError;
        if (sderr & SDERRF_ADDRESS)
            return TDERR_SeekError;
        if (sderr & (SDERRF_ERASESEQ | SDERRF_ERASERES))
            return TDERR_BadSecPreamble;
        if (sderr & SDERRF_CRC)
            return TDERR_BadSecSum;
        if (sderr & SDERRF_ILLEGAL)
            return TDERR_TooFewSecs;
        if (sderr & SDERRF_IDLE)
            return TDERR_PostReset;
    }
    
    iostd->io_Actual = block_size * len;
    return 0;
}

/*
 *  Try to do IO commands. All commands which require talking with ahci devices
 *  will be handled slow, that is they will be passed to bus task which will
 *  execute them as soon as hardware will be free.
 */
static LONG SAGASD_PerformIO(struct IORequest *io)
{
    const UWORD NSDSupported[] = {
        CMD_READ,
        CMD_WRITE,
        CMD_UPDATE,
        CMD_CLEAR,
        TD_ADDCHANGEINT,
        TD_CHANGENUM,
        TD_CHANGESTATE,
        TD_EJECT,
        TD_FORMAT,
        TD_GETGEOMETRY,
        TD_MOTOR,
        TD_PROTSTATUS,
        TD_READ64,
        TD_REMCHANGEINT,
        TD_WRITE64,
        NSCMD_DEVICEQUERY,
        NSCMD_TD_READ64,
        NSCMD_TD_WRITE64,
        0
    };
    struct SAGASDBase *sd = (struct SAGASDBase *)io->io_Device;
    struct Library *SysBase = sd->sd_ExecBase;
    struct SAGASDUnit *sdu = (struct SAGASDUnit *)io->io_Unit;
    struct IOStdReq *iostd = (struct IOStdReq *)io;
    struct IOExtTD *iotd = (struct IOExtTD *)io;
    APTR data = iotd->iotd_Req.io_Data;
    ULONG len = iotd->iotd_Req.io_Length;
    UQUAD off64;
    struct DriveGeometry *geom;
    struct NSDeviceQueryResult *nsqr;
    LONG err = IOERR_NOCMD;

    if (io->io_Error == IOERR_ABORTED)
        return io->io_Error;

    debug("IO %p Start, io_Flags = %d, io_Command = %d", io, io->io_Flags, io->io_Command);

    switch (io->io_Command) {
    case NSCMD_DEVICEQUERY:
        if (len < sizeof(*nsqr))
            return IOERR_BADLENGTH;

        nsqr = data;
        nsqr->DevQueryFormat = 0;
        nsqr->SizeAvailable  = sizeof(struct NSDeviceQueryResult);
        nsqr->DeviceType     = NSDEVTYPE_TRACKDISK;
        nsqr->DeviceSubType  = 0;
        nsqr->SupportedCommands = (UWORD *)NSDSupported;
        iostd->io_Actual = sizeof(*nsqr);
        err = 0;
        break;
    case TD_PROTSTATUS:
        iostd->io_Actual = sdu->sdu_ReadOnly ? 1 : 0;
        err = 0;
        break;
    case TD_CHANGENUM:
        iostd->io_Actual = sdu->sdu_ChangeNum;
        err = 0;
        break;
    case TD_CHANGESTATE:
        Forbid();
        iostd->io_Actual = sdu->sdu_Present ? 1 : 0;
        Permit();
        err = 0;
        break;
    case TD_EJECT:
        // Eject removable media
        // We mark is as invalid, then wait for Present to toggle.
        Forbid();
        sdu->sdu_Valid = FALSE;
        Permit();
        err = 0;
        break;
    case TD_GETDRIVETYPE:
        iostd->io_Actual = DRIVE_NEWSTYLE;
        err = 0;
        break;
    case TD_GETGEOMETRY:
        if (len < sizeof(*geom))
            return IOERR_BADLENGTH;

        geom = data;
        memset(geom, 0, len);
        /* Linear "single track" device */
        geom->dg_SectorSize   = sdu->sdu_Identify.block_size;
        geom->dg_TotalSectors = sdu->sdu_Identify.blocks;
        geom->dg_Cylinders    = 1;
        geom->dg_CylSectors   = sdu->sdu_Identify.blocks;
        geom->dg_Heads        = 1;
        geom->dg_TrackSectors = sdu->sdu_Identify.blocks;
        geom->dg_BufMemType   = MEMF_PUBLIC;
        geom->dg_DeviceType   = DG_DIRECT_ACCESS;
        geom->dg_Flags        = DGF_REMOVABLE;
        iostd->io_Actual = sizeof(*geom);
        err = 0;
        break;
    case TD_FORMAT:
        off64  = iotd->iotd_Req.io_Offset;
        err = SAGASD_ReadWrite(io, off64, TRUE);
        break;
    case TD_MOTOR:
        // FIXME: Tie in with power management
        iostd->io_Actual = 1;
        err = TRUE;
        break;
    case CMD_WRITE:
        off64  = iotd->iotd_Req.io_Offset;
        err = SAGASD_ReadWrite(io, off64, TRUE);
        break;
    case TD_WRITE64:
    case NSCMD_TD_WRITE64:
        off64  = iotd->iotd_Req.io_Offset;
        off64 |= ((UQUAD)iotd->iotd_Req.io_Actual)<<32;
        err = SAGASD_ReadWrite(io, off64, TRUE);
        break;
    case CMD_READ:
        off64  = iotd->iotd_Req.io_Offset;
        err = SAGASD_ReadWrite(io, off64, FALSE);
        break;
    case TD_READ64:
    case NSCMD_TD_READ64:
        off64  = iotd->iotd_Req.io_Offset;
        off64 |= ((UQUAD)iotd->iotd_Req.io_Actual)<<32;
        err = SAGASD_ReadWrite(io, off64, FALSE);
        break;
    default:
        debug("Unknown IO command %d\n", io->io_Command);
        err = IOERR_NOCMD;
        break;
    }

    return err;
}

/* This low-priority task handles all the non-quick IO
 */
static void SAGASD_IOTask(struct Library *SysBase)
{
    struct Task *this = FindTask(NULL);
    struct SAGASDUnit *sdu = this->tc_UserData;
    struct MsgPort *mport = sdu->sdu_MsgPort;
    struct MsgPort *tport = NULL;
    struct timerequest *treq = NULL;
    ULONG sigset;
    struct Message status;

    status.mn_Length = 1;   // Failed?
    if ((status.mn_ReplyPort = CreateMsgPort())) {
        if ((tport = CreateMsgPort())) {
            if ((treq = (struct timerequest *)CreateIORequest(tport, sizeof(*treq)))) {
                if (0 == OpenDevice(TIMERNAME, UNIT_VBLANK, treq, 0)) {
                    status.mn_Length = 0;   // Success!
                } else {
                    DeleteIORequest(treq);
                }
            } else {
                DeleteMsgPort(tport);
            }
        } else {
            DeleteMsgPort(status.mn_ReplyPort);
        }
    }

    sdu->sdu_MsgPort = status.mn_ReplyPort;

    /* Send the 'I'm Ready' message */
    PutMsg(mport, &status);

    if (status.mn_ReplyPort) {
        WaitPort(mport);
        GetMsg(mport);
    }

    if (status.mn_Length) {
        /* There was an error... */
        DeleteMsgPort(mport);
        return;
    }

    mport = sdu->sdu_MsgPort;
       
    sigset = (1 << tport->mp_SigBit) | (1 << mport->mp_SigBit);

    for (;;) {
        struct IORequest *io;
        BOOL present;

        io = (struct IORequest *)GetMsg(mport);
        if (!io) {
            ULONG sigs;

            /* Wait up to 100ms for a IO message. If none, then
             * recheck the SD DETECT pin.
             */
            treq->tr_node.io_Command = TR_ADDREQUEST;
            treq->tr_time.tv_secs = 0;
            treq->tr_time.tv_micro = 100 * 1000;
            SendIO(treq);

            /* Wait on either the MsgPort, or the timer */
            sigs = Wait(sigset);
            if (sigs & (1 << mport->mp_SigBit)) {
                /* Message port was signalled */
                io = (struct IORequest *)GetMsg(mport);
                /* Cancel the timer */
                AbortIO(treq);
            } else {
                /* Timeout was signalled */
                io = NULL;
            }

            /* Clean up the timer IO */
            WaitIO(treq);
        }

        /* Update sdu_Present, regardless */
        present = sdcmd_present(sdu->sdu_IOBase);
        if (present != sdu->sdu_Present) {
            Forbid();
            if (present) {
                UBYTE sderr;

                /* Make the drive present. */
                sdu->sdu_Present = TRUE;
                sdu->sdu_ChangeNum++;

                /* Re-run the identify */
                sderr = sdcmd_detect(sdu->sdu_IOBase, &sdu->sdu_Identify);
                sdu->sdu_Valid = (sderr == 0) ? TRUE : FALSE;
            } else {
                sdu->sdu_Present = FALSE;
                sdu->sdu_Valid = FALSE;
            }
            Permit();
        }

        /* If there was no io, continue on...
         */
        if (!io)
            continue;

        /* If io_Command == ~0, this indicates that we are killing
         * this task.
         */
        if (io->io_Command == ~0) {
            io->io_Error = 0;
            io->io_Message.mn_Node.ln_Type=NT_MESSAGE;
            ReplyMsg(&io->io_Message);
            break;
        }

        io->io_Error = SAGASD_PerformIO(io);
        io->io_Message.mn_Node.ln_Type=NT_MESSAGE;

        /* Need a reply now? */
        ReplyMsg(&io->io_Message);
    }

    /* Clean up */
    CloseDevice(treq);
    DeleteIORequest(treq);
    DeleteMsgPort(tport);
    DeleteMsgPort(mport);
}

AROS_LH1(void, BeginIO,
 AROS_LHA(struct IORequest *, io, A1),
    struct SAGASDBase *, SAGASDBase, 5, SAGASD)
{
    AROS_LIBFUNC_INIT

    struct Library *SysBase = SAGASDBase->sd_ExecBase;
    struct SAGASDUnit *sdu = (struct SAGASDUnit *)io->io_Unit;

    if (io->io_Flags & IOF_QUICK) {
        /* Commands that don't require any IO */
	switch(io->io_Command)
	{
            case NSCMD_DEVICEQUERY:
	    case TD_GETNUMTRACKS:
	    case TD_GETDRIVETYPE:
	    case TD_GETGEOMETRY:
	    case TD_REMCHANGEINT:
	    case TD_ADDCHANGEINT:
	    case TD_PROTSTATUS:
	    case TD_CHANGENUM:
	    io->io_Error = SAGASD_PerformIO(io);
            io->io_Message.mn_Node.ln_Type=NT_MESSAGE;
	    return;
	}
    }

    /* Not done quick */
    io->io_Flags &= ~IOF_QUICK;

    /* Forward to the unit's IO task */
    PutMsg(sdu->sdu_MsgPort, &io->io_Message);

    AROS_LIBFUNC_EXIT
}

AROS_LH1(LONG, AbortIO,
    AROS_LHA(struct IORequest *, io, A1),
    struct SAGASDBase *, SAGASDBase, 6, SAGASD)
{
    AROS_LIBFUNC_INIT

    struct Library *SysBase = SAGASDBase->sd_ExecBase;

    Forbid();
    io->io_Error = IOERR_ABORTED;
    Permit();

    return 0;

    AROS_LIBFUNC_EXIT
}


static void SAGASD_BootNode(
        struct Library *ExpansionBase,
        ULONG unit)
{
    TEXT dosdevname[4] = "SD0";
    IPTR pp[4 + DE_BOOTBLOCKS + 1] = {};
    struct DeviceNode *devnode;

    dosdevname[2] += unit;
    debug("Adding bootnode %s", dosdevname);

    pp[0] = (IPTR)dosdevname;
    pp[1] = (IPTR)"sagasd.device";
    pp[2] = unit;
    pp[DE_TABLESIZE + 4] = DE_BOOTBLOCKS;
    pp[DE_SIZEBLOCK + 4] = 512 >> 2;
    pp[DE_NUMHEADS + 4] = 1;
    pp[DE_SECSPERBLOCK + 4] = 1;
    pp[DE_BLKSPERTRACK + 4] = 1;
    pp[DE_RESERVEDBLKS + 4] = 2;
    pp[DE_LOWCYL + 4] = 0;
    pp[DE_HIGHCYL + 4] = 0xffffffff;
    pp[DE_NUMBUFFERS + 4] = 10;
    pp[DE_BUFMEMTYPE + 4] = MEMF_PUBLIC;
    pp[DE_MAXTRANSFER + 4] = 0x00200000;
    pp[DE_MASK + 4] = 0xFFFFFFFE;
    pp[DE_BOOTPRI + 4] = 5 - (unit * 10);
    pp[DE_DOSTYPE + 4] = 0;
    pp[DE_BOOTBLOCKS + 4] = 2;
    devnode = MakeDosNode(pp);

    if (devnode)
   	AddBootNode(pp[DE_BOOTPRI + 4], 0, devnode, NULL);
}

static void SAGASD_InitUnit(struct SAGASDBase * SAGASDBase, int id)
{
    struct Library *SysBase = SAGASDBase->sd_ExecBase;
    struct SAGASDUnit *sdu = &SAGASDBase->sd_Unit[id];

    switch (id) {
    case 0:
        sdu->sdu_IOBase  = SAGA_SD_BASE;
        sdu->sdu_Enabled = TRUE;
        break;
    default:
        sdu->sdu_Enabled = FALSE;
    }

    sdu->sdu_ExecBase = SysBase;

    /* If the unit is present, create an IO task for it
     */
    if (sdu->sdu_Enabled) {
        struct Task *utask = &sdu->sdu_Task;
        struct MsgPort *initport;

        if ((initport = CreateMsgPort())) {
            struct Message *msg;

            strncpy(sdu->sdu_Name, "SDIO0", sizeof(sdu->sdu_Name));
            sdu->sdu_Name[4] += id;

            /* Initialize the task */
            memset(utask, 0, sizeof(*utask));
            utask->tc_Node.ln_Pri = -21;
            utask->tc_Node.ln_Name = &sdu->sdu_Name[0];
            utask->tc_SPReg = utask->tc_SPUpper = &sdu->sdu_Stack[SDU_STACK_SIZE];
            utask->tc_SPLower = &sdu->sdu_Stack[0];
            NEWLIST(&utask->tc_MemEntry);
            utask->tc_UserData = sdu;

            AddTask(utask, SAGASD_IOTask, NULL);

            WaitPort(initport);
            msg = GetMsg(initport);
            sdu->sdu_Enabled = (msg->mn_Length == 0) ? TRUE : FALSE;
            ReplyMsg(msg);

            DeleteMsgPort(initport);
        }
    }

    debug("id=%08x enabled=%d", i, SAGASDBase->sd_Unit[i].sdu_Enabled ? 1 : 0);
}


static int GM_UNIQUENAME(init)(struct SAGASDBase * SAGASDBase)
{
    struct Library *SysBase = SAGASDBase->sd_ExecBase;
    ULONG i;
    struct Library *ExpansionBase;
 
    debug("Init");
    
    ExpansionBase = TaggedOpenLibrary(TAGGEDOPEN_EXPANSION);
    if (!ExpansionBase)
  	Alert(AT_DeadEnd | AO_TrackDiskDev | AG_OpenLib);

    for (i = 0; i < SAGASD_UNITS; i++)
	SAGASD_InitUnit(SAGASDBase, i);

    /* Only add bootnode if recalibration succeeded */
    for (i = 0; i < SAGASD_UNITS; i++) {
	if (SAGASDBase->sd_Unit[i].sdu_Enabled)
	    SAGASD_BootNode(ExpansionBase, i);
    }

    CloseLibrary((struct Library *)ExpansionBase);

    debug("done %d", drives);

    return TRUE;
}

static int GM_UNIQUENAME(expunge)(struct SAGASDBase * SAGASDBase)
{
    struct Library *SysBase = SAGASDBase->sd_ExecBase;
    struct IORequest io = {};
    int i;

    for (i = 0; i < SAGASD_UNITS; i++) {
        io.io_Device = &SAGASDBase->sd_Device;
        io.io_Unit = &SAGASDBase->sd_Unit[i].sdu_Unit;
        io.io_Flags = 0;
        io.io_Command = ~0;

        /* Signal the unit task to die */
        DoIO(&io.io_Message);
    }

    return TRUE;
}

static int GM_UNIQUENAME(open)(struct SAGASDBase * SAGASDBase,
                               struct IOExtTD *iotd, ULONG unitnum,
                               ULONG flags)
{
    iotd->iotd_Req.io_Error = IOERR_OPENFAIL;

    /* Is the requested unitNumber valid? */
    if (unitnum < SAGASD_UNITS) {
        struct SAGASDUnit *sdu;

        iotd->iotd_Req.io_Device = (struct Device *)SAGASDBase;

        /* Get SDU structure */
        sdu = &SAGASDBase->sd_Unit[unitnum];
	if (sdu->sdu_Enabled) {
    	    iotd->iotd_Req.io_Unit = &sdu->sdu_Unit;
    	    sdu->sdu_Unit.unit_OpenCnt++;
    	    iotd->iotd_Req.io_Error = 0;
	}
    }
    debug("Open=%d", unitnum, iotd->iotd_Req.io_Error);
  
    return iotd->iotd_Req.io_Error == 0;

}

static int GM_UNIQUENAME(close)(struct SAGASDBase *SAGASDBase,
                                struct IOExtTD *iotd)
{
    iotd->iotd_Req.io_Unit->unit_OpenCnt --;

    return TRUE;
}

ADD2INITLIB(GM_UNIQUENAME(init), 0)
ADD2EXPUNGELIB(GM_UNIQUENAME(expunge), 0)
ADD2OPENDEV(GM_UNIQUENAME(open), 0)
ADD2CLOSEDEV(GM_UNIQUENAME(close), 0)
/* vim: set shiftwidth=4 expandtab:  */
