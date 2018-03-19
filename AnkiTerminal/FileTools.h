#ifndef FileTools_h
#define FileTools_h

#include <SdFat.h>
#include <CSVFile.h>
#include "Config.h"

extern SdFat g_sd;

/* Some helpful utilites for work with files and filesystem encapsulate the more often operations.
 */
namespace FileTools
{  
  // Change dir to other directory in the root SD card directory.
  void chdir(const char * directory)
  {
    g_sd.chdir();
    g_sd.mkdir(directory);
    g_sd.chdir(directory);
  }

  void chdirToDictionaryDir()
  {
    chdir(DICTIONARY_DIR);
  }

  void chdirToApplicationDir()
  {
    chdir(APPLICATION_DIR);
  }
  
  // Check if in @directory exist @filename
  bool isExistFile(const char * directory, const char * filename)
  {
    FileTools::chdir(directory);
    return g_sd.exists(filename); 
  }
  
  // Check if in application directory exist @filename
  bool isExistApplicationFile(const char * filename) {
    return FileTools::isExistFile(APPLICATION_DIR, filename);
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

  void copyFile(FatFile * sourceFile, FatFile * targetFile)
  {
    sourceFile->rewind();
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
