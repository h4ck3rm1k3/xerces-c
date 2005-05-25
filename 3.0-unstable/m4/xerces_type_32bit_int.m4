dnl @synopsis XERCES_TYPE_32BIT_INT
dnl
dnl Determines what type to use a 32 bit int
dnl defines TYPE_32BIT_INT if it is found.
dnl
dnl @category C
dnl @author James Berry
dnl @version 2005-02-20
dnl @license AllPermissive
dnl
dnl $Id$

AC_DEFUN([XERCES_TYPE_32BIT_INT],
	[
	AC_CHECK_SIZEOF(int)
	AC_CHECK_SIZEOF(long)
	AC_CACHE_CHECK([for an appropriate 32 bit integer type], [xerces_cv_type_32bit_int],
			[
			 if test $ac_cv_sizeof_int -eq 4; then
			  xerces_cv_type_32bit_int=int
			 elif test $ac_cv_sizeof_long -eq 4; then
			  xerces_cv_type_32bit_int=long
			 else
			  AC_MSG_ERROR([Couldn't find a 32 bit int type])
			 fi
			]
		)
	AC_DEFINE_UNQUOTED(TYPE_32BIT_INT, $xerces_cv_type_32bit_int, [An appropriate 32 bit integer type])
	]
)

