INCLUDES = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" $(GMATHML_CFLAGS) \
	-DG_LOG_DOMAIN=\"GMathml\"

AM_CFLAGS =\
	 -Wall\
	 -Werror \
	 -g

lib_LTLIBRARIES = libgmathml.la

libgmathml_la_SOURCES =			\
	gdomdebug.c			\
	gdomnode.c			\
	gdomnodelist.c			\
	gdomdocument.c			\
	gdomelement.c			\
	gdomcharacterdata.c		\
	gdomtext.c			\
	gmathmlenums.c			\
	gmathmlentitydictionary.c	\
	gmathmloperatordictionary.c	\
	gmathmldocument.c		\
	gmathmlelement.c		\
	gmathmlphantomelement.c		\
	gmathmlmathelement.c		\
	gmathmltableelement.c		\
	gmathmltablerowelement.c	\
	gmathmltablecellelement.c	\
	gmathmlspaceelement.c		\
	gmathmlradicalelement.c		\
	gmathmlscriptelement.c		\
	gmathmlfractionelement.c	\
	gmathmlunderoverelement.c	\
	gmathmlpresentationtoken.c	\
	gmathmloperatorelement.c	\
	gmathmlpresentationcontainer.c	\
	gmathmlstyleelement.c		\
	gmathmlrowelement.c		\
	gmathmlfencedelement.c		\
	gmathmlpaddedelement.c		\
	gmathmlerrorelement.c		\
	gmathmlattributes.c		\
	gmathmlview.c			\
	gmathmlglyphtableams.c		\
	gmathmlutils.c			\
	gmathmllayoututils.c		\
	gmathmlparser.c

libgmathml_ladir = $(includedir)/gmathml
libgmathml_la_HEADERS = \
	gdom.h				\
	gdomnode.h			\
	gdomnodelist.h			\
	gdomdocument.h			\
	gdomelement.h			\
	gdomcharacterdata.h		\
	gdomtext.h			\
	gmathml.h			\
	gmathmlentitydictionary.h	\
	gmathmloperatordictionary.h	\
	gmathmldocument.h		\
	gmathmlelement.h		\
	gmathmlphantomelement.h		\
	gmathmlmathelement.h		\
	gmathmltableelement.h		\
	gmathmltablerowelement.h	\
	gmathmltablecellelement.h	\
	gmathmlspaceelement.h		\
	gmathmlradicalelement.h		\
	gmathmlscriptelement.h		\
	gmathmlfractionelement.h	\
	gmathmlunderoverelement.h	\
	gmathmlpresentationtoken.h	\
	gmathmloperatorelement.h	\
	gmathmlpresentationcontainer.h	\
	gmathmlstyleelement.h		\
	gmathmlrowelement.h		\
	gmathmlfencedelement.h		\
	gmathmlpaddedelement.h		\
	gmathmlerrorelement.h		\
	gmathmlattributes.h		\
	gmathmlview.h			\
	gmathmlglyphtableams.h		\
	gmathmlenums.h			\
	gmathmlutils.h			\
	gmathmllayoututils.h		\
	gmathmlparser.h


bin_PROGRAMS = gmathmltest gmathmlrender

gmathmlrender_SOURCES = 		\
	gmathmlrender.c

gmathmlrender_LDFLAGS =

gmathmlrender_LDADD = $(GMATHML_LIBS) libgmathml.la ../itex2mml/libitex2mml.la

gmathmltest_SOURCES = 			\
	gmathmltest.c

gmathmltest_LDFLAGS =

gmathmltest_LDADD = $(GMATHML_LIBS) libgmathml.la ../itex2mml/libitex2mml.la

