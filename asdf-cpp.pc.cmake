libdir=@LIB_INSTALL_DIR@
includedir=@INCLUDE_INSTALL_ROOT_DIR@

Name: Asdf-cpp
Description: An ASDF parser and emitter for C++
Version: @ASDF_CPP_VERSION@
Requires:
Libs: -L${libdir} -lasdf-cpp
Cflags: -I${includedir}
