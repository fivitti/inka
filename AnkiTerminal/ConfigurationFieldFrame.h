#ifndef ConfigurationFieldFrame_h
#define ConfigurationFieldFrame_h

#include <SdFat.h>
#include <CSVFile.h>
#include "Config.h"
#include "IFrameBase.h"
#include "ConfigurationData.h"
#include "MinLcd.h"
#include "ConfigurationFile.h"
#include "SdCardTools.h"
#include "FileTools.h"

/*
 * Universal class for frame to change value of configuration field.
 * User see header and can select numberic option.
 * Range of option is continous and total.
 * The start position is set to current value.
 * After select value is update in configuration file.
 */
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
    MinLcd::lcdWriteNumber(index);
  }

public:
  ConfigurationFieldFrame(const ConfigurationData * configurationData) : IFrameBase(), m_configurationData(configurationData)
  {
    m_header = m_configurationData->m_header;
  }
  ~ConfigurationFieldFrame() {}

  void show() override {
    m_numPositions = m_configurationData->m_maximum;

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