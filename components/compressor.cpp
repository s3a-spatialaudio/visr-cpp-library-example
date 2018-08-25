/* Copyright Andreas Franck 2018 - All rights reserved */
/* Copyright Institute of Sound and Vibration Research 2018
   - All rights reserved */

#include "compressor.hpp"

#include <libvisr/signal_flow_context.hpp>
#include <libvisr/constants.hpp>

#include <libefl/vector_functions.hpp>

#include <cmath>

namespace drc
{
namespace components
{

Compressor::Compressor( visr::SignalFlowContext & context,
                        char const * name,
                        visr::CompositeComponent * parent,
                        std::size_t numberOfChannels,
                        SampleType compressorThresholdDB,
                        SampleType compressorSlope,
                        SampleType averagingTimeSeconds,
                        SampleType attackTimeSeconds,
                        SampleType releaseTimeSeconds
                      )
  : visr::AtomicComponent( context, name, parent )
  , mInput( "in", *this, numberOfChannels )
  , mOutput( "out", *this, numberOfChannels )
  , mAveragingCoefficient( timeConstantToCoefficient(averagingTimeSeconds) )
  , mAttackCoefficient( timeConstantToCoefficient(attackTimeSeconds) )
  , mReleaseCoefficient( timeConstantToCoefficient(releaseTimeSeconds) )
  , mCompressorThreshold( compressorThresholdDB )
  , mCompressorSlope( compressorSlope )
  , mControlValues( context.period(), visr::cVectorAlignmentSamples )
  , mGainValues( context.period(), visr::cVectorAlignmentSamples )
  , mPastPeakValues( numberOfChannels, visr::cVectorAlignmentSamples )
  , mPastRmsValues( numberOfChannels, visr::cVectorAlignmentSamples )
{
}
 
Compressor::~Compressor() = default;

void Compressor::process()
{
  std::size_t const numberOfChannels{ mInput.width() };
  std::size_t const numberOfSamples{ period() };
  for( std::size_t chIdx{0}; chIdx < numberOfChannels; ++chIdx )
  {
    averagingFilter( mInput[chIdx], mControlValues.data(),
                     mAveragingCoefficient,
                     numberOfSamples, mPastRmsValues[chIdx] );
    // Linear -> log conversion
    // should be a vectorised library call
    // Note: The averaging filter returns the averaged signal power, that means we have to use factor 10 in the lin->log conversion.
    std::for_each( mControlValues.data(), mControlValues.data()+numberOfSamples,
                  []( SampleType & val ){ val = static_cast<SampleType>(10.0) * std::log10( val ); } );
    for( std::size_t sampleIdx{0}; sampleIdx < numberOfSamples; ++sampleIdx )
    {
      if( mControlValues[sampleIdx] > mCompressorThreshold )
      {
        mControlValues[sampleIdx] = (mCompressorThreshold - mControlValues[sampleIdx]) * mCompressorSlope;
      }
      else
      {
        mControlValues[sampleIdx] = static_cast<SampleType>(0.0);
      }
    }
    // Log-> linear conversion
    // This should be a vectorised function call (e.g., from libefl)
    // Note: these are amplitude values, hence the constant 0.05 = 1/20 in the conversion.
    std::for_each( mControlValues.data(), mControlValues.data()+numberOfSamples,
                  []( SampleType & val ){ val =  std::pow( static_cast<SampleType>(10.0),
                                                           static_cast<SampleType>(0.05)*val ); } );
    peakFilterInplace( mControlValues.data(), mAttackCoefficient, mReleaseCoefficient,
                       numberOfSamples, mPastPeakValues[chIdx] );

#if 0
    // Output the linear control gain instead of the compressed signal
    visr::efl::ErrorCode const err = visr::efl::vectorCopy( mControlValues.data(),
                                                           mOutput[chIdx],
                                                           numberOfSamples,
                                                           visr::cVectorAlignmentSamples );
#else
    visr::efl::ErrorCode const err = visr::efl::vectorMultiply( mInput[chIdx],
                                                                mControlValues.data(),
                                                                mOutput[chIdx],
                                                                numberOfSamples,
                                                                visr::cVectorAlignmentSamples );
#endif
    if( err != visr::efl::noError )
    {
      status( visr::StatusMessage::Error, "Error while multiplying control gain.",
              visr::efl::errorMessage(err));
    }
  }
}

Compressor::SampleType
Compressor::timeConstantToCoefficient( SampleType timeConstant ) const
{
  SampleType const arg{ -2.2f / timeConstant / static_cast<SampleType>(samplingFrequency()) };
  return static_cast<SampleType>(1.0) - std::exp( arg );
}

/*static*/ void Compressor::
peakFilterInplace( SampleType * const values,
                          SampleType attackCoefficient,
                          SampleType releaseCoefficient,
                          std::size_t numberOfSamples,
                          SampleType & state )
{
  SampleType xp = state;
  for( std::size_t sampleIdx{0}; sampleIdx < numberOfSamples; ++sampleIdx )
  {
    SampleType const xc{ values[sampleIdx] };
    SampleType const coeff{ xc > xp ? attackCoefficient : releaseCoefficient };
    xp = (static_cast<SampleType>(1.0)-coeff) * xp +
            + coeff * xc;
    values[sampleIdx] = xp;
  }
  state = xp;
}

/*static*/ void Compressor::
averagingFilter( SampleType const * const input,
                 SampleType * const output,
                 SampleType filterCoeff,
                 std::size_t numberOfSamples,
                 SampleType & state )
{
  SampleType const oneMinusC{ static_cast<SampleType>(1.0) - filterCoeff };
  for( std::size_t sampleIdx{0}; sampleIdx < numberOfSamples; ++sampleIdx )
  {
    SampleType const xPwr = input[sampleIdx] * input[sampleIdx];
    state = oneMinusC * state + filterCoeff * xPwr;
    output[sampleIdx] = state;
  }
}

} // namespace components
} // namespace drc
