#ifndef ResetConfigFrame_h
#define ResetConfigFrame_h

#include "YesNoAcceptFrame.h"
#include <SdFat.h>
#include "SdCardTools.h"
#include "FileTools.h"
#include "Config.h"
#include "Lang.h"
#include "ConfigurationFile.h"
#include <CSVFile.h>

/*
 * If user choose @ACCEPT_ACTION (yes) then current configuration
 * file will be delete and recreate with default values.
 */
class ResetConfigFrame : public YesNoAcceptFrame
{

protected:
  void onPositionSelect(byte position_) override {
    YesNoAcceptFrame::onPositionSelect(position_);

    if (position_ == ACCEPT_ACTION)
    {
      SdFat sd;
      SdCardTools::initSdCard(&sd);
      CSVFile csv;
      ConfigurationFile::resetConfigurationFile(&sd, &csv);
    }
  }

public:
  ResetConfigFrame() : YesNoAcceptFrame(F(LANG_STR_RESET_CONFIG_POPUP)) {
  };
  ~ResetConfigFrame() {};
};


#endif // !ResetConfigFrame_h
