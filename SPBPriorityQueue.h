#ifndef SPBPRIORITYQUEUE_H_
#define SPBPRIORITYQUEUE_H_
#include "SPListElement.h"
#include <stdbool.h>
#include "SPList.h"
/**
 * SP Bounded Priority Queue summary
 *
 * TODO Complete documentation
 */


/** type used to define Bounded priority queue **/
typedef struct sp_bp_queue_t* SPBPQueue;

struct sp_bp_queue_t{	//TODO move from here
	int maxSize;
	SPList list;
};

/** type for error reporting **/
typedef enum sp_bp_queue_msg_t {
	SP_BPQUEUE_OUT_OF_MEMORY,
	SP_BPQUEUE_FULL,
	SP_BPQUEUE_EMPTY,
	SP_BPQUEUE_INVALID_ARGUMENT,
	SP_BPQUEUE_SUCCESS
} SP_BPQUEUE_MSG;

/**
 * Allocates a new Bounded Priority Queue in the memory.
 *
 * @param
 * int maxSize, the maximum capacity of the queue.
 * @return
 * NULL in case allocation failure ocurred,
 * Otherwise, the new queue is returned
 */

SPBPQueue spBPQueueCreate(int maxSize);

/**
 * Creates a copy of target bpqueue.
 *
 * The new copy will contain all the elements from the source queue
 *
 * @param source The target queue to copy
 * @return
 * NULL if a NULL was sent or a memory allocation failed.
 * A new queue containing the same elements otherwise.
 */

SPBPQueue spBPQueueCopy(SPBPQueue source);

/**
 * TODO Complete documentation
 */
void spBPQueueDestroy(SPBPQueue source);

/**
 * TODO Complete documentation
 */
void spBPQueueClear(SPBPQueue source);

/**
 * TODO Complete documentation
 */
int spBPQueueSize(SPBPQueue source);

/**
 * TODO Complete documentation
 */
int spBPQueueGetMaxSize(SPBPQueue source);

/**
 * TODO Complete documentation
 */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element);

/**
 * TODO Complete documentation
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source);

/**
 * TODO Complete documentation
 */
SPListElement spBPQueuePeek(SPBPQueue source);

/**
 * TODO Complete documentation
 */
SPListElement spBPQueuePeekLast(SPBPQueue source);

/**
 * TODO Complete documentation
 */
double spBPQueueMinValue(SPBPQueue source);

/**
 * TODO Complete documentation
 */
double spBPQueueMaxValue(SPBPQueue source);

/**
 * TODO Complete documentation
 */
bool spBPQueueIsEmpty(SPBPQueue source);

/**
 * TODO Complete documentation
 */
bool spBPQueueIsFull(SPBPQueue source);

#endif
