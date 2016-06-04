/*
 * SPBPriorityQueue.c
 *
 *  Created on: May 20, 2016
 *      Author: pshepsels
 */
#include "SPBPriorityQueue.h"
#include <stdio.h>
#include <stdlib.h>

SPBPQueue spBPQueueCreate(int maxSize)
{
	SPBPQueue newQueue;
	SPList l;

	if (maxSize < 1)
	{
		return NULL;
	}
	newQueue = (SPBPQueue)malloc(sizeof(struct sp_bp_queue_t));
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
	//create the list in order to connect to the queue
	l = spListCreate();
	newQueue->list = l;
	if (newQueue->list == NULL)
	{
		free(newQueue);
		return NULL;
	}
	return newQueue;
}

SPBPQueue spBPQueueCopy(SPBPQueue source)
{
	SPList l;
	SPBPQueue copyQueue;

	if (source == NULL)
	{
		return NULL;
	}

	if(spBPQueueSize(source) > 0)	// queue isn't empty, should copy all elements
		{
			copyQueue = (SPBPQueue)malloc(sizeof(struct sp_bp_queue_t));
			copyQueue->maxSize = source->maxSize;
			if (copyQueue == NULL)		// allocation failure
			{
				return NULL;
			}
			l = spListCopy(source->list);
			copyQueue->list = l;
		}
	else							// original queue is empty, can create new list.
	{
		copyQueue = spBPQueueCreate(source->maxSize);
		if (copyQueue == NULL)		// allocation failure
		{
			return NULL;
		}
	}
	return copyQueue;
}

void spBPQueueDestroy(SPBPQueue source)
{
	if(source != NULL)
	{
		// should only destroy the list and free its memory, and the queue's memory itself
		spListDestroy(source->list);
		free(source);
	}
	return;
}

void spBPQueueClear(SPBPQueue source)
{
	if (source == NULL) {
		return;
	}
	// clear the list
	spListClear(source->list);
	return;
}

int spBPQueueSize(SPBPQueue source)
{
	return spListGetSize(source->list);
}

int spBPQueueGetMaxSize(SPBPQueue source)
{
	return source->maxSize;
}

SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element)
{
	int cnt=0, listSize;
	bool isFull, alreadyAdded = false;
	SP_LIST_MSG m;
	SPList list;
	SPListElement curr;
	// temp init
	m = SP_BPQUEUE_INVALID_ARGUMENT;

	listSize = spBPQueueSize(source);

	// one of the arguments is invalid
	if (source == NULL || element == NULL)
	{
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}

	list = source->list;
	// the queue is at its max capacity
	isFull = (source->maxSize == listSize);
	// queue is empty, can set the new element to be the inserted element
	if (spListGetSize(list) == 0)
	{
		m = spListInsertFirst(list, element);
		if (m == SP_LIST_SUCCESS){
			return SP_BPQUEUE_SUCCESS;
		}
		else{
			return SP_BPQUEUE_INVALID_ARGUMENT;
		}
	}

	// queue is not empty
	// while loop to find the correct place to insert the node, until one before last element
	curr = spListGetFirst(list);
	if (curr == NULL)
	{
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	// the queue is not full, so should not delete last element after inserting
	if (!isFull)
	{
		// running on each element in the list
		while(curr != NULL)
		{
			// if value of the inserted element is smaller or equal to the current's value
			// or they both have the same value, but the new one has lower index
			// it should be inserted before current element
			if ((spListElementGetValue(curr) == spListElementGetValue(element) &&
					spListElementGetIndex(curr) > spListElementGetIndex(element))
					|| (spListElementGetValue(curr) > spListElementGetValue(element)))
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
			curr = spListGetNext(list);
		}
	}

	//list is full, should run until the end in order to delete the last element
	else
	{
		// running on each element in the list
		while(curr != NULL)
		{
			cnt++;
			// element should be inserted here

			// if value of the inserted element is smaller or equal to the current's value
			// or they both have the same value, but the new one has lower index, and not added already
			// it should be inserted before current element
			if (((spListElementGetValue(curr) == spListElementGetValue(element) &&
					spListElementGetIndex(curr) > spListElementGetIndex(element))
					|| (spListElementGetValue(curr) > spListElementGetValue(element))) &&
					alreadyAdded == false)

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
	if (spBPQueueIsEmpty(source)){return SP_BPQUEUE_EMPTY;}	//TODO paz
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
