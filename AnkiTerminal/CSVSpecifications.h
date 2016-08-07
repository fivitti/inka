#ifndef CSVSpecifications_h
#define CSVSpecifications_h

/*
 * Here are defined constans described all CSV formats.
 * We have four type files:
 *  1) Progres files - file stored progress to learn of dictionary.
 *                     Exist one file for each used dictionary, auto generate after select dictionary.
 *                     It is use for generate session file. It is update after each session.
 *  2) Configuration file - file stored values user configuration.
 *                     Exist only one piece, it is create on start application (if no exist).
 *                     It is update by @ConfigurationFieldFrame's.
 *  3) Session file - temporary file stored state of current session.
 *                     Exist only one piece, it is create on start of session and delete on end of session.
 *  4) Dictionary file - file contain content of cards. It file must be create by user and put on the SD card in dictionary directory.
 *  
 *  Specfication notes:
 *    1) Each field except the last is ended by delimiter ';'.
 *    2) Last field is ended by end of line character in the UNIX style '\n'
 *    3) Last field of the last line don't have end of line character
 *    4) Exist numberic fields with fixed size. These fields can be edit.
 *    5) In configuration file last line of file is dedicated for dictionary name. It is non-fixed field, but can be edit.
 *    6) Record: |000;s*;0*;0?| will mean: line contain first fixed numberic field with size 3, second field is text field with any size,
 *       third field is non-fixed numberic field with any size and fourth field is fixed numberic field with not-important size.
 *    7) Each fixed field must have given size. If size of content of the field is less ten field size other places
 *       should be set to null character '\0'
 */

/*
 *  VERY IMPORTANT DEFINE. DON'T FORGET UPDATE THIS
 */
#define MAXIMUM_NUMBERIC_FIXED_CONFIGURATION_FIELD_SIZE 3

/* Progress file
 * Specification:
 *    Creation by:    Application
 *    Filename:       As associated dictionary (but this file is stored in application dir)
 *    Number of line: As much as dictionary
 *    Content:        Each line in format: |00;00| 
 *                    where first field is probability draw as question first language verb from dictionary
 *                    and second field is probability draw as question second language verb from dictionary
 *                    For better performance range of the probability is [0-99].           
 *                    Each line describe knowledge verbs from dictionary file.
 */
#define CSV_FIELD_PROGRESS_PROBABILITY_SIZE 2 // <0; 99>
#define CSV_FIELD_PROGRESS_PROBABILITY_FIRST_LANG_POSITION 0
#define CSV_FIELD_PROGRESS_PROBABILITY_SECOND_LANG_POSITION 1
#define CSV_FIELD_PROGRESS_PROBABILITY_MINIMUM 0
#define CSV_FIELD_PROGRESS_PROBABILITY_MAXIMUM 99

/* Config file
 * Specification:
 *    Creation by:    Application
 *    Filename:       Value of @CONFIGURATION_FILENAME define
 *    Number of line: 6
 *    Content:        All line except last contain only numberic fixed size fields.
 *                    Each line describes another aspect of the system
 *    Content graph:  | Maximum card per session   ; Which language draw as question ; Repeat number              |
 *                    | Behaviour when good rate   ; Behaviour when  bad rate        |
 *                    | Decrease chance easy cards ; Decrease chance medium cards    ; Increase chance hard cards ; Increase chance forget cards ; When hard |
 *                    | Begin chance - probability |
 *                    | LCD contrast               ; LCD brightness                  |
 *                    | Current dictionary name    |
 *
 *    Size graph:     |00;00;00|
 *                    |00;00;00|
 *                    |00;00;00;00;00|
 *                    |00|
 *                    |000;000|
 *                    |s*|
 *
 */
#define CSV_LINE_CONFIG_LEARN_DRAW 0
#define CSV_LINE_CONFIG_LEARN_DRAW_SIZE 3 //Number of fields
#define CSV_FIELD_CONFIG_LEARN_DRAW_MAXIMUM_CARD_PER_SESSION 0
#define CSV_FIELD_CONFIG_LEARN_DRAW_MAXIMUM_CARD_PER_SESSION_SIZE 3 //<0-254>
#define CSV_FIELD_CONFIG_LEARN_DRAW_MODE 1 // 1 - first language; 2 - second language; 3 - both
#define CSV_FIELD_CONFIG_LEARN_DRAW_MODE_SIZE 1
#define CSV_FIELD_CONFIG_LEARN_DRAW_REPEAT_NUMBER 2
#define CSV_FIELD_CONFIG_LEARN_DRAW_REPEAT_NUMBER_SIZE 2 //<0-99>
#define CSV_FIELD_MAXIMUM_REPEAT 99

#define CSV_LINE_CONFIG_LEARN_SESSION 1
#define CSV_LINE_CONFIG_LEARN_SESSION_SIZE 2
#define CSV_FIELD_CONFIG_LEARN_SESSION_NUMBER_DECREASE_REPEAT_WHEN_GOOD 0
#define CSV_FIELD_CONFIG_LEARN_SESSION_NUMBER_DECREASE_REPEAT_WHEN_GOOD_SIZE CSV_FIELD_CONFIG_LEARN_DRAW_REPEAT_NUMBER_SIZE
#define CSV_FIELD_CONFIG_LEARN_SESSION_NUMBER_INCREASE_REPEAT_WHEN_BAD 1
#define CSV_FIELD_CONFIG_LEARN_SESSION_NUMBER_INCREASE_REPEAT_WHEN_BAD_SIZE CSV_FIELD_CONFIG_LEARN_DRAW_REPEAT_NUMBER_SIZE

#define CSV_LINE_CONFIG_LEARN_SUMMARY 2
#define CSV_LINE_CONFIG_LEARN_SUMMARY_SIZE 5
#define CSV_FIELD_CONFIG_LEARN_SUMMARY_EASY_DECREASE_PROBABILITY 0
#define CSV_FIELD_CONFIG_LEARN_SUMMARY_EASY_DECREASE_PROBABILITY_SIZE CSV_FIELD_PROGRESS_PROBABILITY_SIZE
#define CSV_FIELD_CONFIG_LEARN_SUMMARY_MEDIUM_DECREASE_PROBABILITY 1
#define CSV_FIELD_CONFIG_LEARN_SUMMARY_MEDIUM_DECREASE_PROBABILITY_SIZE CSV_FIELD_PROGRESS_PROBABILITY_SIZE
#define CSV_FIELD_CONFIG_LEARN_SUMMARY_HARD_INCREASE_PROBABILITY 2
#define CSV_FIELD_CONFIG_LEARN_SUMMARY_HARD_INCREASE_PROBABILITY_SIZE CSV_FIELD_PROGRESS_PROBABILITY_SIZE
#define CSV_FIELD_CONFIG_LEARN_SUMMARY_FORGET_INCREASE_PROBABILITY 3
#define CSV_FIELD_CONFIG_LEARN_SUMMARY_FORGET_INCREASE_PROBABILITY_SIZE CSV_FIELD_PROGRESS_PROBABILITY_SIZE
#define CSV_FIELD_CONFIG_LEARN_SUMMARY_HARD_WHEN_NUMBER_BAD_ANSWER_WHEN_OTHERS_NORMAL 4
#define CSV_FIELD_CONFIG_LEARN_SUMMARY_HARD_WHEN_NUMBER_BAD_ANSWER_WHEN_OTHERS_NORMAL_SIZE CSV_FIELD_CONFIG_LEARN_DRAW_REPEAT_NUMBER_SIZE

#define CSV_LINE_CONFIG_LEARN_INIT_PROGRESS 3
#define CSV_LINE_CONFIG_LEARN_INIT_PROGRESS_SIZE 1
#define CSV_FIELD_CONFIG_LEARN_INIT_PROGRESS_BEGIN_PROBABILITY 0
#define CSV_FIELD_CONFIG_LEARN_INIT_PROGRESS_BEGIN_PROBABILITY_SIZE CSV_FIELD_PROGRESS_PROBABILITY_SIZE

#define CSV_LINE_CONFIG_LCD 4
#define CSV_LINE_CONFIG_LCD_SIZE 2
#define CSV_FIELD_CONFIG_LCD_CONTRAST 0
#define CSV_FIELD_CONFIG_LCD_CONTRAST_SIZE 3 //<0-254>
#define CSV_FIELD_CONFIG_LCD_BACKLIGHT 1
#define CSV_FIELD_CONFIG_LCD_BACKLIGHT_SIZE 3 //<0-254>

#define CSV_LINE_CONFIG_DICTIONARY 5 

/* Session file
 * Specification:
 *    Creation by:    Application
 *    Filename:       Value of @SESSION_SET_FILENAME define
 *    Number of line: As much as drawed cards. Maximum is value of @CSV_FIELD_CONFIG_LEARN_DRAW_MAXIMUM_CARD_PER_SESSION field.
 *    Content:        Each line in format: |00;00;s*;s*;0*;0?|
 *                    where first field is repeat remaining to end session
 *                    and second is count repeats in current session
 *                    and third is question verb - copy verb from dictionary
 *                    and fourth is answer verb - copy verb from dictionary
 *                    and fifth is number of line in progress file with this card
 *                    and sixth described which language is used for question verb (0 for first and 1 for second)
 */
#define CSV_FIELD_SESSION_REPEAT_TO_END 0
#define CSV_FIELD_SESSION_REPEAT_TO_END_SIZE CSV_FIELD_CONFIG_LEARN_DRAW_REPEAT_NUMBER_SIZE
#define CSV_FIELD_SESSION_REPEAT_TOTAL 1
#define CSV_FIELD_SESSION_REPEAT_TOTAL_SIZE CSV_FIELD_CONFIG_LEARN_DRAW_REPEAT_NUMBER_SIZE
#define CSV_FIELD_SESSION_QUESTION 2
#define CSV_FIELD_SESSION_ANSWER 3
#define CSV_FIELD_SESSION_NUMBER_LINE 4
#define CSV_FIELD_SESSION_TYPE 5

/* Dictionary file
* Specification:
*    Creation by:    User
*    Filename:       These filename should have maximum size @FILENAME_LIMIT_SIZE = 13 in 8.3 format - 8 chars for name and 3 for extension. 1 for dot.
*    Number of line: As much as drawed cards. Maximum is value of @CSV_FIELD_CONFIG_LEARN_DRAW_MAXIMUM_CARD_PER_SESSION field.
*    Content:        Each line in format: |00;00;s*;s*;0*;0?|
*                    where first field is repeat remaining to end session
*                    and second is count repeats in current session
*                    and third is question verb - copy verb from dictionary
*                    and fourth is answer verb - copy verb from dictionary
*                    and fifth is number of line in progress file with this card
*                    and sixth described which language is used for question verb (0 for first and 1 for second)
*/
#define CSV_FIELD_DICTIONARY_FIRST_LANG 0
#define CSV_FIELD_DICTIONARY_SECOND_LANG 1
#define CSV_FIELD_DICTIONARY_EXTRAS 2
#define CSV_FIELD_DICTIONARY_PRONUNCIATION 3

//Draw constans
//This constans cannot change
#define DRAW_MODE_FIRST_LANG 0
#define DRAW_MODE_SECOND_LANG 1
#define DRAW_MODE_BOTH_LANG 2
#endif
