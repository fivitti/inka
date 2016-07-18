#ifndef AbstractAcceptFrame_h
#define AbstractAcceptFrame_h

#define FRAME_NUM_POSITION 2
#define ACCEPT_ACTION 0
#define DELINE_ACTION 1
#include "AbstractSelectFrame.h"

class AbstractAcceptFrame : public AbstractSelectFrame {
  
  protected:
  void writePosition(byte index) override {
    switch (index) {
      case ACCEPT_ACTION:
        lcdWriteString(F("Tak"));
        break;
      case DELINE_ACTION:
        lcdWriteString(F("Nie"));
        break;
      default:
        break;
    }
  }
  
  public:
  AbstractAcceptFrame() : AbstractSelectFrame() {
    numPositions = FRAME_NUM_POSITION;
  }
  ~AbstractAcceptFrame() {};
};

#undef FRAME_NUM_POSITION
#endif //AbstractAcceptFrame_h
