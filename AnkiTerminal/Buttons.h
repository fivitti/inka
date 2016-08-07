#ifndef Buttons_h
#define Buttons_h

#include "Config.h"

//Number all real buttons
#define NUM_BUTTONS 4

#define BUTTON_UP 3
#define BUTTON_MIDDLE 2
#define BUTTON_DOWN 1
#define BUTTON_MENU 0

//Return when error reading from analog port
#define BUTTON_UNDEFINED 4
#define BUTTON_NOT_PRESSED 5

#define ANY_BUTTON_PRESSED(state) (state != BUTTON_NOT_PRESSED)

/*
 * Set of function for work with some buttons connected to single analog pin.
 * Buttons are voltage divider.
 *
 *        ____________________________________________________________________________________[100k]________GND
 *        |               |                   |                 |                 |
 *  [BUTTON_UP]     [BUTTON_MIDDLE]     [BUTTON_DOWN]     [BUTTON_MENU]     [ANALOG_PIN]
 *        |               |                   |                 |               
 *      [1k]            [22k]               [47k]             [51k]
 *        |_______________|___________________|_________________|___________________________________________VCC
 *        
 */
namespace Buttons
{
  byte getButtonNumber(int reading) {
    if (reading < DOWN_TRESHOLD_BUTTON)
      return BUTTON_NOT_PRESSED;
    else if (reading < LOW_MENU_BUTTON)
      return BUTTON_UNDEFINED;
    else if (reading < HIGH_MENU_BUTTON)
      return BUTTON_MENU;
    else if (reading < LOW_DOWN_BUTTON)
      return BUTTON_UNDEFINED;
    else if (reading < HIGH_DOWN_BUTTON)
      return BUTTON_DOWN;
    else if (reading < LOW_MIDDLE_BUTTON)
      return BUTTON_UNDEFINED;
    else if (reading < HIGH_MIDDLE_BUTTON)
      return BUTTON_MIDDLE;
    else if (reading < LOW_UP_BUTTON)
      return BUTTON_UNDEFINED;
    else if (reading < HIGH_UP_BUTTON)
      return BUTTON_UP;
    else
      return BUTTON_UNDEFINED;
  }

  byte getButtonState() {
    return getButtonNumber(analogRead(PIN_BUTTON));
  }

  void waitForUnpress() {
    byte state = getButtonState();
    while (ANY_BUTTON_PRESSED(state)) {
      delay(10);
      state = getButtonState();
    }
  }

  byte waitForKey() {
    byte state = getButtonState();

    while (!ANY_BUTTON_PRESSED(state)) {
      delay(10);
      state = getButtonState();
    }
    return state;
  }
}

#undef NUM_BUTTONS
#undef ANY_BUTTON_PRESSED
#endif //Buttons.h





