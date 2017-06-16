/*
The MIT License (MIT)

Copyright (c) 2016 Christoffer Hjalmarsson

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "utility/AudioControlData.hpp"
#include "utility/AudioControlDataConverter.hpp"
#include "utility/SPITransactionalWriter.hpp"
#include "Muses72320.h"

#include <SPI.h>

typedef Muses72320 Self;

using namespace MusesDetails;

namespace ControlSelectAddress
{
	constexpr byte attenuationL() { return 0b0000 << 4; }
	constexpr byte attenuationR() { return 0b0010 << 4; }
	constexpr byte gainL() { return 0b0001 << 4; }
	constexpr byte gainR() { return 0b0011 << 4; }
	constexpr byte state() { return 0b0100 << 4; }
}

namespace StateControlBits
{
	constexpr byte zeroCrossing() { return 5; }
	constexpr byte gain() { return 6; }
	constexpr byte attenuation() { return 7; }
}

constexpr byte boolToByte(byte x) { return x ? HIGH : LOW; }
constexpr byte boolToInvertedByte(byte x) { return x ? LOW : HIGH; }

static byte translateStateControlData(StateControlData stateData);

static const SPISettings MUSES_SPI_SETTINGS(250000, MSBFIRST, SPI_MODE2);

Self::Muses72320(byte chipAddress, byte slaveSelectPin) :
	chipAddress(chipAddress & 0b0111),
	slaveSelectPin(slaveSelectPin),
	state{ true, false, false }
{ }

void Self::begin()
{
	digitalWrite(slaveSelectPin, HIGH);
	pinMode(slaveSelectPin, OUTPUT);

	SPI.begin();
}

void Self::setVolumeLeft(int16_t volume)
{
	const auto audio = AudioControlDataConverter::fromVolume(volume);
	transfer(ControlSelectAddress::attenuationL(), audio.getAttenuationData());
	transfer(ControlSelectAddress::gainL(), audio.getGainData());
}

void Self::setVolumeRight(int16_t volume)
{
	const auto audio = AudioControlDataConverter::fromVolume(volume);
	if (!isAttenuationLinked())
		transfer(ControlSelectAddress::attenuationR(), audio.getAttenuationData());
	if (!isGainLinked())
		transfer(ControlSelectAddress::gainR(), audio.getGainData());
}

void Self::setAttenuationLeft(int16_t attenuation)
{
	const auto audio = AudioControlDataConverter::fromAttenuation(attenuation);
	transfer(ControlSelectAddress::attenuationL(), audio.getAttenuationData());
}

void Self::setAttenuationRight(int16_t attenuation)
{
	if (isAttenuationLinked()) return;
	const auto audio = AudioControlDataConverter::fromAttenuation(attenuation);
	transfer(ControlSelectAddress::attenuationR(), audio.getAttenuationData());
}

void Self::setGainLeft(int16_t gain)
{
	const auto audio = AudioControlDataConverter::fromGain(gain);
	transfer(ControlSelectAddress::gainL(), audio.getGainData());
}

void Self::setGainRight(int16_t gain)
{
	if (isGainLinked()) return;
	const auto audio = AudioControlDataConverter::fromGain(gain);
	transfer(ControlSelectAddress::gainR(), audio.getGainData());
}

void Self::muteLeft()
{
	transfer(ControlSelectAddress::attenuationL(), 0);
}

void Self::muteRight()
{
	if (isAttenuationLinked()) return;
	transfer(ControlSelectAddress::attenuationR(), 0);
}

void Self::enableZeroCrossing()
{
	state.zeroCrossing = true;
	transferState();
}

void Self::disableZeroCrossing()
{
	state.zeroCrossing = false;
	transferState();
}

void Self::enableAttenuationLink()
{
	state.linkAttenuation = true;
	transferState();
}

void Self::disableAttenuationLink()
{
	state.linkAttenuation = false;
	transferState();
}

void Self::enableGainLink()
{
	state.linkGain = true;
	transferState();
}

void Self::disableGainLink()
{
	state.linkGain = false;
	transferState();
}

void Self::transferState()
{
	byte data = translateStateControlData(state);
	transfer(ControlSelectAddress::state(), data);
}

void Self::transfer(byte selectAddress, byte data)
{
	SPITransactionalWriter writer(MUSES_SPI_SETTINGS, slaveSelectPin);
	writer.write(chipAddress, selectAddress, data);
}

byte translateStateControlData(StateControlData stateData)
{
	byte data = 0;
	bitWrite(data, StateControlBits::zeroCrossing(), boolToInvertedByte(stateData.zeroCrossing));
	bitWrite(data, StateControlBits::gain(), boolToByte(stateData.linkGain));
	bitWrite(data, StateControlBits::attenuation(), boolToByte(stateData.linkAttenuation));

	return data;
}
