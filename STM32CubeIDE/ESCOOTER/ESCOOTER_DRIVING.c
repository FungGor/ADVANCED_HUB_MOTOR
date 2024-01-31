/*
 * ESCOOTER_DRIVING.c
 *
 *  Created on: 18 Jul 2023
 *      Author: TerenceLeung
 */

#include "ESCOOTER_DRIVING.h"
#include "POWER_CONTROL.h"
#include "POWER_CONTROL_LL.h"
#include "Cruise_Control.h"
#include "SafetySetting.h"
#include "ESCOOTER_MainTask.h"
#include "ERROR_HANDLER.h"
#include "mc_type.h"
#include "cmsis_os.h"
#include "mc_api.h"
#include "main.h"

ESCOOTER_Driving_State_t Driving_State;     /*Determines the current E-Scooter's operation status*/
ESCOOTER_BrakeANDThrottleInput_t modeControl; /*Motor Physical Limitations*/
ESCOOTER_Physical_State_t motorStatus; /*Current Motor Physical Status*/
Cruise_Control_t *cruiseHandle;

void ESCOOTER_Safety_Limit_Setting()
{
	Cruise_Control_Setting(&CruiseControl);
}

void ESCOOTER_saveStatus (uint8_t state)
{
	  Driving_State = state;
}

ESCOOTER_Driving_State_t ESCOOTER_getStatus()
{
	return Driving_State;
}

void ESCOOTER_Set_Limit(ESCOOTER_BrakeANDThrottleInput_t *limitHandle)
{
     modeControl = *limitHandle;
}

void ESCOOTER_Set_PhysicalParam(ESCOOTER_Physical_State_t *motorParam)
{
	 motorStatus = *motorParam;
}

int16_t MOTOR_SPEED = 0;
void ESCOOTER_Get_Speed()
{
	/*Convert Back to RPM since the rotor speed is expressed in tenths of Hz*/
	MOTOR_SPEED = (motorStatus.current_speed*_RPM)/SPEED_UNIT;
	/*RPM -> km/h*/
	//MOTOR_SPEED = 22*MOTOR_SPEED*0.001885;
}

float PHASE_CURRENT = 0;
void ESCOOTER_Get_PhaseCurrent()
{
   /*Convert back to Amps from s16A*/
	PHASE_CURRENT = ((motorStatus.phase_current)*3.3) / (65536*20*0.002);
	PHASE_CURRENT = PHASE_CURRENT/1.4142;
}

float PHASE_VOLTAGE = 0;
void ESCOOTER_Get_PhaseVoltage()
{
   /*Convert back to Volt from s16V*/
	PHASE_VOLTAGE = ((motorStatus.phase_voltage)/(1.7321*32767))*36;
	PHASE_VOLTAGE = PHASE_VOLTAGE/1.4142;
}

int16_t throttle_Current = 0;
int16_t speedLimit = 0;
int16_t AMPMAX = 0;
uint16_t acceleration = 0;

uint8_t go = 0;
bool run = false;

#undef POWER_SUPPLY
bool connect_failed = false;
bool motor_failed = false;
void ESCOOTER_Driving_Start()
{
    throttle_Current = modeControl.TARGET_IQ;
    speedLimit = modeControl.SPEED_LIMIT;
    AMPMAX = modeControl.IQ_LIMIT;
    acceleration = modeControl.RAMP_DURATION;
    /*Acceleration Mode*/
    if(connect_failed == false && motor_failed == false){
    	if(throttle_Current != 0) /*Try to modify when Speed == 0 or Speed <= 3 km/h then shutdown*/
    	{
            MC_ProgramTorqueRampMotor1(throttle_Current,0);
            run = MC_StartMotor1();
    	}

    	if(MOTOR_SPEED <= 10 && throttle_Current == 0)
    	{
    		MC_ProgramTorqueRampMotor1(throttle_Current,0);
    		run = MC_StartMotor1();
    		MC_StopMotor1();
    	}
        /*WHEN THE SYSTEM IS RUNNING WITH POWER SUPPLY ONLY WITHOUT DEBUGGING MODE. THE FOLLOWING ERROR OCCURS.
         *TO RUN THE MOTOR, THE ERROR MSG IS REMOVED EVERY TIME THE WARNING HAPPENS
         *IT'S BETTER TO COMMENT OUT THOSE INSTRUCTIONS WHEN THE E-SCOOTER IS GET OFF THE GROUND, OTHERWISE IT'S
         *VERY FUCKING DANGEROUS.*/
#ifdef POWER_SUPPLY
        if( MC_GetOccurredFaultsMotor1() == 0x0004)
        {
        	//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
        	MC_AcknowledgeFaultMotor1();
        }
#endif

        /*System Does Error Handling (Just for System Debug)*/
        if (ESCOOTER_GetReportStatus() == true)
        {
        	/*JUMP OUT OF THIS LOOP*/
        	connect_failed = true;
        }

        if(motorStatus.previous_error != 0)
        {
            motor_failed = true;
        }
    	go = 1;
    }

    /*HANDLE ONE MORE CASE: --> TAKE STM32 ERROR CODE in case motor errors*/
    if(connect_failed == true) /*Emergency Stop the Motor in case heartbeat loses*/
    {
    	MC_ProgramTorqueRampMotor1(0,0);
    	MC_StartMotor1();
    	MC_StopMotor1();
    	ESCOOTER_saveStatus(3);
    	ESCOOTER_UpdateDrivingState(3);
    	//Send Error Codes
    	//ERROR_HANDLE_MSG(CONNECTION_FAIL);
    }

    if(motor_failed == true)
    {
    	ESCOOTER_saveStatus(3);
    	ESCOOTER_UpdateDrivingState(3);
    	SET_ERROR_CODE(motorStatus.previous_error);
    }
}

uint8_t stop = 0;
void ESCOOTER_Driving_Stop()
{
	stop = 1;
}

void MOTOR_BRAKE()
{
   MC_ProgramTorqueRampMotor1(0,0);
   MC_StartMotor1();
   MC_StopMotor1();
}
