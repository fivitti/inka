#ifndef ChooseLangModeFrame_h
#define ChooseLangModeFrame_h

#include "ConfigurationFieldFrame.h"
#include "CSVSpecifications.h"
#include "MinLcd.h"

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
      lcdWriteString(F("1. język"));
      break;
    case DRAW_MODE_SECOND_LANG:
      lcdWriteString(F("2. język"));
      break;
    case DRAW_MODE_BOTH_LANG:
      lcdWriteString(F("Losowo oba"));
      break;
    default:
      break;
    }
  }

};

#endif // !ChooseLangModeFrame_h

