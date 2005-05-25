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
	
	AC_MSG_CHECKING([for which Mutex Manager to use])
	mgr=
	
	# Platform specific checks
	case $host_os in
	windows* | cygwin*)
		#mgr=Windows
		;;
	esac
	
	# Fall back to using posix mutex
	AS_IF([test -z "$mgr"],
		[mgr=POSIX;
			AC_DEFINE([XERCES_USE_POSIX_MUTEXMGR], 1, [Define to use the POSIX mutex mgr])
		])

	AC_MSG_RESULT($mgr)
	
	# Define the auto-make conditionals which determine what actually gets compiled
	# Note that these macros can't be executed conditionally, which is why they're here, not above.
	AM_CONDITIONAL([XERCES_USE_POSIX_MUTEXMGR],	[test x"$mgr" = xPOSIX])
	
	]
)


