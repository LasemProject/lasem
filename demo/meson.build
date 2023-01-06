if get_option('demo').enabled()

demo_dependencies = [
  lasem_dependencies,
  dependency('gtk4')
]

lasemdemo = executable(
  'lsm-demo',
  'demo.c',
  dependencies : demo_dependencies,
  link_with : lasem_library,
  include_directories : [library_inc],
  install : true
)

endif