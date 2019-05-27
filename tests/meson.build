tests = [
	['dom',		[]],
	['str', 	[]],
	['filter', 	[]],
	['suite',	['-DSUITE_DATA_DIRECTORY="@0@/tests/data"'.format (meson.source_root ()),
	                 '-DSUITE_OPTION_FILE="@0@/tests/suite.ini"'.format (meson.source_root ())]]
]

foreach t: tests
  exe = executable (t[0], '@0@.c'.format (t[0]),
    c_args: [t[1]],
    link_with: lasem_library,
    dependencies: lasem_dependencies,
    include_directories: [library_inc])
  test (t[0], exe)
endforeach

examples = [
	['lsm-test',			'lsmtest.c']
]

foreach example: examples
  exe = executable (example[0], example[1],
    link_with: lasem_library,
    dependencies: lasem_dependencies,
    include_directories: [library_inc])
endforeach
