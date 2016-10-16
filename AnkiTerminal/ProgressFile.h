#ifndef ProgressFile_h
#define ProgressFile_h

#include <SdFat.h>
#include <CSVFile.h>
#include "Lang.h"
#include "Config.h"
#include "FileTools.h"
#include "ConfigurationFile.h"

/*
 * Utility for create progress file. See @Sumary class for show how this file is edit.
 * Specification this file is in @CSVSpecification.h file.
 * Each line of progress file is coresponding to line in dictionary file.
 */
namespace ProgressFile
{
  void createProgressFile(SdFat * sd, CSVFile * csv, const char * dictionaryFilename)
  {
    FileTools::chdirToDictionaryDir(sd);

    csv->open(dictionaryFilename, O_RDWR);
    unsigned int numLine = FileTools::calculateNumberLine(csv);
    csv->close();

    FileTools::chdirToApplicationDir(sd);

    int beginProbability = BEGIN_PROBABILITY_DEFAULT;
    ConfigurationFile::readConfigurationField(csv, beginProbability, CSV_LINE_CONFIG_LEARN_INIT_PROGRESS, CSV_FIELD_CONFIG_LEARN_INIT_PROGRESS_BEGIN_PROBABILITY);

    csv->open(dictionaryFilename, O_RDWR | O_CREAT);

    for (unsigned int i = 0; i<numLine - 1; i++) {
      // probabilityQuestion;probabilityAnswer
      csv->addField(beginProbability, CSV_FIELD_PROGRESS_PROBABILITY_SIZE);
      csv->addField(beginProbability, CSV_FIELD_PROGRESS_PROBABILITY_SIZE);
      csv->addLine();
    }

    csv->addField(beginProbability, CSV_FIELD_PROGRESS_PROBABILITY_SIZE);
    csv->addField(beginProbability, CSV_FIELD_PROGRESS_PROBABILITY_SIZE);
    csv->close();
  }
}

#endif
