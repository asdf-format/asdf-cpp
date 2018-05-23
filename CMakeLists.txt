cmake_minimum_required( VERSION 3.5.2 )
project( asdf-cpp )

include(CMakeFunctions.txt)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O1 --std=gnu++11 -Wall -Werror" )
set( BUILD_SHARED_LIBS OFF )

# The compiler command database can be used by editors for code completion
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(yaml-cpp REQUIRED)

set( INCLUDE_DIR include )
include_directories( ${INCLUDE_DIR} )

set(asdf_c_flags ${CMAKE_C_FLAGS})
set(asdf_cxx_flags ${CMAKE_CXX_FLAGS})

file(GLOB SOURCE_FILES src/[a-z_]*.cpp)

# Defined in CmakeFunctions.txt
add_sources(${SOURCE_FILES})

add_library( asdf-cpp ${SOURCE_FILES})
target_link_libraries( asdf-cpp yaml-cpp )

set_target_properties(asdf-cpp PROPERTIES
  COMPILE_FLAGS "${asdf_c_flags} ${asdf_cxx_flags}"
)

enable_testing()
option(ASDF_CPP_BUILD_TESTS "Enable testing" ON)

if(ASDF_CPP_BUILD_TESTS)
    add_subdirectory(tests)
    add_subdirectory(examples)
endif()

#
# Installation
#
set(LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${LIB_SUFFIX}")
install(
    TARGETS asdf-cpp EXPORT asdf-cpp-targets ${_INSTALL_DESTINATIONS}
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/${LIB_SUFFIX}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}"
)
install(
    DIRECTORY include
    DESTINATION ${CMAKE_INSTALL_PREFIX}
    FILES_MATCHING PATTERN "*.hpp"
    PATTERN "private" EXCLUDE
)
