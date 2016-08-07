#ifndef RootFrame_h
#define RootFrame_h

#include "Config.h"
#include "IFrameBase.h"
#include "AbstractSelectFrame.h"
#include "ChooseDictionaryFrame.h"
#include "ConfigurationLineFrame.h"
#include "ConfigurationData.h"
#include "DrawConfigurationLineFrame.h"
#include "LcdConfigurationLineFrame.h"
#include "ResetConfigFrame.h"
#include "Lang.h"

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

/*
 * Main menu of program.
 * User can here start learn or change configuration.
 * If you add new position don't forget change @FRAME_NUM_POSITION
 */
class RootFrame : public AbstractSelectFrame {
  
private:
  //Value of flash helpers cannot be assign here. I do it in constuctor.
  const __FlashStringHelper * strDraw;
  const __FlashStringHelper * strLcd;
  const __FlashStringHelper * strSession;
  const __FlashStringHelper * strSummary;
  const __FlashStringHelper * strInit;
  const __FlashStringHelper * strDictionary;

protected:
  void onPositionSelect(byte position_) override {
    AbstractSelectFrame::onPositionSelect(position_);
    
    IFrameBase * frame;
    switch (position_)
    {
    case POSITION_DICTIONARY:
      frame = new ChooseDictionaryFrame(strDictionary);
      break;
    case POSITION_LCD:
      frame = new LcdConfigurationLineFrame(strLcd);
      break;
    case POSITION_DRAW:
    {
      ConfigurationData datas[CSV_LINE_CONFIG_LEARN_DRAW_SIZE] = {
        ConfigurationData(F(LANG_STR_DRAW_MAXIMUM_CARD_PER_SESSION), CSV_LINE_CONFIG_LEARN_DRAW, CSV_FIELD_CONFIG_LEARN_DRAW_MAXIMUM_CARD_PER_SESSION, CSV_FIELD_CONFIG_LEARN_DRAW_MAXIMUM_CARD_PER_SESSION_SIZE),
        ConfigurationData(F(LANG_STR_DRAW_MODE_QUESTION_LANG), CSV_LINE_CONFIG_LEARN_DRAW, CSV_FIELD_CONFIG_LEARN_DRAW_MODE, CSV_FIELD_CONFIG_LEARN_DRAW_MODE_SIZE, 3),
        ConfigurationData(F(LANG_STR_DRAW_REPEAT_NUMBER), CSV_LINE_CONFIG_LEARN_DRAW, CSV_FIELD_CONFIG_LEARN_DRAW_REPEAT_NUMBER, CSV_FIELD_CONFIG_LEARN_DRAW_REPEAT_NUMBER_SIZE)
      };
      frame = new DrawConfigurationLineFrame(strDraw, datas, CSV_LINE_CONFIG_LEARN_DRAW_SIZE);
      break;
    }
    case POSITION_SESSION:
    {
      ConfigurationData datas[CSV_LINE_CONFIG_LEARN_SESSION_SIZE] = {
        ConfigurationData(F(LANG_STR_SESSION_NUMBER_DECREASE_REPEAT_WHEN_GOOD), CSV_LINE_CONFIG_LEARN_SESSION, CSV_FIELD_CONFIG_LEARN_SESSION_NUMBER_DECREASE_REPEAT_WHEN_GOOD, CSV_FIELD_CONFIG_LEARN_SESSION_NUMBER_DECREASE_REPEAT_WHEN_GOOD_SIZE),
        ConfigurationData(F(LANG_STR_SESSION_NUMBER_INCREASE_REPEAT_WHEN_BAD), CSV_LINE_CONFIG_LEARN_SESSION, CSV_FIELD_CONFIG_LEARN_SESSION_NUMBER_INCREASE_REPEAT_WHEN_BAD, CSV_FIELD_CONFIG_LEARN_SESSION_NUMBER_INCREASE_REPEAT_WHEN_BAD_SIZE)
      };
      frame = new ConfigurationLineFrame(strSession, datas, CSV_LINE_CONFIG_LEARN_SESSION_SIZE);
      break;
    }
    case POSITION_SUMMARY:
    {
      ConfigurationData datas[CSV_LINE_CONFIG_LEARN_SUMMARY_SIZE] = {
        ConfigurationData(F(LANG_STR_SUMMARY_EASY_DECREASE_PROBABILITY), CSV_LINE_CONFIG_LEARN_SUMMARY, CSV_FIELD_CONFIG_LEARN_SUMMARY_EASY_DECREASE_PROBABILITY, CSV_FIELD_CONFIG_LEARN_SUMMARY_EASY_DECREASE_PROBABILITY_SIZE),
        ConfigurationData(F(LANG_STR_SUMMARY_MEDIUM_DECREASE_PROBABILITY), CSV_LINE_CONFIG_LEARN_SUMMARY, CSV_FIELD_CONFIG_LEARN_SUMMARY_MEDIUM_DECREASE_PROBABILITY, CSV_FIELD_CONFIG_LEARN_SUMMARY_MEDIUM_DECREASE_PROBABILITY_SIZE),
        ConfigurationData(F(LANG_STR_SUMMARY_HARD_INCREASE_PROBABILITY), CSV_LINE_CONFIG_LEARN_SUMMARY, CSV_FIELD_CONFIG_LEARN_SUMMARY_HARD_INCREASE_PROBABILITY, CSV_FIELD_CONFIG_LEARN_SUMMARY_HARD_INCREASE_PROBABILITY_SIZE),
        ConfigurationData(F(LANG_STR_SUMMARY_FORGET_INCREASE_PROBABILITY), CSV_LINE_CONFIG_LEARN_SUMMARY, CSV_FIELD_CONFIG_LEARN_SUMMARY_FORGET_INCREASE_PROBABILITY, CSV_FIELD_CONFIG_LEARN_SUMMARY_FORGET_INCREASE_PROBABILITY_SIZE),
        ConfigurationData(F(LANG_STR_SUMMARY_HARD_WHEN_NUMBER_BAD_ANSWER_WHEN_OTHERS_NORMAL), CSV_LINE_CONFIG_LEARN_SUMMARY, CSV_FIELD_CONFIG_LEARN_SUMMARY_HARD_WHEN_NUMBER_BAD_ANSWER_WHEN_OTHERS_NORMAL, CSV_FIELD_CONFIG_LEARN_SUMMARY_HARD_WHEN_NUMBER_BAD_ANSWER_WHEN_OTHERS_NORMAL_SIZE)
      };
      frame = new ConfigurationLineFrame(strSummary, datas, CSV_LINE_CONFIG_LEARN_SUMMARY_SIZE);
      break;
    }
    case POSITION_INIT:
    {
      ConfigurationData datas[CSV_LINE_CONFIG_LEARN_INIT_PROGRESS_SIZE] = {
        ConfigurationData(F(LANG_STR_INIT_PROGRESS_BEGIN_PROBABILITY), CSV_LINE_CONFIG_LEARN_INIT_PROGRESS, CSV_FIELD_CONFIG_LEARN_INIT_PROGRESS_BEGIN_PROBABILITY, CSV_FIELD_CONFIG_LEARN_INIT_PROGRESS_BEGIN_PROBABILITY_SIZE)
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
        MinLcd::lcdWriteString(F(LANG_STR_START_LEARN_FLOW));
        break;
      case POSITION_DICTIONARY:
        MinLcd::lcdWriteString(strDictionary);
        break;
      case POSITION_LCD:
        MinLcd::lcdWriteString(strLcd);
        break;
      case POSITION_DRAW:
        MinLcd::lcdWriteString(strDraw);
        break;
      case POSITION_SESSION:
        MinLcd::lcdWriteString(strSession);
        break;
      case POSITION_SUMMARY:
        MinLcd::lcdWriteString(strSummary);
        break;
      case POSITION_INIT:
        MinLcd::lcdWriteString(strInit);
        break;
      case POSITION_RESET:
        MinLcd::lcdWriteString(F(LANG_STR_RESET_CONFIG));
        break;
      default:
        break;
    }
  }
  
public:
  RootFrame() : AbstractSelectFrame() {
    m_numPositions = FRAME_NUM_POSITION;
    strLcd = F(LANG_STR_LCD);
    strDraw = F(LANG_STR_DRAW_HEADER);
    strSummary = F(LANG_STR_SUMMARY_HEADER);
    strInit = F(LANG_STR_INIT_HEADER);
    strSession = F(LANG_STR_SESSION_HEADER);
    strDictionary = F(LANG_STR_DICTIONARY);
    m_header = F(LANG_STR_APPLICATION_TITLE);
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





