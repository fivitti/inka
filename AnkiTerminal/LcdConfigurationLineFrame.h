#ifndef LcdConfigurationLineFrame_h
#define LcdConfigurationLineFrame_h

#include "IFrameBase.h"
#include "InstantEffectConfigurationFieldFrame.h"
#include "MinLcd.h"
#include "CSVSpecifications.h"

/*
* This class provide access to feature @InstantEffectConfigurationFieldFrame.
* It is simpliest and least use of resources solution.
* User can choose backlight and contrast of the screen. During user set the options see how screen is changed.
*/
class LcdConfigurationLineFrame : public IFrameBase
{
private:
  const __FlashStringHelper * m_strContrast;
  const __FlashStringHelper * m_strBacklight;

protected:

  virtual void onPositionSelect(byte position_) override
  {
    if (position_ == NOT_SELECTED_POSITION)
      return;
    
    IFrameBase * frame;

    if (position_ == CSV_FIELD_CONFIG_LCD_CONTRAST)
    {
      const ConfigurationData cd(m_strContrast, CSV_LINE_CONFIG_LCD, CSV_FIELD_CONFIG_LCD_CONTRAST, CSV_FIELD_CONFIG_LCD_CONTRAST_SIZE);
      frame = new InstantEffectConfigurationFieldFrame(&cd, &MinLcd::setupLcdContrast);
    }
    else if (position_ == CSV_FIELD_CONFIG_LCD_BACKLIGHT)
    {
      const ConfigurationData cd(m_strBacklight, CSV_LINE_CONFIG_LCD, CSV_FIELD_CONFIG_LCD_BACKLIGHT, CSV_FIELD_CONFIG_LCD_BACKLIGHT_SIZE);
      frame = new InstantEffectConfigurationFieldFrame(&cd, &MinLcd::setupLcdBacklight);
    }
    else
      return;

    frame->show();
    delete frame;

    show();
  }
  virtual void writePosition(byte index) override
  {
    switch (index)
    {
    case CSV_FIELD_CONFIG_LCD_CONTRAST:
      MinLcd::lcdWriteString(m_strContrast);
      break;
    case CSV_FIELD_CONFIG_LCD_BACKLIGHT:
      MinLcd::lcdWriteString(m_strBacklight);
      break;
    default:
      break;
    }
  }

public:
  LcdConfigurationLineFrame(const __FlashStringHelper * header) : IFrameBase()
  {
    m_numPositions = CSV_LINE_CONFIG_LCD_SIZE;
    m_strContrast = F(LANG_STR_CONTRAST);
    m_strBacklight = F(LANG_STR_BRIGHTNESS);
    m_header = header;
  }
  virtual ~LcdConfigurationLineFrame() {}

};

#endif //LcdConfigurationLineFrame_h
