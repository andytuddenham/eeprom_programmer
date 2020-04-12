#pragma once

#include "EEPROM_interface.hpp"
#include "config.hpp"

namespace ext_memory
{

class EEPROM
{
public:
  EEPROM(const Config& c) : m_interface(c){};

  size_t size() { return m_interface.m_config.size; }

private:
  const EEPROM_interface m_interface;
};

} // namespace ext_memory