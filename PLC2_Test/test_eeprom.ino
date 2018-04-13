#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <SD.h>
#include <PLDuino.h>
#include <PLDuinoGUI.h>
#include "utils.h"

const int EEPROM_I2C_ADDRESS = 0x52;
const int EEPROM_MAX_ADDRESS = 4096;

byte readEEPROM (int deviceaddress, unsigned int eeaddress);
void writeEEPROM (int deviceaddress, unsigned int eeaddress, byte data);


void testEEPROM()
{
  tft.fillScreen(ILI9341_BLACK);
  
  Label lblStatus("EEPROM TEST", ILI9341_WHITE, ILI9341_BLACK, VERT_CENTER|HORZ_CENTER);
  lblStatus.setPositionAndSize(0,5*RH, 26*CW, RH); lblStatus.draw(tft);
  
  Label lblNotice("TEST destroys all data!!", ILI9341_MAGENTA, ILI9341_BLACK, VERT_CENTER|HORZ_CENTER);
  lblNotice.setPositionAndSize(0,0, 26*CW, RH); lblNotice.draw(tft);
  
  Label lblSizeNotice("Size: " + String(EEPROM_MAX_ADDRESS) + " bytes", ILI9341_ORANGE, ILI9341_BLACK, VERT_CENTER|HORZ_CENTER);
  lblSizeNotice.setPositionAndSize(0,7*RH, 26*CW, RH); lblSizeNotice.draw(tft);
  
  Label lblTest("[TEST]", ILI9341_WHITE, ILI9341_BLACK, VERT_CENTER|HORZ_CENTER);
  lblTest.setPositionAndSize(2*CW,12*RH, 10*CW, RH); lblTest.draw(tft);
  
  Label lblNext("[NEXT]", ILI9341_WHITE, ILI9341_BLACK, VERT_CENTER|HORZ_CENTER);
  lblNext.setPositionAndSize(12*CW,12*RH, 10*CW, RH); lblNext.draw(tft);

  while(true)
  {
    if (touch.dataAvailable())
    {
      Point pt = touch.read();
      if (lblTest.isTouched(pt.x, pt.y))
      {
        // Remove Test and Next buttons from screen
        lblTest.erase(tft); lblNext.erase(tft);

        long valid_bytes = 0;
        for(unsigned int address=0; address<EEPROM_MAX_ADDRESS; ++address) {
          byte data = rand() % 0x100;
          writeEEPROM(address, data);
          byte written = readEEPROM(address);
          bool byte_valid = (data==written);
          
          valid_bytes += (byte_valid? 1 : 0);
          bool all_valid = (valid_bytes == address+1);
          
          Serial.println("#" + String((uint32_t)address, HEX) + ": " + (byte_valid? "valid" : "FAILED"));
          lblStatus.updateTextAndColor(
            String(valid_bytes) + " valid of " + String(EEPROM_MAX_ADDRESS),
            (all_valid? ILI9341_GREEN : ILI9341_RED),
            tft
          );
        }

        // Place the buttons back.
        lblTest.draw(tft); lblNext.draw(tft);
      }
      
      // Exit by touching Next button
      if (lblNext.isTouched(pt.x, pt.y))
        break;
    }
  }
}

void writeEEPROM (unsigned int eeaddress, byte data)
{
  Wire.beginTransmission(EEPROM_I2C_ADDRESS);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();
  delay(10);
}

byte readEEPROM (unsigned int eeaddress)
{
  byte rdata = 0xFF;
  Wire.beginTransmission(EEPROM_I2C_ADDRESS);
  Wire.write((int)(eeaddress >> 8));
  Wire.write((int)(eeaddress & 0xFF));
  Wire.endTransmission();
  Wire.requestFrom(EEPROM_I2C_ADDRESS, 1);
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}

