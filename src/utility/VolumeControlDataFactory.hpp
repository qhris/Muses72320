#ifndef INCLUDED_MUSES_72320_VOLUME_CONVERSION
#define INCLUDED_MUSES_72320_VOLUME_CONVERSION

#include "../MusesTypes.h"
#include "Math.hpp"

using namespace MusesTypes;

class VolumeControlData
{
public:
    VolumeControlData() = delete;
    VolumeControlData(data_t attenuation, data_t gain) :
    attenuation(attenuation), gain(gain) { }

    data_t getGain() const { return gain; }
    data_t getAttenuation() const { return attenuation; }

private:
    data_t attenuation;
    data_t gain;
};

struct VolumeControlDataFactory
{
    static inline VolumeControlData fromAttenuation(const volume_t attenuation)
    {
        // Volume to attenuation data conversion:
        // #=====================================#
        // |    0.0 dB | in: [   0] -> 0b00010000 |
        // | -111.5 dB | in: [-223] -> 0b11101111 |
        // #=====================================#
        const volume_t clampedAttenuation = Math::clamp<volume_t>(attenuation, -223, 0);
        const data_t attenuationData = static_cast<data_t>(0x10 + Math::abs(clampedAttenuation));
        return create(attenuationData, 0);
    }

    static inline VolumeControlData fromGain(const volume_t gain);
    static inline VolumeControlData fromFineGain();
    static inline VolumeControlData fromVolume(const volume_t volume)
    {
        return create(0, 0);
    }

private:
    static inline VolumeControlData create(data_t attenuation, data_t gain)
    {
        return VolumeControlData(attenuation, gain);
    }
};

#endif // INCLUDED_MUSES_72320_VOLUME_CONVERSION
