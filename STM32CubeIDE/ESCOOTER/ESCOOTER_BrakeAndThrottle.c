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
