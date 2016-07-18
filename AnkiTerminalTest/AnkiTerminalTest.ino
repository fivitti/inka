#include <SPI.h>
#include <SdFat.h>

//#include "CSVFileTestPart1.h"
#include "CSVFileTestPart2.h"
//#include "CSVFileTestPart3.h"
//#include "CSVFileTestPart4.h"
//#include "CSVFileTestPart5.h"

void setup() {
  setupPinout();
  SPI.begin();  
  
  #if ! SPI_USE_TRANSACTION  
  SPI.setDataMode(0);
  SPI.setBitOrder(MSBFIRST);
  #endif //SPI_USE_TRANSACTION

  Serial.begin(9600);

  setupTest();
}

void loop() {
  Test::run();
}
