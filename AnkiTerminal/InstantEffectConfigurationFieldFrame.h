#ifndef InstantEffectConfigurationFieldFrame_h
#define InstantEffectConfigurationFieldFrame_h

#include "ConfigurationFieldFrame.h"

class InstantEffectConfigurationFieldFrame : public ConfigurationFieldFrame
{

private:
  void(*m_instantEffectFunc)(byte);

protected:
  virtual void onPositionSelect(byte position_) override
  {
    if (position_ == NOT_SELECTED_POSITION)
    {
      m_instantEffectFunc(m_startPosition);
    }
    
    ConfigurationFieldFrame::onPositionSelect(position_);
  }
  virtual void onChangeIndicatorPosition(byte index) override
  {
    m_instantEffectFunc(index);
    IFrameBase::onChangeIndicatorPosition(index);
  }

public:
  InstantEffectConfigurationFieldFrame(const ConfigurationData * configurationData, void (*instantEffectFunc)(byte)) : ConfigurationFieldFrame(configurationData), m_instantEffectFunc(instantEffectFunc)
  {  }
};

#endif // !InstantEffectConfigurationFieldFrame_h

