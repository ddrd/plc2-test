#include <Arduino.h>
#include "utils.h"

void testRS485Loopback()
{
  tft.fillScreen(ILI9341_BLACK);
  
  Label lblResult("Ser1 RS485 LOOPBACK", ILI9341_ORANGE, ILI9341_BLACK, VERT_CENTER|HORZ_CENTER);
  lblResult.setPositionAndSize(2*CW,5*RH, 22*CW, RH); lblResult.draw(tft);
  
  Label lblStart("[START]", ILI9341_GREEN, ILI9341_BLACK, VERT_CENTER|HORZ_CENTER);
  lblStart.setPositionAndSize(2*CW,10*RH, 10*CW, RH); lblStart.draw(tft);
  
  Label lblNext("[NEXT]", ILI9341_GREEN, ILI9341_BLACK, VERT_CENTER|HORZ_CENTER);
  lblNext.setPositionAndSize(12*CW,10*RH, 10*CW, RH); lblNext.draw(tft);

  pinMode(23, OUTPUT);
  
  while(true)
  {
    if (touch.dataAvailable())
    {
      Point pt = touch.read();
      if (lblStart.isTouched(pt.x, pt.y))
      {
        bool failed = false;
        Serial1.begin(28800);
        Serial1.flush();
        while(Serial1.available()) Serial1.read();
        for(long ch = 0x00; ch <= 0xff && !failed; ++ch)
        {
          lblResult.updateTextAndColor(
            "Testing pattern " + String(ch, HEX),
            ILI9341_WHITE,
            tft
          );
          
          digitalWrite(23, HIGH);
          Serial1.write((unsigned char)ch);
          delay(5);
          digitalWrite(23, LOW);
          
          long start_time = millis();
          while(!failed)
          {
            if (millis() - start_time > 3000)
              failed = true;
            
            if (Serial1.available())
            {
              unsigned char rc = Serial1.read();
              if (ch != rc)
              {
                lblResult.updateTextAndColor(String(rc, HEX) + " MUST BE " + String(ch, HEX), ILI9341_RED, tft);
                delay(2000);
                failed = true;
              }
              else
              {
                lblResult.updateText(String(rc, HEX) + " OK", tft);
                delay(50);
                break;
              }
            }
          }
        }
        
        lblResult.updateTextAndColor(
          (failed? "FAILED" : "PASSED"),
          (failed? ILI9341_RED : ILI9341_GREEN),
          tft
        );
        Serial1.end();
      }
      if (lblNext.isTouched(pt.x, pt.y))
        break;
    }
    delay(10);
  }
}

