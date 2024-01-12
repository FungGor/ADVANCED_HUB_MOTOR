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
ESCOOTER_Operation_t opInitHandle; //Store Static Variables

ESCOOTER_Physical_State_t pstateHandle; //Monitor the Escooter's Electrical & Mechanical Parameters
ESCOOTER_BrakeANDThrottleInput_t inputHandle;

/*ESboot() must be called when the E-Scooter is turned-on！*/
void ESboot()
{
	tempHandle.bDrivingState    = DRIVING_IDLE;
	tempHandle.BRAKE_Trigger    = false;
	tempHandle.THROTTLE_Pressed = false;
	tempHandle.need_KICK_OFF    = true;
	tempHandle.ESCOOTER_MOVE    = false;
	tempHandle.systemError      = false;


    /*Create The Task for controlling the E-Scooter Driving Status !!!*/
    //osThreadDef(driving,ESCOOTER_DrivingTaskControl,osPriorityBelowNormal,0,128);
    //tempHandle.EScooterInDriving = osThreadCreate(osThread(driving),NULL);

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

/**To be called by motor control protocol**/
/**Before running the Main Task, please verify this function carefully**/
/*Input some dummy commands -> check it*/
void ESCOOTER_InputBrakeSignal()
{
	tempHandle.BRAKE_Trigger = ESCOOTER_BrakeSignalTrigger(&tempHandle,&inputHandle);

	/*to give the message if the brake is pressed or not*/
}

/**To be called by motor control protocol**/
/**Before running the Main Task, please verify this function carefully**/
/*Input some dummy commands -> check it*/
void ESCOOTER_DEBUG_CancelBrakeSignal()
{
	ESCOOTER_DEBUG_ReleaseBrakeSignal(&tempHandle,&inputHandle);
}

/**KICK OFF STEP**/
/**Before running the Main Task, please verify this function carefully**/
/*Input some dummy commands -> check it*/
bool ESCOOTER_IsCarReady()
{
	/*Before using the MC_API to track the Motor's status, to ensure the State Machine runs properly, it's better to input dummy data to check if the
	 *logic is right*/
    //int16_t minSpeed = MC_GetMecSpeedAverageMotor1();
    int16_t minSpeed = 110;
    if (minSpeed >= 100)
    {
    	return true;
    }
    return false;
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
     tempHandle.bDrivingState = 3;
}

uint8_t emergency_triggered = 0;
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
    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,GPIO_PIN_SET);
		emergency_triggered = 1;
		failure = 1;
		/*RESET all original states. Might be it's not necessary (?) */
		tempHandle.THROTTLE_Pressed = false;
		//tempHandle.bDrivingState = EMERGENCY_STOP;
		/*Now the system stucks in this loop, for safety purpose, use must power off and check the connections again before power on again*/
	}
}

/**Control The E-Scooter's Main Task**/
/*Please check all the functionalities e.g. brake,throttle,parameter inputs carefully before running the High Frequency Task*/
/*Input some dummy commands -> check it*/
uint16_t error = 0;
__weak void ESCOOTER_StateMachineHighFrequencyTask(void const *argument)
{
	 ERROR_HANDLE_MSG(BOOT_COMPLETED);
     for(;;)
     {
    	  /*Before using the MC_API to track the Motor's status, to ensure the State Machine runs properly, it's better to input dummy data to check if the
    	   *logic is right*/
           //uint16_t error = MC_GetOccurredFaultsMotor1();
           ESCOOTER_saveStatus(tempHandle.bDrivingState);
           /*To verify the state machine for debug purpose, please un-comment the following lines: */
    	   //int32_t status = 0;
         ESCOOTER_DRIVING_CONTROL();
         if(failure == 0)
         {
             tempHandle.THROTTLE_Pressed = true;
         }
         if ((tempHandle.ESCOOTER_MOVE == false) && (tempHandle.need_KICK_OFF == true))
         {
             if ( (ESCOOTER_IsCarReady() == true) && (tempHandle.THROTTLE_Pressed == true) && (error == 0)
                   && (tempHandle.BRAKE_Trigger == false) )
             {
            	 tempHandle.ESCOOTER_MOVE = true;
            	 tempHandle.need_KICK_OFF = false;
            	 tempHandle.bDrivingState = DRIVING_START;
            	 ESCOOTER_saveStatus(tempHandle.bDrivingState);
            	 //ESCOOTER_Driving_Start();
            	 //HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,GPIO_PIN_SET);

             }
             else
             {
            	 tempHandle.ESCOOTER_MOVE = false;
            	 tempHandle.need_KICK_OFF = true;
            	 tempHandle.bDrivingState = DRIVING_IDLE;
            	 //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,GPIO_PIN_SET);
            	 //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,GPIO_PIN_RESET);
            	 ESCOOTER_saveStatus(tempHandle.bDrivingState);
             }
         }
         else if (tempHandle.ESCOOTER_MOVE == true)
		 {
        	 /*Stop it normally
        	  * There are several ways to stop the E-Scooter:
        	  * --> User Triggers the Brake Signal (Press Brake)
        	  * --> User Releases the Throttle with Iq closing to 0 !
        	  * --> Emergency Stop
        	  * The program must handle with the above conditions
        	  * To test for the second condition, we must get a throttle with dash-board / generate some dummy data!
        	  * */
             if (tempHandle.BRAKE_Trigger == true && (inputHandle.TARGET_IQ <= 6) )

             {
            	 tempHandle.THROTTLE_Pressed = false;
            	 tempHandle.need_KICK_OFF = true;
            	 tempHandle.bDrivingState = DRIVING_STOP;
            	 tempHandle.BRAKE_Trigger = false;
            	 tempHandle.ESCOOTER_MOVE = false;
            	 ESCOOTER_saveStatus(tempHandle.bDrivingState);
            	 HAL_Delay(1500);
            	 tempHandle.bDrivingState = DRIVING_IDLE;
            	 //osSignalSet(tempHandle.EScooterInDriving,0x03);
                 //HAL_Delay(2000);
             }

             /*Emergency Stop due to motor error, system error, ..... Once error occurs, system jumps to the following instructions... */
             /*As the MCSDK automatically stops the motor,we no need to call the driving stop functions....*/
             else if(error != 0)
             {
            	 tempHandle.THROTTLE_Pressed = false;
            	 tempHandle.need_KICK_OFF = true;
            	 tempHandle.bDrivingState = DRIVING_STOP;
            	 tempHandle.BRAKE_Trigger = false;
            	 tempHandle.ESCOOTER_MOVE = false;
            	 ESCOOTER_saveStatus(tempHandle.bDrivingState);
            	 HAL_Delay(1500);
            	 tempHandle.bDrivingState = DRIVING_IDLE;
             }
		 }
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
		tempHandle.need_KICK_OFF = true;
		tempHandle.bDrivingState = DRIVING_STOP;
		tempHandle.BRAKE_Trigger = false;
		tempHandle.ESCOOTER_MOVE = false;
		ESCOOTER_saveStatus(tempHandle.bDrivingState);
		HAL_Delay(1500);
		tempHandle.bDrivingState = DRIVING_IDLE;
		ESCOOTER_Driving_Stop();
		//vTaskSuspend(opInitHandle.EScooterInDriving);
	}
	else if (ESCOOTER_getStatus() == DRIVING_IDLE)
	{
		//vTaskSuspend(opInitHandle.EScooterInDriving);
	}
	//osThreadTerminate(opInitHandle.EScooterInDriving);
}
