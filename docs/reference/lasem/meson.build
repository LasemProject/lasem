doc_ignore_headers = [
	'lsm.h',
	'lsmconfig.h',
	'lsmcairo.h',
	'lsmstr.h',
	'lsmdebug.h',
	'lsmtraits.h',
	'lsmproperties.h',
	'lsmattributes.h',
	'lsmitex.h',
	'itex2MM.tab.h',
] + mathml_headers + svg_headers

gnome.gtkdoc ('lasem',
	      main_xml: 'lasem-docs.xml',
	      src_dir: [join_paths (meson.source_root (), 'src')],
	      dependencies: [lasem_library_dependencies],
	      ignore_headers: doc_ignore_headers,
	      scan_args: ['--rebuild-types'],
	      content_files: ['lasem-overview.xml'],
	      install_dir: 'lasem-@0@'.format (lasem_api_version),
	      install: true)
