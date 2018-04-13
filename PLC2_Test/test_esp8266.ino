#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <PLDuino.h>
#include <DS3232RTC.h>
#include <Time.h>
#include <Wire.h>
#include <SimpleCommandProcessor.h>
#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>
#include "utils.h"

using namespace PLDuino;
HardwareSerial &wifi = Serial2;
BridgeServer server;


String getState()
{
  String response;
  
  // an equivalent to (sstream << setw(width) << setfill(fill) << value).str()
  auto fmt = [](int value, int width, char fill='0') {
    String r = String(value);
    while(r.length() < width)
      r = fill + r;
    return r;
  };

  for(int i=0; i<6; ++i)
    response += String(digitalRead(RELAY1+i)==HIGH? "1" : "0");
  for(int i=0; i<8; ++i)
    response += String(digitalRead(30+i)==HIGH? "1" : "0");
  for(int i=0; i<8; ++i)
    response += String(digitalRead(38+i)==HIGH? "1" : "0");
  for(int i=0; i<8; ++i)
    response += String(fmt(analogRead(A0+i),4).c_str());
  response += String((
      fmt(hour(),2) +
      fmt(minute(), 2)
    ).c_str());
  response += String((
      fmt(year(), 4) +
      fmt(month(), 2) +
      fmt(day(), 2)
    ).c_str());
  response += ("e");
  return response;
}

bool cmd_adjustTime (const String &t)
{
  if (t.length() == String("19990101T1010").length() && t[8]=='T')
  {
    tmElements_t tm; {
      int y = t.substring(0, 4).toInt();
      tm.Year = (y>=1000? CalendarYrToTm(y) : y2kYearToTm(y));
      tm.Month = t.substring(4, 6).toInt();
      tm.Day = t.substring(6, 8).toInt();
      tm.Hour = t.substring(9, 11).toInt();
      tm.Minute = t.substring(11, 13).toInt();
      tm.Second = 0;
    }
    time_t t = makeTime(tm);
    RTC.set(t); setTime(t);
    Serial.println("\n\nTime set!\n\n");
    return true;
  }
  else
  {
    Serial.println("\n\nCan't set time!\n\n");
    return false;
  }
}

bool cmd_processSwitchCommand (const String &cmd)
{
  char op = cmd[0];
  int index = cmd[1] - '1';
  if (index < 0) return false;
  switch(op)
  {
    case 'r':
    case 'R':
      if (index >= 6) { return false; }
      Serial.println("relay" + String(index) + (op=='R'? "ON" : "off"));
      digitalWrite(RELAY1+index, (op=='R'? HIGH : LOW));
      return true;
      break;
    case 'o':
    case 'O':
      if (index >= 8) { return false; }
      Serial.println("dout" + String(index) + (op=='O'? "ON" : "off"));
      digitalWrite(38+index, (op=='O'? HIGH : LOW));
      return true;
      break;
  }
  return false;
}

#define R_OK "OK"
#define R_FAIL "FAIL"

void testWiFi()
{
  // Setup screen
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);
  tft.println("Wi-Fi test.");
  tft.setTextSize(1);
  tft.println();

  // Print Wi-Fi credentials for convenience
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_GREEN);  tft.println("Default settings:");
  tft.setTextColor(ILI9341_WHITE);  tft.print("1.Connect to \"");
  tft.setTextColor(ILI9341_YELLOW); tft.print("Wi-Fi Test");
  tft.setTextColor(ILI9341_WHITE);  tft.println("\"");
  tft.setTextColor(ILI9341_WHITE);  tft.print("2.Use password \"");
  tft.setTextColor(ILI9341_YELLOW); tft.print("password");
  tft.setTextColor(ILI9341_WHITE);  tft.println("\"");
  tft.setTextColor(ILI9341_WHITE);  tft.print("3.Open URL: ");
  tft.setTextColor(ILI9341_YELLOW); tft.println("192.168.4.1");
  
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.println();

  // power-on Atheros
  pinMode(ESP_PWR, OUTPUT);
  digitalWrite(ESP_PWR, HIGH);
  wifi.begin(250000);

  tft.println("Initializing Atheros... please wait");
  Serial.println("Starting Bridge...");
  Bridge.begin();
  Serial.println("Bridge started.");
  Serial.println("Starting server...");
  server.begin();
  Serial.println("Server started.");
  tft.println("Atheros initialization complete.");

  Process p;
  p.runShellCommandAsynchronously("/usr/bin/python /opt/configurator/main.py");

  while(true)
  {
    BridgeClient client = server.accept();
    if (client)
    {
      String cmd;
      while(true)
      {
        for(int i=0; i<200/20; ++i)
          if (!client.available()) delay(20);
        if (!client.available()) break;
        cmd += char(client.read());
      }
      Serial.println("command: \"" + cmd + "\"");
      
      if (cmd == "state")
      {
        client.print(getState());
      }
      else if (cmd.substring(0, 4) == "time")
      {
        client.print(
          cmd_adjustTime(cmd.substring(4))? R_OK : R_FAIL
        );
      }
      else if (cmd.length() == 2)
      {
        tft.print(cmd);
        cmd_processSwitchCommand(cmd);
        client.print(R_OK);
      }
      else
      {
        Serial.println("ERROR");
        client.print(R_FAIL);
      }
      client.stop();
    }
  }
}

