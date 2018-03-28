# Inka

Inka is the application for learn languages based on "flashcard" method for Arduino.

## Main features

* Learn flow optimized for Arduino
* Support multi big dictionaries in CSV format
* Support diacritics
* Support localization
* Single or both side card learn
* Dictionary shuffling
* Full customization learn flow
* GUI inspired old phones - clear and simply
* Software protect against sudden shutdown

## Dependencies

Inka uses external libraries to support SD card reader.

* [SdFat](https://github.com/greiman/SdFat)
* [CSVFile](https://github.com/fivitti/Arduino-CSV-File)

## Electric components

![Circuit](/Circuits/circuit.png)

* Core: Atmega328p-PU 3.3V with internal 8MHz oscillator 
* Screen: Nokia 5110 LCD
* Storage: MicroSD card reader on SPI bus
* Controls: 4 tactical switches connected to single analog pin using voltage divider
* Power: Nokia battery connected by TP4056 loader module

## Flow algorithm

On start the application draws set of cards based on results previous sessions.

The application displays the flashcards one-by-one. First it is show the question side card. Next user should answer to yourself and press any button. Then application display the answer. Now user should rate how well know the answer - good, medium or bad.

The words which user didn't know will be displayed more often in that and next session.

### Input parameters

__Max cards per session__ - define how many cards user will have to learn in single session
__Draw mode__ - define which side of card should be draw as question side
__Repeat number__ - how many single card will be display in single session (if user select only "medium" rate)  
__Decrease repeats when good__ - how many decrease the repeat count for specific card when user select "good" rate 
__Increase repeats when bad__ - how many increase the repeat count for specific card when user select "bad" rate  
__Easy decrease probability__ - how many decrease probability of draw specific card when user select often "good" rate for specific card  
__Medium decrease probability__ - how many decrease probability of draw specific card when user select often "medium" rate for specific card  
__Hard increase probability__ - how many increase probability of draw specific card when user select often "bad" rate for specific card  
__Forgotten increase probability__ - how many increase probability of draw specific card when it isn't draw to current session. It simulate forgotting of the card  
__How many "bad" rate for recognize card as hard__ - define how many times user must select "bad" rate for specific card for recognize it as "hard" (accepting that others answers was "medium")  
__Initial probability__ - initial probability for new dictionaries. This probability is set when dictionary is loaded first time  

## Dictionaries

Dictionaries are files with verbs to learn

### Format

Dictionary is CSV files with semicolon as delimiter (by default). It has a four columns.

1. Phrase in first language. Variable length. Any length, but you should remeber that Nokia LCD can display only 70 chars in the same time
2. Phrase in second language
3. Extras in second language (not used in this version, can be empty or ignored)
4. Pronunciation in second language (not used in this version, can be empty or ignored)

The file must be save with UNIX end line style (only '\n'). Line cannot be ended by semicolon (it must be '\n').
End line cannot be ended by new line char.
The filename must be in format 8.3 (at most 8 characters in name and 3 characters in extension). 

### Support diacritics

The Arduino doesn't support UTF-8. It has only ASCII support. But Inka has implemented workaround for it.

Each non-ASCII character is read by Arduino as pair of two negative number (range from -128 to -1).

You can provide this pair in **Font.h** file in __EXTEND_ASCII_LOCKUP__. Next you should provide the graphic representation in __ASCII__ in new row.

Current out of the box Inka supports German and Polish diacritics. CSV file must be save in Windows 1250 encoding.
If you want to use other encoding then you must manually edit the __EXTEND_ASCII_LOOKUP__.

## Localization application

You can easly localize application by create __LangXX.h__ file by copy and translate any existing lang file.
Next you should set valid value __LANG__ define in __Lang.h__.
Out of the box are available english and polish translations.
