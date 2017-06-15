#ifndef INCLUDED_AUDIO_CONTROL_DATA
#define INCLUDED_AUDIO_CONTROL_DATA

#include <Arduino.h>

class AudioControlData
{
public:
    AudioControlData() = delete;
    AudioControlData(byte attenuation, byte gain) :
        attenuation(attenuation), gain(gain) { }

    byte getGainData() const { return gain; }
    byte getAttenuationData() const { return attenuation; }

private:
    byte attenuation;
    byte gain;
};

#endif // INCLUDED_AUDIO_CONTROL_DATA
