#ifndef TESTLINKEDLIST_H
#define TESTLINKEDLIST_H

      
#include <iostream>
#include <string>

// Note 1
#include <TestFixture.h>
#include <extensions/HelperMacros.h>

extern "C" {
  #include "linkedlist.h"   
}



using namespace std;

class LinkedListTestCase : public CppUnit::TestFixture { // Note 2 
  CPPUNIT_TEST_SUITE(LinkedListTestCase);
  CPPUNIT_TEST(testInit);

  CPPUNIT_TEST(testAddNullList);
  CPPUNIT_TEST(testAddNullObject);
  CPPUNIT_TEST(testAddZeroSize);
  CPPUNIT_TEST(testSingleElementAdd);
  CPPUNIT_TEST(testSingleElementAddOfStructure);
  CPPUNIT_TEST(testTwoElementAdd);
  CPPUNIT_TEST(testThreeElementAdd);
  CPPUNIT_TEST(testSize);
  CPPUNIT_TEST(testAddIndex);
  CPPUNIT_TEST(testAddCreatesSeparateMemoryBlock);
  CPPUNIT_TEST(testClear);
  CPPUNIT_TEST(testGet);
  CPPUNIT_TEST(testRemoveNullListFails);
  CPPUNIT_TEST(testRemoveOutOfRangeElementFails);
  CPPUNIT_TEST(testRemoveTail);
  CPPUNIT_TEST(testRemoveMiddle);
  CPPUNIT_TEST(testRemoveHead);
  CPPUNIT_TEST(testRemove);
  CPPUNIT_TEST(testIterator);
  CPPUNIT_TEST(testIteratorNull);
  CPPUNIT_TEST(testIteratorFreeDoesNotDamageList);
  CPPUNIT_TEST_SUITE_END();

 private:
  linkedList myList;
  void setupBasicList();

public:
    void setUp();
    void tearDown();

 protected:
  void testInit();
  void testAddIndex();
  void testAddNullList();
  void testAddNullObject();
  void testAddZeroSize();
  void testSingleElementAdd();
  void testSingleElementAddOfStructure();
  void testAddCreatesSeparateMemoryBlock();
  void testTwoElementAdd();
  void testThreeElementAdd();
  void testRemoveNullListFails();
  void testRemoveOutOfRangeElementFails();
  void testRemoveTail();
  void testRemoveHead();
  void testRemoveMiddle();
  void testRemove();
  void testGet();
  void testClear();
  void testSize();
  void testIterator();
  void testIteratorNull();
  void testIteratorFreeDoesNotDamageList();
};
#endif
          
