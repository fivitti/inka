#ifndef DrawConfigurationLineFrame_h
#define DrawConfigurationLineFrame_h

#include "IFrameBase.h"
#include "ConfigurationLineFrame.h"
#include "ChooseLangModeFrame.h"
#include "CSVSpecifications.h"

class DrawConfigurationLineFrame : public ConfigurationLineFrame
{
protected:
  virtual void onPositionSelect(byte position_) override
  {
    if (position_ == NOT_SELECTED_POSITION)
      return;
    else if (position_ != CSV_FIELD_CONFIG_LEARN_DRAW_MODE)
    {
      ConfigurationLineFrame::onPositionSelect(position_);
    }
    else
    {
      IFrameBase * frame = new ChooseLangModeFrame(&m_datas[position_]);
      frame->show();
      delete frame;

      show();
    }
  }

public:
  DrawConfigurationLineFrame(const __FlashStringHelper * header, ConfigurationData * configurationDatas, byte configurationDatasSize) : ConfigurationLineFrame(header, configurationDatas, configurationDatasSize)
  {}
};

#endif // !DrawConfigurationLineFrame_h

