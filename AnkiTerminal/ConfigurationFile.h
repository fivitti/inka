#ifndef ConfigurationFile_h
#define ConfigurationFile_h

#include <SdFat.h>
#include <CSVFile.h>
#include "Config.h"
#include "CSVSpecifications.h"
#include "FileTools.h"

namespace ConfigurationFile
{
  bool createConfigurationFile(SdFat * sd, CSVFile * csv)
  {
    sd->chdir();
    sd->mkdir(APPLICATION_DIR);
    sd->chdir(APPLICATION_DIR);

    if (sd->exists(CONFIGURATION_FILENAME))
      return true;

    csv->open(CONFIGURATION_FILENAME, O_RDWR | O_CREAT);

    csv->gotoBeginOfFile();
    csv->addField(MAX_CARD_PER_SESSION_DEFAULT, CSV_FIELD_CONFIG_LEARN_DRAW_MAXIMUM_CARD_PER_SESSION_SIZE);
    csv->addField(DRAW_MODE_DEFAULT, CSV_FIELD_CONFIG_LEARN_DRAW_MODE_SIZE);
    csv->addField(REPEAT_NUMBER_DEFAULT, CSV_FIELD_CONFIG_LEARN_DRAW_REPEAT_NUMBER_SIZE);
    csv->addLine();
    csv->addField(DECREASE_REPEAT_WHEN_GOOD_DEFAULT, CSV_FIELD_CONFIG_LEARN_SESSION_NUMBER_DECREASE_REPEAT_WHEN_GOOD_SIZE);
    csv->addField(INCREASE_REPEAT_WHEN_BAD_DEFAULT, CSV_FIELD_CONFIG_LEARN_SESSION_NUMBER_INCREASE_REPEAT_WHEN_BAD_SIZE);
    csv->addLine();
    csv->addField(EASY_DECREASE_PROBABILITY_DEFAULT, CSV_FIELD_CONFIG_LEARN_SUMMARY_EASY_DECREASE_PROBABILITY_SIZE);
    csv->addField(MEDIUM_DECREASE_PROBABILITY_DEFAULT, CSV_FIELD_CONFIG_LEARN_SUMMARY_MEDIUM_DECREASE_PROBABILITY_SIZE);
    csv->addField(HARD_INCREASE_PROBABILITY_DEFAULT, CSV_FIELD_CONFIG_LEARN_SUMMARY_HARD_INCREASE_PROBABILITY_SIZE);
    csv->addField(FORGOTEN_INCREASE_PROBABILITY_DEFAUT, CSV_FIELD_CONFIG_LEARN_SUMMARY_FORGET_INCREASE_PROBABILITY_SIZE);
    csv->addField(CSV_FIELD_CONFIG_LEARN_SUMMARY_HARD_WHEN_NUMBER_BAD_ANSWER_WHEN_OTHERS_NORMAL_DEFAULT, CSV_FIELD_CONFIG_LEARN_SUMMARY_HARD_WHEN_NUMBER_BAD_ANSWER_WHEN_OTHERS_NORMAL_SIZE);
    csv->addLine();
    csv->addField(BEGIN_PROBABILITY_DEFAULT, CSV_FIELD_CONFIG_LEARN_INIT_PROGRESS_BEGIN_PROBABILITY_SIZE);
    csv->addLine();
    csv->addField(LCD_CONTRAST_DEFAULT, CSV_FIELD_CONFIG_LCD_CONTRAST_SIZE);
    csv->addField(LCD_BACKLIGHT_DEFAULT, CSV_FIELD_CONFIG_LCD_BACKLIGHT_SIZE);
    csv->addLine(); //Dictionary

    csv->close();
    return true;
  }

  //Required set SD to application directory
  //Required outputArray of size of number field of line
  void readConfigurationLine(CSVFile * csv, byte * outputArray, byte numLine)
  {
    char buffer_[MAXIMUM_NUMBER_FIXED_FIELD_SIZE + 1];
    buffer_[MAXIMUM_NUMBER_FIXED_FIELD_SIZE] = '\0';
    int numBuffer = 0;

    csv->open(CONFIGURATION_FILENAME, O_RDWR);
    csv->gotoLine(numLine);
    do
    {
      csv->readField(numBuffer, buffer_, MAXIMUM_NUMBER_FIXED_FIELD_SIZE);
      *outputArray++ = numBuffer;
    }
    while (csv->nextField());
    csv->close();
  }

  void readConfigurationField(CSVFile * csv, int& out, const byte numLine, const byte numField)
  {
    char buffer_[MAXIMUM_NUMBER_FIXED_FIELD_SIZE + 1];
    buffer_[MAXIMUM_NUMBER_FIXED_FIELD_SIZE] = '\0';

    csv->open(CONFIGURATION_FILENAME, O_RDWR);
    csv->gotoLine(numLine);
    csv->gotoField(numField);
    csv->readField(out, buffer_, MAXIMUM_NUMBER_FIXED_FIELD_SIZE);
    
    csv->close();
  }

  //Required set SD to application directory
  //Required bufferOut of size greater or equals of filename lenght
  void readConfigurationDictionaryName(CSVFile * csv, char * bufferOut, const byte bufferSize)
  {
    csv->open(CONFIGURATION_FILENAME, O_RDWR);
 
    csv->gotoLine(CSV_LINE_CONFIG_DICTIONARY);
    byte readingBytes = csv->readField(bufferOut, bufferSize);
    readingBytes = bufferSize - readingBytes;  //To clean end buffer bytes
    while (readingBytes > 0)
    {
      bufferOut[bufferSize - readingBytes--] = '\0';
    }
    csv->close();
  }

  //Required set SD to application directory
  //Field to edit should have right size for new value
  void editConfigurationField(CSVFile * csv, const byte newValue, const byte numLine, const byte numField)
  {
    csv->open(CONFIGURATION_FILENAME, O_RDWR);
    csv->gotoLine(numLine);
    csv->gotoField(numField);
    csv->editField(newValue);
    csv->close();
  }
}

#endif //ConfigurationFile_h
