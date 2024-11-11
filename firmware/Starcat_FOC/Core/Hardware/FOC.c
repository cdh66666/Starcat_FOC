#include <FOC.h>

//用于计算FOC运行时长
TimerStruct timer_foc={0,0,&htim5};

//us计时,一个定时器只能有一个TimerStruct结构体，不然会混乱
static void TIM_US_COUNT(TimerStruct *timer)
{
    if(timer->flag == 0)
    {
        // 开始计时
        __HAL_TIM_SetCounter(timer->htim_us, 0);
        __HAL_TIM_ENABLE(timer->htim_us);
        timer->flag = 1;
    }
    else
    {
        timer->flag = 0;
        // 停止计时
        __HAL_TIM_DISABLE(timer->htim_us);
        timer->time = __HAL_TIM_GetCounter(timer->htim_us); // 计时完毕,最大只能计到 65ms
    }
}



extern float KP,KI,KD;
extern float uq,ud;
extern float vel;
extern float pos;

uint8_t adc_flag=0;  //常规adc通道采样标志位
float VBUS=8.4f;		// 直流母线电压
float TEMP1=0;	// NMOS温度
float TEMP2=0;	// NMOS温度

//电源电压&温度采样数组
uint16_t ADC2_Value_buffer[3];


Motor_Parameters_Typedef parameters1 =
{
	.zero_electric_angle = 0.0f,	//零电角度
	.DIR = 1.0f,			//旋转方向
	.Pole_pair = 7.0f,	//磁极对数

	.Rs = 0.0f, // 定子电阻
	.Ls = 0.0f, // 定子电感
	.Ld = 0.0f, // 直轴电感
	.Lq = 0.0f, // 交轴电感
	.Gr = 0.0f, // 转子磁链
	.Ke = 0.0f, // 反电动势系数
	.J = 0.0f,  // 转动惯量
	.u = 0.0f,  // 摩擦系数
};
Motor_Parameters_Typedef parameters2 =
{
	.zero_electric_angle = 0.0f,	//零电角度
	.DIR = 1.0f,			//旋转方向
	.Pole_pair = 7.0f,	//磁极对数

	.Rs = 0.0f, // 定子电阻
	.Ls = 0.0f, // 定子电感
	.Ld = 0.0f, // 直轴电感
	.Lq = 0.0f, // 交轴电感
	.Gr = 0.0f, // 转子磁链
	.Ke = 0.0f, // 反电动势系数
	.J = 0.0f,  // 转动惯量
	.u = 0.0f,  // 摩擦系数
};
//电流环pid参数
PIDControllerFloat Iq_pid1 =
{
		.kp = 0.1f,
		.ki = 0.02f,
		.kd = 0.0f,
		.integral = 0.0f,
		.prevError = 0.0f,
		.integralLimitMin = 0.0f,
		.integralLimitMax = 0.0f,
};
//电流环pid参数
PIDControllerFloat Id_pid1 =
{
		.kp = 0.1f,
		.ki = 0.02f,
		.kd = 0.0f,
		.integral = 0.0f,
		.prevError = 0.0f,
		.integralLimitMin = 0.0f,
		.integralLimitMax = 0.0f,
};
//速度环pid参数
PIDControllerFloat V_pid1=
{
		.kp = 0.0f,
		.ki = 0.0f,
		.kd = 0.0f,
		.integral = 0.0f,
		.prevError = 0.0f,
		.integralLimitMin = 0.0f,
		.integralLimitMax = 0.0f,
};
//位置环pid参数
PIDControllerFloat P_pid1 =
{
		.kp = 0.0f,
		.ki = 0.0f,
		.kd = 0.0f,
		.integral = 0.0f,
		.prevError = 0.0f,
		.integralLimitMin = 0.0f,
		.integralLimitMax = 0.0f,
};

//电流环pid参数
PIDControllerFloat Iq_pid2 =
{
		.kp = 0.1f,
		.ki = 0.02f,
		.kd = 0.0f,
		.integral = 0.0f,
		.prevError = 0.0f,
		.integralLimitMin = 0.0f,
		.integralLimitMax = 0.0f,
};
//电流环pid参数
PIDControllerFloat Id_pid2 =
{
		.kp = 0.1f,
		.ki = 0.02f,
		.kd = 0.0f,
		.integral = 0.0f,
		.prevError = 0.0f,
		.integralLimitMin = 0.0f,
		.integralLimitMax = 0.0f,
};
//速度环pid参数
PIDControllerFloat V_pid2=
{
		.kp = 0.0f,
		.ki = 0.0f,
		.kd = 0.0f,
		.integral = 0.0f,
		.prevError = 0.0f,
		.integralLimitMin = -2.0f,
		.integralLimitMax = 2.0f,
};
//位置环pid参数
PIDControllerFloat P_pid2 =
{
		.kp = 0.0f,
		.ki = 0.0f,
		.kd = 0.0f,
		.integral = 0.0f,
		.prevError = 0.0f,
		.integralLimitMin = 0.0f,
		.integralLimitMax = 0.0f,
};
// 假设相关结构体和枚举等定义
Motor_Run_Typedef motor1 =
{

   .PWM_Period = 0,

   .htim = &htim1,
   .DRV8301 = &DRV8301_1,
   .Parameters = &parameters1,
   .encoder = &encoder1,
   .hadc = &hadc1,

   .interruptFrequency = 1.0f,
   .run_flag=0,
   .I_A = 0.0f,
   .I_B = 0.0f,
   .I_C = 0.0f,
   .I_A_Offset =  0.0f,
   .I_B_Offset =  0.0f,
   .electricalAngle=0.0f,
   .speed = 0.0f,
   .prev_speed = 0.0f,
   .Iq_pid = &Iq_pid1,
   .Id_pid = &Id_pid1,
   .V_pid = &V_pid1,
   .P_pid = &P_pid1,
   .except_vel=0.0f,
   .except_iq=0.0f,
   .except_id=0.0f,
   .count=0,
};

Motor_Run_Typedef motor2=
{

   .PWM_Period = 0,

   .htim = &htim8,
   .DRV8301 = &DRV8301_2,
   .Parameters = &parameters2,
   .encoder = &encoder2,
   .hadc = &hadc3,
   .run_flag=0,
   .interruptFrequency = 1.0f,

   .I_A = 0.0f,
   .I_B = 0.0f,
   .I_C = 0.0f,
   .I_A_Offset =  0.0f,
   .I_B_Offset =  0.0f,
   .electricalAngle=0.0f,
   .speed = 0.0f,
   .prev_speed = 0.0f,
   .Iq_pid = &Iq_pid2,
   .Id_pid = &Id_pid2,
   .V_pid = &V_pid2,
   .P_pid = &P_pid2,
   .except_vel=0.0f,
   .except_iq=0.0f,
   .except_id=0.0f,
   .count=0,
};



float TIME=0;
int FPS=0,fps=0;
int TIME_FPS=0;
int time_fps=0;
void HAL_IncTick(void)
{
  uwTick += uwTickFreq;
  if(uwTick%1000 == 0)
  {
	  FPS=fps;
	  fps=0;

  }
  //每100ms采样一次电源电压和双路温度
  if(uwTick%10 == 0)
  {
	  adc_flag=1;
//	  TIME+=1;
  }
//  TIME+=0.001;	//ms计时
}


//注入通道adc中断
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance==ADC1)
        {
//        HAL_GPIO_TogglePin(SERVO0_GPIO_Port, SERVO0_Pin);

//        TIM_US_COUNT(&timer_foc);
        //进行三环运算，执行svpwm
        FOC_Run(&motor1);
//        TIM_US_COUNT(&timer_foc);

//        HAL_GPIO_TogglePin(SERVO0_GPIO_Port, SERVO0_Pin);
        }
    if(hadc->Instance==ADC3)
        {
//    	HAL_GPIO_TogglePin(SERVO1_GPIO_Port, SERVO1_Pin);

    	TIM_US_COUNT(&timer_foc);
        //进行三环运算，执行svpwm
        FOC_Run(&motor2);
        TIM_US_COUNT(&timer_foc);

//        HAL_GPIO_TogglePin(SERVO1_GPIO_Port, SERVO1_Pin);
        }
}



//电流采样偏移值初始化，开启电源电压和温度采样
void FOC_Current_Init(Motor_Run_Typedef *motor)
{
    float ia = 0, ib = 0;
    int count = 100;
    int scale = 100; // 定义放大系数，用于消除小数点

    /****************************进行A,B两相电流检测通道校准***************************/
//    DRV8301_DC_CAL(motor->DRV8301, ON);
    // 采样count次取平均值
    for (int i = 0; i < count; i++)
    {
        HAL_ADC_Start(motor->hadc);
        while (HAL_ADC_PollForConversion(motor->hadc, 50));
        ia += (float)HAL_ADC_GetValue(motor->hadc);
        HAL_ADC_Start(motor->hadc);
        while (HAL_ADC_PollForConversion(motor->hadc, 50));
        ib += (float)HAL_ADC_GetValue(motor->hadc);
    }

    // 计算平均值并转换为整数形式的浮点数
    motor->I_A_Offset = (int)(ia / count * scale + 0.5f) / scale; // 加0.5用于四舍五入
    motor->I_B_Offset = (int)(ib / count * scale + 0.5f) / scale; // 加0.5用于四舍五入
//    DRV8301_DC_CAL(motor->DRV8301, OFF);

}

//FOC运行初始化
void FOC_Init(Motor_Run_Typedef *motor)
{
	float data[MAX_DATA_COUNT];

	//初始化参数数组
	for(int i=0;i<MAX_DATA_COUNT;i++)
	{
		data[i]=0;
	}


	//电源电压采样初始化
	FOC_Regular_ADC_INIT();
	//动态积分限幅
	motor->Iq_pid->integralLimitMin = -VBUS*_1_sqrt3;
	motor->Iq_pid->integralLimitMax = VBUS*_1_sqrt3;
	motor->Id_pid->integralLimitMin = -VBUS*_1_sqrt3;
	motor->Id_pid->integralLimitMax = VBUS*_1_sqrt3;
	//计算定时器中断频率
	calculateInterruptFrequency(motor);
	//使能电机驱动芯片
	FOC_EN(motor, ON);
	//电流采样初始化偏置
	FOC_Current_Init(motor);

	printf("*********************motor%1d************************\n",motor->DRV8301->id);

	//读寄存器参数,无异常则继续
	if(DRV8301_Read_Protect(motor->DRV8301)==1)
	{

		//PWM通道初始化
		if(motor->DRV8301->PWM_Mode == PWM_6_MODE)
		{
			HAL_TIM_PWM_Start(motor->htim,TIM_CHANNEL_1);
			HAL_TIM_PWM_Start(motor->htim,TIM_CHANNEL_2);
			HAL_TIM_PWM_Start(motor->htim,TIM_CHANNEL_3);
			HAL_TIMEx_PWMN_Start(motor->htim, TIM_CHANNEL_1);
			HAL_TIMEx_PWMN_Start(motor->htim, TIM_CHANNEL_2);
			HAL_TIMEx_PWMN_Start(motor->htim, TIM_CHANNEL_3);
		}
		else
		{
			HAL_TIM_PWM_Start(motor->htim,TIM_CHANNEL_1);
			HAL_TIM_PWM_Start(motor->htim,TIM_CHANNEL_2);
			HAL_TIM_PWM_Start(motor->htim,TIM_CHANNEL_3);
		}

		//获取ARR值
		motor->PWM_Period = getTimerARR(motor->htim);

		printf("PWM_Period:%7d\n",motor->PWM_Period);


		//比较值初始化为0
		__HAL_TIM_SET_COMPARE(motor->htim, TIM_CHANNEL_1,0);
		__HAL_TIM_SET_COMPARE(motor->htim, TIM_CHANNEL_2,0);
		__HAL_TIM_SET_COMPARE(motor->htim, TIM_CHANNEL_3,0);

		//首次运行会执行电机参数辨识操作
		if (isDataStoredInFlash(motor->DRV8301->id))
		{
			readMultipleDataFromFlash(data, MAX_DATA_COUNT,motor->DRV8301->id);
			motor->Parameters->zero_electric_angle =  data[1];
			motor->Parameters->DIR =  data[2];
			motor->Parameters->Pole_pair =  data[3];
		}
		else	//还没存储参数
		{
			//进行参数校准,成功校准参数则返回1，并写入参数
			if(Motor_Parameter_Identification(motor,data)==1)
			{
				writeMultipleDataToFlash(data, MAX_DATA_COUNT,motor->DRV8301->id);
				motor->Parameters->zero_electric_angle =  data[1];
				motor->Parameters->DIR =  data[2];
				motor->Parameters->Pole_pair =  data[3];
			}

		}
		//定时器通道4用来触发adc采样
		__HAL_TIM_SET_COMPARE(motor->htim, TIM_CHANNEL_4,motor->PWM_Period-1);
		HAL_TIM_PWM_Start(motor->htim,TIM_CHANNEL_4);  // 启动定时器

		//开启双路电流采样
		HAL_TIM_Base_Start_IT(motor->htim);
		HAL_ADCEx_InjectedStart_IT(motor->hadc);
	}
	printf("zero_electric_angle:%7.3f\n",data[1]);
	printf("DIR:%7.3f\n",data[2]);
	printf("Pole_pair:%7.3f\n",data[3]);
	printf("*************************************************\n");

}


//常规采样初始化
void FOC_Regular_ADC_INIT(void)
{
	static int flag=1;
	if(flag==1)
	{
		flag=0;
		// 开启电源电压和双路温度采样
		HAL_ADC_Start_DMA(&hadc2, (uint32_t *)ADC2_Value_buffer, 3);
		HAL_Delay(1);
		VBUS = (float)ADC2_Value_buffer[0] / 4095.0f * 3.3f * 41.2f / 2.2f;
		float filtered_temp1Value = (float)ADC2_Value_buffer[1] / 4095.0f * 3.3f;
		TEMP1 = getTemperatureFromTable(3.3f * 3300.0f / filtered_temp1Value - 3300.0f);
		float filtered_temp2Value = (float)ADC2_Value_buffer[2] / 4095.0f * 3.3f;
		TEMP2 = getTemperatureFromTable(3.3f * 3300.0f / filtered_temp2Value - 3300.0f);
		printf("VBUS:%7.2f\n",VBUS);
		printf("TEMP1:%7.2f\n",TEMP1);
		printf("TEMP2:%7.2f\n",TEMP1);
	}

}


// FOC - adc 常规通道采样
void FOC_Regular_ADC(void)
{

    static float filtered_temp1Value = 0.0f;
    static float filtered_temp2Value = 0.0f;
    static int count = 0; // 计数进入函数的次数
    static int filter_count = 500; // 滤波次数，可根据需要更改
    static float sum_VBUS = 0.0f; // 存储电压总和
    static float sum_temp1Value = 0.0f; // 存储第一个温度总和
    static float sum_temp2Value = 0.0f; // 存储第二个温度总和

    /***************************电源电压&双路温度采样**************************/

    if (adc_flag == 1)
    {
        // 对电源电压采样值进行均值滤波
        sum_VBUS += (float)ADC2_Value_buffer[0] / 4095.0f * 3.3f * 41.2f / 2.2f;
        if (count >= filter_count)
        {
            VBUS = sum_VBUS / filter_count;
            sum_VBUS = 0.0f;
            count = 0;
        }

        // 对第一个温度采样值进行均值滤波
        sum_temp1Value += (float)ADC2_Value_buffer[1] / 4095.0f * 3.3f;
        if (count >= filter_count)
        {
            filtered_temp1Value = sum_temp1Value / filter_count;
            sum_temp1Value = 0.0f;
            TEMP1 = getTemperatureFromTable(3.3f * 3300.0f / filtered_temp1Value - 3300.0f);
        }

        // 对第二个温度采样值进行均值滤波
        sum_temp2Value += (float)ADC2_Value_buffer[2] / 4095.0f * 3.3f;
        if (count >= filter_count)
        {
            filtered_temp2Value = sum_temp2Value / filter_count;
            sum_temp2Value = 0.0f;
            TEMP2 = getTemperatureFromTable(3.3f * 3300.0f / filtered_temp2Value - 3300.0f);
        }


        count++;
        adc_flag = 0;
    }
}
//获取电源电压
float FOC_Get_VBUS(void)
{
	return VBUS;
}

//获取温度1
float FOC_Get_TEMP1(void)
{
	return TEMP1;
}

//获取温度2
float FOC_Get_TEMP2(void)
{
	return TEMP2;
}

//简单运行foc算法/20khz
void FOC_Run(Motor_Run_Typedef *motor)
{

	//只有开启foc算法才执行
	if(motor->run_flag==1)
	{
		//如果没有报错才进行下一步
		if(DRV8301_Check_Fault(motor->DRV8301)==0)
		{
		  // 求电流值
			FOC_Get_Ia_Ib_Ic(motor);
			//将三相电流转换成Id,Iq
			FOC_clarke_park_Transform(motor);
			//获取编码器位置
			FOC_Get_electricalAngle_speed(motor);
//			FOC_CLOSED_Pos(&motor2,pos);
			motor->except_iq = FOC_CLOSED_Vel(motor,vel);
			motor->except_id = 0;
			//执行力矩控
			FOC_CLOSED_Iq(motor,motor->except_iq,motor->except_id);







//			FOC_CLOSED_Iq(motor,uq,ud);
		}
	}


}

float pidUpdateFloat(PIDControllerFloat* pid, float expectedValue, float actualValue) {
    float error = expectedValue - actualValue;
    float pTerm = pid->kp * error;

    pid->integral += error;
    // 积分限幅
    if (pid->integral > pid->integralLimitMax) {
        pid->integral = pid->integralLimitMax;
    } else if (pid->integral < pid->integralLimitMin) {
        pid->integral = pid->integralLimitMin;
    }

    float iTerm = pid->ki * pid->integral;
    float dTerm = pid->kd * (error - pid->prevError);
    pid->prevError = error;
    return pTerm + iTerm + dTerm;
}



float err_speed=0;

//闭环力矩iq控制函数  控制频率 20khz
void FOC_CLOSED_Iq(Motor_Run_Typedef *motor,float Iq,float Id)
{
	float Uq=0,Ud=0;

	motor->Iq_pid->integral+=(Iq-motor->I_q);
	motor->Iq_pid->integral = LIMIT(motor->Iq_pid->integral,motor->Iq_pid->integralLimitMin,motor->Iq_pid->integralLimitMax);
	Uq = Iq*motor->Iq_pid->kp+motor->Iq_pid->integral*motor->Iq_pid->ki;

	motor->Id_pid->integral+=(Id-motor->I_d);
	motor->Id_pid->integral = LIMIT(motor->Id_pid->integral,motor->Id_pid->integralLimitMin,motor->Id_pid->integralLimitMax);
	Ud = Id*motor->Id_pid->kp+motor->Id_pid->integral*motor->Id_pid->ki;

	//乘以方向确保正方向和编码器正方向一致
    FOC_SVPWM(motor,Uq ,Ud,motor->electricalAngle);

}
//闭环速度控制函数 r/s  控制频率：10khz
float FOC_CLOSED_Vel(Motor_Run_Typedef *motor,float speed)
{

	return pidUpdateFloat(motor->V_pid,speed,motor->speed);

}
//闭环位置控制函数 单位：弧度 rad,控制频率：1khz
float FOC_CLOSED_Pos(Motor_Run_Typedef *motor,float Pos)
{
	return pidUpdateFloat(motor->P_pid,Pos,motor->encoder->rad_all);
}


//使能/失能电机驱动芯片
void FOC_EN(Motor_Run_Typedef *motor,OperationType2 ON_OFF)
{
	//根据初始参数，初始DRV8301使能
	DRV8301_EN( motor->DRV8301,ON_OFF);

}

//开启foc
void FOC_Start(Motor_Run_Typedef *motor)
{
	motor->run_flag=1;
}
//关闭foc
void FOC_Stop(Motor_Run_Typedef *motor)
{
	motor->run_flag=0;
}




//电机参数识别:data[0]-是否校准识别位,data[1]-零电角度，data[2]-方向，data[3]-极对数
int Motor_Parameter_Identification(Motor_Run_Typedef *motor,float* data)
{

	float angle;
	float mid_angle,end_angle;
	float moved;
	float dir,zero_electric_angle;
	int pole_pairs;
	float angle0;
	float count=1000.0f,i;

	printf("Motor parameter identification starts.\r\n");


	/**************************记录零点偏移角度**********************************/
	//使能偏转
	FOC_SVPWM(motor,0, 1,  0);
	HAL_Delay(500);
	angle0=MT6701_GetAngle_ALL(motor->encoder);

	/**************************计算极对数**********************************/
	// find natural direction
	// move one electrical revolution forward
	for(i=0; i<=count; i++)
	{
		angle = _2_PI * i / count;
		FOC_SVPWM(motor,0, 1,  angle);
		HAL_Delay(1);
	}
	mid_angle=MT6701_GetAngle_ALL(motor->encoder);

	for(i=count; i>=0; i--)
	{
		angle =  _2_PI * i / count ;
		FOC_SVPWM(motor,0, 1, angle);
		HAL_Delay(1);
	}
	end_angle=MT6701_GetAngle_ALL(motor->encoder);
	//相当于电机失能
//	FOC_EN(motor,OFF);
	//判断是否带载启动，如果带载的话，失能之后角度会变，这时校准的参数不准（可能太重角度偏移不准），需要重新校准
	if((fabsf(angle0-end_angle)>0.05))
	{
		printf("Failed!Please recalibrate the motor without load.\r\n");
		return 0;
	}

	/**************************计算旋转正方向**********************************/
	moved =  fabsf(mid_angle - end_angle);

	if((moved < 0.05) )  //相等或者几乎没有动，则认为没有上电或者负载太重带不动，校准失败
	{
		printf("Failed!Please recalibrate with POWER ON or without load.\r\n");

		return 0;
	}
	else if(mid_angle < end_angle)
	{
		printf("sensor_direction==CCW\r\n");
		dir=-1.0f;
	}
	else
	{
		printf("sensor_direction==CW\r\n");
		dir=1.0f;
	}

	/**************************计算极对数**********************************/
	pole_pairs=(int)(_2_PI/moved+0.5f);     //浮点数转整形，四舍五入

	printf("PP check: ");     //计算Pole_Pairs
	if( fabsf(moved*pole_pairs - _2_PI) > 0.3 )  // 0.2 is arbitrary number it can be lower or higher!
	{
		printf(" fail - estimated pp!\n");
		return 0;
    }
	else
	{
		printf("OK!\r\n");
	}

	/**************************计算零点偏移角度**********************************/
	zero_electric_angle = _normalizeAngle(dir*pole_pairs*angle0);


	data[0] = 1000.0f;		//如果校准了，这个数据就是1000
	data[1] = zero_electric_angle;
	data[2] = dir;
	data[3] = pole_pairs;



	return 1;
}

//将机械角度转换成实际电角度
float _electricalAngle(Motor_Run_Typedef *motor)
{
  return  _normalizeAngle((motor->Parameters->DIR *  motor->Parameters->Pole_pair)*motor->encoder->rad_all - motor->Parameters->zero_electric_angle);

}

// 归一化角度到 [0,2PI]
float _normalizeAngle(float angle)
{
  float a = fmod(angle, _2_PI);   //取余运算可以用于归一化，列出特殊值例子算便知
  return a >= 0 ? a : (a + _2_PI);
}


//求根公式近似算法
static float _sqrtApprox(float number) {//low in fat
  long i;
  float y;
  // float x;
  // const float f = 1.5F; // better precision

  // x = number * 0.5F;
  y = number;
  i = * ( long * ) &y;
  i = 0x5f375a86 - ( i >> 1 );
  y = * ( float * ) &i;
  // y = y * ( f - ( x * y * y ) ); // better precision
  return number * y;
}
//反正切2公式近似算法
static float fastAtan2(float y, float x) {
    float atan;
    float z = y / x;
    if (x == 0.0f) {
        if (y > 0.0f) return PI_2;
        if (y < 0.0f) return -PI_2;
        return 0.0f;
    }
    if (fabsf(z) < 1.0f) {
        atan = z / (1.0f + 0.28f * z * z);
        if (x < 0.0f) {
            if (y < 0.0f) return atan - PI;
            return atan + PI;
        }
        return atan;
    } else {
        atan = PI_2 - z / (z * z + 0.28f);
        if (y < 0.0f) return atan - PI;
        return atan;
    }
}

//设置三相占空比
void FOC_SVPWM(Motor_Run_Typedef *motor, float Uq, float Ud, float theta)
{

    float Uout;
    uint8_t sector;
    float T0, T1, T2;
    float Ta, Tb, Tc;

	// _sqrt is an approx of sqrt (3 - 4% error)
	Uout = _sqrtApprox(Ud * Ud + Uq * Uq) / VBUS;
	// angle normalisation in between 0 and 2pi
	// only necessary if using _sin and _cos - approximation functions
	theta = _normalizeAngle(theta + fastAtan2(Uq, Ud));
	//Uout限幅
	Uout = LIMIT(Uout,-0.577f,0.577f);


    sector = (theta / _3_PI) + 1;
    T1 = sqrt3 * arm_sin_f32(sector * PI_3 - theta) * Uout;
    T2 = sqrt3 * arm_sin_f32(theta - (sector - 1.0) * PI_3) * Uout;
    T0 = 1 - T1 - T2;

    // calculate the duty cycles(times)
    switch (sector)
    {
    case 1:
        Ta = T1 + T2 + T0 / 2;
        Tb = T2 + T0 / 2;
        Tc = T0 / 2;
        break;
    case 2:
        Ta = T1 + T0 / 2;
        Tb = T1 + T2 + T0 / 2;
        Tc = T0 / 2;
        break;
    case 3:
        Ta = T0 / 2;
        Tb = T1 + T2 + T0 / 2;
        Tc = T2 + T0 / 2;
        break;
    case 4:
        Ta = T0 / 2;
        Tb = T1 + T0 / 2;
        Tc = T1 + T2 + T0 / 2;
        break;
    case 5:
        Ta = T2 + T0 / 2;
        Tb = T0 / 2;
        Tc = T1 + T2 + T0 / 2;
        break;
    case 6:
        Ta = T1 + T2 + T0 / 2;
        Tb = T0 / 2;
        Tc = T1 + T0 / 2;
        break;
    default:  // possible error state
        Ta = 0;
        Tb = 0;
        Tc = 0;
    }

    __HAL_TIM_SetCompare(motor->htim, TIM_CHANNEL_1, Ta * motor->PWM_Period);
    __HAL_TIM_SetCompare(motor->htim, TIM_CHANNEL_2, Tb * motor->PWM_Period);
    __HAL_TIM_SetCompare(motor->htim, TIM_CHANNEL_3, Tc * motor->PWM_Period);

}

// 获取定时器的自动重装载值（ARR）
uint16_t getTimerARR(TIM_HandleTypeDef *htim)
{
    if (htim!= NULL)
    {
    	return htim->Instance->ARR;
    }
    // 返回 0 表示获取失败（可能是传入的定时器句柄无效）
    return 0;
}

//计算进中断的频率
void calculateInterruptFrequency(Motor_Run_Typedef *motor)
{
    // 获取预分频值
    uint16_t prescaler = motor->htim->Instance->PSC;

    // 获取自动重装载值（ARR）
    uint32_t arr = motor->htim->Instance->ARR;

    // 获取 CPU 主频
    uint32_t cpuFrequency = HAL_RCC_GetHCLKFreq();

    // 计算进中断理论频率
    motor->interruptFrequency = (float)cpuFrequency / ((prescaler + 1) * (arr + 1))/2.0f;

}


//获取三相电流值
void FOC_Get_Ia_Ib_Ic(Motor_Run_Typedef *motor)
{
    float commonFactor = 1.0f/4095.0f * 3.3f / Sample_R / motor->DRV8301->I_Gain;
    // 求电流值
    motor->I_C = ((float)motor->hadc->Instance->JDR1 - motor->I_A_Offset)* commonFactor;
    motor->I_B = ((float)motor->hadc->Instance->JDR2 - motor->I_B_Offset) * commonFactor;
    motor->I_A = -motor->I_C - motor->I_B;

//    // 求电流值
//	motor->I_C = ((float)motor->hadc->Instance->JDR1 - motor->I_A_Offset)/4095.0f*3.3f/ Sample_R / motor->DRV8301->I_Gain;
//	motor->I_B = ((float)motor->hadc->Instance->JDR2 - motor->I_B_Offset)/4095.0f*3.3f/ Sample_R / motor->DRV8301->I_Gain;
//    motor->I_A = -motor->I_C-motor->I_B;
}




float Ia,Ib,I_A,I_B;
//a,b,c三相电流转换成Id,Iq
void FOC_clarke_park_Transform(Motor_Run_Typedef *motor)
{
    // Clarke变换--等幅值变换 乘三分之二
    float I_alpha = motor->I_A;
    float I_beta = (motor->I_A + 2.0f * motor->I_B) * _1_sqrt3;

    float cos_angle = arm_cos_f32(motor->electricalAngle);
    float sin_angle = arm_sin_f32(motor->electricalAngle);

//    // Park变换
    motor->I_d =  I_alpha * cos_angle + I_beta * sin_angle;
    motor->I_q = -I_alpha * sin_angle + I_beta * cos_angle;
//
//    //进行反clark变换和反park变化对比验证
//    Ia = motor->I_d*cos_angle-motor->I_q*sin_angle;
//    Ib = motor->I_d*sin_angle+motor->I_q*cos_angle;
//    I_A = Ia;
//    I_B = -0.5f*I_A + sqrt3_2*Ib;
}

//获取电机电角度,pwm中断频率/10
void FOC_Get_electricalAngle_speed(Motor_Run_Typedef *motor)
{
    //获取编码器位置
    MT6701_GetAngle_ALL(motor->encoder);
    //更新实际电角度
    motor->electricalAngle = _electricalAngle(motor);

    // 通过转动弧度差计算转速r/s
    float new_speed = motor->Parameters->DIR*motor->encoder->d_rad*(motor->interruptFrequency/10.0f)/_2_PI;

    // 低通滤波处理速度
    static const float alpha = 0.4f; // 滤波系数，可根据实际情况调整
    if (motor->prev_speed!= 0) {
        motor->speed = alpha * new_speed + (1 - alpha) * motor->speed;
    } else {
        motor->speed = new_speed;
    }
    motor->prev_speed = motor->speed;
}







