/**
 * @file    Queue.h
 * @brief   **Static queue algorithm**
 *
 * This is a multipourpose queue implementation capable of handle any kind of data of any size, the
 * only limitation will be the target memory. The queue does not allow overwriting data it will just
 * stop adding new elemnts if is full
 */
#ifndef QUEUE_H__
#define QUEUE_H__


/**
 * @brief   Queue control structure
 */
typedef struct _QueueType
{
    void *Buffer;      /*!< Pointer to memory array to store data */
    uint32_t Elements; /*!< Number of elements to store queue size */
    uint8_t Size;      /*!< Size of the elements to store */
    uint32_t Head;     /*!< Head pointer */
    uint32_t Tail;     /*!< Tail pointer */
    uint8_t Empty;     /*!< Flag to signal when queue is empty */
    uint8_t Full;      /*!< flag to signal when the queue is full */
} QueueType;

void Queue_Init( QueueType *Queue, void *Buffer, uint32_t Elements, uint8_t Size );
uint8_t Queue_WriteData( QueueType *Queue, void *Data );
uint8_t Queue_ReadData( QueueType *Queue, void *Data );
uint8_t Queue_isQueueEmpty( QueueType *Queue );
void Queue_FlushQueue( QueueType *Queue );

#endif
