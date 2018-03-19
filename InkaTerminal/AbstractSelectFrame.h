#ifndef AbstractSelectFrame_h
#define AbstractSelectFrame_h

#include "IFrameBase.h"

/*
 * Abstract class provide interface for remember and get position choose by user from outside frame.
 */
class AbstractSelectFrame : public IFrameBase {
private:
  byte positionSelected = NOT_SELECTED_POSITION;

protected:
  void onPositionSelect(const byte position_) override {
    positionSelected = position_;
  }
  
public:
  byte getSelectedAction() const {
    return positionSelected;
  }
};

#endif //AbstractSelectFrame_h
