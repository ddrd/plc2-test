#include <Arduino.h>
#include "utils.h"

void testInputs()
{
  Label lblBack("[NEXT]", ILI9341_RED);
  lblBack.setPositionAndSize(111, 195, 100, 35); lblBack.draw(tft);
  
  Label lblTime("", ILI9341_WHITE, ILI9341_BLACK);
  lblTime.setPositionAndSize(10, 200, 100, 25);
  lblTime.draw(tft);
  
  Label lblDate("", ILI9341_WHITE, ILI9341_BLACK);
  lblDate.setPositionAndSize(218, 200, 100, 25);
  lblDate.draw(tft);
  
  Label lblAINtitle[] = {
    Label("a00:", ILI9341_WHITE, ILI9341_BLACK),
    Label("a01:", ILI9341_WHITE, ILI9341_BLACK),
    Label("a02:", ILI9341_WHITE, ILI9341_BLACK),
    Label("a03:", ILI9341_WHITE, ILI9341_BLACK),
    Label("a04:", ILI9341_WHITE, ILI9341_BLACK),
    Label("a05:", ILI9341_WHITE, ILI9341_BLACK),
    Label("a06:", ILI9341_WHITE, ILI9341_BLACK),
    Label("a07:", ILI9341_WHITE, ILI9341_BLACK),
  };
  Label lblAIN[] = {
    Label("---", ILI9341_WHITE, ILI9341_BLACK),
    Label("---", ILI9341_WHITE, ILI9341_BLACK),
    Label("---", ILI9341_WHITE, ILI9341_BLACK),
    Label("---", ILI9341_WHITE, ILI9341_BLACK),
    Label("---", ILI9341_WHITE, ILI9341_BLACK),
    Label("---", ILI9341_WHITE, ILI9341_BLACK),
    Label("---", ILI9341_WHITE, ILI9341_BLACK),
    Label("---", ILI9341_WHITE, ILI9341_BLACK),
  };
  Label lblAIN2title[] = {
    Label("a08:", ILI9341_WHITE, ILI9341_BLACK),
    Label("a09:", ILI9341_WHITE, ILI9341_BLACK),
    Label("a10:", ILI9341_WHITE, ILI9341_BLACK),
    Label("a11:", ILI9341_WHITE, ILI9341_BLACK),
    Label("a12:", ILI9341_WHITE, ILI9341_BLACK),
    Label("a13:", ILI9341_WHITE, ILI9341_BLACK),
    Label("a14:", ILI9341_WHITE, ILI9341_BLACK),
    Label("a15:", ILI9341_WHITE, ILI9341_BLACK),
  };
  Label lblAIN2[] = {
    Label("---", ILI9341_WHITE, ILI9341_BLACK),
    Label("---", ILI9341_WHITE, ILI9341_BLACK),
    Label("---", ILI9341_WHITE, ILI9341_BLACK),
    Label("---", ILI9341_WHITE, ILI9341_BLACK),
    Label("---", ILI9341_WHITE, ILI9341_BLACK),
    Label("---", ILI9341_WHITE, ILI9341_BLACK),
    Label("---", ILI9341_WHITE, ILI9341_BLACK),
    Label("---", ILI9341_WHITE, ILI9341_BLACK),
  };
  
  Label lblDINtitle[] = {
    Label("D30:", ILI9341_WHITE, ILI9341_BLACK),
    Label("D31:", ILI9341_WHITE, ILI9341_BLACK),
    Label("D32:", ILI9341_WHITE, ILI9341_BLACK),
    Label("D33:", ILI9341_WHITE, ILI9341_BLACK),
    Label("D34:", ILI9341_WHITE, ILI9341_BLACK),
    Label("D35:", ILI9341_WHITE, ILI9341_BLACK),
    Label("D36:", ILI9341_WHITE, ILI9341_BLACK),
    Label("D37:", ILI9341_WHITE, ILI9341_BLACK),
  };
  Label lblDIN[] = {
    Label("-", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
    Label("-", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
    Label("-", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
    Label("-", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
    Label("-", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
    Label("-", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
    Label("-", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
    Label("-", ILI9341_WHITE, ILI9341_BLACK, HORZ_CENTER|VERT_CENTER),
  };
  Sizable *allctls[] = {
    &lblAINtitle[0], &lblAIN[0], &lblAIN2title[0], &lblAIN2[0], &lblDINtitle[0], &lblDIN[0],
    &lblAINtitle[1], &lblAIN[1], &lblAIN2title[1], &lblAIN2[1], &lblDINtitle[1], &lblDIN[1],
    &lblAINtitle[2], &lblAIN[2], &lblAIN2title[2], &lblAIN2[2], &lblDINtitle[2], &lblDIN[2],
    &lblAINtitle[3], &lblAIN[3], &lblAIN2title[3], &lblAIN2[3], &lblDINtitle[3], &lblDIN[3],
    &lblAINtitle[4], &lblAIN[4], &lblAIN2title[4], &lblAIN2[4], &lblDINtitle[4], &lblDIN[4],
    &lblAINtitle[5], &lblAIN[5], &lblAIN2title[5], &lblAIN2[5], &lblDINtitle[5], &lblDIN[5],
    &lblAINtitle[6], &lblAIN[6], &lblAIN2title[6], &lblAIN2[6], &lblDINtitle[6], &lblDIN[6],
    &lblAINtitle[7], &lblAIN[7], &lblAIN2title[7], &lblAIN2[7], &lblDINtitle[7], &lblDIN[7],
    NULL
  };
  float cw[6] = { 1.0, 1.0, 1.0, 1.0, 1.0, 0.5 };
  GridLayout(allctls, 6, 0, 0, cw).setPositionAndSize(5, 45, 310, 140);
  
  for(int i=0; allctls[i]; ++i)
    ((Label *)allctls[i])->draw(tft);
  
  while(true)
  {
    if (touch.dataAvailable())
    {
      Point pt = touch.read();
      int x = pt.x;
      int y = pt.y;
      
      if (lblBack.isTouched(x, y))
        break;
    }
    
    // update inputs display
    for(int i=0; i<8; ++i)
    {
      lblDIN[i].updateText((digitalRead(30+i) == HIGH? "1" : "0"), tft);
      lblAIN[i].updateText(String(analogRead(A0+i)), tft);
      lblAIN2[i].updateText(String(analogRead(A8+i)), tft);
    }
  }
  tft.fillScreen(ILI9341_BLACK);  
}

