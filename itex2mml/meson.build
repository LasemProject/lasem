flex = find_program('flex', required: false)
bison = find_program('bison', required: false)

lgen = generator(flex,
output : '@BASENAME@.yy.c',
arguments : ['-P', '@BASENAME@_yy', '-o', '@OUTPUT@', '@INPUT@'])

pgen = generator(bison,
output : ['@BASENAME@.tab.c', '@BASENAME@.tab.h'],
arguments : ['@INPUT@', '--defines=@OUTPUT1@', '--output=@OUTPUT0@', '-v', '-y', '-p', '@BASENAME@_yy', '-d'])

itex2mml_files = [
	lgen.process ('itex2MML.l'),
	pgen.process ('itex2MML.y')
]

itex2mml_inc = include_directories ('.')
