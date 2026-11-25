

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
    tft.setFont(&FreeSans18pt7b);
  } else 
  {
    tft.setFont(&FreeSans9pt7b);
  }
  tft.setTextDatum(CL_DATUM); 
  tft.drawString(label, x, y);
}

void drawTextBox(int x, int y, int w, int h, const char* text, bool cent, bool size) 
{

  int xoff;
  tft.fillRoundRect(x, y, w, h, h/4, TFT_BLUE);
  tft.drawRoundRect(x, y, w, h, h/4, TFT_WHITE);
  tft.setTextColor(TFT_WHITE);
  if (size) 
  {
    tft.setFont(&FreeSans18pt7b);
  } else 
  {
    tft.setFont(&FreeSans9pt7b);
  }
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

//Wrapper around tft.getTouch() that reports "not touched" while touchConsumed is
//true, only reporting real touches again once a genuine release has been seen.
//Use this everywhere a screen or popup polls for touches, instead of calling
//tft.getTouch() directly - see the note on touchConsumed for why.
bool getTouchDebounced(uint16_t *x, uint16_t *y)
{
  bool pressed = tft.getTouch(x, y);
  if(!pressed)
  {
    touchConsumed = false;
    return false;
  }
  if(touchConsumed)
  {
    return false;
  }
  return true;
}

#define TOUCH_CAL_MAGIC 0x56      //marks valid LovyanGFX touch calibration data at EEPROM 4014-4029

void touch_calibrate(bool force)
{
  //LovyanGFX uses 8 calibration values (TFT_eSPI used 5), so the EEPROM
  //marker was changed from 0x55 to 0x56. Any old TFT_eSPI calibration is
  //ignored and a new calibration is run once on first boot.
  uint16_t calData[8];
  uint8_t calDataOK = 0;

  // check if calibration exists
  if (EEPROM.read(4095) == TOUCH_CAL_MAGIC) 
    {   
      EEPROM.get(4014,calData);
      calDataOK = 1;
    }

  if (calDataOK && !REPEAT_CAL && !force)
  {
    // calibration data valid
    tft.setTouchCalibrate(calData);
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

    tft.setTouchThreshold(TOUCH_Z_CALIBRATE);   // corners need a lower pressure threshold (as TFT_eSPI)
    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);
    tft.setTouchThreshold(TOUCH_Z_THRESHOLD);   // back to normal for everyday use

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");

    // store data at the top of the EEPROM
    EEPROM.put(4014,calData);
    EEPROM.write(4095,TOUCH_CAL_MAGIC);
    EEPROM.commit();
  }

}
