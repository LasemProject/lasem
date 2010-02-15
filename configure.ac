AC_PREREQ(2.63.2)

m4_define([lasem_version_major], [0])
m4_define([lasem_version_minor], [1])
m4_define([lasem_version_micro], [4])
m4_define([lasem_version], [lasem_version_major.lasem_version_minor.lasem_version_micro])

AC_INIT([lasem],[lasem_version],[http://bugzilla.gnome.org/enter_bug.cgi?product=lasem],[lasem])

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AC_CONFIG_HEADERS([config.h])
AM_MAINTAINER_MODE
AC_CONFIG_MACRO_DIR([m4])

m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

AC_SEARCH_LIBS([strerror],[cposix])
AC_PROG_CC

AC_HEADER_STDC

AM_PROG_LEX
AC_PROG_YACC

GETTEXT_PACKAGE=lasem
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [GETTEXT package name])
AM_GLIB_GNU_GETTEXT
IT_PROG_INTLTOOL([0.35.0])

LT_INIT

PKG_CHECK_MODULES(LASEM, [gobject-2.0 glib-2.0 gio-2.0 gdk-pixbuf-2.0 >= 2.16 gdk-2.0 >= 2.16 cairo >= 1.2 pangocairo >= 1.16.0 libxml-2.0])
AC_SUBST(LASEM_CFLAGS)
AC_SUBST(LASEM_LIBS)

AC_PATH_PROG(GLIB_MKENUMS, glib-mkenums)

AC_CONFIG_FILES([
Makefile
itex2mml/Makefile
src/Makefile
po/Makefile.in
lasem.pc
])
AC_OUTPUT

echo ""
echo "Configuration:"
echo ""
echo "	Compiler:		${CC}"
echo "	Compiler flags:		${LASEM_CFLAGS}"
echo ""
