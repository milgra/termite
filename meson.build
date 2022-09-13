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
pthread = cc.find_library('pthread',static:false)

termite_dependencies = [math,gl,glew,sdl,pthread]

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
	'src/modules',
	'src/modules/core',
	'src/modules/math',
	'src/modules/utils',
	'src/modules/voxel')

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

	       'src/modules/core/zc_cstring.c',
	       'src/modules/core/zc_map.c',
	       'src/modules/core/zc_memory.c',
	       'src/modules/core/zc_string.c',
	       'src/modules/core/zc_vector.c',
	       'src/modules/core/zc_log.c',
	       'src/modules/core/zc_time.c',
	       'src/modules/core/zc_path.c',

	       'src/modules/math/zc_vec2.c',
	       'src/modules/math/zc_util2.c',
	       'src/modules/math/zc_vec3.c',
	       'src/modules/math/zc_util3.c',
	       'src/modules/math/zc_vec4.c',
	       'src/modules/math/zc_mat4.c',

	       'src/modules/voxel/file.c',
       	       'src/modules/voxel/floatbuffer.c',	
	       'src/modules/voxel/ogl.c',
	       'src/modules/voxel/renderer.c',
	       'src/modules/voxel/settings.c',
	       'src/modules/voxel/voxel.c',
	       'src/modules/voxel/bus.c',
	       'src/modules/voxel/pixeltext.c',

	       'src/modules/utils/str_util.c']

executable(
  'termite',
  com_sources,
  include_directories: [termite_inc],
  dependencies: termite_dependencies,
  install: true,
)