========
asdf-cpp
========

The **A**\ dvanced **S**\ cientific **D**\ ata **F**\ ormat (ASDF) is a
next-generation interchange format for scientific data. This repository
contains a prototype library implementation of the `ASDF Standard
<https://asdf-standard.readthedocs.io/en/latest/>`_ in C++. It is not yet
feature-complete, but it supports ASDF's core capabilities of storing metadata
in YAML and serializing n-dimensional numerical data arrays.

Dependencies
************

Building and installing this library requires `cmake <https://cmake.org>`_.

The YAML implementation is provided by `yaml-cpp
<https://github.com/jbeder/yaml-cpp`_.

The **zlib** and **bzip2** libraries are optional dependencies but are required
in order to support compression of array data blocks.

Build Instructions
******************

CMake is used to configure, build, and install the package. CMake encourages
out-of-place builds. The first step is to create a build directory:

``$ mkdir build``

Next, move into the build directory:

``$ cd build``

Now we configure CMake. This step allows users to configure the path to the
installed library using the ``CMAKE_INSTALL_PREFIX`` variable. If this variable
is omitted from the command line, CMake will use a default system path as a
prefix. This will most likely be ``/usr/local`` on a Unix-like system. Note
that installing to a system path may require elevated privileges.

``$ cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/install/tree``

Now it is possible to build the package. CMake will automatically build the
**yaml-cpp** dependency in this step as well:

``$ make``

The example programs can be run without installing the package. However, in
order to link other applications against this library, it will need to be
installed.

To install the **yaml-cpp** dependency to the path specified by
``CMAKE_INSTALL_PREFIX``, run the following command. This step will be
necessary unless you cloned the repository (remember to use the fork mentioned
above for now) from github and installed it separately:

``$ make yaml-cpp-extern-install``

To install **asdf-cpp** itself, run the following command:

``$ make install``

**N.B**:

    So far little effort has been made to support cross-platform portability.
    It should be possible to build the library with any sufficiently recent
    version of clang or gcc. It almost certainly will not build on Windows yet.
    If this is important to you, please feel free to submit a PR in order to
    help us support cross-platform compatibility.

Linking Against asdf-cpp
************************

To link an application against **asdf-cpp**, it is recommended to use CMake.
When configuring the build for the application, the ``CMAKE_PREFIX_PATH``
should be set to the same path as the ``CMAKE_INSTALL_PREFIX`` that was used
to configure the library build. (If not given, it will default to a system path
such as ``/usr/local``).

A very basic application example complete with ``CMakeLists.txt`` is provided
in ``examples/asdf-toy``. This can be used as a project template for
applications that wish to build against **asdf-cpp**.

To build the toy:

``$ cd examples/asdf-toy``
``$ mkdir build``
``$ cd build``
``$ cmake .. -DCMAKE_PREFIX_PATH=/path/to/install/tree``
``$ make``

Examples
********

The ``examples`` directory contains toy programs that demonstrate the current
functionality of the library. The program ``write-asdf`` creates an ASDF file
that contains YAML metadata and also several n-dimensional arrays. This file
can be read by the program ``read-asdf``.

The program ``write-compressed`` demonstrates ASDF's ability to compress the
n-dimensional data arrays for more efficient storage on disk. The file created
by ``write-compressed`` can be read by ``read-compressed``.

Running the examples
--------------------

After configuring and building the package, the example program executables can
be found in the following directory (where ``$BUILD_ROOT`` represents the path
to the CMake build directory that was created when setting up the build):

``$BUILD_ROOT/examples``

The examples can be run from the command line. For example, to create an ASDF
file, run the following command:

``$BUILD_ROOT/examples/write-asdf example.asdf``

This will create a new file called ``example.asdf``. This file can be read
using the following command:

``$BUILD_ROOT/examples/read-asdf example.asdf``

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
* Handling big-endian data and big-endian platforms

Contributing
************

We welcome feedback and contributions to the project. Contributions of
code, documentation, or general feedback are all appreciated. Please
follow the `contributing guidelines <CONTRIBUTING.md>`__ to submit an
issue or a pull request.

We strive to provide a welcoming community to all of our users by
abiding to the `Code of Conduct <CODE_OF_CONDUCT.md>`__.

See Also
********

* `ASDF Standard <http://asdf-standard.readthedocs.io/en/latest/index.html>`_
* `ASDF for Python <http://asdf.readthedocs.io/en/latest/>`_
* `yaml-cpp <https://github.com/jbeder/yaml-cpp>`_

