/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#include "../components/compressor.hpp"

#include <libvisr/atomic_component.hpp>
#include <libvisr/composite_component.hpp>
#include <libvisr/signal_flow_context.hpp>

#include <pybind11/pybind11.h>

namespace drc
{
namespace python
{

void exportCoefficientCalculator( pybind11::module & m )
{
  namespace py = pybind11;

  using components::Compressor;

  using SampleType = Compressor::SampleType;

  /**
   * Provide a Python wrapping for the Compressor components.
   */
  py::class_<Compressor, visr::AtomicComponent>( m, "Compressor" )
    /**
     * The constructor is the only method that needs to be exposed, because the component will be 
     * automically called from the framework.
     * We also provide the argument names and default values were apllicable.
     * Note that the Python keyword argument features and 
     */
    .def( py::init< visr::SignalFlowContext &, char const *, visr::CompositeComponent *, 
		    std::size_t, SampleType, SampleType, SampleType, SampleType, 
	            SampleType, SampleType >(),
      py::arg( "context" ),
      py::arg( "name" ),
      py::arg( "parent" ) = static_cast<visr::CompositeComponent*>(nullptr),
      py::arg( "numberOfChannels" ) = 1,
      py::arg( "attackTimeSeconds" ) = static_cast<Compressor::SampleType>(0.002),
      py::arg( "releaseTimeSeconds" ) = static_cast<Compressor::SampleType>(0.01),
      py::arg( "compressorThresholdDB" ) = static_cast<Compressor::SampleType>(-20.0),
      py::arg( "compressorSlopeDB" ) = static_cast<Compressor::SampleType>(-10.0),
      py::arg( "limiterThresholdDB" ) = static_cast<Compressor::SampleType>(-6.0),
      py::arg( "limiterSlopeDB" ) = static_cast<Compressor::SampleType>(-60.0)
    )
    ;
}

} // namespace python
} // namespace drc
