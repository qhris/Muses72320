#ifndef INCLUDED_MUSES_72320_VOLUME_CONVERSION
#define INCLUDED_MUSES_72320_VOLUME_CONVERSION

#include "../MusesTypes.h"
#include "Math.hpp"

using namespace MusesTypes;

// Volume to attenuation data conversion:
// #=====================================#
// |    0.0 dB | in: [   0] -> 0b00010000 |
// | -111.5 dB | in: [-223] -> 0b11101111 |
// #=====================================#
inline data_t volume_to_attenuation(volume_t volume)
{
    const auto& v = Math::clamp<volume_t>(volume, -223, 0);
    return static_cast<data_t>(0x10 + Math::abs(v));
}

#endif // INCLUDED_MUSES_72320_VOLUME_CONVERSION
