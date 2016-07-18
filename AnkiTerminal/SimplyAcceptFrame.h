#ifndef SimplyAcceptFrame_h
#define SimplyAcceptFrame_h

#include "AbstractAcceptFrame.h"

class SimplyAcceptFrame : public AbstractAcceptFrame
{
  
  private:
  const __FlashStringHelper * m_header;
  
  void writeHeader() override
  {
     IFrameBase::writeHeader();
     lcdWriteString(m_header);
  }
  
  public:
  SimplyAcceptFrame(const __FlashStringHelper * header) : AbstractAcceptFrame() {
    m_header = header;
  };
  ~SimplyAcceptFrame() {};
};

#endif //SimplyAcceptFrame_h
