/*
 * escooter_control.c
 *
 *  Created on: 7 Jul 2023
 *      Author: Tim
 */
#include "escooter_control.h"
#include "ESCOOTER_MainTask.h"
#include "cmsis_os.h"
#include "main.h"

osThreadId EScooterStateMachineHandle;
osThreadId EScooterMonitorHandle;


__weak void ESCOOTER_init()
{
   ESboot();
}

__weak void ESCOOTER_RunCoreTask(void)
{
	/*Controls the E-Scooter's main operation*/
    osThreadDef(stateMachine,ESCOOTER_StateMachineHighFrequencyTask,osPriorityBelowNormal,0,128);
    EScooterStateMachineHandle = osThreadCreate(osThread(stateMachine), NULL);

    /*Might be it's not necessary (?) */
    osThreadDef(motorParameter,ESCOOTER_ParameterMonitoring,osPriorityBelowNormal,0,128);
    EScooterMonitorHandle = osThreadCreate(osThread(motorParameter), NULL);

}

__weak void ESCOOTER_StopCoreTask(void)
{
    /*Stops the E-Scooter's main application*/
	//ESCOOTER_Stop_Driving_Task();
	vTaskSuspend(EScooterStateMachineHandle);
	vTaskSuspend(EScooterMonitorHandle);
}
