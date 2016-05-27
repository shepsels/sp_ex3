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


SPBPQueue spBPQueueCreate(int maxSize)			// TODO Paz 1
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

void spBPQueueDestroy(SPBPQueue source)			// TODO Paz 3
{
	spListClear(source->list);
	spListDestroy(source->list);
	free(source);
	return;
}

void spBPQueueClear(SPBPQueue source)			// TODO Paz 4
{
	if (source == NULL) {
		return;
	}
	spListClear(source->list);
	return;
}

int spBPQueueSize(SPBPQueue source)				// TODO Paz 5
{
	return spListGetSize(source->list);
}

int spBPQueueGetMaxSize(SPBPQueue source)		// TODO Paz 6
{
	return source->maxSize;
}

SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element)	//TODO Paz 7
{
	int cnt=0;
	bool alreadyAdded = false;
	SP_LIST_MSG m = SP_BPQUEUE_INVALID_ARGUMENT;	// TODO temp init
	int listSize = spBPQueueSize(source);
	if (source == NULL || element == NULL)			// one of the arguments is invalid
	{
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	SPList list = source->list;
	bool isFull = (source->maxSize == listSize);	// the queue is at its max capacity
	if (spListGetSize(list) == 0)					// queue is empty
	{
		SP_LIST_MSG m = spListInsertFirst(list, element);
		if (m == SP_LIST_OUT_OF_MEMORY){
				return SP_BPQUEUE_OUT_OF_MEMORY;
			}
		if (m == SP_LIST_SUCCESS){
			return SP_BPQUEUE_SUCCESS;
		}
	}
	// while loop to find the correct place to insert the node, until one before last element
	SPListElement curr = spListGetFirst(list);
	if (!isFull)
	{
		while(curr != NULL)						// end of the list
		{
			if (curr->value > element->value) 	// element should be inserted here
			{
				m = spListInsertBeforeCurrent(list, element);
				if (m == SP_LIST_OUT_OF_MEMORY){
						return SP_BPQUEUE_OUT_OF_MEMORY;
					}
				if (m == SP_LIST_SUCCESS){
					return SP_BPQUEUE_SUCCESS;
				}
			}
			curr = spListGetNext(list);
		}
	}
	else							//list is full, should run until the end in order to delete
	{
		while(curr != NULL)						// end of the list
		{
			cnt++;
			if (curr->value > element->value && alreadyAdded == false) 	// element should be inserted here
			{
				m = spListInsertBeforeCurrent(list, element);
				alreadyAdded = true;
			}
			if (cnt == source->maxSize && alreadyAdded)			// that is the last element
			{
				spListRemoveCurrent(list);
				break;
			}
			curr = spListGetNext(list);
		}
		//end of loop

		if (m == SP_LIST_OUT_OF_MEMORY){
				return SP_BPQUEUE_OUT_OF_MEMORY;
			}
		if (m == SP_LIST_SUCCESS){
			return SP_BPQUEUE_SUCCESS;
		}
	}
	return SP_BPQUEUE_FULL;
}



