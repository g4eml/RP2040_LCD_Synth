// Full-screen list of chip types to choose from, replacing the old row of
// individually hand-positioned buttons in ConfigScreen.ino. This list needs no
// changes when a new chip type is added - the number of buttons, their layout,
// and their labels are all generated from NUM_CHIP_TYPES/chipTypeName(), which
// in turn come from CHIP_LIST in ChipList.h. See the note at the top of
// SynthChip.h for the full process of adding a new chip type.

#define CHIPPAD_X 40                  //left edge of the button column
#define CHIPPAD_W 400                 //button width
#define CHIPPAD_TOP 20                //top of the available list area
#define CHIPPAD_BOTTOM 300            //bottom of the available list area
#define CHIPPAD_SPACING 10            //vertical gap between buttons
#define CHIPPAD_FONT &FreeSansBold12pt7b

LGFX_Button chipKey[NUM_CHIP_TYPES - 1];     //one button per real chip type (excludes NONE)

//------------------------------------------------------------------------------------------
uint8_t doChipSelect(void)
{
  uint8_t sel = chip;
  int n = NUM_CHIP_TYPES - 1;                    //number of selectable chip types
  int keyH = (CHIPPAD_BOTTOM - CHIPPAD_TOP - (n - 1) * CHIPPAD_SPACING) / n;

  tft.fillRect(0, 0, 480, 320, TFT_DARKGREY);
  tft.setFont(CHIPPAD_FONT);

  for(uint8_t i = 0; i < n; i++)
  {
    chipKey[i].initButton(&tft, CHIPPAD_X + CHIPPAD_W / 2,
                      CHIPPAD_TOP + i * (keyH + CHIPPAD_SPACING) + keyH / 2, // x, y (centre), w, h, outline, fill, text
                      CHIPPAD_W, keyH, TFT_WHITE, (i + 1 == chip) ? TFT_DARKGREEN : TFT_BLUE, TFT_WHITE,
                      "", 1);
    chipKey[i].drawButton(0, chipTypeName(i + 1));
  }

  bool done = false;
  while(!done)
  {
    // Pressed will be set true if there is a valid touch on the screen
    bool pressed = getTouchDebounced(&t_x, &t_y);

    // Check if any key coordinate boxes contain the touch coordinates
    for(uint8_t b = 0; b < n; b++)
    {
      if(pressed && chipKey[b].contains(t_x, t_y))
      {
        chipKey[b].press(true);   // tell the button it is pressed
      }
      else
      {
        chipKey[b].press(false);  // tell the button it is NOT pressed
      }
    }

    // Check if any key has changed state
    for(uint8_t b = 0; b < n; b++)
    {
      if(chipKey[b].justPressed())
      {
        sel = b + 1;
        done = true;
        delay(10);  // UI debouncing
      }
    }
  }

  touchConsumed = true;   // caller's next touch poll will ignore this same physical touch until it's released

  return sel;
}
