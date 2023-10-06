/*
 * Cruise_Control.h
 *
 *  Created on: Aug 31, 2023
 *      Author: TerenceLeung
 */

#ifndef ESCOOTER_CRUISE_CONTROL_H_
#define ESCOOTER_CRUISE_CONTROL_H_

#include "stdint.h"

typedef struct
{
	int16_t DefKpVar; /*Default Kp Parameter*/
	int16_t DefKiVar; /*Default Ki Parameter*/
	int16_t DefKdVar; /*Default Kd Parameter*/
	int16_t KpVar;    /*Instant Kp Parameter*/
	int16_t KiVar;    /*Instant Ki Parameter*/
	int16_t KdVar;    /*Instant Kd Parameter*/
	int32_t error;
}Cruise_Control_t;

void Cruise_Control_Setting(Cruise_Control_t *speedHandle);

void Cruise_Control_Set_Kp(Cruise_Control_t *speedHandle, int16_t Kp);

void Cruise_Control_Set_Ki(Cruise_Control_t *speedHandle, int16_t Ki);

void Cruise_Control_Set_Kd(Cruise_Control_t *speedHandle, int16_t Kd);

void Cruise_Control_PI_Controller(Cruise_Control_t *speedHandle, int32_t errorSignal);

void Cruise_Control_PID_Controller(Cruise_Control_t *speedHandle, int32_t errorSignal);

#endif /* ESCOOTER_CRUISE_CONTROL_H_ */
