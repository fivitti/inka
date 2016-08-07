#ifndef ConfigurationLineFrame_h
#define ConfigurationLineFrame_h

#include "Config.h"
#include "IFrameBase.h"
#include "ConfigurationData.h"
#include "ConfigurationFieldFrame.h"
#include "MinLcd.h"

/*
 * Class - container for @ConfigurationFileField objects.
 * Display frame where positions are headers of frame fields.
 * When user select postion right frame is show.
 * After close field frame is again display this frame.
 */
class ConfigurationLineFrame : public IFrameBase
{

protected:
  const ConfigurationData * m_datas;

  virtual void onPositionSelect(byte position_) override
  {
    if (position_ == NOT_SELECTED_POSITION)
      return;

    IFrameBase * frame = new ConfigurationFieldFrame(&m_datas[position_]);
    frame->show();
    delete frame;

    show();
  }
  virtual void writePosition(byte index) override
  {
    MinLcd::lcdWriteString(m_datas[index].m_header);
  }

public:
  // Arguments:
  // @header - pointer to string header in flash memory
  // @configurationDatas - array of data of the field frames
  // @configurationDatasSize - size of @configurationDatas
  ConfigurationLineFrame(const __FlashStringHelper * header, ConfigurationData * configurationDatas, byte configurationDatasSize) : IFrameBase(),
                          m_datas(configurationDatas)
  {
    m_numPositions = configurationDatasSize;
    m_header = header;
  }
  virtual ~ConfigurationLineFrame() {}

};

#endif //ConfigurationLineFrame_h
