//this module adds chip specific functions needed to support the LCD version. 
//these are kept seperate so that the non-lcd code is not changed.

    void chipSetPower(uint8_t p)
  {

      switch(chip)
        {
          case MAX2870:
            if(p>3) p=3;
            Max2870_APWR = p;
          break;

          case ADF4351:
            if(p>3) p=3;
            ADF4351_RFPWR = p;
          break;

          case LMX2595:
            if(p>63) p=63;
            LMX2595_OUTA_PWR = p;
          break;
        }
    chipEncodeRegs();
    chipUpdate();
  }

    void chipEnableOutput(bool o)
  {

      switch(chip)
        {
          case MAX2870:
            Max2870_RFA_EN = o;
          break;

          case ADF4351:
            ADF4351_RFEN = o;
          break;

          case LMX2595:
          LMX2595_OUTA_PD = !o;
          break;
        }
     chipEncodeRegs();
     chipUpdate();   
  }

      uint8_t chipGetPower()
  {

      switch(chip)
        {
          case MAX2870:
            return Max2870_APWR;
          break;

          case ADF4351:
            return ADF4351_RFPWR;
          break;

          case LMX2595:
            return LMX2595_OUTA_PWR;
          break;
        }
    return 0;
  }

    bool chipGetOutput(void)
  {

      switch(chip)
        {
          case MAX2870:
            return Max2870_RFA_EN;
          break;

          case ADF4351:
            return ADF4351_RFEN;
          break;

          case LMX2595:
          return !LMX2595_OUTA_PD;
          break;
        }
     return 0;
  }