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
#include "Muses72320.h"

#include <SPI.h>

typedef Muses72320 Self;

using namespace MusesDetails;
using namespace MusesTypes;

namespace ControlSelectAddress
{
	constexpr address_t ATTENUATION_L() { return 0b0000 << 4; }
	constexpr address_t ATTENUATION_R() { return 0b0010 << 4; }
	constexpr address_t GAIN_L() { return 0b0001 << 4; }
	constexpr address_t GAIN_R() { return 0b0011 << 4; }
	constexpr address_t STATE() { return 0b0100 << 4; }
}

namespace StateControlBits
{
	constexpr byte ZERO_CROSSING() { return 5; }
	constexpr byte GAIN() { return 6; }
	constexpr byte ATTENUATION() { return 7; }
}

constexpr byte boolToByte(byte x) { return x ? HIGH : LOW; }
constexpr byte boolToInvertedByte(byte x) { return x ? LOW : HIGH; }

static byte translateStateControlData(StateControlData stateData);

static const int s_slave_select_pin = 10;
static const SPISettings MUSES_SPI_SETTINGS(250000, MSBFIRST, SPI_MODE2);

Self::Muses72320(address_t chipAddress) :
	chipAddress(chipAddress & 0b0111),
	state{ true, false, false }
{ }

void Self::begin()
{
	pinMode(s_slave_select_pin, OUTPUT);
	SPI.begin();
}

void Self::setVolumeLeft(volume_t volume)
{
	const auto audio = AudioControlDataConverter::fromVolume(volume);
	transfer(ControlSelectAddress::ATTENUATION_L(), audio.getAttenuationData());
	transfer(ControlSelectAddress::GAIN_L(), audio.getGainData());
}

void Self::setVolumeRight(volume_t volume)
{
	const auto audio = AudioControlDataConverter::fromVolume(volume);
	if (!isAttenuationLinked())
		transfer(ControlSelectAddress::ATTENUATION_R(), audio.getAttenuationData());
	if (!isGainLinked())
		transfer(ControlSelectAddress::GAIN_R(), audio.getGainData());
}

void Self::setAttenuationLeft(volume_t attenuation)
{
	const auto audio = AudioControlDataConverter::fromAttenuation(attenuation);
	transfer(ControlSelectAddress::ATTENUATION_L(), audio.getAttenuationData());
}

void Self::setAttenuationRight(volume_t attenuation)
{
	if (isAttenuationLinked()) return;
	const auto audio = AudioControlDataConverter::fromAttenuation(attenuation);
	transfer(ControlSelectAddress::ATTENUATION_R(), audio.getAttenuationData());
}

void Self::setGainLeft(volume_t gain)
{
	const auto audio = AudioControlDataConverter::fromGain(gain);
	transfer(ControlSelectAddress::GAIN_L(), audio.getGainData());
}

void Self::setGainRight(volume_t gain)
{
	if (isGainLinked()) return;
	const auto audio = AudioControlDataConverter::fromGain(gain);
	transfer(ControlSelectAddress::GAIN_R(), audio.getGainData());
}

void Self::muteLeft()
{
	transfer(ControlSelectAddress::ATTENUATION_L(), 0);
}

void Self::muteRight()
{
	if (isAttenuationLinked()) return;
	transfer(ControlSelectAddress::ATTENUATION_R(), 0);
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
	transfer(ControlSelectAddress::STATE(), data);
}

void Self::transfer(address_t selectAddress, byte data)
{
	SPI.beginTransaction(MUSES_SPI_SETTINGS);
	digitalWrite(s_slave_select_pin, LOW);
	SPI.transfer(data);
	SPI.transfer(selectAddress | chipAddress);
	digitalWrite(s_slave_select_pin, HIGH);
	SPI.endTransaction();
}

byte translateStateControlData(StateControlData stateData)
{
	byte data = 0;
	bitWrite(data, StateControlBits::ZERO_CROSSING(), boolToInvertedByte(stateData.zeroCrossing));
	bitWrite(data, StateControlBits::GAIN(), boolToByte(stateData.linkGain));
	bitWrite(data, StateControlBits::ATTENUATION(), boolToByte(stateData.linkAttenuation));

	return data;
}
