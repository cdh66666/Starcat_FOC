#include "FocSVPWM.h"
#include "FocMath.h"
#include "Motor1TIM1Pwm.h"



//SVPWM模型创建 SVPWM_Mod
Svpwm_Struct SVPWM_Mod = 
{
    _IQ(0),     //阿尔法轴目标电压 	(最大值为母线电压 * sqrt(3) / 3)
    _IQ(0),     //贝塔轴目标电压   	(最大值为母线电压 * sqrt(3) / 3)
    _IQ(0),		//用于扇区判断
    _IQ(0),		//用于扇区判断
    _IQ(0),		//用于扇区判断
    _IQ(0),		//0矢量作用时长
    _IQ(0),		//1矢量作用时长
    _IQ(0),		//2矢量作用时长
    _IQ(0),		//3矢量作用时长
    _IQ(0),		//4矢量作用时长
    _IQ(0),		//5矢量作用时长
    _IQ(0),		//6矢量作用时长
    _IQ(0),		//7矢量作用时长
    _IQ(0),     //a相作用时长
    _IQ(0),     //b相作用时长
    _IQ(0),     //c相作用时长
    _IQ(100),   //SVPWM 周期(us)
    _IQ(14.43375673),//SVPWM 系数K =  sqrt(3) * Ts / Udc = 14.433756729740644112728
    _IQ(12),    //母线电压
    0,          //扇区索引
};
/*************************************************************
** Function name:       SectorJudgment
** Descriptions:        扇区判断,利用uα和uβ来判断扇区
** Input parameters:    pSvpwm：结构体名字
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void SvpwmSectorJudgment(Svpwm_Struct *pSvpwm)
{
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t sector;

    pSvpwm->u1 =  pSvpwm->uBeta;
	  pSvpwm->u2 =  _IQmpy(pSvpwm->uAlpha , SVPWM_SQRT3_2) - _IQdiv2(pSvpwm->uBeta);
	  pSvpwm->u3 = -_IQmpy(pSvpwm->uAlpha , SVPWM_SQRT3_2) - _IQdiv2(pSvpwm->uBeta);

    if (pSvpwm->u1 > _IQ(0)) {
        a = 1;
    } else {
        a = 0;
    }
    if (pSvpwm->u2 > _IQ(0)) {
        b = 1;
    } else {
        b = 0;
    }
    if (pSvpwm->u3 > _IQ(0)) {
        c = 1;
    } else {
        c = 0;
    }

    sector = 4*c + 2*b + a;
    switch (sector) {
        case 3:
            pSvpwm->sector = 1;
            break;
        case 1:
            pSvpwm->sector = 2;
            break;
        case 5:
            pSvpwm->sector = 3;
            break;
        case 4:
            pSvpwm->sector = 4;
            break;
        case 6:
            pSvpwm->sector = 5;
            break;
        case 2:
            pSvpwm->sector = 6;
            break;
    }
}
/*************************************************************
** Function name:       GetVectorDuration
** Descriptions:        获取矢量作用时长
** Input parameters:    pSvpwm:结构体指针
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void GetVectorDuration(Svpwm_Struct *pSvpwm)
{
    switch (pSvpwm->sector) {
        case 1:
            pSvpwm->t4 = _IQmpy(pSvpwm->K,pSvpwm->u2);
        
            pSvpwm->t6 = _IQmpy(pSvpwm->K,pSvpwm->u1);
            pSvpwm->t0 = pSvpwm->t7 = _IQdiv2(pSvpwm->ts - pSvpwm->t4 - pSvpwm->t6);
            break;
        case 2:
            pSvpwm->t2 = -_IQmpy(pSvpwm->K,pSvpwm->u2);
            pSvpwm->t6 = -_IQmpy(pSvpwm->K,pSvpwm->u3);
            pSvpwm->t0 = pSvpwm->t7 = _IQdiv2(pSvpwm->ts - pSvpwm->t2 - pSvpwm->t6);
            break;
        case 3:
            pSvpwm->t2 = _IQmpy(pSvpwm->K,pSvpwm->u1);
            pSvpwm->t3 = _IQmpy(pSvpwm->K,pSvpwm->u3);
            pSvpwm->t0 = pSvpwm->t7 =  _IQdiv2(pSvpwm->ts - pSvpwm->t2 - pSvpwm->t3);
            break;
        case 4:
            pSvpwm->t1 = -_IQmpy(pSvpwm->K,pSvpwm->u1);
            pSvpwm->t3 = -_IQmpy(pSvpwm->K,pSvpwm->u2);
            pSvpwm->t0 = pSvpwm->t7 =  _IQdiv2(pSvpwm->ts - pSvpwm->t1 - pSvpwm->t3);
            break;
        case 5:
            pSvpwm->t1 = _IQmpy(pSvpwm->K,pSvpwm->u3);
            pSvpwm->t5 = _IQmpy(pSvpwm->K,pSvpwm->u2);
            pSvpwm->t0 = pSvpwm->t7 =  _IQdiv2(pSvpwm->ts - pSvpwm->t1 - pSvpwm->t5);
            break;
        case 6:
            pSvpwm->t4 = -_IQmpy(pSvpwm->K,pSvpwm->u3);
            pSvpwm->t5 = -_IQmpy(pSvpwm->K,pSvpwm->u1);
            pSvpwm->t0 = pSvpwm->t7 =  _IQdiv2(pSvpwm->ts - pSvpwm->t4 - pSvpwm->t5);
            break;
        default:
            break;
    }
}
/*************************************************************
** Function name:       SvpwmGenerate
** Descriptions:        SVPWM生成周期函数
** Input parameters:    pSvpwm:结构体指针
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
//Debug 马鞍波
//float tA;
//float tB;
//float tC;
void SvpwmGenerate(Svpwm_Struct *pSvpwm)
{

    switch (pSvpwm->sector) {
        case 1:
            pSvpwm->ta = pSvpwm->t4 + pSvpwm->t6 + pSvpwm->t7;
            pSvpwm->tb = pSvpwm->t6 + pSvpwm->t7;
            pSvpwm->tc = pSvpwm->t7;
            break;
        case 2:
            pSvpwm->ta = pSvpwm->t6 + pSvpwm->t7;
            pSvpwm->tb = pSvpwm->t2 + pSvpwm->t6 + pSvpwm->t7;
            pSvpwm->tc = pSvpwm->t7;
            break;
        case 3:
            pSvpwm->ta = pSvpwm->t7;
            pSvpwm->tb = pSvpwm->t2 + pSvpwm->t3 + pSvpwm->t7;
            pSvpwm->tc = pSvpwm->t3 + pSvpwm->t7;
            break;
        case 4:
            pSvpwm->ta = pSvpwm->t7;
            pSvpwm->tb = pSvpwm->t3 + pSvpwm->t7;
            pSvpwm->tc = pSvpwm->t1 + pSvpwm->t3 + pSvpwm->t7;
            break;
        case 5:
            pSvpwm->ta = pSvpwm->t5 + pSvpwm->t7;
            pSvpwm->tb = pSvpwm->t7;
            pSvpwm->tc = pSvpwm->t1 + pSvpwm->t5 + pSvpwm->t7;
            break;
        case 6:
            pSvpwm->ta = pSvpwm->t4 + pSvpwm->t5 + pSvpwm->t7;
            pSvpwm->tb = pSvpwm->t7;
            pSvpwm->tc = pSvpwm->t5 + pSvpwm->t7;
            break;
    }
		SetMotor1ChannelHighLeaveTime_us(pSvpwm->ta,pSvpwm->tb,pSvpwm->tc);
//    tA = _IQtoF(pSvpwm->ta); //Debug
//    tB = _IQtoF(pSvpwm->tb); //Debug
//    tC = _IQtoF(pSvpwm->tc); //Debug
    
}
/*************************************************************
** Function name:       SvpwmContorol
** Descriptions:        Svpwm控制
** Input parameters:    pSvpwm:结构体指针
**                      uα：阿尔法轴目标电压
**                      uβ：贝塔轴目标电压
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void SvpwmCtrl(_iq uAlpha,_iq uBeta)
{
    SVPWM_Mod.uAlpha = uAlpha;
    SVPWM_Mod.uBeta  = uBeta;
    //1.扇区判断
    SvpwmSectorJudgment(&SVPWM_Mod);
    //2.计算矢量作用时长
    GetVectorDuration(&SVPWM_Mod);
    //3.SVPWM生成
    SvpwmGenerate(&SVPWM_Mod);
}
/*************************************************************
** Function name:       GetSVPWMSector
** Descriptions:        获取SVPWM扇区
** Input parameters:    None
** Output parameters:   None
** Returned value:      SVPWM扇区(1-6)
** Remarks:             None
*************************************************************/
uint8_t GetSVPWMSector(void)
{
    return SVPWM_Mod.sector;
}





