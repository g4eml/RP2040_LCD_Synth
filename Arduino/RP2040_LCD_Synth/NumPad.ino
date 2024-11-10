// Keypad start position, key sizes and spacing
#define PADX 120
#define PADWIDTH 276
#define PADHEIGHT 320
#define KEY_X PADX + 60 // Centre of top left 
#define KEY_Y 120
#define KEY_W 60 // Width and height
#define KEY_H 33
#define KEY_SPACING_X 18 // X and Y gap
#define KEY_SPACING_Y 12
#define KEY_TEXTSIZE 1   // Font size multiplier

// Using two fonts since numbers are nice when bold
#define LABEL1_FONT &FreeSansOblique12pt7b // Key label font 1
#define LABEL2_FONT &FreeSansBold12pt7b    // Key label font 2

// Number display box size and location
#define DISP_X PADX + 6
#define DISP_Y 5
#define DISP_W PADWIDTH - 12
#define DISP_H 50

//Prompt Line
#define PROMPT_X PADX + 10
#define PROMPT_Y 65

// Number length, buffer for storing it and character index
char numberBuffer[13];
uint8_t numberIndex = 0;


// Create 15 keys for the keypad
char keyLabel[15][5] = {"Clr", "Del", "-", "1", "2", "3", "4", "5", "6", "7", "8", "9", ".", "0", "Ent" };
uint16_t keyColor[15] = {TFT_RED, TFT_DARKGREY, TFT_DARKGREY,
                         TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         TFT_BLUE, TFT_BLUE, TFT_BLUE,
                         TFT_BLUE, TFT_BLUE, TFT_DARKGREEN
                        };

// Invoke the TFT_eSPI button class and create all the button objects
TFT_eSPI_Button key[15];


//------------------------------------------------------------------------------------------
double getNumber(const char* prompt, uint8_t digits) 
{

  numberIndex = 0;
  numberBuffer[0] = 0;

  // Draw keypad background
  tft.fillRect(PADX, 0, PADWIDTH, PADHEIGHT, TFT_DARKGREY);

  // Draw Number display area and frame
  tft.fillRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_BLACK);
  tft.drawRect(DISP_X, DISP_Y, DISP_W, DISP_H, TFT_WHITE);
  //write Prompt

  tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
  tft.setFreeFont(&FreeSans12pt7b);  // Choose a nice font that fits box
  // Draw the string, the value returned is the width in pixels
  tft.setTextColor(TFT_CYAN);
  tft.drawString(prompt, PROMPT_X, PROMPT_Y);

  drawKeypad();

  bool done = false;

  while(!done)
  {
      // Pressed will be set true is there is a valid touch on the screen
      bool pressed = tft.getTouch(&t_x, &t_y);

      // / Check if any key coordinate boxes contain the touch coordinates
      for (uint8_t b = 0; b < 15; b++) 
      {
        if (pressed && key[b].contains(t_x, t_y)) 
        {
          key[b].press(true);  // tell the button it is pressed
        }
        else 
        {
          key[b].press(false);  // tell the button it is NOT pressed
        }
      }

      // Check if any key has changed state
      for (uint8_t b = 0; b < 15; b++) 
      {

        if (b < 3) tft.setFreeFont(LABEL1_FONT);
        else tft.setFreeFont(LABEL2_FONT);

        if (key[b].justReleased()) key[b].drawButton();     // draw normal

        if (key[b].justPressed()) 
        {
          key[b].drawButton(true);  // draw invert

          // if a numberpad button, append the relevant # to the numberBuffer
          if ((b >= 2) && (b < 14)) 
          {
            if (numberIndex < digits) 
            {
              numberBuffer[numberIndex] = keyLabel[b][0];
              numberIndex++;
              numberBuffer[numberIndex] = 0; // zero terminate
            }
          }

          // Del button, so delete last char
          if (b == 1) 
          {
            numberBuffer[numberIndex] = 0;
            if (numberIndex > 0) 
            {
              numberIndex--;
              numberBuffer[numberIndex] = 0;//' ';
            }
          }

        //Ent button
          if ((b == 14) && (numberIndex > 0)) 
          {
            done = true;
          }
          // we dont really check that the text field makes sense
          // just try to call
          if (b == 0) 
          {
            numberIndex = 0; // Reset index to 0
            numberBuffer[numberIndex] = 0; // Place null in buffer
          }

          // Update the number display field
          displayNumber(TFT_CYAN);

          delay(10); // UI debouncing
        }
      }
  } 

  return strtod(numberBuffer,NULL);  
}

void displayNumber(int colour)
{
    tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
    tft.setFreeFont(&FreeSans18pt7b);  // Choose a nice font that fits box
    // Draw the string, the value returned is the width in pixels
    tft.setTextColor(colour);
    int xwidth = tft.drawString(numberBuffer, DISP_X + 4, DISP_Y + 12);

    // Now cover up the rest of the line up by drawing a black rectangle.  No flicker this way
    // but it will not work with italic or oblique fonts due to character overlap.
    tft.fillRect(DISP_X + 4 + xwidth, DISP_Y + 1, DISP_W - xwidth - 5, DISP_H - 2, TFT_BLACK);     
}



//------------------------------------------------------------------------------------------

void drawKeypad()
{
  // Draw the keys
  for (uint8_t row = 0; row < 5; row++) 
  {
    for (uint8_t col = 0; col < 3; col++) 
    {
      uint8_t b = col + row * 3;

      if (b < 3) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);

      key[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X),
                        KEY_Y + row * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
                        KEY_W, KEY_H, TFT_WHITE, keyColor[b], TFT_WHITE,
                        keyLabel[b], KEY_TEXTSIZE);
      key[b].drawButton();
    }
  }
}

//------------------------------------------------------------------------------------------

