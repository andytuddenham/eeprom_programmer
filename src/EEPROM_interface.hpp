#pragma once

#include "config.hpp"

#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

namespace ext_memory
{

class EEPROM_interface
{

  EEPROM_interface(const Config& c) : m_config(c) { m_data = new byte[c.size]; }

  byte read(size_t address) const { return m_data[address]; }

  void write(size_t address, byte data) const { m_data[address] = data; }

  bool operator==(const EEPROM_interface& interface) const
  {
    return m_config == interface.m_config;
  }

  const Config& m_config;
  // TODO for testing -- delete later
  byte* m_data;

  friend class EEPROM;
  friend class Ref;
};

} // namespace ext_memory