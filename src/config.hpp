#pragma once

#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

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

  bool operator==(const Config& cfg) const
  {
    return size == cfg.size && SR_CLOCK == cfg.SR_CLOCK && SR_LATCH == cfg.SR_LATCH
           && SR_DATA == cfg.SR_DATA && EEP_OE == cfg.EEP_OE && EEP_WE == cfg.EEP_WE
           && DATA_BIT0 == cfg.DATA_BIT0 && DATA_BIT7 == cfg.DATA_BIT7;
  }
};

constexpr size_t AT28C64_size = static_cast<size_t>(pow(2, 13)) - 1;
constexpr size_t AT28C256_size = static_cast<size_t>(pow(2, 15)) - 1;
constexpr Config suggested_AT28C64_config = {AT28C64_size, A2, A3, 10, A4, A5, 2, 9};
constexpr Config suggested_AT28C256_config = {AT28C256_size, A2, A3, 10, A4, A5, 2, 9};

} // namespace ext_memory