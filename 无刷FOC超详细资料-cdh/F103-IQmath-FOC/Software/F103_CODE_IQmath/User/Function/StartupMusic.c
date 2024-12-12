#include "StartupMusic.h"
#include "Motor1TIM1Pwm.h"
#include "FocControl.h"
#include "IQmathLib.h"

/*************************************************************
** Function name:       StartupMusic_Loop
** Descriptions:        ¿ª»úÒôÀÖÑ­»·  1ms
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void StartupMusic_Loop(void)
{
	static uint16_t TickTime = 0;
	
	if(TickTime < 2000)TickTime ++;
	
	if(TickTime < 100)
	{
		FocMotor_Enable(1);
		SetMotor1Channel_FreqDuty(500,0.5);
	}
	else if(TickTime <200)SetMotor1Channel_FreqDuty(0,0.5);
	else if(TickTime <300)SetMotor1Channel_FreqDuty(500,0.5);
	else if(TickTime <400)SetMotor1Channel_FreqDuty(0,0.5);
	else if(TickTime <600)SetMotor1Channel_FreqDuty(600,0.5); 
//	else if(TickTime <1200)SetMotor1Channel_FreqDuty(0,0.5);
//	else if(TickTime <1400)SetMotor1Channel_FreqDuty(2600);
	else 
	{
		FocMotor_Enable(0);
	  SetMotor1Channel_FreqDuty(20000,0);
	}
}




