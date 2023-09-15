/*
 * escooter_config.c
 *
 *  Created on: 7 Jul 2023
 *      Author: TerenceLeung
 */
#include "escooter_config.h"
#include "ESCOOTER_DRIVING.h"

EScooter_Driving_Mode_t driving_mode;

/*Static + Default*/
void ESCOOTER_DrivingModeSetDefault(ESCOOTER_BrakeANDThrottleInput_t *limit)
{
   limit->TARGET_IQ = 0;
   limit->IQ_LIMIT = 15750;
   limit->SPEED_LIMIT = 663;
   limit->RAMP_DURATION = 2000;
   ESCOOTER_Set_Limit(limit);
}

/*Dynamic*/
void ESCOOTER_ConfigDrivingMode(ESCOOTER_BrakeANDThrottleInput_t *limit,int16_t max_IQ, int16_t allowable_rpm, uint16_t acceleration_ramp)
{
	/*Triggered by Dashboard's commands! Configure the driving mode by adjusting the current, speed and acceleration ramp limits*/
	/*Save those parameters in form of &inputHandle pointer*/
	limit->TARGET_IQ = 0;
	limit->IQ_LIMIT = max_IQ;
	limit->SPEED_LIMIT = allowable_rpm;
	limit->RAMP_DURATION = acceleration_ramp;
	/*Re-Config the driving mode again*/
	ESCOOTER_DetermineDrivingMode(limit);
	ESCOOTER_Set_Limit(limit);

}

void ESCOOTER_DetermineDrivingMode(ESCOOTER_BrakeANDThrottleInput_t *limit)
{
	int16_t max_Current[3] = {6457,11497,15750};
	int16_t max_RPM[3]     = {270,480,663};
	int16_t max_Ramp[3]    = {4000,3000,2000};

	if ( ((limit->IQ_LIMIT) == max_Current[0]) && ((limit->SPEED_LIMIT) == max_RPM[0]) && ((limit->RAMP_DURATION) == max_Ramp[0]) )
	{
       driving_mode = BRAKE_AND_THROTTLE_SPEED_MODE_AMBLE;
	}
	else if ( ((limit->IQ_LIMIT) == max_Current[1]) && ((limit->SPEED_LIMIT) == max_RPM[1]) && ((limit->RAMP_DURATION) == max_Ramp[1]) )
	{
	   driving_mode = BRAKE_AND_THROTTLE_SPEED_MODE_LEISURE;
	}
	else if ( ((limit->IQ_LIMIT) == max_Current[2]) && ((limit->SPEED_LIMIT) == max_RPM[2]) && ((limit->RAMP_DURATION) == max_Ramp[2]) )
	{
	   driving_mode = BRAKE_AND_THROTTLE_SPEED_MODE_SPORTS;
	}
	else
	{
		driving_mode = driving_mode;
	}
}


