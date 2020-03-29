#include <EEPROMP.h>

void setup()
{

    Serial.begin(57600);
    EEPROMP &eeprom = EEPROMP::getEeprom();
    if (eeprom.setChip(EEPROMP::Chip::AT28C64))
    {
        uint16_t addr = 0x00;
        byte data[16] = {0};
        eeprom.readArray(addr, data, sizeof(data));
        // char buf[43];
        // sprintf(buf, "%04x: %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x",
        //         addr, data[0], data[1], data[2], data[3],
        //         data[4], data[5], data[6], data[7],
        //         data[8], data[9], data[10], data[11],
        //         data[12], data[13], data[14], data[15]);
        // Serial.println(buf);
        printData(addr, data, 16);

        byte newData[] = {0x0f, 0x1e, 0x2d, 0x3c, 0xf0, 0xe1, 0xd2, 0xc3};
        eeprom.writeArray(addr, newData, sizeof(newData));
        Serial.println("Reading EEPROM");
        eeprom.printContents(printData);
        Serial.println("Done.");
    }
}

void printData(uint16_t address, byte *data, size_t dataSize)
{
    //  %04x: %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x
    size_t buffSize = 5 /*"%04x:"*/ + (2 * dataSize) /*"%02x"*/ + (dataSize / 4) /*" "*/ + 1 /*null string termination*/;
    char buff[buffSize];

    sprintf(buff, "%04x:", address);
    for (int i = 0; i < dataSize; i++)
    {
        if (i % 4 == 0)
            strcat(buff, " ");

        char *nextPtr = buff + strlen(buff);
        sprintf(nextPtr, "%02x", data[i]);
    }

    Serial.println(buff);
}

void loop()
{
    // put your main code here, to run repeatedly:
}