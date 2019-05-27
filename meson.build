project ('lasem', 'c', version: '0.5.2', meson_version: '>=0.47.0')

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

glib_dep = dependency ('glib-2.0', version: '>=2.32', required: true)
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

if get_option ('documentation')
	subdir ('docs/reference/lasem')
endif
