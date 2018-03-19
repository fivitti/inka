#ifndef AcceptDrawFrame_h
#define AcceptDrawFrame_h

#include "Lang.h"
#include "YesNoAcceptFrame.h"

/*
 * Frame for accept result drawing.
 * This class has custom method draw header (no single string, but with dynamic content).
 */
class AcceptDrawFrame : public YesNoAcceptFrame
{
  private:
  byte m_toAccept{0};
  
  void writeHeader() override
  {
     MinLcd::lcdXY(0, 0);
     MinLcd::lcdWriteString(F(LANG_STR_ACCEPT_DRAW_TITLE_FIRST_PART));
     MinLcd::lcdWriteNumber(m_toAccept);
     MinLcd::lcdWriteString(F(LANG_STR_ACCEPT_DRAW_TITLE_SECOND_PART));
  }
  
  public:
  AcceptDrawFrame() : YesNoAcceptFrame(nullptr) {};
  ~AcceptDrawFrame() {};

  void setToAcceptNumber(const byte toAccept) 
  {
    m_toAccept = toAccept;
  }
};


#endif //AcceptDrawFrame_h
