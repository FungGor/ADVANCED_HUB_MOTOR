/*
 * escooter_config.h
 *
 *  Created on: 7 Jul 2023
 *      Author: TerenceLeung
 */

#ifndef ESCOOTER_ESCOOTER_CONFIG_H_
#define ESCOOTER_ESCOOTER_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ESCOOTER_BrakeAndThrottle.h"


typedef enum
{
	BRAKE_AND_THROTTLE_SPEED_MODE_AMBLE    = 0,
	BRAKE_AND_THROTTLE_SPEED_MODE_LEISURE  = 1,
	BRAKE_AND_THROTTLE_SPEED_MODE_SPORTS   = 2
}EScooter_Driving_Mode_t;

/*If the Motor Control still cannot receive the command from the dash board, default mode is set : Amble Mode*/
void ESCOOTER_DrivingModeSetDefault(ESCOOTER_BrakeANDThrottleInput_t *limit);

void ESCOOTER_ConfigDrivingMode(ESCOOTER_BrakeANDThrottleInput_t *limit,int16_t max_IQ, int16_t allowable_rpm, uint16_t acceleration_ramp);

void ESCOOTER_DetermineDrivingMode(ESCOOTER_BrakeANDThrottleInput_t *limit);

#ifdef __cplusplus
}
#endif

#endif /* ESCOOTER_ESCOOTER_CONFIG_H_ */
