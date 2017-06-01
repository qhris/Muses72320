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
        // Attenuation to attenuation control data conversion:
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
        // Gain to gain control data conversion:
        // #===================================#
        // |     0 dB | in: [ 0] -> 0b00000000 |
        // | +31.5 dB | in: [63] -> 0b01111111 |
        // #===================================#
        const data_t gainData = static_cast<data_t>(Math::clamp<volume_t>(gain, 0, 63));

        return create(zeroControlAttenuation(), gainData);
    }

    static inline VolumeControlData fromFineGain()
    {
        return create(zeroControlAttenuation(), fineControlGain());
    }

    static inline VolumeControlData fromVolume(const volume_t volume)
    {
        // Volume to gain and attenuation control data.
        // #====================================================#
        // |    0.00 dB | in: [   0] -> [0b0000000, 0b00010000] |
        // |   -0.25 dB | in: [   1] -> [0b1000000, 0b00010001] |
        // | -111.50 dB | in: [-446] -> [0b0000000, 0b11101111] |
        // #====================================================#
        const volume_t clampedVolume = Math::clamp<volume_t>(volume, -446, 0);
        const volume_t attenuation = (clampedVolume - 1) / 2;
        const data_t attenuationData = fromAttenuation(attenuation).getAttenuation();
        const data_t gainData = clampedVolume % 2 == 0 ? zeroControlGain() : fineControlGain();

        return create(attenuationData, gainData);
    }

private:
    static constexpr data_t zeroControlAttenuation() { return 0b10000; }
    static constexpr data_t zeroControlGain() { return 0; }
    static constexpr data_t fineControlGain() { return 0b1000000; }

    static inline VolumeControlData create(data_t attenuation, data_t gain)
    {
        return VolumeControlData(attenuation, gain);
    }
};

#endif // INCLUDED_MUSES_72320_VOLUME_CONVERSION
