# Build "scripts" that are useful for developers for testing

set(programs read-asdf write-asdf read-compressed write-compressed)

foreach(name ${programs})
    add_executable(${name} "${name}.cpp")
    target_link_libraries(${name} asdf-cpp)
    set_target_properties(${name} PROPERTIES
      COMPILE_FLAGS "${yaml_c_flags} ${yaml_cxx_flags} ${asdf_test_flags}"
    )
endforeach(name)
