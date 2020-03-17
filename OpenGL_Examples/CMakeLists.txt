cmake_minimum_required (VERSION 3.9)
cmake_policy(SET CMP0048 NEW)

option(BUILD_SHARED_LIBS "Build shared libs for all libraries" OFF) 

if(NOT MSVC)
	SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=gnu++11")
	SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -ldl")
else(NOT MSVC)
	# important flag, for __FILE__ macro to have absolute path 
	SET(COMP_FLAGS "/FC /EHsc")
	SET(CMAKE_CXX_FLAGS "${COMP_FLAGS}")
	# alternatively, add the following line after add_executable
	#target_compile_options(minimal_opengl_example PUBLIC /FC)
	#add_definitions("/EHsc")
endif(NOT MSVC)

###########################################################################
# OpenGL
find_package(OpenGL REQUIRED)
if( NOT OPENGL_FOUND )
	message(FATAL_ERROR 
		"OpenGL library could not be found on this system.")
endif()

###########################################################################

###########################################################################
# glfw
# build as static library - so no dll
add_subdirectory(ext/glfw)
include_directories (
  ext/glfw/include
)
###########################################################################

# gl3w 
###########################################################################
set(OPENGL_EXAMPLES_GL3W_SOURCES ../ext/gl3w/src/gl3w.c)
include_directories(ext/gl3w/include)
###########################################################################

###########################################################################
# glm
# disable testing
set(GLM_TEST_ENABLE OFF CACHE BOOL " " FORCE)
set(GLM_QUIET ON CACHE BOOL " " FORCE)
add_subdirectory(ext/glm)
include_directories (
  ext/glm
  ${CMAKE_BINARY_DIR}/ext/glm
)
###########################################################################

###########################################################################
# glog
set(WITH_GFLAGS OFF CACHE BOOL "Use gflags")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
#IF(WIN32)
#  ADD_DEFINITIONS( -D GOOGLE_GLOG_DLL_DECL= )
#ENDIF()
add_subdirectory(ext/glog)
#SET_PROPERTY(TARGET glog logging_unittest demangle_unittest utilities_unittest stl_logging_unittest PROPERTY FOLDER "ext")
include_directories (
  ${CMAKE_BINARY_DIR}/ext/glog/
)
###########################################################################

# imgui
###########################################################################
include_directories(ext/imgui
	ext/imgui/examples)
set(OPENGL_EXAMPLES_IMGUI_SOURCES 
				"${CMAKE_CURRENT_SOURCE_DIR}/ext/imgui/imgui.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/ext/imgui/imgui_draw.cpp" 
				"${CMAKE_CURRENT_SOURCE_DIR}/ext/imgui/imgui_widgets.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/ext/imgui/examples/imgui_impl_opengl3.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/ext/imgui/examples/imgui_impl_glfw.cpp"
				CACHE INTERNAL
				OPENGL_EXAMPLES_IMGUI_SOURCES)
###########################################################################


# core OpenGL_Examples include files
include_directories (
  core/include
)

	# Visual Studio filters inside the solution explorer
if(MSVC)
    #SET(OPTIONS WIN32)
	MESSAGE(STATUS "Detected Visual Studio as IDE")
	# source files
	SOURCE_GROUP ("Source Files\\core" REGULAR_EXPRESSION core/src/.*)

	# header files
	SOURCE_GROUP ("Header Files\\core" REGULAR_EXPRESSION core/include/.*)

else(MSVC)
	SOURCE_GROUP (core REGULAR_EXPRESSION core/src/.*)
endif()

SET(OPENGL_EXAMPLES_PROJECT_HEADERS 
	"${CMAKE_CURRENT_SOURCE_DIR}/core/include/shadercompiler.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/include/renderer.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/include/windowmanager.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/include/camera.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/include/renderingdevice.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/include/graphicscontext.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/include/renderingmanager.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/include/renderingparameter.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/include/eventhandler.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/include/guiparameter.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/include/shader.h"
	CACHE INTERNAL 
	"OPENGL_EXAMPLES_PROJECT_HEADERS")

SET(OPENGL_EXAMPLES_PROJECT_SOURCES 
	"${CMAKE_CURRENT_SOURCE_DIR}/core/src/shadercompiler.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/src/renderer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/src/windowmanager.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/src/camera.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/src/renderingdevice.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/src/graphicscontext.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/src/renderingmanager.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/src/renderingparameter.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/src/eventhandler.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/src/guiparameter.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/src/shader.cpp"
	CACHE INTERNAL
	"OPENGL_EXAMPLES_PROJECT_SOURCES")


add_subdirectory(Rendering-Triangle)

