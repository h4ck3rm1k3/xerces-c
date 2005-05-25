dnl @synopsis XERCES_TYPE_16BIT_INT
dnl
dnl Determines what type to use a 16 bit int
dnl defines TYPE_16BIT_INT if it is found.
dnl
dnl @category C
dnl @author James Berry
dnl @version 2005-02-20
dnl @license AllPermissive
dnl
dnl $Id$

AC_DEFUN([XERCES_TYPE_16BIT_INT],
	[
	AC_CHECK_SIZEOF(short)
	AC_CHECK_SIZEOF(int)
	AC_CACHE_CHECK([for an appropriate 16 bit integer type], [xerces_cv_type_16bit_int],
			[
			 if test $ac_cv_sizeof_int  -eq 2; then
			  xerces_cv_type_16bit_int=int
			 elif test $ac_cv_sizeof_short -eq 2; then
			  xerces_cv_type_16bit_int=short
			 else
			  AC_MSG_ERROR([Couldn't find a 16 bit int type])
			 fi
			]
		)
	AC_DEFINE_UNQUOTED(TYPE_16BIT_INT, $xerces_cv_type_16bit_int, [An appropriate 16 bit integer type])
	]
)

