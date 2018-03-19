#ifndef Shuffler_h
#define Shuffler_h

#include "Config.h"
#include <CSVFile.h>
#include "FileTools.h"
#include "LFSR.h"
#include "LcdTools.h"
#include "ConfigurationFile.h"
#include "SDCardTools.h"
#include "YesNoAcceptFrame.h"

#define MAXIMUM_LINE_TO_SHUFFLE 255
#define MINIMUM(a,b) (a<b?a:b)

namespace Shuffler
{
  void ShufflePart(CSVFile * fileToShuffle, CSVFile * shuffledFile, const unsigned int fromLine, byte lineToShuffle, const byte seed)
  {
    LFSR randomGenerator;
    randomGenerator.setup(seed, lineToShuffle);

    // If we continue previous shuffle we should go to next line.
    if (shuffledFile->curPosition() != 0)
      shuffledFile->addLine();

    while (lineToShuffle-- > 0)
    {
      fileToShuffle->gotoLine(fromLine + randomGenerator.next());
      do
      {
        fileToShuffle->copyField(shuffledFile);
      } while (fileToShuffle->nextField() && shuffledFile->addField());
      
      //If isn't last line. The last CSV file must not contain end line character.
      if (lineToShuffle > 0)
        shuffledFile->addLine();
    }
  }
  
  // File should be open
  void Shuffle(char * filenameToShuffle, unsigned int lineNumbers, const byte seed)
  {
    CSVFile fileToShuffle;

    if (!fileToShuffle.open(filenameToShuffle, O_RDWR))
      return;
    
    unsigned int currentShuffleLine = 0;
    byte numberLineToShuffle = 0;

    CSVFile shuffleResult;
    shuffleResult.open(TEMPORARY_SHUFFLE_FILENAME, O_RDWR | O_CREAT | O_TRUNC);

    while (lineNumbers > 0)
    {
      numberLineToShuffle = MINIMUM(lineNumbers, MAXIMUM_LINE_TO_SHUFFLE);
      ShufflePart(&fileToShuffle, &shuffleResult, currentShuffleLine, numberLineToShuffle, seed);
      currentShuffleLine += numberLineToShuffle;
      lineNumbers -= numberLineToShuffle;
    }

    // Slowly but less memory.
    FileTools::copyFile(&shuffleResult, &fileToShuffle);
    fileToShuffle.close();
    shuffleResult.close();
  }

  void ShuffleCurrentDictionary()
  {
    YesNoAcceptFrame shuffleConfirm(F(LANG_STR_SHUFFLE_CONFIRM));
    shuffleConfirm.show();
    if (shuffleConfirm.getSelectedAction() != ACCEPT_ACTION)
      return;

    LcdTools::writeFullscreenMessage(F(LANG_STR_SHUFFLE_PROGRESS_IN_PROGRESS_MESSAGE));
    
    char buffer_[FILENAME_LIMIT_SIZE + 1];
    buffer_[FILENAME_LIMIT_SIZE] = '\0';
    byte seed = analogRead(PIN_RANDOM);
    unsigned int lineNumbers = 0;

    SdCardTools::initSdCard();
    FileTools::chdirToApplicationDir();
    
    {
      CSVFile csv;
      if (!ConfigurationFile::readConfigurationDictionaryName(&csv, buffer_, FILENAME_LIMIT_SIZE))
        return;
      csv.open(buffer_, O_RDWR);
      lineNumbers = FileTools::calculateNumberLine(&csv);
      csv.close();
    }
   
    // Shuffle progress
    Shuffle(buffer_, lineNumbers, seed);
    // Shuffle dictionary
    FileTools::chdirToDictionaryDir();
    LcdTools::writeFullscreenMessage(F(LANG_STR_SHUFFLE_DICTIONARY_IN_PROGRESS_MESSAGE));
    Shuffle(buffer_, lineNumbers, seed);
  }
}

#undef MAXIMUM_LINE_TO_SHUFFLE
#undef MINIMUM
#endif // Shuffler_h
