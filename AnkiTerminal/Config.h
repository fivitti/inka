#ifndef Config_h
#define Config_h

// -*-*-*- Suggest configuration of SdFat library -*-*-*-

// It is recommended configuration for this project.
// You don't need set it in SdFatConfig.h before compile project.
#include <SdFatConfig.h>
#undef USE_LONG_FILE_NAMES
#undef ARDUINO_FILE_USES_STREAM
#undef SD_SPI_CONFIGURATION
#undef USE_SD_CRC
#undef ENABLE_SPI_TRANSACTION
#undef ENABLE_SPI_YIELD
#undef FAT12_SUPPORT
#undef DESTRUCTOR_CLOSES_FILE
#undef ENDL_CALLS_FLUSH

#define USE_LONG_FILE_NAMES 0
#define ARDUINO_FILE_USES_STREAM 0
#define SD_SPI_CONFIGURATION 0  //Fast custom SPI configuration for SD card
#define USE_SD_CRC 0
#define ENABLE_SPI_TRANSACTION 1
#define ENABLE_SPI_YIELD 0
#define FAT12_SUPPORT 0
#define DESTRUCTOR_CLOSES_FILE 0
#define ENDL_CALLS_FLUSH 0
#include <SdFat.h>

// -*-*- Configuration CSVFile -*-*-

// It is recommended configuration for this project.
// You should set it in CSVFileConfig.h before compile project.

/*
 * #include <CSVFileConfig.h>
 * #define CSV_FILE_ENABLE_CHECK_EMPTY_LINE 0
 * #define CSV_FILE_ENABLE_GOTO_BEGIN_STARTS_WITH 0
 * #define CSV_FILE_ENABLE_DELETING_LINE 1
 * #define CSV_FILE_ENABLE_CHECK_OVERWRITE_ERROR 0
 * #include <CSVFile.h>
 */

// -*-*-*- Application config -*-*-*-

// SPI config
#define SPI_USE_TRANSACTION ENABLE_SPI_TRANSACTION
//Remember set SD config in SD FAT Config

// SPI pinout
#define PIN_SPI_CLK 13
#define PIN_SPI_MOSI 11
#define PIN_SPI_MISO 12

#define PIN_LCD_CS 3
#define PIN_SD_CS 10

// LCD pinout
#define PIN_LCD_RST 2
#define PIN_LCD_DC  4   
#define PIN_LCD_BACKLIGHT 5 //Must be PWM pin
#define PIN_LCD_CE PIN_LCD_CS
#define PIN_LCD_DIN PIN_SPI_MOSI
#define PIN_LCD_CLK PIN_SPI_CLK

//Switches pinout
#define PIN_BUTTON A2   //Analog pin

//Random pinout
#define PIN_RANDOM A0   //Not used analog pin

// LCD config
#define LCD_CONTRAST_DEFAULT 0xB5 //0-254
#define LCD_BACKLIGHT_DEFAULT 254 //0-254
#define SPI_LCD_MAX_SPEED 2000000

// SD config
// These filename should have maximum size @FILENAME_LIMIT_SIZE - 1= 12 in 8.3 format - 8 chars for name and 3 for extension. 1 for dot.
#define SPI_SD_MAX_SPEED SPI_FULL_SPEED //SPI_QUARTER_SPEED //SPI_HALF_SPEED //It is enum from SdFat
#define APPLICATION_DIR "inka"  //Directory in the root directory of SD. Here will be put config, session and progress files.
#define DICTIONARY_DIR "dicts"  //Directory in the root directory of SD. Should exist and contain only right dictionary files.
#define CONFIGURATION_FILENAME "config.csv" //This file contain all user settings.
#define SESSION_SET_FILENAME "sess.ion"     //File with state current session. It will be create before session and delete after.
#define TEMPORARY_SHUFFLE_FILENAME "shuffle.tmp" //File stored temporary shuffled result. It will be create at begin shuffling and replace with original at end.
#define FILENAME_LIMIT_SIZE 13 //For short filenames  //If you want change this limit you should change solution for open files and enabled support in SdFat. One place reserved for null byte.


// Buttons config
// You should measure your buttons and provide here correct values
// Button will be recognized when measure value will be equal or greater then LOW and less then HIGH level. 
#define DOWN_TRESHOLD_BUTTON 100  // Low pass filter. If value will be less then this value then will be recognized as not pressed button
#define LOW_MENU_BUTTON 675 // If value will be less then this value then will be recognized as unknown button.
#define HIGH_MENU_BUTTON 687
#define LOW_DOWN_BUTTON 695
#define HIGH_DOWN_BUTTON 707
#define LOW_MIDDLE_BUTTON 835
#define HIGH_MIDDLE_BUTTON 848
#define LOW_UP_BUTTON 980
#define HIGH_UP_BUTTON 1015 // If value will be greater then this value then will be recognized as unknown button.

// Other config
#define HUMAN_BUTTON_DELAY 200 //miliseconds //Human response time when single press button.
#define ENABLE_DEBUGGING_FUNCTIONS 0
#define HUMAN_ERROR_DELAY 3000 //miliseconds //Time it takes a human to read (single line) message on the LCD.

#ifdef _DEBUG
#undef _DEBUG
#endif //_DEBUG
#define _DEBUG 0

// Learn config - default values
// "Max card per session" parameter determine the maximum number card will be drawn for single session.
// The mechanism of the draw is very simply. Each card has two value of probability (one for each language)
// Algorithm goes through the dictionary and test each card one by one. It draws number (0-99) and compare with the probability.
// If draw number will be less then probability the card is selected.
// The algorithm searches dictionary only one times. (Hard task is check if card is selected when re-search).
// The search is end when number of drawing card is equals this parameter or end of dictonary.
// It may therefore happen, that the algoritm draw less number of cards then value this parameter.
#define MAX_CARD_PER_SESSION_DEFAULT 20
// "Draw mode" parameter determine which language is base language for cards.
// Each card has words in two language. One word is question and second is answer.
// User can choose by this parameter which word is question word.
// If value of this parameter is 0 the question language is first language in dictionary file.
// Else if value of this parameter is 1 the question language is second language in dictionary file.
// Else if value of this parameter is 2 the question lanugage is both first and second language.
// When the draw algorithm search the cards first draw which language will be tested and then test probability of this language.
#define DRAW_MODE_DEFAULT 2 // 0, 1 or 2
// "Repeat number" parameter determine how many times the card will be displayed to the user assuming that to rate all cards as medium.
// User can rate his knowledge displayed word to
//    good - if does not have any problems with word,
//    medium - if does not have big problems with word, but don't feel confident,
//    bad - if does not know the word.
#define REPEAT_NUMBER_DEFAULT 4
// "Decrease repeat when good" parameter determine while repeats decrease when user rate question as good.
// If user rate question as good then he know it and algorithm should display it rare.
#define DECREASE_REPEAT_WHEN_GOOD_DEFAULT 2
// "Increase repeat when bad" parameter determine while repeats increase when user rate question as bad.
// If user rate question as bas then he know it and algorithm should display it more often.
#define INCREASE_REPEAT_WHEN_BAD_DEFAULT 1
// "Decrease repeat when good" parameter determine while repeats decrease when user rate question as medium.
// If user rate question as medium then he know it and algorithm should display it less.
// I recommend that keep this value to 1.
#define DECREASE_REPEAT_WHEN_MEDIUM 1
// "Easy decrease probability" parameter determine how will change the chance of drawn the card when question
// will be rate always to good. When user rate whenever card as good then card should be draw rare.
#define EASY_DECREASE_PROBABILITY_DEFAULT 15
// "Medium decrease probability" parameter determine how will change the chance of drawn the card which was not
// easy or hard. This card should be draw less, but user needs to learn it yet.
#define MEDIUM_DECREASE_PROBABILITY_DEFAULT 5
// "Hard increase probability" parameter determine how will change the chance of drawn the card which was hard.
// User don't known this cards and it will be draw often.
#define HARD_INCREASE_PROBABILITY_DEFAULT 10
// "Forgoten increase probability" parameter determine how will change the chance of drawn the card which was
// not drawn in this session. This parameter simulates forgetfulness. Cards knowns a long time ago should
// come back sometimes for the remind.
#define FORGOTEN_INCREASE_PROBABILITY_DEFAUT 2
// "Hard when number bad answer when others normal" parameter detemine when card should be recognized as hard.
// If user rate card as bad as much or more times then this card is hard (assuming that the other rates will be normal).
#define CSV_FIELD_CONFIG_LEARN_SUMMARY_HARD_WHEN_NUMBER_BAD_ANSWER_WHEN_OTHERS_MEDIUM_DEFAULT 2
// "Begin probability" parameters define probability set in new progress files.
#define BEGIN_PROBABILITY_DEFAULT 99 //0-99%
//LCD defaults are up

//Constans for actions
// When user don't select any position (press MENU button) then will be return this value.
// Position is stored in byte variable.
#define NOT_SELECTED_POSITION 255
//Signal for break all process and return to root (with next main loop)
#define SHUTDOWN_ACTION NOT_SELECTED_POSITION
//Useful macro
#define RETURN_WHEN_SHUTDOWN(status_) if((status_)==SHUTDOWN_ACTION)return SHUTDOWN_ACTION;

void setupPinout() {
  pinMode(PIN_LCD_RST, OUTPUT);
  pinMode(PIN_LCD_CS, OUTPUT);
  pinMode(PIN_LCD_DC, OUTPUT);
  pinMode(PIN_LCD_BACKLIGHT, OUTPUT);
  pinMode(PIN_SD_CS, OUTPUT);
  pinMode(PIN_SPI_MOSI, OUTPUT);
  pinMode(PIN_SPI_MISO, INPUT);
  pinMode(PIN_SPI_CLK, OUTPUT);
  pinMode(PIN_BUTTON, INPUT);
  //Disable SPI devices
  digitalWrite(PIN_LCD_CS, HIGH);
  digitalWrite(PIN_SD_CS, HIGH);
}

#endif //Config.h





