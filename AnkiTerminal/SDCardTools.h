#ifndef SDCardTools_h
#define SDCardTools_h

#include "Config.h"
#include <SdFat.h>
#include "MinLcd.h"
#include "LcdTools.h"
namespace SdCardTools {

  void WaitForSdCard(SdFat * sd)
  {
    LcdTools::writeFullscreenMessage(F(" Błąd karty SD"));
    
    while(!sd->begin(PIN_SD_CS, SPI_SD_MAX_SPEED))
    {
      delay(500);
    }
  }

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
