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

libgmathml_la_LIBADD = 			\
	../itex2mml/libitex2mml.la	\
	$(GMATHML_LIBS)

libgmathml_la_SOURCES =			\
	gdomdebug.c			\
	gdomnode.c			\
	gdomnodelist.c			\
	gdomdocument.c			\
	gdomelement.c			\
	gdomcharacterdata.c		\
	gdomtext.c			\
	gdomattributes.c		\
	gdomview.c			\
	gdomparser.c			\
	gdomimplementation.c		\
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
	gmathmlstringelement.c		\
	gmathmlpresentationcontainer.c	\
	gmathmlstyleelement.c		\
	gmathmlrowelement.c		\
	gmathmlfencedelement.c		\
	gmathmlpaddedelement.c		\
	gmathmlerrorelement.c		\
	gmathmlactionelement.c		\
	gmathmlattributes.c		\
	gmathmlstyle.c			\
	gmathmlview.c			\
	gmathmlglyphtableams.c		\
	gmathmlalignmarkelement.c	\
	gmathmlaligngroupelement.c	\
	gmathmlutils.c			\
	gmathmllayoututils.c		\
	gsvgdocument.c			\
	gsvgelement.c			\
	gsvggraphic.c			\
	gsvgsvgelement.c		\
	gsvggelement.c			\
	gsvgrectelement.c		\
	gsvgcircleelement.c		\
	gsvgellipseelement.c		\
	gsvglineelement.c		\
	gsvgpolylineelement.c		\
	gsvgpolygonelement.c		\
	gsvgpathelement.c		\
	gsvgtextelement.c		\
	gsvgenums.c			\
	gsvgattributes.c		\
	gsvgattributebags.c		\
	gsvgstyle.c			\
	gsvgcolors.c			\
	gsvgutils.c			\
	gsvgview.c			\
	gsvgmatrix.c

libgmathml_ladir = $(includedir)/gmathml
libgmathml_la_HEADERS = \
	gdom.h				\
	gdomdebug.h			\
	gdomnode.h			\
	gdomnodelist.h			\
	gdomdocument.h			\
	gdomelement.h			\
	gdomcharacterdata.h		\
	gdomtext.h			\
	gdomattributes.h		\
	gdomview.h			\
	gdomparser.h			\
	gdomimplementation.h		\
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
	gmathmlstringelement.h		\
	gmathmlpresentationcontainer.h	\
	gmathmlstyleelement.h		\
	gmathmlrowelement.h		\
	gmathmlfencedelement.h		\
	gmathmlpaddedelement.h		\
	gmathmlerrorelement.h		\
	gmathmlactionelement.h		\
	gmathmlattributes.h		\
	gmathmlstyle.h		\
	gmathmlview.h			\
	gmathmlglyphtableams.h		\
	gmathmlalignmarkelement.h	\
	gmathmlaligngroupelement.h	\
	gmathmlenums.h			\
	gmathmlutils.h			\
	gmathmllayoututils.h		\
	gsvgdocument.h			\
	gsvgelement.h			\
	gsvggraphic.h			\
	gsvgsvgelement.h		\
	gsvggelement.h			\
	gsvgrectelement.h		\
	gsvgcircleelement.h		\
	gsvgellipseelement.h		\
	gsvglineelement.h		\
	gsvgpolylineelement.h		\
	gsvgpolygonelement.h		\
	gsvgtextelement.h		\
	gsvgenums.h			\
	gsvgattributes.h		\
	gsvgattributebags.h		\
	gsvgstyle.h			\
	gsvgcolors.h			\
	gsvgutils.h			\
	gsvgview.h			\
	gsvgmatrix.h

bin_PROGRAMS = gmathmltest gmathmlrender

gmathmlrender_SOURCES = 		\
	gmathmlrender.c

gmathmlrender_LDFLAGS =

gmathmlrender_LDADD = $(GMATHML_LIBS) libgmathml.la ../itex2mml/libitex2mml.la

gmathmltest_SOURCES = 			\
	gmathmltest.c

gmathmltest_LDFLAGS =

gmathmltest_LDADD = $(GMATHML_LIBS) libgmathml.la ../itex2mml/libitex2mml.la

