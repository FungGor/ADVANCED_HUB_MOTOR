/*
 * ESCOOTER_BrakeAndThrottle.c
 *
 *  Created on: 7 Jul 2023
 *      Author: TerenceLeung
 */
#include "ESCOOTER_BrakeAndThrottle.h"
#include "main.h"

/**To be called by motor control protocol**/
/**Before running the Main Task, please verify this function carefully**/
/*Input some dummy commands -> check it*/
bool ESCOOTER_ThrottleSignalTrigger(ESCOOTER_BrakeANDThrottleInput_t *inputHandle)
{
	if(inputHandle->TARGET_IQ != 0)
	{
		return true;
	}
	return false;
}

/**To be called by motor control protocol**/
/**Before running the Main Task, please verify this function carefully**/
/*Input some dummy commands -> check it*/
/*This function must be triggered by UART Commands!!*/
bool ESCOOTER_BrakeSignalTrigger(ESCOOTER_Operation_t *opHandle,ESCOOTER_BrakeANDThrottleInput_t *inputHandle)
{
	if(inputHandle->brake == false)
	{
	    if ((opHandle->ESCOOTER_MOVE == true) || (opHandle->ESCOOTER_MOVE == false))
	    {
	    	inputHandle->brake = true;
	    }
	}
	if(inputHandle->brake == true)
	{
		return true;
	}
	return false;
}

/**To be called by motor control protocol**/
/**Before running the Main Task, please verify this function carefully**/
/*Input some dummy commands -> check it*/
/*This function must be triggered by UART Commands!!*/
/*Change it to boolean*/
void ESCOOTER_DEBUG_ReleaseBrakeSignal(ESCOOTER_Operation_t *opHandle, ESCOOTER_BrakeANDThrottleInput_t *inputHandle)
{
	if(inputHandle->brake == true)
	{
	    if ((opHandle->ESCOOTER_MOVE == true) || (opHandle->ESCOOTER_MOVE == false))
	    {
	    	inputHandle->brake = false;
	    }
	}
	if(inputHandle->brake == false)
	{
		//HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,GPIO_PIN_RESET);
	}
}
