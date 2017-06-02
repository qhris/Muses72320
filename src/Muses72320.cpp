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

// control select addresses, chip address (low 4) ignored.
static const data_t s_control_attenuation_l = 0b00000000;
static const data_t s_control_attenuation_r = 0b00100000;
static const data_t s_control_gain_l        = 0b00010000;
static const data_t s_control_gain_r        = 0b00110000;
static const data_t s_control_states        = 0b01000000;

// control state bits.
static const data_t s_state_bit_zero_crossing = 5;
static const data_t s_state_bit_gain          = 6;
static const data_t s_state_bit_attenuation   = 7;

static const int s_slave_select_pin = 10;
static const SPISettings s_muses_spi_settings(250000, MSBFIRST, SPI_MODE2);

Self::Muses72320(address_t chip_address) :
	chip_address(chip_address & 0b0111),
	states(0)
{ }

void Self::begin()
{
	pinMode(s_slave_select_pin, OUTPUT);
	SPI.begin();
}

void Self::setVolume(volume_t lch, volume_t rch)
{
	if (bitRead(states, s_state_bit_attenuation)) {
		// interconnected left and right channels.
		const auto controlData = VolumeControlDataFactory::fromVolume(lch);
		transfer(s_control_attenuation_l, controlData.getAttenuation());
		transfer(s_control_gain_l, controlData.getGain());
	} else {
		// independent left and right channels.
		const auto controlDataL = VolumeControlDataFactory::fromVolume(lch);
		transfer(s_control_attenuation_l, controlDataL.getAttenuation());
		transfer(s_control_gain_l, controlDataL.getGain());
		const auto controlDataR = VolumeControlDataFactory::fromVolume(rch);
		transfer(s_control_attenuation_r, controlDataR.getAttenuation());
		transfer(s_control_gain_r, controlDataR.getGain());
	}
}

void Self::setAttenuation(volume_t lch, volume_t rch)
{
	if (bitRead(states, s_state_bit_attenuation)) {
		// interconnected left and right channels.
		const auto controlData = VolumeControlDataFactory::fromAttenuation(lch);
		transfer(s_control_attenuation_l, controlData.getAttenuation());
	} else {
		// independent left and right channels.
		const auto controlDataL = VolumeControlDataFactory::fromAttenuation(lch);
		transfer(s_control_attenuation_l, controlDataL.getAttenuation());
		const auto controlDataR = VolumeControlDataFactory::fromAttenuation(rch);
		transfer(s_control_attenuation_r, controlDataR.getAttenuation());
	}
}

void Self::setGain(volume_t lch, volume_t rch)
{
	if (bitRead(states, s_state_bit_gain)) {
		// interconnected left and right channels.
		const auto controlData = VolumeControlDataFactory::fromGain(lch);
		transfer(s_control_gain_l, controlData.getGain());
	} else {
		// independent left and right channels.
		const auto controlDataL = VolumeControlDataFactory::fromGain(lch);
		transfer(s_control_gain_l, controlDataL.getGain());
		const auto controlDataR = VolumeControlDataFactory::fromGain(rch);
		transfer(s_control_gain_r, controlDataR.getGain());
	}
}

void Self::mute(bool left, bool right)
{
	if (bitRead(states, s_state_bit_attenuation)) {
		if (left)  transfer(s_control_attenuation_l, 0);
	} else {
		if (left)  transfer(s_control_attenuation_l, 0);
		if (right) transfer(s_control_attenuation_r, 0);
	}
}

void Self::setZeroCrossing(bool enabled)
{
	// 0 is enabled, 1 is disabled.
	bitWrite(states, s_state_bit_zero_crossing, !enabled);
	transfer(s_control_states, states);
}

void Self::setAttenuationLink(bool enabled)
{
	// 1 is enabled, 0 is disabled.
	bitWrite(states, s_state_bit_attenuation, enabled);
	transfer(s_control_states, states);
}

void Self::setGainLink(bool enabled)
{
	// 1 is enabled, 0 is disabled.
	bitWrite(states, s_state_bit_gain, enabled);
	transfer(s_control_states, states);
}

void Self::transfer(address_t address, data_t data)
{
	SPI.beginTransaction(s_muses_spi_settings);
	digitalWrite(s_slave_select_pin, LOW);
	SPI.transfer(data);
	SPI.transfer(address | chip_address);
	digitalWrite(s_slave_select_pin, HIGH);
	SPI.endTransaction();
}
