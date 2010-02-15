ACLOCAL_AMFLAGS = -I m4

DISTCHECK_CONFIGURE_FLAGS = --enable-introspection

SUBDIRS = itex2mml src po

lasemdocdir = ${prefix}/doc/lasem
lasemdoc_DATA = 	\
	README		\
	COPYING		\
	ChangeLog 	\
	AUTHORS		\
	INSTALL		\
	NEWS

EXTRA_DIST = $(lasemdoc_DATA) m4/introspection.m4

# Copy all the spec files. Of cource, only one is actually used.
dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = lasem.pc
