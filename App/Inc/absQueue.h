#ifndef _ABSQUEUE_H_
#define _ABSQUEUE_H_

//#include "bsp.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct QUEUE_ABS_T
{
    uint16_t front;
    uint16_t rear;
	uint16_t elemLen;
	uint16_t num;
	uint16_t maxNum;
	void *buf;
}QUEUE_ABS;

/**
 * @brief	初始化queue
 * @param	queue:目标队列
 * @return	Nothing
 */
void queueInit(QUEUE_ABS *queue, void *buf, uint16_t elemLen, uint16_t maxNum);

/**
 * @brief	清除queue
 * @param	queue:目标队列
 * @return	Nothing
 */
void queueClear(QUEUE_ABS *queue);

/**
 * @brief	判断queue是否为空
 * @param	queue:目标队列
 * @return	Nothing
 */
bool queueIsNull(QUEUE_ABS *queue);

/**
 * @brief	判断queue是否为满
 * @param	queue:目标队列
 * @return	Nothing
 */
bool queueIsFull(QUEUE_ABS *queue);

/**
 * @brief	获取队列元素数目  
 * @param	queue:目标队列
 * @return	Nothing
 */
uint16_t getQueueElemNum(QUEUE_ABS *queue);

/**
 * @brief	将数据帧推入环形队列
 * @param	data:传入的数据
            queue:目标队列
 * @return	Nothing
 */
bool PushElemToQueue(QUEUE_ABS *queue, void *data);

/**
 * @brief	从环形队列取出数据帧
 * @param	data:传出的数据
            queue:目标队列
 * @return	Nothing
 */
bool GetElemFromQueue(QUEUE_ABS *queue, void *data);

/**
 * @brief	从环形队列从队头取出一个元素不删除元素
 * @param	data:传出的数据
            queue:目标队列
 * @return	Nothing
 */
bool GetElemFromQueueNoDel(QUEUE_ABS *queue, void *data);

/**
 * @brief	从环形队列从队头删除一个元素
 * @param	data:传出的数据
            queue:目标队列
 * @return	Nothing
 */
bool DelElemFromQueue(QUEUE_ABS *queue);

/**
 * @brief	从环形队列从队头取出所有元素
 * @param	data:传出的数据
            queue:目标队列
 * @return	Nothing
 */
bool GetAllElemFromQueue(QUEUE_ABS *queue, void *data);

/**
 * @brief	从环形队列取出数据帧
 * @param	data:传出的数据
            queue:目标队列
 * @return	Nothing
 */
bool PopMsgFromQueue(QUEUE_ABS *queue, void *data);


#endif //_ABSQUEUE_H_

