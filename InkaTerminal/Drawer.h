#ifndef Drawer_h
#define Drawer_h

#include "Config.h"
#include "Lang.h"
#include "CSVSpecifications.h"
#include "ConfigurationFile.h"
#include "Tools.h"
#include "LcdTools.h"
#include "FileTools.h"

#define BUFFER_FILENAME_SIZE FILENAME_LIMIT_SIZE //buffer must be at least as high as filename
#define BUFFER_FILENAME_SIZE_WITH_END_CHAR (FILENAME_LIMIT_SIZE + 1)
#define BUFFER_FIELD_SIZE MAXIMUM_NUMBERIC_FIXED_CONFIGURATION_FIELD_SIZE
#define BUFFER_FIELD_SIZE_WITH_END_CHAR (MAXIMUM_NUMBERIC_FIXED_CONFIGURATION_FIELD_SIZE + 1)
#define MAXIMUM_PROBABILITY_FOR_RANDOM 100
#define BEGIN_REPEAT_TOTAL_NUMBER 0
#define NOT_DRAWED_TYPE 4

/*
 * Class for draw cards for new session.
 * It create session file.
 */
class Drawer
{
  private:
  CSVFile m_csv;
  
  byte m_maximumToDraw;
  byte m_repeatCardNum{0};
  byte m_mode;

  int m_bufferNum;
  char m_buffer[MAXIMUM_NUMBERIC_FIXED_CONFIGURATION_FIELD_SIZE + 1];
  char m_dictionaryName[BUFFER_FILENAME_SIZE_WITH_END_CHAR];
  public:
  Drawer() {
    m_dictionaryName[BUFFER_FILENAME_SIZE_WITH_END_CHAR - 1] = '\0';
    m_buffer[BUFFER_FIELD_SIZE_WITH_END_CHAR - 1] = '\0';
  }
  ~Drawer(){}

  bool readConfiguration()
  {
    FileTools::chdirToApplicationDir();
    
    byte drawConfiguration[CSV_LINE_CONFIG_LEARN_DRAW_SIZE];
    ConfigurationFile::readConfigurationLine(&m_csv, drawConfiguration, CSV_LINE_CONFIG_LEARN_DRAW);
    
    m_maximumToDraw = drawConfiguration[CSV_FIELD_CONFIG_LEARN_DRAW_MAXIMUM_CARD_PER_SESSION];
    m_mode = drawConfiguration[CSV_FIELD_CONFIG_LEARN_DRAW_MODE];
    m_repeatCardNum = drawConfiguration[CSV_FIELD_CONFIG_LEARN_DRAW_REPEAT_NUMBER];

    //Temporary read dictionary name to buffer
    return ConfigurationFile::readConfigurationDictionaryName(&m_csv, m_dictionaryName, BUFFER_FILENAME_SIZE);
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
    
    progressFile->readField(m_bufferNum, m_buffer, BUFFER_FIELD_SIZE);
    if (m_bufferNum >= random(MAXIMUM_PROBABILITY_FOR_RANDOM))
      return mode;
    else
      return NOT_DRAWED_TYPE;
  }

  void fillLineInSessionFile(CSVFile * sessionFile, CSVFile * dictionaryFile, const unsigned int numLine, const byte type)
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

  // The mechanism of the draw is very simply. Each card has two value of probability (one for each language)
  // Algorithm goes through the dictionary and test each card one by one. It draws number (0-99) and compare with the probability.
  // If draw number will be less then probability the card is selected.
  // The algorithm searches dictionary only one times. (Hard task is check if card is selected when re-search).
  // The search is end when number of drawing card is equals this parameter or end of dictonary.
  // It may therefore happen, that the algoritm draw less number of cards then value this parameter.
  byte draw()
  {
    FileTools::chdirToApplicationDir();

    CSVFile progressFile;
    CSVFile dictionaryFile;

    m_csv.open(SESSION_SET_FILENAME, O_RDWR | O_CREAT | O_TRUNC);
    m_csv.gotoBeginOfFile();
    
    progressFile.open(m_dictionaryName, O_RDWR);
    progressFile.gotoBeginOfFile();
    FileTools::chdirToDictionaryDir();
    dictionaryFile.open(m_dictionaryName, O_RDWR);
    dictionaryFile.gotoBeginOfFile();
    
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

  byte getMaximumToDraw() const
  {
    return m_maximumToDraw;
  }
};

#undef BUFFER_FILENAME_SIZE
#undef BUFFER_FIELD_SIZE
#undef BUFFER_FILENAME_SIZE_WITH_END_CHAR
#undef BUFFER_FIELD_SIZE_WITH_END_CHAR
#undef MAXIMUM_PROBABILITY_FOR_RANDOM
#undef BEGIN_REPEAT_TOTAL_NUMBER
#undef NOT_DRAWED_TYPE
#undef BEGIN_REPEAT_TOTAL_NUMBER
#endif //Drawer_h
