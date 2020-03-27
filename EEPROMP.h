#ifndef _EEPROMP_
#define _EEPROMP_

#if (ARDUINO >= 100 )
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class EEPROMP {
  public:
    static EEPROMP& getEeprom();
    bool setAddressLines(int numAddressLines);
    bool readByte(uint16_t address, byte* data) const;
    bool readArray(uint16_t startAddress, byte* data, int size) const;
    bool writeByte(uint16_t address, byte data) const;
    bool writeArray(uint16_t startAddress, byte* data, int size) const;
    bool printContents() const;
    EEPROMP(EEPROMP const&) = delete;
    void operator=(EEPROMP const&) = delete;

  private:
    // SR_ Shift Register pins
    static const uint8_t SR_CLOCK = A2;
    static const uint8_t SR_LATCH = A3;
    static const uint8_t SR_DATA = 10;

    // EEP_ eeprom control pins
    static const uint8_t EEP_OE = A4;
    static const uint8_t EEP_WE = A5;

    // DATA_ eeprom data pins
    static const uint8_t DATA_BIT0 = 2;
    static const uint8_t DATA_BIT7 = 9;

    int _endAddress = 0;
    EEPROMP(){};
    void latchAddress() const;
    void setAddress(uint16_t) const ;
    void setDataPinsTo(uint8_t) const;
};
#endif
