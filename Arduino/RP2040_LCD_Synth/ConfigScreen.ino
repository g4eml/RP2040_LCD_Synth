//ADF4351
#define ADF_LX 130
#define ADF_LY 15
#define ADF_X 210
#define ADF_Y 5
#define ADF_W 20
#define ADF_H 20

//MAX2870
#define MAX_LX 10
#define MAX_LY 15
#define MAX_X 90
#define MAX_Y 5
#define MAX_W 20
#define MAX_H 20

//LMX2595
#define LMX_LX 250
#define LMX_LY 15
#define LMX_X 330
#define LMX_Y 5
#define LMX_W 20
#define LMX_H 20


//Reference Frequency
#define REF_LX 10
#define REF_LY 50
#define REF_X 180
#define REF_Y 40
#define REF_W 110
#define REF_H 20

//PFD
#define PFD_LX 320
#define PFD_LY 50
#define PFD_X 370
#define PFD_Y 40
#define PFD_W 105
#define PFD_H 20

//MULT
#define MULT_LX 10
#define MULT_LY 80
#define MULT_X 180
#define MULT_Y 70
#define MULT_W 40
#define MULT_H 20

//Key Shift
#define KEYSH_LX 10
#define KEYSH_LY 110
#define KEYSH_X 200
#define KEYSH_Y 100
#define KEYSH_W 100
#define KEYSH_H 20

//CWID
#define CWID_LX 10
#define CWID_LY 140
#define CWID_X 100
#define CWID_Y 130
#define CWID_W 375
#define CWID_H 20

//CWSP
#define CWSP_LX 10
#define CWSP_LY 170
#define CWSP_X 170
#define CWSP_Y 160
#define CWSP_W 50
#define CWSP_H 20

//CWINT
#define CWINT_LX 240
#define CWINT_LY 170
#define CWINT_X 405
#define CWINT_Y 160
#define CWINT_W 50
#define CWINT_H 20


//CW Shift
#define CWSH_LX 10
#define CWSH_LY 200
#define CWSH_X 170
#define CWSH_Y 190
#define CWSH_W 100
#define CWSH_H 20


//JTmode 0
#define JTM0_LX 10
#define JTM0_LY 230
#define JTM0_X 120
#define JTM0_Y 220
#define JTM0_W 20
#define JTM0_H 20

//JTmode 1
#define JTM1_LX 160
#define JTM1_LY 230
#define JTM1_X 215
#define JTM1_Y 220
#define JTM1_W 20
#define JTM1_H 20

//JTmode 2
#define JTM2_LX 250
#define JTM2_LY 230
#define JTM2_X 310
#define JTM2_Y 220
#define JTM2_W 20
#define JTM2_H 20

//JTmode 3
#define JTM3_LX 350
#define JTM3_LY 230
#define JTM3_X 410
#define JTM3_Y 220
#define JTM3_W 20
#define JTM3_H 20

//JTT1
#define JTT1_LX 10
#define JTT1_LY 260
#define JTT1_X 200
#define JTT1_Y 250
#define JTT1_W 100
#define JTT1_H 20

//JTID
#define JTID_LX 10
#define JTID_LY 290
#define JTID_X 90
#define JTID_Y 280
#define JTID_W 200
#define JTID_H 20

//exit Button
#define EXIT_LX 10
#define EXIT_LY 310
#define EXIT_X 50
#define EXIT_Y 300
#define EXIT_W 20
#define EXIT_H 20


void configScreenUpdate(void) 
{
  tft.fillScreen(TFT_CYAN);
  drawLabel(MAX_LX, MAX_LY, "MAX2870", TFT_BLUE,0);
  drawOnOff(MAX_X, MAX_Y, ADF_W, ADF_H, chip == 1);
  drawLabel(ADF_LX, ADF_LY, "ADF4351", TFT_BLUE,0);
  drawOnOff(ADF_X, ADF_Y, MAX_W, MAX_H, chip == 2);
  drawLabel(LMX_LX, LMX_LY, "LMX2595", TFT_BLUE,0);
  drawOnOff(LMX_X, LMX_Y, LMX_W, LMX_H, chip == 3);
  drawLabel(REF_LX, REF_LY, "Reference Oscillator", TFT_BLUE,0);
  drawNumBox(REF_X, REF_Y, REF_W, REF_H, refOsc, 6);
  drawLabel(PFD_LX, PFD_LY, "PFD", TFT_BLUE,0);
  drawNumBox(PFD_X, PFD_Y, PFD_W, PFD_H, chipGetPfd(), 6);
  drawLabel(MULT_LX, MULT_LY, "External Multiplier X", TFT_BLUE,0);
  drawNumBox(MULT_X, MULT_Y, MULT_W, MULT_H, chanData[channel].extMult, 0);
  drawLabel(KEYSH_LX, KEYSH_LY, "Ext Key FSK Shift (Hz)", TFT_BLUE,0);
  drawNumBox(KEYSH_X, KEYSH_Y, KEYSH_W, KEYSH_H, (chanData[channel].keyShift * 1000000.0) * (double) chanData[channel].extMult, 0);
  drawLabel(CWID_LX, CWID_LY, "CW Ident", TFT_BLUE,0);
  drawTextBox(CWID_X, CWID_Y, CWID_W, CWID_H, &chanData[channel].cwid[1]);
  drawLabel(CWSP_LX, CWSP_LY, "CW Speed (WPM)", TFT_BLUE,0);
  drawNumBox(CWSP_X, CWSP_Y, CWSP_W, CWSP_H, chanData[channel].cwidSpeed, 0);
  drawLabel(CWINT_LX, CWINT_LY, "CW Interval (Secs)", TFT_BLUE,0);
  drawNumBox(CWINT_X, CWINT_Y, CWINT_W, CWINT_H, chanData[channel].cwidInterval, 0);
  drawLabel(CWSH_LX, CWSH_LY, "CW FSK Shift (Hz)", TFT_BLUE,0);
  drawNumBox(CWSH_X, CWSH_Y, CWSH_W, CWSH_H, (chanData[channel].cwidShift * 1000000.0) * (double) chanData[channel].extMult, 0);
  drawLabel(JTID_LX, JTID_LY, "JT Ident", TFT_BLUE,0);
  drawTextBox(JTID_X, JTID_Y, JTID_W, JTID_H, chanData[channel].jtid);
  drawLabel(JTT1_LX, JTT1_LY, "JT Tone 1 Offset (Hz)", TFT_BLUE,0);
  drawNumBox(JTT1_X, JTT1_Y, JTT1_W, JTT1_H, chanData[channel].jtTone1 * 1000000.0 , 0);
  drawLabel(JTM0_LX, JTM0_LY, "JT Mode Off", TFT_BLUE,0);
  drawOnOff(JTM0_X, JTM0_Y, JTM0_W, JTM0_H, chanData[channel].jtMode == 0);
  drawLabel(JTM1_LX, JTM1_LY, "JT4G", TFT_BLUE,0);
  drawOnOff(JTM1_X, JTM1_Y, JTM1_W, JTM1_H, chanData[channel].jtMode == 1);
  if(chip == 3)
    {
    drawLabel(JTM2_LX, JTM2_LY, "JT65B", TFT_BLUE,0);
    drawOnOff(JTM2_X, JTM2_Y, JTM2_W, JTM2_H, chanData[channel].jtMode == 2);
    drawLabel(JTM3_LX, JTM3_LY, "JT65C", TFT_BLUE,0);
    drawOnOff(JTM3_X, JTM3_Y, JTM3_W, JTM3_H, chanData[channel].jtMode == 3);
    }
  drawLabel(EXIT_LX, EXIT_LY, "Exit", TFT_BLUE,0);
  drawOnOff(EXIT_X, EXIT_Y, EXIT_W, EXIT_H, 0);
}


void doConfigScreen(void) 
{
  bool done = false;
  double ret = 0;
  bool pressed;
  configScreenUpdate();
  while(!done)
  {
    pressed = tft.getTouch(&t_x, &t_y);
    if(pressed)
    {
      if(touchZone(ADF_X, ADF_Y, ADF_W, ADF_H))
      {
        chip = 2;
        changeChip();
        chipEncodeRegs();
        chipUpdate();
        configScreenUpdate();
      }

      if(touchZone(MAX_X, MAX_Y, MAX_W, MAX_H))
      {
        chip = 1;
        changeChip();
        chipEncodeRegs();
        chipUpdate();
        configScreenUpdate();
      }

      if(touchZone(LMX_X, LMX_Y, LMX_W, LMX_H))
      {
        chip = 3;
        changeChip();
        chipEncodeRegs();
        chipUpdate();
        configScreenUpdate();
      }

      if (touchZone(REF_X, REF_Y, REF_W, REF_H)) 
      {
      ret = getNumber("Enter Ref. Freq. (MHz)", 12);
      refOsc = ret;
      configScreenUpdate();
      }

      if (touchZone(PFD_X, PFD_Y, PFD_W, PFD_H)) 
      {
      double temp = chipGetFrequency();
      ret = getNumber("Enter PFD Freq. (MHz)", 12);
      chipCalcPfd(ret);
      chipSetFrequency(temp);
      chipUpdate();  
      configScreenUpdate();
      }

      if (touchZone(MULT_X, MULT_Y, MULT_W, MULT_H)) 
      {
      ret = getNumber("Enter Multiplier", 3);
      chanData[channel].extMult = ret;
      configScreenUpdate();
      }

      if (touchZone(KEYSH_X, KEYSH_Y, KEYSH_W, KEYSH_H)) 
      {
      ret = getNumber("Enter Ext Key FSK (Hz)", 5);
      chanData[channel].keyShift = ret / (double) chanData[channel].extMult ;
      chanData[channel].keyShift = chanData[channel].keyShift / 1000000.0;      //convert to MHz
      if(chanData[channel].keyShift != 0)
        {
          chanData[channel].fskMode |= EXTKEYBIT;
        }
      else
        {
          chanData[channel].fskMode &= NOTEXTKEYBIT;
        }
      configScreenUpdate();
      }

      if (touchZone(CWID_X, CWID_Y, CWID_W, CWID_H)) 
      {
      chanData[channel].cwidLen = getText("Enter CWID", &chanData[channel].cwid[1], 32);
      checkCwValid();
      configScreenUpdate();
      }

      if (touchZone(CWSP_X, CWSP_Y, CWSP_W, CWSP_H)) 
      {
      ret = getNumber("Enter CW Speed (WPM)", 3);
      chanData[channel].cwidSpeed = ret;
      checkCwValid();
      configScreenUpdate();
      }

      if (touchZone(CWINT_X, CWINT_Y, CWINT_W, CWINT_H)) 
      {
      ret = getNumber("Enter CW Interval (Secs)", 3);
      chanData[channel].cwidInterval = ret;
      checkCwValid();
      configScreenUpdate();
      }

      if (touchZone(CWSH_X, CWSH_Y, CWSH_W, CWSH_H)) 
      {
      ret = getNumber("Enter CW ID FSK (Hz)", 5);
      chanData[channel].cwidShift = ret / (double) chanData[channel].extMult ;
      chanData[channel].cwidShift = chanData[channel].cwidShift / 1000000.0;      //convert to MHz
      checkCwValid();
      configScreenUpdate();
      }

      if (touchZone(JTT1_X, JTT1_Y, JTT1_W, JTT1_H)) 
      {
      ret = getNumber("Enter Tone 1 Offset (Hz)", 5);
      chanData[channel].jtTone1 = ret ;
      chanData[channel].jtTone1 = chanData[channel].jtTone1 / 1000000.0;      //convert to MHz
      configScreenUpdate();
      }


      if (touchZone(JTID_X, JTID_Y, JTID_W, JTID_H)) 
      {
      getText("Enter JT ID", &chanData[channel].jtid[0], 13);
      jtInit();
      configScreenUpdate();
      }

      if (touchZone(JTM0_X, JTM0_Y, JTM0_W, JTM0_H)) 
      {
      chanData[channel].jtMode = 0;
      configScreenUpdate();
      }

      if (touchZone(JTM1_X, JTM1_Y, JTM1_W, JTM1_H)) 
      {
      chanData[channel].jtMode = 1;
      seconds = -1;                       //reset the timing after using the menu.
      milliseconds = 0;
      jtInit();
      configScreenUpdate();
      }

      if ((chip == 3) && (touchZone(JTM2_X, JTM2_Y, JTM2_W, JTM2_H))) 
      {
      chanData[channel].jtMode = 2;
      seconds = -1;                       //reset the timing after using the menu.
      milliseconds = 0;
      jtInit();
      configScreenUpdate();
      }

      if ((chip == 3) && (touchZone(JTM3_X, JTM3_Y, JTM3_W, JTM3_H))) 
      {
      chanData[channel].jtMode = 3;
      seconds = -1;                       //reset the timing after using the menu.
      milliseconds = 0;
      jtInit();
      configScreenUpdate();
      }

      if (touchZone(EXIT_X, EXIT_Y, EXIT_W, EXIT_H)) 
      {
      done = true;
      }  
    }
  }
}

void checkCwValid(void)
{
  if((chanData[channel].cwidInterval != 0) && (chanData[channel].cwidShift != 0) && (chanData[channel].cwidSpeed != 0) && (chanData[channel].cwidLen > 1))
   {
      seconds = -1;                       //reset the timing after using the menu.
      milliseconds = 0;
     cwidInit();
     chanData[channel].fskMode |= CWIDBIT;
   }
   else
   {
     chanData[channel].fskMode &= NOTCWIDBIT;
   }
}

