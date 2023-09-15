/*
 * ESCOOTER_MonitorTask.c
 *
 *  Created on: 7 Jul 2023
 *      Author: TerenceLeung
 */
#include "ESCOOTER_MonitorTask.h"
#include "mc_api.h"
#include "main.h"

void ESCOOTER_PhysicalParameterInit(ESCOOTER_Physical_State_t *stateHandle)
{
	stateHandle->current_speed  = 0;
	stateHandle->phase_current  = 0;
	stateHandle->phase_voltage  = 0;
	stateHandle->motor_status   = 0;
	stateHandle->error_status   = 0;
	stateHandle->previous_error = 0;
}

/* FOC rate to high:               0x0001u
 * Software over voltage:          0x0002u
 * Software under voltage:         0x0004u
 * Software over temperature:      0x0008u
 * Startup failed:                 0x0010u
 * Speed feedback:                 0x0020u
 * Emergency input (Over current): 0x0040u
 * Software Error:                 0x0080u
 *
 *
 */
ESCOOTER_Physical_State_t ESCOOTER_PhysicalParameterMonitoring(ESCOOTER_Physical_State_t *stateHandle)
{
	/*You could test it by inputing dummy data */
    stateHandle->current_speed  = MC_GetMecSpeedAverageMotor1();
    stateHandle->phase_current  = MC_GetPhaseCurrentAmplitudeMotor1();
    stateHandle->phase_voltage  = MC_GetPhaseVoltageAmplitudeMotor1();
    stateHandle->motor_status   = (int32_t)MC_GetSTMStateMotor1();
    stateHandle->error_status   = MC_GetCurrentFaultsMotor1();
    stateHandle->previous_error = MC_GetOccurredFaultsMotor1();
    return *stateHandle;
}
