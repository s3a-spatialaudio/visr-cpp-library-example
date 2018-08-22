/* Copyright Andreas Franck 2018 - All rights reserved */
/* Copyright Institute of Sound and Vibration Research 2018
   - All rights reserved */

#include "compressor.hpp"

#include <libvisr/signal_flow_context.hpp>

#include <cmath>

namespace drc
{
namespace components
{

Compressor::Compressor( visr::SignalFlowContext & context,
			char const * name,
			visr::CompositeComponent * parent, 
			std::size_t numberOfChannels,
			SampleType attackTimeSeconds,
			SampleType releaseTimeSeconds,
			SampleType compressorThresholdDB,
			SampleType compressorSlopeDB, 
			SampleType limiterThresholdDB /*= std::numeric_limits<SampleType>::infinity()*/, 
			SampleType limiterGainDB /*= 0.0f*/ )
  : visr::AtomicComponent( context, name, parent )
  , mInput( "in", *this, numberOfChannels )
  , mOutput( "out", *this, numberOfChannels )
{

}
 
Compressor::~Compressor() = default;

void Compressor::process()
{
}

} // namespace components
} // namespace drc
