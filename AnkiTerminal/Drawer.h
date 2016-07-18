#ifndef Drawer_h
#define Drawer_h

#include "Config.h"
#include "CSVSpecifications.h"
#include "ConfigurationFile.h"
#include <SdFat.h>
#include "Tools.h"
#include "LcdTools.h"

#define BUFFER_SIZE FILENAME_LIMIT_SIZE //buffer must be at least as high as filename
#define BUFFER_SIZE_WITH_END_CHAR FILENAME_LIMIT_SIZE + 1
#define MAXIMUM_PROBABILITY_FOR_RANDOM 100
#define BEGIN_REPEAT_TOTAL_NUMBER 0
#define NOT_DRAWED_TYPE 4
class Drawer
{
  private:
  SdFat * m_sd;
  CSVFile m_csv;
  
  byte m_maximumToDraw;
  byte m_repeatCardNum{0};
  byte m_mode;

  int m_bufferNum;
  char m_buffer[BUFFER_SIZE_WITH_END_CHAR];
  public:
  Drawer(SdFat * sdCard) : m_sd(sdCard) {
    m_buffer[BUFFER_SIZE_WITH_END_CHAR - 1] = '\0';
  }
  ~Drawer(){}

  bool readConfiguration()
  {
    m_sd->chdir();
    m_sd->chdir(APPLICATION_DIR);
    
    byte drawConfiguration[CSV_LINE_CONFIG_LEARN_DRAW_SIZE];
    ConfigurationFile::readConfigurationLine(&m_csv, drawConfiguration, CSV_LINE_CONFIG_LEARN_DRAW);
    
    m_maximumToDraw = drawConfiguration[CSV_FIELD_CONFIG_LEARN_DRAW_MAXIMUM_CARD_PER_SESSION];
    m_mode = drawConfiguration[CSV_FIELD_CONFIG_LEARN_DRAW_MODE];
    m_repeatCardNum = drawConfiguration[CSV_FIELD_CONFIG_LEARN_DRAW_REPEAT_NUMBER];

    //Temporary read dictionary name to buffer
    ConfigurationFile::readConfigurationDictionaryName(&m_csv, m_buffer, BUFFER_SIZE);

    if (StringTools::isEmpty(m_buffer))
    {
      LcdTools::writeFullscreenMessage(F(" Brak słownika"));
      delay(HUMAN_ERROR_DELAY);
      return false;
    }
    return true;
  }

  byte drawTypeForDrawing()
  {
    if (m_mode != DRAW_MODE_BOTH_LANG)
      return m_mode;
    else if (random(2) == 0)
      return DRAW_MODE_FIRST_LANG;
    else
      return DRAW_MODE_SECOND_LANG;
  }
  
  //Require gotoBeginOfLine()
  byte drawTypeForCurrentLine(CSVFile * progressFile) {
    byte mode = drawTypeForDrawing();

    if (mode == DRAW_MODE_FIRST_LANG)
    {
      progressFile->gotoField(CSV_FIELD_PROGRESS_PROBABILITY_FIRST_LANG_POSITION);
    }
    else if (mode == DRAW_MODE_SECOND_LANG)
    {
      progressFile->gotoField(CSV_FIELD_PROGRESS_PROBABILITY_SECOND_LANG_POSITION);
    }
    
    progressFile->readField(m_bufferNum, m_buffer, BUFFER_SIZE);
    if (m_bufferNum >= random(MAXIMUM_PROBABILITY_FOR_RANDOM))
      return mode;
    else
      return NOT_DRAWED_TYPE;
  }

  void fillLineInSessionFile(CSVFile * sessionFile, CSVFile * dictionaryFile, unsigned int numLine, byte type)
  {
    sessionFile->addField(m_repeatCardNum, CSV_FIELD_SESSION_REPEAT_TO_END_SIZE);
    sessionFile->addField(BEGIN_REPEAT_TOTAL_NUMBER, CSV_FIELD_SESSION_REPEAT_TO_END_SIZE);
    //Copy words
    sessionFile->addField();
    dictionaryFile->gotoLine(numLine);
    dictionaryFile->gotoField(type == DRAW_MODE_FIRST_LANG ? CSV_FIELD_DICTIONARY_FIRST_LANG : CSV_FIELD_DICTIONARY_SECOND_LANG);
    dictionaryFile->copyField(sessionFile);
    sessionFile->addField();

    dictionaryFile->gotoField(type != DRAW_MODE_FIRST_LANG ? CSV_FIELD_DICTIONARY_FIRST_LANG : CSV_FIELD_DICTIONARY_SECOND_LANG);
    dictionaryFile->copyField(sessionFile);

    sessionFile->addField(numLine);
    sessionFile->addField(type);
  }

  byte draw()
  {
    m_sd->chdir();
    m_sd->chdir(APPLICATION_DIR);

    if (m_sd->exists(SESSION_SET_FILENAME)) {
      m_csv.open(SESSION_SET_FILENAME, O_RDWR);
      m_csv.remove();
      m_csv.close();
    }
//    else if (!sd->exists(m_buffer) {
//        //Missing progress file  
//        //Not implemented
//    }

    CSVFile progressFile;
    CSVFile dictionaryFile;

    m_csv.open(SESSION_SET_FILENAME, O_RDWR | O_CREAT); 
    
    m_csv.gotoBeginOfFile();
    progressFile.open(m_buffer, O_RDWR);

    progressFile.gotoBeginOfFile();
    m_sd->chdir();
    m_sd->chdir(DICTIONARY_DIR);
    dictionaryFile.open(m_buffer, O_RDWR);

    byte drawingCard = 0;
    do
    {
      byte type = drawTypeForCurrentLine(&progressFile);

      if (type == NOT_DRAWED_TYPE)
        continue;
        
      if (drawingCard != 0)
        m_csv.addLine();
        
      fillLineInSessionFile(&m_csv, &dictionaryFile, progressFile.getNumberOfLine(), type);
      drawingCard += 1;
    }
    while (drawingCard < m_maximumToDraw && progressFile.nextLine());

    m_csv.close();
    progressFile.close();
    dictionaryFile.close();

    return drawingCard;
  }
};

#undef BUFFER_SIZE
#undef MAXIMUM_PROBABILITY_FOR_RANDOM
#undef BEGIN_REPEAT_TOTAL_NUMBER
#undef NOT_DRAWED_TYPE
#endif //Drawer_h
