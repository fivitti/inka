#ifndef LcdTools_h
#define LcdTools_h

#include "MinLcd.h"
#include "Lang.h"
#include <avr/pgmspace.h>
#include "Tools.h"

#define FULLSCREEN_HEADER LANG_STR_APPLICATION_TITLE_POPUP
//In this row will be write content of message (first line)
#define FULLSCREEN_MESSAGE_ROW 2
#define FULLSCREEN_HEADER_HEIGHT 1 //Coresponding to IFrameBase

// Simply util to fast display fullscreen message for user.
namespace LcdTools
{

  //Public method
  //Write spaces. Number of spaces is equal @size_.
  void writeBlankSpaces(byte size_)
  {
    while (size_-- > 0)
      MinLcd::lcdWriteCharacter(' ');
  }

  //Public method
  //Method write string. String can contain character from "extend ASCII" table.
  //String is centered in the line. Width of the line is @maxSize.
  void lcdWriteCenteredString(const char * characters, const byte maxSize = MAX_CHARS_IN_ROW)
  {
    writeBlankSpaces(StringTools::calculateShiftString(characters, maxSize) >> 1);
    MinLcd::lcdWriteString(characters);
  }

  //Public method
  //Method write string stored in flash memory. String can contain character from "extend ASCII" table.
  //String is centered in the line. Width of the line is @maxSize.
  void lcdWriteCenteredString(const __FlashStringHelper * characters, const byte maxSize = MAX_CHARS_IN_ROW)
  {
    writeBlankSpaces(StringTools::calculateShiftString(characters, maxSize) >> 1);
    MinLcd::lcdWriteString(characters);
  }

  //Write fullscreen message with header and centered text message in line @FULLSCREEN_MESSAGE_ROW.
  //If @keepHeader is false the current header will be clear and will be write default header @FULLSCREEN_HEADER.
  void writeFullscreenMessage(const __FlashStringHelper * message, const bool keepHeader=false)
  {
    if (keepHeader)
    {
      MinLcd::clearArea(0, FULLSCREEN_HEADER_HEIGHT);
    }
    else {
      MinLcd::clearLcd();
      MinLcd::lcdXY(0, 0);
      lcdWriteCenteredString(F(FULLSCREEN_HEADER));
    }
     
    MinLcd::lcdXY(0,FULLSCREEN_MESSAGE_ROW);
    lcdWriteCenteredString(message);
  }
}

#undef FULLSCREEN_HEADER
#undef FULLSCREEN_HEADER_HEIGHT
#endif //LcdTools_h
