#ifndef InstantEffectConfigurationFieldFrame_h
#define InstantEffectConfigurationFieldFrame_h

#include "ConfigurationFieldFrame.h"

/*
 * Special class for instant show effect current selected option in configuration frame.
 * I use this for instant change brightness and contrast LCD.
 * User doesn't have press MIDDLE button for show effect.
 *
 * After each change position indicator call @m_instantEffectFunc with current indicating
 * position as argument.
 *
 * If user cancel frame (press MENU button) then @m_instantEffectFunc is called with
 * start position index.
 */
class InstantEffectConfigurationFieldFrame : public ConfigurationFieldFrame
{

private:
  void(*m_instantEffectFunc)(byte);

protected:
  virtual void onPositionSelect(const byte position_) override
  {
    if (position_ == NOT_SELECTED_POSITION)
    {
      m_instantEffectFunc(m_startPosition);
    }
    
    ConfigurationFieldFrame::onPositionSelect(position_);
  }
  virtual void onChangeIndicatorPosition(const byte index) override
  {
    m_instantEffectFunc(index);
    IFrameBase::onChangeIndicatorPosition(index);
  }

public:
  InstantEffectConfigurationFieldFrame(const ConfigurationData * configurationData, void (*instantEffectFunc)(byte)) : ConfigurationFieldFrame(configurationData), m_instantEffectFunc(instantEffectFunc)
  {  }
};

#endif // !InstantEffectConfigurationFieldFrame_h

