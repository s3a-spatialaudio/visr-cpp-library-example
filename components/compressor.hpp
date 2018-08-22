/* Copyright Andreas Franck 2018 - All rights reserved */
/* Copyright Institute of Sound and Vibration Research 2018
   - All rights reserved */

#ifndef DRC_COMPONENTS_COMPRESSOR_HPP_INCLUDED
#define DRC_COMPONENTS_COMPRESSOR_HPP_INCLUDED

#include <libvisr/atomic_component.hpp>
#include <libvisr/audio_input.hpp>
#include <libvisr/audio_output.hpp>
#include <libvisr/parameter_input.hpp>

#include <libefl/basic_vector.hpp>

#include <libpml/double_buffering_protocol.hpp>

#include <memory>
#include <limits>

namespace drc
{
namespace components
{

class Compressor: public visr::AtomicComponent
{
public:
  using SampleType = visr::SampleType;

  Compressor( visr::SignalFlowContext & context,
                         char const * name,
                         visr::CompositeComponent * parent, 
                         std::size_t numberOfChannels,
			 SampleType attackTimeSeconds,
			 SampleType releaseTimeSeconds,
                         SampleType compressorThresholdDB,
                         SampleType compressorSlopeDB, 
	                 SampleType limiterThresholdDB = std::numeric_limits<SampleType>::infinity(), 
                         SampleType limiterGainDB = 0.0f );

  virtual ~Compressor();

  void process() override;
private:
  visr::AudioInput mInput;

  visr::AudioInput mOutput;

  /*
  visr::ParameterInput< visr::pml::DoubleBufferingProtocol, visr::pml::ScalarParameter<SampleType> > mListenerInput;

  visr::ParameterOutput< visr::pml::DoubleBufferingProtocol, visr::pml::MatrixParameter<SampleType> > mGainOutput;

  visr::ParameterOutput< visr::pml::DoubleBufferingProtocol, visr::pml::MatrixParameter<SampleType> > mDelayOutput;

  visr::ParameterOutput< visr::pml::MessageQueueProtocol, visr::pml::IndexedVectorFloatType > mFilterOutput;
  */
};

} // namespace components
} // namespace drc

#endif // DRC_COMPONENTS_COMPRESSOR_HPP_INCLUDED
