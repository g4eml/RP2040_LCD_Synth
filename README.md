# RP2040_LCD_Synth

## Description

This Project is a development of the RP2040_Synth project with the addition of a Touch screen LCD display module, refered to here as the HMI Module (Human Machine Interface). This is intended to be used as a piece of test equipment. 
Currently supports 3 different type of synthesiser chip, the ADF4351, the Max2870 and the LMX2595.

## Features

Control and configuration can be done from the Touch Screen or from a USB serial port connection. 

No special programming hardware or software required. 

Serial programming using the RP2040s built in USB serial port and standard boot loader. 

Settings are saved to EEPROM for automatic load on power on.

Support for FSK CW Identification for beacon use.

Supports JT data modes for Beacon Identification.  JT4G is available on all chip types. JT65B and JT65C are only available on the LMX2595.

Note:- Whilst JT4G mode is available on all chips there may be some limitations. The ADF4351 and MAX2870 chips have a limited frequency resolution and may not be able to accurately set the required tone frequencies especially when an external multiplication chain is used.

Supports GPS connection for the accurate timing required for JT data modes. 

Supports 10 different channels which can also be selected by external switches. 

Supports External Key Input with independently configurable FSK Shift. 

Default register settings can be loaded for all synthesiser types to produce some initial RF output. This is often the hardest thing to do with a new chip type.

Supports use with an external frequency multiplier for the higher microwave bands. Allows frequency entry and display of the final output frequency. Adjusts FSK shifts to compensate for multiplication factor.  

Direct output frequency and phase detector frequency entry. Register values are calculated automatically.  


## Requirements

This code is designed to work with the Elecrow CrowPanel Pico-3.5 inch 480x320 TFT LCD HMI Module. https://www.aliexpress.com/item/1005007250778536.html 

![LCD_Main](https://github.com/user-attachments/assets/862dfbea-d7c5-44f0-9848-70c811d6a43f)

Note:- similar HMI Panels are available using the ESP32 processor chip. Make sure that you are purchasing the RP2040 version. 


Suitable Synthesiser boards are available from Ebay, Amazon, Ali Express, SV1AFN etc. 

![ADF4351](https://github.com/g4eml/RP2040_Synth/assets/1881884/3eacdbbf-ae51-43bd-9b6d-3b69f2eebe77)  ![max2870](https://github.com/g4eml/RP2040_Synth/assets/1881884/94cc4c3b-1bc7-4b79-8cc2-5abb77e51a20)

![lmx2595](https://github.com/g4eml/RP2040_Synth/assets/1881884/57675dc3-57eb-4d40-87e9-84847eca80ed) ![SV1AFN](https://github.com/g4eml/RP2040_Synth/assets/1881884/6c4bbb31-debc-4be2-bb3c-899d4c1da64c)

If you wish to use the JT data modes you will also need a GPS module to provide the necessary accurate timing.  

![GPS](https://github.com/user-attachments/assets/53ceb650-0525-4138-a7ff-bff1f450c409)

## Connecting

All of the various synthesiser moduels use different connectors and pinouts.

The simplest solution is to make an adaptor cable that plugs into the GP connector of the HMI module and fit suitable plugs for the modules to the other end. Keep the wiring as short as possible. Probably no more than 100mm long. 

The power can be provided by the USB-C connector marked 'USB' on the end of the HMI module



## Programming or updating the HMI Module (quick method)

This can be done either before or after the Syntesiser has been connected, it makes no difference. Updating to a new firmware version is done the same way.   

1. Locate the latest compiled firmware file 'RP2040_LCD_Synth.uf2' which will be found here https://github.com/g4eml/RP2040_LCD_Synth/releases and save it to your desktop. 

2. Connect the HMI Module to your PC using the USB-C port on the side. 

3. Hold down the BOOT button on the back of the HMI module and briefly press the Reset button. The RP2040 should appear as a USB disk drive on your PC.

3. Copy the .uf2 file onto the USB drive. The RP2040 will recognise the file and immediately update its firmware and reboot.

## Building your own version of the firmware (longer method and not normally required)

The RP2040 is programmed using the Arduino IDE with the Earl F. Philhower, III  RP2040 core. 

#### Installing the Arduino IDE

1. Download and Install the Arduino IDE 2.3.0 from here https://www.arduino.cc/en/software

2. Open the Arduino IDE and go to File/Preferences.

3. in the dialog enter the following URL in the 'Additional Boards Manager URLs' field: https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json

4. Hit OK to close the Dialog.

5. Go to Tools->Board->Board Manager in the IDE.

6. Type “RP2040” in the search box.

7. Locate the entry for 'Raspberry Pi Pico/RP2040 by Earle F. Philhower, III' and click 'Install'

### Installing the required libraries

1. From the Arduino IDE select (Tools > Manage Libraries)
2. Search for 'JTEncode' Scroll down to find the Etherkit JTEncode library by Jason Milldrum.
3. Click Install
4. Now search for 'TFT_eSPI' and find the TFT graphics library by Bodmer.
5. Click Install

#### Downloading the Software.

1. Download the latest released source code .zip file from https://github.com/g4eml/RP2040_LCD_Synth/releases

2. Save it to a convenient location and then unzip it.

The TFT_eSPI Library is unusual in that it needs to be configured to the TFT display in use by modifying library files. The required modified versions of the files are located in the 'LCD-eSPI Settings' folder of this repository. Copy the files 'User_Setup.h' and 'User_Setup_Select.h' from the downloaded 'LCD-eSPI Settings' folder to your Arduino libraries directory. This will normaly be found at 'Documents/Arduino/libraries/TFT_eSPI'

#### Programming the RP2040

1. Open the Arduino IDE and click File/Open

2. Navigate to the File RP2404Synth/RP2040_LCD_Synth.ino (downloaded in the previous step) and click Open.

3. Select Tools and make the following settings.

   Board: "Raspberry Pi Pico"
   
   Debug Level: "None"

   Debug Port: "Disabled"

   C++ Exceptions: "Disabled"

   Flash Size: "2Mb (no FS)"

   CPU Speed: "133MHz"

   IP/Bluetooth Stack: "IPV4 Only"

   Optimise: "Small (-Os)(standard)"

   RTTI: "Disabled"

   Stack Protection: "Disabled"

   Upload Method: "Default (UF2)"

   USB Stack: "Pico SDK"  

5. Connect the HMI Module to the USB port, hold down the BOOT button and briefly press the reset Button. 

6. Click Sketch/Upload.

The Sketch should compile and upload automatically to the Pico. If the upload fails you may need to disconnect the module and then hold down the BOOT button while reconnecting. 

## Connections

The pinout of the various Synthesiser modules tends to vary so it is difficult to give exact wiring diagrams.
The LMX2595 takes a lot of current so is best supplied from the USB 5V line using its own 3V3 regulator. The other chips can be powered from the 3V3 output of the HMI module. Power arrangements may need additional attention to minimise phase noise. 
The HMI module does not have a connector for the USB 5V supply. You may need to add your own additional wire for this. 

The generic wiring instructions are as follows:-

| HMI Module | ADF4351 | MAX2870 | LMX2595 |
| :---:  |  :---:  |  :---:  |   :---: |
|5V      |   N/C   |   N/C   |    5V   |
|3V3     |   3V3   |   3V3   |   N/C   |
|GND     |   GND   |   GND   |   GND   |
|GPO3    |   CE    |   CE    |   CE    |
|GPO4    |   MUX   |   MUX   |  MUXout |
|GPO5    |   LE    |   LE    |   CSB   |
|GPO6    |   CLK   |   CLK   |   SCK   |
|GPO7    |   DAT   |   DATA  |   SDI   |

The firmware also supports the optional connection of a GPS module. This is used to accurately set the time, which is needed for the JT modes. It is not needed for Local Oscillator or a CW only beacon. Any GPS module with a 3V3 output can be used. It needs to output NMEA data at 9600 Baud. One of the low cost NEO6M modules was used for development. 

| HMI Module | GPS Module |
| :---:  |    :---:   |
|GP0     |Rx          |
|GP1     |Tx          |

4 switches can be used to select the operating channel. (These are optional. If they are not used the power-up channel can be selected using the menu.)
The 4 inputs have internal pull up resistors to 3.3V. All that is needed is 4 switches to connect the pins to ground. 
Inverted logic is used. A grounded pin is read as a Logic 1. A floating pin is read as a logic 0.
For example, all pins left open circuit is read as channel 0. 

An external Morse Key can be connected between GP27 and Ground. This can be configured for FSK keying using the K Menu Option. 

| HMI Module | Function |
| :---:  |    :---:   |
|GP19    |Bit 0  Channel 1|
|GP20    |Bit 1  Channel 2|
|GP21    |Bit 2  Channel 4|
|GP26    |Bit 3  Channel 8|
|GP27    |External Key|

## Firmware description

On first use the screen will show the Touch calibration screen. Follow the instructions and touch each of the four corners of the screen. 

By default on power up the firmware will immediately send the EEPROM saved register values to the sunthesiser chip.  If the CW ident feature is enabled the ident will begin. If the JT mode is enabled the JT sequence will begin. Note that this will initially not be correctly timed. Once the GPS aquires its satellites the timing will synchronise. 

## Main Screen

![LCD_Main](https://github.com/user-attachments/assets/56206c6a-67a6-4de1-874d-cec92da580d4)

The main screen allows entry of output frequency, power setting and output On/Off. 

To change a value just click on the black box and a pop-up alphabetic or numeric keypad will appear. 

10 separate channels (memories) can be selected. 

The power setting is the value sent to the Synthesiser chip, the available range of values will be chip dependent. See the relevant datasheets for more details. 

Entry to the configuration screen is by the Config button. 

The Save button will save the current settings to the selected Channel. 

## Configuring directly from the LCD Screen config page. 

![LCD_CONFIG](https://github.com/user-attachments/assets/7a5d4b7b-7f5a-4216-963c-416c593c8d08)

Configuration should be straightforward. First select the Synthesiser type, then enter the Reference Oscillator Frequency and PFD.  CW Ident and /or JT Mode can be configured if required. 
When complete select Exit to return to the main screen. Then on the main screen select Save to save the new settings to EEPROM. 

## Using the USB serial port and a terminal program such as Putty or Terraterm. Pressing any key should result in the menu being displayed.

![Screenshot (112)](https://github.com/user-attachments/assets/1e3f835e-65b7-4d6e-8606-7d9504bda405)

### Commands
Commands are entered by a single key press. Text and numbers require a carriage return to enter. Pressing Question Mark will usually give more detailed help. 
Changes to registers and parameters will be applied immediatey so you should be able to observe the result of the change in real time. 

#### The first three Menu Items are common to all channels. 

T = Select Chip Type. Allows the Synthesiser chip type to be selected. Normally only needed the first time you configure the firmware. This will also clear all memories and reset them to the default settings. 

O = Set Reference Oscillator Frequency. Sets the Reference Oscillator Frequency. Enter the actual frequency of your reference including any frequency error. The firmware will attempt to adjust for this. 

N = Set Channel Number. Enter the channel number from 0 to 9. This channel will then be loaded and you can adjust the settings using the menu. This overrides the channel selected by any external switches. if you wish to use external switches to select a channel then you must change this setting to 255 before saving the EEPROM. A setting of 255 enables external channel selection. 

#### The following menu settings are applied to the currently selected channel. Each channel may have different settings. 

D = Restores the Default Register Values for chip. Sets default values to all registers. Tries to program a 10Mhz PFD and requests a frequency. After this your Synthesiser should start to output RF. 

P = Enter PFD Frequency. Set the required PFD frequency. The firmware will attempt to get as close as possible to the requested value but not all values can be achieved.

M = Set External Multiplier. If the Synthesiser is followed by a multiplier for higher frequencies this option allows you to specify the multiplcation factor. Final Multiplied Frequencies can then be entered and viewed. 

F = Enter Output Frequency. Set the required output frequency. The firmware will attempt to calculate the closest register values to achieve this. Changing the PFD may improve the result. 

C = Calculate and display frequency from current settings. The firmware will calculte the expected output frequency from the current register settings. Useful to check after youy make manual changes.

V = View / Enter Variables for Registers. Allows viewing or entry of parameters using the same names as defined in the chip datasheet. Press ? for a full list of the available parameter names. 

R = View / Enter Registers Directly in Hex. Allws direct entry of regiser values in Hexadecimal. Useful when transfering values calculated by another program. 

I = Configure CW Ident. Alows entry of CW Ident, CW Speed, Ident Period and FSK Shift.  A shift of -800Hz is a typical value. Ident period is only valid for a CW only configuration. If a JT mode is also enbled then the CW ident will be sent every odd minute. 

J = Configure JT modes. Allows entry of a 13 character message. This would normally be the Callsign and Maidenhead locator. The JT ident will be sent every Even minute. Accurate timing requires a GPS module to be connected.

K = Configure External Key. An External Morse key can be connected and used to frequency shift the signal. The FSK Shift can be entered and is seperate from the CWID shift. 

G = View GPS NMEA data. This is used to test the GPS connection. when selected it will echo all GPS data to the screen. Press any key to exit this mode. 

S = Save Registers to EEPROM. Saves the current Synthesiser settings to EEPROM. They will then be automatically loaded on the next power cycle. You must do this at least once. 

X = Exit Menu. Exits from the menu, re-initialises the Synthesiser and starts the CW Ident. 

## 
