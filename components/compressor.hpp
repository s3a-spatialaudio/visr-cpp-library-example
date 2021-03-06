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

/**
 * Audio processing component for multichannel dynamic range compression.
 */
class Compressor: public visr::AtomicComponent
{
public:
  using SampleType = visr::SampleType;

  /**
   * Constructor.
   * @param context Object containing standard signal processing parameters
   * (sampling rate, block size).
   * @param name
   * @param parent The containing component, or nullptr if this is the top-level component.
   * @param numberOfChannels The umber of channels that are processed
   * simultaneously.
   * @param compressorThresholdDB The level [in dB] where the compression sets in.
   * @param compressorSlope The slope of the compressor, in dB/octave.
   * @param averagingTimeSeconds,
   * @param attackTimeSeconds,
   * @param releaseTimeSeconds
   */
  Compressor( visr::SignalFlowContext & context,
              char const * name,
              visr::CompositeComponent * parent,
              std::size_t numberOfChannels,
              SampleType compressorThresholdDB,
              SampleType compressorSlope,
              SampleType averagingTimeSeconds,
              SampleType attackTimeSeconds,
              SampleType releaseTimeSeconds
            );

  virtual ~Compressor();

  void process() override;
private:
  visr::AudioInput mInput;

  visr::AudioOutput mOutput;

  SampleType mAveragingCoefficient;

  SampleType mAttackCoefficient;

  SampleType mReleaseCoefficient;

  SampleType mCompressorThreshold;

  SampleType mCompressorSlope;

  visr::efl::BasicVector<SampleType> mControlValues;

  visr::efl::BasicVector<SampleType> mGainValues;

  visr::efl::BasicVector<SampleType> mPastRmsValues;

  visr::efl::BasicVector<SampleType> mPastPeakValues;

  /**
   * Compute a filter coefficient for a attack or release  coefficient based on a
   * time constant.
   */
  SampleType timeConstantToCoefficient( SampleType timeConstant ) const;

  /**
   * Compute a sequence of peak measurement values based on the attack/release time settings.
   * Note: The input values are assumed to be positive
   * @param [in,out] values
   * @param attackCoefficient
   * @param releaseCoefficient 
   * @param [in,out] state
   */
  static void peakFilterInplace( SampleType * const values,
                                 SampleType attackCoefficient,
                                 SampleType releaseCoefficient,
                                 std::size_t numberOfSamples,
                                 SampleType & state );

  static void averagingFilter( SampleType const * const input,
                               SampleType * const output,
                               SampleType filterCoeff,
                               std::size_t numberOfSamples,
                               SampleType & state );
};

} // namespace components
} // namespace drc

#endif // DRC_COMPONENTS_COMPRESSOR_HPP_INCLUDED
