INCLUDES = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" $(GMATHML_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = gmathml-test

gmathml_test_SOURCES = 			\
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
	gmathmlmathelement.c		\
	gmathmlradicalelement.c		\
	gmathmlscriptelement.c		\
	gmathmlfractionelement.c	\
	gmathmlpresentationtoken.c	\
	gmathmloperatorelement.c	\
	gmathmlpresentationcontainer.c	\
	gmathmlrowelement.c		\
	gmathmlattributes.c		\
	gmathmlview.c			\
	gmathmlutils.c			\
	gmathmlparser.c			\
	gmathml-test.c

gmathml_test_LDFLAGS =

gmathml_test_LDADD = $(GMATHML_LIBS)

