#include "EEPROMP.h"

EEPROMP &EEPROMP::getEeprom()
{
  static EEPROMP eepromp;
  return eepromp;
}

bool EEPROMP::setChip(const Chip chip)
{
  bool success = setAddressLines(_chipAddressLines[static_cast<int>(chip)]);
  if (success)
    initPins();
  return success;
}

bool EEPROMP::setAddressLines(int numAddressLines)
{
  if (_endAddress != 0)
    return false;

  _endAddress = pow(2, numAddressLines) - 1;
  return true;
}

void EEPROMP::initPins() const
{
  digitalWrite(SR_DATA, LOW);
  pinMode(SR_DATA, OUTPUT);

  digitalWrite(SR_CLOCK, LOW);
  pinMode(SR_CLOCK, OUTPUT);

  digitalWrite(SR_LATCH, LOW);
  pinMode(SR_LATCH, OUTPUT);

  digitalWrite(EEP_OE, HIGH);
  pinMode(EEP_OE, OUTPUT);

  digitalWrite(EEP_WE, HIGH); // make sure WE is disabled before setting pin mode to OUTPUT
  pinMode(EEP_WE, OUTPUT);
}

void EEPROMP::latchAddress() const
{
  digitalWrite(SR_LATCH, LOW);
  digitalWrite(SR_LATCH, HIGH);
  digitalWrite(SR_LATCH, LOW);
}

void EEPROMP::setAddress(uint16_t address) const
{
  shiftOut(SR_DATA, SR_CLOCK, LSBFIRST, static_cast<uint8_t>(address));
  shiftOut(SR_DATA, SR_CLOCK, LSBFIRST, static_cast<uint8_t>(address >> 8));
  latchAddress();
}

void EEPROMP::setDataPinsTo(uint8_t mode) const
{
  for (uint8_t pin = DATA_BIT0; pin <= DATA_BIT7; pin++)
    pinMode(pin, mode);
}

bool EEPROMP::readByte(uint16_t address, byte &data) const
{
  if (_endAddress == 0)
    return false;

  // prepare to read
  setDataPinsTo(INPUT);
  setAddress(address);
  digitalWrite(EEP_OE, LOW);
  delayMicroseconds(1);

  // read the byte value one bit at a time
  data = 0x00;
  for (uint8_t pin = DATA_BIT0; pin <= DATA_BIT7; pin++)
  {
    data |= digitalRead(pin) << (pin - DATA_BIT0);
  }

  digitalWrite(EEP_OE, HIGH);
  return true;
}

bool EEPROMP::readArray(uint16_t startAddress, byte *data, int size) const
{
  if (_endAddress == 0)
    return false;

  for (int offset = 0; offset < size; offset++)
  {
    if (!readByte(startAddress + offset, data[offset]))
      return false;
  }

  return true;
}

void EEPROMP::pollTillWriteComplete(byte bit7Value) const
{
  // change I/O7 to input and turn output enable on
  pinMode(DATA_BIT7, INPUT);
  digitalWrite(EEP_OE, LOW);

  // reading I/O7 will return the opposite of what was written
  // until the write is complete
  while (digitalRead(DATA_BIT7) != bit7Value)
  {
    delayMicroseconds(1);
  }
  delayMicroseconds(1);
}

bool EEPROMP::writeByte(uint16_t address, byte data) const
{
  if (_endAddress == 0)
    return false;

  // prepare to write
  digitalWrite(EEP_OE, HIGH);
  setDataPinsTo(OUTPUT);
  setAddress(address);

  // send the byte value one bit at a time
  for (uint8_t pin = DATA_BIT0; pin <= DATA_BIT7; pin++)
  {
    digitalWrite(pin, (data >> pin - DATA_BIT0) & 0x01);
  }

  // start the write cycle
  digitalWrite(EEP_WE, LOW);
  delayMicroseconds(1);
  digitalWrite(EEP_WE, HIGH);

  // wait for the write cycle to finish
  byte bit7Value = data >> 7;
  pollTillWriteComplete(bit7Value);
  return true;
}

bool EEPROMP::writeArray(uint16_t startAddress, byte *data, int size) const
{
  if (_endAddress == 0)
    return false;

  for (int offset = 0; offset < size; offset++)
  {
    if (!writeByte(startAddress + offset, data[offset]))
      return false;
  }
  return true;
}

bool EEPROMP::printContents(void (*callback)(uint16_t address, byte *data, size_t dataSize)) const
{
  if (_endAddress == 0)
    return false;

  const size_t dataSize = 16;

  for (uint16_t baseAddress = 0; baseAddress <= _endAddress - dataSize; baseAddress += dataSize)
  {
    byte data[dataSize] = {0};
    if (!readArray(baseAddress, data, dataSize))
      return false;

    callback(baseAddress, data, dataSize);
  }

  return true;
}
