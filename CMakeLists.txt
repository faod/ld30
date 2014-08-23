cmake_minimum_required(VERSION 2.8)

if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
	message(FATAL_ERROR, "Building on OSX is not currently supported !")
endif()

#-----------#
# Configure
#-----------#

project(allegro_game C CXX)

# The user must fill these variables if the files arnt in the paths envvar
set(ALLEGRO_HOME "" CACHE PATH "Allegro Home path on windows, must contains include, lib and bin directories.")
set(ALLEG_VERSION "5.0.7" CACHE STRING "Your installed allegro version e.g. '5.0.7'.")

option(ALLEGRO_STATICLINK "static link to allegro ?" off)
option(ALLEGRO_LINK_DBG "link with DEBUG libs ?" off)
option(ALLEGRO_LINK_MONOLITH "link with Monolith builds ? (Windows only)" off)
option(ALLEGRO_LINK_MT "link with MT libs (Windows only) ?" off)

include_directories("${ALLEGRO_HOME}/include")
link_directories("${ALLEGRO_HOME}/lib")

#-----------#
#    Env
#-----------#

include("${PROJECT_SOURCE_DIR}/cmake/alleg.cmake")
make_libs_filenames(deps ${ALLEG_VERSION})
append_deps(deps "${deps}")

find_package("OpenGL")
include_directories("${OPENGL_INCLUDE_DIR}")
list(APPEND deps "${OPENGL_gl_LIBRARY}" "${OPENGL_glu_LIBRARY}")

set(CMAKE_CXX_FLAGS "--std=c++11")

#-----------#
#   Build
#-----------#

file(GLOB sources "${PROJECT_SOURCE_DIR}/src/*.cpp" "${PROJECT_SOURCE_DIR}/src/*/*.cpp")
add_executable(${PROJECT_NAME} ${sources})
if(ALLEGRO_STATICLINK)
	set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS "ALLEGRO_STATICLINK")
endif(ALLEGRO_STATICLINK)
target_link_libraries(${PROJECT_NAME} ${deps})

if(UNIX)
	target_link_libraries(${PROJECT_NAME} m)
endif(UNIX)
