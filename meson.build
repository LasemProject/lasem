project ('lasem', 'c', version: '0.6.0', meson_version: '>=0.56.0')

gnome = import('gnome')
i18n = import ('i18n')
pkg = import ('pkgconfig')

lasem_version = meson.project_version ()
lasem_api_version = '0.6'

versions = lasem_version.split ('.')
lasem_major_version = versions[0].to_int ()
lasem_minor_version = versions[1].to_int ()
lasem_micro_version = versions[2].to_int ()

cc = meson.get_compiler ('c')

glib_dep = dependency ('glib-2.0', version: '>=2.36', required: true)
gobject_dep = dependency ('gobject-2.0', required: true)
gio_dep = dependency ('gio-2.0', required: true)
gdk_dep = dependency ('gdk-pixbuf-2.0', required: true)
cairo_dep = dependency ('cairo', version: '>= 1.2', required: true)
pango_cairo_dep = dependency ('pangocairo', version: '>= 1.16.0', required: true)
libxml2_dep = dependency ('libxml-2.0', required: true)

libm = cc.find_library ('m', required: true)

lasem_dependencies = [glib_dep, gobject_dep, gio_dep, gdk_dep, cairo_dep, pango_cairo_dep, libxml2_dep, libm]

subdir ('po')
subdir ('itex2mml')
subdir ('src')
subdir ('tests')
subdir ('viewer')

doc_deps = dependency ('gi-docgen', version:'>= 2021.1', fallback: ['gi-docgen', 'dummy_dep'], required:get_option('documentation'))
subdir('docs', if_found: doc_deps)

summary (
  {
  'prefix': get_option('prefix'),
  'bindir': get_option('bindir'),
  'libdir': get_option('libdir'),
  'datadir': get_option('datadir'),
  },
  section: 'Directories'
)
