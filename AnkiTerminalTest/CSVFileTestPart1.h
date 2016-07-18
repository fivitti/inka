#ifndef CSVFileTestPart_1_h
#define CSVFileTestPart_1_h

#include "CSVFileTestInit.h"

test(gotoBeginOfFile_ok) {
  beginTest();
  assertTrue(file.gotoBeginOfFile());
  assertEqual(file.curPosition(), 0);
  file.seekSet(10);
  assertTrue(file.gotoBeginOfFile());
  assertEqual(file.curPosition(), 0);
  file.seekSet(8);
  assertTrue(file.gotoBeginOfFile());
  assertEqual(file.curPosition(), 0);
  endTest();
}

test(substring_ok) {
  beginTest();
  bool status_ = file.isCurrentSubstring("Ala");
  assertTrue(status_);
  file.gotoBeginOfFile();
  status_ = file.isCurrentSubstring("A");
  assertTrue(status_);
  file.gotoBeginOfFile();
  status_ = file.isCurrentSubstring("Al");
  assertTrue(status_);
  file.gotoBeginOfFile();
  status_ = file.isCurrentSubstring("");
  assertTrue(status_);
  endTest();
}

test(substring_failed) {
  beginTest();
  file.gotoBeginOfFile();
  bool status_ = file.isCurrentSubstring("ala");
  assertFalse(status_);
  file.gotoBeginOfFile();
  status_ = file.isCurrentSubstring("ba");
  assertFalse(status_);
  file.gotoBeginOfFile();
  status_ = file.isCurrentSubstring("123");
  assertFalse(status_);
  file.gotoBeginOfFile();
  status_ = file.isCurrentSubstring("!@");
  assertFalse(status_);
  endTest();
}

test(isEndOfFile_ok) {
  beginTest();
  file.seekEnd();
  assertTrue(file.isEndOfFile());
  endTest();
}

test(isEndOfFile_failed) {
  beginTest();
  assertFalse(file.isEndOfFile());
  endTest();
}

test(isBeginOfLine_ok) {
  beginTest();
  assertTrue(file.isBeginOfLine());
  endTest();
}

test(isBeginOfLine_ok_2) {
  beginTest();
  file.nextLine();
  assertTrue(file.isBeginOfLine());
  endTest();
}

test(isBeginOfLine_failed) {
  beginTest();
  file.seekSet(1);
  assertFalse(file.isBeginOfLine());
  endTest();
}

test(isEndOfLine_ok) {
  beginTest();
  file.nextLine();
  file.seekCur(-1);
  assertTrue(file.isEndOfLine());
  endTest();
}

test(isEndOfLine_ok_2) {
  beginTest();
  file.seekEnd();
  assertTrue(file.isEndOfLine());
  endTest();
}

test(isEndOfLine_failed) {
  beginTest();
  file.seekCur(1);
  assertFalse(file.isEndOfLine());
  endTest();
}

test(isLineMarkedAsRemove_ok) {
  beginTest();
  assertFalse(file.isLineMarkedAsRemove());
  endTest();
}

test(isLineMarkedAsRemove_ok2) {
  beginTest();
  file.nextLine();
  file.nextLine();
  file.seekCur(length3Line);
  assertTrue(file.isLineMarkedAsRemove());
  endTest();
}

test(isLineMarkedAsRemove_failed) {
  beginTest();
  file.nextLine();
  file.nextLine();
  file.seekCur(length3Line);
  assertTrue(file.isLineMarkedAsRemove());
  // When we repeat check at the same line we get false,
  // because this function don't back pointer in file
  assertFalse(file.isLineMarkedAsRemove());
  endTest();
}

test(gotoBeginOfLine_ok) {
  beginTest();
  assertTrue(file.gotoBeginOfLine());
  assertTrue(file.isBeginOfLine());
  endTest();
}

test(gotoBeginOfLine_ok2) {
  beginTest();
  file.nextLine();
  file.seekCur(2);
  assertTrue(file.gotoBeginOfLine());
  assertTrue(file.isBeginOfLine());
  endTest();
}

test(nextLine_ok) {
  beginTest();
  assertTrue(file.nextLine());  //Second line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start2Line);
  assertTrue(file.nextLine());  //Third line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start3Line);
  //Remove line
  assertTrue(file.nextLine());  //Fourth line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start5Line);
  endTest();
}

test(nextLine_failed) {
  beginTest();
  file.seekEnd();
  uint32_t endPosition = file.curPosition();
  assertFalse(file.nextLine());
  assertEqual(file.curPosition(), endPosition);
  file.seekCur(-3);
  assertFalse(file.nextLine());
  assertEqual(file.curPosition(), endPosition);
}

#endif //CSVFileTestPart_1_h





