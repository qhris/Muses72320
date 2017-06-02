#ifndef INCLUDED_VOLUME_CONTROL_DATA
#define INCLUDED_VOLUME_CONTROL_DATA

#include "../MusesTypes.h"

class VolumeControlData
{
    using data_t = MusesTypes::data_t;

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

#endif // INCLUDED_VOLUME_CONTROL_DATA
