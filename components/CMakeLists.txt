# Copyright Andreas Franck a.franck@soton.ac.uk - All rights reserved
# Copyright Institute of Sound and Vibration Research - All rights reserved

# Components defined in a C++ extension library for the VISR framework

set( SOURCES
compressor.cpp
)

# Basically, this makes the files show up in the Visual studio project.
set( HEADERS
compressor.hpp
)

add_library( components STATIC ${SOURCES} ${HEADERS} )

# VISR dependencies
target_link_libraries( components PUBLIC VISR::pml_shared )
target_link_libraries( components PUBLIC VISR::rcl_shared )
target_link_libraries( components PUBLIC VISR::visr_shared )
target_link_libraries( components PUBLIC VISR::efl_shared )
target_link_libraries( components PUBLIC VISR::rbbl_shared )

# In order to create Python wrappers, we set the code to be position-independent even 
# if we build a static library.
set_property(TARGET components PROPERTY POSITION_INDEPENDENT_CODE ON)

set_target_properties(components PROPERTIES OUTPUT_NAME drccomponents )

# Set public headers to be installed for the shared target.
# Because the headers are the same for the static library, the headers are only
# installled once.
set_target_properties(components PROPERTIES PUBLIC_HEADER "${HEADERS}" )
