dnl @synopsis XERCES_NETACCESSOR_SELECTION
dnl
dnl Determines the which netaccessor to use
dnl
dnl @category C
dnl @author James Berry
dnl @version 2005-05-23
dnl @license AllPermissive
dnl
dnl $Id$

AC_DEFUN([XERCES_NETACCESSOR_SELECTION],
	[

	######################################################
	# Test for availability of each netaccessor on this host.
	# For each netaccessor that's available, and hasn't been disabled, add it to our list.
	# If the netaccessor has been explicitly "enable"d, then vote for it strongly,
	# in upper case.
	######################################################
	na_list=
	
	AC_MSG_CHECKING([for whether we can support the libcurl-based NetAccessor])
	list_add=
	# TODO: netaccessor-curl is disabled for now until actually written: (false just below)
	AS_IF([false && test x"$xerces_cv_curl_prefix" != x], [
		AC_ARG_ENABLE([netaccessor-curl],
			AS_HELP_STRING([--enable-netaccessor-curl],
				[Enable libcurl-based NetAccessor support]),
			[AS_IF([test x"$enableval" = xyes],
				[list_add=CURL])],
			[list_add=curl])
	])
	AS_IF([test x"$list_add" != x],
		[na_list="$na_list $list_add"; AC_MSG_RESULT(yes)],
		[AC_MSG_RESULT(no)]
	)
	
	
	AC_MSG_CHECKING([for whether we can support the libwww-based NetAccessor])
	list_add=
	AS_IF([test x"$libwww_dir" != x], [
		AC_ARG_ENABLE([netaccessor-libwww],
			AS_HELP_STRING([--enable-netaccessor-libwww],
				[Enable libwww-based NetAccessor support]),
			[AS_IF([test x"$enableval" = xyes],
				[list_add=LIBWWW])],
			[na_list=libwww])
	])
	AS_IF([test x"$list_add" != x],
		[na_list="$na_list $list_add"; AC_MSG_RESULT(yes)],
		[AC_MSG_RESULT(no)]
	)
	
	
	AC_MSG_CHECKING([for whether we can support the sockets-based NetAccessor])
	list_add=
	AS_IF([test x"$ac_cv_header_sys_socket_h" = xyes],
		[AC_ARG_ENABLE([netaccessor-socket],
			AS_HELP_STRING([--enable-netaccessor-socket],
				[Enable sockets-based NetAccessor support]),
			[AS_IF([test x"$enableval" = xyes],
				[list_add=SOCKET])],
			[list_add=socket])
		])
	AS_IF([test x"$list_add" != x],
		[na_list="$na_list $list_add"; AC_MSG_RESULT(yes)],
		[AC_MSG_RESULT(no)]
	)
	
	
	AC_MSG_CHECKING([for whether we can support the CFURL NetAccessor (Mac OS X)])
	list_add=
	case $host_os in
	darwin*)
		if test x"$ac_cv_header_CoreServices_CoreServices_h" = xyes; then
		AC_ARG_ENABLE([netaccessor-cfurl],
			AS_HELP_STRING([--enable-netaccessor-cfurl],
				[Enable cfurl-based NetAccessor support]),
			[AS_IF([test x"$enableval" = xyes],
				[list_add=CFURL])],
			[list_add=cfurl])
		fi
		;;
	esac
	AS_IF([test x"$list_add" != x],
		[na_list="$na_list $list_add"; AC_MSG_RESULT(yes)],
		[AC_MSG_RESULT(no)]
	)
	
	
	# TODO: FINALIZE THIS TEST FOR AVAILABILITY OF WINSOCK NETACCESSOR
	AC_MSG_CHECKING([for whether we can support the WinSock NetAccessor (Windows)])
	list_add=
	case $host_os in
	some-test-for-windows-os*)
		AC_ARG_ENABLE([netaccessor-winsock],
			AS_HELP_STRING([--enable-netaccessor-winsock],
				[Enable winsock-based NetAccessor support]),
			[AS_IF([test x"$enableval" = xyes],
				[list_add=WINSOCK])],
			[list_add=winsock])
		;;
	esac
	AS_IF([test x"$list_add" != x],
		[na_list="$na_list $list_add"; AC_MSG_RESULT(yes)],
		[AC_MSG_RESULT(no)]
	)
	
	
	######################################################
	# Determine which netaccessor to use.
	#
	# We do this in two passes. Accessors that have been enabled with "yes",
	# and which start out in upper case, get the top priority on the first pass.
	# On the second pass, we consider those which are simply available, but
	# which were not "disable"d (these won't even be in our list).
	######################################################
	netaccessor=
	AC_MSG_CHECKING([for which NetAccessor to use (choices:$na_list)])
	for i in 1 2; do
		# Swap upper/lower case in string:
		na_list=`echo $na_list | tr 'A-Za-z' 'a-zA-Z'`
		
		# Check for each netaccessor, in implicit rank order
		case $na_list in
		*curl*)
			AC_DEFINE([XERCES_USE_NETACCESSOR_CURL], 1, [Define to use the CURL NetAccessor])
			netaccessor=curl
			break
			;;
		*cfurl*)
			AC_DEFINE([XERCES_USE_NETACCESSOR_CFURL], 1, [Define to use the Mac OS X CFURL NetAccessor])
			netaccessor=cfurl
			XERCES_LINK_DARWIN_FRAMEWORK([CoreServices])
			break
			;;
		*winsock*)
			AC_DEFINE([XERCES_USE_NETACCESSOR_WINSOCK], 1, [Define to use the WinSock NetAccessor])
			netaccessor=winsock
			break
			;;
		*socket*)
			AC_DEFINE([XERCES_USE_NETACCESSOR_SOCKET], 1, [Define to use the Sockets-based NetAccessor])
			netaccessor=socket
			break
			;;
		*libwww*)
			AC_DEFINE([XERCES_USE_NETACCESSOR_LIBWWW], 1, [Define to use the libwww NetAccessor])
			netaccessor=libwww
			break
			;;
		*)
			if [test $i -eq 2]; then
				AC_MSG_RESULT([none available; there will be no network access!!!])
			fi
			;;
		esac
	done
	if test x"$netaccessor" != x; then
		AC_MSG_RESULT($netaccessor)
	fi
	
	# Define the auto-make conditionals which determine what actually gets compiled
	# Note that these macros can't be executed conditionally, which is why they're here, not above.
	AM_CONDITIONAL([XERCES_USE_NETACCESSOR_CURL],		[test x"$netaccessor" = xcurl])
	AM_CONDITIONAL([XERCES_USE_NETACCESSOR_CFURL],		[test x"$netaccessor" = xcfurl])
	AM_CONDITIONAL([XERCES_USE_NETACCESSOR_WINSOCK],	[test x"$netaccessor" = xwinsockl])
	AM_CONDITIONAL([XERCES_USE_NETACCESSOR_SOCKET],		[test x"$netaccessor" = xsocket])
	AM_CONDITIONAL([XERCES_USE_NETACCESSOR_LIBWWW],		[test x"$netaccessor" = xlibwww])

	]
)

