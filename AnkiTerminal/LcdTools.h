#ifndef LcdTools_h
#define LcdTools_h

#include "MinLcd.h"

#define FULLSCREEN_HEADER "     INKA"
#define FULLSCREEN_MESSAGE_ROW 2

namespace LcdTools
{

  void writeFullscreenMessage(const __FlashStringHelper * message)
  {
      clearLcd();
      lcdXY(0,0);
      lcdWriteString(F(FULLSCREEN_HEADER));
      lcdXY(0,FULLSCREEN_MESSAGE_ROW);
      lcdWriteString(message);
  }

  void writeFullscreenMessage(const __FlashStringHelper * firstLine, const char * secondLine)
  {
    writeFullscreenMessage(firstLine);
    lcdXY(0, FULLSCREEN_MESSAGE_ROW + 1);
    lcdWriteString(secondLine);
  }

  void writeBlankSpace(byte size_)
  {
    while (size_-- > 0)
      lcdWriteCharacter(' ');
  }
}

#undef FULLSCREEN_MESSAGE_ROW
#undef FULLSCREEN_HEADER
#endif //LcdTools_h
