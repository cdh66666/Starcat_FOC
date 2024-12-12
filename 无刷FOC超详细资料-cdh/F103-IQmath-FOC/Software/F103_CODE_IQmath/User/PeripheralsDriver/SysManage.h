/***********************************************************************
 * @file SysManage.h
 * Description:
 *
 **********************************************************************/
#ifndef __SysManage_h__
#define __SysManage_h__

#include "stdint.h"
#include "main.h"

//系统管理 结构体声明
typedef enum
{
    Startup_Check 	 = 0x00,	  //开机自检
    Debug_Action     = 0x01,    //Debug调试
    StopRun_Action   = 0x02,    //停止运行
    NormalRun_Action = 0x03,    //正常运行
    ErrWarning_Action= 0x04,    //报错警告
}SystemAction;

typedef struct
{
    uint8_t StateAction;
}SystemState;

extern SystemState Sys;

uint8_t Get_SysState(void);
void Set_SysState(SystemAction SysAction);
void SysManage_Loop(void);
#endif // __SysManage_h__

