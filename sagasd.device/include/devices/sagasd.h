/*
 * Copyright (C) 2016, The AROS Development Team
 * All rights reserved.
 * Author: Jason S. McMullan <jason.mcmullan@gmail.com>
 *
 * Licensed under the AROS PUBLIC LICENSE (APL) Version 1.1
 *
 * $Id$
 */

#ifndef SAGASD_H
#define SAGASD_H

/* OpenDevice() flags
 */
#define SAGASD_FLAGS_CLKDIV(_x)         (((_x) & 0xff) << 0)
#define SAGASD_FLAGS_CLKDIV_of(_x)      (((_x) >> 0) & 0xff)
#define SAGASD_FLAGS_HAS_CLKDIV         (1 << 8)

#endif /* SAGASD_H */
/* vim: set shiftwidth=4 expandtab:  */
