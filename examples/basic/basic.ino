#include <ext_memory.hpp>

using ext_memory::EEPROM;

void setup()
{

  Serial.begin(57600);
  Serial.println("Setup EEPROM");
  EEPROM eeprom(ext_memory::suggested_AT28C64_config);

  eeprom[1] = 0xff;

  Serial.print("EEPROM (1): ");
  Serial.println(static_cast<int>(eeprom[1]), HEX);

  Serial.println("Done");
}

void loop() {}
