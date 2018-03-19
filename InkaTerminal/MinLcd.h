#ifndef minlcd_h
#define minlcd_h

#include <avr/pgmspace.h>
#include <SPI.h>
#include "Config.h"
#include "Font.h"

#define MAX_CHARS_IN_ROW 14
#define CHAR_WIDTH 6
#define LCD_COLUMN_NUM (MAX_CHARS_IN_ROW * CHAR_WIDTH)
#define LCD_ROW_NUM 6
#define START_NUMBER_PART_IN_ASCII 48

/* 
 * Minimal library for work with Nokia 5110 LCD.
 */
namespace MinLcd
{
  /***************************
   ***   PRIVATE METHODS   ***
   ***************************/

  //Private method.
  //@data is one vertical line of character to write.
  //Each character has six vertical lines (and eight horizontal lines)
  void _lcdWriteData(const byte data)
  {
    digitalWrite(PIN_LCD_DC, HIGH); //DC pin is low for commands
    digitalWrite(PIN_LCD_CE, LOW);

    SPI.transfer(data);

    digitalWrite(PIN_LCD_CE, HIGH);
  }

  //Private method.
  //Write characters form standard ASCII table.
  void _lcdWriteFromAscii(const char index)
  {
    for (int i = 0; i < 5; i++) {
      _lcdWriteData(pgm_read_byte(&(ASCII[index][i])));
    }
    _lcdWriteData(0x00);
  }

  //Private method, candidate for public
  //Write not standard character by hash (from "extend ASCII table")
  //You should use @ASCII_EXTEND_HASH function for get hash.
  //Not ASCII characters are stored as pair of bytes, therefore you must use hash.
  //If hash was not found then method don't write any character.
  void _lcdWriteCharacterByHash(const char hash) {
    byte index = 0;
    for (index = 0; (index < EXTEND_ASCII_LENGTH) &&
      (((pgm_read_byte_near(EXTEND_ASCII_LOCKUP + index))) != hash); index++);

      _lcdWriteFromAscii(index + EXTEND_ASCII_TABLE_START_SHIFT);
  }

  //Private method.
  //Write command to LCD controler.
  void _lcdWriteCmd(const byte cmd)
  {
    digitalWrite(PIN_LCD_DC, LOW); //DC pin is low for commands
    digitalWrite(PIN_LCD_CE, LOW);

    SPI.transfer(cmd);

    digitalWrite(PIN_LCD_CE, HIGH);
  }

  /***************************
   ***    PUBLIC METHODS   ***
   ***************************/

  //Public method
  //Write standard character (from ASCII table, not from "extend ASCII" table)
  void lcdWriteCharacter(const char character)
  {
    _lcdWriteFromAscii(character - ASCII_TABLE_START_SHIFT);
  }

  //Public method
  //Method write string. String can contain character from "extend ASCII" table.
  void lcdWriteString(const char *characters)
  {
#if SPI_USE_TRANSACTION
    SPI.beginTransaction(SPISettings(SPI_LCD_MAX_SPEED, MSBFIRST, SPI_MODE0));
#endif //SPI_USE_TRANSACTION

    while (*characters) {
      if (*characters < 0) {
        _lcdWriteCharacterByHash(ASCII_EXTEND_HASH(*characters++, *characters++));
      }
      else
        lcdWriteCharacter(*characters++);
    }

#if SPI_USE_TRANSACTION
    SPI.endTransaction();
#endif //SPI_USE_TRANSACTION
  }

  //Public method
  //Method write string stored in flash memory. String can contain character from "extend ASCII" table.
  void lcdWriteString(const __FlashStringHelper * characters) {
#if SPI_USE_TRANSACTION
    SPI.beginTransaction(SPISettings(SPI_LCD_MAX_SPEED, MSBFIRST, SPI_MODE0));
#endif //SPI_USE_TRANSACTION
    const char * progmemCharacters = (const char PROGMEM *)characters;
    char c = pgm_read_byte(progmemCharacters++);
    while (c) {
      if (c < 0) {
        //char c2 = pgm_read_byte(progmemCharacters++);
        _lcdWriteCharacterByHash(ASCII_EXTEND_HASH(c, (char)pgm_read_byte(progmemCharacters++))); //Order is important
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

  //Public method
  //Method write number on the screen. It support sign '-'.
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

  //Public method
  //Set screen pointer to (@x, @y) position, where
  //@x is column and @y is row. Next character will be
  //write in this position.
  //Coordinate @x in pixels unit (for chars you should multiply them by @CHAR_WIDTH)
  //Coordinate @y in char unit. (Between @y row and @y+1 row are pixels number equals height of single char)
  void lcdXY(const byte x, const byte y)
  {
#if SPI_USE_TRANSACTION
    SPI.beginTransaction(SPISettings(SPI_LCD_MAX_SPEED, MSBFIRST, SPI_MODE0));
#endif //SPI_USE_TRANSACTION

    _lcdWriteCmd(0x80 | x);  // Column.
    _lcdWriteCmd(0x40 | y);  // Row.

#if SPI_USE_TRANSACTION
    SPI.endTransaction();
#endif //SPI_USE_TRANSACTION
  }

  //Public method
  //Clear area screen.
  //Area not include @rowEnd and @columnEnd.
  void clearArea(const byte columnStart, const byte rowStart, const byte columnEnd, const byte rowEnd) {
#if SPI_USE_TRANSACTION
    SPI.beginTransaction(SPISettings(SPI_LCD_MAX_SPEED, MSBFIRST, SPI_MODE0));
#endif //SPI_USE_TRANSACTION

    for (byte col = columnStart; col < columnEnd; col++)
      for (byte row = rowStart; row < rowEnd; row++) {
        _lcdWriteCmd(0x80 | col);  // Column.
        _lcdWriteCmd(0x40 | row);  // Row.
        _lcdWriteData(0x00);
      }

#if SPI_USE_TRANSACTION
    SPI.endTransaction();
#endif //SPI_USE_TRANSACTION
  }

  //Public method
  //Clear area screen.
  //Screem is cleared from (@columnStart, @rowStart) to right down corner of screen.
  void clearArea(const byte columnStart, const byte rowStart) {
    clearArea(columnStart, rowStart, LCD_COLUMN_NUM, LCD_ROW_NUM);
  }

  //Public method.
  //Clear all screen.
  void clearLcd()
  {
#if SPI_USE_TRANSACTION
    SPI.beginTransaction(SPISettings(SPI_LCD_MAX_SPEED, MSBFIRST, SPI_MODE0));
#endif //SPI_USE_TRANSACTION

    for (int i = 0; i < LCD_COLUMN_NUM * LCD_ROW_NUM; i++)
      _lcdWriteData(0x00);

#if SPI_USE_TRANSACTION
    SPI.endTransaction();
#endif //SPI_USE_TRANSACTION
  }

  //Public method.
  //Set contrast of screen. You should choose correct value for the best quality.
  //This value is different for each factory screen.
  void setupLcdContrast(const byte contrast)
  {
#if SPI_USE_TRANSACTION
    SPI.beginTransaction(SPISettings(SPI_LCD_MAX_SPEED, MSBFIRST, SPI_MODE0));
#endif //SPI_USE_TRANSACTION

    _lcdWriteCmd(0x21);  // LCD extended commands
    _lcdWriteCmd(contrast);  // set LCD Vop (contrast) //0xBF for red display
    _lcdWriteCmd(0x04);  // set temp coefficent
    _lcdWriteCmd(0x14);  // LCD bias mode 1:40
    _lcdWriteCmd(0x20);  // LCD basic commands
    _lcdWriteCmd(0x0C);  // LCD normal video

#if SPI_USE_TRANSACTION
    SPI.endTransaction();
#endif //SPI_USE_TRANSACTION
  }

  //Public method
  //Setup backlight of screen. 0 - for disable backlight.
  void setupLcdBacklight(const byte backlight)
  {
    analogWrite(PIN_LCD_BACKLIGHT, backlight);
  }

  //Public method.
  //Setup pinout of the screen.
  void setupLcd(const byte contrast = LCD_CONTRAST_DEFAULT, const byte backlight = LCD_BACKLIGHT_DEFAULT)
  {
    digitalWrite(PIN_LCD_RST, LOW);
    digitalWrite(PIN_LCD_RST, HIGH);

    setupLcdContrast(contrast);
    setupLcdBacklight(backlight);
  }
}
#endif  // minlcd.h





