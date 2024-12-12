#ifndef RingBuffer_h
#define RingBuffer_h

#include <stdint.h>
#include <stdbool.h>

#pragma pack(1)
struct RingBufferParm {
    void *msgBuff;                      /* 消息buf */
    uint32_t maxMsgNum;                 /* 最大消息个数 */
    uint32_t msgSize;                   /* 消息单元大小 */
    volatile uint32_t deIndex;          /* 出队索引 */
    volatile uint32_t enIndex;          /* 入队索引 */
	volatile uint32_t deNum;          	/* 出队个数 */
    volatile uint32_t enNum;          	/* 入队个数 */
};
typedef struct RingBufferParm RingBufferPARM; /* 定义类型*/
typedef RingBufferPARM *PRingBufferPARM; /* 定义指针类型*/
#pragma pack()


extern bool RingBuffer_IsEmpty(PRingBufferPARM p);
extern bool RingBuffer_IsFull(PRingBufferPARM p);
extern void RingBuffer_Clear(PRingBufferPARM p);
extern uint8_t RingBuffer_En(PRingBufferPARM p, void *pMsg);
extern uint8_t RingBuffer_De(PRingBufferPARM p, void *pMsg);
extern uint32_t RingBuffer_GetUnuseMsgNum(PRingBufferPARM p);
extern uint32_t RingBuffer_GetMsgNum(PRingBufferPARM p);
extern uint8_t RingBuffer_Look(PRingBufferPARM p, void *pMsg, uint16_t nums);
/*************************************************************
** Function name:       RingBuffer_EXPORT
** Descriptions:        环形队列初始化
** Input parameters:    x：参数结构体
**                      pMsg：存放消息指针
** Output parameters:   None
** Returned value:      返回出队的索引位置
** Remarks:             None
*************************************************************/
#define RingBuffer_EXPORT(x,type,num)                        	\
		type x##Buffer[num];                                    \
		RingBufferPARM x = {									\
			x##Buffer,                                          \
			num,                                                \
			sizeof(type),                                       \
			0,													\
			0,													\
			0,                                                  \
			0,                                                  \
		};
#endif  /* RingBuffer_h */
