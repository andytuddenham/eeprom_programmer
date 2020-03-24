#include "EEPROMP.h"

EEPROMP& EEPROMP::getEEPROMP() {
  static EEPROMP _eeprom;
  return _eeprom;
}

bool EEPROMP::setAddressLines(int numAddressLines) {
  if (_endAddress != 0) return false;
  _endAddress = pow(2, numAddressLines)-1;

  digitalWrite(SR_DATA, LOW);
  pinMode(SR_DATA, OUTPUT);

  digitalWrite(SR_CLOCK, LOW);
  pinMode(SR_CLOCK, OUTPUT);

  digitalWrite(SR_LATCH, LOW);
  pinMode(SR_LATCH, OUTPUT);

  digitalWrite(EEP_OE, HIGH);
  pinMode(EEP_OE, OUTPUT);

  digitalWrite(EEP_WE, HIGH);  // make sure WE is disabled before setting pin mode to OUTPUT
  pinMode(EEP_WE, OUTPUT);
}

void EEPROMP::latchAddress() const {
  digitalWrite(SR_LATCH, LOW);
  digitalWrite(SR_LATCH, HIGH);
  digitalWrite(SR_LATCH, LOW);
}

void EEPROMP::setAddress(uint16_t address) const {
  shiftOut(SR_DATA, SR_CLOCK, LSBFIRST, (uint8_t) address);
  shiftOut(SR_DATA, SR_CLOCK, LSBFIRST, (uint8_t)(address >> 8));
  latchAddress();
}

void EEPROMP::setDataPinsTo(uint8_t mode) const {
  for (uint8_t pin = DATA_BIT0; pin <= DATA_BIT7; pin++) {
    pinMode(pin, mode);
  }
}

byte EEPROMP::readByte(uint16_t address) const {
  setDataPinsTo(INPUT);
  setAddress(address);
  digitalWrite(EEP_OE, LOW);

  byte data = 0;
  for (uint8_t pin = DATA_BIT7, i=0; pin >= DATA_BIT0; pin--, i++) {
    data |= digitalRead(pin) >> i;
  }

  digitalWrite(EEP_OE, HIGH);
  return data;
}

void EEPROMP::writeByte(uint16_t address, byte data) const {
  digitalWrite(EEP_OE, HIGH);
  setDataPinsTo(OUTPUT);
  setAddress(address);

  for (uint8_t pin = DATA_BIT0, i=0; pin <= DATA_BIT7; pin++, i++) {
    digitalWrite(pin, data & i);
  }

  digitalWrite(EEP_WE, LOW);
  delayMicroseconds(1);
  digitalWrite(EEP_WE, HIGH);
  //ToDo wait for the write cycle to finish by polling I/O7
  delay(10); // replace this!
}

void EEPROMP::printContents() const {
  for (uint16_t base = 0; base <= _endAddress; base +=16) {
    byte data[16] = {0};
    for (uint16_t offset = 0; offset <= 15; offset++) {
      data[offset] = readByte(base + offset);
    }
    char buf[56];
    sprintf(buf, "%04x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3],
            data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11],
            data[12], data[13], data[14], data[15]);
    Serial.println(buf);
  }
}

