#ifndef ResetConfigFrame_h
#define ResetConfigFrame_h

#include "AbstractAcceptFrame.h"
#include <SdFat.h>
#include "SDCardTools.h"
#include "FileTools.h"
#include "Config.h"
#include "ConfigurationFile.h"
#include <CSVFile.h>

class ResetConfigFrame : public AbstractAcceptFrame
{

private:
  void writeHeader() override
  {
    AbstractAcceptFrame::writeHeader();
    lcdWriteString(F(" Reset opcji?"));
  }

protected:
  void onPositionSelect(byte position_) override {
    AbstractAcceptFrame::onPositionSelect(position_);

    if (position_ == ACCEPT_ACTION)
    {
      SdFat sd;
      SdCardTools::initSdCard(&sd);
      FileTools::chdir(&sd, APPLICATION_DIR);
      sd.remove(CONFIGURATION_FILENAME);
      CSVFile csv;
      ConfigurationFile::createConfigurationFile(&sd, &csv);
    }
  }

public:
  ResetConfigFrame() : AbstractAcceptFrame() {  };
  ~ResetConfigFrame() {};
};


#endif // !ResetConfigFrame_h
