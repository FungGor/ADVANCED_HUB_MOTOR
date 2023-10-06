/*
 * Cruise_Conrol.c
 *
 *  Created on: Aug 31, 2023
 *      Author: TerenceLeung
 */
#include "Cruise_Control.h"

void Cruise_Control_Setting(Cruise_Control_t *speedHandle)
{
	speedHandle->KpVar = speedHandle->DefKpVar;
	speedHandle->KiVar = speedHandle->DefKiVar;
	speedHandle->KdVar = speedHandle->DefKdVar;
}

void Cruise_Control_Set_Kp(Cruise_Control_t *speedHandle, int16_t Kp)
{
     speedHandle->KpVar = Kp;
}

void Cruise_Control_Set_Ki(Cruise_Control_t *speedHandle, int16_t Ki)
{
	speedHandle->KiVar = Ki;
}

void Cruise_Control_Set_Kd(Cruise_Control_t *speedHandle, int16_t Kd)
{
	speedHandle->KdVar = Kd;
}

void Cruise_Control_PI_Controller(Cruise_Control_t *speedHandle, int32_t errorSignal)
{
	speedHandle->error = errorSignal;
}

void Cruise_Control_PID_Controller(Cruise_Control_t *speedHandle, int32_t errorSignal)
{
    speedHandle->error = errorSignal;
}
