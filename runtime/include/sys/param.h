/*
 * param.h
 *
 * This file is part of the Mingw32 package.
 *
 * Contributors:
 *  Created by Earnie Boyd <earnie@users.sf.net>
 *
 *  THIS SOFTWARE IS NOT COPYRIGHTED
 *
 *  This source code is offered for use in the public domain. You may
 *  use, modify or distribute it freely.
 *
 *  This code is distributed in the hope that it will be useful but
 *  WITHOUT ANY WARRANTY. ALL WARRANTIES, EXPRESS OR IMPLIED ARE HEREBY
 *  DISCLAIMED. This includes but is not limited to warranties of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * $Revision: 1.4 $
 * $Author: earnie $
 * $Date: 2004-04-19 17:22:41 $
 *
 */

#ifndef _SYS_PARAM_H
#define _SYS_PARAM_H

#include <sys/types.h>
#include <limits.h>

/* These are useful for cross-compiling */ 
#define BIG_ENDIAN	4321
#define LITTLE_ENDIAN	1234
#define BYTE_ORDER	LITTLE_ENDIAN

#define MAXPATHLEN PATH_MAX

#endif
