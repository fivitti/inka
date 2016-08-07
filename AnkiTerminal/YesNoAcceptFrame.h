#ifndef YesNoAcceptFrame_h
#define YesNoAcceptFrame_h

#define FRAME_NUM_POSITION 2
#define ACCEPT_ACTION 0
#define DELINE_ACTION 1
#include "AbstractSelectFrame.h"
#include "Lang.h"

/*
 * Frame for simply question. It have two option: YES and NO.
 * You should set header to understandably question.
 * You can get answer by @getSelectedPosition method after method @show.
 * Remember that user can press MENU button and you get @NOT_SELECTED_POSITION.
 */
class YesNoAcceptFrame : public AbstractSelectFrame {
  
  protected:
  void writePosition(byte index) override {
    switch (index) {
      case ACCEPT_ACTION:
        MinLcd::lcdWriteString(F(LANG_STR_YES));
        break;
      case DELINE_ACTION:
        MinLcd::lcdWriteString(F(LANG_STR_NO));
        break;
      default:
        break;
    }
  }
  
  public:
  YesNoAcceptFrame(const __FlashStringHelper * header) : AbstractSelectFrame() {
    m_header = header;
    m_numPositions = FRAME_NUM_POSITION;
  }
  ~YesNoAcceptFrame() {};
};

#undef FRAME_NUM_POSITION
#endif //YesNoAcceptFrame_h
