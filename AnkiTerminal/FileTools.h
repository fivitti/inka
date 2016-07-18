#ifndef FileTools_h
#define FileTools_h

#include <SdFat.h>
#include "Config.h"

namespace FileTools
{
  void chdir(SdFat * sd, const char * directory)
  {
    sd->chdir();
    sd->chdir(directory);
  }
  
  bool isExistFile(SdFat * sd, const char * directory, const char * fileName)
  {
    FileTools::chdir(sd, directory);
    return sd->exists(fileName); 
  }
  
  bool isExistApplicationFile(SdFat * sd, const char * fileName) {
    return FileTools::isExistFile(sd, APPLICATION_DIR, fileName);
  }
}


#endif
