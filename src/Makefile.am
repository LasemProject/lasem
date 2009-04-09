INCLUDES = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" $(LASEM_CFLAGS) \
	-DG_LOG_DOMAIN=\"Lasem\"

AM_CFLAGS =\
	 -Wall\
	 -Werror \
	 -g

lib_LTLIBRARIES = liblasem.la

liblasem_la_LIBADD = 				\
	../itex2mml/libitex2mml.la		\
	$(LASEM_LIBS)

liblasem_la_SOURCES =				\
	lsmdebug.c				\
	lsmdomnode.c				\
	lsmdomnodelist.c			\
	lsmdomdocument.c			\
	lsmdomelement.c				\
	lsmdomcharacterdata.c			\
	lsmdomtext.c				\
	lsmdomattributes.c			\
	lsmdomview.c				\
	lsmdomparser.c				\
	lsmdomimplementation.c			\
	lsmmathmlenums.c			\
	lsmmathmlentitydictionary.c		\
	lsmmathmloperatordictionary.c		\
	lsmmathmldocument.c			\
	lsmmathmlelement.c			\
	lsmmathmlphantomelement.c		\
	lsmmathmlmathelement.c			\
	lsmmathmltableelement.c			\
	lsmmathmltablerowelement.c		\
	lsmmathmltablecellelement.c		\
	lsmmathmlspaceelement.c			\
	lsmmathmlradicalelement.c		\
	lsmmathmlscriptelement.c		\
	lsmmathmlfractionelement.c		\
	lsmmathmlunderoverelement.c		\
	lsmmathmlpresentationtoken.c		\
	lsmmathmloperatorelement.c		\
	lsmmathmlstringelement.c		\
	lsmmathmlpresentationcontainer.c	\
	lsmmathmlstyleelement.c			\
	lsmmathmlrowelement.c			\
	lsmmathmlfencedelement.c		\
	lsmmathmlpaddedelement.c		\
	lsmmathmlerrorelement.c			\
	lsmmathmlactionelement.c		\
	lsmmathmlattributes.c			\
	lsmmathmlstyle.c			\
	lsmmathmlview.c				\
	lsmmathmlglyphtableams.c		\
	lsmmathmlalignmarkelement.c		\
	lsmmathmlaligngroupelement.c		\
	lsmmathmlutils.c			\
	lsmmathmllayoututils.c			\
	lsmsvgdocument.c			\
	lsmsvgelement.c				\
	lsmsvggraphic.c				\
	lsmsvgsvgelement.c			\
	lsmsvguseelement.c			\
	lsmsvgsymbolelement.c			\
	lsmsvggelement.c			\
	lsmsvgdefselement.c			\
	lsmsvgrectelement.c			\
	lsmsvgcircleelement.c			\
	lsmsvgellipseelement.c			\
	lsmsvglineelement.c			\
	lsmsvgpolylineelement.c			\
	lsmsvgpolygonelement.c			\
	lsmsvgpathelement.c			\
	lsmsvgtextelement.c			\
	lsmsvggradientelement.c			\
	lsmsvglineargradientelement.c		\
	lsmsvgradialgradientelement.c		\
	lsmsvgstopelement.c			\
	lsmsvgpatternelement.c			\
	lsmsvgenums.c				\
	lsmsvgattributes.c			\
	lsmsvgattributebags.c			\
	lsmsvgstyle.c				\
	lsmsvgcolors.c				\
	lsmsvgutils.c				\
	lsmsvgview.c				\
	lsmsvgmatrix.c

liblasem_ladir = $(includedir)/lasem
liblasem_la_HEADERS = \
	lsmdebug.h				\
	lsmdom.h				\
	lsmdomnode.h				\
	lsmdomnodelist.h			\
	lsmdomdocument.h			\
	lsmdomelement.h				\
	lsmdomcharacterdata.h			\
	lsmdomtext.h				\
	lsmdomattributes.h			\
	lsmdomview.h				\
	lsmdomparser.h				\
	lsmdomimplementation.h			\
	lsmmathml.h				\
	lsmmathmlentitydictionary.h		\
	lsmmathmloperatordictionary.h		\
	lsmmathmldocument.h			\
	lsmmathmlelement.h			\
	lsmmathmlphantomelement.h		\
	lsmmathmlmathelement.h			\
	lsmmathmltableelement.h			\
	lsmmathmltablerowelement.h		\
	lsmmathmltablecellelement.h		\
	lsmmathmlspaceelement.h			\
	lsmmathmlradicalelement.h		\
	lsmmathmlscriptelement.h		\
	lsmmathmlfractionelement.h		\
	lsmmathmlunderoverelement.h		\
	lsmmathmlpresentationtoken.h		\
	lsmmathmloperatorelement.h		\
	lsmmathmlstringelement.h		\
	lsmmathmlpresentationcontainer.h	\
	lsmmathmlstyleelement.h			\
	lsmmathmlrowelement.h			\
	lsmmathmlfencedelement.h		\
	lsmmathmlpaddedelement.h		\
	lsmmathmlerrorelement.h			\
	lsmmathmlactionelement.h		\
	lsmmathmlattributes.h			\
	lsmmathmlstyle.h			\
	lsmmathmlview.h				\
	lsmmathmlglyphtableams.h		\
	lsmmathmlalignmarkelement.h		\
	lsmmathmlaligngroupelement.h		\
	lsmmathmlenums.h			\
	lsmmathmlutils.h			\
	lsmmathmllayoututils.h			\
	lsmsvgdocument.h			\
	lsmsvgelement.h				\
	lsmsvggraphic.h				\
	lsmsvgsvgelement.h			\
	lsmsvggelement.h			\
	lsmsvgdefselement.h			\
	lsmsvguseelement.h			\
	lsmsvgsymbolelement.h			\
	lsmsvgrectelement.h			\
	lsmsvgcircleelement.h			\
	lsmsvgellipseelement.h			\
	lsmsvglineelement.h			\
	lsmsvgpolylineelement.h			\
	lsmsvgpolygonelement.h			\
	lsmsvgtextelement.h			\
	lsmsvggradientelement.h			\
	lsmsvglineargradientelement.h		\
	lsmsvgradialgradientelement.h		\
	lsmsvgstopelement.h			\
	lsmsvgpatternelement.h			\
	lsmsvgenums.h				\
	lsmsvgattributes.h			\
	lsmsvgattributebags.h			\
	lsmsvgstyle.h				\
	lsmsvgcolors.h				\
	lsmsvgutils.h				\
	lsmsvgview.h				\
	lsmsvgmatrix.h

bin_PROGRAMS = lasemtest lasemrender

lasemrender_SOURCES = 				\
	lasemrender.c

lasemrender_LDFLAGS =

lasemrender_LDADD = $(LASEM_LIBS) liblasem.la ../itex2mml/libitex2mml.la

lasemtest_SOURCES = 				\
	lasemtest.c

lasemtest_LDFLAGS =

lasemtest_LDADD = $(LASEM_LIBS) liblasem.la ../itex2mml/libitex2mml.la
