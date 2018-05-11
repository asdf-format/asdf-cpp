# Build "scripts" that are useful for developers for testing

add_executable(read-asdf read-asdf.cpp)
target_link_libraries(read-asdf asdf-cpp)
set_target_properties(read-asdf PROPERTIES
  COMPILE_FLAGS "${yaml_c_flags} ${yaml_cxx_flags} ${asdf_test_flags}"
)
