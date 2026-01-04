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

          case ADF5355:
            if(p>3) p=3;
            ADF5355_RFPWR = p;
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
            digitalWrite(LMX2595CEPin,o);
            LMX2595_OUTA_PD = !o;
          break;
          
          case ADF5355:
            ADF5355_RFAEN = o;
            ADF5355_RFBDIS = !o;
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

          case ADF5355:
            return ADF5355_RFPWR;
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

          case ADF5355:
            return ADF5355_RFAEN;
          break;
        }
     return 0;
  }

      bool chipLocked(void)
  {

      switch(chip)
        {
          case MAX2870:
          return digitalRead(MAX2870MUXPin);
          break;

          case ADF4351:
          return digitalRead(ADF4351MUXPin);
          break;

          case LMX2595:
          return digitalRead(LMX2595MUXPin);
          break;

          case ADF5355:
          return digitalRead(ADF5355MUXPin);
          break;
        }
     return 0;
  }

