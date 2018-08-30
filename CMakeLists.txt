# Copyright Andreas Franck a.franck@soton.ac.uk - All rights reserved
# Copyright Institute of Sound and Vibration Research - All rights reserved

# Tutorial example for an external C++ library to extend the VISR framework with new functionality

CMAKE_MINIMUM_REQUIRED( VERSION 3.0.0 )

PROJECT( drc_visr CXX )

set (CMAKE_CXX_STANDARD 14)

## CMake configuration

# Enable new behaviour (use  <package>_ROOT variable in find_package() for CMake >= 3.12)
if( POLICY CMP0074)
  cmake_policy( SET CMP0074 NEW )
endif( POLICY CMP0074)

# Define build configuration options
option( BUILD_PYTHON_BINDINGS "Whether to build a Python module to use the defined components from Python" ON )
option( BUILD_STANDALONE_RENDERER "Whether to build a standalone realtime renderer" ON )

# Additional CMake functionality in separate files.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules")

if( WIN32 ) # Covers 64-bit Windows as well
  # Avoid complaints about STL members in classes with VISR_CORE_LIBRARY_SYMBOL not having DLL interfaces.
  add_definitions( "/wd4251" )
  # Disable warnings about unsafe STL calls (while calling them with raw pointers is perfectly legal).
  add_definitions( "/wd4996" )
endif( WIN32 )

# Import VISR targets (libraries and include files) 
find_package( VISR 0.9.0 REQUIRED ) 

add_definitions("-DBOOST_ALL_DYN_LINK")
add_definitions("-DBOOST_ALL_NO_LIB")
find_package( Boost REQUIRED COMPONENTS filesystem )

add_subdirectory( components )

if( BUILD_PYTHON_BINDINGS )
  add_subdirectory( pythonwrappers )
endif( BUILD_PYTHON_BINDINGS )

if( BUILD_STANDALONE_RENDERER )
  add_subdirectory( standalone )  
endif( BUILD_STANDALONE_RENDERER )
