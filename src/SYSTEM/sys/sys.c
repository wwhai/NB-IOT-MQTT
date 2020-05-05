#include "sys.h"
#include "stm32f10x_iwdg.h"

void NVIC_Configuration(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级

}

void IWDG_Configuration(void)
{
 IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
 IWDG_SetPrescaler(IWDG_Prescaler_256);
 IWDG_SetReload(0xFFF);
	
 IWDG_ReloadCounter();
 IWDG_Enable();
}

void IWDG_Feed(void)
{
    IWDG_ReloadCounter();    /*reload*/
}

void Soft_Reset(void)
{
    __set_FAULTMASK(1);
    NVIC_SystemReset();
}


