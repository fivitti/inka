#ifndef LcdTools_h
#define LcdTools_h

#include "MinLcd.h"
#include "Lang.h"

#define FULLSCREEN_HEADER LANG_STR_APPLICATION_TITLE_POPUP
//In this row will be write content of message (first line)
#define FULLSCREEN_MESSAGE_ROW 2
#define FULLSCREEN_HEADER_HEIGHT 1 //Coresponding to IFrameBase

// Simply util to fast display fullscreen message for user.
namespace LcdTools
{
  //Write fullscreen message with header and centered text message in line @FULLSCREEN_MESSAGE_ROW.
  //If @keepHeader is false the current header will be clear and will be write default header @FULLSCREEN_HEADER.
  void writeFullscreenMessage(const __FlashStringHelper * message, bool keepHeader=false)
  {
    if (keepHeader)
    {
      MinLcd::clearArea(0, FULLSCREEN_HEADER_HEIGHT);
    }
    else {
      MinLcd::clearLcd();
      MinLcd::lcdXY(0, 0);
      MinLcd::lcdWriteCenteredString(F(FULLSCREEN_HEADER));
    }
     
    MinLcd::lcdXY(0,FULLSCREEN_MESSAGE_ROW);
    MinLcd::lcdWriteCenteredString(message);
  }
}

#undef FULLSCREEN_MESSAGE_ROW
#undef FULLSCREEN_HEADER
#undef FULLSCREEN_HEADER_HEIGHT
#endif //LcdTools_h
