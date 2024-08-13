if get_option('viewer').enabled()

  demo_dependencies = [
    lasem_dependencies,
    dependency('gtk4', version : '>=4.9')
  ]

  lasemdemo = executable(
    'lsm-viewer',
    'viewer.c',
    dependencies : demo_dependencies,
    link_with : lasem_library,
    include_directories : [library_inc],
    install : true
  )

endif
