#pragma once

#include "EEPROM_interface.hpp"
#include "Ref.hpp"

namespace ext_memory
{

// Proxy Iterator for EEPROM
class Iter
{
public:
  Iter(const EEPROM_interface& interface, size_t index) : m_index(index), m_interface(interface) {}
  Iter(const Iter& iter) : Iter(iter.m_interface, iter.m_index) {}

  operator size_t() const { return m_index; }
  operator int() const { return m_index; }

  Ref operator*() const { return Ref(m_interface, m_index); }

  bool operator==(const Iter& Iter) const
  {
    return m_index == Iter.m_index && m_interface == Iter.m_interface;
  }

  bool operator!=(const Iter& Iter) const { return !(*this == Iter); }

  // TODO vvv (Doesn't seem to be needed but is reccomended)
  Iter& operator=(const Iter& iter) = delete;

  Iter& operator=(size_t val) { return m_index = val, *this; }
  Iter& operator+=(size_t val) { return m_index += val, *this; }
  Iter& operator-=(size_t val) { return m_index -= val, *this; }
  Iter& operator*=(size_t val) { return m_index *= val, *this; }
  Iter& operator/=(size_t val) { return m_index /= val, *this; }
  Iter& operator^=(size_t val) { return m_index ^= val, *this; }
  Iter& operator%=(size_t val) { return m_index %= val, *this; }
  Iter& operator&=(size_t val) { return m_index &= val, *this; }
  Iter& operator|=(size_t val) { return m_index |= val, *this; }
  Iter& operator<<=(size_t val) { return m_index <<= val, *this; }
  Iter& operator>>=(size_t val) { return m_index <<= val, *this; }

  Iter& operator++() { return ++m_index, *this; }
  Iter& operator--() { return --m_index, *this; }

  Iter operator++(int)
  {
    Iter iter(*this);
    operator++();
    return iter;
  }

  Iter operator--(int)
  {
    Iter iter(*this);
    operator--();
    return iter;
  }

private:
  size_t m_index;
  const EEPROM_interface& m_interface;
};

} // namespace ext_memory