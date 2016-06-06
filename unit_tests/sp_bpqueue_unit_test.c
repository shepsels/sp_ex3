/*
 * sp_bpqueue_unit_test.c
 *
 *  Created on: 29 áîàé 2016
 *      Author: Noa
 */


#include "unit_test_util.h"
#include "../SPBPriorityQueue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>


/**
 * creates queue with maximum capacity of 3
 * checks if it's created
 * validates max size and list size
 */
bool queueCreateTest() {
	int maxSize = 3;
	SPBPQueue q = spBPQueueCreate(maxSize);
	ASSERT_TRUE(q != NULL);
	spBPQueueDestroy(q);
	return true;
}

/**
 * checks if destroy can run on null
 */
static bool queueDestroyTest() {
	spBPQueueDestroy(NULL);
	return true;
}

/**
 * creates queue with maximun capacity of 2
 * validates that queue is empty
 * inserts 3,3.0 and makes sure it was successfully added
 * inserts 2,2.0 and makes sure it was successfully added
 * tries to insert bigger element and queue is full
 *
 */
static bool insertQueueTest() {
	ASSERT_TRUE(NULL == spBPQueueCreate(-1));
	//create queue and elements
	SPBPQueue q1 = spBPQueueCreate(2);
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);

	ASSERT_TRUE(spBPQueueSize(q1) == 0);
	// add one element and check the size
	SP_BPQUEUE_MSG msg1 = spBPQueueEnqueue(q1, e3);
	ASSERT_TRUE(msg1 == SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueSize(q1) == 1);

	// add another element and check size
	spBPQueueEnqueue(q1, e2);
	ASSERT_TRUE(spBPQueueSize(q1) == 2);

	// dequeue and check the elements are on the correct order
	ASSERT_TRUE(spBPQueueMinValue(q1) == 2.0);
	spBPQueueDequeue(q1);
	ASSERT_TRUE(spBPQueueSize(q1) == 1);

	// dequeue and check the elements are on the correct order
	ASSERT_TRUE(spBPQueueMinValue(q1) == 3.0);
	spBPQueueDequeue(q1);
	ASSERT_TRUE(spBPQueueSize(q1) == 0);

	// add same elements as before
	spBPQueueEnqueue(q1, e3);
	spBPQueueEnqueue(q1, e2);

	// add smaller element
	msg1 = spBPQueueEnqueue(q1, e1);
	ASSERT_TRUE(msg1 == SP_BPQUEUE_SUCCESS);

	// add bigger element
	msg1 = spBPQueueEnqueue(q1, e4);
	ASSERT_TRUE(msg1 == SP_BPQUEUE_FULL);

	// dequeue and check the elements are on the correct order
	ASSERT_TRUE(spBPQueueMinValue(q1) == 1.0);
	spBPQueueDequeue(q1);
	ASSERT_TRUE(spBPQueueSize(q1) == 1);

	ASSERT_TRUE(spBPQueueMinValue(q1) == 2.0);
	spBPQueueDequeue(q1);
	ASSERT_TRUE(spBPQueueSize(q1) == 0);

	//free memory
	spBPQueueDestroy(q1);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

/**
 * creates queue with maximun capacity of 2
 * makes a copy of the empty queue
 * inserts to the original queue
 * validates that the copy didn't change
 * makes another copy of non empty queue
 * validates it has same elements
 */
static bool queueCopyTest() {
	SPBPQueue q, copy, copy2;
	SPListElement e1, e2, e3, e4;

	ASSERT_TRUE(spBPQueueCopy(NULL) == NULL);
	// copy empty queue and check it's empty
	q = spBPQueueCreate(2);
	copy = spBPQueueCopy(q);
	ASSERT_TRUE(copy != NULL);
	ASSERT_TRUE(0 == spBPQueueSize(copy));

	// add elements and check they are not in the copy queue
	e1 = spListElementCreate(1, 1.0);
	e2 = spListElementCreate(2, 2.0);
	e3 = spListElementCreate(3, 3.0);
	e4 = spListElementCreate(4, 4.0);
	spBPQueueEnqueue(q, e3);
	ASSERT_TRUE(0 == spBPQueueSize(copy));

	// enqueue and copy again, to check that elements were copied
	spBPQueueEnqueue(q, e2);
	copy2 = spBPQueueCopy(q);
	ASSERT_TRUE(2 == spBPQueueSize(copy2));

	// dequeue and check the elements are on the correct order
	ASSERT_TRUE(spBPQueueMinValue(q) == 2.0);
	spBPQueueDequeue(q);
	ASSERT_TRUE(spBPQueueSize(q) == 1);
	ASSERT_TRUE(spBPQueueMinValue(q) == 3.0);
	spBPQueueDequeue(q);
	ASSERT_TRUE(spBPQueueSize(q) == 0);
	ASSERT_TRUE(spBPQueueDequeue(q) == SP_BPQUEUE_EMPTY);

	//free memory
	spBPQueueDestroy(q);
	spBPQueueDestroy(copy);
	spBPQueueDestroy(copy2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

/**
 * creates a queue with maximun capacity of 3
 * inserts 4 elements and make sure it has 3
 * clears the queue
 * makes sure its not null, and it has no elements
 * creates another empty queue
 * tries to clear and make sure there are no changes
 */
static bool queueClearTest() {
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPBPQueue q = spBPQueueCreate(3), q2;

	spBPQueueEnqueue(q, e3);
	spBPQueueEnqueue(q, e4);
	spBPQueueEnqueue(q, e1);
	spBPQueueEnqueue(q, e2);
	ASSERT_TRUE(spBPQueueSize(q) == 3);

	// clear and make sure the queue is empty but not null
	spBPQueueClear(q);
	ASSERT_TRUE(q != NULL);
	ASSERT_TRUE(0 == spBPQueueSize(q));
	q2 = spBPQueueCreate(3);
	spBPQueueClear(q2);
	ASSERT_TRUE(q2 != NULL);
	ASSERT_TRUE(0 == spBPQueueSize(q2));

	//free memory
	spBPQueueDestroy(q);
	spBPQueueDestroy(q2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

/**
 * creates queue with maximum capacity of 3
 * inserts element 3, 3.0 and validate size
 * inserts element 2, 2.0 and validate size
 * inserts another element 2, 2.0
 * makes sure the elements in queue are 2.0, 2.0, 3.0
 * inserts element 1, 1.0
 * inserts 2 elements: 5, 0.5, 6, 0.5
 * makes sure they inserted in the correct order
 */
static bool insertQueueWithDuplicatesTest() {
//	SPListElement temp1, temp2, temp3;
	SPBPQueue q = spBPQueueCreate(3);
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e2_2 = spListElementCreate(2, 2.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPListElement e5 = spListElementCreate(5, 0.5);
	SPListElement e6 = spListElementCreate(6, 0.5);

	ASSERT_TRUE(spBPQueueSize(q) == 0);
	SP_BPQUEUE_MSG msg1 = spBPQueueEnqueue(q, e3);
	ASSERT_TRUE(msg1 == SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueSize(q) == 1);
	spBPQueueEnqueue(q, e2);
	ASSERT_TRUE(spBPQueueSize(q) == 2);
	spBPQueueEnqueue(q, e2_2);
	ASSERT_TRUE(spBPQueueSize(q) == 3);

	// dequeue and check the elements are on the correct order
	ASSERT_TRUE(spBPQueueMinValue(q) == 2.0);
	spBPQueueDequeue(q);
	ASSERT_TRUE(spBPQueueSize(q) == 2);
	ASSERT_TRUE(spBPQueueMinValue(q) == 2.0);
	spBPQueueDequeue(q);
	ASSERT_TRUE(spBPQueueSize(q) == 1);
	ASSERT_TRUE(spBPQueueMinValue(q) == 3.0);
	spBPQueueDequeue(q);
	ASSERT_TRUE(spBPQueueSize(q) == 0);
	ASSERT_TRUE(spBPQueueDequeue(q) == SP_BPQUEUE_EMPTY);

	// enqueue again to continue with the test
	spBPQueueEnqueue(q, e3);
	spBPQueueEnqueue(q, e2);
	spBPQueueEnqueue(q, e2_2);
	ASSERT_TRUE(spBPQueueSize(q) == 3);

	// enqueue smaller element, and then bigger element
	msg1 = spBPQueueEnqueue(q, e1);
	ASSERT_TRUE(msg1 == SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueSize(q) == 3);
	msg1 = spBPQueueEnqueue(q, e4);
	ASSERT_TRUE(msg1 == SP_BPQUEUE_FULL);

	// dequeue and check the elements are on the correct order
	ASSERT_TRUE(spBPQueueMinValue(q) == 1.0);
	spBPQueueDequeue(q);
	ASSERT_TRUE(spBPQueueSize(q) == 2);
	ASSERT_TRUE(spBPQueueMinValue(q) == 2.0);
	spBPQueueDequeue(q);
	ASSERT_TRUE(spBPQueueSize(q) == 1);
	ASSERT_TRUE(spBPQueueMinValue(q) == 2.0);
	spBPQueueDequeue(q);
	ASSERT_TRUE(spBPQueueSize(q) == 0);
	ASSERT_TRUE(spBPQueueDequeue(q) == SP_BPQUEUE_EMPTY);

	// enqueue again to continue with the test
	spBPQueueEnqueue(q, e1);
	spBPQueueEnqueue(q, e2);
	spBPQueueEnqueue(q, e2_2);
	ASSERT_TRUE(spBPQueueSize(q) == 3);

	// enqueue elements with same value and different indexes. peek in order to determine
	// if the values and indexes are correct
	msg1 = spBPQueueEnqueue(q, e6);
	SPListElement temp1 = spBPQueuePeek(q);
	ASSERT_TRUE(spListElementGetValue(temp1) == 0.5 && spListElementGetIndex(temp1) == 6);

	msg1 = spBPQueueEnqueue(q, e5);
	SPListElement temp2 = spBPQueuePeek(q);
	ASSERT_TRUE(spListElementGetValue(temp2) == 0.5 && spListElementGetIndex(temp2) == 5);

	spBPQueueDequeue(q);
	ASSERT_TRUE(spBPQueueSize(q) == 2);
	SPListElement temp3 = spBPQueuePeek(q);
	ASSERT_TRUE(spListElementGetValue(temp3) == 0.5 && spListElementGetIndex(temp3) == 6);

	// free memory
	spBPQueueDestroy(q);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e2_2);
	spListElementDestroy(e4);
	spListElementDestroy(e5);
	spListElementDestroy(e6);
	spListElementDestroy(temp1);
	spListElementDestroy(temp2);
	spListElementDestroy(temp3);
	return true;
}

/**
 * creates queue with maximum capacity of 3
 * inserts 3 elements with different values
 * dequeues them one at a time
 * in each dequeue checks that the correct element is out of the queue and that the size is correct
 * checks that the message the dequeue function returns is correct
 */
static bool dequeueTest(){
	SP_BPQUEUE_MSG msg;
	SPBPQueue q ;
	SPListElement e1,e2,e3,e4;

	q = spBPQueueCreate(3);
	e1 = spListElementCreate(2, 2.0);
	e2 = spListElementCreate(3, 3.0);
	e3 = spListElementCreate(5, 0.5);

	spBPQueueEnqueue(q, e1);  //value 2.0
	spBPQueueEnqueue(q, e2);  //value 3.0
	spBPQueueEnqueue(q, e3);  //value 0.5

	// q = {e2,e1,e2}

	e4=spBPQueuePeek(q);
	ASSERT_TRUE(spListElementGetValue(e4) == 0.5);
	ASSERT_TRUE(spBPQueueSize(q)==3);
	msg=spBPQueueDequeue(q);
	ASSERT_TRUE(msg==SP_BPQUEUE_SUCCESS); //message test
	spListElementDestroy(e4);

	e4=spBPQueuePeek(q);
	ASSERT_TRUE(spListElementGetValue(e4) == 2.0);
	spListElementDestroy(e4);
	e4=spBPQueuePeekLast(q);
	ASSERT_TRUE(spListElementGetValue(e4) == 3.0);
	ASSERT_TRUE(spBPQueueSize(q)==2);
	spBPQueueDequeue(q);
	spListElementDestroy(e4);

	e4=spBPQueuePeek(q);
	ASSERT_TRUE(spListElementGetValue(e4) == 3.0);
	ASSERT_TRUE(spBPQueueSize(q)==1);
	spListElementDestroy(e4);

	spBPQueueDequeue(q);
	e4=spBPQueuePeek(q);
	ASSERT_TRUE(e4 == NULL);
	ASSERT_TRUE(spBPQueueSize(q)==0);

	// memory free
	spBPQueueDestroy(q);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

/**
 * creates queue with maximum capacity of 3
 * inserts 3 elements with different values
 * checks that the spBPQueuePeek function creates a copy of the first element of the queue
 * (the one with the smallest value)and that the size of the queue didn't change
 * dequeues them until the list is empty and checks that spBPQueuePeek returns NULL
 */
static bool spBPQueuePeekTest(){
	SPBPQueue q ;
	SPListElement e1,e2,e3,e4;

	q = spBPQueueCreate(3);
	e1 =spListElementCreate(2, 2.0);
	e2 =spListElementCreate(3, 3.0);
	e3 =spListElementCreate(5, 0.5);

	spBPQueueEnqueue(q, e1);  //value 2.0
	spBPQueueEnqueue(q, e2);  //value 3.0
	spBPQueueEnqueue(q, e3);  //value 0.5
	// q = {e3,e1,e2}

	e4=spBPQueuePeek(q);  //copy of e3
	ASSERT_TRUE(spListElementGetValue(e4)==0.5);
	ASSERT_TRUE(spListElementGetIndex(e4)==5);
	ASSERT_TRUE(spBPQueueSize(q)==3);

	ASSERT_TRUE(spListElementCompare(e3, e4) == 0);
	spListElementSetValue(e4,8.0);  //changing the copy's value in order to check that it is a copy
	ASSERT_TRUE(spListElementGetValue(e3)==0.5);
	ASSERT_TRUE(spListElementGetValue(e4)==8.0);
	ASSERT_TRUE(spListElementCompare(e3, e4) != 0);
	spListElementDestroy(e4);
	spBPQueueDequeue(q);
	// q = {e1,e2}

	e4=spBPQueuePeek(q);  //copy of e1
	ASSERT_TRUE(spListElementGetValue(e4)==2.0);
	ASSERT_TRUE(spListElementGetIndex(e4)==2);
	ASSERT_TRUE(spBPQueueSize(q)==2);
	ASSERT_TRUE(spListElementCompare(e1, e4) == 0);
	spListElementDestroy(e4);
	spBPQueueDequeue(q);
	// q = {e2}

	e4=spBPQueuePeek(q);  //copy of e2
	ASSERT_TRUE(spListElementGetValue(e4)==3.0);
	ASSERT_TRUE(spListElementGetIndex(e4)==3);
	ASSERT_TRUE(spBPQueueSize(q)==1);
	ASSERT_TRUE(spListElementCompare(e2, e4) == 0);
	spListElementDestroy(e4);
	spBPQueueDequeue(q);
	// q = {}

	e4=spBPQueuePeek(q);  //NULL
	ASSERT_TRUE(e4==NULL);


	// memory free
	spBPQueueDestroy(q);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);

	return true;
}

/**
 * creates queue with maximum capacity of 3
 * inserts 3 elements with different values
 * checks that the spBPQueuePeekLast function creates a copy of the last element of the queue
 * (the one with the highest value)and that the size of the queue didn't change
 * dequeues them until the list is empty and checks that spBPQueuePeekLast returns NULL
 */
static bool spBPQueuePeekLastTest(){
	SPBPQueue q ;
	SPListElement e1,e2,e3,e4,e5;

	q = spBPQueueCreate(3);
	e1 =spListElementCreate(1, 1.0);
	e2 =spListElementCreate(2, 2.0);
	e3 =spListElementCreate(3, 3.0);

	spBPQueueEnqueue(q, e1);  //value 1.0
	spBPQueueEnqueue(q, e2);  //value 2.0
	spBPQueueEnqueue(q, e3);  //value 3.0
	// q = {e1,e2,e3}

	e4 = spBPQueuePeekLast(q);  //copy of e3
	ASSERT_TRUE(spListElementGetValue(e4)==3.0);
	ASSERT_TRUE(spListElementGetIndex(e4)==3);
	ASSERT_TRUE(spBPQueueSize(q)==3);

	ASSERT_TRUE(spListElementCompare(e3, e4) == 0);
	spListElementSetValue(e4,8.0);  //changing the copy value in order to check that it is a copy
	ASSERT_TRUE(spListElementGetValue(e4)==8.0);
	ASSERT_TRUE(spListElementGetIndex(e3)==3.0);
	ASSERT_TRUE(spListElementCompare(e1, e4) != 0);
	spListElementDestroy(e4);
	spBPQueueDequeue(q);
	// q = {e2,e3}

	//checking that the order of the queue is correct
	e4=spBPQueuePeek(q);  //copy of e2
	ASSERT_TRUE(spListElementGetValue(e4)==2.0);
	ASSERT_TRUE(spListElementGetIndex(e4)==2);
	ASSERT_TRUE(spBPQueueSize(q)==2);
	spListElementDestroy(e4);

	e4 = spBPQueuePeekLast(q);  //copy of e3
	ASSERT_TRUE(spListElementGetValue(e4)==3.0);
	ASSERT_TRUE(spListElementGetIndex(e4)==3);
	ASSERT_TRUE(spBPQueueSize(q)==2);
	spListElementDestroy(e4);
	spBPQueueDequeue(q);
	// q = {e3}

	e5=spBPQueuePeek(q);  //copy of e3
	e4 = spBPQueuePeekLast(q);  //copy of e3
	ASSERT_TRUE(spListElementCompare(e5, e4) == 0);
	ASSERT_TRUE(spBPQueueSize(q)==1);
	spListElementDestroy(e5);
	spBPQueueDequeue(q);
	//q = {}

	e5=spBPQueuePeekLast(q); //NULL
	ASSERT_TRUE(e5==NULL);

	// memory free
	spBPQueueDestroy(q);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spListElementDestroy(e5);
	return true;
}

/**
 * creates queue with maximum capacity of 3
 * inserts 3 elements with different values
 * checks that the spBPQueueMinValue function returns the value of the first element of the queue
 * (the lowest one) , changes the first element and re-check
 */
static bool spBPQueueMinValueTest(){
	SPBPQueue q;
	SPListElement e1,e2,e3,e4,e5;

	q = spBPQueueCreate(3);
	e1 = spListElementCreate(1, 1.0);
	e2 = spListElementCreate(2, 2.0);
	e3 = spListElementCreate(3, 3.0);
	e4 = spListElementCreate(4, 0.5);

	spBPQueueEnqueue(q, e2);  //value 2.0
	spBPQueueEnqueue(q, e1);  //value 1.0
	spBPQueueEnqueue(q, e3);  //value 3.0
	// q = {e1,e2,e3}

	ASSERT_TRUE(spBPQueueMinValue(q)==1.0);
	e5=spBPQueuePeek(q);
	ASSERT_TRUE(spListElementGetValue(e5) == 1.0);
	spBPQueueDequeue(q);
	spBPQueueEnqueue(q, e4);  //value 0.5
	spListElementDestroy(e5);
	// q = {e4,e2,e3}

	ASSERT_TRUE(spBPQueueMinValue(q)==0.5);
	e5=spBPQueuePeek(q);
	ASSERT_TRUE(spListElementGetValue(e5) == 0.5);

	// memory free
	spBPQueueDestroy(q);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spListElementDestroy(e5);

	return true;
}

/**
 * creates queue with maximum capacity of 3
 * inserts 3 elements with different values
 * checks that the spBPQueueMaxValue function returns the value of the last element of the queue
 * (the highest one) , inserts element with higher value and re-check
 */
static bool spBPQueueMaxValueTest(){
	SPBPQueue q;
	SPListElement e1,e2,e3,e4;

	q = spBPQueueCreate(3);
	e1 = spListElementCreate(1, 1.0);
	e2 = spListElementCreate(2, 2.0);
	e3 = spListElementCreate(3, 3.0);
	e4 = spListElementCreate(4, 4.0);

	spBPQueueEnqueue(q, e2);  //value 2.0
	spBPQueueEnqueue(q, e1);  //value 1.0
	spBPQueueEnqueue(q, e3);  //value 3.0
	// q = {e1,e2,e3}

	ASSERT_TRUE(spBPQueueMaxValue(q)==3.0);
	spBPQueueDequeue(q);

	ASSERT_TRUE(spBPQueueMaxValue(q)==3.0);
	spBPQueueEnqueue(q, e4);  //value 4.0
	// q = {e2,e3,e4}

	ASSERT_TRUE(spBPQueueMaxValue(q)==4.0);

	// memory free
	spBPQueueDestroy(q);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);

	return true;
}

/**
 * checks that spBPQueueIsEmpty function returns false for a NULL argument
 * creates queue with maximum capacity of 3
 * checks that spBPQueueIsEmpty returns true (the queue is empty when initializing it)
 * inserts element and checks that spBPQueueIsEmpty returns false
 * inserts 2 more elements and re-check
 * clears the queue and re-check
 */
static bool spBPQueueIsEmptyTest(){
	SPBPQueue q;
	SPListElement e1,e2,e3;

	ASSERT_TRUE(spBPQueueIsEmpty(NULL)==false);

	q = spBPQueueCreate(3);
	e1 = spListElementCreate(1, 1.0);
	e2 = spListElementCreate(2, 2.0);
	e3 = spListElementCreate(3, 3.0);

	ASSERT_TRUE(spBPQueueIsEmpty(q)==true);
	spBPQueueEnqueue(q, e2);  //value 2.0
	ASSERT_TRUE(spBPQueueIsEmpty(q)==false);
	spBPQueueEnqueue(q, e1);  //value 1.0
	spBPQueueEnqueue(q, e3);  //value 3.0
	ASSERT_TRUE(spBPQueueIsEmpty(q)==false);
	spBPQueueClear(q);
	ASSERT_TRUE(spBPQueueIsEmpty(q)==true);

	// memory free
	spBPQueueDestroy(q);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);

	return true;
}

/**
 * checks that spBPQueueIsFull function returns false for a NULL argument
 * creates queue with maximum capacity of 3
 * checks that spBPQueueIsFull returns false (the queue is empty when initializing it)
 * inserts element and checks that spBPQueueIsFull returns false (1 element ,capacity of 3)
 * inserts 2 more elements and check if it returns true (3 elements ,capacity of 3)
 * dequeues and inserts the same element and re-check
 * clears the queue and re-check
 */
static bool spBPQueueIsFullTest(){
	SPBPQueue q;
	SPListElement e1,e2,e3;

	ASSERT_TRUE(spBPQueueIsFull(NULL)==false);

	q = spBPQueueCreate(3);
	e1 = spListElementCreate(1, 1.0);
	e2 = spListElementCreate(2, 2.0);
	e3 = spListElementCreate(3, 3.0);

	ASSERT_TRUE(spBPQueueIsFull(q)==false);
	spBPQueueEnqueue(q, e2);  //value 2.0
	ASSERT_TRUE(spBPQueueIsFull(q)==false);
	spBPQueueEnqueue(q, e1);  //value 1.0
	spBPQueueEnqueue(q, e3);  //value 3.0
	ASSERT_TRUE(spBPQueueIsFull(q)==true);
	spBPQueueDequeue(q);
	ASSERT_TRUE(spBPQueueIsFull(q)==false);
	spBPQueueEnqueue(q, e1);  //value 1.0
	ASSERT_TRUE(spBPQueueIsFull(q)==true);
	spBPQueueClear(q);
	ASSERT_TRUE(spBPQueueIsFull(q)==false);

	// memory free
	spBPQueueDestroy(q);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);

	return true;
}


int main() {
	RUN_TEST(queueCreateTest);
	RUN_TEST(queueDestroyTest);
	RUN_TEST(insertQueueTest);
	RUN_TEST(queueCopyTest);
	RUN_TEST(queueClearTest);
	RUN_TEST(insertQueueWithDuplicatesTest);
	RUN_TEST(dequeueTest);
	RUN_TEST(spBPQueuePeekTest);
	RUN_TEST(spBPQueuePeekLastTest);
	RUN_TEST(spBPQueueMinValueTest);
	RUN_TEST(spBPQueueMaxValueTest);
	RUN_TEST(spBPQueueIsEmptyTest);
	RUN_TEST(spBPQueueIsFullTest);

	return 0;
}
