#pragma once

namespace ext_memory
{

struct Config
{
  size_t size;

  // SR_ Shift Register pins
  uint8_t SR_CLOCK;
  uint8_t SR_LATCH;
  uint8_t SR_DATA;

  // EEP_ eeprom control pins
  uint8_t EEP_OE;
  uint8_t EEP_WE;

  // TODO take in arbitrary array of pins
  // TODO allow data to be shifted out on one pin, same as address
  // TODO allow address to be output on multiple pins (as data is now)
  // DATA_ eeprom data pins - must be consecutive ascending pins
  uint8_t DATA_BIT0;
  uint8_t DATA_BIT7;
};

} // namespace ext_memory