#ifndef Menu_h
#define Menu_h

#include "Config.h"
#include "MinLcd.h"
#include "Buttons.h"

#define NOT_SELECTED_POSITION 255
#define HEADER_HEIGHT 1         //in rows
#define MAX_POSITIONS_ON_SCREEN 5
#define MENU_INDICATOR_WIDTH 1*CHAR_WIDTH  //in pixels - 6 pixel is width of char (with one empty pixel
#define INDICATOR '>'

/***********************************
 ***********************************
 ***                             ***
 ***          DEPRECATED         ***
 ***                             ***
 ***********************************
 ***********************************/
void writeHeader(char * header) {
  lcdXY(0,0);
  lcdWriteString(header);
}

void writeIndicator(byte position_) {
  clearArea(0, HEADER_HEIGHT, MENU_INDICATOR_WIDTH, LCD_ROW_NUM);
  lcdXY(0,HEADER_HEIGHT+position_);
  lcdWriteCharacter(INDICATOR);
}

void writePositions(char ** positions, byte positionStart, byte numPositions) {
  clearArea(MENU_INDICATOR_WIDTH, HEADER_HEIGHT);
  //clearArea(12, 4);
  for (int i=positionStart; (i<numPositions) && ((i-positionStart) < MAX_POSITIONS_ON_SCREEN); i++)
  {
    lcdXY(MENU_INDICATOR_WIDTH, HEADER_HEIGHT + i - positionStart);
    lcdWriteString(positions[i]);
  }
}

byte waitForSelectPosition(char ** positions, byte numPositions) {
  byte indicatorPosition = 0;
  byte positionSelected = 0;
  writePositions(positions, 0, numPositions);
  writeIndicator(indicatorPosition);
  while (true) {
    switch(waitForKey()) {
      case BUTTON_UP:
        if (indicatorPosition > 0) {
          indicatorPosition -= 1;
          positionSelected -= 1;
        }
        else if (positionSelected > 0)
        {
          writePositions(positions, positionSelected - MAX_POSITIONS_ON_SCREEN, numPositions);
          indicatorPosition = MAX_POSITIONS_ON_SCREEN - 1;
          positionSelected -= 1;
        }
        break;
      case BUTTON_MIDDLE:
        return positionSelected;
      case BUTTON_DOWN:
        if (positionSelected == numPositions - 1)
          break;
        else if (indicatorPosition < MAX_POSITIONS_ON_SCREEN - 1)
        {
          indicatorPosition += 1;
          positionSelected += 1;
        }
        else {
          writePositions(positions, positionSelected + 1, numPositions);
          indicatorPosition = 0;
          positionSelected += 1;
        }
        break;
      case BUTTON_MENU:
        return NOT_SELECTED_POSITION;
      default:
        break;
    }
    writeIndicator(indicatorPosition);
    delay(HUMAN_BUTTON_DELAY);
  }
}

#endif





