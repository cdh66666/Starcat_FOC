#include "LedDriver.h"
#include "SysManage.h"


/*************************************************************
** Function name:       RLedRun_Loop
** Descriptions:        RLED闪烁控制
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void RLedRun_Loop(uint16_t *TickTime, uint16_t x1,uint16_t x2,uint16_t x3,uint16_t ALL)
{
    if(*TickTime < ALL)
    {
        *TickTime += 1;   //时基递加
        if(*TickTime < x1)      LED_R_ON;
        else if(*TickTime < x1+x2) LED_R_OFF;
        else if(*TickTime < x1+x2+x3) LED_R_ON;
        else LED_R_OFF;
    }        
    else *TickTime = 0;
}
/*************************************************************
** Function name:       BLedRun_Loop
** Descriptions:        BLED闪烁控制
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void BLedRun_Loop(uint16_t *TickTime, uint16_t x1,uint16_t x2,uint16_t x3,uint16_t ALL)
{
    if(*TickTime < ALL)
    {
        *TickTime += 1;   //时基递加
        if(*TickTime < x1)      LED_B_ON;
        else if(*TickTime < x1+x2) LED_B_OFF;
        else if(*TickTime < x1+x2+x3) LED_B_ON;
        else LED_B_OFF;
    }        
    else *TickTime = 0;
}
/*************************************************************
** Function name:       LedDriver_Loop
** Descriptions:        LED底层刷新函数
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             1ms刷新
*************************************************************/
void LedDriver_Loop(void)
{
    static uint16_t RTickTimes = 0;
    static uint16_t BTickTimes = 0;
    static uint8_t LastSysState = 0;
    uint8_t SystemState;
    
    SystemState = Get_SysState();     //获取当前系统状态
    if(SystemState != LastSysState)   //时基清零
    {
        RTickTimes = 0;
        BTickTimes = 0;
        LED_B_OFF;
        LED_R_OFF;
    }
    switch(SystemState)
    {
        case Startup_Check:
            RLedRun_Loop(&RTickTimes,60,20,60,200);
            BLedRun_Loop(&BTickTimes,80,30,30,160);
            break;
        case Debug_Action:
            RLedRun_Loop(&RTickTimes,100,50,100,400);
            BLedRun_Loop(&BTickTimes,50,20,50,1000);
            break;
        case StopRun_Action:
            RLedRun_Loop(&RTickTimes,50,20,50,1000);
            break;
        case NormalRun_Action:
            BLedRun_Loop(&BTickTimes,80,40,80,1000);
            break;
        case ErrWarning_Action:
            RLedRun_Loop(&RTickTimes,100,50,100,400);
            break;
        default:break;
    }
    LastSysState = SystemState;
}





