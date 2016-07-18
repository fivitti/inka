#include <SPI.h>
#include "Config.h"
#include "MinLcd.h"
#include "LcdTools.h"
#include <SdFat.h>
#include <CSVFile.h>
//#include <MemoryFree.h> //Disable for release
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

  //Serial.begin(9600); //<-- Disable for release

  SdFat sd;
  CSVFile csvFile;

  setupLcd();
  
  SdCardTools::initSdCard(&sd);

  ConfigurationFile::createConfigurationFile(&sd, &csvFile);
  
  byte lcdConfiguration[CSV_LINE_CONFIG_LCD_SIZE];
  ConfigurationFile::readConfigurationLine(&csvFile, lcdConfiguration, CSV_LINE_CONFIG_LCD);

  setupLcd(lcdConfiguration[CSV_FIELD_CONFIG_LCD_CONTRAST], lcdConfiguration[CSV_FIELD_CONFIG_LCD_BACKLIGHT]);
}

void loop() {  
    RootFrame frame;
    frame.show();

    if (frame.getSelectedAction() != START_ACTION)
      return;
    
    SdFat * sd = new SdFat();
    SdCardTools::initSdCard(sd);
    
    LearnFlow::ankiMain(sd);

    delete sd;
}

