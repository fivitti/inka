#ifndef LcdConfigurationLineFrame_h
#define LcdConfigurationLineFrame_h

#include "IFrameBase.h"
#include "InstantEffectConfigurationFieldFrame.h"
#include "MinLcd.h"
#include "CSVSpecifications.h"

class LcdConfigurationLineFrame : public IFrameBase
{
private:
  const __FlashStringHelper * m_strContrast;
  const __FlashStringHelper * m_strBacklight;
  const __FlashStringHelper * m_header;

protected:

  virtual void onPositionSelect(byte position_) override
  {
    if (position_ == NOT_SELECTED_POSITION)
      return;
    
    IFrameBase * frame;

    if (position_ == CSV_FIELD_CONFIG_LCD_CONTRAST)
    {
      const ConfigurationData cd(m_strContrast, CSV_LINE_CONFIG_LCD, CSV_FIELD_CONFIG_LCD_CONTRAST, CSV_FIELD_CONFIG_LCD_CONTRAST_SIZE);
      frame = new InstantEffectConfigurationFieldFrame(&cd, &setupLcdContrast);
    }
    else if (position_ == CSV_FIELD_CONFIG_LCD_BACKLIGHT)
    {
      const ConfigurationData cd(m_strBacklight, CSV_LINE_CONFIG_LCD, CSV_FIELD_CONFIG_LCD_BACKLIGHT, CSV_FIELD_CONFIG_LCD_BACKLIGHT_SIZE);
      frame = new InstantEffectConfigurationFieldFrame(&cd, &setupLcdBacklight);
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
      lcdWriteString(F("Kontrast"));
      break;
    case CSV_FIELD_CONFIG_LCD_BACKLIGHT:
      lcdWriteString(F("Podświetlenie"));
      break;
    default:
      break;
    }
  }
  virtual void writeHeader()
  {
    IFrameBase::writeHeader();
    lcdWriteString(m_header);
  }

public:
  LcdConfigurationLineFrame(const __FlashStringHelper * header) : IFrameBase(),
    m_header(header)
  {
    numPositions = CSV_LINE_CONFIG_LCD_SIZE;
    m_strContrast = F("Kontrast");
    m_strBacklight = F("Podświetlenie");
  }
  virtual ~LcdConfigurationLineFrame() {}

};

#endif //LcdConfigurationLineFrame_h
