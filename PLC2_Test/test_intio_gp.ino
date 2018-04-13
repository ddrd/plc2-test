#include <Arduino.h>
#include "utils.h"

void testINTIOGPPWMs()
{
  Label lblNext("[NEXT]", ILI9341_GREEN);
  lblNext.setPositionAndSize(111, 195, 100, 35); lblNext.draw(tft);
  
  Label lblTitle("INTIO,GP,PWM pins test", ILI9341_WHITE, ILI9341_BLACK, VERT_CENTER|HORZ_CENTER);
  lblTitle.setPositionAndSize(0,0,320,40); lblTitle.draw(tft);
  
  Label lblHelp1("Pins are changing state", ILI9341_YELLOW, ILI9341_BLACK, VERT_CENTER|HORZ_CENTER);
  lblHelp1.setPositionAndSize(0,40,320,60); lblHelp1.draw(tft);
  Label lblHelp2("with 1 sec interval", ILI9341_YELLOW, ILI9341_BLACK, VERT_CENTER|HORZ_CENTER);
  lblHelp2.setPositionAndSize(0,60,320,60); lblHelp2.draw(tft);

  uint8_t pins[] = {
    
    // INTIOx
    70, 71, 72, 73,
    // GPx
    22, 83, 84, 85, 80, 78, 79,
    // PWMs
    12, 11, 10,
    0
  };

  for(int i=0; pins[i]; ++i)
    pinMode(pins[i], OUTPUT);
  
  long lasttime = millis();
  while(true)
  {
    if (touch.dataAvailable())
    {
      Point pt = touch.read();
      int x = pt.x;
      int y = pt.y;
      
      if (lblNext.isTouched(x, y))
        break;

      while(touch.dataAvailable()) touch.read();
    }

    if (millis() - lasttime >= 1000) {
      Serial.println("Pins switched");
      for(int i=0; pins[i]; ++i)
        digitalWrite(pins[i], digitalRead(pins[i])==HIGH? LOW : HIGH);
      lasttime = millis();
    }
    
    delay(50);
  }
  tft.fillScreen(ILI9341_BLACK);
}

