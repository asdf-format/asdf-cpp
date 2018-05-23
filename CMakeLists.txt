cmake_minimum_required( VERSION 3.5.2 )
project( ASDF_CPP )

set(ASDF_CPP_VERSION_MAJOR "0")
set(ASDF_CPP_VERSION_MINOR "1")
set(ASDF_CPP_VERSION_PATCH "0")
set(ASDF_CPP_VERSION
    "${ASDF_CPP_VERSION_MAJOR}.${ASDF_CPP_VERSION_MINOR}.${ASDF_CPP_VERSION_PATCH}")

include(CMakeFunctions.txt)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O1 --std=gnu++11 -Wall -Werror" )
set( BUILD_SHARED_LIBS OFF )

# The compiler command database can be used by editors for code completion
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(yaml-cpp REQUIRED)

# Look for optional compression packages
find_package(zlib)
find_package(bzip2)

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

if (${ZLIB_FOUND})
    target_compile_definitions(asdf-cpp PRIVATE HAS_ZLIB=1)
endif()

if (${BZIP2_FOUND})
    target_compile_definitions(asdf-cpp PRIVATE HAS_BZIP2=1)
endif()

enable_testing()
option(ASDF_CPP_BUILD_TESTS "Enable testing" ON)

if(ASDF_CPP_BUILD_TESTS)
    add_subdirectory(tests)
    add_subdirectory(examples)
endif()

#
# Installation
#
set(INCLUDE_INSTALL_ROOT_DIR ${CMAKE_INSTALL_PREFIX}/include)
set(LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib/${LIB_SUFFIX}")
set(INSTALL_CMAKE_DIR ${LIB_INSTALL_DIR}/cmake/asdf-cpp)

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

export(TARGETS asdf-cpp FILE "${PROJECT_BINARY_DIR}/asdf-cpp-targets.cmake")
export(PACKAGE asdf-cpp)
set(EXPORT_TARGETS asdf-cpp CACHE INTERNAL "export targets")

set(CONFIG_INCLUDE_DIRS "${ASDF_CPP_SOURCE_DIR}/include")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/asdf-cpp-config.cmake.in
	"${PROJECT_BINARY_DIR}/asdf-cpp-config.cmake" @ONLY)

file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}" "${INCLUDE_INSTALL_ROOT_DIR}")
set(CONFIG_INCLUDE_DIRS "\${ASDF_CPP_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/asdf-cpp-config.cmake.in
	"${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/asdf-cpp-config.cmake" @ONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/asdf-cpp-config-version.cmake.in
	"${PROJECT_BINARY_DIR}/asdf-cpp-config-version.cmake" @ONLY)

install(FILES
	"${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/asdf-cpp-config.cmake"
	"${PROJECT_BINARY_DIR}/asdf-cpp-config-version.cmake"
	DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)
install(EXPORT asdf-cpp-targets DESTINATION ${INSTALL_CMAKE_DIR})

if(UNIX)
    set(PC_FILE ${CMAKE_BINARY_DIR}/asdf-cpp.pc)
    configure_file("asdf-cpp.pc.cmake" ${PC_FILE} @ONLY)
    install(FILES ${PC_FILE} DESTINATION ${LIB_INSTALL_DIR}/pkgconfig)
endif()
