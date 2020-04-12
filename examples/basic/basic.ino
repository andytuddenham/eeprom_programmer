#include <EEPROM.hpp>

using ext_memory::EEPROM;

void setup()
{

  Serial.begin(57600);
  EEPROM eeprom();
}

void loop() {}
