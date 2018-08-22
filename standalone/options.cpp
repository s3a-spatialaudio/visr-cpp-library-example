/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#include "options.hpp"

#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <string>

namespace drc
{
namespace standalone
{

Options::Options()
 : visr::apputilities::Options()
{
  registerOption<std::string>( "audio-backend,D", "The audio backend." );
  registerOption<std::string>( "audio-ifc-options", "Audio interface optional configuration" );
  registerOption<std::string>( "audio-ifc-option-file", "Audio interface optional configuration file" );

  registerOption<std::size_t>( "sampling-frequency,f", "Sampling frequency [Hz]" );
  registerOption<std::size_t>( "period,p", "Period (blocklength) [Number of samples per audio block]" );

  registerOption<std::size_t>( "number-of-channels,n", "Number of channels (default=1)" );

  registerOption<double>( "attack-time,a", "Attack time [seconds], (default=0.002 s)" );
  registerOption<double>( "release-time,r", "Release time [seconds], (default=0.01 s)" );
  
  registerOption<double>( "compressor-threshold,t", "Compressor threshold [dB], (default= -20 dB)" );
  registerOption<double>( "compressor-slope,s", "Compressor slope [dB/dec], (default= -20 dB)" );

  registerOption<double>( "limiter-threshold", "Limiter threshold [dB], (default= inf dB, i.e., limiting disabled)" );
  registerOption<double>( "limiter-slope", "Limiter slope [dB/dec], (default= -60 dB)" );
}

Options::~Options() = default;

} // namespace standalone
} // namespace drc
