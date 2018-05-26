========
asdf-cpp
========

The **A**\ dvanced **S**\ cientific **D**\ ata **F**\ ormat (ASDF) is a
next-generation interchange format for scientific data. This repository
contains a prototype library implementation of the `ASDF Standard
<https://asdf-standard.readthedocs.io/en/latest/>`_ in C++. It is not yet
feature-complete, but it supports ASDF's core capabilities of storing metadata
in YAML and serializing n-dimensional numerical data arrays.

Examples
********

The `examples` directory contains toy programs that demonstrate the current
functionality of the library. The program `write-asdf` creates an ASDF file
that contains YAML metadata and also several n-dimensional arrays. This file
can be read by the program `read-asdf`.

The program `write-compressed` demonstrates ASDF's ability to compress the
n-dimensional data arrays for more efficient storage on disk. The file created
by `write-compressed` can be read by `read-compressed`.

Running the examples
--------------------

After configuring and building the package, the example program executables can
be found in the following directory:

`$BUILD_ROOT/



Dependencies
************

This library currently depends on a fork of `yaml-cpp`, which contains minor
modifications that allow `asdf-cpp` to provide a cleaner and more intuitive API
than would otherwise be possible.

The fork can be found at `<https://github.com/drdavella/yaml-cpp>`. This
dependency is currently packaged CMake external project and is automatically
built and installed when building `asdf-cpp`.

We hope that the changes in this fork will eventually be merged into the
`upstream yaml-cpp repository <https://github.com/jbeder/yaml-cpp`_.  If you
are interested in using this implementation of ASDF, it would be helpful to
leave a comment on the `associated PR in yaml-cpp
<https://github.com/jbeder/yaml-cpp/pull/585`_, which may help encourage it to
be accepted more quickly.

Build Instructions
******************

Limitations and Future Improvements
***********************************

The following features are not currently supported, but will be added
incrementally:

* Schema validation: schemas are currently not validated either when reading or
  writing files. This means that it is currently possible to create ASDF files
  using this API that may not be validated successfully by other
  implementations of ASDF.
* Complex numbers
* Table data
* External arrays and `"exploded" form
  <http://asdf-standard.readthedocs.io/en/latest/file_layout.html#exploded-form>`_

See Also
********
