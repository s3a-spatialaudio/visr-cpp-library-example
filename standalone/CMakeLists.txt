# Copyright Andreas Franck a.franck@soton.ac.uk - All rights reserved
# Copyright Institute of Sound and Vibration Research - All rights reserved

# CMake configuration 

add_executable( standalone main.cpp options.hpp options.cpp )

target_link_libraries(standalone components )
target_link_libraries(standalone VISR::apputilities_static )
target_link_libraries(standalone VISR::audiointerfaces_static )
target_link_libraries(standalone VISR::rrl_static )

target_link_libraries( standalone Boost::filesystem )

install( TARGETS standalone DESTINATION bin )
