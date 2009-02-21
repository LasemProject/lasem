ACLOCAL_AMFLAGS = -I m4

SUBDIRS = itex2mml src po

gmathmldocdir = ${prefix}/doc/gmathml
gmathmldoc_DATA = 	\
	README		\
	COPYING		\
	ChangeLog 	\
	AUTHORS		\
	INSTALL		\
	NEWS

EXTRA_DIST = $(gmathmldoc_DATA)

dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = gmathml.pc
