/************************************************************
  Copyright (C),
  FileName:     absQueue.c
  Author:       AlexLu
  Version :     1.0
  Date:         20210104
  Description:  ��Ϣ��������  
  Version:      1.0
  Function List:   
    1. 
  History:         
      <author>  <time>       <version >   <desc>
***********************************************************/

#include "absQueue.h"

/**
 * @brief	��ʼ��queue
 * @param	queue:Ŀ�����
 * @return	Nothing
 */
void queueInit(QUEUE_ABS *queue, void *buf, uint16_t elemLen, uint16_t maxNum)
{
	if((queue == NULL) || (buf == NULL))
	{
		return;
	}
	memset(queue, 0, sizeof(QUEUE_ABS));
	queue->buf		= buf;
	queue->elemLen	= elemLen;
	queue->maxNum 	= maxNum;
}

/**
 * @brief	���queue
 * @param	queue:Ŀ�����
 * @return	Nothing
 */
void queueClear(QUEUE_ABS *queue)
{
	if(queue == NULL)
	{
		return;
	}
	queue->front 	= 0;
	queue->rear 	= 0;
	queue->num		= 0;
}

/**
 * @brief	�ж�queue�Ƿ�Ϊ��
 * @param	queue:Ŀ�����
 * @return	Nothing
 */
bool queueIsNull(QUEUE_ABS *queue)
{
	if (queue->num == 0)                                        				//���ζ���Ϊ��
    {
        return true;
    }
	return false;
}

/**
 * @brief	�ж�queue�Ƿ�Ϊ��
 * @param	queue:Ŀ�����
 * @return	Nothing
 */
bool queueIsFull(QUEUE_ABS *queue)
{
	if (queue->num == queue->maxNum)                                        	//���ζ����Ƿ���
    {
        return true;
    }
	return false;
}

/**
 * @brief	��ȡ����Ԫ����Ŀ  
 * @param	queue:Ŀ�����
 * @return	Nothing
 */
uint16_t getQueueElemNum(QUEUE_ABS *queue)
{
	return(queue->num);
}

/**
 * @brief	��һ��Ԫ�شӶ�β���뻷�ζ���
 * @param	data:���������
            queue:Ŀ�����
 * @return	Nothing
 */
bool PushElemToQueue(QUEUE_ABS *queue, void *data)
{
	if((queue == NULL) || (data == NULL))
	{
		return false;
	}
    if (queue->num == queue->maxNum)         									//���ζ�����,���Ƕ���ͷ������
    {
        // queue->front = (queue->front + 1) % queue->maxNum;
		// queue->front++;
    }
	else
	{
		queue->num++;
	}
	memcpy((void *)((uint32_t)queue->buf + queue->rear * queue->elemLen), data, queue->elemLen);

    queue->rear++;//queue->rear = ((queue->rear + 1) % queue->maxNum);                 			//�ƶ��洢ָ��
	if (queue->rear >= queue->maxNum)         									//���ζ�����,���Ƕ���ͷ������
    {
		queue->rear = queue->maxNum;
	}

    return true;
}

// bool PushElemToQueue(QUEUE_ABS *queue, void *data)
// {
// 	if((queue == NULL) || (data == NULL))
// 	{
// 		return false;
// 	}
//     if (queue->num == queue->maxNum)         									//���ζ�����,���Ƕ���ͷ������
//     {
//         queue->front = (queue->front + 1) % queue->maxNum;
//     }
// 	else
// 	{
// 		queue->num++;
// 	}
// #if 0
//     memcpy(&(queue->buf[queue->rear]), data, sizeof(uint16_t) * queue->elemLen);
// #else
// 	memcpy((void *)((uint32_t)queue->buf + queue->rear * queue->elemLen), data, queue->elemLen);
// #endif //0
//     queue->rear = ((queue->rear + 1) % queue->maxNum);                 			//�ƶ��洢ָ��
	
//     return true;
// }

/**
 * @brief	�ӻ��ζ��дӶ�ͷȡ��һ��Ԫ��
 * @param	data:����������
            queue:Ŀ�����
 * @return	Nothing
 */
bool GetElemFromQueue(QUEUE_ABS *queue, void *data)
{
	if((queue == NULL) || (data == NULL))
	{
		return false;
	}
    if (queue->num == 0)                                      					//���ζ���Ϊ��
    {
		queue->front 	= 0;
		queue->rear 	= 0;
        return false;
    }
	
	memcpy(data, (void *)((uint32_t)queue->buf + queue->front * queue->elemLen), queue->elemLen);

	queue->front = (queue->front + 1) % queue->maxNum; 
	queue->num--;
	if (queue->front >= queue->maxNum)                                      					//���ζ���Ϊ��
    {
		
		queue->num = queue->maxNum;
	}
    return true;
}

// bool GetElemFromQueue(QUEUE_ABS *queue, void *data)
// {
// 	if((queue == NULL) || (data == NULL))
// 	{
// 		return false;
// 	}
//     if (queue->num == 0)                                      					//���ζ���Ϊ��
//     {
//         return false;
//     }
// 	//memset(data, 0, queue->elemLen);                    						//ȡ��ָ���ƶ�
// #if 0
// 	memcpy(data, &(queue->buf[queue->front]), sizeof(uint16_t) * queue->elemLen);
// #else
// 	memcpy(data, (void *)((uint32_t)queue->buf + queue->front * queue->elemLen), queue->elemLen);
// #endif //0
// 	queue->front = (queue->front + 1) % queue->maxNum; 
// 	queue->num--;
//     return true;
// }

/**
 * @brief	�ӻ��ζ��дӶ�ͷȡ��һ��Ԫ�ز�ɾ��Ԫ��
 * @param	data:����������
            queue:Ŀ�����
 * @return	Nothing
 */
bool GetElemFromQueueNoDel(QUEUE_ABS *queue, void *data)
{
	if((queue == NULL) || (data == NULL))
	{
		return false;
	}
    if (queue->num == 0)                                      					//���ζ���Ϊ��
    {
        return false;
    }
	memcpy(data, (void *)((uint32_t)queue->buf + queue->front * queue->elemLen), queue->elemLen);
    return true;
}

/**
 * @brief	�ӻ��ζ��дӶ�ͷɾ��һ��Ԫ��
 * @param	data:����������
            queue:Ŀ�����
 * @return	Nothing
 */
bool DelElemFromQueue(QUEUE_ABS *queue)
{
	if(queue == NULL)
	{
		return false;
	}
    if (queue->num == 0)                                      					//���ζ���Ϊ��
    {
        return false;
    }
	queue->front = (queue->front + 1) % queue->maxNum; 
	queue->num--;
    return true;
}


/**
 * @brief	�ӻ��ζ��дӶ�ͷȡ������Ԫ��
 * @param	data:����������
            queue:Ŀ�����
 * @return	Nothing
 */
bool GetAllElemFromQueue(QUEUE_ABS *queue, void *data)
{
	if((queue == NULL) || (data == NULL))
	{
		return false;
	}
    if (queue->num == 0)                                      					//���ζ���Ϊ��
    {
        return false;
    }
#if 0
	memcpy(data, (void *)((uint32_t)queue->buf + queue->front * queue->elemLen), 
		queue->elemLen * (queue->num - queue->front));
	memcpy((void *)((uint32_t)data + queue->elemLen * (queue->num - queue->front)),
		queue->buf, queue->front * queue->elemLen);
#else
	if(queue->front + queue->num <= queue->maxNum)
	{
		memcpy((void *)((uint32_t)data + queue->elemLen * (queue->maxNum - queue->num)), 
			(void *)((uint32_t)queue->buf + queue->front * queue->elemLen),
			queue->elemLen * queue->num);										//δ������������� 
	}
	else
	{
		memcpy(data, (void *)((uint32_t)queue->buf + queue->front * queue->elemLen), 
			queue->elemLen * (queue->maxNum - queue->front));
		memcpy((void *)((uint32_t)data + queue->elemLen * (queue->maxNum - queue->front)),
			queue->buf, (queue->num + queue->front - queue->maxNum) * queue->elemLen);
	}
#endif //0
	queue->front 	= 0;
	queue->rear 	= 0;
	queue->num		= 0;
    return true;
}

/**
 * @brief	�ӻ��ζ���ȡ��һ��Ԫ��
 * @param	data:����������
            queue:Ŀ�����
 * @return	Nothing
 */
bool PopMsgFromQueue(QUEUE_ABS *queue,  void *data)
{
	if((queue == NULL) || (data == NULL))
	{
		return false;
	}
    return(GetElemFromQueue(queue, data));
}


