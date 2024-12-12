/***********************************************************************
 * @file CanBus.h
 * Description:
***********************************************************************/
#ifndef __CanBus_h__
#define __CanBus_h__


#include "can.h"

typedef struct
{
		uint32_t mailbox;
		CAN_TxHeaderTypeDef TxMessage;
		uint8_t payload[8];
}CANTxMsg_t;

typedef struct
{
		CAN_RxHeaderTypeDef RxMessage;
		uint8_t payload[8];
}CANRxMsg_t;

extern uint8_t rcvdFlag;
extern CANTxMsg_t TxMsg;
extern CANRxMsg_t RxMsg;

void CanBus_Init(void);
void CanBus_Loop(void);

#endif // __CanBus_h__
