/*
 * utime.h
 *
 * Support for the utime function.
 *
 * This file is part of the Mingw32 package.
 *
 * Contributors:
 *  Created by Colin Peters <colin@bird.fu.is.saga-u.ac.jp>
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
 * $Revision: 1.5 $
 * $Author: earnie $
 * $Date: 2004-04-19 17:22:41 $
 *
 */
#ifndef	_UTIME_H_
#define	_UTIME_H_

/* All the headers include this file. */
#include <_mingw.h>

#define __need_wchar_t
#define __need_size_t
#ifndef RC_INVOKED
#include <stddef.h>
#endif	/* Not RC_INVOKED */
#include <sys/types.h>

#ifndef	RC_INVOKED

/*
 * Structure used by _utime function.
 */
struct _utimbuf
{
	time_t	actime;		/* Access time */
	time_t	modtime;	/* Modification time */
};


#ifndef	_NO_OLDNAMES
/* NOTE: Must be the same as _utimbuf above. */
struct utimbuf
{
	time_t	actime;
	time_t	modtime;
};
#endif	/* Not _NO_OLDNAMES */

struct __utimbuf64
{
	__time64_t actime;
	__time64_t modtime;
};


#ifdef	__cplusplus
extern "C" {
#endif

_CRTIMP int __cdecl	_utime (const char*, struct _utimbuf*);

#ifndef	_NO_OLDNAMES
_CRTIMP int __cdecl	utime (const char*, struct utimbuf*);
#endif	/* Not _NO_OLDNAMES */

_CRTIMP int __cdecl	_futime (int, struct _utimbuf*);

/* The wide character version, only available for MSVCRT versions of the
 * C runtime library. */
#ifdef __MSVCRT__
_CRTIMP int __cdecl	_wutime (const wchar_t*, struct _utimbuf*);
#endif /* MSVCRT runtime */

/* These require newer versions of msvcrt.dll (6.10 or higher).  */ 
#if __MSVCRT_VERSION__ >= 0x0601
_CRTIMP int __cdecl	_utime64 (const char*, struct __utimbuf64*);
_CRTIMP int __cdecl	_wutime64 (const wchar_t*, struct __utimbuf64*);
_CRTIMP int __cdecl	_futime64 (int, struct __utimbuf64*);
#endif /* __MSVCRT_VERSION__ >= 0x0601 */

#ifdef	__cplusplus
}
#endif

#endif	/* Not RC_INVOKED */

#endif	/* Not _UTIME_H_ */
