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
    sd->chdir(directory);
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
  unsigned int calculateNumberLine(CSVFile * csv, const char * filename)
  {
    csv->open(filename, O_RDWR);
    csv->gotoBeginOfFile();
    while (csv->nextLine())
      ; //Skip
    unsigned int numLine = csv->getNumberOfLine();
    csv->close();
    return numLine + 1;
  }
}


#endif
