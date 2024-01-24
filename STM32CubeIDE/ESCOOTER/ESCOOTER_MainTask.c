/*
 * ESCOOTER_StateMachine.c
 *
 *  Created on: 7 Jul 2023
 *      Author: TerenceLeung
 *
 *  ESCOOTER_MainTask.c provides functionalities for E-Scooter Driving Control, which is the core of Driving System
 *  Please verify it under the SYSTEM Debugging Mode
 */
#include "ESCOOTER_MainTask.h"
#include "ESCOOTER_MonitorTask.h"
#include "ESCOOTER_BrakeAndThrottle.h"
#include "ESCOOTER_DRIVING.h"
#include "ERROR_HANDLER.h"
#include "escooter_config.h"
#include "mc_api.h"
#include "main.h"
#include "cmsis_os.h"

/*UART6 --> Tx: PA11; Rx:PA12*/
ESCOOTER_Operation_t tempHandle; //Store Dynamic Variables

ESCOOTER_Physical_State_t pstateHandle; //Monitor the Escooter's Electrical & Mechanical Parameters
ESCOOTER_BrakeANDThrottleInput_t inputHandle;

/*ESboot() must be called when the E-Scooter is turned-on！*/
void ESboot()
{
	tempHandle.bDrivingState    = DRIVING_IDLE;
	tempHandle.BRAKE_Trigger    = false;
	tempHandle.THROTTLE_Pressed = false;
	tempHandle.systemError      = false;

    /*Save the current driving status. IDLE Mode is set in Default*/
    ESCOOTER_saveStatus(tempHandle.bDrivingState);

    /*Initialize the E-Scooter in Ambler Mode*/
    ESCOOTER_DrivingModeSetDefault(&inputHandle);
    ESCOOTER_DetermineDrivingMode(&inputHandle);

    /*Initialize the Motor Parameters*/
    ESCOOTER_PhysicalParameterInit(&pstateHandle);


}

/*Triggered by Dashboard's commands! Configure the driving mode by adjusting the current, speed and acceleration ramp limits*/
/*Save those parameters in form of &inputHandle pointer*/
/*Input some dummy commands -> check it*/
/*It's assumed that the E-Scooter is in IDLE State*/
void ESCOOTER_DriveModeConfig(int16_t max_IQ, int16_t allowable_rpm, uint16_t acceleration_ramp)
{
	ESCOOTER_ConfigDrivingMode(&inputHandle,max_IQ,allowable_rpm,acceleration_ramp);
}

/**To be called by motor control protocol**/
/**Before running the Main Task, please verify this function carefully**/
/*Input some dummy commands -> check it*/
void ESCOOTER_InputThrottleSignal(int16_t targetCurrent)
{
	inputHandle.TARGET_IQ = targetCurrent;

	/*to give the message if the throttle is pressed or not ! */
	tempHandle.THROTTLE_Pressed = ESCOOTER_ThrottleSignalTrigger(&inputHandle);

	/*Send the Iq signal to the ESCOOTER_DRIVING (?)*/
	ESCOOTER_Set_Limit(&inputHandle);

}

void ESCOOTER_SendReportStatus(bool error)
{
	tempHandle.systemError = error;
}

bool ESCOOTER_GetReportStatus()
{
    return tempHandle.systemError;
}

void ESCOOTER_UpdateDrivingState(uint8_t state)
{
     tempHandle.bDrivingState = state;
}

uint8_t failure = 0;
void ESCOOTER_DRIVING_CONTROL()
{
	if(ESCOOTER_getStatus() == DRIVING_IDLE)
	{
		ESCOOTER_Driving_Stop();
	}
	else if (ESCOOTER_getStatus() == DRIVING_START)
	{
		ESCOOTER_Driving_Start();
	}
	else if(ESCOOTER_getStatus() == DRIVING_STOP)
	{
        ESCOOTER_Driving_Stop();
	}
	else if(ESCOOTER_getStatus() == EMERGENCY_STOP)
	{
		ESCOOTER_Driving_Stop();
	}
}

/**Control The E-Scooter's Main Task**/
/*Please check all the functionalities e.g. brake,throttle,parameter inputs carefully before running the High Frequency Task*/
/*Input some dummy commands -> check it*/
uint16_t error = 0;
__weak void ESCOOTER_StateMachineHighFrequencyTask(void const *argument)
{
     for(;;)
     {
    	 ESCOOTER_saveStatus(DRIVING_START);
         ESCOOTER_DRIVING_CONTROL();
     }

}


/**Check the Motor Parameters Regularly**/
/*Input some dummy commands -> check it*/
/*It's better to call this task regularly with specific time interval*/
uint16_t timer_interval = PERIODIC_CAPTURE_TIME;
uint16_t task_Counter = 0;
void ESCOOTER_ParameterMonitoring(void const *argument)
{
    for(;;)
    {
    	/*Sample it in every 100 ms*/
    	if((task_Counter % 2) == 0)
    	{
        	pstateHandle = ESCOOTER_PhysicalParameterMonitoring(&pstateHandle);
        	ESCOOTER_Set_PhysicalParam(&pstateHandle);
        	ESCOOTER_Get_Speed();
        	ESCOOTER_Get_PhaseCurrent();
        	ESCOOTER_Get_PhaseVoltage();
        	/*If there are any faults detected, the monitor must report the errors by calling ESCOOTER_SendReportStatus() !! */
    	}
    	osDelay(PERIODIC_CAPTURE_TIME);
    	task_Counter ++;
    }
}

void ESCOOTER_Stop_Driving_Task()
{
	/*Failure occurs when the E-Scooter is in operation*/
	if((ESCOOTER_getStatus() == DRIVING_START) && (tempHandle.ESCOOTER_MOVE == true))
	{
		/*Stop Driving, the E-Scooter will gradually decelerate*/
		failure = 1;
		tempHandle.THROTTLE_Pressed = false;
		tempHandle.bDrivingState = DRIVING_STOP;
		tempHandle.BRAKE_Trigger = false;
		ESCOOTER_saveStatus(tempHandle.bDrivingState);
		HAL_Delay(1500);
		tempHandle.bDrivingState = DRIVING_IDLE;
		ESCOOTER_Driving_Stop();
	}
	else if (ESCOOTER_getStatus() == DRIVING_IDLE)
	{
	}
}
