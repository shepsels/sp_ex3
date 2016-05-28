/*
 * sp_bpqueue_unit_tests.c
 *
 *  Created on: May 26, 2016
 *      Author: pshepsels
 */

#include "unit_test_util.h"
#include "../SPListElement.h"
#include "../SPList.h"
#include "../SPBPriorityQueue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

bool queueCreateTest() {				//TODO Paz 1
	int maxSize = 3;
	SPBPQueue q = spBPQueueCreate(maxSize);
	ASSERT_TRUE(q!=NULL);
	ASSERT_TRUE(q->maxSize == maxSize);
	ASSERT_TRUE(q->list != NULL);
	spBPQueueDestroy(q);

	return true;
}

static bool testQueueDestroy() {		//TODO Paz 2
	spBPQueueDestroy(NULL);
	return true;
}

static bool testInsertQueue() {			//TODO Paz 3
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
	spBPQueueDestroy(q1);
	return true;
}

static bool testQueueCopy() {			//TODO Paz 4
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
	spBPQueueDestroy(q);
	spBPQueueDestroy(copy);
	spBPQueueDestroy(copy2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

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
	ASSERT_TRUE(0 == spBPQueueSize(q));
	SPBPQueue q2 = spBPQueueCreate(3);
	spBPQueueClear(q2);
	ASSERT_TRUE(0 == spBPQueueSize(q2));
	spBPQueueDestroy(q);
	spBPQueueDestroy(q2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}



int main() {
	RUN_TEST(queueCreateTest);
	RUN_TEST(testQueueDestroy);
	RUN_TEST(testInsertQueue);
	RUN_TEST(testQueueCopy);
	RUN_TEST(testQueueClear);
}

