#ifndef Summary_h
#define Summary_h

#include <CSVFile.h>
#include "FileTools.h"
#include "Config.h"
#include "ConfigurationFile.h"
#include "CSVSpecifications.h"
#include "LcdTools.h"

#define BUFFER_SIZE FILENAME_LIMIT_SIZE
#define SAFE_BUFFER_SIZE BUFFER_SIZE+1

#define RETURN_IF_FALSE(status_) if(!(status_))return false;

class Summary
{
  private:
  SdFat * m_sd;
  CSVFile m_csvProgress;
  CSVFile m_csvSession;

  char m_buffer[SAFE_BUFFER_SIZE];
  int m_numBuffer;

  int m_easyDecreaseProbability;
  int m_mediumDecreaseProbability;
  int m_hardIncreaseProbability;
  int m_forgetIncreaseProbabilty;

  byte m_limitForEasy;
  byte m_limitForHard;

  void readConfigurationFile()
  {
    byte configuration[CSV_LINE_CONFIG_LEARN_SUMMARY_SIZE > CSV_LINE_CONFIG_LEARN_SESSION_SIZE ? CSV_LINE_CONFIG_LEARN_SUMMARY_SIZE : CSV_LINE_CONFIG_LEARN_SESSION_SIZE];
    ConfigurationFile::readConfigurationLine(&m_csvProgress, configuration, CSV_LINE_CONFIG_LEARN_SUMMARY);
    m_easyDecreaseProbability = (-1) * ((int)configuration[CSV_FIELD_CONFIG_LEARN_SUMMARY_EASY_DECREASE_PROBABILITY]);
    m_mediumDecreaseProbability = (-1) * ((int)configuration[CSV_FIELD_CONFIG_LEARN_SUMMARY_MEDIUM_DECREASE_PROBABILITY]);
    m_hardIncreaseProbability = configuration[CSV_FIELD_CONFIG_LEARN_SUMMARY_HARD_INCREASE_PROBABILITY];
    m_forgetIncreaseProbabilty = configuration[CSV_FIELD_CONFIG_LEARN_SUMMARY_FORGET_INCREASE_PROBABILITY];

    ConfigurationFile::readConfigurationLine(&m_csvProgress, configuration, CSV_LINE_CONFIG_LEARN_SESSION);
    byte decreaseRepeatWhenGood = configuration[CSV_FIELD_CONFIG_LEARN_SESSION_NUMBER_DECREASE_REPEAT_WHEN_GOOD];
    byte increaseRepeatWhenBad = configuration[CSV_FIELD_CONFIG_LEARN_SESSION_NUMBER_INCREASE_REPEAT_WHEN_BAD];
    byte hardWhenNumberBadAnswerWhenOtherNormal = configuration[CSV_FIELD_CONFIG_LEARN_SUMMARY_HARD_WHEN_NUMBER_BAD_ANSWER_WHEN_OTHERS_NORMAL];
    
    ConfigurationFile::readConfigurationField(&m_csvProgress, m_numBuffer, CSV_LINE_CONFIG_LEARN_DRAW, CSV_FIELD_CONFIG_LEARN_DRAW_REPEAT_NUMBER);
    byte repeatNumberNormalInSession = m_numBuffer;

    m_limitForEasy = (repeatNumberNormalInSession / decreaseRepeatWhenGood) + 1;
    m_limitForHard = repeatNumberNormalInSession + increaseRepeatWhenBad * (hardWhenNumberBadAnswerWhenOtherNormal + 1);
  }
  
  bool openFiles()
  {
    m_sd->chdir(APPLICATION_DIR);

    RETURN_IF_FALSE(m_csvSession.open(SESSION_SET_FILENAME, O_RDWR));
    ConfigurationFile::readConfigurationDictionaryName(&m_csvProgress, m_buffer, BUFFER_SIZE);
    RETURN_IF_FALSE(m_csvProgress.open(m_buffer, O_RDWR));
    return true;
  }
  
  bool init()
  {
    readConfigurationFile();
    RETURN_IF_FALSE(openFiles());
    return true;
  }
  
  void disposeFiles()
  {
    m_csvProgress.close();
    m_csvSession.close();
  }
  
  bool checkSummaryInProgressAndSetPointerAtBeginFirstNotRemovedLine()
  {
    m_csvSession.gotoBeginOfFile();

    //If first line is not removed then summary is not beggining
    if (!m_csvSession.isLineMarkedAsRemove())
    {
      m_csvSession.gotoBeginOfLine();
      return false;
    }

    //Else we set pointer to begin first not removed line
    while (m_csvSession.nextLine() && m_csvSession.isLineMarkedAsRemove())
    { }

    m_csvSession.gotoBeginOfLine();
    
    return true;
  }

  byte saturateProbability(byte beginProbability, int change)
  {
    const int afterChange = beginProbability + change;
    if (afterChange < CSV_FIELD_PROGRESS_PROBABILITY_MINIMUM)
      return CSV_FIELD_PROGRESS_PROBABILITY_MINIMUM;
    else if (afterChange > CSV_FIELD_PROGRESS_PROBABILITY_MAXIMUM)
      return CSV_FIELD_PROGRESS_PROBABILITY_MAXIMUM;
    else
      return afterChange;
  }

  //Required gotoBeginOfField();
  void editProgressField(int changeProbability)
  {
    m_csvProgress.readField(m_numBuffer, m_buffer, BUFFER_SIZE);
    m_csvProgress.gotoBeginOfField();
    m_csvProgress.editField(saturateProbability(m_numBuffer, changeProbability));
  }
  
  void editProgressLine(int changeProbability, byte type)
  {
    if (type == DRAW_MODE_FIRST_LANG || type == DRAW_MODE_BOTH_LANG)
    {
      m_csvProgress.gotoField(CSV_FIELD_PROGRESS_PROBABILITY_FIRST_LANG_POSITION);
      editProgressField(changeProbability);
    }
    if (type == DRAW_MODE_SECOND_LANG || type == DRAW_MODE_BOTH_LANG)
    {
      m_csvProgress.gotoField(CSV_FIELD_PROGRESS_PROBABILITY_SECOND_LANG_POSITION);
      editProgressField(changeProbability);
    }       
  }

  void increaseProbabilityByForgetting()
  {
    editProgressLine(m_forgetIncreaseProbabilty, DRAW_MODE_BOTH_LANG);
  }
  
  void readSessionLine(byte& repeatTotal, unsigned int& numLine, byte& type)
  {
    m_csvSession.gotoField(CSV_FIELD_SESSION_REPEAT_TOTAL);
    m_csvSession.readField(m_numBuffer, m_buffer, BUFFER_SIZE);
    repeatTotal = m_numBuffer;

    m_csvSession.gotoField(CSV_FIELD_SESSION_NUMBER_LINE);
    m_csvSession.readField(m_numBuffer, m_buffer, BUFFER_SIZE);
    numLine = m_numBuffer;

    m_csvSession.gotoField(CSV_FIELD_SESSION_TYPE);
    m_csvSession.readField(m_numBuffer, m_buffer, BUFFER_SIZE);
    type = m_numBuffer;
  }

  int getChangeProbability(byte totalRepeats)
  {
    if (totalRepeats <= m_limitForEasy)
    {
      return m_easyDecreaseProbability;
    }
    else if (totalRepeats >= m_limitForHard)
      return m_hardIncreaseProbability;
    else
      return m_mediumDecreaseProbability;
  }

  //If summary is partially completed then skip processed lines.
  //Set pointer in session file to first not removed line and
  //in progress file to coressponding line.
  //Return number corresponding line in progress file
  void skipProcessedProgress()
  {
    m_numBuffer = 0;
    if (checkSummaryInProgressAndSetPointerAtBeginFirstNotRemovedLine())
    {
      m_csvSession.gotoField(CSV_FIELD_SESSION_NUMBER_LINE);
      m_csvSession.readField(m_numBuffer, m_buffer, BUFFER_SIZE);
    }
    m_csvProgress.gotoLine(m_numBuffer);
  }


  bool executeImpl()
  {
    LcdTools::writeFullscreenMessage(F(" Podsumowuję"));
    
    if (!init()) //Missing session file;
      return false;

    byte totalRepeats = 0;
    byte type = 0;
    unsigned int targetLineFromSessionFile = 0;

    skipProcessedProgress();

    do
    {
      readSessionLine(totalRepeats, targetLineFromSessionFile, type);
      
      while (m_csvProgress.getNumberOfLine() != targetLineFromSessionFile)
      {
        increaseProbabilityByForgetting();
        RETURN_IF_FALSE(m_csvProgress.nextLine()); //If false then error in session file
      }

      editProgressLine(getChangeProbability(totalRepeats), type);

      m_csvSession.gotoBeginOfLine();
      m_csvSession.markLineAsRemove();
      
      m_csvProgress.nextLine();
    }
    while (m_csvSession.nextLine());

    while(m_csvProgress.nextLine())
    {
      increaseProbabilityByForgetting();
    }
    return true;
  }
  
  public:
  Summary(SdFat * sd) : m_sd(sd) 
  {
    m_buffer[BUFFER_SIZE] = '\0';
  }
  ~Summary() {}

  void execute()
  {
    executeImpl();
    disposeFiles();
    m_sd->remove(SESSION_SET_FILENAME);
    LcdTools::writeFullscreenMessage(F(" Koniec sesji"));
    delay(HUMAN_ERROR_DELAY);
  }
};

#undef RETURN_IF_FALSE

#undef BUFFER_SIZE
#undef SAFE_BUFFER_SIZE

#endif //Summary_h
