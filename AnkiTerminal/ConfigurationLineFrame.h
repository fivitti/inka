#ifndef ConfigurationLineFrame_h
#define ConfigurationLineFrame_h

#include "IFrameBase.h"
#include "ConfigurationData.h"
#include "ConfigurationFieldFrame.h"
#include "MinLcd.h"

class ConfigurationLineFrame : public IFrameBase
{
private:
  const __FlashStringHelper * m_header;

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
    lcdWriteString(m_datas[index].m_header);
  }
  virtual void writeHeader()
  {
    IFrameBase::writeHeader();
    lcdWriteString(m_header);
  }

public:
  ConfigurationLineFrame(const __FlashStringHelper * header, ConfigurationData * configurationDatas, byte configurationDatasSize) : IFrameBase(),
                          m_header(header), m_datas(configurationDatas)
  {
    numPositions = configurationDatasSize;
  }
  virtual ~ConfigurationLineFrame() {}

};

#endif //ConfigurationLineFrame_h
