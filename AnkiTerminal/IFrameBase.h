#ifndef FrameBase_h
#define FrameBase_h

#define HEADER_HEIGHT 1         //in rows
#define MAX_POSITIONS_ON_SCREEN 5
#define MENU_INDICATOR_WIDTH 1*CHAR_WIDTH  //in pixels - 6 pixel is width of char (with one empty pixel
#define INDICATOR '>'

#include <avr/pgmspace.h>
#include "Config.h"
#include "MinLcd.h"
#include "Buttons.h"
#include <MemoryFree.h>

class IFrameBase {
protected: 
  byte m_startPosition{0};
  virtual void onChangeIndicatorPosition(byte position) {}

private:
  void writeIndicator(byte position_) {
    clearArea(0, HEADER_HEIGHT, MENU_INDICATOR_WIDTH, LCD_ROW_NUM);
    lcdXY(0,HEADER_HEIGHT+position_);
    lcdWriteCharacter(INDICATOR);
  }

  void writePositions(byte positionStart) {
    clearArea(MENU_INDICATOR_WIDTH, HEADER_HEIGHT);
    //clearArea(12, 4);
    for (int i=positionStart; (i<numPositions) && ((i-positionStart) < MAX_POSITIONS_ON_SCREEN); i++)
    {
      lcdXY(MENU_INDICATOR_WIDTH, HEADER_HEIGHT + i - positionStart);
      writePosition(i);
    }
  }

  byte writeStartPosition(byte startPosition)
  {
    byte indicatorPosition = startPosition % MAX_POSITIONS_ON_SCREEN;
    writePositions(startPosition - indicatorPosition);
    writeIndicator(indicatorPosition);
    return indicatorPosition;
  }

  byte waitForSelectPosition() {
    byte indicatorPosition = writeStartPosition(m_startPosition);
    byte positionSelected = m_startPosition;

    waitForUnpress();

    while (true) {
      switch(waitForKey()) {
        case BUTTON_UP:
          if (indicatorPosition > 0) {
            indicatorPosition -= 1;
            positionSelected -= 1;
            onChangeIndicatorPosition(positionSelected);
          }
          else if (positionSelected > 0)
          {
            writePositions(positionSelected - MAX_POSITIONS_ON_SCREEN);
            indicatorPosition = MAX_POSITIONS_ON_SCREEN - 1;
            positionSelected -= 1;
            onChangeIndicatorPosition(positionSelected);
          }
          else if (positionSelected == 0)
          {
            writePositions(((numPositions - MAX_POSITIONS_ON_SCREEN + (numPositions % MAX_POSITIONS_ON_SCREEN)) / MAX_POSITIONS_ON_SCREEN) * MAX_POSITIONS_ON_SCREEN);
            indicatorPosition = numPositions % MAX_POSITIONS_ON_SCREEN == 0 ? MAX_POSITIONS_ON_SCREEN - 1 : (numPositions % MAX_POSITIONS_ON_SCREEN) - 1;
            positionSelected = numPositions - 1;
            onChangeIndicatorPosition(positionSelected);
          }
          break;
        case BUTTON_MIDDLE:
          return positionSelected;
        case BUTTON_DOWN:
          if (positionSelected == numPositions - 1)
          {
            writePositions(0);
            indicatorPosition = 0;
            positionSelected = 0;
            onChangeIndicatorPosition(positionSelected);
          }
          else if (indicatorPosition < MAX_POSITIONS_ON_SCREEN - 1)
          {
            indicatorPosition += 1;
            positionSelected += 1;
            onChangeIndicatorPosition(positionSelected);
          }
          else {
            writePositions(positionSelected + 1);
            indicatorPosition = 0;
            positionSelected += 1;
            onChangeIndicatorPosition(positionSelected);
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
    
protected:
//  IFrameBase * parent;
  byte numPositions=0;

  virtual void onPositionSelect(byte position_) = 0;
  virtual void writePosition(byte index) = 0;
  virtual void writeHeader()
  {
    lcdXY(0, 0);
  }
  void writeMsg(const char * message) {
    clearLcd();
    writeHeader();
    lcdXY(MENU_INDICATOR_WIDTH, HEADER_HEIGHT + 1);

    lcdWriteString(message);
  }

  void writeMsg(const __FlashStringHelper * message) {
    clearLcd();
    writeHeader();
    lcdXY(MENU_INDICATOR_WIDTH, HEADER_HEIGHT + 1);

    lcdWriteString(message);
  }

public:
  IFrameBase() /*: parent(parent)*/ {}
  virtual ~IFrameBase() {};
  virtual void setStartPosition(byte startPosition)
  {
    m_startPosition = startPosition;
  }
  virtual void show() {
    clearLcd();
    writeHeader();
    onPositionSelect(waitForSelectPosition());
//    onPositionSelect(2);
  }
};

#endif





