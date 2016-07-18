#ifndef Config_h
#define Config_h

#include <SdFat.h>

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
#define PIN_BUTTON A3

//Random pinout
#define PIN_RANDOM A0

// LCD config
#define LCD_CONTRAST_DEFAULT 0xB5 //0-254
#define LCD_BACKLIGHT_DEFAULT 254 //0-254
#define SPI_LCD_MAX_SPEED 2000000

// SD config
#define SPI_SD_MAX_SPEED SPI_FULL_SPEED //SPI_QUARTER_SPEED //SPI_HALF_SPEED
#define APPLICATION_DIR "inka"
#define DICTIONARY_DIR "dicts"
#define CONFIGURATION_FILENAME "config.csv"
#define SESSION_SET_FILENAME "sess.ion"
#define FILENAME_LIMIT_SIZE 13 //For short filenames

// SPI config
#define SPI_USE_TRANSACTION 1
//Remember set SD config in SD FAT Config

// Buttons config
#define DOWN_TRESHOLD_BUTTON 100
#define LOW_MENU_BUTTON 675
#define HIGH_MENU_BUTTON 687
#define LOW_DOWN_BUTTON 695
#define HIGH_DOWN_BUTTON 707
#define LOW_MIDDLE_BUTTON 835
#define HIGH_MIDDLE_BUTTON 848
#define LOW_UP_BUTTON 980
#define HIGH_UP_BUTTON 1015

// Other config
#define HUMAN_BUTTON_DELAY 200 //miliseconds
#define ENABLE_DEBUGGING_FUNCTIONS 0
#define HUMAN_ERROR_DELAY 3000

// Learn config - default values
#define MAX_CARD_PER_SESSION_DEFAULT 20
#define DRAW_MODE_DEFAULT 2 // 0, 1 or 2

#define REPEAT_NUMBER_DEFAULT 4
#define DECREASE_REPEAT_WHEN_GOOD_DEFAULT 2
#define INCREASE_REPEAT_WHEN_BAD_DEFAULT 1
#define DECREASE_REPEAT_WHEN_MEDIUM 1

#define EASY_DECREASE_PROBABILITY_DEFAULT 15
#define MEDIUM_DECREASE_PROBABILITY_DEFAULT 5
#define HARD_INCREASE_PROBABILITY_DEFAULT 10
#define FORGOTEN_INCREASE_PROBABILITY_DEFAUT 2
#define CSV_FIELD_CONFIG_LEARN_SUMMARY_HARD_WHEN_NUMBER_BAD_ANSWER_WHEN_OTHERS_NORMAL_DEFAULT 2
#define BEGIN_PROBABILITY_DEFAULT 50 //0-99%
//LCD defaults are up

//Constans for actions
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





