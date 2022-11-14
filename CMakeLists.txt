cmake_minimum_required(VERSION 3.14)

project(geometry-lab
  VERSION     0.1
  DESCRIPTION "geometry-lab"
  LANGUAGES   CXX C
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/release")

add_compile_definitions(_USE_MATH_DEFINES)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(extend)
add_subdirectory(src)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
	add_subdirectory(demo)
endif()