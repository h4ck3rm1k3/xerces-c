dnl @synopsis XERCES_MUTEXMGR_SELECTION
dnl
dnl Determines the which XMLMutexMgr to use
dnl
dnl @category C
dnl @author James Berry
dnl @version 2005-05-25
dnl @license AllPermissive
dnl
dnl $Id$

AC_DEFUN([XERCES_MUTEXMGR_SELECTION],
	[
	AC_REQUIRE([XERCES_NO_THREADS])
	
	AC_MSG_CHECKING([for which Mutex Manager to use])
	mgr=
	
	# If no threads is specified, use the NoThread Mutex Mgr
	AS_IF([test x$xerces_cv_no_threads = xyes],
		[
			mgr=NoThreads
			AC_DEFINE([XERCES_USE_MUTEXMGR_NOTHREAD], 1, [Define to use the NoThread Mutex mgr])
		])
	
	# Platform specific checks
	AS_IF([test -z "$mgr"],
		[
			case $host_os in
			windows* | cygwin*)
				#mgr=Windows
				;;
			esac
		])
	
	# Fall back to using posix mutex
	AS_IF([test -z "$mgr"],
		[mgr=POSIX;
			AC_DEFINE([XERCES_USE_MUTEXMGR_POSIX], 1, [Define to use the POSIX mutex mgr])
		])

	AC_MSG_RESULT($mgr)
	
	# Define the auto-make conditionals which determine what actually gets compiled
	# Note that these macros can't be executed conditionally, which is why they're here, not above.
	AM_CONDITIONAL([XERCES_USE_MUTEXMGR_NOTHREAD],	[test x"$mgr" = xNoThreads])
	AM_CONDITIONAL([XERCES_USE_MUTEXMGR_POSIX],		[test x"$mgr" = xPOSIX])
	
	]
)


