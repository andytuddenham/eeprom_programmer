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

  // for (byte b: eeprom)
  // {
  //   Serial.println(b, HEX);
  // }

  uint16_t addr = 0xef;
  byte new_data[] = {0x0f, 0x1e, 0x2d, 0x3c, 0xf0, 0xe1, 0xd2, 0xc3};
  eeprom.set_array(addr, new_data);

  byte read_data[16];
  eeprom.get_array(addr, read_data);
  printData(addr, read_data, sizeof(read_data));

  addr = 0;
  // printData(addr, &eeprom.get<byte>(addr), 1);
  byte read_data2[64];
  eeprom.get_array(addr, read_data2);
  printData(addr, read_data2, sizeof(read_data2));

  eeprom.get_content_in_chunks(64, printData);

  Serial.println("Done");
}

void printData(uint16_t address, byte* data, size_t dataSize)
{
  //  %04x: %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x
  size_t buffSize = 5 /*"%04x:"*/ + ceil(static_cast<float>(dataSize) / 4) /*" "*/
                    + (2 * dataSize) /*"%02x"*/ + 1 /*null string termination*/;
  char buff[buffSize];

  sprintf(buff, "%04x:", address);
  for (size_t i = 0; i < dataSize; i++)
  {
    if (i % 4 == 0)
      strcat(buff, " ");

    char* nextPtr = buff + strlen(buff);
    sprintf(nextPtr, "%02x", data[i]);
  }

  Serial.println(buff);
}

void loop() {}
