/* Copyright Institute of Sound and Vibration Research - All rights reserved */

#ifndef CRC_STANDALONE_OPTIONS_HPP_INCLUDED
#define CRC_STANDALONE_OPTIONS_HPP_INCLUDED

#include <libapputilities/options.hpp>

namespace drc
{
namespace standalone
{

class Options: public visr::apputilities::Options
{
public:
  Options();

  ~Options();
};

} // namespace standalone
} // namespace drc

#endif // #ifndef CTCDSP_CTCRENDERER_OPTIONS_HPP_INCLUDED
