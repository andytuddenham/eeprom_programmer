#include "EEPROMP.h"

EEPROMP &EEPROMP::getEeprom()
{
  static EEPROMP _eeprom;
  return _eeprom;
}

bool EEPROMP::setChip(Chip chip)
{
  for (int i = 0; i < sizeof(_chipAddressLines) / sizeof(_chipAddressLines[0]); i++)
  {
    if (_chipAddressLines[i][0] == chip)
    {
      setAddressLines(_chipAddressLines[chip][1]);
      return true;
    }
  }
  return false;
}

bool EEPROMP::setAddressLines(byte numAddressLines)
{
  if (_endAddress != 0)
    return false;
  _endAddress = pow(2, (float)numAddressLines) - 1;

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
  shiftOut(SR_DATA, SR_CLOCK, LSBFIRST, (uint8_t)address);
  shiftOut(SR_DATA, SR_CLOCK, LSBFIRST, (uint8_t)(address >> 8));
  latchAddress();
}

void EEPROMP::setDataPinsTo(uint8_t mode) const
{
  for (uint8_t pin = DATA_BIT0; pin <= DATA_BIT7; pin++)
  {
    pinMode(pin, mode);
  }
}

bool EEPROMP::readByte(uint16_t address, byte *data) const
{
  if (_endAddress == 0)
    return false;

  // prepare to read
  setDataPinsTo(INPUT);
  setAddress(address);
  digitalWrite(EEP_OE, LOW);
  delayMicroseconds(1);

  // read the byte value one bit at a time
  *data = 0x00;
  for (uint8_t pin = DATA_BIT7; pin >= DATA_BIT0; pin--)
  {
    *data |= digitalRead(pin) << pin - DATA_BIT0;
  }

  digitalWrite(EEP_OE, HIGH);
  return true;
}

bool EEPROMP::readArray(uint16_t startAddress, byte *data, int size) const
{
  if (_endAddress == 0)
    return false;
  uint16_t address = startAddress;
  for (int offset = 0; offset < size; offset++)
  {
    if (!readByte(address + offset, &data[offset]))
    {
      return false;
    }
  }
  return true;
}

void EEPROMP::pollTillWriteComplete(byte bit7Value) const
{
  // change I/O7 to input and turn output enable on
  pinMode(DATA_BIT7, INPUT);
  digitalWrite(EEP_OE, LOW);
  delayMicroseconds(1);

  // reading I/O7 will return the opposite of what was written
  // until the write is complete
  while (bit7Value != digitalRead(DATA_BIT7))
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
  byte bit7Value;
  for (uint8_t pin = DATA_BIT0; pin <= DATA_BIT7; pin++)
  {
    digitalWrite(pin, data & 1);
    if (pin == DATA_BIT7)
    {
      bit7Value = data & 1;
    }
    data = data >> 1;
  }

  // start the write cycle
  digitalWrite(EEP_WE, LOW);
  delayMicroseconds(1);
  digitalWrite(EEP_WE, HIGH);

  // wait for the write cycle to finish
  pollTillWriteComplete(bit7Value);
  return true;
}

bool EEPROMP::writeArray(uint16_t address, byte *data, int size) const
{
  if (_endAddress == 0)
    return false;
  for (int offset = 0; offset < size; offset++)
  {
    if (!writeByte(address + offset, data[offset]))
    {
      return false;
    }
  }
  return true;
}

bool EEPROMP::printContents(void (*callback)(uint16_t, byte *, int)) const
{
  if (_endAddress == 0)
    return false;

  for (uint16_t base = 0; base <= _endAddress; base += 16)
  {
    byte data[16] = {0};
    if (!readArray(base, data, sizeof(data)))
      return false;
    (*callback)(base, data, sizeof(data));
  }

  return true;
}
