AC_PREREQ(2.54)

m4_define([lasem_version_major], [0])
m4_define([lasem_version_minor], [1])
m4_define([lasem_version_micro], [0])
m4_define([lasem_version], [lasem_version_major.lasem_version_minor.lasem_version_micro])

AC_INIT([lasem], [lasem_version])

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AM_CONFIG_HEADER(config.h)
AM_MAINTAINER_MODE
AC_CONFIG_MACRO_DIR([m4])

AC_ISC_POSIX
AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC

AM_PROG_LEX
AC_PROG_YACC

GETTEXT_PACKAGE=lasem
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [GETTEXT package name])
AM_GLIB_GNU_GETTEXT
IT_PROG_INTLTOOL([0.35.0])

AM_PROG_LIBTOOL

PKG_CHECK_MODULES(LASEM, [gobject-2.0 glib-2.0 gio-2.0 cairo >= 1.2 pangocairo >= 1.16.0 libxml-2.0])
AC_SUBST(LASEM_CFLAGS)
AC_SUBST(LASEM_LIBS)

AC_OUTPUT([
Makefile
itex2mml/Makefile
src/Makefile
po/Makefile.in
lasem.pc
])
