//This module redirects generic functions to the relevant chip specific version

void chipUpdate(void)
{
  switch(chip)
  {
    case MAX2870:
    Max2870Update();
    break;

    case ADF4351:
    ADF4351Update();
    break;

    case LMX2595:
    LMX2595Update();
    break;
  }
}

void chipSetParameters(void)
{
  switch(chip)
  {
    case MAX2870:
    Max2870SetParameters();
    break;

    case ADF4351:
    ADF4351SetParameters();
    break;

    case LMX2595:
    LMX2595SetParameters();
    break;
  }
}

void chipInit(void)
{
  switch(chip)
  {
    case MAX2870:
    Max2870Init();
    break;

    case ADF4351:
    ADF4351Init();
    break;

    case LMX2595:
    LMX2595Init();
    break;
  }
}

void chipSetFrequency(double f)
{
  switch(chip)
  {
    case MAX2870:
    Max2870SetFrequency(f);
    break;

    case ADF4351:
    ADF4351SetFrequency(f);
    break;

    case LMX2595:
    LMX2595SetFrequency(f);
    break;
  }
}

double chipGetFrequency(void)
{
  double freq;
  switch(chip)
  {
    case MAX2870:
    freq = Max2870GetFrequency();
    break;

    case ADF4351:
    freq = ADF4351GetFrequency();
    break;

    case LMX2595:
    freq = LMX2595GetFrequency();
    break;
  }

  return freq;
}

void chipCalcFreq(void)
{
        switch(chip)
        {
          case MAX2870:
          Max2870CalcFreq();
          break;

          case ADF4351:
          ADF4351CalcFreq();
          break;

          case LMX2595:
          LMX2595CalcFreq();
          break;
        }
}
  void chipDecodeRegs(void)
  {
      switch(chip)
        {
          case MAX2870:
          Max2870DecodeRegs();
          break;

          case ADF4351:
          ADF4351DecodeRegs();
          break;

          case LMX2595:
          LMX2595DecodeRegs();
          break;
        }
  }

    void chipEncodeRegs(void)
  {
      switch(chip)
        {
          case MAX2870:
          Max2870EncodeRegs();
          break;

          case ADF4351:
          ADF4351EncodeRegs();
          break;

          case LMX2595:
          LMX2595EncodeRegs();
          break;
        }
  }

    void chipSetDefault(void)
  {
      switch(chip)
        {
          case MAX2870:
          Max2870SetDefault();
          break;

          case ADF4351:
          ADF4351SetDefault();
          break;

          case LMX2595:
          LMX2595SetDefault();
          break;
        }
  }

    void chipFskKey(bool key)
  {

      switch(chip)
        {
          case MAX2870:
          Max2870FskKey(key);
          break;

          case ADF4351:
          ADF4351FskKey(key);
          break;

          case LMX2595:
          LMX2595FskKey(key);
          break;
        }
  }

  void chipExtKey(bool key)
  {

      switch(chip)
        {
          case MAX2870:
          Max2870ExtKey(key);
          break;

          case ADF4351:
          ADF4351ExtKey(key);
          break;

          case LMX2595:
          LMX2595ExtKey(key);
          break;
        }
  }

    void chipJtShift(uint8_t val)
  {

      switch(chip)
        {
          case MAX2870:
          Max2870jtShift(val);
          break;

          case ADF4351:
          ADF4351jtShift(val);
          break;

          case LMX2595:
          LMX2595jtShift(val);
          break;
        }
  }


    void chipSaveFskShift(void)
  {

      switch(chip)
        {
          case MAX2870:
          Max2870SaveFskShift();
          break;

          case ADF4351:
          ADF4351SaveFskShift();
          break;

          case LMX2595:
          LMX2595SaveFskShift();
          break;
        }
  }

      void chipSaveKeyShift(void)
  {

      switch(chip)
        {
          case MAX2870:
          Max2870SaveKeyShift();
          break;

          case ADF4351:
          ADF4351SaveKeyShift();
          break;

          case LMX2595:
          LMX2595SaveKeyShift();
          break;
        }
  }


    void chipSaveJt(uint8_t index)
  {

      switch(chip)
        {
          case MAX2870:
          Max2870SaveJt(index);
          break;

          case ADF4351:
          ADF4351SaveJt(index);
          break;

          case LMX2595:
          LMX2595SaveJt(index);
          break;
        }
  }


    double chipGetPfd(void)
  {
     double pfd;
      switch(chip)
        {
          case MAX2870:
          pfd = Max2870GetPfd();
          break;

          case ADF4351:
          pfd = ADF4351GetPfd();
          break;

          case LMX2595:
          pfd = LMX2595GetPfd();
          break;
        }

    return pfd;
  }

  double chipCalcPfd(double pfd)
  {
     double rpfd;
      switch(chip)
        {
          case MAX2870:
          rpfd = Max2870CalcPFD(pfd);
          break;

          case ADF4351:
          rpfd = ADF4351CalcPFD(pfd);
          break;

          case LMX2595:
          rpfd = LMX2595CalcPFD(pfd);
          break;
        }

    return rpfd;
  }