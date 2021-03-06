#ifndef ChooseDictionaryFrame_h
#define ChooseDictionaryFrame_h

#include "Lang.h"
#include "Config.h"
#include "ChooseFileFrame.h"
#include <SdFat.h>
#include <CSVFile.h>
#include "CSVSpecifications.h"
#include "FileTools.h"
#include "ConfigurationFile.h"
#include "YesNoAcceptFrame.h"
#include "LcdTools.h"
#include "Tools.h"
#include "ProgressFile.h"
#include "LearnFlow.h"

extern SdFat g_sd;

/* 
 * Frame for choose dictionary, save dictionary filename in configuration
 * file and generate progress file if needed.
 */
class ChooseDictionaryFrame : public ChooseFileFrame {
private:
  // Progress file has the same name as dictionary, but is stored
  // in application directory.
  void generateProgressFile(const char * fileName) {
    if (!LearnFlow::shouldCreateNewProgressFile(fileName))
      return;
    
    LcdTools::writeFullscreenMessage(F(LANG_STR_INIT_DICTIONARY_MESSAGE), true);
    
    CSVFile * csv = new CSVFile();
    ProgressFile::createProgressFile(csv, fileName);
    delete csv;
  }
  
  void saveInConfiguration(const char * fileNameDictionary) const
  {
    CSVFile * csv = new CSVFile();
    ConfigurationFile::editConfigurationDictionaryName(csv, fileNameDictionary);
    delete csv;
  }

protected:
  // Method from IBaseFrame
  void onPositionSelect(const byte position_) override {
    if (position_ == NOT_SELECTED_POSITION)
      return;

    const char * fileName = getFileName(position_);
    generateProgressFile(fileName);
    FileTools::chdirToApplicationDir();
    saveInConfiguration(fileName);
    // When we change dictionary we remove session file with previous
    // dictionary data.
    g_sd.remove(SESSION_SET_FILENAME);  // Return false if session file not exist
  }

  byte findStartPosition(const char * dictName)
  {
    byte i = 0;
    for (i = 0; i < m_numPositions; ++i)
    {
      if (StringTools::equals(dictName, getFileName(i)))
        return i;
    }
    return 0; //Default
  }

  // Method from ChooseFileFrame
  // Set start position frame as position with current choose dictionary
  bool init() override
  {
    if (!ChooseFileFrame::init())
      return false;

    char buffer_[FILENAME_LIMIT_SIZE + 1];
    buffer_[FILENAME_LIMIT_SIZE] = '\0';

    CSVFile * csv = new CSVFile();

    FileTools::chdirToApplicationDir();    

    if (ConfigurationFile::readConfigurationDictionaryName(csv, buffer_, FILENAME_LIMIT_SIZE))
    {
      setStartPosition(findStartPosition(buffer_));
    }

	  delete csv;
	
    return true;
  }
  
public:
  ChooseDictionaryFrame(const __FlashStringHelper * header) : ChooseFileFrame(DICTIONARY_DIR) {
    m_header = header;
  }
  ~ChooseDictionaryFrame() {
  };
};

#endif





