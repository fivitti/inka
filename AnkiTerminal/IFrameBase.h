#ifndef FrameBase_h
#define FrameBase_h

#include <avr/pgmspace.h>
#include "Config.h"
#include "MinLcd.h"
#include "Buttons.h"

#define HEADER_HEIGHT 1  //in rows
#define MAX_POSITIONS_ON_SCREEN (LCD_ROW_NUM-HEADER_HEIGHT)
#define MENU_INDICATOR_WIDTH (1*CHAR_WIDTH)  //in pixels - 6 pixel is width of char (with one empty pixel
#define INDICATOR '>'

/*
 * Base class for all frames.
 * Provide basic methods as display content and select position
 *
 * When you inherit form this class you must:
 * 1. Assign value to @m_header variable (best in constructor or override @show method) OR override @writeHeader method.
 * 2. Assign value to @m_numPositions (best in constructor or override @show method).
 * 3. Implement @onPositionSelect method.
 * 4. Implement @writePosition method.
 *
 * Also you can:
 * 1. Implement @onChangeIndicatorPosition if you need this functionality.
 * 2. Define custom operation before or after show frame overriding @show method.
 */
class IFrameBase {
protected:
  //This variable should be define in child class before call @show method this class.
  const __FlashStringHelper * m_header;
  //Use setter if you need set this value.
  byte m_startPosition{0};
  //This method is called when indicator change its positions (when user press UP and DOWN buttons).
  //This method is called with current selected position number (NOT with position indicatior).
  virtual void onChangeIndicatorPosition(byte position) { }

private:
  //Write indicator. This method should be called with number of position indicator (NOT with selected position).
  //Maximum @position_ is @MAX_POSITIONS_ON_SCREEN
  void writeIndicator(byte position_) {
    MinLcd::clearArea(0, HEADER_HEIGHT, MENU_INDICATOR_WIDTH, LCD_ROW_NUM);
    MinLcd::lcdXY(0,HEADER_HEIGHT+position_);
    MinLcd::lcdWriteCharacter(INDICATOR);
  }

  //Write positions in frame, start from @positionStart.
  void writePositions(byte positionStart) {
    MinLcd::clearArea(MENU_INDICATOR_WIDTH, HEADER_HEIGHT);
    for (int i=positionStart; (i<m_numPositions) && ((i-positionStart) < MAX_POSITIONS_ON_SCREEN); i++)
    {
      MinLcd::lcdXY(MENU_INDICATOR_WIDTH, HEADER_HEIGHT + i - positionStart);
      writePosition(i);
    }
  }

  // Find start parameters.
  // @startPosition - is index of position which should be selected at start
  // @writeFrom - out parameter with index of position which should be start writing.
  // @startIndicatorPosition - out parameter with index of indicator at start
  void findBeginIndexPositions(byte startPosition, byte& writeFrom, byte& startIndicatorPosition)
  {
    startIndicatorPosition = startPosition % MAX_POSITIONS_ON_SCREEN;
    writeFrom = startPosition - startIndicatorPosition;
  }

  //Method write positions until user don't choose position (press MIDDLE or MENU button).
  //After select method @onPositionSelect is called with index of selected position.
  //When inficator change position is called method @onChangeIndicatorPosition.
  //When indicator indicating to first position and user press UP button then algorithm display last position.
  //Similarly when indicator indicating to last position and user press DOWN button then algorithm display first position.
  //When user hold UP or DOWN button then position will be scroll.
  byte waitForSelectPosition() {
    byte indicatorPosition = 0;
    byte positionSelected = 0;

    findBeginIndexPositions(m_startPosition, positionSelected, indicatorPosition);

    writePositions(positionSelected);
    writeIndicator(indicatorPosition);

    positionSelected = m_startPosition;

    Buttons::waitForUnpress();
    
    while (true) {
      switch(Buttons::waitForKey()) {
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
            writePositions((m_numPositions / MAX_POSITIONS_ON_SCREEN) * MAX_POSITIONS_ON_SCREEN);
            indicatorPosition = (m_numPositions - 1) % MAX_POSITIONS_ON_SCREEN;
            positionSelected = m_numPositions - 1;
            onChangeIndicatorPosition(positionSelected);
          }
          break;
        case BUTTON_MIDDLE:
          return positionSelected;
        case BUTTON_DOWN:
          if (positionSelected == m_numPositions - 1)
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
        default:  //Undefined button
          break;
      }
      writeIndicator(indicatorPosition);
      delay(HUMAN_BUTTON_DELAY);
    }
  }
    
protected:
  //Don't forget assign this variable before call @show method from this class.
  byte m_numPositions=0;
  //Method called when user select position. @position_ is index of selected position.
  virtual void onPositionSelect(byte position_) = 0;
  //Method called when need write position. @index is index of position to write.
  //Pointer in LCD memory is set where should begin text.
  virtual void writePosition(byte index) = 0;
  //Standard implementation for write header.
  virtual void writeHeader()
  {
    MinLcd::lcdXY(0, 0);
    MinLcd::lcdWriteCenteredString(m_header);
  }

public:
  IFrameBase() {}
  virtual ~IFrameBase() {};
  //This method can be call for define selected position at start @show method. Default start position is 0.
  //This method must be call before call @show method.
  void setStartPosition(byte startPosition)
  {
    m_startPosition = startPosition;
  }
  //Method call for display content frame. After call user should select position. Then method @onPositionSelect is called.
  //After control is return to caller.
  //If you must perform specific operation to calculate parameters (e.g. set numPosition or find position) which can do in constructor
  //you can override this method, perform your operation and call this method.
  virtual void show() {
    MinLcd::clearLcd();
    writeHeader();

    if (m_numPositions != 0)
      onPositionSelect(waitForSelectPosition());
    else
      onPositionSelect(NOT_SELECTED_POSITION);
  }
};

#endif





