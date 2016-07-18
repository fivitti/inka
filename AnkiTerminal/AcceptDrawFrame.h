#ifndef AcceptDrawFrame_h
#define AcceptDrawFrame_h

#include "AbstractAcceptFrame.h"

class AcceptDrawFrame : public AbstractAcceptFrame
{
  private:
  byte m_toAccept{0};
  
  void writeHeader() override
  {
     IFrameBase::writeHeader();
     lcdWriteString(F(" Użyć "));
     lcdWriteNumber(m_toAccept);
     lcdWriteString(F(" kart?"));
  }
  
  public:
  AcceptDrawFrame() : AbstractAcceptFrame() {};
  ~AcceptDrawFrame() {};

  void setToAcceptNumber(byte toAccept) 
  {
    m_toAccept = toAccept;
  }
};


#endif //AcceptDrawFrame_h
