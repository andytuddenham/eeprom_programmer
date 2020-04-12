#pragma once

#include "EEPROM_interface.hpp"

namespace ext_memory
{

// Reference to a cell in the EEPROM
class Ref
{
public:
  Ref(const EEPROM_interface& interface, size_t addr) : m_addr(addr), m_interface(interface) {}
  Ref(const Ref& ref) : Ref(ref.m_interface, ref.m_addr) {}
  operator uint8_t() const { return **this; }
  operator int() const { return **this; }

  byte operator*() const { return m_interface.read(m_addr); }

  Ref& operator=(const Ref& ref) { return *this = *ref; }
  Ref& operator=(byte val) { return m_interface.write(m_addr, val), *this; }

  Ref& operator+=(byte val) { return *this = **this + val; }
  Ref& operator-=(byte val) { return *this = **this - val; }
  Ref& operator*=(byte val) { return *this = **this * val; }
  Ref& operator/=(byte val) { return *this = **this / val; }
  Ref& operator^=(byte val) { return *this = **this ^ val; }
  Ref& operator%=(byte val) { return *this = **this % val; }
  Ref& operator&=(byte val) { return *this = **this & val; }
  Ref& operator|=(byte val) { return *this = **this | val; }
  Ref& operator<<=(byte val) { return *this = **this << val; }
  Ref& operator>>=(byte val) { return *this = **this >> val; }

  Ref& operator++() { return *this += 1; }
  Ref& operator--() { return *this -= 1; }

  Ref operator++(int)
  {
    Ref ref(*this);
    operator++();
    return ref;
  }

  Ref operator--(int)
  {
    Ref ref(*this);
    operator--();
    return ref;
  }

  size_t addr() { return m_addr; }

private:
  size_t m_addr;
  const EEPROM_interface& m_interface;
}; // Ref

} // namespace ext_memory