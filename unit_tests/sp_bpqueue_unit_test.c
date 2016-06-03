/*
 * sp_bpqueue_unit_tests.c
 *
 *  Created on: May 26, 2016
 *      Author: pshepsels
 */
// TODO test in the end of each function name
#include "unit_test_util.h"
#include "../SPListElement.h"
#include "../SPList.h"
#include "../SPBPriorityQueue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

/**
 * create queue with maximum capacity of 3
 * check if its created
 * validate max size and list size
 */
bool queueCreateTest() {
	int maxSize = 3;
	SPBPQueue q = spBPQueueCreate(maxSize);
	ASSERT_TRUE(q!=NULL);
	ASSERT_TRUE(q->maxSize == maxSize);
	ASSERT_TRUE(q->list != NULL);
	ASSERT_TRUE(spListGetSize(q->list) == 0);
	spBPQueueDestroy(q);

	return true;
}

/**
 * check destroy can run on null
 */
static bool testQueueDestroy() {
	spBPQueueDestroy(NULL);
	return true;
}

/**
 * create queue with maximun capacity of 2
 * validate that queue is empty
 * insert 3,3.0 and make sure it was successfully added
 * insert 2,2.0 and make sure it was successfully added
 * try to insert bigger element and queue is full
 *
 */
static bool testInsertQueue() {
	ASSERT_TRUE(NULL == spBPQueueCreate(-1));
	SPBPQueue q1 = spBPQueueCreate(2);
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	ASSERT_TRUE(spBPQueueSize(q1) == 0);
	SP_BPQUEUE_MSG msg1 = spBPQueueEnqueue(q1, e3);
	ASSERT_TRUE(msg1 == SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueSize(q1) == 1);
	spBPQueueEnqueue(q1, e2);
	ASSERT_TRUE(spBPQueueSize(q1) == 2);
	ASSERT_TRUE(spListGetFirst(q1->list)->value == 2.0);
	ASSERT_TRUE(spListGetNext(q1->list)->value == 3.0);
	msg1 = spBPQueueEnqueue(q1, e1);
	ASSERT_TRUE(msg1 == SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueSize(q1) == 2);
	ASSERT_TRUE(spListGetFirst(q1->list)->value == 1.0);
	ASSERT_TRUE(spListGetNext(q1->list)->value == 2.0);
	msg1 = spBPQueueEnqueue(q1, e4);
	ASSERT_TRUE(msg1 == SP_BPQUEUE_FULL);

	//free memory
	spBPQueueDestroy(q1);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

/**
 * create queue with maximun capacity of 2
 * make a copy of the empty queue
 * insert to the original queue
 * validate that the copy didn't change
 * make another copy of non empty queue
 * validate it has same elements
 */
static bool testQueueCopy() {
	ASSERT_TRUE(spBPQueueCopy(NULL) == NULL);
	SPBPQueue q = spBPQueueCreate(2);
	SPBPQueue copy = spBPQueueCopy(q);
	ASSERT_TRUE(copy != NULL);
	ASSERT_TRUE(0 == spBPQueueSize(copy));
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	spBPQueueEnqueue(q, e3);
	ASSERT_TRUE(0 == spBPQueueSize(copy));
	spBPQueueEnqueue(q, e2);
	SPBPQueue copy2 = spBPQueueCopy(q);
	ASSERT_TRUE(2 == spBPQueueSize(copy2));
	ASSERT_TRUE(spListElementCompare(e2, spListGetFirst(copy2->list)) == 0);
	ASSERT_TRUE(spListElementCompare(e3, spListGetNext(copy2->list)) == 0);
	ASSERT_TRUE(spListGetNext(copy2->list) == NULL);

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
 * create a queue with maximun capacity of 3
 * insert 4 elements and make sure it has 3
 * clear the queue
 * make sure its not null, and it has no elements
 * create another empty queue
 * try to clear and make sure there are no changes
 */
static bool testQueueClear() {			//TODO Paz 5
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPBPQueue q = spBPQueueCreate(3);
	spBPQueueEnqueue(q, e3);
	spBPQueueEnqueue(q, e4);
	spBPQueueEnqueue(q, e1);
	spBPQueueEnqueue(q, e2);
	ASSERT_TRUE(spBPQueueSize(q) == 3);
	spBPQueueClear(q);
	ASSERT_TRUE(q != NULL);
	ASSERT_TRUE(0 == spBPQueueSize(q));
	SPBPQueue q2 = spBPQueueCreate(3);
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
 * create queue with maximun capacity of 3
 * insert element 3, 3.0 and validate size
 * insert element 2, 2.0 and validate size
 * insert another element 2, 2.0
 * make sure the elements in queue are 2.0, 2.0, 3.0
 * insert elemet 1, 1.0
 * insert 2 elements: 5, 0.5, 6, 0.5
 * make sure they inserted in the correct order
 */
static bool testInsertQueueWithDups() {
	SPListElement temp;
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
	ASSERT_TRUE(spListGetFirst(q->list)->value == 2.0);
	ASSERT_TRUE(spListGetNext(q->list)->value == 2.0);
	ASSERT_TRUE(spListGetNext(q->list)->value == 3.0);
	msg1 = spBPQueueEnqueue(q, e1);
	ASSERT_TRUE(msg1 == SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueSize(q) == 3);
	ASSERT_TRUE(spListGetFirst(q->list)->value == 1.0);
	ASSERT_TRUE(spListGetNext(q->list)->value == 2.0);
	ASSERT_TRUE(spListGetNext(q->list)->value == 2.0);
	msg1 = spBPQueueEnqueue(q, e4);
	ASSERT_TRUE(msg1 == SP_BPQUEUE_FULL);

	msg1 = spBPQueueEnqueue(q, e5);
	temp = spListGetFirst(q->list);
	ASSERT_TRUE(temp->value == 0.5 && temp->index == 5);
	temp = spListGetNext(q->list);
	ASSERT_TRUE(temp->value == 1.0 && temp->index == 1);
	temp = spListGetNext(q->list);
	ASSERT_TRUE(temp->value == 2.0 && temp->index == 2);

	msg1 = spBPQueueEnqueue(q, e6);
	temp = spListGetFirst(q->list);
	ASSERT_TRUE(temp->value == 0.5 && temp->index == 5);
	temp = spListGetNext(q->list);
	ASSERT_TRUE(temp->value == 0.5 && temp->index == 6);
	temp = spListGetNext(q->list);
	ASSERT_TRUE(temp->value == 1);
	spBPQueueDestroy(q);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e2_2);
	spListElementDestroy(e4);
	spListElementDestroy(e5);
	spListElementDestroy(e6);
	return true;
}


static bool dequeueTest(){    //TODO noa 8
	SP_BPQUEUE_MSG msg;
	SPBPQueue q = (SPBPQueue)spBPQueueCreate(3);
	SPListElement e1 =(SPListElement) spListElementCreate(1, 1.0);
	SPListElement e2 = (SPListElement)spListElementCreate(2, 2.0);
	SPListElement e3 = (SPListElement)spListElementCreate(3, 3.0);
	SPListElement e4 = (SPListElement)spListElementCreate(4, 4.0);
	SPListElement e5 = (SPListElement)spListElementCreate(5, 0.5);
	SPListElement e6 = (SPListElement)spListElementCreate(6, 0.5);

	msg=spBPQueueEnqueue(q, e2);  //value 2.0
	ASSERT_TRUE(msg == SP_BPQUEUE_SUCCESS);
	spBPQueueEnqueue(q, e3);  //value 3.0
	spBPQueueEnqueue(q, e5);  //value 0.5
	// q = {e5,e2,e3}
	ASSERT_TRUE(spListElementGetValue(spListGetFirst(q->list)) == 0.5);
	printf("%d",spListGetSize(q->list)); //TODO
	fflush(NULL);
	ASSERT_TRUE(spListGetSize(q->list)==3);
	spBPQueueDequeue(q);
//	ASSERT_TRUE(spListElementGetValue(spListGetFirst(q->list)) == 2.0);
	ASSERT_TRUE(spListGetFirst(q->list)->value == 2.0);
	ASSERT_TRUE(spListGetNext(q->list)->value == 3.0);	//TODO Paz: I changed these 2 lines, its ok like this
//	ASSERT_TRUE(spListElementGetValue(spListGetNext(spListGetFirst(q->list))) == 3.0);
	ASSERT_TRUE(spListGetSize(q->list)==2);
	spBPQueueDequeue(q);
	ASSERT_TRUE(spListElementGetValue(spListGetFirst(q->list)) == 3.0);
	ASSERT_TRUE(spListGetSize(q->list)==1);
	spBPQueueDequeue(q);
	ASSERT_TRUE(spListGetFirst(q->list) == NULL);
	ASSERT_TRUE(spListGetSize(q->list)==0);

	spBPQueueEnqueue(q, e4);  //value 4.0
	spBPQueueEnqueue(q, e6);  //value 0.5
	spBPQueueEnqueue(q, e1);  //value 1.0
	// q = {e6,e1,e4}
	spBPQueueDequeue(q);
	ASSERT_TRUE(spListElementGetValue(spListGetFirst(q->list)) == 1.0);
//	ASSERT_TRUE(spListElementGetValue(spListGetNext(spListGetFirst(q->list))) == 4.0);
	ASSERT_TRUE(spListGetNext(q->list)->value == 4.0);	// TODO Paz also here I changed the line above
	ASSERT_TRUE(spListGetSize(q->list)==2);
	spBPQueueDequeue(q);
	ASSERT_TRUE(spListElementGetValue(spListGetFirst(q->list)) == 4.0);
	ASSERT_TRUE(spListGetSize(q->list)==1);
	spBPQueueEnqueue(q, e2);  //value 2.0
	// q = {e2,e4}
	ASSERT_TRUE(spListElementGetValue(spListGetFirst(q->list)) == 2.0);
	spBPQueueDequeue(q);
	ASSERT_TRUE(spListElementGetValue(spListGetFirst(q->list)) == 4.0);
	ASSERT_TRUE(spListGetSize(q->list)==1);
	spBPQueueDequeue(q);
	ASSERT_TRUE(spListGetFirst(q->list) == NULL);
	ASSERT_TRUE(spListGetSize(q->list)==0);

	//message tests
//	msg=spBPQueueDequeue(NULL);
//	ASSERT_TRUE(msg == SP_BPQUEUE_INVALID_ARGUMENT);
//	q->list=NULL;
//	msg=spBPQueueDequeue(q);
//	ASSERT_TRUE(msg == SP_BPQUEUE_INVALID_ARGUMENT);


	// memory free
	spBPQueueDestroy(q);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spListElementDestroy(e5);
	spListElementDestroy(e6);
	return true;
}

static bool spBPQueuePeekTest(){ //TODO noa 9
	SPBPQueue q = (SPBPQueue)spBPQueueCreate(3);
//	SPListElement e1 =(SPListElement) spListElementCreate(1, 1.0);
	SPListElement e1 = (SPListElement)spListElementCreate(2, 2.0);
	SPListElement e2 = (SPListElement)spListElementCreate(3, 3.0);
//	SPListElement e4 = (SPListElement)spListElementCreate(4, 4.0);
	SPListElement e3 = (SPListElement)spListElementCreate(5, 0.5);
//	SPListElement e6 = (SPListElement)spListElementCreate(6, 0.5);

	spBPQueueEnqueue(q, e1);  //value 2.0
	spBPQueueEnqueue(q, e2);  //value 3.0
	spBPQueueEnqueue(q, e3);  //value 0.5
	// q = {e3,e1,e2}
	SPListElement e4=(SPListElement)spBPQueuePeek(q);  //copy of e3
	ASSERT_TRUE(spListElementGetValue(e4)==0.5);
	ASSERT_TRUE(spListElementGetIndex(e4)==5);
	ASSERT_TRUE(spListGetSize(q->list)==3);
	ASSERT_TRUE(spListElementGetValue(spListGetFirst(q->list)) == 0.5);
//	ASSERT_TRUE(spListElementGetValue(spListGetNext(spListGetFirst(q->list))) == 2.0);
	ASSERT_TRUE(spListGetNext(q->list)->value == 2.0);
//	ASSERT_TRUE(spListElementGetValue(spListGetNext(spListGetNext(spListGetFirst(q->list)))) == 3.0); TODO paz also here
	ASSERT_TRUE(spListGetNext(q->list)->value == 3.0);
	ASSERT_TRUE(spListElementCompare(e3, e4) == 0);
	spListElementSetValue(e4,8.0);  //changing the copy value in order to check that it is a copy
//	ASSERT_TRUE(spListElementGetValue(spListGetNext(spListGetNext(spListGetFirst(q->list)))) == 3.0);
	spListGetFirst(q->list);
	spListGetNext(q->list);
	ASSERT_TRUE(spListGetNext(q->list)->value == 3.0);
//	ASSERT_TRUE(spListElementGetValue(e3)==3.0);		//TODO Paz: why it should be 3?
	ASSERT_TRUE(spListElementGetValue(e4)==8.0);
	ASSERT_TRUE(spListElementCompare(e3, e4) != 0);
	spBPQueueDequeue(q);
	// q = {e1,e2}
	SPListElement e5=(SPListElement)spBPQueuePeek(q);  //copy of e1
	ASSERT_TRUE(spListElementGetValue(e5)==2.0);
	ASSERT_TRUE(spListElementGetIndex(e5)==2);
	ASSERT_TRUE(spListGetSize(q->list)==2);
	ASSERT_TRUE(spListElementGetValue(spListGetFirst(q->list)) == 2.0);

	// memory free
	spBPQueueDestroy(q);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spListElementDestroy(e5);
	return true;
}

static bool spBPQueuePeekLastTest(){  //TODO noa 10
	SPBPQueue q = (SPBPQueue)spBPQueueCreate(3);
	SPListElement e1 =(SPListElement) spListElementCreate(1, 1.0);
	SPListElement e2 = (SPListElement)spListElementCreate(2, 2.0);
	SPListElement e3 = (SPListElement)spListElementCreate(3, 3.0);

	spBPQueueEnqueue(q, e1);  //value 1.0
	spBPQueueEnqueue(q, e2);  //value 2.0
	spBPQueueEnqueue(q, e3);  //value 3.0
	// q = {e1,e2,e3}
	SPListElement e4 = spBPQueuePeek(q);  //copy of e3
	ASSERT_TRUE(spListElementGetValue(e4)==1.0);		//TODO Paz: changed it from 3 to 1. its the first
	ASSERT_TRUE(spListElementGetIndex(e4)==1);			//TODO also here
	ASSERT_TRUE(spListGetSize(q->list)==3);
	ASSERT_TRUE(spListElementGetValue(spListGetFirst(q->list)) == 1.0);
//	ASSERT_TRUE(spListElementGetValue(spListGetNext(spListGetFirst(q->list))) == 2.0);
	spListGetFirst(q->list);	//TODO Paz: changed the line above to these lines
	ASSERT_TRUE(spListGetNext(q->list)->value == 2.0);


//	ASSERT_TRUE(spListElementGetValue(spListGetNext(spListGetNext(spListGetFirst(q->list)))) == 3.0);
	spListGetFirst(q->list);	//TODO Paz: changed the line above to these lines
	spListGetNext(q->list);
	ASSERT_TRUE(spListGetNext(q->list)->value == 3.0);
//	ASSERT_TRUE(spListElementCompare(e3, e4) == 0);		//TODO Paz should be the opposite
	spListElementSetValue(e4,8.0);  //changing the copy value in order to check that it is a copy
//	ASSERT_TRUE(spListElementGetValue(spListGetNext(spListGetNext(spListGetFirst(q->list)))) == 3.0);
	spListGetFirst(q->list);	//TODO Paz: changed the line above to these lines
	spListGetNext(q->list);
	ASSERT_TRUE(spListGetNext(q->list)->value == 3.0);
	ASSERT_TRUE(spListElementGetValue(e4)==8.0);
	ASSERT_TRUE(spListElementGetIndex(e3)==3.0);

	spBPQueueDequeue(q);
	// q = {e1,e2}
	SPListElement e5=(SPListElement)spBPQueuePeek(q);  //copy of e2
	ASSERT_TRUE(spListElementGetValue(e5)==2.0);
	ASSERT_TRUE(spListElementGetIndex(e5)==2);
	ASSERT_TRUE(spListGetSize(q->list)==2);
//	ASSERT_TRUE(spListElementGetValue(spListGetFirst(q->list)) == 1.0);
//	ASSERT_TRUE(spListGetFirst(q->list)->value == 1.0);//TODO Paz: commented this line. not sure what is this
//	ASSERT_TRUE(spListElementGetValue(spListGetNext(spListGetFirst(q->list))) == 2.0);
	spListGetFirst(q->list);	//TODO Paz: changed the line above to these lines
	ASSERT_TRUE(spListGetNext(q->list)->value == 3.0);

	// memory free
	spBPQueueDestroy(q);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spListElementDestroy(e5);
	return true;
}

static bool spBPQueueMinValueTest(){   //TODO noa 11
	SPBPQueue q = (SPBPQueue)spBPQueueCreate(3);
	SPListElement e1 =(SPListElement) spListElementCreate(1, 1.0);
	SPListElement e2 = (SPListElement)spListElementCreate(2, 2.0);
	SPListElement e3 = (SPListElement)spListElementCreate(3, 3.0);
	SPListElement e4 = (SPListElement)spListElementCreate(4, 0.5);

	spBPQueueEnqueue(q, e2);  //value 2.0
	spBPQueueEnqueue(q, e1);  //value 1.0
	spBPQueueEnqueue(q, e3);  //value 3.0
	// q = {e1,e2,e3}
	ASSERT_TRUE(spBPQueueMinValue(q)==1.0);
	ASSERT_TRUE(spListElementGetValue(spListGetFirst(q->list)) == 1.0);
	spBPQueueDequeue(q);
	spBPQueueEnqueue(q, e4);  //value 0.5
	// q = {e4,e2,e3}
	ASSERT_TRUE(spBPQueueMinValue(q)==0.5);
	ASSERT_TRUE(spListElementGetValue(spListGetFirst(q->list))== 0.5);
	// memory free
	spBPQueueDestroy(q);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

static bool spBPQueueMaxValueTest(){  //TODO noa 12
	SPBPQueue q = (SPBPQueue)spBPQueueCreate(3);
	SPListElement e1 =(SPListElement) spListElementCreate(1, 1.0);
	SPListElement e2 = (SPListElement)spListElementCreate(2, 2.0);
	SPListElement e3 = (SPListElement)spListElementCreate(3, 3.0);
	SPListElement e4 = (SPListElement)spListElementCreate(4, 4.0);

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
static bool spBPQueueIsEmptyTest(){   //TODO noa 13
	ASSERT_TRUE(spBPQueueIsEmpty(NULL)==false);

	SPBPQueue q = (SPBPQueue)spBPQueueCreate(3);
	SPListElement e1 =(SPListElement) spListElementCreate(1, 1.0);
	SPListElement e2 = (SPListElement)spListElementCreate(2, 2.0);
	SPListElement e3 = (SPListElement)spListElementCreate(3, 3.0);

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

static bool spBPQueueIsFullTest(){    //TODO noa 14
	ASSERT_TRUE(spBPQueueIsFull(NULL)==false);

	SPBPQueue q = (SPBPQueue)spBPQueueCreate(3);
	SPListElement e1 =(SPListElement) spListElementCreate(1, 1.0);
	SPListElement e2 = (SPListElement)spListElementCreate(2, 2.0);
	SPListElement e3 = (SPListElement)spListElementCreate(3, 3.0);

	ASSERT_TRUE(spBPQueueIsFull(q)==false);
	spBPQueueEnqueue(q, e2);  //value 2.0
	ASSERT_TRUE(spBPQueueIsFull(q)==false);
	spBPQueueEnqueue(q, e1);  //value 1.0
	spBPQueueEnqueue(q, e3);  //value 3.0
	ASSERT_TRUE(spBPQueueIsFull(q)==true);
	spBPQueueDequeue(q);
	ASSERT_TRUE(spBPQueueIsFull(q)==false);
	spBPQueueEnqueue(q, e3);  //value 3.0
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
	RUN_TEST(testQueueDestroy);
	RUN_TEST(testInsertQueue);
	RUN_TEST(testQueueCopy);
	RUN_TEST(testQueueClear);
	RUN_TEST(testInsertQueueWithDups);

	RUN_TEST(dequeueTest);
	RUN_TEST(spBPQueuePeekTest);
	RUN_TEST(spBPQueuePeekLastTest);
	RUN_TEST(spBPQueueMinValueTest);
	RUN_TEST(spBPQueueMaxValueTest);
	RUN_TEST(spBPQueueIsEmptyTest);
	RUN_TEST(spBPQueueIsFullTest);

	return 0;
}

