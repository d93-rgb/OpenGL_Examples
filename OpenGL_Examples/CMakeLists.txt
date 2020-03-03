cmake_minimum_required (VERSION 3.9)
cmake_policy(SET CMP0048 NEW)

option(BUILD_SHARED_LIBS "Build shared libs for all libraries" OFF) 

if(NOT MSVC)
	SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=gnu++11")
	SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -ldl")
else(NOT MSVC)
	# important flag, for __FILE__ macro to have absolute path 
	SET(COMP_FLAGS "/FC")
	SET(CMAKE_CXX_FLAGS "${COMP_FLAGS}")
	# alternatively, add the following line after add_executable
	#target_compile_options(minimal_opengl_example PUBLIC /FC)
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
include_directories(ext/gl3w/include)
set(GL3W_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/ext/gl3w/src/gl3w.c)
###########################################################################


add_subdirectory(Rendering-Triangle)

