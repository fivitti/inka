#ifndef ChooseFileFrame_h
#define ChooseFileFrame_h

#include <SdFat.h>
#include "IFrameBase.h"
#include <SPI.h>
#include "MinLcd.h"
#include "SdCardTools.h"
#include "FileTools.h"
#include "LcdTools.h"
#include "Lang.h"

/*
 * Base frame for choose file. List all files in directory from constructor.
 * Current support only short filenames (13 chars - 9.3 format).
 * Current suppport maximum 254 position
 */

#define MAXIMUM_POSITIONS 254

extern SdFat g_sd;

class ChooseFileFrame : public IFrameBase {
private:
  SdFile file;
  const char * dirName;
  uint32_t * positionsFile;
  char fileNameBuffer[FILENAME_LIMIT_SIZE+1];  //Warning: Only for short filenames

  // Return number of files in directory (without recurent)
  byte getFileCount(FatFile * directory) {
    directory->rewind();
    byte count = 0;
    while (file.openNext(directory, O_READ)) {
      if (file.isFile())
        count += 1;
      file.close();
    }
    file.close(); //For safe
    
    return count;
  }

  // Find begin of position all files in directory.
  // Position is here location in the directory on the SD card.
  void findStartPositionFiles(FatFile * directory) {
    FileTools::chdir(dirName);
    directory->rewind();
    positionsFile = new uint32_t[m_numPositions];
    byte count = 0;
    while (file.openNext(directory, O_READ) && count < MAXIMUM_POSITIONS) {
      if (file.isFile()) {
        positionsFile[count] = directory->curPosition();
        count += 1;
      }
      file.close();
    }
  }
 
protected:

  // Return filename on position.
  // Position is here position in the frame.
  // Filename is print to buffer. It is clean up in destructor.
  const char * getFileName(const byte index) {
      FileTools::chdir(dirName);
      file.open(g_sd.vwd(), (positionsFile[index] / 32) - 1, O_READ); //Magic (with division). See SdFat documentation.
      file.getSFN(fileNameBuffer);
      file.close();
      return fileNameBuffer;
  }
   
  virtual void writePosition(const byte index) override {
      MinLcd::lcdWriteString(getFileName(index));
  }

  virtual bool init() {
    m_numPositions = getFileCount(g_sd.vwd());

    if (m_numPositions == 0) {
      LcdTools::writeFullscreenMessage(F(LANG_STR_MISSING_FILES_MESSAGE));
      delay(HUMAN_ERROR_DELAY);
      return false;
    }
      
    findStartPositionFiles(g_sd.vwd());

    return true;
  }

  void initSD(const char * dirName) {
    SdCardTools::initSdCard();
    FileTools::chdir(dirName);
  }

public:
  ChooseFileFrame(const char * dirName) : IFrameBase(), dirName(dirName) {
    fileNameBuffer[FILENAME_LIMIT_SIZE] = '\0'; //Safe buffer.
  }
  ~ChooseFileFrame() {
    delete[] positionsFile;
  };

  void show() override {
    initSD(dirName);
    init();
    IFrameBase::show();
  }
};

#undef MAXIMUM_POSITIONS
#endif //ChooseFileFrame.h





