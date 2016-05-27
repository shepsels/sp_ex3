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

bool queueCreateTest() {
	int maxSize = 3;
	SPBPQueue q = spBPQueueCreate(maxSize);
	ASSERT_TRUE(q!=NULL);
//	printf("%s %d %s %d %s" , "size of q.:", sizeof(q->list), "size of original:", sizeof(l), "\n");

	ASSERT_TRUE(q->maxSize == maxSize);
	ASSERT_TRUE(q->list != NULL);
//	spBPQueueDestroy(q); TODO uncomment

	return true;
}

static bool testQueueDestroy() {		//TODO test fails. should check if ok or if destroy not removing all
	SPBPQueue q = spBPQueueCreate(4);
	spBPQueueDestroy(q);
	ASSERT_TRUE(q == NULL);
	return true;
}

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
	return true;
}


int main() {
	RUN_TEST(queueCreateTest);
//	printf("1");
//	fflush(NULL);
//	RUN_TEST(testQueueDestroy);
//	printf("1");
//		fflush(NULL);
	RUN_TEST(testInsertQueue);
}

