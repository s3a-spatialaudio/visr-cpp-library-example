/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#include "options.hpp"

#include "../components/compressor.hpp"

#include <libefl/denormalised_number_handling.hpp>

#include <libpml/initialise_parameter_library.hpp>

#include <libvisr/signal_flow_context.hpp>

#include <librrl/audio_signal_flow.hpp>

#include <libaudiointerfaces/audio_interface_factory.hpp>
#include <libaudiointerfaces/audio_interface.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <ciso646>
#include <cstddef>
#include <cstdlib>
#include <cstdio> // for getc(), for testing purposes
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#define CTCDSP_MAJOR_VERSION 0
#define CTCDSP_MINOR_VERSION 1
#define CTCDSP_PATCH_VERSION 0

int main( int argc, char const * const * argv )
{
  using drc::components::Compressor;
  using SampleType = Compressor::SampleType;

  using drc::standalone::Options;
  try
  {
    // Load all parameters and communication protocols into the respective factories.
    visr::pml::initialiseParameterLibrary();
        
    visr::efl::DenormalisedNumbers::State const oldDenormNumbersState
        = visr::efl::DenormalisedNumbers::setDenormHandling();
        
    Options cmdLineOptions;
    std::stringstream errMsg;
    switch (cmdLineOptions.parse(argc, argv, errMsg))
    {
    case Options::ParseResult::Failure:
      std::cerr << "Error while parsing command line options: " << errMsg.str() << std::endl;
      return EXIT_FAILURE;
    case Options::ParseResult::Help:
      cmdLineOptions.printDescription( std::cout );
      return EXIT_SUCCESS;
    case Options::ParseResult::Version:
      // TODO: Outsource the version string generation to a central file.
      std::cout << "Standalone DRC compressor 1.0.0" << std::endl;
      return EXIT_SUCCESS;
    case Options::ParseResult::Success:
      break; // carry on
    }

    std::size_t const numberOfChannels = cmdLineOptions.getDefaultedOption<std::size_t>("number-of-channels", 1 );
    SampleType const attackTimeSeconds = cmdLineOptions.getDefaultedOption<SampleType>("attack-time", 0.002f );
    SampleType const releaseTimeSeconds  = cmdLineOptions.getDefaultedOption<SampleType>("release-time", 0.01f );
    SampleType const compressorThresholdDB = cmdLineOptions.getDefaultedOption<SampleType>("compressor-threshold", -20.0f );
    SampleType const compressorSlopeDB = cmdLineOptions.getDefaultedOption<SampleType>("compressor-slope", 10.0f );
    SampleType const limiterThresholdDB = cmdLineOptions.getDefaultedOption<SampleType>("limiter-threshold",
                                                                                        std::numeric_limits<SampleType>::infinity() );
    SampleType const limiterGainDB = cmdLineOptions.getDefaultedOption<SampleType>("limiter-slope", 1.0f );

    // ============================================================================================
    // Audio configuration
    std::size_t const samplingFrequency = cmdLineOptions.getDefaultedOption<std::size_t>( "sampling-frequency", 48000 );
    std::size_t const periodSize = cmdLineOptions.getDefaultedOption<std::size_t>( "period", 1024 );

    std::string const audioBackend = cmdLineOptions.getDefaultedOption<std::string>( "audio-backend", "default" );
    // TODO: Consider moving boilerplate code to library (apputilities?)
    std::string specConf;
    bool const hasAudioInterfaceOptionString = cmdLineOptions.hasOption( "audio-ifc-options" );
    bool const hasAudioInterfaceOptionFile = cmdLineOptions.hasOption( "audio-ifc-option-file" );
    if( hasAudioInterfaceOptionString and hasAudioInterfaceOptionFile )
    {
      throw std::invalid_argument( "The options \"--audio-ifc-options\" and \"--audio-ifc-option-file\" cannot both be given." );
    }
    if( hasAudioInterfaceOptionFile )
    {
      boost::filesystem::path const audioIfcConfigFile( cmdLineOptions.getOption<std::string>( "audio-ifc-option-file" ) );
      if( not exists( audioIfcConfigFile ) )
      {
        throw std::invalid_argument( "The file specified by the \"--audio-ifc-option-file\" option does not exist." );
      }
      std::ifstream cfgStream( audioIfcConfigFile.string() );
      if( not cfgStream )
      {
        throw std::invalid_argument( "The file specified by the \"--audio-ifc-option-file\" could not be read." );
      }
      std::ostringstream fileContent;
      fileContent << cfgStream.rdbuf();
      specConf = fileContent.str();
    }
    else
    {
      specConf = hasAudioInterfaceOptionString ? cmdLineOptions.getOption<std::string>( "audio-ifc-options" ) : std::string();
    }

    // ========================================================================
    // Instantiation of signal flow.
    visr::SignalFlowContext context( periodSize, samplingFrequency );

    Compressor renderer( context, "Compressor", nullptr,
                        numberOfChannels,
                        attackTimeSeconds,
                        releaseTimeSeconds,
                        compressorThresholdDB,
                        compressorSlopeDB,
                        limiterThresholdDB,
                        limiterGainDB
                        );

    visr::audiointerfaces::AudioInterface::Configuration const
      baseConfig( numberOfChannels, numberOfChannels, samplingFrequency, periodSize );

    visr::rrl::AudioSignalFlow audioFlow( renderer );
    std::unique_ptr<visr::audiointerfaces::AudioInterface>
      audioInterface( visr::audiointerfaces::AudioInterfaceFactory::create( audioBackend, baseConfig, specConf ) );

    audioInterface->registerCallback( &visr::rrl::AudioSignalFlow::processFunction, &audioFlow );

    /*******************************************************************/


    // should there be a separate start() method for the audio interface?
    audioInterface->start();

    // Rendering runs until q<Return> is entered on the console.
    std::cout << "Realtime CTC renderer ruunning. Press \"q<Return>\" or Ctrl-C to quit." << std::endl;
    char c;
    do
    {
      c = std::getc( stdin );
    }
    while( c != 'q' );

    audioInterface->stop();

    audioInterface->unregisterCallback( &visr::rrl::AudioSignalFlow::processFunction );

    visr::efl::DenormalisedNumbers::resetDenormHandling( oldDenormNumbersState );
  }
  catch( std::exception const & ex )
  {
    std::cout << "Exception caught on top level: " << ex.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
