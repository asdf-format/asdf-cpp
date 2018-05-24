# Build "scripts" that are useful for developers for testing

add_executable(read-asdf read-asdf.cpp)
target_link_libraries(read-asdf asdf-cpp)
set_target_properties(read-asdf PROPERTIES
  COMPILE_FLAGS "${yaml_c_flags} ${yaml_cxx_flags} ${asdf_test_flags}"
)

add_executable(write-asdf write-asdf.cpp)
target_link_libraries(write-asdf asdf-cpp)
set_target_properties(write-asdf PROPERTIES
  COMPILE_FLAGS "${yaml_c_flags} ${yaml_cxx_flags} ${asdf_test_flags}"
)

add_executable(read-compressed read-compressed.cpp)
target_link_libraries(read-compressed asdf-cpp)
set_target_properties(read-compressed PROPERTIES
  COMPILE_FLAGS "${yaml_c_flags} ${yaml_cxx_flags} ${asdf_test_flags}"
)

