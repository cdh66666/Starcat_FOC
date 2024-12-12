/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               RingBuffer.h
** Latest modified date:    2019-11-06
** Latest version:          V1.1.0
** Description:             环形队列
**
**------------------------------------------------------------
** Created by:          Liu Zhufu
** Created date:        2016-06-01
** Version:             V1.0.0
** Descriptions:        Ring buffer self implementation
**
**------------------------------------------------------------
** Modified by:         Chen JunTong
** Modified date:       2019-11-06
** Version:             V1.1.0
** Description:         环形队列
**
*************************************************************/
#include "RingBuffer.h"
#include <string.h>

/*************************************************************
** Function name:       RingBuffer_Clear
** Descriptions:        清除环形队列
** Input parameters:    p：参数结构体
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void RingBuffer_Clear(PRingBufferPARM p)
{
    // Dynamic property
    p->deIndex = 0;
    p->enIndex = 0;
    p->deNum = 0;
    p->enNum = 0;
}

/*************************************************************
** Function name:       RingBuffer_En
** Descriptions:        环形队列入队
** Input parameters:    p：参数结构体
**                      pMsg：写入消息指针
** Output parameters:   None
** Returned value:      返回入队是否成功 0成功 1：失败队列已满
** Remarks:             None
*************************************************************/
uint8_t RingBuffer_En(PRingBufferPARM p, void *pMsg)
{
	if (RingBuffer_IsFull(p) == true) {
		return 1;
	}
    if(pMsg) {
        memcpy((uint8_t *)p->msgBuff + p->enIndex * p->msgSize, pMsg, p->msgSize);
    }
    p->enIndex = (p->enIndex + 1) % p->maxMsgNum;
    p->enNum ++;
    return 0;
}

/*************************************************************
** Function name:       RingBuffer_Look
** Descriptions:        看环形队列数据
** Input parameters:    p：参数结构体
**                      pMsg：存放消息指针
** Output parameters:   None
** Returned value:      返回出队是否成功 0成功 1：队列已空
** Remarks:             None
*************************************************************/
uint8_t RingBuffer_Look(PRingBufferPARM p, void *pMsg, uint16_t nums)
{
    uint16_t i = 0;
    uint16_t index;
    index = p->deIndex;
	if (RingBuffer_GetMsgNum(p) < nums) {
		return 1;
	}
    if(pMsg) {
        for (i = 0; i < nums; i++) {
			memcpy((uint8_t *)pMsg + p->msgSize * i, (uint8_t *)p->msgBuff + p->deIndex * p->msgSize, p->msgSize);
            p->deIndex = (p->deIndex + 1) % p->maxMsgNum;
        }
    }
    p->deIndex = index;
    return 0;
}

/*************************************************************
** Function name:       RingBuffer_De
** Descriptions:        环形队列消息出队
** Input parameters:    p：参数结构体
**                      pMsg：存放消息指针
** Output parameters:   None
** Returned value:      返回出队是否成功 0成功 1：队列已空
** Remarks:             None
*************************************************************/
uint8_t RingBuffer_De(PRingBufferPARM p, void *pMsg)
{
	if (RingBuffer_IsEmpty(p) == true) {
		return 1;
	}
    if(pMsg) {
        memcpy(pMsg, (uint8_t *)p->msgBuff + p->deIndex * p->msgSize, p->msgSize);
    }
    p->deIndex = (p->deIndex + 1) % p->maxMsgNum;
    p->deNum ++;
    return 0;
}






/*************************************************************
** Function name:       RingBuffer_IsEmpty
** Descriptions:        返回队列是否为空
** Input parameters:    p：参数结构体
** Output parameters:   None
** Returned value:      true：为空，false：不为空
** Remarks:             None
*************************************************************/
bool RingBuffer_IsEmpty(PRingBufferPARM p)
{
    return p->enIndex == p->deIndex;
}

/*************************************************************
** Function name:       RingBuffer_IsFull
** Descriptions:        返回队列是否存满
** Input parameters:    p：参数结构体
** Output parameters:   None
** Returned value:      true：存满，false：为存满
** Remarks:             None
*************************************************************/
bool RingBuffer_IsFull(PRingBufferPARM p)
{
	return p->deIndex == (p->enIndex + 1) % p->maxMsgNum;
}
/*************************************************************
** Function name:       RingBuffer_GetUnuseMsgNum
** Descriptions:        返回可存储的消息个数
** Input parameters:    p：参数结构体
** Output parameters:   None
** Returned value:      可存储的消息个数
** Remarks:             None
*************************************************************/
uint32_t RingBuffer_GetUnuseMsgNum(PRingBufferPARM p)
{
	return p->maxMsgNum - p->enNum + p->deNum;
}
/*************************************************************
** Function name:       RingBuffer_GetMsgNum
** Descriptions:        返回已经使用的消息的个数
** Input parameters:    p：参数结构体
** Output parameters:   None
** Returned value:      可存储的消息个数
** Remarks:             None
*************************************************************/
uint32_t RingBuffer_GetMsgNum(PRingBufferPARM p)
{
	return p->enNum - p->deNum;
}










