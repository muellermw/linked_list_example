/**
 * This file contains the test suite for the linked list library.
 * @file testLinkedList.cpp
 * @author Walter Schilling
 * @date September 1, 2017
 *
 */
#include "testLinkedList.h"
#include <malloc.h>
#include <TestAssert.h>
#include <cstring>

CPPUNIT_TEST_SUITE_REGISTRATION(LinkedListTestCase);

#include <CompilerOutputter.h>
#include <extensions/TestFactoryRegistry.h>
#include <TestResult.h>
#include <TestResultCollector.h>
#include <TestRunner.h>
#include <BriefTestProgressListener.h>
#include <iostream>

/**
 * This is the main method for the test program.  It will invoke the cppunit test harness.
 * @param argc This is the count of arguments.  Not used in this program.
 * @param argv This is the arguments passed in.  Not used for the execution of this program.
 * @return The return will be 0 if successful or non zero if an error occurs.
 */
int main (int argc, char* argv[])
{
	// informs test-listener about testresults
	CPPUNIT_NS :: TestResult testresult;

	// register listener for collecting the test-results
	CPPUNIT_NS :: TestResultCollector collectedresults;
	testresult.addListener (&collectedresults);

	// register listener for per-test progress output
	CPPUNIT_NS :: BriefTestProgressListener progress;
	testresult.addListener (&progress);

	// insert test-suite at test-runner by registry
	CPPUNIT_NS :: TestRunner testrunner;
	testrunner.addTest (CPPUNIT_NS :: TestFactoryRegistry :: getRegistry ().makeTest ());
	testrunner.run (testresult);

	// output results in compiler-format
	CPPUNIT_NS :: CompilerOutputter compileroutputter (&collectedresults, std::cerr);
	compileroutputter.write ();

	// return 0 if tests were successful
	return collectedresults.wasSuccessful () ? 0 : 1;
}

/**
 * This method will setup a basic linked list that can be used for further testing.
 * In essence, it will add a set of items to the linked list and verify that the data
 * has been setup properly before allowing the other tests to run.
 */
void LinkedListTestCase::setupBasicList() {
	ll_add(&myList, "CS3841", 7);
	ll_add(&myList, "Operating", 11);
	ll_add(&myList, "Systems", 8);
	ll_add(&myList, "Fall", 5);
	ll_add(&myList, "Quarter", 8);

	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			strcmp((char*)myList.head->data, "CS3841")==0);
	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			strcmp((char*)myList.head->nextNode->data, "Operating")==0);
	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			strcmp((char*)myList.head->nextNode->nextNode->data, "Systems")==0);
	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			strcmp((char*)myList.head->nextNode->nextNode->nextNode->data, "Fall")==0);
	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			strcmp((char*)myList.head->nextNode->nextNode->nextNode->nextNode->data, "Quarter")==0);
}

/**
 * This method will be called to setup the tests.  It will perform an initialization
 * on the given linked list.
 */
void LinkedListTestCase::setUp() {
	printf("Setup being called.");
	// Initialize the linked list to be an empty list.
	ll_init(&myList);
}

/**
 * This method will clear the given linked list, freeing any and all allocated memory
 * and allowing the system to operate.
 */
void LinkedListTestCase::tearDown() {
	printf("Teardown being called.");
	// Clear the linked list, making certain everything is empty.
	ll_clear(&myList);
}

/**
 * This method will verify that the init method is functioning properly.
 */
void LinkedListTestCase::testInit() {
	ll_init(&myList);
	CPPUNIT_ASSERT_MESSAGE("Size not zero.", ll_size(&myList)==0);
	CPPUNIT_ASSERT_MESSAGE("Head not null.", myList.head==NULL);
	CPPUNIT_ASSERT_MESSAGE("Tail not null.", myList.tail==NULL);
}

/**
 * This method will ensure that trying to add to a NULL list is handled properly.
 */
void LinkedListTestCase::testAddNullList() {
	// Check that null is handled properly.
	CPPUNIT_ASSERT_MESSAGE("Improper handling of null list",
			ll_add(NULL, "Hello", 6)==false);
}

/**
 * This method will verify that trying to add NULL items to the list results in an
 * appropriate failure.
 */
void LinkedListTestCase::testAddNullObject() {
	// Check that null is handled properly.
	CPPUNIT_ASSERT_MESSAGE("Improper handling of NULL object.",
			ll_add(&myList, NULL, 0)==false);
}

/**
 * This method ensures that trying to add an item of length zero fails appropriately.
 */
void LinkedListTestCase::testAddZeroSize() {
	CPPUNIT_ASSERT_MESSAGE("Improper handling of 0 size.",
			ll_add(&myList, "Hello", 0)==false);
}

/**
 * This method will test adding a single item onto the list.  It will verify that
 * the item is added appropriately from a location standpoint and that the size
 * is correct.
 */
void LinkedListTestCase::testSingleElementAdd() {
	// Add a single valid element.
	CPPUNIT_ASSERT_MESSAGE("Inappropriate return value from simple add.",
			ll_add(&myList, "Hello", 6)==true);
	CPPUNIT_ASSERT_MESSAGE("Head and tail of list match.",
			myList.head==myList.tail);
	CPPUNIT_ASSERT_MESSAGE("Size is incorrect.", myList.size==1);
}

struct testStructure
{
	char name[10];
	int number;
	char name2[10];
	int number2;
	char name3[10];
};


/**
 * This method will test adding a single item onto the list.  It will verify that
 * the item is added appropriately from a location standpoint and that the size
 * is correct.
 */
void LinkedListTestCase::testSingleElementAddOfStructure() {
	struct testStructure testInstance;
	strcpy(&testInstance.name[0],  "ABCDEFGHI");
	strcpy(&testInstance.name2[0], "JKLMNOPQR");
	strcpy(&testInstance.name3[0], "012345678");
	testInstance.number=0;
	testInstance.number2=0;


	// Add a single valid element.
	CPPUNIT_ASSERT_MESSAGE("Inappropriate return value from simple add.",
			ll_add(&myList, &testInstance, sizeof(struct testStructure))==true);
	CPPUNIT_ASSERT_MESSAGE("Head and tail of list match.",
			myList.head==myList.tail);
	CPPUNIT_ASSERT_MESSAGE("Size is incorrect.", myList.size==1);
	CPPUNIT_ASSERT_MESSAGE("Structures do not properly match.", memcmp(&testInstance, (struct testStructure*)ll_get(&myList, 0), sizeof(struct testStructure))==0);
}



/**
 * This test will ensure that the add method creates a separate memory block appropriately
 * and does not simply add a pointer to the passed in data, potentially causing memory
 * issues in the future.
 */
void LinkedListTestCase::testAddCreatesSeparateMemoryBlock() {
	char const *messages[] = { "Hello", "World" };
	char *dataToAdd = new char[6];

	strcpy(dataToAdd, (char*) &messages[0][0]);

	// Add the first message.
	ll_add(&myList, dataToAdd, 6);

	// Add a single valid element.
	CPPUNIT_ASSERT_MESSAGE("Memory not properly allocated for block",
			myList.head->data!=dataToAdd);
	CPPUNIT_ASSERT_MESSAGE("Memory not properly allocated for block",
			myList.head->data!=&dataToAdd);
	CPPUNIT_ASSERT_MESSAGE("Memory not properly allocated for block",
			myList.head->data!=&messages[0][0]);

	delete [] dataToAdd;
	dataToAdd = new char[6];

	// Add the second message.
	strcpy(dataToAdd, (char*) &messages[1][0]);

	ll_add(&myList, dataToAdd, 6);
	delete [] dataToAdd;

	CPPUNIT_ASSERT_MESSAGE("Memory not properly allocated for block",
			myList.tail->data!=dataToAdd);
	CPPUNIT_ASSERT_MESSAGE("Memory not properly allocated for block",
			myList.tail->data!=&dataToAdd);
	CPPUNIT_ASSERT_MESSAGE("Memory not properly allocated for block",
			myList.tail->data!=&messages[0][0]);
}

/**
 * This method will test adding two items onto the list.  It will verify that
 * the item is added appropriately from a location standpoint and that the size
 * is correct.
 */
void LinkedListTestCase::testTwoElementAdd() {
	// Add a single valid element.
	CPPUNIT_ASSERT_MESSAGE("Inappropriate return value from simple add.",
			ll_add(&myList, "Hello", 6)==true);

	// Add a second valid element.
	CPPUNIT_ASSERT_MESSAGE("Inappropriate return value from simple add.",
			ll_add(&myList, "World", 6)==true);
	CPPUNIT_ASSERT_MESSAGE("Head and tail of list match.",
			myList.head!=myList.tail);
	CPPUNIT_ASSERT_MESSAGE("Size is incorrect.", myList.size==2);
	CPPUNIT_ASSERT_MESSAGE(
			"List data does not match properly for head element.",
			strcmp((const char *)myList.head->data,"Hello")==0);
	CPPUNIT_ASSERT_MESSAGE(
			"List data does not match properly for tail element.",
			strcmp((const char *)myList.tail->data, "World")==0);
	CPPUNIT_ASSERT_MESSAGE("List does not walk properly in forward direction.",
			strcmp((const char *)myList.head->nextNode->data, "World")==0);
	CPPUNIT_ASSERT_MESSAGE(
			"List does not walk properly in a backward direction.",
			strcmp((const char *)myList.tail->prevNode->data, "Hello")==0);
}

/**
 * This method will test adding three items onto the list.  It will verify that
 * the item is added appropriately from a location standpoint and that the size
 * is correct.
 */

void LinkedListTestCase::testThreeElementAdd() {
	// Add a single valid element.
	CPPUNIT_ASSERT_MESSAGE("Inappropriate return value from simple add.",
			ll_add(&myList, "Hello", 6)==true);

	// Add a second valid element.
	CPPUNIT_ASSERT_MESSAGE("Inappropriate return value from simple add.",
			ll_add(&myList, "World", 6)==true);

	// Add a third valid element
	CPPUNIT_ASSERT_MESSAGE("Inappropriate return value from simple add.",
			ll_add(&myList, "This is London.", 16)==true);
	CPPUNIT_ASSERT_MESSAGE("Head and tail of list match.",
			myList.head!=myList.tail);
	CPPUNIT_ASSERT_MESSAGE("Size is incorrect.", myList.size==3);
	CPPUNIT_ASSERT_MESSAGE(
			"List data does not match properly for head element.",
			strcmp((const char *)myList.head->data,"Hello")==0);
	CPPUNIT_ASSERT_MESSAGE(
			"List data does not match properly for tail element.",
			strcmp((const char *)myList.tail->data, "This is London.")==0);
	CPPUNIT_ASSERT_MESSAGE("List does not walk properly in forward direction.",
			strcmp((const char *)myList.head->nextNode->data, "World")==0);
	CPPUNIT_ASSERT_MESSAGE("List does not walk properly in forward direction.",
			strcmp((const char *)myList.head->nextNode->nextNode->data, "This is London.")==0);
	CPPUNIT_ASSERT_MESSAGE(
			"List does not walk properly in a backward direction.",
			strcmp((const char *)myList.tail->prevNode->data, "World")==0);
	CPPUNIT_ASSERT_MESSAGE(
			"List does not walk properly in a backward direction.",
			strcmp((const char *)myList.tail->prevNode->prevNode->data, "Hello")==0);
}

/**
 * This method will that trying to remove from a null list fails.
 */

void LinkedListTestCase::testRemoveNullListFails() {
	setupBasicList();

	CPPUNIT_ASSERT_MESSAGE("Checking that a NULL list is ignored.",
			ll_remove(NULL, 5)==false);
}

/**
 * This method will test adding a single item onto the list.  It will verify that
 * the item is added appropriately from a location standpoint and that the size
 * is correct.
 */

void LinkedListTestCase::testRemoveOutOfRangeElementFails() {
	setupBasicList();
	CPPUNIT_ASSERT_MESSAGE("Checking that an out of range element is ignored.",
			ll_remove(&myList, 5)==false);
}

/**
 * This method will verify that we can properly remove the tail of the given list.
 */
void LinkedListTestCase::testRemoveTail() {
	setupBasicList();

	// Try removing the tail as a first attempt.
	CPPUNIT_ASSERT_MESSAGE("Tail removal failed.", ll_remove(&myList, 4)==true);
	CPPUNIT_ASSERT_MESSAGE("Tail message incorrect after removal.",
			strcmp("Fall", (const char*)myList.tail->data)==0);
	CPPUNIT_ASSERT_MESSAGE("Tail has a next element.",
			myList.tail->nextNode==NULL);
	CPPUNIT_ASSERT_MESSAGE("Incorrect size after removal.", myList.size==4);

	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			strcmp((char*)myList.head->data, "CS3841")==0);
	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			strcmp((char*)myList.head->nextNode->data, "Operating")==0);
	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			strcmp((char*)myList.head->nextNode->nextNode->data, "Systems")==0);
	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			strcmp((char*)myList.head->nextNode->nextNode->nextNode->data, "Fall")==0);
	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			myList.head->nextNode->nextNode->nextNode->nextNode==NULL);
}

/**
 * This test verifies that the remove head function operates properly.
 */
void LinkedListTestCase::testRemoveHead() {
	setupBasicList();

	// Now try removing the head.
	CPPUNIT_ASSERT_MESSAGE("Head removal failed.", ll_remove(&myList, 0)==true);
	CPPUNIT_ASSERT_MESSAGE("Head message incorrect after removal.",
			strcmp("Operating", (const char*)myList.head->data)==0);
	CPPUNIT_ASSERT_MESSAGE("Head has previous element.",
			myList.head->prevNode==NULL);
	CPPUNIT_ASSERT_MESSAGE("Incorrect size after removal.", myList.size==4);

	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			strcmp((char*)myList.head->data, "Operating")==0);
	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			strcmp((char*)myList.head->nextNode->data, "Systems")==0);
	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			strcmp((char*)myList.head->nextNode->nextNode->data, "Fall")==0);
	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			strcmp((char*)myList.head->nextNode->nextNode->nextNode->data, "Quarter")==0);
	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			myList.head->nextNode->nextNode->nextNode->nextNode==NULL);
}

/**
 * This method will verify that removing from the middle of the list functions properly.
 */
void LinkedListTestCase::testRemoveMiddle() {
	setupBasicList();

	// Now try removing the head.
	CPPUNIT_ASSERT_MESSAGE("Middle removal failed.",
			ll_remove(&myList, 2)==true);
	CPPUNIT_ASSERT_MESSAGE("Incorrect size after removal.", myList.size==4);

	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			strcmp((char*)myList.head->data, "CS3841")==0);
	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			strcmp((char*)myList.head->nextNode->data, "Operating")==0);
	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			strcmp((char*)myList.head->nextNode->nextNode->data, "Fall")==0);
	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			strcmp((char*)myList.head->nextNode->nextNode->nextNode->data, "Quarter")==0);
	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			myList.head->nextNode->nextNode->nextNode->nextNode==NULL);
}

/**
 * This method will test the remove operation extensivly.
 */
void LinkedListTestCase::testRemove() {
	setupBasicList();

	// Try removing the tail as a first attempt.
	CPPUNIT_ASSERT_MESSAGE("Tail removal failed.", ll_remove(&myList, 4)==true);
	CPPUNIT_ASSERT_MESSAGE("Tail message incorrect after removal.",
			strcmp("Fall", (const char*)myList.tail->data)==0);
	CPPUNIT_ASSERT_MESSAGE("Tail has a next element.",
			myList.tail->nextNode==NULL);
	CPPUNIT_ASSERT_MESSAGE("Incorrect size after removal.", myList.size==4);

	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			strcmp((char*)myList.head->data, "CS3841")==0);
	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			strcmp((char*)myList.head->nextNode->data, "Operating")==0);
	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			strcmp((char*)myList.head->nextNode->nextNode->data, "Systems")==0);
	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			strcmp((char*)myList.head->nextNode->nextNode->nextNode->data, "Fall")==0);
	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			myList.head->nextNode->nextNode->nextNode->nextNode==NULL);

	// Now remove the head.
	CPPUNIT_ASSERT_MESSAGE("Head removal failed.", ll_remove(&myList, 0)==true);
	CPPUNIT_ASSERT_MESSAGE("Tail message incorrect after removal.",
			strcmp("Fall", (const char*)myList.tail->data)==0);
	CPPUNIT_ASSERT_MESSAGE("Tail has a next element.",
			myList.tail->nextNode==NULL);
	CPPUNIT_ASSERT_MESSAGE("Head has a previous element.",
			myList.head->prevNode==NULL);
	CPPUNIT_ASSERT_MESSAGE("Incorrect size after removal.", myList.size==3);

	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			strcmp((char*)myList.head->data, "Operating")==0);
	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			strcmp((char*)myList.head->nextNode->data, "Systems")==0);
	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			strcmp((char*)myList.head->nextNode->nextNode->data, "Fall")==0);
	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			myList.head->nextNode->nextNode->nextNode==NULL);

	// Now try removing from the middle
	CPPUNIT_ASSERT_MESSAGE("Middle removal failed.",
			ll_remove(&myList, 1)==true);
	CPPUNIT_ASSERT_MESSAGE("Tail message incorrect after removal.",
			strcmp("Fall", (const char*)myList.tail->data)==0);
	CPPUNIT_ASSERT_MESSAGE("Tail has a next element.",
			myList.tail->nextNode==NULL);
	CPPUNIT_ASSERT_MESSAGE("Head has a previous element.",
			myList.head->prevNode==NULL);
	CPPUNIT_ASSERT_MESSAGE("Incorrect size after removal.", myList.size==2);

	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			strcmp((char*)myList.head->data, "Operating")==0);
	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			strcmp((char*)myList.head->nextNode->data, "Fall")==0);
	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			myList.head->nextNode->nextNode==NULL);

	// Remove from the end of the list again.
	CPPUNIT_ASSERT_MESSAGE("Tail removal failed.", ll_remove(&myList, 1)==true);
	CPPUNIT_ASSERT_MESSAGE("Tail message incorrect after removal.",
			strcmp("Operating", (const char*)myList.tail->data)==0);
	CPPUNIT_ASSERT_MESSAGE("Tail has a next element.",
			myList.tail->nextNode==NULL);
	CPPUNIT_ASSERT_MESSAGE("Head has a previous element.",
			myList.head->prevNode==NULL);
	CPPUNIT_ASSERT_MESSAGE("Incorrect size after removal.", myList.size==1);

	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			strcmp((char*)myList.head->data, "Operating")==0);
	CPPUNIT_ASSERT_MESSAGE("List Improperly Structured",
			myList.head->nextNode==NULL);
	CPPUNIT_ASSERT_MESSAGE("Head and tail should match at this point.",
			myList.head==myList.tail);

	// Now try removing the head.
	CPPUNIT_ASSERT_MESSAGE("Head removal failed.", ll_remove(&myList, 0)==true);
	CPPUNIT_ASSERT_MESSAGE("Head and tail should be NULL.", myList.head==NULL);
	CPPUNIT_ASSERT_MESSAGE("Head and tail should be NULL.", myList.tail==NULL);

	// We we should have an empty list.  Check all of the status.
	CPPUNIT_ASSERT_MESSAGE("Head not set properly.", myList.head==NULL);
	CPPUNIT_ASSERT_MESSAGE("Tail not set properly.", myList.tail==NULL);
	CPPUNIT_ASSERT_MESSAGE("Size not set properly.", myList.size==0);
}

/**
 * This method ensures that the addIndex method is functioning properly.
 */
void LinkedListTestCase::testAddIndex() {
	// Start by setting up a basic list.
	ll_add(&myList, "Hello", 6);
	ll_add(&myList, "This is London.", 16);

	// Check to make sure the list is in the right shape.  This should never fail unless one of the test above is also failing.
	CPPUNIT_ASSERT_MESSAGE("Size is incorrect.", myList.size==2);

	// Check to make ceratin all of the invalid parameters fail properly.
	CPPUNIT_ASSERT_MESSAGE("Invalid list parameter",
			ll_addIndex(NULL, "World", 6, 1)==false);
	CPPUNIT_ASSERT_MESSAGE("Invalid object to add.",
			ll_addIndex(&myList, NULL, 6, 1)==false);
	CPPUNIT_ASSERT_MESSAGE("Invalid size of object to add.",
			ll_addIndex(&myList, "World", 0, 1)==false);
	CPPUNIT_ASSERT_MESSAGE("Invalid index for add.",
			ll_addIndex(&myList, "World", 6, 3)==false);

	// Check to make certain adding to the tail effectivly works.
	CPPUNIT_ASSERT_MESSAGE("Improper tail element with addIndex function.",
			ll_addIndex(&myList, "Here is the news.", 18, 2)==true);
	CPPUNIT_ASSERT_MESSAGE("Size improper after add.", myList.size==3);
	CPPUNIT_ASSERT_MESSAGE("Tail set up properly.",
			strcmp("Here is the news.", (const char*)myList.tail->data)==0);
	CPPUNIT_ASSERT_MESSAGE("Tail not given null as next element.",
			myList.tail->nextNode==NULL);
	CPPUNIT_ASSERT_MESSAGE("Previous not set up properly from tail add.",
			strcmp("This is London.", (const char*)myList.tail->prevNode->data)==0);

	// Now try adding to the head of the list.
	CPPUNIT_ASSERT_MESSAGE("Improper behavior of addindex function.",
			ll_addIndex(&myList, "CS3841", 7, 0)==true);
	CPPUNIT_ASSERT_MESSAGE("Size improper after add.", myList.size==4);
	CPPUNIT_ASSERT_MESSAGE("Head set up properly.",
			strcmp("CS3841", (const char*)myList.head->data)==0);
	CPPUNIT_ASSERT_MESSAGE("Head not given null as previous element.",
			myList.head->prevNode==NULL);
	CPPUNIT_ASSERT_MESSAGE("Next not set up properly from tail add.",
			strcmp("Hello", (const char*)myList.head->nextNode->data)==0);

	// Now try adding in the middle of the list.
	CPPUNIT_ASSERT_MESSAGE("Improper tail element with addIndex function.",
			ll_addIndex(&myList, "World", 6, 2)==true);
	CPPUNIT_ASSERT_MESSAGE("Size improper after add.", myList.size==5);

	// Now walk ther list and check everything.
	// List list should be the following.

	CPPUNIT_ASSERT_MESSAGE("Head set up properly.",
			strcmp("CS3841", (const char*)(myList.head->data))==0);
	CPPUNIT_ASSERT_MESSAGE("Head not given null as previous element.",
			myList.head->prevNode==NULL);
	CPPUNIT_ASSERT_MESSAGE("Next not set up properly from tail add.",
			strcmp("Hello", (const char*)myList.head->nextNode->data)==0);

	struct listNode* n = myList.head;
	CPPUNIT_ASSERT_MESSAGE("Head is incorrect.",
			strcmp("CS3841", (const char*)n->data)==0);
	CPPUNIT_ASSERT_MESSAGE("Improper next link.", n->nextNode!=NULL);
	CPPUNIT_ASSERT_MESSAGE("Improper previous link.", n->prevNode==NULL);

	// Walk to the next node.
	n = n->nextNode;
	CPPUNIT_ASSERT_MESSAGE("Node data is incorrect.",
			strcmp("Hello", (const char*)n->data)==0);
	CPPUNIT_ASSERT_MESSAGE("Improper next link.", n->nextNode!=NULL);
	CPPUNIT_ASSERT_MESSAGE("Improper previous link.", n->prevNode!=NULL);

	// Walk to the next node.
	n = n->nextNode;
	CPPUNIT_ASSERT_MESSAGE("Node data is incorrect.",
			strcmp("World", (const char*)n->data)==0);
	CPPUNIT_ASSERT_MESSAGE("Improper next link.", n->nextNode!=NULL);
	CPPUNIT_ASSERT_MESSAGE("Improper previous link.", n->prevNode!=NULL);

	// Walk to the next node.
	n = n->nextNode;
	CPPUNIT_ASSERT_MESSAGE("Node data is incorrect.",
			strcmp("This is London.", (const char*)n->data)==0);
	CPPUNIT_ASSERT_MESSAGE("Improper next link.", n->nextNode!=NULL);
	CPPUNIT_ASSERT_MESSAGE("Improper previous link.", n->prevNode!=NULL);

	// Walk to the next node.
	n = n->nextNode;
	CPPUNIT_ASSERT_MESSAGE("Node data is incorrect.",
			strcmp("Here is the news.", (const char*)n->data)==0);
	CPPUNIT_ASSERT_MESSAGE("Improper next link.", n->nextNode==NULL);
	CPPUNIT_ASSERT_MESSAGE("Improper previous link.", n->prevNode!=NULL);
}

/**
 * This method ensures that the get operation is working properly.
 */
void LinkedListTestCase::testGet() {

	const char* messages[] = { "CS3841", "Operating", "Systems", "Fall",
			"Quarter" };
	int index;

	setupBasicList();
	char *data;

	// Verify basic parameter checks operate properly.
	CPPUNIT_ASSERT_MESSAGE("NULL parameter improperly allowed.",
			ll_get(NULL, 1)==NULL);
	CPPUNIT_ASSERT_MESSAGE("OUT of range parameter not ignored.",
			ll_get(&myList, 1000)==false);

	for (index = 0; index < 5; index++) {
		data = (char*) ll_get(&myList, index);
		CPPUNIT_ASSERT_MESSAGE("Improper message on get",
				strcmp(messages[index], data)==0);
	}
}

/**
 * This method will ensure that the clear operation clears properly.
 */
void LinkedListTestCase::testClear() {
	setupBasicList();
	CPPUNIT_ASSERT_MESSAGE("Starting size incorrect.", ll_size(&myList)==5);
	ll_clear(&myList);

	CPPUNIT_ASSERT_MESSAGE("Head not set properly.", myList.head==NULL);
	CPPUNIT_ASSERT_MESSAGE("Tail not set properly.", myList.tail==NULL);
	CPPUNIT_ASSERT_MESSAGE("Size not set properly.", myList.size==0);
}

/**
 * This method will verify that the size operator is working properly.
 */
void LinkedListTestCase::testSize() {
	CPPUNIT_ASSERT_MESSAGE("Empty list does not have 0 size.",
			ll_size(&myList)==0);
	CPPUNIT_ASSERT_MESSAGE("Size does not match structure.",
			ll_size(&myList)==myList.size);

	// Add something and make certain the size goes up.
	ll_add(&myList, "Hello", 6);

	CPPUNIT_ASSERT_MESSAGE("1 element list does not have 1 size.",
			ll_size(&myList)==1);
	CPPUNIT_ASSERT_MESSAGE("Size does not match structure.",
			ll_size(&myList)==myList.size);
}

/**
 * This method ensures that the iterator being null does not impact the system.
 */
void LinkedListTestCase::testIteratorNull() {
	setupBasicList();

	// Start by testing the obtaining of the iterator.
	struct linkedListIterator *iter;

	iter = ll_getIterator(NULL);
	CPPUNIT_ASSERT_MESSAGE("Iterator should be NULL.", iter==NULL);
}

/**
 * This method ensures that doing a free on the iterator does not damage the list.
 */
void LinkedListTestCase::testIteratorFreeDoesNotDamageList() {
	const char* messages[] = { "CS3841", "Operating", "Systems", "Fall",
			"Quarter" };

	setupBasicList();
	char* data;

	// Start by testing the obtaining of the iterator.
	struct linkedListIterator *iter;

	// Now test obtaining a valid iterator.
	iter = ll_getIterator(&myList);

	data = (char*) ll_next(iter);

	free(iter);

	CPPUNIT_ASSERT_MESSAGE("Data damaged when free operation occurred.",
			strcmp(messages[0], data)==0);
}

/**
 * This method ensures that the iterator walks through the code properly.
 */
void LinkedListTestCase::testIterator() {
	const char* messages[] = { "CS3841", "Operating", "Systems", "Fall",
			"Quarter" };
	int index;

	setupBasicList();
	char* data;

	// Start by testing the obtaining of the iterator.
	struct linkedListIterator *iter;

	// Now test obtaining a valid iterator.
	iter = ll_getIterator(&myList);
	CPPUNIT_ASSERT_MESSAGE("Iterator should not be NULL.", iter!=NULL);
	CPPUNIT_ASSERT_MESSAGE("Iterator's current should be the head of the list.",
			iter->current==myList.head);

	for (index = 0; index < 5; index++) {
		CPPUNIT_ASSERT_MESSAGE(
				"An invalid iterator should result in a false value.",
				ll_hasNext(NULL)==false);
		CPPUNIT_ASSERT_MESSAGE("Iterator should have a next right now.",
				ll_hasNext(iter)==true);

		CPPUNIT_ASSERT_MESSAGE(
				"Invalid parameter to next should result in NULL being returned,",
				ll_next(NULL)==NULL);
		data = (char*) ll_next(iter);
		CPPUNIT_ASSERT_MESSAGE("Expected true for correct next operation.",
				data!=NULL);
		CPPUNIT_ASSERT_MESSAGE("Data returned does not match expected data.",
				strcmp(messages[index], data)==0);
	}

	// Now we have stepped through everything.  There should be nothing left for the next.
	CPPUNIT_ASSERT_MESSAGE("Extra next operation.", ll_hasNext(iter)==false);
	CPPUNIT_ASSERT_MESSAGE(
			"Invalid return value when attempting to retrieve beyond the end of the list.",
			ll_next(iter)==NULL);

	free(iter);

}
