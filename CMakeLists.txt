cmake_minimum_required( VERSION 3.5.2 )
project( asdf-cpp )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=gnu++11 -Wall -Werror" )
set( BUILD_SHARED_LIBS ON )


set( INCLUDE_DIR include )
include_directories( ${INCLUDE_DIR} )

set( HEADER_FILES ${INCLUDE_DIR}/asdf.hpp )
set( SOURCE_FILES src/asdf.cpp )

add_library( asdf ${SOURCE_FILES} ${HEADER_FILES} )
target_link_libraries( asdf yaml-cpp )

add_executable( asdf-test test/test.cpp )
target_link_libraries( asdf-test asdf )
