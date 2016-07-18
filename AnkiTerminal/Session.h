#ifndef Session_h
#define Session_h

#include <SdFat.h>
#include "Config.h"
#include "ConfigurationFile.h"
#include "CSVSpecifications.h"
#include "CSVFile.h"
#include "FileTools.h"
#include "MinLcd.h"
#include "LcdTools.h"
#include "Tools.h"
#include "LFSR.h"
#include "Buttons.h"

#define END_SESSION_ACTION 0

#define SESSION_HEADER_HEIGHT 1         //in rows
#define SESSION_HEADER_TEXT "INKA"
#define SESSION_HEADER_TEXT_SIZE 4
#define SESSION_HEADER_MODE_SIZE 1
#define BUFFER_SIZE 10//Should be greater then CSV_FIELD_CONFIG_LEARN_DRAW_REPEAT_NUMBER_SIZE
#define SAFE_BUFFER_SIZE BUFFER_SIZE+1

#define GOOD_DECISION BUTTON_UP
#define MEDIUM_DECISION BUTTON_MIDDLE
#define BAD_DECISION BUTTON_DOWN

#define SYMBOL_QUESTION 'P'
#define SYMBOL_ANSWER 'O'
#define SYMBOL_TYPE_CARD_SIZE 1

#define OK_PROCESSED 0 

#define MENU_AS_SHUTDOWN(btn) (btn==BUTTON_MENU?SHUTDOWN_ACTION:btn)

class Session
{
  private:
  byte m_decreaseWhenGood;
  byte m_increaseWhenBad;
  byte m_toLearnCards;
  byte m_totalCards;

  SdFat * m_sd;
  CSVFile m_csv;
  char m_buffer[SAFE_BUFFER_SIZE];
  int m_numBuffer;
  LFSR m_generator;
  
  //Required gotoBeginOfLine();
  void readRepeatInfo(byte& repeatToEnd, byte& repeatTotal)
  {
    //m_csv.gotoField(CSV_FIELD_SESSION_REPEAT_TO_END);
    m_csv.readField(m_numBuffer, m_buffer, BUFFER_SIZE);
    repeatToEnd = m_numBuffer;
    m_csv.nextField();
    //m_csv.gotoField(CSV_FIELD_SESSION_REPEAT_TOTAL);
    m_csv.readField(m_numBuffer, m_buffer, BUFFER_SIZE);
    repeatTotal = m_numBuffer;
  }

  void calculateLearnCards()
  {
    m_csv.open(SESSION_SET_FILENAME, O_RDWR);
    m_csv.gotoBeginOfFile();
    m_toLearnCards = 0;
    m_totalCards = 0;
    do 
    {
      m_csv.readField(m_numBuffer, m_buffer, BUFFER_SIZE);
      m_totalCards += 1;
      
      if (m_numBuffer > 0)
        m_toLearnCards += 1;
    }
    while (m_csv.nextLine());
    
    m_csv.close();
  }

  #define NUM_CARDS_DELIMITER '/'
  #define NUM_CARDS_DELIMITER_SIZE 1
  void writeHeader()
  {
    clearArea(0, 0, LCD_COLUMN_NUM, SESSION_HEADER_HEIGHT);
    lcdXY(0,0);
    lcdWriteString(F(SESSION_HEADER_TEXT));

    LcdTools::writeBlankSpace(MAX_CHARS_IN_ROW -
                              SESSION_HEADER_TEXT_SIZE -
                              NumberTools::numberOfDigits(m_toLearnCards) -
                              NUM_CARDS_DELIMITER_SIZE -
                              NumberTools::numberOfDigits(m_totalCards));
    lcdWriteNumber(m_toLearnCards);
    lcdWriteCharacter(NUM_CARDS_DELIMITER);
    lcdWriteNumber(m_totalCards);
  }
  #undef NUM_CARDS_DELIMITER
  #undef NUM_CARDS_DELIMITER_SIZE

  void writeSymbol(const char symbol)
  {
    //clearArea(SESSION_HEADER_TEXT_SIZE * CHAR_WIDTH, 0, (SESSION_HEADER_TEXT_SIZE + SYMBOL_TYPE_CARD_SIZE) * CHAR_WIDTH, 0);
    lcdXY((SESSION_HEADER_TEXT_SIZE + 1) * CHAR_WIDTH,0); //1 space after session title header
    lcdWriteCharacter(symbol);
  }

  //Required gotoBeginOfLine()
  //Set pointer to end of field
  void writeCurrentField()
  {
    clearArea(0, SESSION_HEADER_HEIGHT);
    lcdXY(0,SESSION_HEADER_HEIGHT);

    byte numReading = m_csv.readField(m_buffer, BUFFER_SIZE);
    while (numReading != 0)
    {
      //if (numReading < BUFFER_SIZE) //its safe buffer
      m_buffer[numReading] = '\0';
      lcdWriteString(m_buffer);
      numReading = m_csv.readField(m_buffer, BUFFER_SIZE);
    }
  }

  void findAndGoToNextNotLearnedCard(byte& repeatToEnd, byte& repeatTotal)
  {
    do
    {
      byte gen = m_generator.next();
      m_csv.gotoLine(gen);
      readRepeatInfo(repeatToEnd, repeatTotal);
    }
    while(repeatToEnd <= 0);
  }

  void saveProgress(byte decision, byte repeatToEnd, byte repeatTotal)
  {    
    switch(decision) {
        case GOOD_DECISION:
          repeatToEnd = repeatToEnd < m_decreaseWhenGood ? 0 : repeatToEnd - m_decreaseWhenGood;
          break;
        case MEDIUM_DECISION:
          repeatToEnd = repeatToEnd < DECREASE_REPEAT_WHEN_MEDIUM ? 0 : repeatToEnd - DECREASE_REPEAT_WHEN_MEDIUM;
          break;
        case BAD_DECISION:
          repeatToEnd = repeatToEnd > CSV_FIELD_MAXIMUM_REPEAT - m_increaseWhenBad ? CSV_FIELD_MAXIMUM_REPEAT : repeatToEnd + m_increaseWhenBad;
          break;
//        case BUTTON_MENU:  //Support in back function
//          break;
        default:
          break;
    }

    if (repeatToEnd == 0)
      m_toLearnCards -= 1;
      
    m_csv.gotoField(CSV_FIELD_SESSION_REPEAT_TO_END);
    m_csv.editField(repeatToEnd);
    m_csv.nextField();
    m_csv.editField(repeatTotal < CSV_FIELD_MAXIMUM_REPEAT ? repeatTotal + 1 : CSV_FIELD_MAXIMUM_REPEAT);
  }

  byte performSingleCard()
  {
    byte repeatToEnd = 0;
    byte repeatTotal = 0;
    findAndGoToNextNotLearnedCard(repeatToEnd, repeatTotal);
    
    m_csv.gotoField(CSV_FIELD_SESSION_QUESTION);
    writeSymbol(SYMBOL_QUESTION);
    writeCurrentField();

    waitForUnpress();
    byte decision = waitForKey();
    
    RETURN_WHEN_SHUTDOWN(MENU_AS_SHUTDOWN(decision));

    m_csv.gotoField(CSV_FIELD_SESSION_ANSWER);   
    writeSymbol(SYMBOL_ANSWER);
    writeCurrentField();
    
    saveProgress(decision, repeatToEnd, repeatTotal);

    waitForUnpress();
    return MENU_AS_SHUTDOWN(waitForKey());
  }
  
  public:
  Session(SdFat * sd) : m_sd(sd){
    m_buffer[BUFFER_SIZE] = '\0';
  }
  
  void configure()
  {
    LcdTools::writeFullscreenMessage(F("Inicjuję sesję"));
    FileTools::chdir(m_sd, APPLICATION_DIR);
    
    byte sessionConfiguration[CSV_LINE_CONFIG_LEARN_SESSION_SIZE];
    ConfigurationFile::readConfigurationLine(&m_csv, sessionConfiguration, CSV_LINE_CONFIG_LEARN_SESSION);

    m_decreaseWhenGood = sessionConfiguration[CSV_FIELD_CONFIG_LEARN_SESSION_NUMBER_DECREASE_REPEAT_WHEN_GOOD];
    m_increaseWhenBad = sessionConfiguration[CSV_FIELD_CONFIG_LEARN_SESSION_NUMBER_INCREASE_REPEAT_WHEN_BAD];

    calculateLearnCards();

    m_generator.setup(random(MINIMAL_SEED_FOR_LFSR, m_totalCards), m_totalCards);
  }

  byte executeSession()
  {
    m_csv.open(SESSION_SET_FILENAME, O_RDWR);
    
    clearLcd();

    byte shutdownStatus = OK_PROCESSED;
    while (m_toLearnCards > 0)
    {
      writeHeader();
      if (performSingleCard() == SHUTDOWN_ACTION)
      {
        shutdownStatus = SHUTDOWN_ACTION;
        break;
      }
      
    }
    
    m_csv.close();
    
    return shutdownStatus;
  }

};

#undef BUFFER_SIZE
#undef SAFE_BUFFER_SIZE
#undef SESSION_HEADER_HEIGHT
#undef SESSION_HEADER_TEXT
#undef SESSION_HEADER_TEXT_SIZE
#undef SESSION_HEADER_MODE_SIZE
#undef OK_PROCESSED

#undef GOOD_DECISION
#undef MEDIUM_DECISION
#undef BAD_DECISION

#undef MENU_AS_SHUTDOWN

#undef MODE_QUESTION
#undef MODE_ANSWER
#endif //Session_h
