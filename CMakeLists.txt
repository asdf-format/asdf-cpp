cmake_minimum_required( VERSION 3.5.2 )
project( asdf-cpp )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O1 --std=gnu++11 -Wall -Werror" )
set( BUILD_SHARED_LIBS OFF )

# The compiler command database can be used by editors for code completion
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(yaml-cpp REQUIRED)

set( INCLUDE_DIR include )
include_directories( ${INCLUDE_DIR} )

set(asdf_c_flags ${CMAKE_C_FLAGS})
set(asdf_cxx_flags ${CMAKE_CXX_FLAGS})

# From http://www.cmake.org/pipermail/cmake/2010-March/035992.html:
# function to collect all the sources from sub-directories
# into a single list
function(add_sources)
  get_property(is_defined GLOBAL PROPERTY SRCS_LIST DEFINED)
  if(NOT is_defined)
    define_property(GLOBAL PROPERTY SRCS_LIST
      BRIEF_DOCS "List of source files"
      FULL_DOCS "List of all source files in the entire project")
  endif()
  # make absolute paths
  set(SRCS)
  foreach(s IN LISTS ARGN)
    if(NOT IS_ABSOLUTE "${s}")
      get_filename_component(s "${s}" ABSOLUTE)
    endif()
    list(APPEND SRCS "${s}")
  endforeach()
  # append to global list
  set_property(GLOBAL APPEND PROPERTY SRCS_LIST "${SRCS}")
endfunction(add_sources)

file(GLOB SOURCE_FILES src/[a-z_]*.cpp)

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
