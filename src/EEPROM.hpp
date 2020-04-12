#pragma once

#include "EEPROM_interface.hpp"
#include "Iter.hpp"
#include "Ref.hpp"
#include "config.hpp"

namespace ext_memory
{

class EEPROM
{
public:
  EEPROM() = delete;
  EEPROM(const Config& c) : m_interface(c){};
  byte read(const size_t index) { return static_cast<byte>(new_ref(index)); }
  void write(const size_t index, const byte val) { new_ref(index) = val; }
  Iter begin() { return new_iter(0); }
  Iter end() { return new_iter(size()); }
  Ref operator[](const size_t index) { return new_ref(index); }

  size_t size() { return m_interface.m_config.size; }

private:
  const EEPROM_interface m_interface;
  Ref new_ref(size_t index) const { return Ref(m_interface, index); }
  Iter new_iter(size_t index) const { return Iter(m_interface, index); }
};

} // namespace ext_memory