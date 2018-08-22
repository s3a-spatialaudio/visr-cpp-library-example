/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#include <libpml/initialise_parameter_library.hpp>

#include <pybind11/pybind11.h>

namespace ctcdsp
{
namespace python
{
void exportCoefficientCalculator( pybind11::module & m );
void exportCtcRenderer( pybind11::module & m );
void exportListenerPositionDecoder( pybind11::module & m );
void exportRealtimeCtcRenderer( pybind11::module & m );
}
}

PYBIND11_MODULE( ctc, m )
{
  visr::pml::initialiseParameterLibrary();

  pybind11::module::import( "efl" );
  pybind11::module::import( "visr" );
  pybind11::module::import( "pml" );

  using namespace ctcdsp::python;

  // Export the communication protocols
  exportCoefficientCalculator( m );
  exportCtcRenderer( m );
  exportListenerPositionDecoder( m );
  exportRealtimeCtcRenderer( m );
}
