// This file defines the catch main entry point. This should only happen in this
// file, other test files should only include the header.
#define CATCH_CONFIG_MAIN
#include <catch/catch.hpp>
#include <Arduino.h>
#include <SPI.h>

uint8_t PORTD;
uint8_t PORTB;

SPIClass SPI;

int ArduinoMock::pinModes[16] = {};
byte ArduinoMock::pinValues[16] = {};
