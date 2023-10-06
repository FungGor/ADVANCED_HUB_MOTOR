/*
 * ESCOOTER_DRIVING.h
 *
 *  Created on: 18 Jul 2023
 *      Author: TerenceLeung
 */

#ifndef ESCOOTER_ESCOOTER_DRIVING_H_
#define ESCOOTER_ESCOOTER_DRIVING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"
#include "ESCOOTER_MainTask.h"
#include "ESCOOTER_BrakeAndThrottle.h"
#include "ESCOOTER_MonitorTask.h"

void ESCOOTER_Safety_Limit_Setting();

void ESCOOTER_saveStatus (uint8_t state);

ESCOOTER_Driving_State_t ESCOOTER_getStatus();

void ESCOOTER_Set_Limit(ESCOOTER_BrakeANDThrottleInput_t *limitHandle);

void ESCOOTER_Set_PhysicalParam(ESCOOTER_Physical_State_t *motorParam);

void ESCOOTER_Get_Speed();

void ESCOOTER_Get_PhaseCurrent();

void ESCOOTER_Get_PhaseVoltage();

void ESCOOTER_Get_MotorState();

void Speed_Cruise_Control();

bool ESCOOTER_Detect_Decelerate();

void ESCOOTER_Driving_Start();

void ESCOOTER_Driving_Stop();

void MOTOR_BRAKE(); /*Just for debugging purpose (?) **/

/*This thread might be useful (?) This thread will be deleted (?)*/
void ESCOOTER_DrivingTaskControl(void const * argument);

#ifdef __cplusplus
}
#endif
#endif /* ESCOOTER_ESCOOTER_DRIVING_H_ */
