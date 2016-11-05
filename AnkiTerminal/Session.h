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
#include "Lang.h"

#define END_SESSION_ACTION 0

#define SESSION_HEADER_HEIGHT 1         //in rows
#define SESSION_HEADER_TEXT LANG_STR_TEXT_IN_PROGRESS_SESSION
#define SESSION_HEADER_TEXT_SIZE 4      //in characters
#define SESSION_HEADER_MODE_SIZE 1      //in characters
#define BUFFER_SIZE 10//Should be greater then @CSV_FIELD_CONFIG_LEARN_DRAW_REPEAT_NUMBER_SIZE
#define SAFE_BUFFER_SIZE (BUFFER_SIZE+1)

#define GOOD_DECISION BUTTON_UP
#define MEDIUM_DECISION BUTTON_MIDDLE
#define BAD_DECISION BUTTON_DOWN

#define SYMBOL_QUESTION LANG_STR_SYMBOL_QUESTION
#define SYMBOL_ANSWER LANG_STR_SYMBOL_ANSWER
#define SYMBOL_TYPE_CARD_SIZE 1         //in characters

#define OK_PROCESSED 0 

#define MENU_AS_SHUTDOWN(btn) (btn==BUTTON_MENU?SHUTDOWN_ACTION:btn)

/*
 * Class perform learn session.
 * It using session file for display cards.
 * User rate knowledge each cards. The cards good knowledge by the user
 * are display less then cards known worse.
 */
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
  //Read parameters from session file.
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

  //This method in searching the session file for not-learned file.
  //When session file is new all cards are not-learned, but if it
  //restore session file part of cards can be learned.
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

  /* Write the header in format:
   * AAAA S 111/222
   * Where AAAA - short name application (size 4)
   *       S    - symbol answer or question (size 1).
   *              Symbol is write in other function.
   *       111  - number of cards to learn (size 1-3)
   *       222  - total number of cards (size 1-3)
   */
  #define NUM_CARDS_DELIMITER '/'
  #define NUM_CARDS_DELIMITER_SIZE 1
  void writeHeader()
  {
    MinLcd::clearArea(0, 0, LCD_COLUMN_NUM, SESSION_HEADER_HEIGHT);
    MinLcd::lcdXY(0,0);
    MinLcd::lcdWriteString(F(SESSION_HEADER_TEXT));

    MinLcd::writeBlankSpaces(MAX_CHARS_IN_ROW -
                     SESSION_HEADER_TEXT_SIZE -
                     NumberTools::numberOfDigits(m_toLearnCards) -
                     NUM_CARDS_DELIMITER_SIZE -
                     NumberTools::numberOfDigits(m_totalCards));
    MinLcd::lcdWriteNumber(m_toLearnCards);
    MinLcd::lcdWriteCharacter(NUM_CARDS_DELIMITER);
    MinLcd::lcdWriteNumber(m_totalCards);
  }
  #undef NUM_CARDS_DELIMITER
  #undef NUM_CARDS_DELIMITER_SIZE

  //Write symbol. It inform user about if displayed verb is question or answer.
  void writeSymbol(const char symbol) const
  {
    MinLcd::lcdXY((SESSION_HEADER_TEXT_SIZE + 1) * CHAR_WIDTH, 0); //1 space after session title header
    MinLcd::lcdWriteCharacter(symbol);
  }

  //Required gotoBeginOfLine()
  //Set pointer SD card to end of field
  //Length of field should be less then content area
  //Content area has size: @LCD_COLUMN_NUM * (@LCD_ROW_NUM - 1) = 420 characters.
  void writeCurrentField()
  {
    MinLcd::clearArea(0, SESSION_HEADER_HEIGHT);
    MinLcd::lcdXY(0,SESSION_HEADER_HEIGHT);

    byte numReading = m_csv.readField(m_buffer, BUFFER_SIZE);
    while (numReading != 0)
    {
      m_buffer[numReading] = '\0';
      MinLcd::lcdWriteString(m_buffer);
      numReading = m_csv.readField(m_buffer, BUFFER_SIZE);
    }
  }

  //By using LFSR user will not wait indefinitely.
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

  //Save progress in session file (not in progress file).
  //Arguments @repeatToEnd and @repeatTotal represent state before rate by user.
  void saveProgress(const byte decision, byte repeatToEnd, const byte repeatTotal)
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
//        case BUTTON_MENU:  //Support in prevoius function
//          break;
        default:
          break;
    }

    if (repeatToEnd == 0)
      m_toLearnCards -= 1;
      
    m_csv.gotoField(CSV_FIELD_SESSION_REPEAT_TO_END);
    m_csv.editField(repeatToEnd);
    m_csv.nextField(); //or m_csv.gotoField(CSV_FIELD_SESSION_REPEAT_TOTAL);
    
    m_csv.editField(repeatTotal < CSV_FIELD_MAXIMUM_REPEAT ? repeatTotal + 1 : CSV_FIELD_MAXIMUM_REPEAT);
  }

  //All operation for single card
  //1. Find not learned card
  //2. Display question
  //3. Wait for any button
  //4. Display answer
  //5. Wait for rate
  //6. Save progress
  byte performSingleCard()
  {
    byte repeatToEnd = 0;
    byte repeatTotal = 0;
    findAndGoToNextNotLearnedCard(repeatToEnd, repeatTotal);
    
    m_csv.gotoField(CSV_FIELD_SESSION_QUESTION);
    writeSymbol(SYMBOL_QUESTION);
    writeCurrentField();

    Buttons::waitForUnpress();    
    RETURN_WHEN_SHUTDOWN(MENU_AS_SHUTDOWN(Buttons::waitForKey()));

    m_csv.gotoField(CSV_FIELD_SESSION_ANSWER);   
    writeSymbol(SYMBOL_ANSWER);
    writeCurrentField();
    
	  Buttons::waitForUnpress();
	  byte decision = Buttons::waitForKey();
	  RETURN_WHEN_SHUTDOWN(MENU_AS_SHUTDOWN(decision));

      saveProgress(decision, repeatToEnd, repeatTotal);

	  return decision;
  }
  
  public:
  Session(SdFat * sd) : m_sd(sd){
    m_buffer[BUFFER_SIZE] = '\0'; //Safe buffer
  }
  
  //This method should be call before @executeSession.
  void configure()
  {
    LcdTools::writeFullscreenMessage(F(LANG_STR_SESSION_INIT_MESSAGE));
    FileTools::chdirToApplicationDir(m_sd);
    
    byte sessionConfiguration[CSV_LINE_CONFIG_LEARN_SESSION_SIZE];
    ConfigurationFile::readConfigurationLine(&m_csv, sessionConfiguration, CSV_LINE_CONFIG_LEARN_SESSION);

    m_decreaseWhenGood = sessionConfiguration[CSV_FIELD_CONFIG_LEARN_SESSION_NUMBER_DECREASE_REPEAT_WHEN_GOOD];
    m_increaseWhenBad = sessionConfiguration[CSV_FIELD_CONFIG_LEARN_SESSION_NUMBER_INCREASE_REPEAT_WHEN_BAD];

    calculateLearnCards();

    m_generator.setup(random(MINIMAL_SEED_FOR_LFSR, m_totalCards), m_totalCards);
  }

  // Run learn flow.
  // It end when:
  // - user learned all cards
  // - user press MENU BUTTON - then session is save and user can restore its.
  byte executeSession()
  {
    m_csv.open(SESSION_SET_FILENAME, O_RDWR);
    
    MinLcd::clearLcd();

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
