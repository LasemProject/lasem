include $(top_srcdir)/Makefile.decl

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" $(LASEM_CFLAGS) \
	-DG_LOG_DOMAIN=\"Lasem\"

AM_CFLAGS = -Wall

lib_LTLIBRARIES = liblasem-@LASEM_API_VERSION@.la

BUILT_SOURCES = lsmdomenumtypes.h	\
		lsmdomenumtypes.c	\
		lsmmathmlenumtypes.h	\
		lsmmathmlenumtypes.c	\
		lsmsvgenumtypes.h	\
		lsmsvgenumtypes.c

EXTRA_DIST = lsmdomenumtypes.h.template 	\
	     lsmdomenumtypes.c.template		\
	     lsmmathmlenumtypes.h.template 	\
	     lsmmathmlenumtypes.c.template	\
	     lsmsvgenumtypes.h.template 	\
	     lsmsvgenumtypes.c.template

liblasem_@LASEM_API_VERSION@_la_LIBADD = 	\
	../itex2mml/libitex2mml.la		\
	$(LASEM_LIBS)

LASEM_DOM_SRCS =				\
	lsm.c					\
	lsmdebug.c				\
	lsmstr.c				\
	lsmutils.c				\
	lsmtraits.c				\
	lsmproperties.c				\
	lsmattributes.c				\
	lsmcairo.c				\
	lsmitex.c				\
	lsmdomentities.c			\
	lsmdomnode.c				\
	lsmdomnodelist.c			\
	lsmdomnamednodemap.c			\
	lsmdomdocument.c			\
	lsmdomdocumentfragment.c		\
	lsmdomelement.c				\
	lsmdomcharacterdata.c			\
	lsmdomtext.c				\
	lsmdomview.c				\
	lsmdomparser.c				\
	lsmdomimplementation.c

LASEM_DOM_BUILT_SRCS =				\
	lsmdomenumtypes.c			\
	lsmdomenumtypes.h

LASEM_MATHML_SRCS =				\
	lsmmathmlenumtypes.c			\
	lsmmathmlenums.c			\
	lsmmathmltraits.c			\
	lsmmathmlattributes.c			\
	lsmmathmloperatordictionary.c		\
	lsmmathmldocument.c			\
	lsmmathmlelement.c			\
	lsmmathmlsemanticselement.c		\
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
	lsmmathmlencloseelement.c		\
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
	lsmmathmllayoututils.c

LASEM_SVG_SRCS =				\
	lsmsvgenumtypes.c			\
	lsmsvgenums.c				\
	lsmsvgtraits.c				\
	lsmsvgstyle.c				\
	lsmsvgcolors.c				\
	lsmsvglength.c				\
	lsmsvgview.c				\
	lsmsvgmatrix.c				\
	lsmsvgdocument.c			\
	lsmsvgelement.c				\
	lsmsvgtransformable.c			\
	lsmsvgclippathelement.c			\
	lsmsvgsvgelement.c			\
	lsmsvguseelement.c			\
	lsmsvgimageelement.c			\
	lsmsvgsymbolelement.c			\
	lsmsvgmarkerelement.c			\
	lsmsvgaelement.c			\
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
	lsmsvgtspanelement.c			\
	lsmsvggradientelement.c			\
	lsmsvglineargradientelement.c		\
	lsmsvgradialgradientelement.c		\
	lsmsvgstopelement.c			\
	lsmsvgswitchelement.c			\
	lsmsvgpatternelement.c			\
	lsmsvgmaskelement.c			\
	lsmsvgfilterelement.c			\
	lsmsvgfilterprimitive.c			\
	lsmsvgfilterblend.c			\
	lsmsvgfiltercolormatrix.c		\
	lsmsvgfiltercomposite.c			\
	lsmsvgfilterflood.c			\
	lsmsvgfiltergaussianblur.c		\
	lsmsvgfilterimage.c			\
	lsmsvgfilteroffset.c			\
	lsmsvgfiltermerge.c			\
	lsmsvgfiltermergenode.c			\
	lsmsvgfiltermorphology.c		\
	lsmsvgfilterspecularlighting.c		\
	lsmsvgfiltertile.c			\
	lsmsvgfiltersurface.c

LASEM_DOM_HDRS = 				\
	lsm.h					\
	lsmtypes.h				\
	lsmcairo.h				\
	lsmstr.h				\
	lsmutils.h				\
	lsmdebug.h				\
	lsmtraits.h				\
	lsmproperties.h				\
	lsmattributes.h				\
	lsmitex.h				\
	lsmdomentities.h			\
	lsmdom.h				\
	lsmdomtypes.h				\
	lsmdomnode.h				\
	lsmdomnodelist.h			\
	lsmdomnamednodemap.h			\
	lsmdomdocument.h			\
	lsmdomdocumentfragment.h		\
	lsmdomelement.h				\
	lsmdomcharacterdata.h			\
	lsmdomtext.h				\
	lsmdomview.h				\
	lsmdomparser.h				\
	lsmdomimplementation.h

LASEM_MATHML_HDRS =				\
	lsmmathml.h				\
	lsmmathmltypes.h			\
	lsmmathmlenums.h			\
	lsmmathmltraits.h			\
	lsmmathmlattributes.h			\
	lsmmathmloperatordictionary.h		\
	lsmmathmldocument.h			\
	lsmmathmlelement.h			\
	lsmmathmlsemanticselement.h		\
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
	lsmmathmlencloseelement.h			\
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
	lsmmathmllayoututils.h

LASEM_SVG_HDRS =				\
	lsmsvg.h				\
	lsmsvgtypes.h				\
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
	lsmsvgtransformable.h			\
	lsmsvgclippathelement.h			\
	lsmsvgsvgelement.h			\
	lsmsvgaelement.h			\
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
	lsmsvgtspanelement.h			\
	lsmsvggradientelement.h			\
	lsmsvglineargradientelement.h		\
	lsmsvgradialgradientelement.h		\
	lsmsvgstopelement.h			\
	lsmsvgswitchelement.h			\
	lsmsvgpatternelement.h			\
	lsmsvgmaskelement.h			\
	lsmsvgfilterelement.h			\
	lsmsvgfilterprimitive.h			\
	lsmsvgfilterblend.h			\
	lsmsvgfiltercolormatrix.h		\
	lsmsvgfiltercomposite.h			\
	lsmsvgfilterflood.h			\
	lsmsvgfiltergaussianblur.h		\
	lsmsvgfilterimage.h			\
	lsmsvgfilteroffset.h			\
	lsmsvgfiltermerge.h			\
	lsmsvgfiltermergenode.h			\
	lsmsvgfiltermorphology.h		\
	lsmsvgfilterspecularlighting.h		\
	lsmsvgfiltertile.h			\
	lsmsvgfiltersurface.h


liblasem_@LASEM_API_VERSION@_ladir = $(includedir)/lasem-@LASEM_API_VERSION@

liblasem_@LASEM_API_VERSION@_la_SOURCES = $(LASEM_DOM_SRCS) $(LASEM_DOM_BUILT_SRCS) $(LASEM_MATHML_SRCS) $(LASEM_SVG_SRCS)
liblasem_@LASEM_API_VERSION@_la_SOURCES +=  $(LASEM_MATHML_HDRS) $(LASEM_SVG_HDRS)
liblasem_@LASEM_API_VERSION@_la_SOURCES += lsmmathmlenumtypes.h lsmsvgenumtypes.h

liblasem_@LASEM_API_VERSION@_la_HEADERS = $(LASEM_DOM_HDRS)
liblasem_@LASEM_API_VERSION@_la_HEADERS += lsmdomenumtypes.h

liblasem_@LASEM_API_VERSION@_la_LDFLAGS = -version-info $(LASEM_LIBTOOL_VERSION)

lsmdomenumtypes.h: lsmdomenumtypes.h.template $(LASEM_DOM_HDRS) $(GLIB_MKENUMS)
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template lsmdomenumtypes.h.template $(LASEM_DOM_HDRS)) > $@

lsmdomenumtypes.c: lsmdomenumtypes.c.template $(LASEM_DOM_HDRS) $(GLIB_MKENUMS)
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template lsmdomenumtypes.c.template $(LASEM_DOM_HDRS)) > $@

lsmmathmlenumtypes.h: lsmmathmlenumtypes.h.template $(LASEM_MATHML_HDRS) $(GLIB_MKENUMS)
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template lsmmathmlenumtypes.h.template $(LASEM_MATHML_HDRS)) > $@

lsmmathmlenumtypes.c: lsmmathmlenumtypes.c.template $(LASEM_MATHML_HDRS) $(GLIB_MKENUMS)
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template lsmmathmlenumtypes.c.template $(LASEM_MATHML_HDRS)) > $@

lsmsvgenumtypes.h: lsmsvgenumtypes.h.template $(LASEM_SVG_HDRS) $(GLIB_MKENUMS)
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template lsmsvgenumtypes.h.template $(LASEM_SVG_HDRS)) > $@

lsmsvgenumtypes.c: lsmsvgenumtypes.c.template $(LASEM_SVG_HDRS) $(GLIB_MKENUMS)
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template lsmsvgenumtypes.c.template $(LASEM_SVG_HDRS)) > $@

bin_PROGRAMS = lasem-render-@LASEM_API_VERSION@

lasem_render_@LASEM_API_VERSION@_SOURCES = 				\
	lasemrender.c

lasem_render_@LASEM_API_VERSION@_LDFLAGS =

lasem_render_@LASEM_API_VERSION@_LDADD = $(LASEM_LIBS) liblasem-@LASEM_API_VERSION@.la ../itex2mml/libitex2mml.la

CLEANFILES = $(BUILT_SOURCES)

-include $(INTROSPECTION_MAKEFILE)

INTROSPECTION_GIRS =
INTROSPECTION_SCANNER_ARGS = --add-include-path=$(srcdir)
INTROSPECTION_COMPILER_ARGS = --includedir=$(srcdir)

if HAVE_INTROSPECTION

introspection_files = $(LASEM_DOM_SRCS) \
		      $(LASEM_DOM_HDRS)

introspection_built_files = $(LASEM_DOM_BUILT_SRCS)

Lasem-@LASEM_API_VERSION@.gir: $(INTROSPECTION_SCANNER) liblasem-@LASEM_API_VERSION@.la Makefile

Lasem_@LASEM_API_VERSION_U@_gir_INCLUDES = GObject-2.0 cairo-1.0 Gio-2.0 Pango-1.0 GdkPixbuf-2.0
Lasem_@LASEM_API_VERSION_U@_gir_SCANNERFLAGS = --identifier-prefix=Lsm --warn-all
Lasem_@LASEM_API_VERSION_U@_gir_CFLAGS = $(INCLUDES) -I$(top_srcdir)/src
Lasem_@LASEM_API_VERSION_U@_gir_LIBS = liblasem-@LASEM_API_VERSION@.la
Lasem_@LASEM_API_VERSION_U@_gir_FILES = $(addprefix $(srcdir)/,$(introspection_files)) $(addprefix $(top_builddir)/src/,$(introspection_built_files))

INTROSPECTION_GIRS += Lasem-@LASEM_API_VERSION@.gir

girdir = $(datadir)/gir-1.0
gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(gir_DATA) $(typelib_DATA)

endif
