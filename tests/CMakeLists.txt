set(gtest_force_shared_crt ${MSVC_SHARED_RT} CACHE BOOL
  "Use shared (DLL) run-time lib even when Google Test built as a static lib.")

set (GTEST_ROOT "googletest")
add_subdirectory(${GTEST_ROOT})
include_directories(SYSTEM ${GTEST_ROOT}/googlemock/include)
include_directories(SYSTEM ${GTEST_ROOT}/googletest/include)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR
   CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(asdf_test_flags "-Wno-variadic-macros -Wno-sign-compare")

  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(asdf_test_flags "${asdf_test_flags} -Wno-c99-extensions")
  endif()

  if(CMAKE_COMPILER_IS_GNUCXX)
    set(asdf_test_flags "${asdf_test_flags} -std=gnu++11")
  else()
    set(asdf_test_flags "${asdf_test_flags} -std=c++11")
  endif()
endif()


file(GLOB test_headers [a-z_]*.h)
file(GLOB test_sources [a-z_]*.cpp)

add_sources(${test_sources} ${test_headers})

include_directories(${ASDF_CPP_SOURCE_DIR}/src)
include_directories(${ASDF_CPP_SOURCE_DIR}/test)

#
# This is the actual gtest suite
#
add_executable(run-tests ${test_sources} ${test_headers})
set_target_properties(run-tests PROPERTIES
  COMPILE_FLAGS "${yaml_c_flags} ${yaml_cxx_flags} ${asdf_test_flags}"
)
target_link_libraries(run-tests asdf-cpp gmock)

add_test(asdf-test
    ${CMAKE_CURRENT_BINARY_DIR}/run-tests
    ${CMAKE_CURRENT_BINARY_DIR}/data)

#
# Add the data directory to the build output
#
add_custom_command(
    TARGET run-tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/data
            ${CMAKE_CURRENT_BINARY_DIR}/data)

add_subdirectory(scripts)
