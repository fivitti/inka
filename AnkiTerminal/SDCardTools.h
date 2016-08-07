#ifndef SDCardTools_h
#define SDCardTools_h

#include "Config.h"
#include <SdFat.h>
#include "MinLcd.h"
#include "LcdTools.h"
#include "Lang.h"
//Utilites for work with SD Card
namespace SdCardTools {
  
  //Block the main thread until user don't insert the SD card.
  void WaitForSdCard(SdFat * sd)
  {
    LcdTools::writeFullscreenMessage(F(LANG_STR_SD_CARD_ERROR_MESSAGE));
    
    while(!sd->begin(PIN_SD_CS, SPI_SD_MAX_SPEED))
    {
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
