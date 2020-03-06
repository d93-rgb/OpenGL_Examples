cmake_minimum_required (VERSION 3.9)
cmake_policy(SET CMP0048 NEW)
project(Rendering-Triangle VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)

set(ALL_PROJECT_LIBRARIES
    OpenGL::GL
	OpenGL::GLU
	glm
	glfw	# <- target name, contains absolute path to library
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

	
add_executable(Rendering-Triangle ${OPTIONS} ${PROJECT_HEADERS} ${PROJECT_SOURCES} ${OPENGL_EXAMPLES_GL3W_SOURCES})

target_link_libraries(Rendering-Triangle ${ALL_PROJECT_LIBRARIES})

