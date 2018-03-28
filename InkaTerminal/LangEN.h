#ifndef LangPL_h
#define LangPL_h

/*
 * Instruction:
 * 1. Copy this content to your new header file.
 * 2. Translate text from "Localized" column to your language
 *    - Size of localized string don't can be longest then "Max size" column value
 *    - We support only characters from ASCII and "extend ASCII" table. See "Font.h" for list of all supported characters.
 *    - The characters from "extend ASCII" table have size 1 as normal characters.
 *    - Pay atention to notes
 *    - Type can be helpful for choosse the sounds of the words without research code
 *      + Message - short text display for user which inform about state of program or errors
 *      + Header - text display on the top of the frame. Inform about meaning positions
 *      + Position - text described position. User can choose one position for define how the program working
 *      + Popup - text displayed on the center the screen (on top is header)
 *      + Popup yes/no - User will be can choose one for option yes or no. Content of header should be question
 * 3. Set define @LANG to name your dictionary.
 */

/*
        ID                                      Localized                     Max size        Type              Notes
*/

// AccepDrawFrame
#define LANG_STR_ACCEPT_DRAW_TITLE_FIRST_PART   " Use "                  //  0-12            Part of header    This and second part together should have 12 size. Don't forget about one space at end this string. This text isn't centered. You should manually add spaces for align.
#define LANG_STR_ACCEPT_DRAW_TITLE_SECOND_PART  " cards?"                  //  0-12            Part of header    This and first part together should have 12 size. Don't forget about one space at begin this string.
// ChooseDictionaryFrame
#define LANG_STR_INIT_DICTIONARY_MESSAGE        "Initialise"                //  14              Message           Popup
// ChooseFileFrame
#define LANG_STR_MISSING_FILES_MESSAGE          "No files"             //  14              Message           Popup
// ChooseLangModeFrame
#define LANG_STR_FIRST_LANG                     "1. lang"                //  13              Position
#define LANG_STR_SECOND_LANG                    "2. lang"                //  13              Position             
#define LANG_STR_BOTH_RANDOM_LANG               "Randomly both"              //  13              Position
// Drawer
#define LANG_STR_MISSING_DICTIONARY_MESSAGE     "No dict"           //  14              Message           Popup
// LcdTools
#define LANG_STR_APPLICATION_TITLE_POPUP        "INKA"                    //  14              Header            Popup. If you change this value to longest then 4 characters then you should @LANG_STR_TEXT_IN_PROGRESS_SESSION too.
// LcdConfigurationLineFrame
#define LANG_STR_CONTRAST                       "Contrast"                //  13              Header/position
#define LANG_STR_BRIGHTNESS                     "Brightness"           //  13              Header/position
// LearnFlow
#define LANG_STR_CONTINUE_SESSION_POPUP         "Continue?"            //  14              Header            Popup yes/no
#define LANG_STR_DRAW_IN_PROGRESS_MESSAGE       "Drawing..."               //  14              Message           Popup
#define LANG_STR_CONTINUE_PROGRESS_POPUP        "Resume learn?"          //  14              Header            Popup yes/no
// ResetConfigFrame
#define LANG_STR_RESET_CONFIG_POPUP             "Reset config?"            //  14              Header            Popup yes/no
// RootFrame
#define LANG_STR_DRAW_MAXIMUM_CARD_PER_SESSION  "Max card/game"            //  13              Header/position
#define LANG_STR_DRAW_MODE_QUESTION_LANG        "Question lang"             //  13              Header/position
#define LANG_STR_DRAW_REPEAT_NUMBER             "Repeat count"           //  13              Header/position
#define LANG_STR_DRAW_HEADER                    "Drawing"               //  13              Header/position
#define LANG_STR_SESSION_NUMBER_DECREASE_REPEAT_WHEN_GOOD "+ good answer" //  13              Header/position
#define LANG_STR_SESSION_NUMBER_INCREASE_REPEAT_WHEN_BAD  "- bad answer"   //  13              Header/position
#define LANG_STR_SESSION_HEADER                 "Session"                   //  13              Header/position
#define LANG_STR_SUMMARY_EASY_DECREASE_PROBABILITY    "- for easy"        //  13              Header/position
#define LANG_STR_SUMMARY_MEDIUM_DECREASE_PROBABILITY  "- for medium"      //  13              Header/position
#define LANG_STR_SUMMARY_HARD_INCREASE_PROBABILITY    "+ for hard"       //  13              Header/position
#define LANG_STR_SUMMARY_FORGET_INCREASE_PROBABILITY  "+ for forgot"     //  13              Header/position
#define LANG_STR_SUMMARY_HARD_WHEN_NUMBER_BAD_ANSWER_WHEN_OTHERS_NORMAL "x bad=hard"  // 13 Header/position
#define LANG_STR_SUMMARY_HEADER                 "Summary"            //  13              Header/position
#define LANG_STR_INIT_PROGRESS_BEGIN_PROBABILITY  "Chance new"         //  13              Header/position
#define LANG_STR_INIT_HEADER                    "Initialise"           //  13              Header/position
#define LANG_STR_LCD                            "LCD"                     //  13              Header/position
#define LANG_STR_DICTIONARY                     "Dicts"                //  13              Header/position
#define LANG_STR_START_LEARN_FLOW               "Start"              //  14              Position
#define LANG_STR_SHUFFLE_DICTIONARY             "Shuffle dict"           //  14              Position
#define LANG_STR_RESET_CONFIG                   "Reset"             //  14              Position
#define LANG_STR_APPLICATION_TITLE              LANG_STR_APPLICATION_TITLE_POPUP  //  14      Header
// SdCardTools
#define LANG_STR_SD_CARD_ERROR_MESSAGE          "Error SD card"           //  14              Message           Popup
#define LANG_STR_SD_CARD_MISSING                "Insert SD card"           //  14              Message           Popup
#define LANG_STR_SD_CARD_NEED_FORMAT            "Format SD"            //  14              Message           Popup
#define LANG_STR_SD_CARD_ACCESS_DENIED          "SD denied"          //  14              Message           Popup
// Session
#define LANG_STR_TEXT_IN_PROGRESS_SESSION       LANG_STR_APPLICATION_TITLE_POPUP  //  4       Part of header    Warning! Small length. Text in left up corner display when cards are show
#define LANG_STR_SYMBOL_QUESTION                'Q'                       //  1               Character
#define LANG_STR_SYMBOL_ANSWER                  'A'                       //  1               Character
#define LANG_STR_SESSION_INIT_MESSAGE           "Init session"          //  14              Message           Popup
#define LANG_STR_SESSION_RATE                   "   Rate"                 //  7               Part of header    Warning! Don't forget to add blank space. Small length. Text in right up corner display when user should rate answer. Text should be align to right.
// Shuffler
#define LANG_STR_SHUFFLE_DICTIONARY_IN_PROGRESS_MESSAGE "Shuffle dict"  //  14              Message           Popup
#define LANG_STR_SHUFFLE_PROGRESS_IN_PROGRESS_MESSAGE "Proc. progress"  //  14              Message           Popup
#define LANG_STR_SHUFFLE_CONFIRM                "Are you sure?"           //  14              Header            Popup yes/no
// Summary
#define LANG_STR_SUMMARY_IN_PROGRESS_MESSAGE    "Summarize"             //  14              Message           Popup
#define LANG_STR_SUMMARY_END_SESSION_MESSAGE    "End session"            //  14              Message           Popup
#define LANG_STR_SUMMARY_CURRENT_PROGRESS_MESSAGE "Progress: "              //  10              Part of message   Popup. Don't forget about one space at end this string.
// YesNoAcceptFrame
#define LANG_STR_YES                            "Yes"                     //  13              Position
#define LANG_STR_NO                             "No"                     //  13              Position

#endif //LangPL_h
