#ifndef ChooseLangModeFrame_h
#define ChooseLangModeFrame_h

#include "Lang.h"
#include "ConfigurationFieldFrame.h"
#include "CSVSpecifications.h"
#include "MinLcd.h"

/*
 * This class provide user-friendly (not numbers) labels for
 * choose base language for drawing cards.
 */
class ChooseLangModeFrame : public ConfigurationFieldFrame
{
public:
  ChooseLangModeFrame(const ConfigurationData * configurationData) : ConfigurationFieldFrame(configurationData) {}
  ~ChooseLangModeFrame() {}

protected:
  virtual void writePosition(byte index) override
  {
    switch (index)
    {
    case DRAW_MODE_FIRST_LANG:
      MinLcd::lcdWriteString(F(LANG_STR_FIRST_LANG));
      break;
    case DRAW_MODE_SECOND_LANG:
      MinLcd::lcdWriteString(F(LANG_STR_SECOND_LANG));
      break;
    case DRAW_MODE_BOTH_LANG:
      MinLcd::lcdWriteString(F(LANG_STR_BOTH_RANDOM_LANG));
      break;
    default:
      break;
    }
  }
};

#endif // !ChooseLangModeFrame_h

