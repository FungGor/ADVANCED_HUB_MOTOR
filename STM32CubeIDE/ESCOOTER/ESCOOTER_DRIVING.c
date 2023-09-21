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
#include "ESCOOTER_MainTask.h"
#include "mc_type.h"
#include "cmsis_os.h"
#include "mc_api.h"
#include "main.h"

ESCOOTER_Driving_State_t Driving_State;     /*Determines the current E-Scooter's operation status*/
ESCOOTER_BrakeANDThrottleInput_t modeControl; /*Motor Physical Limitations*/
ESCOOTER_Physical_State_t motorStatus; /*Current Motor Physical Status*/

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
}

float PHASE_VOLTAGE = 0;
void ESCOOTER_Get_PhaseVoltage()
{
   /*Convert back to Volt from s16V*/
	PHASE_VOLTAGE = ((motorStatus.phase_voltage)/(1.7321*32767))*36;
	PHASE_VOLTAGE = PHASE_VOLTAGE/1.4142;
}

void ESCOOTER_Get_MotorState()
{

}

void Speed_Cruise_Control()
{

}

int16_t throttle_Current = 0;
int16_t speedLimit = 0;
int16_t AMPMAX = 0;
uint16_t acceleration = 0;
uint8_t Speed_Compare = 0;
int16_t prevSpeed = 0;
uint8_t decelerate = 0;
uint8_t go = 0;
bool run = false;
bool ESCOOTER_Detect_Decelerate()
{
    if(Speed_Compare == 0)
    {
        prevSpeed = throttle_Current;
        Speed_Compare = 1;
    }
    else if (Speed_Compare == 1)
    {
    	if (throttle_Current < prevSpeed)
    	{
            decelerate = 1;
    	}
    	else if(throttle_Current > prevSpeed)
    	{
    		decelerate = 0;
    	}
    	Speed_Compare = 0;
    }
    return decelerate;
}

bool connect_failed = false;
void ESCOOTER_Driving_Start()
{
    throttle_Current = modeControl.TARGET_IQ;
    speedLimit = modeControl.SPEED_LIMIT;
    AMPMAX = modeControl.IQ_LIMIT;
    acceleration = modeControl.RAMP_DURATION;
    ESCOOTER_Detect_Decelerate();
    if(ESCOOTER_Detect_Decelerate() == 1 && connect_failed == false)
    {
    	//qd_t targetValue;
    	//targetValue.q = throttle_Current;
    	//targetValue.d = 0;
    	//MC_SetCurrentReferenceMotor1(targetValue);
    	//MC_StartMotor1();
    	//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
        MC_ProgramTorqueRampMotor1(throttle_Current,0);
        run = MC_StartMotor1();
        /*Make a debug console in case it doesn't work*/
        if(run == true || (MC_HasRampCompletedMotor1() == true) )
        {
        	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,GPIO_PIN_SET);
        }

        /*WHEN THE SYSTEM IS RUNNING WITH POWER SUPPLY ONLY WITHOUT DEBUGGING MODE. THE FOLLOWING ERROR OCCURS.
         *TO RUN THE MOTOR, THE ERROR MSG IS REMOVED EVERY TIME THE WARNING HAPPENS
         *IT'S BETTER TO COMMENT OUT THOSE INSTRUCTIONS WHEN THE E-SCOOTER IS GET OFF THE GROUND, OTHERWISE IT'S
         *VERY FUCKING DANGEROUS.*/
        if(MC_GetOccurredFaultsMotor1() == 0x0004)
        {
        	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
        	MC_AcknowledgeFaultMotor1();
        }

        /*System Does Error Handling (Just for System Debug)*/
        if (ESCOOTER_GetReportStatus() == true)
        {
        	connect_failed = true;
        }

    	go = 0;
    }
    else if(ESCOOTER_Detect_Decelerate() == 0 && connect_failed == false){
    	//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
        MC_ProgramTorqueRampMotor1(throttle_Current,acceleration);
        run = MC_StartMotor1();
        /*Make a debug console in case it doesn't work*/
        if(run == true || (MC_HasRampCompletedMotor1() == true) )
        {
        	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,GPIO_PIN_SET);
        }
        /*WHEN THE SYSTEM IS RUNNING WITH POWER SUPPLY ONLY WITHOUT DEBUGGING MODE. THE FOLLOWING ERROR OCCURS.
         *TO RUN THE MOTOR, THE ERROR MSG IS REMOVED EVERY TIME THE WARNING HAPPENS
         *IT'S BETTER TO COMMENT OUT THOSE INSTRUCTIONS WHEN THE E-SCOOTER IS GET OFF THE GROUND, OTHERWISE IT'S
         *VERY FUCKING DANGEROUS.*/
        if( MC_GetOccurredFaultsMotor1() == 0x0004)
        {
        	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
        	MC_AcknowledgeFaultMotor1();
        }

        /*System Does Error Handling (Just for System Debug)*/
        if (ESCOOTER_GetReportStatus() == true)
        {
        	/*JUMP OUT OF THIS LOOP*/
        	connect_failed = true;
        }
    	go = 1;
    }

    if(connect_failed == true) /*Emergency Stop the Motor in case heartbeat loses*/
    {
    	MC_ProgramTorqueRampMotor1(0,0);
    	MC_StartMotor1();
    }
}

uint8_t stop = 0;
void ESCOOTER_Driving_Stop()
{
	stop = 1;
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5,GPIO_PIN_RESET);
	/*Stop Motor!*/
	//MC_StopMotor1();
}

uint8_t fuckup = 0;
void MOTOR_BRAKE()
{
   fuckup = 1;
   MC_StopMotor1();
}

/*This thread might be useful (?) This thread will be deleted (?)*/
void ESCOOTER_DrivingTaskControl(void const * argument)
{
     for(;;)
     {

           switch(Driving_State)
           {
               case DRIVING_IDLE:
            	   ESCOOTER_Driving_Stop();
            	   break;

               case DRIVING_START:
            	   ESCOOTER_Driving_Start();
            	   break;

               case DRIVING_STOP:
            	   ESCOOTER_Driving_Stop();
            	   break;

               default:
            	   break;

           }
     }
}
