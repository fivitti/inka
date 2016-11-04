#ifndef FileTools_h
#define FileTools_h

#include <SdFat.h>
#include <CSVFile.h>
#include "Config.h"

/* Some helpful utilites for work with files and filesystem encapsulate the more often operations.
 */
namespace FileTools
{
  // Change dir to other directory in the root SD card directory.
  void chdir(SdFat * sd, const char * directory)
  {
    sd->chdir();
    sd->mkdir(directory);
    sd->chdir(directory);
  }

  void chdirToDictionaryDir(SdFat * sd)
  {
    chdir(sd, DICTIONARY_DIR);
  }

  void chdirToApplicationDir(SdFat * sd)
  {
    chdir(sd, APPLICATION_DIR);
  }
  
  // Check if in @directory exist @filename
  bool isExistFile(SdFat * sd, const char * directory, const char * filename)
  {
    FileTools::chdir(sd, directory);
    return sd->exists(filename); 
  }
  
  // Check if in application directory exist @filename
  bool isExistApplicationFile(SdFat * sd, const char * filename) {
    return FileTools::isExistFile(sd, APPLICATION_DIR, filename);
  }

  // Calculate number of line in file.
  // Required go to directory with file.
  // Lines was recognized by end of line character in UNIX style: '\n'
  // At end set position of file to end of file
  unsigned int calculateNumberLine(CSVFile * csv)
  { 
    csv->gotoBeginOfFile();
    while (csv->nextLine())
      ; //Skip
    unsigned int numLine = csv->getNumberOfLine();

    return numLine + 1;
  }

  void copyFile(SdBaseFile * sourceFile, SdBaseFile * targetFile)
  {
    sourceFile->seekSet(0);
    targetFile->truncate(0);
    int chVal = sourceFile->read();
    while (chVal >= 0)
    {
      targetFile->write((char)chVal);
      chVal = sourceFile->read();
    }
  }

}


#endif
