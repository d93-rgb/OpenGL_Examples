cmake_minimum_required (VERSION 3.9)
cmake_policy(SET CMP0048 NEW)
project(Rendering-Triangle VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)

set(ALL_PROJECT_LIBRARIES
    OpenGL::GL
	OpenGL::GLU
	glm
	glfw	# <- target name, contains absolute path to library
	gl3w
	glog)

SET(PROJECT_HEADERS 
	"../core/include/shadercompiler.h"
	"../core/include/renderer.h"
	"../core/include/windowmanager.h"
	"../core/include/camera.h"
	"../core/include/renderingdevice.h"
	"../core/include/graphicscontext.h"
	"../core/include/renderingmanager.h"
	"../core/include/renderingparameter.h")

SET(PROJECT_SOURCES 
	"src/main.cpp"
	"../core/src/shadercompiler.cpp"
	"../core/src/renderer.cpp"
	"../core/src/windowmanager.cpp"
	"../core/src/camera.cpp"
	"../core/src/renderingdevice.cpp"
	"../core/src/graphicscontext.cpp"
	"../core/src/renderingmanager.cpp"
	"../core/src/renderingparameter.cpp")

	
set_source_files_properties("${CMAKE_BINARY_DIR}/OpenGL_Examples/ext/gl3w/src/gl3w.c" GENERATED)

add_executable(Rendering-Triangle ${OPTIONS} ${PROJECT_HEADERS} ${PROJECT_SOURCES} ${GL3W_SOURCES})

add_dependencies(Rendering-Triangle gl3w)

target_link_libraries(Rendering-Triangle ${ALL_PROJECT_LIBRARIES})

