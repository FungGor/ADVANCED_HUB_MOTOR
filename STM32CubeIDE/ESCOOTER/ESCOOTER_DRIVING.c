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
void ESCOOTER_Driving_Start()
{
	//retransmissionTimerStart();
	//MC_ProgramTorqueRampMotor1(modeControl.TARGET_IQ,modeControl.RAMP_DURATION);
	//MC_StartMotor1();
    throttle_Current = modeControl.TARGET_IQ;
    speedLimit = modeControl.SPEED_LIMIT;
    AMPMAX = modeControl.IQ_LIMIT;
    acceleration = modeControl.RAMP_DURATION;
    MC_ProgramTorqueRampMotor1(throttle_Current,acceleration);
    MC_StartMotor1();
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
