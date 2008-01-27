INCLUDES = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" $(GMATHML_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = gmathmlsvg

gmathmlsvg_SOURCES = 			\
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
	gmathmlattributes.c		\
	gmathmlview.c			\
	gmathmlutils.c			\
	gmathmlparser.c			\
	gmathmlsvg.c

gmathmlsvg_LDFLAGS =

gmathmlsvg_LDADD = $(GMATHML_LIBS)

