#ifndef SDCardTools_h
#define SDCardTools_h

#include "Config.h"
#include <SdFat.h>
#include "MinLcd.h"
#include "LcdTools.h"
#include "Lang.h"
//Utilites for work with SD Card
namespace SdCardTools {
  
  void WriteSdError(SdFat * sd)
  {
    int errorCode = sd->card()->errorCode();

    const __FlashStringHelper * error;

    if (errorCode == SD_CARD_ERROR_CMD0)
    {
      error = F(LANG_STR_SD_CARD_MISSING);
    }
    else if (sd->vol()->fatType() == 0)
    {
      error = F(LANG_STR_SD_CARD_NEED_FORMAT);
    }
    else if (sd->vwd()->isOpen())
    {
      error = F(LANG_STR_SD_CARD_ACCESS_DENIED);
    }
    else
    {
      error = F(LANG_STR_SD_CARD_ERROR_MESSAGE);
    }

    LcdTools::writeFullscreenMessage(error);
  }

  //Block the main thread until user don't insert the SD card.
  void WaitForSdCard(SdFat * sd)
  {
    while(!sd->begin(PIN_SD_CS, SPI_SD_MAX_SPEED))
    {
      WriteSdError(sd);

      #if _DEBUG
      sd->errorPrint();
      sd->initErrorPrint();
      #endif //_DEBUG
      delay(500);
    }
  }

  //Perform initialization SD card.
  void initSdCard(SdFat * sd)
  {
    digitalWrite(PIN_LCD_CS, HIGH);
    if (!sd->begin(PIN_SD_CS, SPI_SD_MAX_SPEED))
    {
      SdCardTools::WaitForSdCard(sd);
    }
  }

}
#endif //SDCardtools_h
