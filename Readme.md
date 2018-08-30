DRC-VISR --- Tutorial example for creating an external C++ library to extend the VISR framework
===============================================================================================

Copyright Andreas Franck <a.franck@soton.ac.uk> 2018 --- All rights reserved.

Copyright Institute of Sound and Vibration Research 2018 --- All rights reserved.

Overview
========

The **DRC-VISR** repository is a tutorial example to explain how to extend the VISR framework with code implemented in C++.

It contains a set of simple components, implementing dynamic range control (DRC) algorithms as compressors, expanders, etc. 

Contents
========

The following components are provided:

* libdrccomponents: A C++ library containing a set of VISR components. These can be used to build C++ binaries such as standalone renderers, DAW plugins, or Max/MSP external.
   * Compressor: A multichannel compressor with an optional limiter setting, implemented as an atomic VISR component. 

Build instructions
==================

Requirements
------------

* CMake (description uses the CMake GUI) 
* VISR (either as an installed package or a build tree)
* A compiler (as GCC) or a development environment (Apple XCode, Microsoft Visual Studio) depending on your platform`

Installation
------------

* Clone the drc-visr repository
* Create a build directory (for instance visr-external-library-example-build). We refer to this directory as $DRC_BUILD in the following.
* In the CMake GUI, set the source and the build directory.
* Click the 'Configure' button
* Depending on the platform you might need to set the following variables
  (they might already be filled automatically, esp. on Linux and Mac OS X):

   + VISR_DIR This entry has to be set to either an installed VISR package or a build tree.
      - In case of a build tree, the entry must point to the src/ directory within, e.g., /home/af5u13/dev/VISR-build/src/
      - For an installed package on Linux, point to the directory containing the CMake export files: /usr/lib/cmake/visr/ . Normally this path is being found automatically.
      - For an installed package on Mac OS X, point to the root directory of that package, i.e., /Applications/VISR-X.X.X/lib/cmake/visr
      - For an installed package on Windows, point to the root directory of the package, e.g., c:\Program Files\VISR-X.X.X\lib\cmake\visr
* For single-configuration build system (such as Makefiles), you might set the variable CMAKE_BUILD_TYPE to either Debug or Release to get fast code or the ability to debug the code. In multi-configuration build systems (e.g., Visual Studio or XCode), you can select the build type in the IDE.
In the latter case the binary code is contained in either Debug/ or Release/ subdirectories within $DRC_BUILD.
* Run 'Configure' again.
* Check the PYTHON entries and set them accordingly. In particular, make sure that it points to the Python distribution (typically Python3) you intend to use. See the VISR installation instructions how to fill these settings.
* Click 'Generate'
* Build the project with the build system/IDE selected (e.g., make, XCode or Visual Studio).
   - For a Make project, go into the build directory and execute 'make all'
   
Usage
=====

The drc-visr package can be used in different ways:

Linking to external applications
--------------------------------

The project creates a library, libdrccomponents, that contains all atomic and composite components as C++ objects. These can be used and linked in other applications.

TODO: Provide proper visibility settings and install the public headers.

Usage in Python
---------------

All components (atomic and composite) can be used from Python, e.g., to create more complex signal flows or to run the CTC rendering in realtime.
To this end, Python wrappers are provided for these components.
They are contained in the Python module **drc-visr** and can be used in exactly the same way as builtin VISR components (contained in the **rcl** library) or Python-based components

The Python external **drc_visr** must be found by the Python interpreter. In most cases you should add the directory $DRC_BUILD/python to the $PYTHONPATH variable. For multi-configuration builds, the actual directory name can be $DRC_BUILD/python/Debug or $DRC_BUILD/python/Release (depending on the chosen build type).

::
  from drc_visr import Compressor

Standalone realtime renderer
----------------------------

In addition to the components, the drc-visr repository also provides a realtime C++ application that performs multichannel compression. It can use different, configurable audio interfaces. Extensive configuration options for the compressor can be passed as command line arguments (we suggest using a configuration file using the **--option-file** option (shorthand: **@**).

The full list of options can be displayed by

::
  ./standalone/standalone --help
