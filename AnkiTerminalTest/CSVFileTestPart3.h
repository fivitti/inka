#ifndef CSVFileTestPart_3_h
#define CSVFileTestPart_3_h

#include "CSVFileTestInit.h"

test(gotoLine_start_ok) {
  beginTest();
  // Enumeration from 0 to 3
  assertTrue(file.gotoLine("Ala"));  //Second line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), 0);
  assertTrue(file.gotoLine("2"));  //Second line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start2Line);
  assertTrue(file.gotoLine("Ale"));  //Third line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start3Line);
  //Remove line
  assertTrue(file.gotoLine("historia"));  //Fourth line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start5Line);

  // Enumeration from 3 to 0
  file.seekEnd();
  assertTrue(file.gotoLine("his"));  //Fourth line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start5Line);
  assertTrue(file.gotoLine("Ale;"));  //Third line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start3Line);
  assertTrue(file.gotoLine("2;kot"));  //Second line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start2Line);
  assertTrue(file.gotoLine("Al"));  //Second line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), 0);

  // Random access
  assertTrue(file.gotoLine("Ale"));  //Third line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start3Line);
  assertTrue(file.gotoLine("Ala;"));  //Second line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), 0);
  assertTrue(file.gotoLine("hi"));  //Fourth line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start5Line);
  assertTrue(file.gotoLine("2;"));  //Second line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start2Line);
  
  endTest();
}

test(gotoLine_start_failed) {
  beginTest();
  assertFalse(file.gotoLine("ala"));
  assertTrue(file.isEndOfFile());
  assertFalse(file.gotoLine("xxx"));
  assertTrue(file.isEndOfFile());
  endTest();
}

test(markLineAsRemove_begin_file_ok) {
  beginTest();
  file.gotoBeginOfFile();
  assertTrue(file.markLineAsRemove());
  file.gotoBeginOfFile();
  assertTrue(file.isLineMarkedAsRemove());
  endTest();
}

test(markLineAsRemove_remark_ok) {
  beginTest();
  // Mark first
  file.gotoBeginOfFile();
  assertTrue(file.markLineAsRemove());
  file.gotoBeginOfFile();
  assertTrue(file.isLineMarkedAsRemove());
  // Mark second
  file.gotoBeginOfFile();
  assertTrue(file.markLineAsRemove());
  file.gotoBeginOfFile();
  assertTrue(file.isLineMarkedAsRemove());
  endTest();
}

test(markLineAsRemove_ok) {
  beginTest();
  file.nextLine();
  assertTrue(file.markLineAsRemove());
  file.seekSet(start2Line);
  assertTrue(file.isLineMarkedAsRemove());
  endTest();
}

test(addLine_ok) {
  beginTest();
  file.seekEnd();
  assertTrue(file.addLine());
  assertTrue(file.isEndOfFile());
  file.seekCur(-1);
  assertTrue(file.read() == '\n');
  endTest();
}

test(addLine_failed) {
  #if ! CSV_CHECK_ADDING_ERROR
    skip();
    return;
  #endif
  beginTest();
  file.gotoBeginOfFile();
  assertFalse(file.addLine());
  endTest();
}
#endif //CSVFileTestPart_3_h





