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

        return create(attenuationData, zeroControlGain());
    }

    static inline VolumeControlData fromGain(const volume_t gain)
    {
        // volume to gain data conversion:
        // #===================================#
        // |     0 dB | in: [ 0] -> 0b00000000 |
        // | +31.5 dB | in: [63] -> 0b01111111 |
        // #===================================#
        const data_t gainData = static_cast<data_t>(Math::clamp<volume_t>(gain, 0, 63));

        return create(zeroControlAttenuation(), gainData);
    }

    static inline VolumeControlData fromFineGain();
    static inline VolumeControlData fromVolume(const volume_t volume)
    {
        return create(0, 0);
    }

private:
    static constexpr data_t zeroControlAttenuation() { return 0b10000; }
    static constexpr data_t zeroControlGain() { return 0; }

    static inline VolumeControlData create(data_t attenuation, data_t gain)
    {
        return VolumeControlData(attenuation, gain);
    }
};

#endif // INCLUDED_MUSES_72320_VOLUME_CONVERSION
