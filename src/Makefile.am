INCLUDES = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" $(LASEM_CFLAGS) \
	-DG_LOG_DOMAIN=\"Lasem\"

AM_CFLAGS = -Wall

lib_LTLIBRARIES = liblasem-@LASEM_API_VERSION@.la

BUILT_SOURCES = lsmenumtypes.c lsmenumtypes.h

EXTRA_DIST = lsmenumtypes.c.template \
	     lsmenumtypes.h.template

liblasem_@LASEM_API_VERSION@_la_LIBADD = 				\
	../itex2mml/libitex2mml.la		\
	$(LASEM_LIBS)

liblasem_@LASEM_API_VERSION@_la_SOURCES =				\
	lsmenumtypes.c				\
	lsmdebug.c				\
	lsmstr.c				\
	lsmtraits.c				\
	lsmproperties.c				\
	lsmattributes.c				\
	lsmdomnode.c				\
	lsmdomnodelist.c			\
	lsmdomdocument.c			\
	lsmdomelement.c				\
	lsmdomcharacterdata.c			\
	lsmdomtext.c				\
	lsmdomview.c				\
	lsmdomparser.c				\
	lsmdomimplementation.c			\
	lsmmathmlenums.c			\
	lsmmathmltraits.c			\
	lsmmathmlattributes.c			\
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
	lsmmathmlstyle.c			\
	lsmmathmlview.c				\
	lsmmathmlglyphtableams.c		\
	lsmmathmlalignmarkelement.c		\
	lsmmathmlaligngroupelement.c		\
	lsmmathmlitexelement.c			\
	lsmmathmlutils.c			\
	lsmmathmllayoututils.c			\
	lsmsvgenums.c				\
	lsmsvgtraits.c				\
	lsmsvgstyle.c				\
	lsmsvgcolors.c				\
	lsmsvglength.c				\
	lsmsvgview.c				\
	lsmsvgmatrix.c				\
	lsmsvgdocument.c			\
	lsmsvgelement.c				\
	lsmsvgclippathelement.c			\
	lsmsvgsvgelement.c			\
	lsmsvguseelement.c			\
	lsmsvgimageelement.c			\
	lsmsvgsymbolelement.c			\
	lsmsvgmarkerelement.c			\
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
	lsmsvgmaskelement.c

LASEM_HDRS = \
	lsm.h					\
	lsmstr.h				\
	lsmdebug.h				\
	lsmtraits.h				\
	lsmproperties.h				\
	lsmattributes.h				\
	lsmdom.h				\
	lsmdomnode.h				\
	lsmdomnodelist.h			\
	lsmdomdocument.h			\
	lsmdomelement.h				\
	lsmdomcharacterdata.h			\
	lsmdomtext.h				\
	lsmdomview.h				\
	lsmdomparser.h				\
	lsmdomimplementation.h			\
	lsmmathml.h				\
	lsmmathmlenums.h			\
	lsmmathmltraits.h			\
	lsmmathmlattributes.h			\
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
	lsmmathmlstyle.h			\
	lsmmathmlview.h				\
	lsmmathmlglyphtableams.h		\
	lsmmathmlalignmarkelement.h		\
	lsmmathmlaligngroupelement.h		\
	lsmmathmlitexelement.h			\
	lsmmathmlutils.h			\
	lsmmathmllayoututils.h			\
	lsmsvg.h				\
	lsmsvgenums.h				\
	lsmsvgtraits.h				\
	lsmsvgattributes.h			\
	lsmsvgstyle.h				\
	lsmsvgcolors.h				\
	lsmsvglength.h				\
	lsmsvgview.h				\
	lsmsvgmatrix.h				\
	lsmsvgdocument.h			\
	lsmsvgelement.h				\
	lsmsvgclippathelement.h			\
	lsmsvgsvgelement.h			\
	lsmsvggelement.h			\
	lsmsvgdefselement.h			\
	lsmsvguseelement.h			\
	lsmsvgimageelement.h			\
	lsmsvgsymbolelement.h			\
	lsmsvgmarkerelement.h			\
	lsmsvgrectelement.h			\
	lsmsvgcircleelement.h			\
	lsmsvgellipseelement.h			\
	lsmsvglineelement.h			\
	lsmsvgpathelement.h			\
	lsmsvgpolylineelement.h			\
	lsmsvgpolygonelement.h			\
	lsmsvgtextelement.h			\
	lsmsvggradientelement.h			\
	lsmsvglineargradientelement.h		\
	lsmsvgradialgradientelement.h		\
	lsmsvgstopelement.h			\
	lsmsvgpatternelement.h			\
	lsmsvgmaskelement.h

liblasem_@LASEM_API_VERSION@_ladir = $(includedir)/lasem-@LASEM_API_VERSION@
liblasem_@LASEM_API_VERSION@_la_HEADERS = $(LASEM_HDRS)
liblasem_@LASEM_API_VERSION@_la_HEADERS += lsmenumtypes.h
liblasem_@LASEM_API_VERSION@_la_LDFLAGS = -version-info $(LASEM_LIBTOOL_VERSION)

lsmenumtypes.h: lsmenumtypes.h.template $(LASEM_HDRS) $(GLIB_MKENUMS)
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template lsmenumtypes.h.template $(liblasem_@LASEM_API_VERSION@_la_HEADERS)) > $@

lsmenumtypes.c: lsmenumtypes.c.template $(LASEM_HDRS) $(GLIB_MKENUMS)
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template lsmenumtypes.c.template $(liblasem_@LASEM_API_VERSION@_la_HEADERS)) > $@

bin_PROGRAMS = lasemrender
noinst_PROGRAMS = lasemtest

lasemrender_SOURCES = 				\
	lasemrender.c

lasemrender_LDFLAGS =

lasemrender_LDADD = $(LASEM_LIBS) liblasem-@LASEM_API_VERSION@.la ../itex2mml/libitex2mml.la

lasemtest_SOURCES = 				\
	lasemtest.c

lasemtest_LDFLAGS =

lasemtest_LDADD = $(LASEM_LIBS) liblasem-@LASEM_API_VERSION@.la ../itex2mml/libitex2mml.la

CLEAN_FILES = $(BUILT_SOURCES)
