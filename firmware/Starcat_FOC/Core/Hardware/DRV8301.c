#include "DRV8301.h"


/*
 * 爱你哟
 * 陈东豪爱胡斓馨
 * 嘻嘻！
*/



//NFAULT报错引脚中断
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == DRV8301_1.GPIO_Parameter->NFAULT_PIN)
	{
		DRV8301_1.FAULT_FLAG = ON;
	}
	else if(GPIO_Pin == DRV8301_2.GPIO_Parameter->NFAULT_PIN)
	{
		DRV8301_2.FAULT_FLAG = ON;
	}
}

//各种保护寄存器的参数
DRV8301_Protect_TypeDef Protect_Parameter_1;
DRV8301_Protect_TypeDef Protect_Parameter_2;

DRV8301_GPIO_TypeDef GPIO_Parameter_1 = {
		.NSCS_PORT = NSCS_1_GPIO_Port,
		.NSCS_PIN = NSCS_1_Pin,
		.SCLK_PORT = SPI_SCK_GPIO_Port,
		.SCLK_PIN = SPI_SCK_Pin,
		.SDI_PORT = SPI_MOSI_GPIO_Port,
		.SDI_PIN = SPI_MOSI_Pin,
		.SDO_PORT = SPI_MISO_GPIO_Port,
		.SDO_PIN = SPI_MISO_Pin,
		.EN_GATE_PORT = EN_GATE_GPIO_Port,
		.EN_GATE_PIN = EN_GATE_Pin,
		.NFAULT_PORT = NFAULT_1_GPIO_Port,
		.NFAULT_PIN = NFAULT_1_Pin
};
//初始化DRV8301状态
DRV8301_TypeDef DRV8301_1=
{
	.id = 1,
	.FAULT_FLAG= OFF,
	.GPIO_Parameter = &GPIO_Parameter_1,
	.Protect_Parameter = &Protect_Parameter_1,
	.EN_State = OFF,
	.I_Gain_mode=Gain_20,
	.I_Gain=20.0f,
	.PWM_Mode=PWM_6_MODE,
};

DRV8301_GPIO_TypeDef GPIO_Parameter_2 = {
		.NSCS_PORT = NSCS_2_GPIO_Port,
		.NSCS_PIN = NSCS_2_Pin,
		.SCLK_PORT = SPI_SCK_GPIO_Port,
		.SCLK_PIN = SPI_SCK_Pin,
		.SDI_PORT = SPI_MOSI_GPIO_Port,
		.SDI_PIN = SPI_MOSI_Pin,
		.SDO_PORT = SPI_MISO_GPIO_Port,
		.SDO_PIN = SPI_MISO_Pin,
		.EN_GATE_PORT = EN_GATE_GPIO_Port,
		.EN_GATE_PIN = EN_GATE_Pin,
		.NFAULT_PORT = NFAULT_2_GPIO_Port,
		.NFAULT_PIN = NFAULT_2_Pin
};

//初始化DRV8301状态
DRV8301_TypeDef DRV8301_2=
{
	.id = 2,
	.FAULT_FLAG= OFF,
	.GPIO_Parameter = &GPIO_Parameter_2,
	.Protect_Parameter = &Protect_Parameter_2,
	.EN_State = OFF,
	.I_Gain_mode=Gain_20,
	.I_Gain=20.0f,
	.PWM_Mode=PWM_6_MODE,

};

/*
SPI 输入(SDI)数据格式由一个 16 位的字、1 个读写位、4 个地址位和 11 个数据位组成。
*/


//芯片初始化
void DRV8301_Init(DRV8301_TypeDef *DRV8301)
{


	//根据初始参数，初始DRV8301使能
	DRV8301_EN(DRV8301,ON);

	HAL_Delay(500);

	//清零控制寄存器
	DRV8301_SPI_ReadWrite(DRV8301,Control_Register_1,0,WRITE);
	DRV8301_SPI_ReadWrite(DRV8301,Control_Register_2,0,WRITE);

	/*
	 * 使用默认设置：
	 * GATE_CURRENT:Gate drive peak current 1.7 A
	 * GATE_RESET:Normal mode
	 * PWM_MODE:6 PWM input
	 * OCP_MODE:Current limit
	 * OC_ADJ_SET:Vds<0.06V -- 因为使用的NMOS导通电阻7mΩ,所以当NMOS中流过电流超过22A才会触发过流保护
	 * */

	//设置3pwm模式/6pwm模式
	DRV8301_PWM_Mode(DRV8301,DRV8301->PWM_Mode);
	DRV8301_Set_OC(DRV8301,Vds_0_109);
	DRV8301_Set_Gain(DRV8301,DRV8301->I_Gain_mode);
	DRV8301_GATE_CURRENT(DRV8301,peak_current_1A7);


#ifdef DEBUG_MODE
	uint16_t data1=0,data2=0,data3=0,data4=0;
	printf("Set 6PWM Mode.\n");
		//设置3pwm模式/6pwm模式
		DRV8301_PWM_Mode(DRV8301,DRV8301->PWM_Mode);
		printf("Control_Register_1:\n");
		data3=DRV8301_SPI_ReadWrite(DRV8301,Control_Register_1,0,READ);
		printBinary(data3);
		printf("\n");
		printf("Control_Register_2:\n");
		data4=DRV8301_SPI_ReadWrite(DRV8301,Control_Register_2,0,READ);
		printBinary(data4);
		printf("\n\n");

		printf("Set OC Vds.\n");
		DRV8301_Set_OC(DRV8301,Vds_0_109);

		printf("Control_Register_1:\n");
		data3=DRV8301_SPI_ReadWrite(DRV8301,Control_Register_1,0,READ);
		printBinary(data3);
		printf("\n");
		printf("Control_Register_2:\n");
		data4=DRV8301_SPI_ReadWrite(DRV8301,Control_Register_2,0,READ);
		printBinary(data4);
		printf("\n\n");
		/*
		 * 使用默认设置：
		 * OCTW_MODE:Report both OT and OC at nOCTW pin
		 * GAIN:Gain of shunt amplifier: 10 V/V
		 * DC_CAL_CH1:Shunt amplifier 1 connects to load through input pins
		 * DC_CAL_CH2:Shunt amplifier 2 connects to load through input pins
		 * OC_TOFF:Cycle by cycle
		 * Reserved:--
		 */
		printf("Set Gain is 80V/V.\n");
		//设置采样电流增益10/20/40/80
		DRV8301_Set_Gain(DRV8301,DRV8301->I_Gain_mode);
		printf("Control_Register_1:\n");
		data3=DRV8301_SPI_ReadWrite(DRV8301,Control_Register_1,0,READ);
		printBinary(data3);
		printf("\n");
		printf("Control_Register_2:\n");
		data4=DRV8301_SPI_ReadWrite(DRV8301,Control_Register_2,0,READ);
		printBinary(data4);
		printf("\n\n");
#endif

}


//检查是否出错
int DRV8301_Check_Fault(DRV8301_TypeDef *DRV8301)
{
	if(DRV8301->FAULT_FLAG==ON)
	{
		DRV8301->FAULT_FLAG=OFF;
		 return DRV8301_Read_Protect(DRV8301);
	}
	return 0;

}

//读DRV8301里的保护参数,无异常返回1
int DRV8301_Read_Protect(DRV8301_TypeDef *DRV8301 )
{
	int flag=1;

	uint16_t data1=DRV8301_SPI_ReadWrite(DRV8301,Status_Register_1,0,READ) & 0x7ff;
	uint16_t data2=DRV8301_SPI_ReadWrite(DRV8301,Status_Register_2,0,READ) & 0x7ff;

	DRV8301->Protect_Parameter->Device_ID =  data2 & (0x000f) ;
	DRV8301->Protect_Parameter->GVDD_OV = ((data2 & (0x0080))>0)?1:0;

	DRV8301->Protect_Parameter->FAULT = ((data1 & (0x0400))>0)?1:0;
	DRV8301->Protect_Parameter->GVDD_UV =  ((data1 & (0x0200))>0)?1:0;
	DRV8301->Protect_Parameter->PVDD_UV =  ((data1 & (0x0100))>0)?1:0;
	DRV8301->Protect_Parameter->OTSD =  ((data1 & (0x0080))>0)?1:0;
	DRV8301->Protect_Parameter->OTW =  ((data1 & (0x0040))>0)?1:0;
	DRV8301->Protect_Parameter->FETHA_OC =  ((data1 & (0x0020))>0)?1:0;
	DRV8301->Protect_Parameter->FETLA_OC =  ((data1 & (0x0010))>0)?1:0;
	DRV8301->Protect_Parameter->FETHB_OC =  ((data1 & (0x0008))>0)?1:0;
	DRV8301->Protect_Parameter->FETLB_OC =  ((data1 & (0x0004))>0)?1:0;
	DRV8301->Protect_Parameter->FETHC_OC =  ((data1 & (0x0002))>0)?1:0;
	DRV8301->Protect_Parameter->FETLC_OC =  ((data1 & (0x0001))>0)?1:0;

	//读出来设备id=0，说明未通信成功
	if(DRV8301->Protect_Parameter->Device_ID==0)
	{
	    printf("DRV8301_%1d:DRV8301 SPI Can‘t Work!\n",DRV8301->id);
	    flag = 0;
	}
	if(DRV8301->Protect_Parameter->Device_ID>1)
	{
	    printf("DRV8301_%1d:Read Device id = %1d!\n",DRV8301->id,DRV8301->Protect_Parameter->Device_ID);
	    flag = 0;
	}
	// 判断 FAULT
	if (DRV8301->Protect_Parameter->FAULT!= 0)
	{
	    printf("DRV8301_%1d:There is a fault state.\n",DRV8301->id);
	    flag = 0;
	}

	// 判断 GVDD_UV
	if (DRV8301->Protect_Parameter->GVDD_UV!= 0)
	{
	    printf("DRV8301_%1d:Gate drive under - voltage <8V.\n",DRV8301->id);
	    flag = 0;
	}

	// 判断 PVDD_UV
	if (DRV8301->Protect_Parameter->PVDD_UV!= 0)
	{
	    printf("DRV8301_%1d:Power supply under - voltage <6V.\n",DRV8301->id);
	    flag = 0;
	}

	// 判断 OTSD
	if (DRV8301->Protect_Parameter->OTSD!= 0)
	{
	    printf("DRV8301_%1d:Over - temperature lockout driver flag set.\n",DRV8301->id);
	    flag = 0;
	}

	// 判断 OTW
	if (DRV8301->Protect_Parameter->OTW!= 0)
	{
	    printf("DRV8301_%1d:Over - temperature warning >150°.\n",DRV8301->id);
	    flag = 0;
	}

	// 判断 FETHA_OC
	if (DRV8301->Protect_Parameter->FETHA_OC!= 0)
	{
	    printf("DRV8301_%1d:MOSFET A_H over - current.\n",DRV8301->id);
	    flag = 0;
	}

	// 判断 FETLA_OC
	if (DRV8301->Protect_Parameter->FETLA_OC!= 0)
	{
	    printf("DRV8301_%1d:MOSFET A_L over - current.\n",DRV8301->id);
	    flag = 0;
	}

	// 判断 FETHB_OC
	if (DRV8301->Protect_Parameter->FETHB_OC!= 0)
	{
	    printf("DRV8301_%1d:MOSFET B_H over - current.\n",DRV8301->id);
	    flag = 0;
	}

	// 判断 FETLB_OC
	if (DRV8301->Protect_Parameter->FETLB_OC!= 0)
	{
	    printf("DRV8301_%1d:MOSFET B_L over - current.\n",DRV8301->id);
	    flag = 0;
	}

	// 判断 FETHC_OC
	if (DRV8301->Protect_Parameter->FETHC_OC!= 0)
	{
	    printf("DRV8301_%1d:MOSFET C_H over - current.\n",DRV8301->id);
	    flag = 0;
	}

	// 判断 FETLC_OC
	if (DRV8301->Protect_Parameter->FETLC_OC!= 0)
	{
	    printf("DRV8301_%1d:MOSFET C_L over - current.\n",DRV8301->id);
	    flag = 0;
	}

	// 判断 GVDD_OV
	if (DRV8301->Protect_Parameter->GVDD_OV!= 0)
	{
	    printf("DRV8301_%1d:Gate drive over - voltage >16V.\n",DRV8301->id);
	    flag = 0;
	}

	if(flag==1)
	{
		printf("DRV8301_%1d is OK！\n",DRV8301->id);
	}
 return flag;

}




// SPI 读写指定寄存器的值
uint16_t DRV8301_SPI_ReadWrite(DRV8301_TypeDef *DRV8301, uint8_t addr, uint16_t data, OperationType operation) {
    uint16_t sendData = 0;


    if (operation == READ) {
        sendData |= 0x8000;
		#ifdef DEBUG_MODE
				printf("Read:");
		#endif

    }
    else
    {
		#ifdef DEBUG_MODE
				printf("Write:");
		#endif

    }

    sendData |= ((addr & 0xF) << 11);
    sendData |= (data & 0x7FF);

	#ifdef DEBUG_MODE
		printf("Addr ");
		printBinary_8(addr);
		printf("\n");

		printf("data will be send:");
		printBinary(sendData);
		printf("\n");
	#endif



    //拉低sck
    HAL_GPIO_WritePin(DRV8301->GPIO_Parameter->SCLK_PORT, DRV8301->GPIO_Parameter->SCLK_PIN, GPIO_PIN_RESET);
    // 发送命令，大端先行
    HAL_GPIO_WritePin(DRV8301->GPIO_Parameter->NSCS_PORT, DRV8301->GPIO_Parameter->NSCS_PIN, GPIO_PIN_RESET);
    for (int8_t i = 15; i >= 0; i--) {
        HAL_GPIO_WritePin(DRV8301->GPIO_Parameter->SCLK_PORT, DRV8301->GPIO_Parameter->SCLK_PIN, GPIO_PIN_SET);
        // 发送数据
        if (sendData & (0x0001 << i)) {
            HAL_GPIO_WritePin(DRV8301->GPIO_Parameter->SDI_PORT, DRV8301->GPIO_Parameter->SDI_PIN, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(DRV8301->GPIO_Parameter->SDI_PORT, DRV8301->GPIO_Parameter->SDI_PIN, GPIO_PIN_RESET);
        }
        HAL_GPIO_WritePin(DRV8301->GPIO_Parameter->SCLK_PORT, DRV8301->GPIO_Parameter->SCLK_PIN, GPIO_PIN_RESET);
    }
    HAL_GPIO_WritePin(DRV8301->GPIO_Parameter->NSCS_PORT, DRV8301->GPIO_Parameter->NSCS_PIN, GPIO_PIN_SET);

    //CS由高到低
   HAL_GPIO_WritePin(DRV8301->GPIO_Parameter->NSCS_PORT, DRV8301->GPIO_Parameter->NSCS_PIN, GPIO_PIN_RESET);
    // 接收数据，大端在前
    sendData = 0;
    for (int8_t i = 15; i >= 0; i--) {
        HAL_GPIO_WritePin(DRV8301->GPIO_Parameter->SCLK_PORT, DRV8301->GPIO_Parameter->SCLK_PIN, GPIO_PIN_SET);
        // 接收数据
        if (HAL_GPIO_ReadPin(DRV8301->GPIO_Parameter->SDO_PORT, DRV8301->GPIO_Parameter->SDO_PIN) == GPIO_PIN_SET) {
            sendData |= (0x0001 << i);
        }
        HAL_GPIO_WritePin(DRV8301->GPIO_Parameter->SCLK_PORT, DRV8301->GPIO_Parameter->SCLK_PIN, GPIO_PIN_RESET);
    }
    HAL_GPIO_WritePin(DRV8301->GPIO_Parameter->NSCS_PORT, DRV8301->GPIO_Parameter->NSCS_PIN, GPIO_PIN_SET);

    //如果错误位置1，则发送出错
    if((sendData & (0x0001<<15))!=0)
    {
    	printf("RxData Frame error! \n");
    }
    //如果是读命令，则接收的地址与读地址一致
    else if((((sendData&0x7800)>>11) != ((uint16_t)addr)) && (operation == READ))
    {
    	printf("Read:RxData Addr error! \n");
    }
    //如果是写命令，则接收的地址是0x00
    else if((((sendData&0x7800)>>11) != 0) && (operation == WRITE))
    {
    	printf("Write:RxData Addr error! \n");
    }
    //读命令会返回状态寄存器1的值
    else if(operation == WRITE)
    {
    	DRV8301->Protect_Parameter->FAULT = ((sendData & (0x0400))>0)?1:0;
    	DRV8301->Protect_Parameter->GVDD_UV =  ((sendData & (0x0200))>0)?1:0;
    	DRV8301->Protect_Parameter->PVDD_UV =  ((sendData & (0x0100))>0)?1:0;
    	DRV8301->Protect_Parameter->OTSD =  ((sendData & (0x0080))>0)?1:0;
    	DRV8301->Protect_Parameter->OTW =  ((sendData & (0x0040))>0)?1:0;
    	DRV8301->Protect_Parameter->FETHA_OC =  ((sendData & (0x0020))>0)?1:0;
    	DRV8301->Protect_Parameter->FETLA_OC =  ((sendData & (0x0010))>0)?1:0;
    	DRV8301->Protect_Parameter->FETHB_OC =  ((sendData & (0x0008))>0)?1:0;
    	DRV8301->Protect_Parameter->FETLB_OC =  ((sendData & (0x0004))>0)?1:0;
    	DRV8301->Protect_Parameter->FETHC_OC =  ((sendData & (0x0002))>0)?1:0;
    	DRV8301->Protect_Parameter->FETLC_OC =  ((sendData & (0x0001))>0)?1:0;

    }


	#ifdef DEBUG_MODE
		printf("RxData:");
		printBinary(sendData);
		printf("\n");
	#endif

    return sendData;
}


//电机使能/失能
void DRV8301_EN(DRV8301_TypeDef *DRV8301,OperationType2 ON_OFF)
{
	if(ON_OFF == ON)
	{
		DRV8301->EN_State = ON;
		HAL_GPIO_WritePin(DRV8301->GPIO_Parameter->EN_GATE_PORT , DRV8301->GPIO_Parameter->EN_GATE_PIN, GPIO_PIN_SET);
	}
	else
	{
		DRV8301->EN_State = OFF;
		HAL_GPIO_WritePin(DRV8301->GPIO_Parameter->EN_GATE_PORT , DRV8301->GPIO_Parameter->EN_GATE_PIN, GPIO_PIN_RESET);
	}
}

//清除电机错误状态
void DRV8301_GATE_RESET(DRV8301_TypeDef *DRV8301 )
{
	uint16_t data=DRV8301_SPI_ReadWrite(DRV8301,Control_Register_1,0,READ) & 0x7ff;

	data |= (0x01<<GATE_RESET_OFFSET);//开启复位，清除错误状态

	DRV8301_SPI_ReadWrite(DRV8301,Control_Register_1,data,WRITE);

	data &= (~(0x01<<GATE_RESET_OFFSET));//关闭复位，继续运行

	DRV8301_SPI_ReadWrite(DRV8301,Control_Register_1,data,WRITE);

}


//开启电流检测通道校准
void DRV8301_DC_CAL(DRV8301_TypeDef *DRV8301,OperationType2 ON_OFF)
{
	uint16_t data=DRV8301_SPI_ReadWrite(DRV8301,Control_Register_2,0,READ) & 0x7ff;
	if(ON_OFF == ON)
	{
		data |= (0x01<<DC_CAL_CH1_OFFSET);//校准通道1
		data |= (0x01<<DC_CAL_CH2_OFFSET);//校准通道2
	}
	else {

		data &= (~(0x01<<DC_CAL_CH1_OFFSET));//校准通道1
		data &= (~(0x01<<DC_CAL_CH2_OFFSET));//校准通道2
	}
	DRV8301_SPI_ReadWrite(DRV8301,Control_Register_2,data,WRITE);
}


//设置NMOS过流保护值为0.109V，因为使用的NMOS导通电阻7mΩ,所以当NMOS中流过电流超过15.5A才会触发过流保护
void DRV8301_Set_OC(DRV8301_TypeDef *DRV8301,OC_Vds_Type Vds)
{
	uint16_t data=DRV8301_SPI_ReadWrite(DRV8301,Control_Register_1,0,READ) & 0x7ff;

	data |= ((uint16_t)Vds<<OC_ADJ_SET_OFFSET);//设置NMOS过流保护值

	DRV8301_SPI_ReadWrite(DRV8301,Control_Register_1,data,WRITE);

}

//设置电流增益值
void DRV8301_Set_Gain(DRV8301_TypeDef *DRV8301,Gain_Type Gain)
{
	uint16_t data=DRV8301_SPI_ReadWrite(DRV8301,Control_Register_2,0,READ) & 0x7ff;



	data |= (((uint16_t)Gain)<<GAIN_OFFSET);//设置增益

	DRV8301_SPI_ReadWrite(DRV8301,Control_Register_2,data,WRITE);


	DRV8301->I_Gain_mode = Gain;
	if(DRV8301->I_Gain_mode == Gain_10)
	{
		DRV8301->I_Gain = 10.0f;
	}
	else if(DRV8301->I_Gain_mode == Gain_20)
	{
		DRV8301->I_Gain = 20.0f;
	}
	else if(DRV8301->I_Gain_mode == Gain_40)
	{
		DRV8301->I_Gain = 40.0f;
	}
	else if(DRV8301->I_Gain_mode == Gain_80)
	{
		DRV8301->I_Gain = 80.0f;
	}
}



//设置栅极驱动电流，不能太大
void DRV8301_GATE_CURRENT(DRV8301_TypeDef *DRV8301,Peak_Current_Type current)
{
	uint16_t data=DRV8301_SPI_ReadWrite(DRV8301,Control_Register_1,0,READ);

	if(current == peak_current_1A7)
	{
		data &= (~(peak_current_1A7<<GATE_CURRENT_OFFSET));//1.7A
	}
	else if(current == peak_current_0A7){
		data |= (peak_current_0A7<<GATE_CURRENT_OFFSET);//0x7A
	}
	else {
		data |= (peak_current_0A25<<GATE_CURRENT_OFFSET);//0x25A
	}
	DRV8301_SPI_ReadWrite(DRV8301,Control_Register_1,data,WRITE);

}




//3PWM mode / 6PWM mode切换
void DRV8301_PWM_Mode(DRV8301_TypeDef *DRV8301,Pwm_Mode_Type mode)
{
	uint16_t data=DRV8301_SPI_ReadWrite(DRV8301,Control_Register_1,0,READ);

	if(mode == PWM_6_MODE)
	{
		data &= (~(0x01<<PWM_MODE_OFFSET));//3pwm mode
	}
	else {
		data |= (0x01<<PWM_MODE_OFFSET);//3pwm mode
	}

	DRV8301_SPI_ReadWrite(DRV8301,Control_Register_1,data,WRITE);

	DRV8301->PWM_Mode = mode;
}
