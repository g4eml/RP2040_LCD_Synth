

void drawNumBox(int x, int y, int w, int h, double value, int decplaces, bool cent) 
{
  char st[20];
  int xoff;

  tft.fillRoundRect(x, y, w, h, h/4, TFT_BLUE);
  tft.drawRoundRect(x, y, w, h, h/4, TFT_WHITE);
  tft.setTextColor(TFT_WHITE);
  if(cent)
  {
    tft.setTextDatum(CC_DATUM);
    xoff = x+w/2;
  }
  else
  {
    tft.setTextDatum(CL_DATUM);
    xoff = x+h/4; 
  }
  sprintf(st, "%.*f", decplaces, value);
  tft.drawString(st, xoff, y + h / 2);
}


void drawLabel(int x, int y, const char* label, int colour, bool size)
{
  tft.setTextColor(colour);
  if (size) 
  {
    tft.setFreeFont(&FreeSans18pt7b);
  } else 
  {
    tft.setFreeFont(&FreeSans9pt7b);
  }
  tft.setTextDatum(CL_DATUM); 
  tft.drawString(label, x, y);
}

void drawTextBox(int x, int y, int w, int h, const char* text, bool cent) 
{

  int xoff;
  tft.fillRoundRect(x, y, w, h, h/4, TFT_BLUE);
  tft.drawRoundRect(x, y, w, h, h/4, TFT_WHITE);
  tft.setTextColor(TFT_WHITE);
  if(cent)
  {
    tft.setTextDatum(CC_DATUM);
    xoff = x+w/2;
  }
  else
  {
    tft.setTextDatum(CL_DATUM);
    xoff = x+h/4;
  }
  tft.drawString(text, xoff, y + h / 2);
}

void drawOnOff(int x, int y, int w, int h, bool on) 
{
  int col;

  if (on) 
  {
    col = TFT_GREEN;
  } 
  else 
  {
    col = TFT_RED;
  }
  tft.setTextColor(TFT_BLUE);
  tft.fillRoundRect(x, y, w, h, h/4, col);
  tft.drawRoundRect(x, y, w, h, h/4, TFT_WHITE);
}

bool touchZone(int x, int y, int w, int h) 
{
  return ((t_x > x) && (t_x < x + w) && (t_y > y) && (t_y < y + h));
}

void touch_calibrate(bool force)
{
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // check if calibration exists
  if (EEPROM.read(4095) == 0x55) 
    {   
      EEPROM.get(4014,calData);
      calDataOK = 1;
    }

  if (calDataOK && !REPEAT_CAL && !force)
  {
    // calibration data valid
    tft.setTouch(calData);
  } 
  else 
  {
    // data not valid so recalibrate
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.println("Touch corners as indicated");

    tft.setTextFont(1);
    tft.println();

    if (REPEAT_CAL) 
    {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println("Set REPEAT_CAL to false to stop this running again!");
    }

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");

    // store data at the top of the EEPROM
    EEPROM.put(4014,calData);
    EEPROM.write(4095,0x55);
    EEPROM.commit();
  }

}
