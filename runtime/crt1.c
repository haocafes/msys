/*
 * crt1.c
 *
 * Source code for the startup proceedures used by all programs. This code
 * is compiled to make crt1.o, which should be located in the library path.
 *
 * This code is part of the Mingw32 package.
 *
 * Contributors:
 *  Created by Colin Peters <colin@bird.fu.is.saga-u.ac.jp>
 *  Maintained by Mumit Khan <khan@xraylith.wisc.EDU>
 *
 *  THIS SOFTWARE IS NOT COPYRIGHTED
 *
 *  This source code is offered for use in the public domain. You may
 *  use, modify or distribute it freely.
 *
 *  This code is distributed in the hope that it will be useful but
 *  WITHOUT ANY WARRANTY. ALL WARRENTIES, EXPRESS OR IMPLIED ARE HEREBY
 *  DISCLAMED. This includes but is not limited to warrenties of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * $Revision: 1.2 $
 * $Author: earnie $
 * $Date: 2001-06-05 00:26:30 $
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <process.h>
#include <float.h>
#include <windows.h>
#include <signal.h>

/* NOTE: The code for initializing the _argv, _argc, and environ variables
 *       has been moved to a separate .c file which is included in both
 *       crt1.c and dllcrt1.c. This means changes in the code don't have to
 *       be manually synchronized, but it does lead to this not-generally-
 *       a-good-idea use of include. */
#include "init.c"

extern int main (int, char **, char **);

/*
 * Must have the correct app type for MSVCRT. 
 */

#ifdef __MSVCRT__
#define __UNKNOWN_APP    0
#define __CONSOLE_APP    1
#define __GUI_APP        2
__MINGW_IMPORT void __set_app_type(int);
#endif /* __MSVCRT__ */

/*
 * Setup the default file handles to have the _CRT_fmode mode, as well as
 * any new files created by the user.
 */
extern unsigned int _CRT_fmode;

static void
_mingw32_init_fmode ()
{
  /* Don't set the file mode if the user hasn't set any value for it. */
  if (_CRT_fmode)
    {
      _fmode = _CRT_fmode;

      /*
       * This overrides the default file mode settings for stdin,
       * stdout and stderr. At first I thought you would have to
       * test with isatty, but it seems that the DOS console at
       * least is smart enough to handle _O_BINARY stdout and
       * still display correctly.
       */
      if (stdin)
	{
	  _setmode (_fileno (stdin), _CRT_fmode);
	}
      if (stdout)
	{
	  _setmode (_fileno (stdout), _CRT_fmode);
	}
      if (stderr)
	{
	  _setmode (_fileno (stderr), _CRT_fmode);
	}
    }
}

/* This function will be called when a trap occurs. Thanks to Jacob
   Navia for his contribution. */
static CALLBACK long
_gnu_exception_handler (EXCEPTION_POINTERS * exception_data)
{
  void (*old_handler) (int);
  long action = EXCEPTION_CONTINUE_SEARCH;
  int reset_fpu = 0;

  switch (exception_data->ExceptionRecord->ExceptionCode)
    {
    case EXCEPTION_ACCESS_VIOLATION:
      /* test if the user has set SIGSEGV */
      old_handler = signal (SIGSEGV, SIG_DFL);
      if (old_handler == SIG_IGN)
	{
	  /* this is undefined if the signal was raised by anything other
	     than raise ().  */
	  signal (SIGSEGV, SIG_IGN);
	  action = EXCEPTION_CONTINUE_EXECUTION;
	}
      else if (old_handler != SIG_DFL)
	{
	  /* This means 'old' is a user defined function. Call it */
	  (*old_handler) (SIGSEGV);
	  action = EXCEPTION_CONTINUE_EXECUTION;
	}
      break;

    case EXCEPTION_FLT_INVALID_OPERATION:
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
    case EXCEPTION_FLT_DENORMAL_OPERAND:
    case EXCEPTION_FLT_OVERFLOW:
    case EXCEPTION_FLT_UNDERFLOW:
    case EXCEPTION_FLT_INEXACT_RESULT:
      reset_fpu = 1;
      /* fall through. */

    case EXCEPTION_INT_DIVIDE_BY_ZERO:
      /* test if the user has set SIGFPE */
      old_handler = signal (SIGFPE, SIG_DFL);
      if (old_handler == SIG_IGN)
	{
	  signal (SIGFPE, SIG_IGN);
	  if (reset_fpu)
	    _fpreset ();
	  action = EXCEPTION_CONTINUE_EXECUTION;
	}
      else if (old_handler != SIG_DFL)
	{
	  /* This means 'old' is a user defined function. Call it */
	  (*old_handler) (SIGFPE);
	  action = EXCEPTION_CONTINUE_EXECUTION;
	}
      break;

    default:
      break;
    }
  return action;
}

/*
 * The function mainCRTStartup is the entry point for all console programs.
 */
static int
__mingw_CRTStartup ()
{
  int nRet;

  /*
   * Set up the top-level exception handler so that signal handling
   * works as expected. The mapping between ANSI/POSIX signals and
   * Win32 SE is not 1-to-1, so caveat emptore.
   * 
   */
  SetUnhandledExceptionFilter (_gnu_exception_handler);

  /*
   * Initialize floating point unit.
   */
  _fpreset ();			/* Supplied by the runtime library. */

  /*
   * Set up __argc, __argv and _environ.
   */
  _mingw32_init_mainargs ();

  /*
   * Sets the default file mode for stdin, stdout and stderr, as well
   * as files later opened by the user, to _CRT_fmode.
   * NOTE: DLLs don't do this because that would be rude!
   */
  _mingw32_init_fmode ();

  /*
   * Call the main function. If the user does not supply one
   * the one in the 'libmingw32.a' library will be linked in, and
   * that one calls WinMain. See main.c in the 'lib' dir
   * for more details.
   */
  nRet = main (_argc, _argv, environ);

  /*
   * Perform exit processing for the C library. This means
   * flushing output and calling 'atexit' registered functions.
   */
  _cexit ();

  ExitProcess (nRet);

  return 0;
}

/*
 * The function mainCRTStartup is the entry point for all console programs.
 */
int
mainCRTStartup ()
{
#ifdef __MSVCRT__
  __set_app_type (__CONSOLE_APP);
#endif
  __mingw_CRTStartup ();
  return 0;
}

/*
 * For now the GUI startup function is the same as the console one.
 * This simply gets rid of the annoying warning about not being able
 * to find WinMainCRTStartup when linking GUI applications.
 */
int
WinMainCRTStartup ()
{
#ifdef __MSVCRT__
  __set_app_type (__GUI_APP);
#endif
  __mingw_CRTStartup ();
}
