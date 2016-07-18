#ifndef minlcd_h
#define minlcd_h

#include <avr/pgmspace.h>
#include <SPI.h>
#include "Config.h"
#include "Font.h"

#define MAX_CHARS_IN_ROW 14
#define CHAR_WIDTH 6
#define LCD_COLUMN_NUM MAX_CHARS_IN_ROW * CHAR_WIDTH
#define LCD_ROW_NUM 6
#define START_NUMBER_PART_IN_ASCII 48

void lcdWriteData(const byte data)
{
  digitalWrite(PIN_LCD_DC, HIGH); //DC pin is low for commands
  digitalWrite(PIN_LCD_CE, LOW);
  
  SPI.transfer(data);
  
  digitalWrite(PIN_LCD_CE, HIGH);
}

void lcdWriteFromAscii(const char index)
{
  for(int i=0; i<5; i++) {
    lcdWriteData(pgm_read_byte(&(ASCII[index][i])));
  }
  lcdWriteData(0x00);
}

void lcdWriteCharacter(const char character)
{
  lcdWriteFromAscii(character - 0x20);
}

void lcdWriteCharacterByHash(const char hash) {
  byte index = 0;
  for (index=0; (index < EXTEND_ANSII_LENGTH) &&
       (((pgm_read_byte_near(EXTEND_ANSII_LOCKUP+index))) != hash); index++) ;

  lcdWriteFromAscii(index + 0x60);
}

void lcdWriteString(const char *characters)
{
  #if SPI_USE_TRANSACTION
  SPI.beginTransaction(SPISettings(SPI_LCD_MAX_SPEED, MSBFIRST, SPI_MODE0));
  #endif //SPI_USE_TRANSACTION

  while(*characters) {
    if (*characters < 0){
      lcdWriteCharacterByHash(ANSII_EXTEND_HASH(*characters++, *characters++));
    }
    else
      lcdWriteCharacter(*characters++);
  }
    
  #if SPI_USE_TRANSACTION
  SPI.endTransaction();
  #endif //SPI_USE_TRANSACTION
}

void lcdWriteString(const __FlashStringHelper * characters) {
  #if SPI_USE_TRANSACTION
  SPI.beginTransaction(SPISettings(SPI_LCD_MAX_SPEED, MSBFIRST, SPI_MODE0));
  #endif //SPI_USE_TRANSACTION
  const char * progmemCharacters = (const char PROGMEM *)characters;
  char c = pgm_read_byte(progmemCharacters++);
  while(c) {
    if (c < 0){
      //char c2 = pgm_read_byte(progmemCharacters++);
      lcdWriteCharacterByHash(ANSII_EXTEND_HASH(c, (char) pgm_read_byte(progmemCharacters++))); //Order is important
    }
    else {
      lcdWriteCharacter(c);
    }
        
    c = pgm_read_byte(progmemCharacters++); 
  }
    
  #if SPI_USE_TRANSACTION
  SPI.endTransaction();
  #endif //SPI_USE_TRANSACTION
}

void lcdWriteNumber(int number)
{
  #if SPI_USE_TRANSACTION
  SPI.beginTransaction(SPISettings(SPI_LCD_MAX_SPEED, MSBFIRST, SPI_MODE0));
  #endif //SPI_USE_TRANSACTION

  if (number == 0)
  {
    lcdWriteCharacter('0');
    return;
  }
  else if (number < 0)
  {
    lcdWriteCharacter('-');
    number = abs(number);
  }
  
  int powerResult = 1; // = pow(10, decimalPlace)
  while (number >= powerResult)
  {
    powerResult *= 10;
  }
  powerResult /= 10;
  
  while (powerResult > 0)
  {
      lcdWriteCharacter(START_NUMBER_PART_IN_ASCII + (number / powerResult));
      number = number % powerResult;
      powerResult /= 10;
  }
  
  #if SPI_USE_TRANSACTION
  SPI.endTransaction();
  #endif //SPI_USE_TRANSACTION
}

void lcdWriteCmd(const byte cmd)
{
  digitalWrite(PIN_LCD_DC, LOW); //DC pin is low for commands
  digitalWrite(PIN_LCD_CE, LOW);
  
  SPI.transfer(cmd);
  
  digitalWrite(PIN_LCD_CE, HIGH);
}
void lcdXY(const unsigned int x, const unsigned int y)
{
  #if SPI_USE_TRANSACTION
  SPI.beginTransaction(SPISettings(SPI_LCD_MAX_SPEED, MSBFIRST, SPI_MODE0));
  #endif //SPI_USE_TRANSACTION
  
  lcdWriteCmd(0x80 | x);  // Column.
  lcdWriteCmd(0x40 | y);  // Row.
  
  #if SPI_USE_TRANSACTION
  SPI.endTransaction();
  #endif //SPI_USE_TRANSACTION
}

void clearArea(const byte columnStart, const byte rowStart, const byte columnEnd, const byte rowEnd) {
  #if SPI_USE_TRANSACTION
  SPI.beginTransaction(SPISettings(SPI_LCD_MAX_SPEED, MSBFIRST, SPI_MODE0));
  #endif //SPI_USE_TRANSACTION
  
  for(byte col=columnStart; col<columnEnd; col++)
    for (byte row=rowStart; row<rowEnd; row++) {
      lcdWriteCmd(0x80 | col);  // Column.
      lcdWriteCmd(0x40 | row);  // Row.
      lcdWriteData(0x00);
  }
    
  #if SPI_USE_TRANSACTION
  SPI.endTransaction();
  #endif //SPI_USE_TRANSACTION
}

void clearArea(const byte columnStart, const byte rowStart) {
  clearArea( columnStart, rowStart, LCD_COLUMN_NUM, LCD_ROW_NUM);
}

void clearLcd()
{
  #if SPI_USE_TRANSACTION
  SPI.beginTransaction(SPISettings(SPI_LCD_MAX_SPEED, MSBFIRST, SPI_MODE0));
  #endif //SPI_USE_TRANSACTION
  
  for(int i=0; i<504; i++)
    lcdWriteData(0x00);
    
  #if SPI_USE_TRANSACTION
  SPI.endTransaction();
  #endif //SPI_USE_TRANSACTION
}

#if ENABLE_DEBUGGING_FUNCTIONS
void blackLcd() {
  #if SPI_USE_TRANSACTION
  SPI.beginTransaction(SPISettings(SPI_LCD_MAX_SPEED, MSBFIRST, SPI_MODE0));
  #endif //SPI_USE_TRANSACTION
  
  for(int i=0; i<504; i++)
    lcdWriteData(0xff);
    
  #if SPI_USE_TRANSACTION
  SPI.endTransaction();
  #endif //SPI_USE_TRANSACTION
}
#endif //ENABLE_DEBUGGING_FUNCTIONS

void setupLcdContrast(const byte contrast)
{
#if SPI_USE_TRANSACTION
  SPI.beginTransaction(SPISettings(SPI_LCD_MAX_SPEED, MSBFIRST, SPI_MODE0));
#endif //SPI_USE_TRANSACTION

  lcdWriteCmd(0x21);  // LCD extended commands
  lcdWriteCmd(contrast);  // set LCD Vop (contrast) //0xBF for red display
  lcdWriteCmd(0x04);  // set temp coefficent
  lcdWriteCmd(0x14);  // LCD bias mode 1:40
  lcdWriteCmd(0x20);  // LCD basic commands
  lcdWriteCmd(0x0C);  // LCD normal video

#if SPI_USE_TRANSACTION
  SPI.endTransaction();
#endif //SPI_USE_TRANSACTION
}

void setupLcdBacklight(const byte backlight)
{
  analogWrite(PIN_LCD_BACKLIGHT, backlight);
}

void setupLcd(const byte contrast=LCD_CONTRAST_DEFAULT, const byte backlight=LCD_BACKLIGHT_DEFAULT)
{
  digitalWrite(PIN_LCD_RST, LOW);
  digitalWrite(PIN_LCD_RST, HIGH);

  setupLcdContrast(contrast);
  setupLcdBacklight(backlight);
}

#endif  // minlcd.h





