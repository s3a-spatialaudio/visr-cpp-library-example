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
   * 


  Computes filters and beamforming gains and delays based on the array geometry and the listener position(s).
ctc.ListenerPositionDecoder
  Converts JSON messages (e.g., as received througha network connection) to the internal format for listener position messages (3 x numberOfEars MatrixParameterFloat objects)
ctc.CtcRenderer
  A basic, configurable rendering graph that receives listener position updates through a parameter port.
ctc.RealtimeRenderer
  A CtcRenderer extended by facilities to receive and decode listener position updates from a UDP network port, suitable for realtime rendering.

Build instructions
==================

Requirements
------------

* CMake (description uses the CMake GUI) 
* VISR (either as an installed package or a build tree)

Installation
------------

* Clone the drc-visr repository
* Create a build directory
* In the CMake GUI, set the source and the build directory.
* Click the 'Configure' button
* Depending on the platform you might need to set the following variables
  (they might already be filled automatically, esp. on Linux and Mac OS X):

   + BOOST_DIR: Set this to the base directory of the unpacked Boost package (e.g. c:/dev/boost_1_66_0). On Linux and Mac OS X, Boost should be found automatically, especially if brew is used on Mac OS X.
   + EIGEN3_INCLUDE_DIR: Set this to the base directory of the eigen package, e.g., c:/dev/eigen. Note that the variable Eigen3_DIR is updated by 'Configure', so it does not need to be touched.
   + VISR_DIR This entry has to be set to either an installed VISR package or a build tree.
      - In case of a build tree, the entry must point to the src/ directory within, e.g., /home/af5u13/dev/VISR-build/src/
      - For an installed package on Linux, point to the directory containing the CMake export files: /usr/lib/cmake/visr/
      - For an installed package on Mac OS X, point to the root directory of that package, i.e., /Applications/VISR-X.X.X/
      - For an installed package on Windows, point to the root directory of the package, e.g., c:\Program Files\VISR-X.X.X\

* Run 'Configure' again.
* Check the PYTHON entries and set them accordingly. In particular, make sure that it points to the Python distribution (typically Python3) you intend to use. See the VISR installation instructions how to fill these settings.
* Click 'Generate'
* Build the project with the build system/IDE selected (e.g., make, XCode or Visual Studio).
  
Usage
=====

The ctcdsp package can be used in different ways:

Linking to external applications
--------------------------------

The project creates a library, libdrccomponents, that contains all atomic and composite components as C++ objects. These can be used and linked in other applications.

TODO: Provide proper visibility settings and install the public headers.

Usage in Python
---------------

All components (atomic and composite) can be used from Python, e.g., to create more complex signal flows or to run the CTC rendering in realtime.
To this end, Python wrappers are provided for these components.
They are contained in the Python module **drc-visr** and can be used in exactly the same way as builtin VISR components (contained in the **rcl** library) or Python-based components

::

  from drc import Compressor

Standalone realtime renderer
----------------------------

In addition to the components, the ctcdsp repository also provides a realtime C++ renderer for the CTC algorithm. It can use different audio interfaces and receives listener position updates via an UDP port. Extensive configuraion options can be passed as command line arguments (we suggest using a configuration file using the **--option-file** option (shorthand: **@**).

The full list of options can be displayed by

::

  ./standalone/standalone --help
