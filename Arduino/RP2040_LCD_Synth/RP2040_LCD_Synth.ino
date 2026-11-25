// Synthesiser controller with touchscreen LCD interface
// Colin Durbridge G4EML 2024
// Optional support for LCD controller using crowpanel 3.5" HMI panel.

#include "SynthChip.h"
#include "ChipList.h"

#define LCDVERSION

#define VERSION 2.00

#define EEPROMVER 0x53

#define NUMBEROFCHANNELS 10

//Global values...

//enum chipType and NUM_CHIP_TYPES are both generated from CHIP_LIST in ChipList.h -
//to add a new chip type, edit that list, not this one.
enum chipType
{
  NONE,
  #define CHIP_ENTRY(name, instance) name,
  CHIP_LIST
  #undef CHIP_ENTRY
  NUM_CHIP_TYPES        //always 1 past the last valid chip type; also equals the number of chip types including NONE
};

//Each chip's display name is set in its own constructor (see e.g. MAX2870.ino) and
//retrieved via chipTypeName(index) in redirects.ino - there is no separate name list to update.

//Pointer to the SynthChip object for the currently selected chip type.
//Set (and kept in sync with "chip") by chipInit() in redirects.ino.
//To add a new chip type see the notes at the top of SynthChip.h.
SynthChip* activeChip = nullptr;

bool saveRequired = false;

//These values are saved to the eeprom for recall on statup. 
//these values alpply to all channels

uint8_t chip = MAX2870;                 //index to the current chip type (applies to all channels)
uint8_t selChan = 0;                    //FF if channel is externally selected by switches. Channel number to force one channel. 
double refOsc = 100.000 ;               //reference oscillator frequency in MHz (applies to all channels)
uint16_t batcal = 492;                  //battery calibration value

//Bit definitions for fskMode flags

#define CWIDBIT 0x01
#define NOTCWIDBIT 0xFE
#define EXTKEYBIT 0x02
#define NOTEXTKEYBIT 0xFD

//this structure is repeated for each channel

struct chanstruct
{
uint32_t reg[80];                       // allow for up to 80 32 bit registers. 
uint8_t fskMode = 0;                    // 8 flag bits to indicate enabled special FSK modes.  Bit 0 = CWID   Bit 1 = External Key  
uint8_t cwidLen = 1;                    //number of characters in the CWID
uint8_t cwidSpeed = 10;                 //CWID speed in words per minute
uint8_t cwidInterval = 60;              //CWID Inteval in seconds.
float cwidShift = 0;                    //CW ID FSK Shift in MHz 
float keyShift = 0;                     //external key FSK Shift in MHz 
char cwid[32] = " ";                    // up to 30 CWID characters
uint8_t jtMode = 0;                     //JT mode
char jtid[13] = " ";                    //JT Message
float jtTone1 = 0;                      //JT Tone 1 Offset from Nominal Frequency (Mhz)
uint8_t jtInterval = 60;              //Digi mode Interval in seconds.
uint8_t extMult = 1;                    //Multiplcation factor for external frequency multiplier. (used to calculate the correct FSK Shifts.)
};

struct chanstruct chanData[NUMBEROFCHANNELS];


//End of saved values

//Note: numberOfRegs, maxPfd, minPfd, maxOsc, minOsc, jt4Only and jtDisable used to be
//duplicated here as globals, kept in sync with the selected chip by chipInit(). They are
//now read directly from activeChip (e.g. activeChip->numberOfRegs) wherever needed, so
//there is only one copy of this data - the one on the chip object itself.
uint8_t channel = 0;                      //currently active channel.

uint32_t cwidKeyUpN = 1;                  //key up value for the PLL N used to shift the frequency for CWID. Calculated by cwidInit()
uint32_t cwidKeyUpNum = 1;                //Key up value for the PLL numerator used to shift the frequency for CWID. Calculated by cwidInit()
uint32_t cwidKeyUpDen = 1;                //Key up value for the PLL denominator used to shift the frequency for CWID. Calculated by cwidInit()

uint32_t ExtKeyUpN = 1;                  //key up value for the PLL N used to shift the frequency for External Key. Calculated by ExtKeyInit()
uint32_t ExtKeyUpNum = 1;                //Key up value for the PLL numerator used to shift the frequency for External Key. Calculated by ExtKeyInit()
uint32_t ExtKeyUpDen = 1;                //Key up value for the PLL denominator used to shift the frequency for External Key. Calculated by ExtKeyInit()

void saveSettings(void);

#include <EEPROM.h>
#include <SPI.h>
#include <JTEncode.h>



#define JT4G_TONE_SPACING        315         // 72 * 4.37 Hz
#define JT4_DELAY                229         // Delay value for JT4
#define Q65_TONE_SPACING        3.33333       // 3.3333 Hz for Mode 30A
#define Q65_DELAY               150         // Delay in ms for Q65 15
#define Q65_SYMBOL_COUNT     85          //85 symbols


uint8_t jtBuffer[256];
uint8_t jtSymbolCount;
uint16_t jtToneDelay;
double jtToneSpacing;
uint8_t jtNumberOfTones;

uint32_t jtN[66];               //Note Element 0 is the nominal frequency. Element 1 is the Tone 1 frequency
uint32_t jtNum[66];
uint32_t jtDen[66];


JTEncode jtEncode;

int seconds = 0;                         //seconds counter.  counts up to 120 seconds.  0-60 is even minute. 60-120 is odd minute. 
int milliseconds = 0;                   //millisecond counter used to increment seconds counter. 

bool cwidActive = false;                //flag to start sending CW ID
int nextcwidTime = 60;                   //trigger time for next CWID

bool jtActive = false;                  //flag to start Jt Sending
int  nextjtTime = 1;                       //trigger time for next JT sequence

bool lastKeyState = 1;                 //external key state last pass 1 = key up 0 = key down

#define GPSTXPin 0                      //Serial data to GPS module 
#define GPSRXPin 1                      //SeriaL data from GPS module

//The touchscreen board's display/touch SPI wiring uses GPIO 8-16, so channel select and
//external key use different pins here than on the non-LCD board.
#define CHANSEL0Pin 19                  //External channel select pins. Pulled up to 3V3. High is Logic 0 Low is Logic 1
#define CHANSEL1Pin 20
#define CHANSEL2Pin 21
#define CHANSEL3Pin 26
#define EXTKEYPin 27                    //External Key input Pulled upp to 3V3 ground to key

char gpsBuffer[256];                     //GPS data buffer
int gpsPointer;                          //GPS buffer pointer. 
char gpsCh;
int gpsSec = -1;                         //gps Seconds counter 0-59 is even minute, 60-119 is odd minute. -1 is GPS Invalid 
bool gpsActive = false; 
int lastsec=0;
int gpsH=0;
int gpsM=0;
int gpsS=0;

bool showSync=false;
bool showingGPS = false;
int gpstimeout =0;

#include "LGFX_Config.h"   // LovyanGFX display/touch configuration for this hardware (see LGFX_Config.h)
LGFX tft;                   // Display and touch object
// Set REPEAT_CAL to true instead of false to run calibration again, otherwise it will only be done once.
#define REPEAT_CAL false
uint16_t t_x = 0, t_y = 0; // To store the touch coordinates

//Set true by a screen/pad right before it returns after acting on a touch (e.g.
//selecting an item from a popup list). While true, getTouchDebounced() reports
//"not touched" regardless of what the touch controller says, so the same
//physical touch can't immediately bleed through and re-trigger something on the
//screen underneath as soon as it redraws. Cleared automatically the first time
//a genuine release is seen. See getTouchDebounced() in commonScreen.ino.
bool touchConsumed = false;

void setup() 
{
  Serial.begin();                       //USB serial port
  analogReadResolution(12);
  Serial1.setRX(GPSRXPin);              //Configure the GPIO pins for the GPS module
  Serial1.setTX(GPSTXPin);

  pinMode(CHANSEL0Pin, INPUT_PULLUP);   //configure the channel select pins. 
  pinMode(CHANSEL1Pin, INPUT_PULLUP);   //Inverted logic to allow switches to ground. 
  pinMode(CHANSEL2Pin, INPUT_PULLUP);
  pinMode(CHANSEL3Pin, INPUT_PULLUP);

  pinMode(EXTKEYPin, INPUT_PULLUP);     //configure the external Key Input

  Serial1.begin(9600);                  //start GPS port comms on pins 0 and 1
  gpsPointer = 0;
  delay(1000);
  EEPROM.begin(4096);
  if(EEPROM.read(0) == EEPROMVER)        //magic number to indcate EEPROM is valid
    {
      EEPROM.get(1,chip);              //chip type for all channels
      EEPROM.get(2,selChan);           //read the selected channel. 0xFF if externally switched.
      EEPROM.get(3,refOsc);           //reference oscillator for all channels.
      EEPROM.get(11,batcal);          //battery cal factor 
      EEPROM.get(13,chanData);         //get channel data structure.
      if(selChan == 255)
        {
          channel = readChannelInputs();      //read the channel select bits. 
        }
      else
        {
          channel = selChan;          //fix the channel number. 
        } 
    }
  else
    {
      changeChip();                 //blank/invalid EEPROM - force reset of all channels to valid defaults.
    }
   chipInit();
   initChannel();
   seconds = -1;

  tft.init();
  tft.setRotation(1);
  tft.setBrightness(255);             // backlight fully on (TFT_eSPI did this automatically in init())
  if(homeScreenTouched())
   {
    while(homeScreenTouched());
    delay(1000);
    touch_calibrate(1);
   }
   else
   {
    touch_calibrate(0);
   }
  homeScreenUpdate();

}

void loop() 
{
  Serial.print("\n");
  Serial.print(chipTypeName(chip));
  Serial.println(" Synthesiser programmed, Sleeping");

  chipUpdate();
  delay(100);
  chipUpdate();
  
  unsigned long loopTimer = millis();
  while(1)
   {
     while(loopTimer == millis());          //hang waiting for the next 1mS timeslot
     loopTimer = millis();
     milliseconds++;
     if(milliseconds == 1000)
      {
        seconds++;
        milliseconds = 0;
        if(seconds == 120)
          {
            seconds = 0;
          }
        displayVolts();
      }

     if((gpsSec !=-1)&&(gpsS != lastsec))
       {
        lastsec=gpsS;
        displayGPS();
        showingGPS = true;
        gpstimeout = 0;
        if(gpsSec != seconds) 
         {
          seconds = gpsSec;
          milliseconds = 500;
         }      
       }

    if(showingGPS)
    {
      gpstimeout++;
    }

    if((gpstimeout > 10000) && (showingGPS))
      {
        showingGPS = false;
        gpsSec = -1;
        homeScreenUpdate();
      }

//synchronise the local clock to the GPS clock if available

     if(selChan == 255)             //external chaannel selection
       {
         uint8_t newChan=readChannelInputs();
         if(newChan != channel)
           {
            channel=newChan;
            seconds = -1;                       //reset the timing after changing channel.
            milliseconds = 0;
            jtActive = false;
            cwidActive = false;
            chipUpdate();
            initChannel();
           }

       }

     if((chanData[channel].fskMode & CWIDBIT) && (seconds == nextcwidTime))
       {
        cwidActive = true;                                        //start this CW ID
        nextcwidTime = (seconds + chanData[channel].cwidInterval) % 120;            //schedule the next CW ID
       }

     if((chanData[channel].jtMode != 0) && (seconds == nextjtTime))
       {
        jtActive = true;                                        //start the JT Sequence
        nextjtTime = (seconds + chanData[channel].jtInterval) % 120;            //schedule the next CW ID
       }

    if(chanData[channel].fskMode & CWIDBIT)
      {
        cwidTick();
      }

    if(chanData[channel].jtMode != 0)
      {
        jtTick();
      }

    if(chanData[channel].fskMode & EXTKEYBIT)
      {
        keyTick();
      }

      if(Serial.available() > 0 )          //test for USB command connected
     {
      bool valid = false;
         while(Serial.available())
         {
           if(Serial.read() == 13)
            {
              valid = true;
            }
         }
       if(!valid) return;
       chipExtKey(true);                    //reset to nominal carrier frequency
       mainMenu();                         //timing loop stops while the menu system is running.
       seconds = -1;                       //reset the timing after using the menu.
       milliseconds = 0;
       initChannel();  
       chipUpdate();    
       Serial.print("\nSynthesiser programmed. Press any key for menu");
       delay(500);
       flushInput();
     }
    
    if(Serial1.available() > 0)           //data received from GPS module on pins 0 and 1
      {
        while(Serial1.available() >0)
          {
            gpsCh=Serial1.read();
            if(gpsCh > 31) gpsBuffer[gpsPointer++] = gpsCh;
            if((gpsCh == 13) || (gpsPointer > 255))
              {
                gpsBuffer[gpsPointer] = 0;
                processNMEA();
                gpsPointer = 0;
              }
          }

      }

   if(homeScreenTouched())
    {
    if(processHomeScreenTouch())
      {
       homeScreenUpdate();
      }
    }   

  }  
}

void initChannel()
{
  chipDecodeRegs();
  if(chanData[channel].fskMode & CWIDBIT)
    {
      cwidInit();
    }
   
  if(chanData[channel].fskMode & EXTKEYBIT)
    {
      keyInit();
    }

   if(chanData[channel].jtMode)
     {
       jtInit();
     }


}

void processNMEA(void)
{
  float gpsTime;

 if(strstr(gpsBuffer , "RMC") != NULL)                         //is this the RMC sentence?
  {
    if(strstr(gpsBuffer , ",A,") != NULL)                       // is the data valid?
      {
       int p=strcspn(gpsBuffer , ",");                         // find the first comma
       gpsTime = strtof(gpsBuffer+p+1 , NULL);                 //copy the time to a floating point number
       gpsSec = 60 * ((int(gpsTime) / 100) % 2);                 // count odd and even minutes
       gpsSec = gpsSec + (int(gpsTime) % 100); 
       gpsH=int(gpsTime)/10000;
       gpsM=(int(gpsTime)/100) % 100;
       gpsS= int(gpsTime) % 100;
       gpsActive = true;
      }
    else
     {
       gpsSec = -1;                                            //GPS time not valid
     }
  }


}

uint8_t readChannelInputs(void)
{
  uint8_t ch;
  ch = (digitalRead(CHANSEL3Pin) << 3) | (digitalRead(CHANSEL2Pin) << 2) | (digitalRead(CHANSEL1Pin) << 1) | (digitalRead(CHANSEL0Pin));
  ch = ~ch & 0x0F;           //invert the logic and limit to 4 bits. 
  if(ch > (NUMBEROFCHANNELS -1)) ch = NUMBEROFCHANNELS-1;
  return ch;
}

void saveSettings(void)
{
    EEPROM.write(0, EEPROMVER);         //magic number to indcate EEPROM is valid
    EEPROM.put(1,chip);            //save the chip type
    EEPROM.put(2,selChan);         //Save the currently selected channel
    EEPROM.put(3,refOsc);          //reference oscillator for all channels.
    EEPROM.put(11,batcal);           //battery calibration factor
    EEPROM.put(13,chanData);         //get channel data structure. 
    EEPROM.commit();
}
