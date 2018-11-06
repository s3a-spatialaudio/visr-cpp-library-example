/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#include <libpml/initialise_parameter_library.hpp>

#include <pybind11/pybind11.h>

namespace drc
{
namespace python
{
void exportCompressor( pybind11::module & m );
}
}

PYBIND11_MODULE( visr_drc, m )
{
  visr::pml::initialiseParameterLibrary();

  pybind11::module::import( "efl" );
  pybind11::module::import( "visr" );
  pybind11::module::import( "pml" );

  using namespace drc::python;

  // Export the communication protocols
  exportCompressor( m );
}
