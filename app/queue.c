/**
 * @file    Queue.c
 * @brief   **Static queue algorithm**
 *
 * This is a multipourpose queue implementation capable of handle any kind of Data of any syze, the
 * only limitation will be the target memory. The queue does not allow overwriting Data it will just
 * stop adding new Elements when is Full
 */
#include <stdint.h>
#include "queue.h"
#include <string.h>

/**
  * @defgroup Boolean true and flase definitions
  @{ */
#ifndef FALSE
#define FALSE 0u /*!< FALSE definition */
#endif

#ifndef TRUE
#define TRUE 1u /*!< TRUE definition */
#endif
/**
  @} */

/**
 * @brief   **Queue intilization**
 *
 * Set the internal queue Elements as a queue Empty for the first time, Head and Tail set zero and
 * indication of queue not Full and Empty
 *
 * @param Queue pointer to a QueueType structure that contains queue control information.
 */
/* cppcheck-suppress misra-c2012-8.7 ; external linkage is neccesary */
void Queue_Init( QueueType *Queue, void *Buffer, uint32_t Elements, uint8_t Size )
{
    Queue->Buffer   = Buffer;
    Queue->Elements = Elements;
    Queue->Size     = Size;

    Queue->Empty = TRUE;
    Queue->Full  = FALSE;
    Queue->Head  = 0u;
    Queue->Tail  = 0u;
}

/**
 * @brief   **Write one element into the queue**
 *
 * Write an element Data into the queue if there is room for one more element, the queue first 
 * make sure if is not full before writing the Data, in case of full queue the Data will not be written
 * and the function will return FALSE, otherwise the Data will be written and the function will return TRUE
 *
 * @param Queue pointer to a QUEUE_HandleTypeDef structure that contains queue control information.
 * @param Data   pointer to Data to be written
 *
 * @retval  #TRUE if Data could be written otherwise #FALSE
 */
uint8_t Queue_WriteData( QueueType *Queue, void *Data )
{
    uint8_t result = FALSE; /*assume queue is full*/

    /* fist check if queue is not full*/
    if( Queue->Full == FALSE )
    {
        /* cppcheck-suppress misra-c2012-11.5 ; convertion is neccesary to supress compiler warning */
        (void)memcpy( &( (uint8_t *)Queue->Buffer )[ Queue->Head * Queue->Size ], Data, Queue->Size );
        Queue->Empty = FALSE;           /*signal queue is not empty*/
        Queue->Head++;                  /*move head pointer to the next element*/
        Queue->Head %= Queue->Elements; /*if head pointer is at the end of the queue, move it to the beginning*/

        /*check if queue is full, this only could happens when Head pointer reach Tail after
        writting an element*/
        if( Queue->Head == Queue->Tail )
        {
            Queue->Full = TRUE;
        }

        result = TRUE;
    }

    return result;  /*return the result of the operation*/
}

/**
 * @brief   **Read one element from the queue**
 *
 * Read an element Data from the queue if there is at least one element, the queue first make sure
 * if is not empty before reading the Data, in case of empty queue the Data will not be read and the
 * function will return FALSE, otherwise the Data will be read and the function will return TRUE
 *
 * @param Queue pointer to a QueueType structure that contains queue control information.
 * @param Data   pointer to address to store the Data read
 *
 * @retval  #TRUE if Data could be read otherwise #FALSE
 */
uint8_t Queue_ReadData( QueueType *Queue, void *Data )
{
    uint8_t result = FALSE; /*assume queue is empty*/

    /* first check if queue is not empty*/
    if( Queue->Empty == FALSE )
    {
        /* cppcheck-suppress misra-c2012-11.5 ; convertion is neccesary to supress compiler warning */
        (void)memcpy( Data, &( (uint8_t *)Queue->Buffer )[ Queue->Tail * Queue->Size ], Queue->Size ); /* cppcheck-suppress misra-c2012-18.4*/
        Queue->Full = FALSE;            /*signal queue is not full*/
        Queue->Tail++;                  /*move tail pointer to the next element*/
        Queue->Tail %= Queue->Elements; /*if tail pointer is at the end of the queue, move it to the beginning*/

        /*check if queue is empty, this only could happens when Tail pointer reach Head after
        reading an element*/
        if( Queue->Tail == Queue->Head )
        {
            Queue->Empty = TRUE;
        }

        result = TRUE;
    }

    return result; /*return the result of the operation*/
}

/**
 * @brief   **Inquier if the queue is Empty**
 *
 * Return the internal queue Empty flag, the flag is set when the read and write operations
 * has been carried out and the queue is empty
 *
 * @param Queue pointer to a QueueType structure that contains queue control information
 *
 * @retval  #TRUE if queue is Empty otherwise #FALSE
 */
uint8_t Queue_isQueueEmpty( QueueType *Queue )
{
    return Queue->Empty;
}

/**
 * @brief   **Flush the queue to Empty state**
 *
 * Set the internal queue Empty flag, Head and Tail set zero and indication of queue not Full and Empty
 * this function will not erase the data stored in the queue, just set the queue to Empty state
 *
 * @param Queue pointer to a QueueType structure that contains queue control information
 */
void Queue_FlushQueue( QueueType *Queue )
{
    Queue_Init( Queue, Queue->Buffer, Queue->Elements, Queue->Size );
}
