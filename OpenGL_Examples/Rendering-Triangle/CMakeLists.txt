cmake_minimum_required (VERSION 3.9)
cmake_policy(SET CMP0048 NEW)
project(Rendering-Triangle VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)

set(ALL_PROJECT_LIBRARIES
    OpenGL::GL
	OpenGL::GLU
	glm
	glfw
	glog)
	
if(MSVC)
    #SET(OPTIONS WIN32)
	MESSAGE(STATUS "Detected Visual Studio as IDE")
	# source files
	SOURCE_GROUP ("a" FILES ${OPENGL_EXAMPLES_PROJECT_HEADERS})

	# header files
	SOURCE_GROUP ("b" FILES ${OPENGL_EXAMPLES_PROJECT_SOURCES})

else(MSVC)
	SOURCE_GROUP (core REGULAR_EXPRESSION core/src/.*)
endif()

add_executable(Rendering-Triangle ${OPTIONS} 
	${OPENGL_EXAMPLES_PROJECT_HEADERS} 
	${OPENGL_EXAMPLES_PROJECT_SOURCES} 
	${OPENGL_EXAMPLES_GL3W_SOURCES} 
	${OPENGL_EXAMPLES_IMGUI_SOURCES}
	src/main.cpp)

target_link_libraries(Rendering-Triangle ${ALL_PROJECT_LIBRARIES})

