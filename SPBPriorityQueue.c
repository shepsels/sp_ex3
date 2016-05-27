/*
 * SPBPriorityQueue.c
 *
 *  Created on: May 20, 2016
 *      Author: pshepsels
 */
#include "SPBPriorityQueue.h"
#include <stdio.h>
#include <stdlib.h>
#include "SPList.h"


SPBPQueue spBPQueueCreate(int maxSize)		// TODO Paz 1
{
	if (maxSize < 0)
	{
		return NULL;
	}
	SPBPQueue newQueue = (SPBPQueue)malloc(sizeof(struct sp_bp_queue_t));
	if (newQueue == NULL)		// allocation failure
	{
		return NULL;
	}
	newQueue->maxSize = maxSize;
	if (newQueue->maxSize == NULL)
	{
		free(newQueue);
		return NULL;
	}
	newQueue->list = spListCreate();
	if (newQueue->list == NULL)
	{
		free(newQueue);
		return NULL;
	}
	return newQueue;
}

SPBPQueue spBPQueueCopy(SPBPQueue source)		// TODO Paz 2
{
	if (source == NULL)
	{
		return NULL;
	}
	SPBPQueue copyQueue;
	copyQueue = spBPQueueCreate(source->maxSize);
	// TODO not sure if should do malloc and
																// on which size (of list)
	if (copyQueue == NULL)		// allocation failure
	{
		return NULL;
	}
	copyQueue->list = spListCopy(source);
	return copyQueue;
}

