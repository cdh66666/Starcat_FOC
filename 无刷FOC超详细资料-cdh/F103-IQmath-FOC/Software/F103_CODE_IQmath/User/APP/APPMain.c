#include "APPMain.h"
#include "PerDriverMain.h"
#include "MCUDriverMain.h"
#include "FunctionMain.h"
#include "Timer.h"


/*************************************************************
** Function name:      APPMain_Init
** Descriptions:       APP初始化
** Input parameters:   none
** Output parameters:  none
** Returned value:     none
** Created by:         none
** Created date:       none
*************************************************************/
void APPMain_Init(void)
{
    MCUDriverMain_Init();
    FunctionMain_Init();
    PerDriverMain_Init();
}

/*************************************************************
** Function name:      APPMain_Loop
** Descriptions:       APP循环
** Input parameters:   none
** Output parameters:  none
** Returned value:     none
** Created by:         none
** Created date:       none
*************************************************************/
void APPMain_Loop(void)
{
    MCUDriverMain_Loop();
    FunctionMain_Loop();
    PerDriverMain_Loop();
}
