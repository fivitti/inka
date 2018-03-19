#include <SPI.h>
#include "Config.h"

#if _DEBUG & DEBUG_OPTION_MEMORY
#include <SdFatUtil.h>
#endif //DEBUG_OPTION_MEMORY

#if _DEBUG & DEBUG_OPTION_MINIMAL_SERIAL
#ifndef UDR0
#error "Minimal serial not allowed";
#endif //UDR0
#include <MinimumSerial.h>
MinimumSerial MiniSerial;
#endif //DEBUG_OPTION_MINIMAL_SERIAL
  
#include "MinLcd.h"
#include "LcdTools.h"
#include <SdFat.h>
#include <CSVFile.h>
#include "ConfigurationFile.h"
#include "SdCardTools.h"
#include "RootFrame.h"
#include "LearnFlow.h"

/*global*/ SdFat g_sd;

void setup() {  
  setupPinout();
  SPI.begin();

  randomSeed(analogRead(PIN_RANDOM));
  
  #if ! SPI_USE_TRANSACTION  
  SPI.setDataMode(0);
  SPI.setBitOrder(MSBFIRST);
  #endif //SPI_USE_TRANSACTION

  #if _DEBUG & DEBUG_OPTION_SERIAL 
  Serial.begin(9600); //<-- Disable for release
  while (!Serial);
  Serial.println(F("DEBUG MODE"));
  #endif //DEBUG_OPTION_SERIAL
  
  #if _DEBUG & DEBUG_OPTION_MINIMAL_SERIAL  
  MiniSerial.begin(9600);
  MiniSerial.println(F("DEBUG MODE"));
  #endif //DEBUG_OPTION_MINIMAL_SERIAL

  //We must initialize LCD with default values, because we can
  //get error SD card and we must display message for user.
  MinLcd::setupLcd();
  
  SdCardTools::initSdCard();

  CSVFile csvFile;
  //It create new configuration file if not exist else do nothing
  ConfigurationFile::createConfigurationFile(&csvFile);
  
  //Right configuration LCD
  byte lcdConfiguration[CSV_LINE_CONFIG_LCD_SIZE];
  ConfigurationFile::readConfigurationLine(&csvFile, lcdConfiguration, CSV_LINE_CONFIG_LCD);

  MinLcd::setupLcd(lcdConfiguration[CSV_FIELD_CONFIG_LCD_CONTRAST], lcdConfiguration[CSV_FIELD_CONFIG_LCD_BACKLIGHT]);
}

void loop() {
  #if ENABLE_ROOT_FRAME
    //Show main menu
    RootFrame frame;
    frame.show();

    if (frame.getSelectedAction() != START_ACTION)
      return;
  #endif //ENABLE_ROOT_FRAME
  #if ENABLE_LEARN_FLOW
    //Start learn program
    SdCardTools::initSdCard();
    
    LearnFlow::performSessionLearn();
  #endif //ENABLE_LEARN_FLOW
}

