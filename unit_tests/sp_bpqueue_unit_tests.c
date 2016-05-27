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


int main() {
	RUN_TEST(queueCreateTest);
}

