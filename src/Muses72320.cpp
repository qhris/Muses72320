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

using namespace MusesTypes;

namespace ControlSelectAddress
{
	constexpr data_t ATTENUATION_L() { return 0b0000 << 4; }
	constexpr data_t ATTENUATION_R() { return 0b0010 << 4; }
	constexpr data_t GAIN_L() { return 0b0001 << 4; }
	constexpr data_t GAIN_R() { return 0b0011 << 4; }
	constexpr data_t STATE() { return 0b0100 << 4; }
}

// control state bits.
static const data_t s_state_bit_zero_crossing = 5;
static const data_t s_state_bit_gain          = 6;
static const data_t s_state_bit_attenuation   = 7;

static const int s_slave_select_pin = 10;
static const SPISettings s_muses_spi_settings(250000, MSBFIRST, SPI_MODE2);

Self::Muses72320(address_t chipAddress) :
	chipAddress(chipAddress & 0b0111),
	states(0)
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

bool Self::isAttenuationLinked() const
{
	return bitRead(states, s_state_bit_attenuation);
}

bool Self::isGainLinked() const
{
	return bitRead(states, s_state_bit_gain);
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

void Self::setZeroCrossing(bool enabled)
{
	const int value = enabled ? LOW : HIGH;
	bitWrite(states, s_state_bit_zero_crossing, value);
	transfer(ControlSelectAddress::STATE(), states);
}

void Self::setAttenuationLink(bool enabled)
{
	bitWrite(states, s_state_bit_attenuation, enabled);
	transfer(ControlSelectAddress::STATE(), states);
}

void Self::setGainLink(bool enabled)
{
	bitWrite(states, s_state_bit_gain, enabled);
	transfer(ControlSelectAddress::STATE(), states);
}

void Self::transfer(address_t selectAddress, data_t data)
{
	SPI.beginTransaction(s_muses_spi_settings);
	digitalWrite(s_slave_select_pin, LOW);
	SPI.transfer(data);
	SPI.transfer(selectAddress | chipAddress);
	digitalWrite(s_slave_select_pin, HIGH);
	SPI.endTransaction();
}
