#ifndef PercentCounter_h
#define PercentCounter_h

#include "MinLcd.h"
#include "Tools.h"

#define MAX_DISPLAY_VALUE 100
#define SYMBOL_PERCENT '%'
#define SYMBOL_PERCENT_SIZE 1
#define PERCENT_COUNTER_ROW 3
#define COLUMN_PERCENT_SYMBOL (MAX_CHARS_IN_ROW/2)

class PercentCounter
{
  private:
  byte m_step;
  byte m_currentTick{0};
  byte m_nextValueInterval;
  
  public:
  PercentCounter(unsigned int totalTicks) : m_step(MAX_DISPLAY_VALUE / totalTicks), m_nextValueInterval( {
    lcdXY((COLUMN_PERCENT_SYMBOL - SYMBOL_PERCENT_SIZE) * CHAR_WIDTH, PERCENT_COUNTER_ROW);
    lcdWriteCharacter('0');
    lcdWriteCharacter(SYMBOL_PERCENT);
  }
  ~PercentCounter() {};

  void tick()
  {
    m_currentValue += m_step;
    byte currentInt = (byte)round(m_currentValue);
    lcdXY((COLUMN_PERCENT_SYMBOL - NumberTools::numberOfDigits(currentInt)) * CHAR_WIDTH, PERCENT_COUNTER_ROW);
    lcdWriteNumber(currentInt);
    lcdWriteCharacter(SYMBOL_PERCENT);
  }
};

#undef MAX_DISPLAY_VALUE
#undef SYMBOL_PERCENT
#endif //PercentCounter_h

