#pragma once

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class EEPROMP
{
public:
  enum class Chip
  {
    // used to index an array, so must start with zero
    // and increment sequentially (the default).
    AT28C64, // ATMEL 64K Parallel eeprom (8K x 8)
    AT28C256 // ATMEL 256K Parallel eeprom (32K x 8)
  };

  static EEPROMP &getEeprom();

  bool setChip(Chip);
  bool readByte(uint16_t address, byte &data) const;
  bool readArray(uint16_t startAddress, byte *data, int size) const;
  bool writeByte(uint16_t address, byte data) const;
  bool writeArray(uint16_t startAddress, byte *data, int size) const;
  bool printContents(void (*callback)(uint16_t address, byte *data, size_t dataSize)) const;

  EEPROMP(EEPROMP const &) = delete;
  void operator=(EEPROMP const &) = delete;

private:
  // SR_ Shift Register pins
  static const uint8_t SR_CLOCK = A2;
  static const uint8_t SR_LATCH = A3;
  static const uint8_t SR_DATA = 10;

  // EEP_ eeprom control pins
  static const uint8_t EEP_OE = A4;
  static const uint8_t EEP_WE = A5;

  // DATA_ eeprom data pins - must be consecutive ascending pins
  static const uint8_t DATA_BIT0 = 2;
  static const uint8_t DATA_BIT7 = 9;

  int _endAddress = 0;
  int _chipAddressLines[2] = {
      13,
      15};

  EEPROMP(){};

  bool setAddressLines(int numAddressLines);
  void initPins() const;
  void latchAddress() const;
  void setAddress(uint16_t address) const;
  void setDataPinsTo(uint8_t mode) const;
  void readSetup() const;
  void readByteFromEeprom(byte &data) const;
  void pollTillWriteComplete(byte bit7Value) const;
};
