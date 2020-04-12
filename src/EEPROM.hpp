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

  Iter begin() { return new_iter(0); }
  Iter end() { return new_iter(size()); }

  Ref operator[](const size_t addr) { return new_ref(addr); }
  size_t size() { return m_interface.m_config.size; }

  void erase() { fill(0xff); }

  void fill(uint8_t val)
  {
    for (Ref ref: *this)
    {
      ref = val;
    }
  }

  template<typename T>
  T get(const size_t addr)
  {
    T data;
    get(addr, data);
    return data;
  }

  template<typename T>
  void get(const size_t addr, T& t)
  {
    Iter iter = new_iter(addr);
    uint8_t* data_ptr = reinterpret_cast<uint8_t*>(&t);
    for (size_t b = 0; b < sizeof(T); ++b)
    {
      *data_ptr++ = *iter++;
    }
  }

  template<typename T>
  void get_array(const size_t addr, T* data, size_t size)
  {
    for (size_t i = 0; i < size; ++i)
    {
      get(addr + (i * sizeof(T)), data[i]);
    }
  }

  template<typename T, size_t N>
  void get_array(const size_t addr, T (&data)[N])
  {
    for (size_t i = 0; i < N; ++i)
    {
      get(addr + (i * sizeof(T)), data[i]);
    }
  }

  template<typename T>
  void set(const size_t addr, const T& t)
  {
    Iter iter = new_iter(addr);
    const uint8_t* data_ptr = reinterpret_cast<const uint8_t*>(&t);
    for (size_t b = 0; b < sizeof(T); ++b)
    {
      *iter++ = *data_ptr++;
    }
  }

  template<typename T, size_t N>
  void set_array(const size_t addr, T (&data)[N])
  {
    for (size_t i = 0; i < N; ++i)
    {
      set(addr + (i * sizeof(T)), data[i]);
    }
  }

  template<typename F>
  void get_content_in_chunks(size_t chunk_size, F& f)
  {
    for (uint16_t addr = 0; addr <= size() - chunk_size; addr += chunk_size)
    {
      byte data[chunk_size] = {0};
      get_array(addr, data, chunk_size);
      f(addr, data, chunk_size);
    }
  }

private:
  const EEPROM_interface m_interface;
  Ref new_ref(size_t addr) const { return Ref(m_interface, addr); }
  Iter new_iter(size_t addr) const { return Iter(m_interface, addr); }
};

} // namespace ext_memory