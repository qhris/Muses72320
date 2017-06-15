#ifndef INCLUDED_AUDIO_CONTROL_DATA_CONVERTER
#define INCLUDED_AUDIO_CONTROL_DATA_CONVERTER

#include "AudioControlData.hpp"
#include <Arduino.h>

struct AudioControlDataConverter
{
    static inline AudioControlData fromAttenuation(const int16_t attenuation)
    {
        // Attenuation to attenuation control data conversion:
        // #=====================================#
        // |    0.0 dB | in: [   0] -> 0b00010000 |
        // | -111.5 dB | in: [-223] -> 0b11101111 |
        // #=====================================#
        const int16_t clampedAttenuation = clampAttenuation(attenuation);
        const byte attenuationData = static_cast<byte>(0x10 + abs(clampedAttenuation));

        return create(attenuationData, zeroControlGain());
    }

    static inline AudioControlData fromGain(const int16_t gain)
    {
        // Gain to gain control data conversion:
        // #===================================#
        // |     0 dB | in: [ 0] -> 0b00000000 |
        // | +31.5 dB | in: [63] -> 0b01111111 |
        // #===================================#
        const byte gainData = static_cast<byte>(clampGain(gain));

        return create(zeroControlAttenuation(), gainData);
    }

    static inline AudioControlData fromFineGain()
    {
        return create(zeroControlAttenuation(), fineControlGain());
    }

    static inline AudioControlData fromVolume(const int16_t volume)
    {
        // Volume to gain and attenuation control data.
        // #====================================================#
        // |    0.00 dB | in: [   0] -> [0b0000000, 0b00010000] |
        // |   -0.25 dB | in: [   1] -> [0b1000000, 0b00010001] |
        // | -111.50 dB | in: [-446] -> [0b0000000, 0b11101111] |
        // #====================================================#
        const int16_t clampedVolume = clampVolume(volume);
        const int16_t attenuation = (clampedVolume - 1) / 2;
        const byte attenuationData = fromAttenuation(attenuation).getAttenuationData();
        const byte gainData = clampedVolume % 2 == 0 ? zeroControlGain() : fineControlGain();

        return create(attenuationData, gainData);
    }

private:
    static constexpr int16_t clampAttenuation(int16_t attenuation)
    {
        return constrain(attenuation, -223, 0);
    }

    static constexpr int16_t clampGain(int16_t gain)
    {
        return constrain(gain, 0, 63);
    }

    static constexpr int16_t clampVolume(int16_t volume)
    {
        return constrain(volume, -446, 0);
    }

    static constexpr byte zeroControlAttenuation() { return 0b10000; }
    static constexpr byte zeroControlGain() { return 0; }
    static constexpr byte fineControlGain() { return 0b1000000; }

    static inline AudioControlData create(byte attenuation, byte gain)
    {
        return AudioControlData(attenuation, gain);
    }
};

#endif // INCLUDED_AUDIO_CONTROL_DATA_CONVERTER
