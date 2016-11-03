#ifndef Shuffler_h
#define Shuffler_h

#include "Config.h"
#include <CSVFile.h>
#include "FileTools.h"
#include "LFSR.h"
#include "LcdTools.h"
#include "ConfigurationFile.h"
#include "SDCardTools.h"

#define MAXIMUM_LINE_TO_SHUFFLE 255
#define MINIMUM(a,b) (a<b?a:b)

namespace Shuffler
{
  void ShufflePart(CSVFile * fileToShuffle, CSVFile * shuffledFile, unsigned int fromLine, byte lineToShuffle, byte seed)
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
        shuffledFile->addField();
        fileToShuffle->copyField(shuffledFile);
      } while (fileToShuffle->nextField());
      
      //If isn't last line. The last CSV file must not contain end line character.
      if (lineToShuffle > 0)
        shuffledFile->addLine();
    }
  }
  
  // File should be open
  void Shuffle(CSVFile * fileToShuffle, char * filenameToShuffle, unsigned lineNumbers, byte seed)
  {
    unsigned int currentShuffleLine = 0;
    byte numberLineToShuffle = 0;

    CSVFile shuffleResult;
    shuffleResult.open(TEMPORARY_SHUFFLE_FILENAME, O_RDWR | O_CREAT);
    shuffleResult.truncate(0);  //Remove file content if exist.

    while (lineNumbers > 0)
    {
      numberLineToShuffle = MINIMUM(lineNumbers, MAXIMUM_LINE_TO_SHUFFLE);
      ShufflePart(fileToShuffle, &shuffleResult, currentShuffleLine, numberLineToShuffle, seed);
      currentShuffleLine += numberLineToShuffle;
      lineNumbers -= numberLineToShuffle;
    }
    
    fileToShuffle->close();
    fileToShuffle->remove();
    shuffleResult.rename(shuffleResult.cwd(), filenameToShuffle);
    shuffleResult.close();
  }

  void ShuffleCurrentDictionary()
  {
    LcdTools::writeFullscreenMessage(F(LANG_STR_SHUFFLE_PROGRESS_IN_PROGRESS_MESSAGE));

    SdFat sd;
    CSVFile csv;
    char buffer_[FILENAME_LIMIT_SIZE + 1];
    buffer_[FILENAME_LIMIT_SIZE] = '\0';
    byte seed = analogRead(PIN_RANDOM);

    SdCardTools::initSdCard(&sd);
    FileTools::chdirToApplicationDir(&sd);
    ConfigurationFile::readConfigurationDictionaryName(&csv, buffer_, FILENAME_LIMIT_SIZE);

    if (!csv.open(buffer_, O_RDWR))
      return;

    unsigned int lineNumbers = FileTools::calculateNumberLine(&csv);
    // Shuffle progress
    Shuffle(&csv, buffer_, lineNumbers, seed);
    // Shuffle dictionary
    if (!csv.open(buffer_, O_RDWR))
      return;

    // Shuffle dictionary
    FileTools::chdirToDictionaryDir(&sd);
    LcdTools::writeFullscreenMessage(F(LANG_STR_SHUFFLE_DICTIONARY_IN_PROGRESS_MESSAGE));
    Shuffle(&csv, buffer_, lineNumbers, seed);
  }
}

#undef MAXIMUM_LINE_TO_SHUFFLE
#undef MINIMUM
#endif // Shuffler_h
