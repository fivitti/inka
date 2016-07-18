#ifndef ChooseDictionaryFrame_h
#define ChooseDictionaryFrame_h

#include "Config.h"
#include "ChooseFileFrame.h"
#include <CSVFile.h>
#include "CSVSpecifications.h"
#include "FileTools.h"
#include "ConfigurationFile.h"
#include "SimplyAcceptFrame.h"
#include "LcdTools.h"
#include "Tools.h"

class ChooseDictionaryFrame : public ChooseFileFrame {
private:
  void writeHeader() override {
    IFrameBase::writeHeader();
    lcdWriteString(F("   Słowniki"));
  }

  bool isExistApplicationFile(const char * fileName) {
    sd.chdir();
    sd.chdir(APPLICATION_DIR);
    return sd.exists(fileName); 
  }

  void generateProgressFile(const char * fileName) {
    if (FileTools::isExistApplicationFile(&sd, fileName))
    {
      SimplyAcceptFrame deleteProgress(F("Usunąć postęp?"));
      deleteProgress.show();
      if (deleteProgress.getSelectedAction() == ACCEPT_ACTION)
      {
        sd.remove(fileName);
      }
      else
      {
        return;
      }
      
    }
    
    writeMsg(F(" Inicjuję")); 

    FileTools::chdir(&sd, DICTIONARY_DIR);
    
    CSVFile * csv = new CSVFile();
    csv->open(fileName, O_RDWR);
    csv->gotoBeginOfFile();
    while (csv->nextLine())
      ; //Skip
    unsigned int numLine = csv->getNumberOfLine();
    csv->close();
    
    FileTools::chdir(&sd, APPLICATION_DIR);

    int beginProbability = BEGIN_PROBABILITY_DEFAULT;
    ConfigurationFile::readConfigurationField(csv, beginProbability, CSV_LINE_CONFIG_LEARN_INIT_PROGRESS, CSV_FIELD_CONFIG_LEARN_INIT_PROGRESS_BEGIN_PROBABILITY);
    
    csv->open(fileName, O_RDWR | O_CREAT);

    for (unsigned int i=0; i<numLine; i++) {
      // probabilityQuestion;probabilityAnswer
      csv->addField(beginProbability, CSV_FIELD_PROGRESS_PROBABILITY_SIZE);
      csv->addField(beginProbability, CSV_FIELD_PROGRESS_PROBABILITY_SIZE);
      csv->addLine();
    }

    csv->addField(beginProbability, CSV_FIELD_PROGRESS_PROBABILITY_SIZE);
    csv->addField(beginProbability, CSV_FIELD_PROGRESS_PROBABILITY_SIZE);
    csv->close();
       
    delete csv;
  }

  void removeSessionFileIfExist()
  {
    if (FileTools::isExistApplicationFile(&sd, SESSION_SET_FILENAME))
    {
      sd.remove(SESSION_SET_FILENAME);
    }
  }
  
  void saveInConfiguration(const char * fileNameDictionary)
  {
    FileTools::chdir(&sd, APPLICATION_DIR);
    CSVFile * csv = new CSVFile();
    csv->open(CONFIGURATION_FILENAME, O_RDWR);
    csv->gotoLine(CSV_LINE_CONFIG_DICTIONARY);
    csv->addField(fileNameDictionary);
    csv->clearToEnd();
    csv->close();
    delete csv;
  }

protected:

  void onPositionSelect(byte position_) override {
    if (position_ == NOT_SELECTED_POSITION)
      return;

    const char * fileName = getFileName(position_);
    generateProgressFile(fileName);
    saveInConfiguration(fileName);
    removeSessionFileIfExist();
  }

  void init() override
  {
    ChooseFileFrame::init();

    char buffer_[FILENAME_LIMIT_SIZE + 1];
    buffer_[FILENAME_LIMIT_SIZE] = '\0';

    CSVFile * csv = new CSVFile();

    FileTools::chdir(&sd, APPLICATION_DIR);
    ConfigurationFile::readConfigurationDictionaryName(csv, buffer_, FILENAME_LIMIT_SIZE);

    delete csv;

    if (!StringTools::isEmpty(buffer_))
    {
      LcdTools::writeFullscreenMessage(F("Obecny słownik"), buffer_);
      delay(HUMAN_ERROR_DELAY);
    }
  }
  
public:
  ChooseDictionaryFrame() : ChooseFileFrame(DICTIONARY_DIR) {
    
  }
  ~ChooseDictionaryFrame() {
  };
};

#endif





