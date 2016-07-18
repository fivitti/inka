#ifndef ConfigurationFieldFrame_h
#define ConfigurationFieldFrame_h

#include "IFrameBase.h"
#include "ConfigurationData.h"
#include "MinLcd.h"
#include "ConfigurationFile.h"
#include "SDCardTools.h"
#include <SdFat.h>
#include <CSVFile.h>
#include "FileTools.h"
#include "Config.h"

class ConfigurationFieldFrame : public IFrameBase
{
protected:
  const ConfigurationData * m_configurationData;

  virtual void onPositionSelect(byte position_) override
  {
    if (position_ == NOT_SELECTED_POSITION)
      return;

    SdFat * sd = new SdFat();;
    SdCardTools::initSdCard(sd);
    FileTools::chdir(sd, APPLICATION_DIR);
    CSVFile * csv = new CSVFile();

    ConfigurationFile::editConfigurationField(csv, position_, m_configurationData->m_numLine, m_configurationData->m_numField);

    delete csv;
    delete sd;
  }
  virtual void writePosition(byte index) override
  {
    lcdWriteNumber(index);
  }
  virtual void writeHeader() override
  {
    IFrameBase::writeHeader();
    lcdWriteString(m_configurationData->m_header);
  }

public:
  ConfigurationFieldFrame(const ConfigurationData * configurationData) : IFrameBase(), m_configurationData(configurationData)
  {
  }
  ~ConfigurationFieldFrame() {}

  void show() override {
    numPositions = m_configurationData->m_maximum;

    SdFat * sd = new SdFat();;
    SdCardTools::initSdCard(sd);
    FileTools::chdir(sd, APPLICATION_DIR);
    CSVFile * csv = new CSVFile();

    int currentValue;
    ConfigurationFile::readConfigurationField(csv, currentValue, m_configurationData->m_numLine, m_configurationData->m_numField);

    delete csv;
    delete sd;

    setStartPosition(currentValue);

    IFrameBase::show();
  }
};

#endif //ConfigurationFieldFrame_h