#ifndef RootFrame_h
#define RootFrame_h

#include "Config.h"
#include "IFrameBase.h"
#include "AbstractSelectFrame.h"
#include "ChooseDictionaryFrame.h"
#include <MemoryFree.h>
#include "ConfigurationLineFrame.h"
#include "ConfigurationData.h"
#include "DrawConfigurationLineFrame.h"
#include "LcdConfigurationLineFrame.h"
#include "ResetConfigFrame.h"

#define FRAME_NUM_POSITION 8
#define START_ACTION 0

//Should be in order
#define POSITION_DICTIONARY 1
#define POSITION_LCD 2
#define POSITION_DRAW 3
#define POSITION_SESSION 4
#define POSITION_SUMMARY 5
#define POSITION_INIT 6
#define POSITION_RESET 7

class RootFrame : public AbstractSelectFrame {
  
private:
  const __FlashStringHelper * strDraw;
  const __FlashStringHelper * strLcd;
  const __FlashStringHelper * strSession;
  const __FlashStringHelper * strSummary;
  const __FlashStringHelper * strInit;

protected:
  void onPositionSelect(byte position_) override {
    AbstractSelectFrame::onPositionSelect(position_);
    
    IFrameBase * frame;
    switch (position_)
    {
    case POSITION_DICTIONARY:
      frame = new ChooseDictionaryFrame();
      break;
    case POSITION_LCD:
      frame = new LcdConfigurationLineFrame(strLcd);
      break;
    case POSITION_DRAW:
    {
      ConfigurationData datas[CSV_LINE_CONFIG_LEARN_DRAW_SIZE] = {
        ConfigurationData(F("Max kart/grę"), CSV_LINE_CONFIG_LEARN_DRAW, CSV_FIELD_CONFIG_LEARN_DRAW_MAXIMUM_CARD_PER_SESSION, CSV_FIELD_CONFIG_LEARN_DRAW_MAXIMUM_CARD_PER_SESSION_SIZE),
        ConfigurationData(F("Język pytań"), CSV_LINE_CONFIG_LEARN_DRAW, CSV_FIELD_CONFIG_LEARN_DRAW_MODE, CSV_FIELD_CONFIG_LEARN_DRAW_MODE_SIZE, 3),
        ConfigurationData(F("Ile powtórzeń"), CSV_LINE_CONFIG_LEARN_DRAW, CSV_FIELD_CONFIG_LEARN_DRAW_REPEAT_NUMBER, CSV_FIELD_CONFIG_LEARN_DRAW_REPEAT_NUMBER_SIZE)
      };
      frame = new DrawConfigurationLineFrame(strDraw, datas, CSV_LINE_CONFIG_LEARN_DRAW_SIZE);
      break;
    }
    case POSITION_SESSION:
    {
      ConfigurationData datas[CSV_LINE_CONFIG_LEARN_SESSION_SIZE] = {
        ConfigurationData(F("+ dobrej odp."), CSV_LINE_CONFIG_LEARN_SESSION, CSV_FIELD_CONFIG_LEARN_SESSION_NUMBER_DECREASE_REPEAT_WHEN_GOOD, CSV_FIELD_CONFIG_LEARN_SESSION_NUMBER_DECREASE_REPEAT_WHEN_GOOD_SIZE),
        ConfigurationData(F("- złej odp."), CSV_LINE_CONFIG_LEARN_SESSION, CSV_FIELD_CONFIG_LEARN_SESSION_NUMBER_INCREASE_REPEAT_WHEN_BAD, CSV_FIELD_CONFIG_LEARN_SESSION_NUMBER_INCREASE_REPEAT_WHEN_BAD_SIZE)
      };
      frame = new ConfigurationLineFrame(strSession, datas, CSV_LINE_CONFIG_LEARN_SESSION_SIZE);
      break;
    }
    case POSITION_SUMMARY:
    {
      ConfigurationData datas[CSV_LINE_CONFIG_LEARN_SUMMARY_SIZE] = {
        ConfigurationData(F("+ za łatwe"), CSV_LINE_CONFIG_LEARN_SUMMARY, CSV_FIELD_CONFIG_LEARN_SUMMARY_EASY_DECREASE_PROBABILITY, CSV_FIELD_CONFIG_LEARN_SUMMARY_EASY_DECREASE_PROBABILITY_SIZE),
        ConfigurationData(F("+ za średnie"), CSV_LINE_CONFIG_LEARN_SUMMARY, CSV_FIELD_CONFIG_LEARN_SUMMARY_MEDIUM_DECREASE_PROBABILITY, CSV_FIELD_CONFIG_LEARN_SUMMARY_MEDIUM_DECREASE_PROBABILITY_SIZE),
        ConfigurationData(F("- za trudne"), CSV_LINE_CONFIG_LEARN_SUMMARY, CSV_FIELD_CONFIG_LEARN_SUMMARY_HARD_INCREASE_PROBABILITY, CSV_FIELD_CONFIG_LEARN_SUMMARY_HARD_INCREASE_PROBABILITY_SIZE),
        ConfigurationData(F("- zapomnienia"), CSV_LINE_CONFIG_LEARN_SUMMARY, CSV_FIELD_CONFIG_LEARN_SUMMARY_FORGET_INCREASE_PROBABILITY, CSV_FIELD_CONFIG_LEARN_SUMMARY_FORGET_INCREASE_PROBABILITY_SIZE),
        ConfigurationData(F("x złe=trudne"), CSV_LINE_CONFIG_LEARN_SUMMARY, CSV_FIELD_CONFIG_LEARN_SUMMARY_HARD_WHEN_NUMBER_BAD_ANSWER_WHEN_OTHERS_NORMAL, CSV_FIELD_CONFIG_LEARN_SUMMARY_HARD_WHEN_NUMBER_BAD_ANSWER_WHEN_OTHERS_NORMAL_SIZE)
      };
      frame = new ConfigurationLineFrame(strSummary, datas, CSV_LINE_CONFIG_LEARN_SUMMARY_SIZE);
      break;
    }
    case POSITION_INIT:
    {
      ConfigurationData datas[CSV_LINE_CONFIG_LEARN_INIT_PROGRESS_SIZE] = {
        ConfigurationData(F("Szansa nowych"), CSV_LINE_CONFIG_LEARN_INIT_PROGRESS, CSV_FIELD_CONFIG_LEARN_INIT_PROGRESS_BEGIN_PROBABILITY, CSV_FIELD_CONFIG_LEARN_INIT_PROGRESS_BEGIN_PROBABILITY_SIZE)
      };
      frame = new ConfigurationLineFrame(strInit, datas, CSV_LINE_CONFIG_LEARN_INIT_PROGRESS_SIZE);
      break;
    }
    case POSITION_RESET:
    {
      frame = new ResetConfigFrame();
      break;
    }
    default:
      return; //It is important
    }

    frame->show();
    delete frame;
  }

  void writePosition(byte index) {
    switch (index) {
      case START_ACTION:
        lcdWriteString(F("Rozpocznij"));
        break;
      case POSITION_DICTIONARY:
        lcdWriteString(F("Słowniki"));
        break;
      case POSITION_LCD:
        lcdWriteString(strLcd);
        break;
      case POSITION_DRAW:
        lcdWriteString(strDraw);
        break;
      case POSITION_SESSION:
        lcdWriteString(strSession);
        break;
      case POSITION_SUMMARY:
        lcdWriteString(strSummary);
        break;
      case POSITION_INIT:
        lcdWriteString(strInit);
        break;
      case POSITION_RESET:
        lcdWriteString(F("Resetowanie"));
        break;
      default:
        break;
    }
  }
  
  void writeHeader() override {
     IFrameBase::writeHeader();
     lcdWriteString(F("     INKA"));
  }
  
public:
  RootFrame() : AbstractSelectFrame() {
    numPositions = FRAME_NUM_POSITION;
    strLcd = F("LCD");
    strDraw = F("Losowanie");
    strSummary = F("Podsumowanie");
    strInit = F("Inicjalizacja");
    strSession = F("Sesja");
  }
  ~RootFrame() {};
};
#undef FRAME_NUM_POSITION
#undef POSITION_DICTIONARY
#undef POSITION_LCD
#undef POSITION_DRAW
#undef POSITION_SESSION
#undef POSITION_SUMMARY
#undef POSITION_INIT
#endif //RootFrame.h





