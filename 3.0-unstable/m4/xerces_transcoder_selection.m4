dnl @synopsis XERCES_TRANSCODER_SELECTION
dnl
dnl Determines the which transcoder to use
dnl
dnl @category C
dnl @author James Berry
dnl @version 2005-05-23
dnl @license AllPermissive
dnl
dnl $Id$

AC_DEFUN([XERCES_TRANSCODER_SELECTION],
	[

	######################################################
	# Test for availability of each transcoder on this host.
	# For each transcoder that's available, and hasn't been disabled, add it to our list.
	# If the transcoder has been explicitly "enable"d, then vote for it strongly,
	# in upper case.
	######################################################
	
	tc_list=
	
	# Check for iconv support
	no_iconv=false
	AC_CHECK_HEADERS([wchar.h], [], [no_iconv=true])
	AC_CHECK_FUNCS([mblen wcstombs mbstowcs], [], [no_iconv=true])
	AC_MSG_CHECKING([for whether we can support the iconv Transcoder])
	list_add=
	AS_IF([! $no_iconv], [
		AC_ARG_ENABLE([transcoder-iconv],
			AS_HELP_STRING([--enable-transcoder-iconv],
				[Enable iconv-based transcoder support]),
			[AS_IF([test x"$enableval" = xyes],
				[list_add=ICONV])],
			[list_add=iconv])
	])
	AS_IF([test x"$list_add" != x],
		[tc_list="$tc_list -$list_add-"; AC_MSG_RESULT(yes)],
		[AC_MSG_RESULT(no)]
	)

	# Check for ICU
	AC_REQUIRE([XERCES_ICU_PREFIX])
	AC_MSG_CHECKING([for whether we can support the ICU Transcoder])
	list_add=
	AS_IF([test x"$xerces_cv_icu_prefix" != x], [
		AC_ARG_ENABLE([transcoder-icu],
			AS_HELP_STRING([--enable-transcoder-icu],
				[Enable icu-based transcoder support]),
			[AS_IF([test x"$enableval" = xyes],
				[list_add=ICU])],
			[list_add=icu])
	])
	AS_IF([test x"$list_add" != x],
		[tc_list="$tc_list -$list_add-"; AC_MSG_RESULT(yes)],
		[AC_MSG_RESULT(no)]
	)
	
	
	# Check for MacOSUnicodeConverter
	AC_MSG_CHECKING([for whether we can support the MacOSUnicodeConverter Transcoder])
	list_add=
	case $host_os in
	darwin*)
		if test x"$ac_cv_header_CoreServices_CoreServices_h" = xyes; then
		AC_ARG_ENABLE([transcoder-macosunicodeconverter],
			AS_HELP_STRING([--enable-transcoder-macosunicodeconverter],
				[Enable MacOSUnicodeConverter-based transcoder support]),
			[AS_IF([test x"$enableval" = xyes],
				[list_add=MACOSUNICODECONVERTER])],
			[list_add=macosunicodeconverter])
		fi
		;;
	esac
	AS_IF([test x"$list_add" != x],
		[tc_list="$tc_list -$list_add-"; AC_MSG_RESULT(yes)],
		[AC_MSG_RESULT(no)]
	)

	# TODO: Tests for additional transcoders
	
	
	######################################################
	# Determine which transcoder to use.
	#
	# We do this in two passes. Transcoders that have been enabled with "yes",
	# and which start out in upper case, get the top priority on the first pass.
	# On the second pass, we consider those which are simply available, but
	# which were not "disable"d (these won't even be in our list).
	######################################################
	transcoder=
	AC_MSG_CHECKING([for which Transcoder to use (choices:$tc_list)])
	for i in 1 2; do
		# Swap upper/lower case in the tc_list
		tc_list=`echo $tc_list | tr 'A-Za-z' 'a-zA-Z'`
		
		# Check for each transcoder, in implicit rank order
		case $tc_list in
		
		*-icu-*)
			transcoder=icu
			AC_DEFINE([XERCES_USE_TRANSCODER_ICU], 1, [Define to use the ICU-based transcoder])
			LIBS="${LIBS} -L${xerces_cv_icu_prefix}/lib -licuuc -licudata"
			break
			;;
			
		*-macosunicodeconverter-*)
			transcoder=macosunicodeconverter
			AC_DEFINE([XERCES_USE_TRANSCODER_MACOSUNICODECONVERTER], 1, [Define to use the Mac OS UnicodeConverter-based transcoder])
			XERCES_LINK_DARWIN_FRAMEWORK([CoreServices])
			break
			;;

		*-iconv-*)
			transcoder=iconv
			AC_DEFINE([XERCES_USE_TRANSCODER_ICONV], 1, [Define to use the iconv transcoder])
			break
			;;
			
		*)
			if [test $i -eq 2]; then
				AC_MSG_RESULT([none])
				AC_MSG_ERROR([Xerces cannot function without a transcoder])
			fi
			;;
		esac
	done
	if test x"$transcoder" != x; then
		AC_MSG_RESULT($transcoder)
	fi
	
	# Define the auto-make conditionals which determine what actually gets compiled
	# Note that these macros can't be executed conditionally, which is why they're here, not above.
	AM_CONDITIONAL([XERCES_USE_TRANSCODER_ICU],						[test x"$transcoder" = xicu])
	AM_CONDITIONAL([XERCES_USE_TRANSCODER_MACOSUNICODECONVERTER],	[test x"$transcoder" = xmacosunicodeconverter])
	AM_CONDITIONAL([XERCES_USE_TRANSCODER_ICONV],					[test x"$transcoder" = xiconv])

	]
)

