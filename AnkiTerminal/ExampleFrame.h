#ifndef ExampleFrame_h
#define ExampleFrame_h

#include "IFrameBase.h"

#define FRAME_NUM_POSITION 13*4

class ExampleFrame : public IFrameBase {
private:
  const char * positions[FRAME_NUM_POSITION] = {"POS1", "POS2", "POS3", "POS4", "POS5", "POS6", "POS7", "POS8", "POS9", "POS10", "POS11", "POS12", "POS13",
  "POS1", "POS2", "POS3", "POS4", "POS5", "POS6", "POS7", "POS8", "POS9", "POS10", "POS11", "POS12", "POS13",
  "POS1", "POS2", "POS3", "POS4", "POS5", "POS6", "POS7", "POS8", "POS9", "POS10", "POS11", "POS12", "POS13",
  "POS1", "POS2", "POS3", "POS4", "POS5", "POS6", "POS7", "POS8", "POS9", "POS10", "POS11", "POS12", "POS13"};
  
  const char * getHeader() {
    return "HEADER";
  }
  void onPositionSelect(byte position_) override {
    Serial.print("Selected: ");
    Serial.println(position_);
  }

protected:
  const char * getPosition(byte index) override {
    return positions[index];
  }
public:
  ExampleFrame() : IFrameBase() {
    numPositions = FRAME_NUM_POSITION;
  }
  ~ExampleFrame() {
    Serial.println("Destructor of Example frame");
//    delete[] positions;
  };
};
#undef FRAME_NUM_POSITION
#endif //ExampleFrame_h





