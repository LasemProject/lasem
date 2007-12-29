INCLUDES = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" $(GMATHML_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = gmathml-test

gmathml_test_SOURCES = 			\
	gdomdocument.c			\
	gdomnode.c			\
	gdomnodelist.c			\
	gdomelement.c			\
	gdomcharacterdata.c		\
	gdomtext.c			\
	gmathmlelement.c		\
	gmathmlpresentationtoken.c	\
	gmathmlnumberelement.c		\
	gmathmlpresentationcontainer.c	\
	gmathmlrowelement.c		\
	gmathml-test.c

gmathml_test_LDFLAGS =

gmathml_test_LDADD = $(GMATHML_LIBS)

