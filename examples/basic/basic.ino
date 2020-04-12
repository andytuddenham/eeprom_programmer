#include <ext_memory.hpp>

using ext_memory::Config;
using ext_memory::EEPROM;

void setup()
{

  Serial.begin(57600);
  Serial.println("Setup EEPROM");
  size_t numAddressLines = 13;
  size_t size = static_cast<size_t>(pow(2, numAddressLines)) - 1;
  const Config c = {size, A2, A3, 10, A4, A5, 2, 9};
  EEPROM eeprom(c);

  eeprom[1] = 0xff;

  Serial.print("EEPROM (1): ");
  Serial.println(static_cast<int>(eeprom[1]), HEX);

  Serial.println("Done");
}

void loop() {}
