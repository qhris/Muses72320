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
#include "utility/VolumeControlData.hpp"
#include "utility/VolumeControlDataFactory.hpp"
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
	const auto controlData = VolumeControlDataFactory::fromVolume(volume);
	transfer(ControlSelectAddress::ATTENUATION_L(), controlData.getAttenuation());
	transfer(ControlSelectAddress::GAIN_L(), controlData.getGain());
}

void Self::setVolumeRight(volume_t volume)
{
	const auto controlData = VolumeControlDataFactory::fromVolume(volume);
	if (!isAttenuationLinked())
		transfer(ControlSelectAddress::ATTENUATION_R(), controlData.getAttenuation());
	if (!isGainLinked())
		transfer(ControlSelectAddress::GAIN_R(), controlData.getGain());
}

void Self::setAttenuationLeft(volume_t attenuation)
{
	const auto controlData = VolumeControlDataFactory::fromAttenuation(attenuation);
	transfer(ControlSelectAddress::ATTENUATION_L(), controlData.getAttenuation());
}

void Self::setAttenuationRight(volume_t attenuation)
{
	if (isAttenuationLinked()) return;
	const auto controlData = VolumeControlDataFactory::fromAttenuation(attenuation);
	transfer(ControlSelectAddress::ATTENUATION_R(), controlData.getAttenuation());
}

void Self::setGainLeft(volume_t gain)
{
	const auto controlData = VolumeControlDataFactory::fromGain(gain);
	transfer(ControlSelectAddress::GAIN_L(), controlData.getGain());
}

void Self::setGainRight(volume_t gain)
{
	if (isGainLinked()) return;
	const auto controlData = VolumeControlDataFactory::fromGain(gain);
	transfer(ControlSelectAddress::GAIN_R(), controlData.getGain());
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
	bitWrite(data, StateControlBits::ZERO_CROSSING(), stateData.zeroCrossing ? LOW : HIGH);
	bitWrite(data, StateControlBits::GAIN(), stateData.linkGain ? HIGH : LOW);
	bitWrite(data, StateControlBits::ATTENUATION(), stateData.linkAttenuation ? HIGH : LOW);

	return data;
}
