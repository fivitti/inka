#ifndef ChooseFileFrame_h
#define ChooseFileFrame_h

#include "IFrameBase.h"
#include <SPI.h>
#include <SdFat.h>
#include "MinLcd.h"
#include "SdCardTools.h"
#include "FileTools.h"

class ChooseFileFrame : public IFrameBase {
private:
  const char * dirName;
  uint32_t * positionsFile;
  char fileNameBuffor[FILENAME_LIMIT_SIZE+1];  //Warning: Only for short filenames
//  void onPositionSelect(byte position_) override {
//    if (position_ == NOT_SELECTED_POSITION)
//      return;
//
////    file.open(sd.vwd(), (positionsFile[index] / 32) - 1, O_READ);
////    SdFile * configuration = new SdFile();
////    sd.mkdir(dirName);
////    
////
////    delete configuration;
//    //show();
//  }
  byte getFileCount(FatFile * directory) {
    directory->rewind();
    byte count = 0;
    while (file.openNext(directory, O_READ)) {
      if (file.isFile())
        count += 1;
      file.close();
    }
    file.close();
    return count;
  }

  void findStartPositionFiles(FatFile * directory) {
    FileTools::chdir(&sd, dirName);
    directory->rewind();
    positionsFile = new uint32_t[numPositions];
    byte count = 0;
    while (file.openNext(directory, O_READ)) {
      if (file.isFile()) {
        positionsFile[count] = directory->curPosition();
        count += 1;
      }
      file.close();
    }
  }
 
protected:
  SdFat sd;
  SdFile file;

  const char * getFileName(byte index) {
      FileTools::chdir(&sd, dirName);
      file.open(sd.vwd(), (positionsFile[index] / 32) - 1, O_READ);
      file.getSFN(fileNameBuffor);
      file.close();
      return fileNameBuffor;
  }
   
  virtual void writePosition(byte index) override {
      lcdWriteString(getFileName(index));
  }

  virtual void init() {
    numPositions = getFileCount(sd.vwd());
    findStartPositionFiles(sd.vwd());
  }

  bool initSD(const char * dirName) {
    SdCardTools::initSdCard(&sd);
    sd.chdir();
    sd.mkdir(dirName);
    sd.chdir(dirName);
    return true;
  }

public:
  ChooseFileFrame(const char * dirName) : IFrameBase(), dirName(dirName) {
    fileNameBuffor[FILENAME_LIMIT_SIZE] = '\0';
//    initSD(dirName);
    
  }
  ~ChooseFileFrame() {
    delete[] positionsFile;
//    delete[] dirName;
  };

  void show() override {
    initSD(dirName);
    init();
    IFrameBase::show();
  }
};
#endif //ChooseFileFrame.h





