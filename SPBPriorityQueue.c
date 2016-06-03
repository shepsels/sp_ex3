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
	if (maxSize < 1)
	{
		return NULL;
	}
	SPBPQueue newQueue = (SPBPQueue)malloc(sizeof(struct sp_bp_queue_t));
	if (newQueue == NULL)		// allocation failure
	{
		return NULL;
	}
	newQueue->maxSize = maxSize;
	if (newQueue->maxSize < 0)
	{
		free(newQueue);
		return NULL;
	}
	SPList l = spListCreate();
	newQueue->list = l;
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

	if(spBPQueueSize(source) > 0)	// queue isn't empty, should copy all elements
		{
			copyQueue = (SPBPQueue)malloc(sizeof(struct sp_bp_queue_t));
			copyQueue->maxSize = source->maxSize;
			if (copyQueue == NULL)		// allocation failure
			{
				return NULL;
			}
			SPList l = spListCopy(source->list);
			copyQueue->list = l;
		}
	else							// original queue is empty
	{
		copyQueue = spBPQueueCreate(source->maxSize);
		if (copyQueue == NULL)		// allocation failure
		{
			return NULL;
		}
	}
	return copyQueue;
}

void spBPQueueDestroy(SPBPQueue source)			// TODO Paz 3
{
	if(source != NULL)
	{
		spListDestroy(source->list);
		free(source);
	}
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
	int cnt=0, listSize;
	bool isFull, alreadyAdded = false;
	SP_LIST_MSG m;
	SPList list;
	SPListElement curr;
	m = SP_LIST_NULL_ARGUMENT;						// temp init

	listSize = spBPQueueSize(source);
	if (source == NULL || element == NULL)			// one of the arguments is invalid
	{
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	list = source->list;
	isFull = (source->maxSize == listSize);			// the queue is at its max capacity
	if (spListGetSize(list) == 0)					// queue is empty
	{
		m = spListInsertFirst(list, element);
		if (m == SP_LIST_SUCCESS){
			return SP_BPQUEUE_SUCCESS;
		}
		else{
			return SP_BPQUEUE_INVALID_ARGUMENT;
		}
	}
	// while loop to find the correct place to insert the node, until one before last element
	curr = spListGetFirst(list);
	if (curr == NULL)
	{
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	if (!isFull)
	{
		while(curr != NULL)						// end of the list
		{
			if (curr->value >= element->value) 	// element should be inserted here
			{
				if ((curr->value == element->value && curr->index > element->index) ||
						(curr->value > element->value))
				{
					m = spListInsertBeforeCurrent(list, element);
					alreadyAdded = true;
					if (m == SP_LIST_OUT_OF_MEMORY){
							return SP_BPQUEUE_OUT_OF_MEMORY;
						}
					if (m == SP_LIST_SUCCESS){
						return SP_BPQUEUE_SUCCESS;
					}
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
			if (((curr->value == element->value && curr->index > element->index) ||
				(curr->value > element->value)) && alreadyAdded == false) 	// element should be inserted here
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
	}
	//end of loop. should check if element should be inserted at the end
	if(alreadyAdded == false && ! isFull)
	{
		m = spListInsertLast(list, element);
	}
	if (m == SP_LIST_OUT_OF_MEMORY){
			return SP_BPQUEUE_OUT_OF_MEMORY;
		}
	if (m == SP_LIST_SUCCESS){
		return SP_BPQUEUE_SUCCESS;
	}

	return SP_BPQUEUE_FULL;
}
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source){  //TODO noa 8
	SPList list = (SPList)source->list;
	SP_LIST_MSG m;
	if (source == NULL)			// the argument is invalid
		{
			return SP_BPQUEUE_INVALID_ARGUMENT;
		}
	spListGetFirst(list);  // list->current = spListGetFirst(list);
	m=spListRemoveCurrent(list);
	switch (m){
	case SP_LIST_OUT_OF_MEMORY:
		break;
	case SP_LIST_NULL_ARGUMENT:
		return SP_BPQUEUE_INVALID_ARGUMENT;
	case SP_LIST_INVALID_CURRENT:
		return SP_BPQUEUE_INVALID_ARGUMENT;
	case SP_LIST_SUCCESS:
		return SP_BPQUEUE_SUCCESS;
	}
	return SP_BPQUEUE_SUCCESS;
}

SPListElement spBPQueuePeek(SPBPQueue source){   //TODO  noa 9  tester if it is the lowest value
	if (source==NULL || source->list==NULL){  //TODO documantation!!!
		return NULL;
	}
	SPList list = (SPList)source->list;
	SPListElement copy = spListElementCopy(spListGetFirst(list));
	return copy;
}

SPListElement spBPQueuePeekLast(SPBPQueue source){  //TODO  noa 10 tester if it is the highest value
	if (source==NULL || source->list==NULL){
			return NULL;
	}
	int cnt=0;
	SPList list = (SPList)source->list;
	SPListElement curr = spListGetFirst(list);
	while (cnt<spListGetSize(source->list)){ //getting the last node
		cnt++;
		if (cnt<spListGetSize(source->list)){
			curr=spListGetNext(list);
		}
	}
	SPListElement copy = spListElementCopy(curr);
	return copy;  //TODO if list is empty, returning NULL??

}

double spBPQueueMinValue(SPBPQueue source){ //TODO noa 11 same as above
	if (source==NULL || source->list==NULL ){
		return -1.0;
	}
	SPList list = (SPList)source->list;
	SPListElement firstE=(SPListElement) spListGetFirst(list);
	return spListElementGetValue(firstE);
}

double spBPQueueMaxValue(SPBPQueue source){ //TODO noa 12 same as above
	if (source==NULL){
		return -1.0;
	}
	int cnt=0;
	SPList list = (SPList)source->list;
	SPListElement curr = spListGetFirst(list);
	while (cnt<spListGetSize(source->list)){ //getting the last node
		cnt++;
		if (cnt<spListGetSize(source->list)){
			curr=spListGetNext(list);
		}
	}
//	SPListElement lastE = (SPListElement)spListElementCopy(curr);	//TODO Paz: why copy? there's memory leak here
	return spListElementGetValue(curr);
}

bool spBPQueueIsEmpty(SPBPQueue source){  //TODO noa 13
	if ((source!=NULL) && ((source->list)!=NULL) && ((spListGetSize(source->list))==0)){
		return true;
	}
	return false;
}

bool spBPQueueIsFull(SPBPQueue source){  //TODO noa 14
	if ((source!=NULL) && (source->list!=NULL) && (spListGetSize(source->list)==source->maxSize)){
		return true;
	}
	return false;
}
