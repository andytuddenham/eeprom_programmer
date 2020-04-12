#pragma once

#include "config.hpp"

#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

namespace ext_memory
{

// A class to interface between the EEPROM container class and the physical circuit.
class EEPROM_interface
{

  enum class Mode
  {
    READ,
    WRITE
  };

  EEPROM_interface(const Config& c) : m_config(c)
  {
    digitalWrite(m_config.SR_DATA, LOW);
    pinMode(m_config.SR_DATA, OUTPUT);

    digitalWrite(m_config.SR_CLOCK, LOW);
    pinMode(m_config.SR_CLOCK, OUTPUT);

    digitalWrite(m_config.SR_LATCH, LOW);
    pinMode(m_config.SR_LATCH, OUTPUT);

    digitalWrite(m_config.EEP_OE, HIGH);
    pinMode(m_config.EEP_OE, OUTPUT);

    // make sure WE is disabled before setting pin mode to OUTPUT
    digitalWrite(m_config.EEP_WE, HIGH);
    pinMode(m_config.EEP_WE, OUTPUT);

    digitalWrite(LED_BUILTIN, LOW);
    pinMode(LED_BUILTIN, OUTPUT);
  }

  byte read(size_t address) const
  {
    byte data = 0x00;
    read_write_handler(Mode::READ, [&]() {
      setAddress(address);
      delayMicroseconds(1);
      for (uint8_t pin = m_config.DATA_BIT0; pin <= m_config.DATA_BIT7; pin++)
      {
        data |= digitalRead(pin) << (pin - m_config.DATA_BIT0);
      }
    });

    return data;
  }

  void write(size_t address, byte data) const
  {
    // prepare to write
    read_write_handler(Mode::WRITE, [&]() {
      setAddress(address);

      // send the byte value one bit at a time
      for (uint8_t pin = m_config.DATA_BIT0; pin <= m_config.DATA_BIT7; pin++)
      {
        digitalWrite(pin, (data >> (pin - m_config.DATA_BIT0)) & 0x01);
      }

      // start the write cycle
      digitalWrite(m_config.EEP_WE, LOW);
      delayMicroseconds(1);
      digitalWrite(m_config.EEP_WE, HIGH);
    });

    // wait for the write cycle to finish
    pollUntillWriteComplete(data >> 7);
  }

  void pollUntillWriteComplete(byte bit7Value) const
  {
    read_write_handler(Mode::READ, [&]() {
      // Indicate we are entering the loop
      digitalWrite(LED_BUILTIN, HIGH);

      // reading I/O7 will return the opposite of what was written
      // until the write is complete
      while (digitalRead(m_config.DATA_BIT7) != bit7Value)
      {
        delayMicroseconds(1);
      }

      // Indicate that we are now out of the loop
      digitalWrite(LED_BUILTIN, LOW);
    });
  }

  template<typename F>
  void read_write_handler(const Mode mode, const F& op) const
  {

    if (mode == Mode::READ)
    {
      setDataPinsTo(INPUT);
      digitalWrite(m_config.EEP_OE, LOW);
      op();
      digitalWrite(m_config.EEP_OE, HIGH);
    }
    else
    {
      setDataPinsTo(OUTPUT);
      op();
    }
  }

  void setDataPinsTo(uint8_t mode) const
  {
    for (uint8_t pin = m_config.DATA_BIT0; pin <= m_config.DATA_BIT7; pin++)
    {
      pinMode(pin, mode);
    }
  }

  template<typename T>
  void setAddress(T address) const
  {
    for (size_t b = 0; b < sizeof(T); ++b)
    {
      shiftOut(
          m_config.SR_DATA,
          m_config.SR_CLOCK,
          LSBFIRST,
          static_cast<uint8_t>(address >> (8 * b)));
    }

    // latch address
    digitalWrite(m_config.SR_LATCH, LOW);
    digitalWrite(m_config.SR_LATCH, HIGH);
    digitalWrite(m_config.SR_LATCH, LOW);
  }

  bool operator==(const EEPROM_interface& interface) const
  {
    return m_config == interface.m_config;
  }

  const Config& m_config;

  friend class EEPROM;
  friend class Iter;
  friend class Ref;
};

} // namespace ext_memory