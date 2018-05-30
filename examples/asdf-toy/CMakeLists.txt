cmake_minimum_required( VERSION 3.5.2 )
project( asdf-toy )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O1 --std=gnu++11 -Wall -Werror" )

# The compiler command database can be used by editors for code completion
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(asdf-cpp REQUIRED)

set(asdf_c_flags ${CMAKE_C_FLAGS})
set(asdf_cxx_flags ${CMAKE_CXX_FLAGS})

add_executable( asdf-toy asdf-toy.cpp )
include_directories( asdf-toy ${ASDF_CPP_INCLUDE_DIR} )
target_link_libraries( asdf-toy asdf-cpp )

set_target_properties(asdf-cpp PROPERTIES
  COMPILE_FLAGS "${asdf_c_flags} ${asdf_cxx_flags}"
)
