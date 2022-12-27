toml_conf = configuration_data()
toml_conf.set('version', meson.project_version())

gidocgen = find_program('gi-docgen')

gidocgen_common_args = [
  '--quiet',
  '--no-namespace-dir',
]

docs_dir = get_option('datadir') / 'doc'

expand_content_md_files = [
  'overview.md',
  'building.md',
]

lasem_toml=configure_file(
  input: 'lasem.toml.in',
  output: 'lasem.toml',
  configuration: toml_conf,
  install: true,
  install_dir: docs_dir / 'lasem-@0@'.format(lasem_api_version)
)

custom_target(
  'lasem-doc',
  input: [ lasem_toml, lasem_gir[0] ],
  output: 'lasem-@0@'.format(lasem_api_version),
  command: [
    gidocgen,
    'generate',
    gidocgen_common_args,
    '--add-include-path=@0@'.format(meson.current_build_dir() / '../../../src'),
    '--config=@INPUT0@',
    '--output-dir=@OUTPUT@',
    '--content-dir=@0@'.format(meson.current_source_dir()),
    '@INPUT1@'
    ],
    depend_files: [ expand_content_md_files ],
    build_by_default: true,
    install: true,
    install_dir: docs_dir
  )
