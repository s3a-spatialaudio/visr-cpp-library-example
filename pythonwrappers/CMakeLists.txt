# Copyright Andreas Franck a.franck@soton.ac.uk - All rights reserved
# Copyright Institute of Sound and Vibration Research - All rights reserved


set( CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/3rd/pybind11/include/pybind11 )
find_package( PythonLibs REQUIRED )

# Define the file name extension for Python externals.
if( WIN32 )
  set( PYTHON_MODULE_EXTENSION ".pyd" )
elseif( APPLE )
  set( PYTHON_MODULE_EXTENSION ".so" )
else( WIN32 )
  set( PYTHON_MODULE_EXTENSION ${CMAKE_SHARED_LIBRARY_SUFFIX} )
endif( WIN32 )

include( pybind11Tools )

set( SOURCES
drcmodule.cpp
compressor.cpp
)

set(PROJECT_NAME pythonwrappers)

set(MODULE_NAME visr_drc )

pybind11_add_module( ${PROJECT_NAME} MODULE ${SOURCES} )

target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/3rd/pybind11/include )

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${MODULE_NAME} )

set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/python" )

# In case of Windows, we need to make sure that the import library part does not conflict
# with a static library of the same name.
# So we leave it in the build directory and add an _import suffix.
set_target_properties( ${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
set_target_properties( ${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_NAME "${PROJECT_NAME}_import" )

target_link_libraries( ${PROJECT_NAME} PRIVATE components )

# install(TARGETS ${PROJECT_NAME} DESTINATION ${PYTHON_MODULE_INSTALL_DIRECTORY})
