project(
  'termite',
  'c',
  version: '0.8',
  license: 'GPL',
  default_options: ['c_std=gnu99']
)

cc = meson.get_compiler('c')

math = cc.find_library('m',static:false)
gl = cc.find_library('GL',static:false)
glew = cc.find_library('GLEW',static:false)
glu = cc.find_library('GLU',static:false)
sdl = cc.find_library('SDL2',static:false)

termite_dependencies = [math,gl,glew,sdl]

termite_version = '"@0@"'.format(meson.project_version())
pkg_datadir = join_paths(get_option('prefix'), get_option('datadir')) / 'termite'
add_project_arguments('-DPKG_DATADIR="' + pkg_datadir + '"',
		      '-DTERMITE_VERSION=@0@'.format(termite_version),
		      language: 'c')
add_project_arguments('-Wno-unused-but-set-variable', language: 'c')

if get_option('buildtype') == 'debug'
    add_project_arguments('-DDEBUG',language: 'c')
endif

termite_inc = include_directories(
	'/usr/include',
	'/usr/include/GL',
	'/usr/include/SDL2',
	'src/termite',
	'src/termite/gl',
	'src/zc_core',
	'src/zc_math')

com_sources = ['src/termite/ant.c',
	       'src/termite/control.c',
	       'src/termite/defaults.c',
	       'src/termite/food.c',
	       'src/termite/grid.c',
	       'src/termite/grid2d.c',
	       'src/termite/levels.c',
	       'src/termite/main.c',
	       'src/termite/particle.c',
	       'src/termite/scene.c',
	       'src/termite/ui.c',
	       'src/termite/voxelutil.c',
	       'src/termite/file.c',
	       'src/termite/settings.c',
	       'src/termite/voxel.c',
	       'src/termite/bus.c',
	       'src/termite/pixeltext.c',
	       'src/termite/str_util.c',

	       'src/termite/gl/floatbuffer.c',	
	       'src/termite/gl/ogl.c',
	       'src/termite/gl/renderer.c',

	       'src/zc_core/zc_cstring.c',
	       'src/zc_core/zc_map.c',
	       'src/zc_core/zc_memory.c',
	       'src/zc_core/zc_string.c',
	       'src/zc_core/zc_vector.c',
	       'src/zc_core/zc_log.c',
	       'src/zc_core/zc_time.c',
	       'src/zc_core/zc_path.c',

	       'src/zc_math/zc_vec2.c',
	       'src/zc_math/zc_util2.c',
	       'src/zc_math/zc_vec3.c',
	       'src/zc_math/zc_util3.c',
	       'src/zc_math/zc_vec4.c',
	       'src/zc_math/zc_mat4.c']

executable(
  'termite',
  com_sources,
  include_directories: [termite_inc],
  dependencies: termite_dependencies,
  install: true,
)