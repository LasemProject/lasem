## Process this file with automake to produce Makefile.in
## Created by Anjuta

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

# Copy all the spec files. Of cource, only one is actually used.
dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = gmathml.pc
