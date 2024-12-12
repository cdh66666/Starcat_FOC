#include "Protocol.h"
#include "Crc.h"
#include "RingBuffer.h"
#include "Uart1.h"
#include "string.h"

#include "SEGGER_RTT_Conf.h"
#include "SEGGER_RTT.h"
#include "stdio.h"

volatile uint32_t gProtocol_TimeCNT;


struct SProtocol_Struct {
    uint8_t head;
    uint8_t length;
    uint8_t data[1000];
    uint16_t crc;

};
struct SProtocol_Struct gProtocol;






void Func00(void)
{
	uint8_t data;
	printf("CD %s\r\n",__FUNCTION__);
	data = 0;
	DataPacking(&data,1);
}
void Func01(void)
{
	uint8_t data;
	printf("CD %s\r\n",__FUNCTION__);
	data = 1;
	DataPacking(&data,1);
}

struct SFuncList_Struct {
	uint8_t id;
	void (*Func)(void);
};


struct SFuncList_Struct gFuncList[] = {
	0,Func00,
	1,Func01,
};





/*************************************************************
** Function name:       DataPacking
** Descriptions:        数据打包
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void DataPacking(uint8_t *data,uint8_t length)
{
	uint8_t sendData[255];
	uint16_t crc;
	crc = IOT_CRC16_CCITT(data,length);
	
	sendData[0] = 0xAA;
	sendData[1] = length;
	memcpy(&sendData[2],data,length);
	memcpy(&sendData[2 + length],&crc,sizeof(uint16_t));
	
	Uart1_SendData(sendData,length + 4);
}
/*************************************************************
** Function name:       DataFiltering
** Descriptions:        数据筛选
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint16_t crc1;
uint16_t crc;
uint8_t DataFiltering(void)
{
    uint8_t head;
    uint8_t length;
	
    uint16_t dataNums;
    uint8_t data[255];
    //循环解析数据
    while(1) {
        //读取剩余数据
        dataNums = Uart1_GetDataNums();
        //判断剩余的数据够不够一帧
        if(dataNums < 4) {
            return 1;
        }
        //判断包头
        Uart1_ReadData(&head,1);
        if (head != 0xAA) {
            continue;
        }
        //判断数据长度是否足够
        Uart1_LookData(&length,1);

        dataNums = Uart1_GetDataNums();
        if (length + 3 > dataNums) {
            continue;
        }
        //读出固定长度的数据
        Uart1_LookData(data,length + 3);

        //判断CRC
        crc = *(uint16_t *)&data[length + 1];
		crc1 = IOT_CRC16_CCITT(&data[1],length);
        if (crc != crc1 ) {
            continue;
        }
        //装填数据
		gProtocol.head = head;
		gProtocol.length = length;
		memcpy(gProtocol.data,&data[1],length);
		gProtocol.crc = crc;
		return 0;
    }
}
/*************************************************************
** Function name:       DataProcessing
** Descriptions:        数据处理
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void DataProcessing(void)
{
	uint8_t i;
	for(i = 0; i < sizeof(gFuncList) / sizeof(struct SFuncList_Struct); i++) {
		if (gProtocol.data[0] == gFuncList[i].id) {
			gFuncList[i].Func();
		}
	}
}



void ProtocolExcel(void)
{
	static uint32_t statrtTime = 0;
	if(Protocol_TIMEOUT(0.01 * 1000,statrtTime)) {
		statrtTime = Protocol_GETTIME();
		
		uint8_t res;
		res = DataFiltering();
		if (res != 0) {
			//解析错误
		} else {
			DataProcessing();
		}

	}
    
	
	
}

