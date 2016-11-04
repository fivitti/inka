#include <SPI.h>
#include "Config.h"

#if DEBUG_OPTION_MEMORY
#include <MemoryFree.h>
#endif //DEBUG_OPTION_MEMORY

#include "MinLcd.h"
#include "LcdTools.h"
#include <SdFat.h>
#include <CSVFile.h>
#include "ConfigurationFile.h"
#include "SdCardTools.h"
#include "RootFrame.h"
#include "LearnFlow.h"

void setup() {  
  setupPinout();
  SPI.begin();

  randomSeed(analogRead(PIN_RANDOM));
  
  #if ! SPI_USE_TRANSACTION  
  SPI.setDataMode(0);
  SPI.setBitOrder(MSBFIRST);
  #endif //SPI_USE_TRANSACTION

  #if _DEBUG != DEBUG_OPTION_DISABLED 
  Serial.begin(9600); //<-- Disable for release
  while (!Serial);
  Serial.println(F("DEBUG MODE"));
  #endif //_DEBUG

  SdFat sd;
  CSVFile csvFile;

  //We must initialize LCD with default values, because we can
  //get error SD card and we must display message for user.
  MinLcd::setupLcd();
  
  SdCardTools::initSdCard(&sd);

  //It create new configuration file if not exist else do nothing
  ConfigurationFile::createConfigurationFile(&sd, &csvFile);
  
  //Right configuration LCD
  byte lcdConfiguration[CSV_LINE_CONFIG_LCD_SIZE];
  ConfigurationFile::readConfigurationLine(&csvFile, lcdConfiguration, CSV_LINE_CONFIG_LCD);

  MinLcd::setupLcd(lcdConfiguration[CSV_FIELD_CONFIG_LCD_CONTRAST], lcdConfiguration[CSV_FIELD_CONFIG_LCD_BACKLIGHT]);
}

void loop() {  
    //Show main menu
    RootFrame frame;
    frame.show();

    if (frame.getSelectedAction() != START_ACTION || !ENABLE_LEARN_FLOW)
      return;
    
    //Start learn program
    SdFat * sd = new SdFat();
    SdCardTools::initSdCard(sd);
    
    LearnFlow::performSessionLearn(sd);

    delete sd;
}

