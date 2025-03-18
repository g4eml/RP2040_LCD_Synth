
//Frequency 
#define FREQ_LX 10
#define FREQ_LY 30
#define FREQ_X 135
#define FREQ_Y 5
#define FREQ_W 240
#define FREQ_H 50
#define MHx 395
#define MHy 30

//Mult 
#define M_LX 95
#define M_LY 35

//Power
#define POWER_LX 10
#define POWER_LY 95
#define POWER_X 120
#define POWER_Y 70
#define POWER_W 60
#define POWER_H 50


//Output On/Off Button
#define OUT_LX 250
#define OUT_LY 95
#define OUT_X 370
#define OUT_Y 70
#define OUT_W 50
#define OUT_H 50

//Channel
#define CHANNEL_LX 10
#define CHANNEL_LY 160
#define CHANNEL_X 150
#define CHANNEL_Y 135
#define CHANNEL_W 50
#define CHANNEL_H 50


//Configure Button
#define CONFIG_X 10
#define CONFIG_Y 290
#define CONFIG_W 70
#define CONFIG_H 30

//Sync time  Button
#define SYNC_X 280
#define SYNC_Y 290
#define SYNC_W 100
#define SYNC_H 30

//Save Button

#define SAVE_X 400
#define SAVE_Y 290
#define SAVE_W 70
#define SAVE_H 30


 void homeScreenUpdate(void)
 {
  tft.fillScreen(TFT_CYAN);
  // Draw numeric Boxes
  drawLabel(FREQ_LX, FREQ_LY, "Freq.", TFT_BLUE,1);
  drawNumBox(FREQ_X, FREQ_Y, FREQ_W, FREQ_H, chipGetFrequency() * chanData[channel].extMult , 6,true);
  drawLabel(MHx,MHy,"MHz",TFT_BLUE,1);

  if(chanData[channel].extMult > 1)
  {
  char ms[8];
  sprintf(ms,"(x%d)",chanData[channel].extMult);
  drawLabel(M_LX, M_LY, ms, TFT_BLUE,0); 
  }
  drawLabel(POWER_LX, POWER_LY, "Power", TFT_BLUE, 1);
  drawNumBox(POWER_X, POWER_Y, POWER_W, POWER_H, chipGetPower() , 0, true);
  drawLabel(OUT_LX, OUT_LY, "Output", TFT_BLUE,1);  
  drawOnOff(OUT_X, OUT_Y, OUT_W, OUT_H, chipGetOutput());
  drawLabel(CHANNEL_LX, CHANNEL_LY, "Channel", TFT_BLUE, 1);
  drawNumBox(CHANNEL_X, CHANNEL_Y, CHANNEL_W, CHANNEL_H, channel , 0, true);
  drawTextBox(CONFIG_X, CONFIG_Y, CONFIG_W, CONFIG_H, "Config" , true, 0);


  if(!gpsActive && ((chanData[channel].fskMode & CWIDBIT) || (chanData[channel].jtMode >0)))
  {
  drawTextBox(SYNC_X, SYNC_Y, SYNC_W, SYNC_H, "Sync Time" , true, 0);
  }

  if(saveRequired)
  {
    drawTextBox(SAVE_X, SAVE_Y, SAVE_W, SAVE_H, "Save" , true, 0);
  }

 }

 bool homeScreenTouched(void)
 {
  uint16_t raw = tft.getTouchRawZ();
  if(raw > 1000)
  {
    bool pressed =tft.getTouch(&t_x, &t_y);
    return pressed;
  }
  else
  {
    return false;
  }
    
 }

 bool processHomeScreenTouch(void)
 {
  double ret = 0;
  if(touchZone(FREQ_X, FREQ_Y, FREQ_W, FREQ_H))
  {
  ret = getNumber("Enter Frequency" , 12);
  chipSetFrequency(ret / chanData[channel].extMult);
  chipUpdate();
  saveRequired = true;
  return true;
  }

  if(touchZone(POWER_X, POWER_Y, POWER_W, POWER_H))
  {
  ret = getNumber("Enter Power",3);
  chipSetPower(ret);
  saveRequired = true;
  return true;
  }

  if(touchZone(OUT_X, OUT_Y, OUT_W, OUT_H))
  {
  chipEnableOutput(!chipGetOutput());
  saveRequired = true;
  return true;
  }

  if(touchZone(CHANNEL_X, CHANNEL_Y, CHANNEL_W, CHANNEL_H))
  {
  channel = doMemPad();
  selChan=channel;
  seconds = -1;                       //reset the timing after using the menu.
  milliseconds = 0;
  initChannel();
  chipUpdate();
  saveRequired = true;
  return true;
  }

  if(touchZone(CONFIG_X, CONFIG_Y, CONFIG_W, CONFIG_H))
  {
  doConfigScreen();
  return true;
  }

  if(touchZone(SYNC_X, SYNC_Y, SYNC_W, SYNC_H))
  {
  seconds = -1;
  milliseconds = 0;
  if(chanData[channel].fskMode & CWIDBIT) cwidInit();
  if(chanData[channel].jtMode >0) jtInit();
  return true;
  }

  if(touchZone(SAVE_X, SAVE_Y, SAVE_W, SAVE_H))
  {
  saveSettings();
  saveRequired = false;
  return true;
  }

  return false;
 }

